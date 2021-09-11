// Created by Yehor on 08.09.2021.

#ifndef DATABASELAB_CUSTOMER_H
#define DATABASELAB_CUSTOMER_H


#include <malloc.h>
#include <string.h>

typedef struct Customer {
    char firstName[64];
    char lastName[64];
    char address[128];
} Customer;

struct Customer* newCustomer(const char firstName[64], char lastName[64], char address[128]) {
    struct Customer* customer = malloc(sizeof(Customer));
    strcpy(customer->firstName, firstName);
    strcpy(customer->lastName, lastName);
    strcpy(customer->address, address);
    return customer;
};

#endif //DATABASELAB_CUSTOMER_H
