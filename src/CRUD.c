// Created by Yehor on 08.09.2021

#include "CRUD.h"

const char customerPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\customer.bin";
const char reviewPath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\review.bin";
const char mathTablePath[] = "C:\\Users\\Yehor\\CLionProjects\\databaseLab\\tables\\match-table.bin";

void createM(struct CustomerMetaEntity* data) {
    FILE* file;
    file = fopen(customerPath, "wb");

    int dataIndex = getFileByteLength(file) / (int) sizeof(CustomerMetaEntity);

    fwrite(data, sizeof(CustomerMetaEntity), 1, file);
    fclose(file);

    struct IndexEntity indexEntity;
    indexEntity.key = data->pk;
    indexEntity.addressIndex = dataIndex;

    file = fopen(mathTablePath, "wb");
    fwrite(&indexEntity, sizeof (IndexEntity), 1, file);

    fclose(file);
}


//void read(void* dest, int id, FILE* file) {
//
//}