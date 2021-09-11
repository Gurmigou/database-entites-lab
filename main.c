#include "src/CRUD.h"

int main() {
    struct Customer* customer = newCustomer("Yehor", "Bukh", "123 address");
    struct CustomerMetaEntity* customerMeta = newCustomerMetaEntity(1, *customer);

    createM(customerMeta);

    return 0;
}
