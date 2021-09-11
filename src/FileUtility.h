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

void sortIndexTable();

// This function uses binary search to find index.
// This function implies that the "index-table.bin" file is sorted
int findFileIndexByPK(int pk);
int indexEntityComparator(const void* e1, const void* e2);

ReviewMetaEntity findLastReviewInChain(CustomerMetaEntity* firstIndex);
bool chainSearchByPK(CustomerMetaEntity* firstIndex, int pk, ReviewMetaEntity* dest);

#endif //DATABASELAB_FILEUTILITY_H
