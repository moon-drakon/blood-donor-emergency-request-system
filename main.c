#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DONOR_FILE "donors.dat"
#define TEMP_DONOR_FILE "temp_donors.dat"
#define REQUEST_FILE "requests.dat"
#define TEMP_REQUEST_FILE "temp_requests.dat"
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
int generateNextRequestId(void);
int isDonorAvailable(const Donor *donor);
void printMatchedDonorRow(const Donor *donor);
void getCurrentDateTime(char *buffer, int size);
void displayDonor(const Donor *donor);
void displayRequest(const Request *request);

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
void matchDonorsByBloodGroup(void);
void matchDonorsByRequestId(void);

void showDonorSummary(void);
void showRequestSummary(void);
void exportDonorReportToTXT(void);
void exportRequestReportToTXT(void);
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
    printf("6. Match Donors by Blood Group\n");
    printf("7. Match Donors by Request ID\n");
    printf("0. Back to Main Menu\n");
    printf("------------------------------------\n");
}

void showReportMenu(void)
{
    printf("\n--- Reports and Activity Log ---\n");
    printf("1. Show Donor Summary\n");
    printf("2. Show Request Summary\n");
    printf("3. Export Donor Report to TXT\n");
    printf("4. Export Request Report to TXT\n");
    printf("5. View Activity Log\n");
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
        case 6:
            matchDonorsByBloodGroup();
            break;
        case 7:
            matchDonorsByRequestId();
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
            showDonorSummary();
            break;
        case 2:
            showRequestSummary();
            break;
        case 3:
            exportDonorReportToTXT();
            break;
        case 4:
            exportRequestReportToTXT();
            break;
        case 5:
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

void getCurrentDateTime(char *buffer, int size)
{
    time_t now;
    struct tm *timeInfo;

    now = time(NULL);
    timeInfo = localtime(&now);

    if (timeInfo == NULL)
    {
        snprintf(buffer, size, "Unknown Time");
        return;
    }

    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeInfo);
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

int generateNextRequestId(void)
{
    FILE *file;
    Request request;
    int lastId = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        return 1;
    }

    /* Read all requests to find the last used ID. */
    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        lastId = request.requestId;
    }

    fclose(file);
    return lastId + 1;
}

int isDonorAvailable(const Donor *donor)
{
    if (strcmp(donor->availabilityStatus, "1") == 0)
    {
        return 1;
    }

    if (strcmp(donor->availabilityStatus, "Available") == 0 ||
        strcmp(donor->availabilityStatus, "available") == 0)
    {
        return 1;
    }

    return 0;
}

void printMatchedDonorRow(const Donor *donor)
{
    printf("%-8d %-20s %-10s %-15s %-12s\n",
           donor->donorId,
           donor->name,
           donor->bloodGroup,
           donor->phone,
           isDonorAvailable(donor) ? "Available" : "Unavailable");
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
    printf("Availability       : %s\n", isDonorAvailable(donor) ? "Available" : "Unavailable");
    printf("-----------------------------------\n");
}

