#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DONOR_FILE "donors.dat"
#define TEMP_DONOR_FILE "temp_donors.dat"
#define REQUEST_FILE "requests.dat"
#define DONOR_REPORT_FILE "donor_report.txt"
#define REQUEST_REPORT_FILE "request_report.txt"
#define ACTIVITY_LOG_FILE "activity_log.txt"

typedef struct
{
    int donorId;
    char name[50];
    int age;
    char gender[10];
    char bloodGroup[5];
    char phone[20];
    char address[100];
    char lastDonationDate[20];
    char availabilityStatus[20];
} Donor;

typedef struct
{
    int requestId;
    char patientName[50];
    char bloodGroupNeeded[5];
    int unitsNeeded;
    char hospitalName[60];
    char location[100];
    char contactNumber[20];
    char urgencyLevel[20];
    char requestStatus[20];
} Request;

void showMainMenu(void);
void showDonorMenu(void);
void showRequestMenu(void);
void showReportMenu(void);
void donorMenu(void);
void requestMenu(void);
void reportMenu(void);
void clearInputBuffer(void);
int getMenuChoice(void);
void pauseScreen(void);
void getTextInput(const char *prompt, char *buffer, int size);
int generateNextDonorId(void);
void displayDonor(const Donor *donor);

void addDonor(void);
void viewAllDonors(void);
void searchDonorById(void);
void updateDonor(void);
void deleteDonor(void);
void changeDonorAvailability(void);

void addRequest(void);
void viewAllRequests(void);
void searchRequestById(void);
void updateRequestStatus(void);
void deleteRequest(void);

void exportDonorReport(void);
void exportRequestReport(void);
void viewActivityLog(void);
void writeActivityLog(const char *action);

int main(void)
{
    int choice;

    while (1)
    {
        showMainMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            donorMenu();
            break;
        case 2:
            requestMenu();
            break;
        case 3:
            reportMenu();
            break;
        case 0:
            printf("\nThank you for using the system.\n");
            printf("Exiting program...\n");
            return 0;
        default:
            printf("\nInvalid choice. Please try again.\n");
            pauseScreen();
        }
    }
}

void showMainMenu(void)
{
    printf("\n");
    printf("===========================================================\n");
    printf("   NSU Blood Donor and Emergency Request Management System\n");
    printf("===========================================================\n");
    printf("1. Donor Management\n");
    printf("2. Emergency Request Management\n");
    printf("3. Reports and Activity Log\n");
    printf("0. Exit\n");
    printf("===========================================================\n");
}

void showDonorMenu(void)
{
    printf("\n--- Donor Management ---\n");
    printf("1. Add Donor\n");
    printf("2. View All Donors\n");
    printf("3. Search Donor by ID\n");
    printf("4. Update Donor\n");
    printf("5. Delete Donor\n");
    printf("6. Change Donor Availability\n");
    printf("0. Back to Main Menu\n");
    printf("-----------------------------------\n");
}

void showRequestMenu(void)
{
    printf("\n--- Emergency Request Management ---\n");
    printf("1. Add Emergency Request\n");
    printf("2. View All Requests\n");
    printf("3. Search Request by ID\n");
    printf("4. Update Request Status\n");
    printf("5. Delete Request\n");
    printf("0. Back to Main Menu\n");
    printf("------------------------------------\n");
}

void showReportMenu(void)
{
    printf("\n--- Reports and Activity Log ---\n");
    printf("1. Export Donor Report\n");
    printf("2. Export Request Report\n");
    printf("3. View Activity Log\n");
    printf("0. Back to Main Menu\n");
    printf("--------------------------------\n");
}

void donorMenu(void)
{
    int choice;

    while (1)
    {
        showDonorMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            addDonor();
            break;
        case 2:
            viewAllDonors();
            break;
        case 3:
            searchDonorById();
            break;
        case 4:
            updateDonor();
            break;
        case 5:
            deleteDonor();
            break;
        case 6:
            changeDonorAvailability();
            break;
        case 0:
            return;
        default:
            printf("\nInvalid choice. Please try again.\n");
            pauseScreen();
        }
    }
}

