// Created by Yehor on 08.09.2021.
#ifndef DATABASELAB_CUSTOMERMETAENTITY_H
#define DATABASELAB_CUSTOMERMETAENTITY_H

#include "Customer.h"
#include "stdbool.h"

typedef struct CustomerMetaEntity {
    Customer customer;
    bool deleted;
    int nextReviewIndex;
    int numOfReviews;
} CustomerMetaEntity;

struct CustomerMetaEntity* newCustomerMetaEntity(Customer customer) {
    struct CustomerMetaEntity* customerMetaEntity = malloc(sizeof(CustomerMetaEntity));
    customerMetaEntity->customer = customer;
    customerMetaEntity->deleted = false;
    customerMetaEntity->nextReviewIndex = 0;
    customerMetaEntity->numOfReviews = 0;
    return customerMetaEntity;
}

#endif //DATABASELAB_CUSTOMERMETAENTITY_H