void displayRequest(const Request *request)
{
    printf("\n-----------------------------------\n");
    printf("Request ID         : %d\n", request->requestId);
    printf("Patient Name       : %s\n", request->patientName);
    printf("Blood Group Needed : %s\n", request->bloodGroupNeeded);
    printf("Units Needed       : %d\n", request->unitsNeeded);
    printf("Hospital Name      : %s\n", request->hospitalName);
    printf("Location           : %s\n", request->location);
    printf("Contact Number     : %s\n", request->contactNumber);
    printf("Urgency Level      : %s\n", request->urgencyLevel);
    printf("Request Status     : %s\n", request->requestStatus);
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
    getTextInput("Enter availability status (1 for available, 0 for unavailable): ",
                 donor.availabilityStatus,
                 sizeof(donor.availabilityStatus));

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
    writeActivityLog("Added a new donor record.");
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
    writeActivityLog("Updated donor information.");
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
    writeActivityLog("Deleted a donor record.");
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
            getTextInput("Enter new availability status (1 for available, 0 for unavailable): ",
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
    writeActivityLog("Changed donor availability status.");
    pauseScreen();
}

void addRequest(void)
{
    FILE *file;
    Request request;

    request.requestId = generateNextRequestId();
    file = fopen(REQUEST_FILE, "ab");

    if (file == NULL)
    {
        printf("\nUnable to open request file.\n");
        pauseScreen();
        return;
    }

    printf("\n--- Add Emergency Request ---\n");
    printf("Assigned Request ID: %d\n", request.requestId);

    getTextInput("Enter patient name: ", request.patientName, sizeof(request.patientName));
    getTextInput("Enter blood group needed: ",
                 request.bloodGroupNeeded,
                 sizeof(request.bloodGroupNeeded));

    printf("Enter units needed: ");
    while (scanf("%d", &request.unitsNeeded) != 1)
    {
        printf("Invalid input. Enter units again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter hospital name: ", request.hospitalName, sizeof(request.hospitalName));
    getTextInput("Enter location: ", request.location, sizeof(request.location));
    getTextInput("Enter contact number: ", request.contactNumber, sizeof(request.contactNumber));
    getTextInput("Enter urgency level: ", request.urgencyLevel, sizeof(request.urgencyLevel));
    getTextInput("Enter request status: ", request.requestStatus, sizeof(request.requestStatus));

    /* Save the whole request record in binary format. */
    if (fwrite(&request, sizeof(Request), 1, file) != 1)
    {
        printf("\nFailed to save request information.\n");
        fclose(file);
        pauseScreen();
        return;
    }

    fclose(file);

    printf("\nEmergency request added successfully.\n");
    displayRequest(&request);
    writeActivityLog("Added a new emergency request.");
    pauseScreen();
}

void viewAllRequests(void)
{
    FILE *file;
    Request request;
    int found = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\n--- All Emergency Requests ---\n");

    /* Read each request one by one from the binary file. */
    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        displayRequest(&request);
        found = 1;
    }

    if (!found)
    {
        printf("\nNo request records found.\n");
    }

    fclose(file);
    pauseScreen();
}

void searchRequestById(void)
{
    FILE *file;
    Request request;
    int searchId;
    int found = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter request ID to search: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestId == searchId)
        {
            printf("\nRequest found.\n");
            displayRequest(&request);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\nNo request found with ID %d.\n", searchId);
    }

    fclose(file);
    pauseScreen();
}