void requestMenu(void)
{
    int choice;

    while (1)
    {
        showRequestMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            addRequest();
            break;
        case 2:
            viewAllRequests();
            break;
        case 3:
            searchRequestById();
            break;
        case 4:
            updateRequestStatus();
            break;
        case 5:
            deleteRequest();
            break;
        case 0:
            return;
        default:
            printf("\nInvalid choice. Please try again.\n");
            pauseScreen();
        }
    }
}

void reportMenu(void)
{
    int choice;

    while (1)
    {
        showReportMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            exportDonorReport();
            break;
        case 2:
            exportRequestReport();
            break;
        case 3:
            viewActivityLog();
            break;
        case 0:
            return;
        default:
            printf("\nInvalid choice. Please try again.\n");
            pauseScreen();
        }
    }
}

void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int getMenuChoice(void)
{
    int choice;

    printf("Enter your choice: ");

    while (scanf("%d", &choice) != 1)
    {
        printf("Invalid input. Please enter a number: ");
        clearInputBuffer();
    }

    clearInputBuffer();
    return choice;
}

void pauseScreen(void)
{
    printf("\nPress Enter to continue...");
    getchar();
}

void getTextInput(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int generateNextDonorId(void)
{
    FILE *file;
    Donor donor;
    int lastId = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        return 1;
    }

    /* Read all donors to find the last used ID. */
    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        lastId = donor.donorId;
    }

    fclose(file);
    return lastId + 1;
}

void displayDonor(const Donor *donor)
{
    printf("\n-----------------------------------\n");
    printf("Donor ID           : %d\n", donor->donorId);
    printf("Name               : %s\n", donor->name);
    printf("Age                : %d\n", donor->age);
    printf("Gender             : %s\n", donor->gender);
    printf("Blood Group        : %s\n", donor->bloodGroup);
    printf("Phone              : %s\n", donor->phone);
    printf("Address            : %s\n", donor->address);
    printf("Last Donation Date : %s\n", donor->lastDonationDate);
    printf("Availability       : %s\n", donor->availabilityStatus);
    printf("-----------------------------------\n");
}

void addDonor(void)
{
    FILE *file;
    Donor donor;

    donor.donorId = generateNextDonorId();
    file = fopen(DONOR_FILE, "ab");

    if (file == NULL)
    {
        printf("\nUnable to open donor file.\n");
        pauseScreen();
        return;
    }

    printf("\n--- Add Donor ---\n");
    printf("Assigned Donor ID: %d\n", donor.donorId);

    getTextInput("Enter donor name: ", donor.name, sizeof(donor.name));

    printf("Enter age: ");
    while (scanf("%d", &donor.age) != 1)
    {
        printf("Invalid input. Enter age again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter gender: ", donor.gender, sizeof(donor.gender));
    getTextInput("Enter blood group: ", donor.bloodGroup, sizeof(donor.bloodGroup));
    getTextInput("Enter phone number: ", donor.phone, sizeof(donor.phone));
    getTextInput("Enter address: ", donor.address, sizeof(donor.address));
    getTextInput("Enter last donation date: ", donor.lastDonationDate, sizeof(donor.lastDonationDate));
    getTextInput("Enter availability status: ", donor.availabilityStatus, sizeof(donor.availabilityStatus));

    /* Save the whole donor record in binary format. */
    if (fwrite(&donor, sizeof(Donor), 1, file) != 1)
    {
        printf("\nFailed to save donor information.\n");
        fclose(file);
        pauseScreen();
        return;
    }

    fclose(file);

    printf("\nDonor added successfully.\n");
    displayDonor(&donor);
    pauseScreen();
}

void viewAllDonors(void)
{
    FILE *file;
    Donor donor;
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\n--- All Donors ---\n");

    /* Read each donor one by one from the binary file. */
    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        displayDonor(&donor);
        found = 1;
    }

    if (!found)
    {
        printf("\nNo donor records found.\n");
    }

    fclose(file);
    pauseScreen();
}

