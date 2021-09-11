// Created by Yehor on 11.09.2021

#include "MetaDB.h"

Meta* read() {
    FILE* file;
    file = fopen(metaPath, "rb");

    struct Meta* meta = malloc(sizeof(Meta));
    fread(&meta, sizeof(Meta), 1, file);

    fclose(file);

    return meta;
}

void write(Meta* meta) {
    FILE* file;
    file = fopen(metaPath, "w+b");

    fseek(file, 0, SEEK_SET);
    fwrite(&meta, sizeof(Meta), 1, file);
    fclose(file);
}


void incrementLastIndex() {
    struct Meta* meta = read();
    meta->lastIndex++;
    write(meta);
}

void setLastIndex(int lastIndex) {
    struct Meta* meta = read();
    meta->lastIndex = lastIndex;
    write(meta);
}

void incrementRealSize() {
    struct Meta* meta = read();
    meta->realSize++;
    write(meta);
}

void setRealSize(int realSize) {
    struct Meta* meta = read();
    meta->realSize = realSize;
    write(meta);
}

void incrementNumDeleted() {
    struct Meta* meta = read();
    meta->numDeleted++;
    write(meta);
}

void setNumDeleted(int numDeleted) {
    struct Meta* meta = read();
    meta->numDeleted = numDeleted;
    write(meta);
}