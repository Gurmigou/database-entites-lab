// Created by Yehor on 11.09.2021.

#ifndef DATABASELAB_METADB_H
#define DATABASELAB_METADB_H

#include <stdio.h>
#include <malloc.h>

extern const char metaPath[];

typedef struct Meta {
    int lastIndex;
    int realSize;
    int numDeleted;
} Meta;

void incrementLastIndex();
void setLastIndex(int lastIndex);

void incrementRealSize();
void setRealSize(int realSize);

void incrementNumDeleted();
void setNumDeleted(int numDeleted);

Meta* read();
void write(Meta* meta);

#endif //DATABASELAB_METADB_H