void searchDonorById(void)
{
    FILE *file;
    Donor donor;
    int searchId;
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter donor ID to search: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.donorId == searchId)
        {
            printf("\nDonor found.\n");
            displayDonor(&donor);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo donor found with ID %d.\n", searchId);
    }

    fclose(file);
    pauseScreen();
}

void updateDonor(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int searchId;
    int found = 0;

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printf("\nUnable to open temporary donor file.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter donor ID to update: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == searchId)
        {
            found = 1;

            printf("\nCurrent donor information:\n");
            displayDonor(&donor);
            printf("\nEnter new information for this donor.\n");

            getTextInput("Enter donor name: ", donor.name, sizeof(donor.name));

            printf("Enter age: ");
            while (scanf("%d", &donor.age) != 1)
            {
                printf("Invalid input. Enter age again: ");
                clearInputBuffer();
            }
            clearInputBuffer();

            getTextInput("Enter gender: ", donor.gender, sizeof(donor.gender));
            getTextInput("Enter blood group: ", donor.bloodGroup, sizeof(donor.bloodGroup));
            getTextInput("Enter phone number: ", donor.phone, sizeof(donor.phone));
            getTextInput("Enter address: ", donor.address, sizeof(donor.address));
            getTextInput("Enter last donation date: ", donor.lastDonationDate, sizeof(donor.lastDonationDate));
        }

        /* Write every donor to the temporary file, including the updated one. */
        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printf("\nFailed to update donor information.\n");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\nNo donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printf("\nFailed to replace donor file after update.\n");
        pauseScreen();
        return;
    }

    printf("\nDonor updated successfully.\n");
    pauseScreen();
}

void deleteDonor(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int searchId;
    int found = 0;

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printf("\nUnable to open temporary donor file.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter donor ID to delete: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == searchId)
        {
            found = 1;
            continue;
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printf("\nFailed to delete donor information.\n");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\nNo donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printf("\nFailed to replace donor file after deletion.\n");
        pauseScreen();
        return;
    }

    printf("\nDonor deleted successfully.\n");
    pauseScreen();
}

void changeDonorAvailability(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int searchId;
    int found = 0;

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printf("\nUnable to open temporary donor file.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter donor ID to change availability: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == searchId)
        {
            found = 1;

            printf("\nCurrent donor information:\n");
            displayDonor(&donor);
            getTextInput("Enter new availability status: ",
                         donor.availabilityStatus,
                         sizeof(donor.availabilityStatus));
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printf("\nFailed to change donor availability.\n");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\nNo donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printf("\nFailed to replace donor file after availability change.\n");
        pauseScreen();
        return;
    }

    printf("\nDonor availability updated successfully.\n");
    pauseScreen();
}

void addRequest(void)
{
    printf("\nAdd Emergency Request feature will be implemented in the next step.\n");
    pauseScreen();
}

void viewAllRequests(void)
{
    printf("\nView All Requests feature will be implemented in the next step.\n");
    pauseScreen();
}

void searchRequestById(void)
{
    printf("\nSearch Request by ID feature will be implemented in the next step.\n");
    pauseScreen();
}

void updateRequestStatus(void)
{
    printf("\nUpdate Request Status feature will be implemented in the next step.\n");
    pauseScreen();
}

void deleteRequest(void)
{
    printf("\nDelete Request feature will be implemented in the next step.\n");
    pauseScreen();
}

void exportDonorReport(void)
{
    printf("\nExport Donor Report feature will be implemented later.\n");
    pauseScreen();
}

void exportRequestReport(void)
{
    printf("\nExport Request Report feature will be implemented later.\n");
    pauseScreen();
}

void viewActivityLog(void)
{
    printf("\nView Activity Log feature will be implemented later.\n");
    pauseScreen();
}

void writeActivityLog(const char *action)
{
    printf("\nwriteActivityLog(\"%s\") will be implemented later.\n", action);
}
