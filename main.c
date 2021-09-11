#include "src/CRUD.h"

int main() {
//    Customer* customer = newCustomer("Yehor", "Bukh", "999 address");
//    CustomerMetaEntity* customerMeta = newCustomerMetaEntity(1, *customer);

//    CustomerMetaEntity customerMeta1;
//    printf("m: %d\n", readM(&customerMeta1, 1, NULL));

//    updateM(customerMeta->pk, customerMeta);

//    CustomerMetaEntity customerMeta2;
//    printf("m: %d\n", readM(&customerMeta2, 1, NULL));

//    createM(customerMeta);
//
    Review* review = newReview("Updated review", "11-09-2021");
    ReviewMetaEntity* reviewMetaEntity = newReviewMetaEntity(1, *review);

    ReviewMetaEntity reviewMetaEntity1;
    printf("m: %d\n", readS(&reviewMetaEntity1, 1, NULL));

    updateS(1, reviewMetaEntity);

    printf("m: %d\n", readS(&reviewMetaEntity1, 1, NULL));
//
//    int r = createS(reviewMetaEntity, 3);
//    printf("%d\n", r);


//    CustomerMetaEntity customerMeta;
//    ReviewMetaEntity reviewMetaEntity1;
//
//    printf("m: %d\n", readM(&customerMeta, 1, NULL));
//    printf("s: %d\n", readS(&reviewMetaEntity1, 0, NULL));

//    sortIndexTable();

//    int value = findFileIndexOfPK(-1);
//    printf("%d", value);

//    CustomerMetaEntity read;
//    IndexEntity indexEntityRead;
//
//    FILE* file;
//    file = fopen(indexTablePath, "rb");
//
//    fread(&indexEntityRead, sizeof (IndexEntity), 1, file);
//    int a = 5;

    return 0;
}
