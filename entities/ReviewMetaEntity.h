// Created by Yehor on 08.09.2021.

#ifndef DATABASELAB_REVIEWMETAENTITY_H
#define DATABASELAB_REVIEWMETAENTITY_H

#include "Review.h"

typedef struct ReviewMetaEntity {
    Review review;
    int nextReviewIndex;
} ReviewMetaEntity;

struct ReviewMetaEntity* newReviewMetaEntity(Review review) {
    struct ReviewMetaEntity* reviewMetaEntity = malloc(sizeof(ReviewMetaEntity));
    reviewMetaEntity->review = review;
    reviewMetaEntity->nextReviewIndex = 0;
    return reviewMetaEntity;
}

#endif //DATABASELAB_REVIEWMETAENTITY_H
