#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEATS 24

struct seat {
    int seatID;
    int assigned;
    char lastName[30];
    char firstName[30];
};

/* Make all 24 seats empty */
void initialize(struct seat flight[]) {
    for (int i = 0; i < SEATS; i++) {
        flight[i].seatID = i + 1;
        flight[i].assigned = 0;
        strcpy(flight[i].firstName, "");
        strcpy(flight[i].lastName, "");
    }
}

/* Show number of empty seats */
void showEmpty(struct seat flight[]) {
    int count = 0;

    for (int i = 0; i < SEATS; i++) {
        if (flight[i].assigned == 0) {
            count++;
        }
    }

    printf("There are %d empty seats.\n", count);
}

/* Show which seats are empty */
void listEmpty(struct seat flight[]) {
    printf("Empty seats: ");

    for (int i = 0; i < SEATS; i++) {
        if (flight[i].assigned == 0) {
            printf("%d ", flight[i].seatID);
        }
    }

    printf("\n");
}

/* Show assigned passengers */
void showPassengers(struct seat flight[]) {
    printf("\nPassenger List:\n");

    for (int i = 0; i < SEATS; i++) {
        if (flight[i].assigned == 1) {
            printf("Seat %d: %s %s\n",
                   flight[i].seatID,
                   flight[i].firstName,
                   flight[i].lastName);
        }
    }
}

/* Assign a passenger to a seat */
void assignSeat(struct seat flight[]) {
    int number;

    printf("Enter seat number (1-24), or 0 to cancel: ");

    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        return;
    }

    if (number == 0) {
        return;
    }

    if (number < 1 || number > SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (flight[number - 1].assigned == 1) {
        printf("That seat is already assigned.\n");
        return;
    }

    printf("Enter first name: ");

    if (scanf("%29s", flight[number - 1].firstName) != 1) {
        printf("Invalid first name.\n");
        return;
    }

    printf("Enter last name: ");

    if (scanf("%29s", flight[number - 1].lastName) != 1) {
        printf("Invalid last name.\n");
        return;
    }

    flight[number - 1].assigned = 1;

    printf("Seat %d assigned successfully.\n", number);
}

/* Delete a passenger */
void deleteSeat(struct seat flight[]) {
    int number;

    printf("Enter seat number to delete, or 0 to cancel: ");

    if (scanf("%d", &number) != 1) {
        printf("Invalid input.\n");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        return;
    }

    if (number == 0) {
        return;
    }

    if (number < 1 || number > SEATS) {
        printf("Invalid seat number.\n");
        return;
    }

    if (flight[number - 1].assigned == 0) {
        printf("That seat is already empty.\n");
        return;
    }

    flight[number - 1].assigned = 0;
    strcpy(flight[number - 1].firstName, "");
    strcpy(flight[number - 1].lastName, "");

    printf("Seat assignment deleted.\n");
}

/* Menu for each flight */
void flightMenu(struct seat flight[]) {
    char choice;

    while (1) {
        printf("\n--- Flight Menu ---\n");
        printf("a) Show number of empty seats\n");
        printf("b) Show list of empty seats\n");
        printf("c) Show passenger list\n");
        printf("d) Assign a customer\n");
        printf("e) Delete a seat assignment\n");
        printf("f) Return to Main Menu\n");
        printf("Choice: ");

        /* Stop safely if input runs out */
        if (scanf(" %c", &choice) != 1) {
            return;
        }

        switch (choice) {
            case 'a':
                showEmpty(flight);
                break;

            case 'b':
                listEmpty(flight);
                break;

            case 'c':
                showPassengers(flight);
                break;

            case 'd':
                assignSeat(flight);
                break;

            case 'e':
                deleteSeat(flight);
                break;

            case 'f':
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

/* Save both flights */
void saveData(struct seat outbound[], struct seat inbound[]) {
    FILE *file = fopen("flight_data.bin", "wb");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    if (fwrite(outbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error saving outbound flight.\n");
        fclose(file);
        return;
    }

    if (fwrite(inbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error saving inbound flight.\n");
        fclose(file);
        return;
    }

    fclose(file);

    printf("Flight data saved.\n");
}

/* Load saved flight information */
int loadData(struct seat outbound[], struct seat inbound[]) {
    FILE *file = fopen("flight_data.bin", "rb");

    if (file == NULL) {
        printf("No saved file found. Starting with empty seats.\n");
        return 0;
    }

    /* Check file size */
    if (fseek(file, 0, SEEK_END) != 0) {
        printf("Error checking file.\n");
        fclose(file);
        return 0;
    }

    long fileSize = ftell(file);

    if (fileSize == -1) {
        printf("Error checking file size.\n");
        fclose(file);
        return 0;
    }

    rewind(file);

    long expectedSize = sizeof(struct seat) * SEATS * 2;

    if (fileSize != expectedSize) {
        printf("Corrupted file detected. Starting with empty seats.\n");
        fclose(file);
        return 0;
    }

    /* Read outbound flight */
    if (fread(outbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error reading outbound data.\n");
        fclose(file);
        return 0;
    }

    /* Read inbound flight */
    if (fread(inbound, sizeof(struct seat), SEATS, file) != SEATS) {
        printf("Error reading inbound data.\n");
        fclose(file);
        return 0;
    }

    fclose(file);

    /* Validate seat numbers and assigned values */
    for (int i = 0; i < SEATS; i++) {

        if (outbound[i].seatID != i + 1 ||
            inbound[i].seatID != i + 1) {

            printf("Invalid seat number detected in file.\n");
            printf("Starting with empty seats.\n");
            return 0;
        }

        if ((outbound[i].assigned != 0 &&
             outbound[i].assigned != 1) ||
            (inbound[i].assigned != 0 &&
             inbound[i].assigned != 1)) {

            printf("Invalid seat status detected in file.\n");
            printf("Starting with empty seats.\n");
            return 0;
        }
    }

    printf("Saved flight data loaded.\n");

    return 1;
}

int main(void) {
    struct seat outbound[SEATS];
    struct seat inbound[SEATS];

    char choice;

    initialize(outbound);
    initialize(inbound);

    /* Try loading saved reservations */
    if (loadData(outbound, inbound) == 0) {
        initialize(outbound);
        initialize(inbound);
    }

    while (1) {
        printf("\n=== COLOSSUS AIRLINES ===\n");
        printf("a) Outbound Flight\n");
        printf("b) Inbound Flight\n");
        printf("c) Quit\n");
        printf("Choice: ");

        /*
         * If automated input reaches the end of the file,
         * exit instead of printing forever.
         */
        if (scanf(" %c", &choice) != 1) {
            printf("\nEnd of input detected.\n");
            saveData(outbound, inbound);
            break;
        }

        switch (choice) {
            case 'a':
                flightMenu(outbound);
                break;

            case 'b':
                flightMenu(inbound);
                break;

            case 'c':
                saveData(outbound, inbound);
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}