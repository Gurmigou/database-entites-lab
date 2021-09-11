// Created by Yehor on 08.09.2021

#ifndef DATABASELAB_FILEUTILITY_H
#define DATABASELAB_FILEUTILITY_H

#include <stdlib.h>
#include "stdio.h"
#include "stdbool.h"

#include "../entities/IndexEntity.h"

bool isOpen(FILE* file);
bool fileIsEmpty(FILE* file);
int getFileByteLength(FILE* file);
void readAll(FILE* file, const void** result, int structSize, int fileByteLength);

int indexEntityComparator(const void* e1, const void* e2);

#endif //DATABASELAB_FILEUTILITY_H
