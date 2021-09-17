// Created by Yehor on 08.09.2021

#ifndef DATABASELAB_FILEUTILITY_H
#define DATABASELAB_FILEUTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "CRUD.h"
#include "../entities/IndexEntity.h"
#include "../entities/CustomerMetaEntity.h"
#include "../entities/ReviewMetaEntity.h"

bool isOpen(FILE* file);
bool fileIsEmpty(FILE* file);
int getFileByteLength(FILE* file);
int getNextDataIndex(FILE* file, int sizeofData);

void sortIndexTable();
void sortIndexTableRam(IndexEntity* arr, int length);

// This function uses binary search to find index.
// This function implies that the "index-table.bin" file is sorted
int findFileIndexByPK(int pk);
int indexEntityComparator(const void* e1, const void* e2);

void writeToPosition(void* data, int pos, int dataSize, FILE* file);
void readOnPosition(void* dest, int pos, int dataSize, FILE* file);

void copyReviewData(ReviewMetaEntity* source, ReviewMetaEntity* dest);

ReviewMetaEntity findLastReviewInChain(CustomerMetaEntity* firstIndex, int* index);
bool deleteAllReviewsChain(CustomerMetaEntity* firstIndex);
bool deleteReview(CustomerMetaEntity* firstIndex, int pkDelete);

#endif //DATABASELAB_FILEUTILITY_H
