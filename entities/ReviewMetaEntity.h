// Created by Yehor on 08.09.2021.

#ifndef DATABASELAB_REVIEWMETAENTITY_H
#define DATABASELAB_REVIEWMETAENTITY_H

#include "Review.h"

typedef struct ReviewMetaEntity {
    int pk;
    Review review;
    int nextReviewIndex;
} ReviewMetaEntity;

ReviewMetaEntity* newReviewMetaEntity(int pk, Review review);

#endif //DATABASELAB_REVIEWMETAENTITY_H
