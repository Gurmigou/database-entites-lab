#include <stdio.h>
#include "entities/CustomerMetaEntity.h"
#include "entities/ReviewMetaEntity.h"

#include "src/FileUtility.h"

void foo(const void* a, int length, int sizeEl) {
    int* b = malloc(sizeof(int));
    *b = 5;
    (*(int*)a) = *b;
}

int main() {

    FILE* file;
    file = fopen("C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\test.bin", "rb");

    int length = getFileByteLength(file);

    printf("%d\n", sizeof(CustomerMetaEntity));
    printf("%d\n", length);

    printf("%d", fileIsEmpty(file) == true);

    fread


    int value = 0;
    foo(&value, 0, 0);
    printf("%d", value);

    return 0;
}
