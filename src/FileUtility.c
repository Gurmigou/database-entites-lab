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

void readAll(FILE* file, const void** result, int elementSize, int fileByteLength) {
    int elements = fileByteLength / elementSize;
    fread(result, elementSize, fileByteLength / elementSize, file);
}

int indexEntityComparator(const void* e1, const void* e2) {
    IndexEntity* elem1 = (IndexEntity*) e1;
    IndexEntity* elem2 = (IndexEntity*) e2;
    return (elem1->key < elem2->key) ? -1 : (elem1->key > elem2->key) ? 1 : 0;
}