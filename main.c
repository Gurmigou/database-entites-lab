#include "src/CRUD.h"

int main() {
    Customer* customer = newCustomer("Yehor", "Bukh", "1 address");
    CustomerMetaEntity* customerMeta = newCustomerMetaEntity(1, *customer);

//    createM(customerMeta);

    Review* review = newReview("First review", "11-09-2021");
    ReviewMetaEntity* reviewMetaEntity = newReviewMetaEntity(1, *review);

    createS(reviewMetaEntity, customerMeta->pk);

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
