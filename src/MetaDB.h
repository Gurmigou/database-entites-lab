// Created by Yehor on 11.09.2021.

#ifndef DATABASELAB_METADB_H
#define DATABASELAB_METADB_H

#include <stdio.h>
#include <malloc.h>

extern const char metaPath[];

void incrementNumDeleted();
void setNumDeleted(int numDeleted);
int getNumDeleted();

#endif //DATABASELAB_METADB_H
