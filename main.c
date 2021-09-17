#include "src/CRUD.h"

int main() {
    runDataBase();


    char command[20] = "";
    while (1){
        printf("\nEnter the command or 'q':");
        scanf("%s", command);

        if (strcmp(command, "q") == 0) break;

        else if (strcmp(command, "get-m") == 0)
            getM();
        else if (strcmp(command, "get-s") == 0)
            getS();

        else if (strcmp(command, "insert-m") == 0)
            insertM();
        else if (strcmp(command, "insert-s") == 0)
            insertS();

        else if (strcmp(command, "update-m") == 0)
            updateM();
        else if (strcmp(command, "update-s") == 0)
            updateS();

        else if (strcmp(command, "delete-m") == 0)
            deleteM();
        else if (strcmp(command, "delete-s") == 0)
            deleteS();

        else printf("Unknown command !\n");
    }

    return 0;
}
