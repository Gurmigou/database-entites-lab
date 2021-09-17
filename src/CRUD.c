// Created by Yehor on 08.09.2021

#include "CRUD.h"

const char customerPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\customer.bin";
const char reviewPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\review.bin";
const char indexTablePath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\index-table.bin";

bool createM(CustomerMetaEntity* data) {
    CustomerMetaEntity tmp;
    // if the record already exists, update it
    if (readM(&tmp, data->pk, NULL) && !tmp.deleted)
        return updateM(data->pk, data);

    FILE* file = fopen(customerPath, "ab+");

    int dataIndex = getNextDataIndex(file, sizeof(CustomerMetaEntity));

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

bool createS(ReviewMetaEntity* data) {
    ReviewMetaEntity tmp;
    // if the record already exists, update it
    if (readS(&tmp, data->pk, NULL) && !tmp.deleted)
        return updateS(data->pk, data);

    CustomerMetaEntity customerMetaEntity;
    int customerIndex;
    bool successfullyRead = readM(&customerMetaEntity, data->fk, &customerIndex);

    if (!successfullyRead)
        return false;

    FILE* file = fopen(reviewPath, "ab+");
    int dataIndex = getNextDataIndex(file, sizeof(ReviewMetaEntity));

    // means that there is no review records in the table
    if (customerMetaEntity.nextReviewIndex == -1)
        customerMetaEntity.nextReviewIndex = dataIndex;
    else {
        int lastReviewIndex;
        ReviewMetaEntity lastReview = findLastReviewInChain(&customerMetaEntity, &lastReviewIndex);
        lastReview.nextReviewIndex = dataIndex;

        FILE* reviewFile = fopen(reviewPath, "r+b");
        writeToPosition(&lastReview, lastReviewIndex, sizeof(ReviewMetaEntity), reviewFile);
        fclose(reviewFile);
    }

    FILE* customerFile = fopen(customerPath, "r+b");

    customerMetaEntity.numOfReviews++;
    writeToPosition(&customerMetaEntity, customerIndex, sizeof(CustomerMetaEntity), customerFile);

    fclose(customerFile);

    fwrite(data, sizeof(ReviewMetaEntity), 1, file);

    fclose(file);
    return true;
}

bool readM(CustomerMetaEntity* dest, int pk, int* index) {
    // sort elements in the index table
    sortIndexTable();
    // binary search
    int recordIndex = findFileIndexByPK(pk);

    if (recordIndex == -1)
        return false;

    CustomerMetaEntity customerMetaEntity;
    FILE* file = fopen(customerPath, "rb");

    readOnPosition(&customerMetaEntity, recordIndex, sizeof(CustomerMetaEntity), file);
    fclose(file);

    if (index != NULL)
        *index = recordIndex;

    dest->pk = customerMetaEntity.pk;
    dest->deleted = customerMetaEntity.deleted;
    dest->nextReviewIndex = customerMetaEntity.nextReviewIndex;
    dest->customer = customerMetaEntity.customer;
    dest->numOfReviews = customerMetaEntity.numOfReviews;

    return true;
}

bool readS(ReviewMetaEntity* dest, int pk, int* index) {
    FILE* file = fopen(reviewPath, "rb");
    int fileLength = getNextDataIndex(file, sizeof(ReviewMetaEntity));

    ReviewMetaEntity reviewMetaEntity;
    int curIndex = 0;

    while (curIndex < fileLength) {
        readOnPosition(&reviewMetaEntity, curIndex, sizeof(ReviewMetaEntity), file);

        if (reviewMetaEntity.pk == pk && !reviewMetaEntity.deleted)
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

    copyReviewData(&reviewMetaEntity, dest);

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
    file = fopen(customerPath, "r+b");

    writeToPosition(updateData, dataIndex, sizeof(CustomerMetaEntity), file);

    fclose(file);
    return true;
}

bool updateS(int pk, ReviewMetaEntity* updateData) {
    ReviewMetaEntity reviewMetaEntity;
    int dataIndex;
    bool successfullyRead = readS(&reviewMetaEntity, pk, &dataIndex);

    if (!successfullyRead || reviewMetaEntity.pk != updateData->pk) {
        return false;
    }

    updateData->nextReviewIndex = reviewMetaEntity.nextReviewIndex;

    FILE* file;
    file = fopen(reviewPath, "r+b");

    writeToPosition(updateData, dataIndex, sizeof(ReviewMetaEntity), file);

    fclose(file);
    return true;
}

bool deleteM(int pk) {
    CustomerMetaEntity customerMetaEntity;
    int dataIndex;
    bool successfullyRead = readM(&customerMetaEntity, pk, &dataIndex);

    if (!successfullyRead)
        return false;

    deleteAllReviewsChain(&customerMetaEntity);

    FILE* file;
    file = fopen(customerPath, "r+b");

    customerMetaEntity.deleted = true;
    writeToPosition(&customerMetaEntity, dataIndex, sizeof(CustomerMetaEntity), file);

    // meta information
    incrementNumDeleted();

    fclose(file);
    return true;
}

bool deleteS(int pk) {
    ReviewMetaEntity reviewMetaEntity;
    bool successfullyRead = readS(&reviewMetaEntity, pk, NULL);

    if (!successfullyRead)
        return false;

    CustomerMetaEntity customerMetaEntity;
    int customerIndex;
    readM(&customerMetaEntity, reviewMetaEntity.fk, &customerIndex);

    bool successfullyDeleted = deleteReview(&customerMetaEntity, reviewMetaEntity.pk);

    if (!successfullyDeleted)
        return false;

    FILE* file = fopen(customerPath, "r+b");
    customerMetaEntity.numOfReviews--;
    writeToPosition(&customerMetaEntity, customerIndex, sizeof(CustomerMetaEntity), file);
    fclose(file);

    // meta information
    incrementNumDeleted();
    incrementNumDeleted();

    int v = getNumDeleted();

    setNumDeleted(99);

    int v1 = getNumDeleted();

    return true;
}