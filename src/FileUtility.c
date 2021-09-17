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

int getNextDataIndex(FILE* file, int sizeofData) {
    return getFileByteLength(file) / sizeofData;
}

int indexEntityComparator(const void* e1, const void* e2) {
    IndexEntity* elem1 = (IndexEntity*) e1;
    IndexEntity* elem2 = (IndexEntity*) e2;
    return (elem1->key < elem2->key) ? -1 : (elem1->key > elem2->key) ? 1 : 0;
}

void writeToPosition(void* data, int pos, int dataSize, FILE* file) {
    fseek(file, pos * dataSize, SEEK_SET);
    fwrite(data, dataSize, 1, file);
}

void readOnPosition(void* dest, int pos, int dataSize, FILE* file) {
    fseek(file, pos * dataSize, SEEK_SET);
    fread(dest, dataSize, 1, file);
}

void sortIndexTableRam(IndexEntity* arr, int length) {
    qsort(arr, length,sizeof(IndexEntity), indexEntityComparator);
}

void sortIndexTable() {
    FILE* file;
    file = fopen(indexTablePath, "ab+");

    int numberOfElements = getFileByteLength(file) / (int) sizeof(IndexEntity);
    IndexEntity* indexEntityArray = malloc(numberOfElements * sizeof(IndexEntity));

    // read elements from file
    fread(indexEntityArray, sizeof(IndexEntity), numberOfElements, file);

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
    if (pk < 0)
        return - 1;

    FILE* file = fopen(indexTablePath, "ab+");

    int left = 0;
    int right = getFileByteLength(file) / (int) sizeof(IndexEntity);

    while (left <= right) {
        int mid = (left + right) / 2;

        IndexEntity indexEntity;
        readOnPosition(&indexEntity, mid, sizeof(IndexEntity), file);

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

ReviewMetaEntity findLastReviewInChain(CustomerMetaEntity* firstIndex, int* index) {
    FILE* file = fopen(reviewPath, "rb");
    int next = firstIndex->nextReviewIndex;
    int prev = -1;

    ReviewMetaEntity reviewMetaEntity;
    while (next >= 0) {
        readOnPosition(&reviewMetaEntity, next, sizeof(ReviewMetaEntity), file);
        prev = next;
        next = reviewMetaEntity.nextReviewIndex;
    }

    if (index != NULL)
        *index = prev;

    fclose(file);
    return reviewMetaEntity;
}

bool deleteAllReviewsChain(CustomerMetaEntity* firstIndex) {
    int next = firstIndex->nextReviewIndex;

    FILE* file;
    file = fopen(reviewPath, "r+b");

    ReviewMetaEntity reviewMetaEntity;
    while (next >= 0) {
        readOnPosition(&reviewMetaEntity, next, sizeof(ReviewMetaEntity), file);
        reviewMetaEntity.deleted = true;

        writeToPosition(&reviewMetaEntity, next, sizeof(ReviewMetaEntity), file);
        next = reviewMetaEntity.nextReviewIndex;
    }

    fclose(file);
    return false;
}

void copyReviewData(ReviewMetaEntity* source, ReviewMetaEntity* dest) {
    dest->nextReviewIndex = source->nextReviewIndex;
    dest->deleted = source->deleted;
    dest->pk = source->pk;
    dest->fk = source->fk;
    dest->review = source->review;
}

bool deleteReview(CustomerMetaEntity* firstIndex, int pkDelete)
{
    int next = firstIndex->nextReviewIndex;

    // record
    if (next < 0) return false;

    FILE* file = fopen(reviewPath, "r+b");

    ReviewMetaEntity prevEntity;
    ReviewMetaEntity curEntity;

    bool first = true;
    int prev = -1;

    while (next >= 0) {
        readOnPosition(&curEntity, next, sizeof(ReviewMetaEntity), file);

        if (curEntity.pk == pkDelete) {
            // next after current entity
            int nextNext = curEntity.nextReviewIndex;

            if (first) {
                if (nextNext == -1)
                    firstIndex->nextReviewIndex = -1;
                else
                    firstIndex->nextReviewIndex = nextNext;
            } else {
                if (nextNext == -1)
                    prevEntity.nextReviewIndex = -1;
                else
                    prevEntity.nextReviewIndex = nextNext;
                writeToPosition(&prevEntity, prev, sizeof(ReviewMetaEntity), file);
            }
            curEntity.deleted = true;
            writeToPosition(&curEntity, next, sizeof(ReviewMetaEntity), file);
            break;
        }

        copyReviewData(&curEntity, &prevEntity);
        first = false;
        prev = next;
        next = curEntity.nextReviewIndex;
    }

    fclose(file);
    return next >= 0;
}