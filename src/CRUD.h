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
extern const char mathTablePath[];

void createM(struct CustomerMetaEntity* data);
void createS(struct ReviewMetaEntity* data, int fkValue);

void read(void* dest, int id, FILE* file);
void readAllWhere(void** destArr, bool (*predicate)(const void* e), FILE* file);

void update(int pkValue, void (*consumer)(const void* e));


#endif //DATABASELAB_CRUD_H
