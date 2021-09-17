// Created by Yehor on 11.09.2021

#include "MetaDB.h"

const char metaPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\meta.bin";

void incrementNumDeleted() {
    int deleted = getNumDeleted();
    setNumDeleted(deleted + 1);
}

void setNumDeleted(int numDeleted) {
    FILE* file = fopen(metaPath, "wb");
    fwrite(&numDeleted, sizeof(int), 1, file);
    fclose(file);
}

int getNumDeleted() {
    FILE* file = fopen(metaPath, "rb");
    int deleted;
    fread(&deleted, sizeof(int), 1, file);
    fclose(file);
    return deleted;
}