void updateRequestStatus(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int searchId;
    int found = 0;

    sourceFile = fopen(REQUEST_FILE, "rb");

    if (sourceFile == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printf("\nUnable to open temporary request file.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter request ID to update status: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == searchId)
        {
            found = 1;

            printf("\nCurrent request information:\n");
            displayRequest(&request);
            getTextInput("Enter new request status: ",
                         request.requestStatus,
                         sizeof(request.requestStatus));
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printf("\nFailed to update request status.\n");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\nNo request found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printf("\nFailed to replace request file after status update.\n");
        pauseScreen();
        return;
    }

    printf("\nRequest status updated successfully.\n");
    writeActivityLog("Updated emergency request status.");
    pauseScreen();
}

void deleteRequest(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int searchId;
    int found = 0;

    sourceFile = fopen(REQUEST_FILE, "rb");

    if (sourceFile == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printf("\nUnable to open temporary request file.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter request ID to delete: ");
    while (scanf("%d", &searchId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == searchId)
        {
            found = 1;
            continue;
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printf("\nFailed to delete request information.\n");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\nNo request found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printf("\nFailed to replace request file after deletion.\n");
        pauseScreen();
        return;
    }

    printf("\nRequest deleted successfully.\n");
    writeActivityLog("Deleted an emergency request.");
    pauseScreen();
}

void matchDonorsByBloodGroup(void)
{
    FILE *donorFile;
    Donor donor;
    char bloodGroup[5];
    int found = 0;

    donorFile = fopen(DONOR_FILE, "rb");

    if (donorFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    getTextInput("\nEnter blood group to match: ", bloodGroup, sizeof(bloodGroup));

    printf("\n==============================================================\n");
    printf("               MATCHED AVAILABLE DONORS\n");
    printf("==============================================================\n");
    printf("Blood Group Filter: %s\n", bloodGroup);
    printf("Only donors with availability = 1 are shown.\n");
    printf("--------------------------------------------------------------\n");
    printf("%-8s %-20s %-10s %-15s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Status");
    printf("--------------------------------------------------------------\n");

    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
    {
        if (strcmp(donor.bloodGroup, bloodGroup) == 0 && isDonorAvailable(&donor))
        {
            printMatchedDonorRow(&donor);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No available donors found for blood group %s.\n", bloodGroup);
    }

    printf("==============================================================\n");
    fclose(donorFile);
    writeActivityLog("Matched donors by blood group.");
    pauseScreen();
}

void matchDonorsByRequestId(void)
{
    FILE *requestFile;
    FILE *donorFile;
    Request request;
    Donor donor;
    int requestId;
    int requestFound = 0;
    int donorFound = 0;

    requestFile = fopen(REQUEST_FILE, "rb");

    if (requestFile == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\nEnter request ID to find matching donors: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&request, sizeof(Request), 1, requestFile) == 1)
    {
        if (request.requestId == requestId)
        {
            requestFound = 1;
            break;
        }
    }

    fclose(requestFile);

    if (!requestFound)
    {
        printf("\nNo request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    donorFile = fopen(DONOR_FILE, "rb");

    if (donorFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    printf("\n==============================================================\n");
    printf("              DONOR MATCH RESULT FOR REQUEST ID %d\n", request.requestId);
    printf("==============================================================\n");
    printf("Patient Name       : %s\n", request.patientName);
    printf("Blood Group Needed : %s\n", request.bloodGroupNeeded);
    printf("Units Needed       : %d\n", request.unitsNeeded);
    printf("Urgency Level      : %s\n", request.urgencyLevel);
    printf("Request Status     : %s\n", request.requestStatus);
    printf("--------------------------------------------------------------\n");
    printf("%-8s %-20s %-10s %-15s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Status");
    printf("--------------------------------------------------------------\n");

    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
    {
        if (strcmp(donor.bloodGroup, request.bloodGroupNeeded) == 0 &&
            isDonorAvailable(&donor))
        {
            printMatchedDonorRow(&donor);
            donorFound = 1;
        }
    }

    if (!donorFound)
    {
        printf("No available donors found for request ID %d.\n", request.requestId);
    }

    printf("==============================================================\n");
    fclose(donorFile);
    writeActivityLog("Matched donors by request ID.");
    pauseScreen();
}

void showDonorSummary(void)
{
    FILE *file;
    Donor donor;
    int totalDonors = 0;
    int availableDonors = 0;
    int unavailableDonors = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        totalDonors++;

        if (isDonorAvailable(&donor))
        {
            availableDonors++;
        }
        else
        {
            unavailableDonors++;
        }
    }

    fclose(file);

    printf("\n=================================================\n");
    printf("               DONOR SUMMARY REPORT\n");
    printf("=================================================\n");
    printf("Total Donors         : %d\n", totalDonors);
    printf("Available Donors     : %d\n", availableDonors);
    printf("Unavailable Donors   : %d\n", unavailableDonors);
    printf("=================================================\n");

    writeActivityLog("Viewed donor summary report.");
    pauseScreen();
}

void showRequestSummary(void)
{
    FILE *file;
    Request request;
    int totalRequests = 0;
    int pendingRequests = 0;
    int fulfilledRequests = 0;
    int otherRequests = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        totalRequests++;

        if (strcmp(request.requestStatus, "Pending") == 0 ||
            strcmp(request.requestStatus, "pending") == 0)
        {
            pendingRequests++;
        }
        else if (strcmp(request.requestStatus, "Fulfilled") == 0 ||
                 strcmp(request.requestStatus, "fulfilled") == 0)
        {
            fulfilledRequests++;
        }
        else
        {
            otherRequests++;
        }
    }

    fclose(file);

    printf("\n=================================================\n");
    printf("              REQUEST SUMMARY REPORT\n");
    printf("=================================================\n");
    printf("Total Requests       : %d\n", totalRequests);
    printf("Pending Requests     : %d\n", pendingRequests);
    printf("Fulfilled Requests   : %d\n", fulfilledRequests);
    printf("Other Status         : %d\n", otherRequests);
    printf("=================================================\n");

    writeActivityLog("Viewed request summary report.");
    pauseScreen();
}

void exportDonorReportToTXT(void)
{
    FILE *dataFile;
    FILE *reportFile;
    Donor donor;
    int totalDonors = 0;
    int availableDonors = 0;
    int unavailableDonors = 0;
    char dateTime[30];

    dataFile = fopen(DONOR_FILE, "rb");

    if (dataFile == NULL)
    {
        printf("\nNo donor records found yet.\n");
        pauseScreen();
        return;
    }

    reportFile = fopen(DONOR_REPORT_FILE, "w");

    if (reportFile == NULL)
    {
        fclose(dataFile);
        printf("\nUnable to create donor report file.\n");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    fprintf(reportFile, "NSU Blood Donor Management System\n");
    fprintf(reportFile, "Donor Report\n");
    fprintf(reportFile, "Generated On: %s\n", dateTime);
    fprintf(reportFile, "============================================================\n\n");

    while (fread(&donor, sizeof(Donor), 1, dataFile) == 1)
    {
        totalDonors++;

        if (isDonorAvailable(&donor))
        {
            availableDonors++;
        }
        else
        {
            unavailableDonors++;
        }

        fprintf(reportFile, "Donor ID           : %d\n", donor.donorId);
        fprintf(reportFile, "Name               : %s\n", donor.name);
        fprintf(reportFile, "Age                : %d\n", donor.age);
        fprintf(reportFile, "Gender             : %s\n", donor.gender);
        fprintf(reportFile, "Blood Group        : %s\n", donor.bloodGroup);
        fprintf(reportFile, "Phone              : %s\n", donor.phone);
        fprintf(reportFile, "Address            : %s\n", donor.address);
        fprintf(reportFile, "Last Donation Date : %s\n", donor.lastDonationDate);
        fprintf(reportFile, "Availability       : %s\n",
                isDonorAvailable(&donor) ? "Available" : "Unavailable");
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Donors       : %d\n", totalDonors);
    fprintf(reportFile, "Available Donors   : %d\n", availableDonors);
    fprintf(reportFile, "Unavailable Donors : %d\n", unavailableDonors);

    fclose(dataFile);
    fclose(reportFile);

    printf("\nDonor report exported successfully to %s\n", DONOR_REPORT_FILE);
    writeActivityLog("Exported donor report to TXT.");
    pauseScreen();
}

void exportRequestReportToTXT(void)
{
    FILE *dataFile;
    FILE *reportFile;
    Request request;
    int totalRequests = 0;
    int pendingRequests = 0;
    int fulfilledRequests = 0;
    int otherRequests = 0;
    char dateTime[30];

    dataFile = fopen(REQUEST_FILE, "rb");

    if (dataFile == NULL)
    {
        printf("\nNo request records found yet.\n");
        pauseScreen();
        return;
    }

    reportFile = fopen(REQUEST_REPORT_FILE, "w");

    if (reportFile == NULL)
    {
        fclose(dataFile);
        printf("\nUnable to create request report file.\n");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    fprintf(reportFile, "NSU Blood Donor Management System\n");
    fprintf(reportFile, "Emergency Request Report\n");
    fprintf(reportFile, "Generated On: %s\n", dateTime);
    fprintf(reportFile, "============================================================\n\n");

    while (fread(&request, sizeof(Request), 1, dataFile) == 1)
    {
        totalRequests++;

        if (strcmp(request.requestStatus, "Pending") == 0 ||
            strcmp(request.requestStatus, "pending") == 0)
        {
            pendingRequests++;
        }
        else if (strcmp(request.requestStatus, "Fulfilled") == 0 ||
                 strcmp(request.requestStatus, "fulfilled") == 0)
        {
            fulfilledRequests++;
        }
        else
        {
            otherRequests++;
        }

        fprintf(reportFile, "Request ID         : %d\n", request.requestId);
        fprintf(reportFile, "Patient Name       : %s\n", request.patientName);
        fprintf(reportFile, "Blood Group Needed : %s\n", request.bloodGroupNeeded);
        fprintf(reportFile, "Units Needed       : %d\n", request.unitsNeeded);
        fprintf(reportFile, "Hospital Name      : %s\n", request.hospitalName);
        fprintf(reportFile, "Location           : %s\n", request.location);
        fprintf(reportFile, "Contact Number     : %s\n", request.contactNumber);
        fprintf(reportFile, "Urgency Level      : %s\n", request.urgencyLevel);
        fprintf(reportFile, "Request Status     : %s\n", request.requestStatus);
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Requests     : %d\n", totalRequests);
    fprintf(reportFile, "Pending Requests   : %d\n", pendingRequests);
    fprintf(reportFile, "Fulfilled Requests : %d\n", fulfilledRequests);
    fprintf(reportFile, "Other Status       : %d\n", otherRequests);

    fclose(dataFile);
    fclose(reportFile);

    printf("\nRequest report exported successfully to %s\n", REQUEST_REPORT_FILE);
    writeActivityLog("Exported request report to TXT.");
    pauseScreen();
}

void viewActivityLog(void)
{
    FILE *logFile;
    char line[200];

    logFile = fopen(ACTIVITY_LOG_FILE, "r");

    if (logFile == NULL)
    {
        printf("\nNo activity log found yet.\n");
        pauseScreen();
        return;
    }

    printf("\n============================================================\n");
    printf("                     ACTIVITY LOG\n");
    printf("============================================================\n");

    while (fgets(line, sizeof(line), logFile) != NULL)
    {
        printf("%s", line);
    }

    printf("============================================================\n");
    fclose(logFile);
    pauseScreen();
}

void writeActivityLog(const char *action)
{
    FILE *logFile;
    char dateTime[30];

    logFile = fopen(ACTIVITY_LOG_FILE, "a");

    if (logFile == NULL)
    {
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));
    fprintf(logFile, "[%s] %s\n", dateTime, action);
    fclose(logFile);
}
