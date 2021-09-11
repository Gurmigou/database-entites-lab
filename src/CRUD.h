// Created by Yehor on 08.09.2021.

#ifndef DATABASELAB_CRUD_H
#define DATABASELAB_CRUD_H

#include "stdio.h"

#include "FileUtility.h"
#include "../entities/CustomerMetaEntity.h"
#include "../entities/ReviewMetaEntity.h"
#include "../entities/IndexEntity.h"

extern const char customerPath[];
extern const char reviewPath[];
extern const char indexTablePath[];

bool createM(CustomerMetaEntity* data);
bool createS(ReviewMetaEntity* data, int fkValue);

bool readM(CustomerMetaEntity* dest, int pk, int* index);
bool readS(ReviewMetaEntity* dest, int pk, int* index);
//void readAllWhere(void** destArr, bool (*predicate)(const void* e), FILE* file);

bool updateM(int pk, CustomerMetaEntity* updateData);
bool updateS(int pk, ReviewMetaEntity* updateData);

#endif //DATABASELAB_CRUD_H
