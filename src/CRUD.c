// Created by Yehor on 08.09.2021

#include "CRUD.h"

const char customerPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\customer.bin";
const char reviewPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\review.bin";
const char indexTablePath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\index-table.bin";

void writeToPosition(void* data, int pos, int dataSize, FILE* file);
void readOnPosition(void* dest, int pos, int dataSize, FILE* file);

bool createM(CustomerMetaEntity* data) {
    FILE* file;
    file = fopen(customerPath, "ab+");

    int dataIndex = getFileByteLength(file) / (int) sizeof(CustomerMetaEntity);

    fwrite(data, sizeof(CustomerMetaEntity), 1, file);
    fclose(file);

    IndexEntity indexEntity;
    indexEntity.key = data->pk;
    indexEntity.addressIndex = dataIndex;

    file = fopen(indexTablePath, "ab+");
    fwrite(&indexEntity, sizeof (IndexEntity), 1, file);

    fclose(file);
    return true;
}

bool createS(ReviewMetaEntity* data, int fkValue) {
    CustomerMetaEntity customerMetaEntity;
    int index;
    bool successfullyRead = readM(&customerMetaEntity, fkValue, &index);

    if (!successfullyRead)
        return false;

    FILE* file;
    file = fopen(reviewPath, "ab+");

    int dataIndex = getFileByteLength(file) / (int) sizeof(ReviewMetaEntity);

    // means that there is no review records in the table
    if (customerMetaEntity.nextReviewIndex == -1) {
        customerMetaEntity.nextReviewIndex = dataIndex;

        FILE* customerFile;
        customerFile = fopen(customerPath, "w+b");

        writeToPosition(&customerMetaEntity, index * (int) sizeof(CustomerMetaEntity),
                        sizeof(CustomerMetaEntity), customerFile);
        fclose(customerFile);
    }
    else {
        ReviewMetaEntity lastReview = findLastReviewInChain(&customerMetaEntity);
        lastReview.nextReviewIndex = dataIndex;

        FILE* reviewFile;
        reviewFile = fopen(customerPath, "w+b");

        writeToPosition(&lastReview, index * (int) sizeof(ReviewMetaEntity),
                        sizeof(CustomerMetaEntity), reviewFile);
        fclose(reviewFile);
    }

    fwrite(data, sizeof(ReviewMetaEntity), 1, file);

    fclose(file);
    return true;
}

void readOnPosition(void* dest, int pos, int dataSize, FILE* file) {
    fseek(file, pos * dataSize, SEEK_SET);
    fread(dest, dataSize, 1, file);
}

bool readM(CustomerMetaEntity* dest, int pk, int* index) {
    FILE* file;
    file = fopen(customerPath, "rb");

    // sort elements in the index table
    sortIndexTable();

    // find index using binary search
    int fileIndex = findFileIndexByPK(pk);

    if (fileIndex == -1)
        return false;

    if (index != NULL)
        *index = fileIndex;

//    fseek(file, fileIndex * (int) sizeof(CustomerMetaEntity), SEEK_SET);
//    fread(dest, sizeof(CustomerMetaEntity), 1, file);

    readOnPosition(dest, fileIndex * (int) sizeof(CustomerMetaEntity),sizeof(CustomerMetaEntity), file);
    fclose(file);
    return true;
}

bool readS(ReviewMetaEntity* dest, int pk, int* index) {
    FILE* file;
    file = fopen(reviewPath, "rb");
    int fileLength = getFileByteLength(file) / (int) sizeof(ReviewMetaEntity);

    ReviewMetaEntity reviewMetaEntity;
    int curIndex = 0;

    while (curIndex < fileLength) {
        readOnPosition(&reviewMetaEntity, curIndex * (int) sizeof(ReviewMetaEntity), sizeof(ReviewMetaEntity), file);
//        fseek(file, curIndex * (int) sizeof(ReviewMetaEntity), SEEK_SET);
//        fread(&reviewMetaEntity, sizeof(ReviewMetaEntity), 1, file);

        if (reviewMetaEntity.pk == pk)
            break;

        curIndex++;
    }

    // "Review meta entity" wasn't found
    if (curIndex == fileLength) {
        fclose(file);
        return false;
    }

    if (index != NULL)
        *index = curIndex;

    dest->review = reviewMetaEntity.review;
    dest->pk = reviewMetaEntity.pk;
    dest->nextReviewIndex = reviewMetaEntity.nextReviewIndex;

    fclose(file);
    return true;
}

void writeToPosition(void* data, int pos, int dataSize, FILE* file) {
    fseek(file, pos * dataSize, SEEK_SET);
    fwrite(data, dataSize, 1, file);
}

bool updateM(int pk, CustomerMetaEntity* updateData) {
    CustomerMetaEntity customerMetaEntity;
    int dataIndex;
    bool successfullyRead = readM(&customerMetaEntity, pk, &dataIndex);

    if (!successfullyRead || customerMetaEntity.pk != updateData->pk)
        return false;

    FILE* file;
    file = fopen(customerPath, "w+b");

    writeToPosition(updateData, dataIndex * (int) sizeof(CustomerMetaEntity),
                    sizeof(CustomerMetaEntity), file);

    fclose(file);
    return true;
}

bool updateS(int pk, ReviewMetaEntity* updateData) {
    ReviewMetaEntity reviewMetaEntity;
    int dataIndex;
    bool successfullyRead = readS(&reviewMetaEntity, pk, &dataIndex);

    if (!successfullyRead || reviewMetaEntity.pk != updateData->pk ||
        reviewMetaEntity.nextReviewIndex != updateData->nextReviewIndex) {
        return false;
    }

    FILE* file;
    file = fopen(reviewPath, "w+b");

    writeToPosition(updateData, dataIndex * (int) sizeof(CustomerMetaEntity),
                    sizeof(ReviewMetaEntity), file);

    fclose(file);
    return true;
}