// Created by Yehor on 11.09.2021

#include "ReviewMetaEntity.h"

ReviewMetaEntity* newReviewMetaEntity(int pk, Review review) {
    struct ReviewMetaEntity* reviewMetaEntity = malloc(sizeof(ReviewMetaEntity));
    reviewMetaEntity->pk = pk;
    reviewMetaEntity->review = review;
    reviewMetaEntity->nextReviewIndex = 0;
    return reviewMetaEntity;
}
