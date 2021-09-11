// Created by Yehor on 08.09.2021.
#include "FileUtility.h"

bool isOpen(FILE* file) {
    return file != NULL;
}

bool fileIsEmpty(FILE* file) {
    return getFileByteLength(file) == 0;
}

int getFileByteLength(FILE* file) {
    long curPos = ftell(file);
    fseek(file, 0L, SEEK_END);
    long length = ftell(file);
    fseek(file, curPos, SEEK_SET);
    return length;
}

int indexEntityComparator(const void* e1, const void* e2) {
    IndexEntity* elem1 = (IndexEntity*) e1;
    IndexEntity* elem2 = (IndexEntity*) e2;
    return (elem1->key < elem2->key) ? -1 : (elem1->key > elem2->key) ? 1 : 0;
}

void sortIndexTable() {
    FILE* file;
    file = fopen(indexTablePath, "ab+");

    int numberOfElements = getFileByteLength(file) / (int) sizeof(IndexEntity);
    IndexEntity* indexEntityArray = malloc(numberOfElements * sizeof(IndexEntity));

    // read elements from file
    fread(indexEntityArray, sizeof(IndexEntity), numberOfElements, file);

    for (int i = 0; i < numberOfElements; ++i) {
        IndexEntity entity = indexEntityArray[i];
        int a = 5;
    }

    // sort elements
    qsort(indexEntityArray, numberOfElements,
          sizeof(IndexEntity), indexEntityComparator);

    fclose(file);

    // delete previous file
    remove(indexTablePath);

    // create new file and write the result
    file = fopen(indexTablePath, "wb");

    fwrite(indexEntityArray, sizeof(IndexEntity), numberOfElements, file);
    fclose(file);
}

// Uses binary search
int findFileIndexByPK(int pk) {
    FILE* file;
    file = fopen(indexTablePath, "ab+");

    int left = 0;
    int right = getFileByteLength(file) / (int) sizeof(IndexEntity);

    while (left <= right) {
        int mid = (left + right) / 2;

        fseek(file, mid * (int) sizeof(IndexEntity), SEEK_SET);

        IndexEntity indexEntity;
        fread(&indexEntity, sizeof(IndexEntity), 1, file);

        if (indexEntity.key == pk) {
            fclose(file);
            return indexEntity.addressIndex;
        }

        if (indexEntity.key < pk)
            left = mid + 1;
        else
            right = mid - 1;
    }

    fclose(file);
    return -1;
}

ReviewMetaEntity findLastReviewInChain(CustomerMetaEntity* firstIndex) {
    int next = firstIndex->nextReviewIndex * (int) sizeof(ReviewMetaEntity);

    FILE* file;
    file = fopen(reviewPath, "rb");

    ReviewMetaEntity reviewMetaEntity;
    while (next >= 0) {
        fread(&reviewMetaEntity, sizeof(ReviewMetaEntity), 1, file);
        next = reviewMetaEntity.nextReviewIndex * (int) sizeof(ReviewMetaEntity);
    }

    fclose(file);
    return reviewMetaEntity;
}

bool chainSearchByPK(CustomerMetaEntity* firstIndex, int pk, ReviewMetaEntity* dest) {
    int next = firstIndex->nextReviewIndex * (int) sizeof(ReviewMetaEntity);

    FILE* file;
    file = fopen(reviewPath, "rb");

    ReviewMetaEntity reviewMetaEntity;
    while (next >= 0) {
        fread(&reviewMetaEntity, sizeof(ReviewMetaEntity), 1, file);

        if (reviewMetaEntity.pk == pk) {
            dest->review = reviewMetaEntity.review;
            dest->pk = reviewMetaEntity.pk;
            dest->nextReviewIndex = reviewMetaEntity.nextReviewIndex;

            fclose(file);
            return true;
        }

        next = reviewMetaEntity.nextReviewIndex * (int) sizeof(ReviewMetaEntity);
    }

    fclose(file);
    return false;
}