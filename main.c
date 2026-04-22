#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DONOR_FILE "donors.dat"
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

void addDonor(void);
void viewAllDonors(void);
void searchDonorById(void);
void updateDonor(void);
void deleteDonor(void);

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

void addDonor(void)
{
    printf("\nAdd Donor feature will be implemented in the next step.\n");
    pauseScreen();
}

void viewAllDonors(void)
{
    printf("\nView All Donors feature will be implemented in the next step.\n");
    pauseScreen();
}

void searchDonorById(void)
{
    printf("\nSearch Donor by ID feature will be implemented in the next step.\n");
    pauseScreen();
}

void updateDonor(void)
{
    printf("\nUpdate Donor feature will be implemented in the next step.\n");
    pauseScreen();
}

void deleteDonor(void)
{
    printf("\nDelete Donor feature will be implemented in the next step.\n");
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
