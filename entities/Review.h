// Created by Yehor on 08.09.2021

#ifndef DATABASELAB_REVIEW_H
#define DATABASELAB_REVIEW_H

#include <malloc.h>

typedef struct Review {
    char text[512];

    // dd-mm-yyyy -> 10 bytes + '\0' = 11
    char createdData[11];
    int customerId;
} Review;

struct Review* newReview(char text[512], char createdData[11], int customerId) {
    struct Review* review = malloc(sizeof(Review));
    strcpy(review->text, text);
    strcpy(review->createdData, createdData);
    review->customerId = customerId;
    return review;
}

#endif //DATABASELAB_REVIEW_H
