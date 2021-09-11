// Created by Yehor on 08.09.2021

#include "CRUD.h"

const char customerPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\customer.bin";
const char reviewPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\review.bin";
const char indexTablePath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\index-table.bin";

bool createM(struct CustomerMetaEntity* data) {
    FILE* file;
    file = fopen(customerPath, "ab+");

    int dataIndex = getFileByteLength(file) / (int) sizeof(CustomerMetaEntity);

    fwrite(data, sizeof(CustomerMetaEntity), 1, file);
    fclose(file);

    struct IndexEntity indexEntity;
    indexEntity.key = data->pk;
    indexEntity.addressIndex = dataIndex;

    file = fopen(indexTablePath, "ab+");
    fwrite(&indexEntity, sizeof (IndexEntity), 1, file);

    fclose(file);
    return true;
}

bool createS(struct ReviewMetaEntity* data, int fkValue) {
    CustomerMetaEntity customerMetaEntity;
    bool successfullyRead = readM(&customerMetaEntity, fkValue,NULL);

    if (!successfullyRead)
        return false;

    FILE* file;
    file = fopen(reviewPath, "ab+");

    int dataIndex = getFileByteLength(file) / (int) sizeof(ReviewMetaEntity);

    // means that there is no review records in the table
    if (customerMetaEntity.nextReviewIndex == -1) {
        customerMetaEntity.nextReviewIndex = dataIndex;
        updateM(customerMetaEntity.pk, &customerMetaEntity);
    }
    else {
        ReviewMetaEntity lastReview = findLastReviewInChain(&customerMetaEntity);
        lastReview.nextReviewIndex = dataIndex;
        updateS(lastReview.pk, &lastReview);
    }

    fwrite(data, sizeof(ReviewMetaEntity), 1, file);

    fclose(file);
    return true;
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

    fseek(file, fileIndex * (int) sizeof(CustomerMetaEntity), SEEK_SET);
    fread(dest, sizeof(CustomerMetaEntity), 1, file);
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
        fseek(file, curIndex * (int) sizeof(ReviewMetaEntity), SEEK_SET);
        fread(&reviewMetaEntity, sizeof(ReviewMetaEntity), 1, file);

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

bool updateM(int pk, CustomerMetaEntity* updateData) {
    CustomerMetaEntity customerMetaEntity;
    int dataIndex;
    bool successfullyRead = readM(&customerMetaEntity, pk, &dataIndex);

    if (!successfullyRead || customerMetaEntity.pk != updateData->pk)
        return false;

    FILE* file;
    file = fopen(customerPath, "w+b");

    fseek(file, dataIndex * (int) sizeof(CustomerMetaEntity), SEEK_SET);
    fwrite(updateData, sizeof(CustomerMetaEntity), 1, file);

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

    fseek(file, dataIndex * (int) sizeof(CustomerMetaEntity), SEEK_SET);
    fwrite(updateData, sizeof(ReviewMetaEntity), 1, file);

    fclose(file);
    return true;
}