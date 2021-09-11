// Created by Yehor on 11.09.2021.

#include "Review.h"

Review* newReview(char text[512], char createdData[11], int customerId) {
    struct Review* review = malloc(sizeof(Review));
    strcpy(review->text, text);
    strcpy(review->createdData, createdData);
    review->customerId = customerId;
    return review;
}