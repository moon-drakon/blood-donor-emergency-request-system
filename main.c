#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DONOR_FILE "donors.dat"
#define TEMP_DONOR_FILE "temp_donors.dat"
#define REQUEST_FILE "requests.dat"
#define TEMP_REQUEST_FILE "temp_requests.dat"
#define ADMIN_FILE "admins.dat"
#define TEMP_ADMIN_FILE "temp_admins.dat"
#define DONATION_FILE "donations.dat"
#define TEMP_DONATION_FILE "temp_donations.dat"
#define DONOR_REPORT_FILE "donor_report.txt"
#define REQUEST_REPORT_FILE "request_report.txt"
#define DONATION_REPORT_FILE "donation_report.txt"
#define ACTIVITY_LOG_FILE "activity_log.txt"

#define REQUEST_PENDING 0
#define REQUEST_MATCHED 1
#define REQUEST_WAITING_VERIFICATION 2
#define REQUEST_FULFILLED 3

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
    char password[30];
    int donationCount;
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
    char trackingPIN[10];
    int assignedDonorId;
    int requesterConfirmed;
    int adminVerified;
    int requestStatus;
} Request;

typedef struct
{
    int adminId;
    char password[30];
    char name[50];
} Admin;

typedef struct
{
    int donationId;
    int requestId;
    int donorId;
    char bloodGroup[5];
    int unitsDonated;
    char donationDate[20];
    int requesterConfirmed;
    int adminVerified;
} DonationRecord;

void showAccessMenu(void);
void showMainMenu(void);
void showAdminMenu(void);
void showDonorMenu(void);
void showRequestMenu(void);
void showReportMenu(void);
void showGuestMenu(void);
void adminMenu(int adminId);
void adminDonorMenu(void);
void donorMenu(int donorId);
void requestMenu(void);
void reportMenu(void);
void requesterAccess(void);
void guestMenu(void);
void clearInputBuffer(void);
int getMenuChoice(void);
void pauseScreen(void);
void getTextInput(const char *prompt, char *buffer, int size);
void printLine(char symbol, int count);
void printSectionHeader(const char *title);
void printStatusMessage(const char *type, const char *message);
int generateNextDonorId(void);
int generateNextRequestId(void);
int generateNextDonationId(void);
int isDonorAvailable(const Donor *donor);
void printMatchedDonorRow(const Donor *donor);
void getCurrentDateTime(char *buffer, int size);
const char *getRequestStatusText(int status);
const char *getAvailabilityText(const Donor *donor);
void generateRequestPin(char *pin, int size);
void displayDonor(const Donor *donor);
void displayRequest(const Request *request);
void displayDonationRecord(const DonationRecord *record);

void initializeDefaultAdmin(void);
int adminLogin(void);
void changeAdminPassword(int adminId);
int donorLogin(void);
void viewOwnDonorProfile(int donorId);
void changeOwnDonorAvailability(int donorId);
void changeDonorPassword(int donorId);

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
void trackRequestByIdAndPin(void);
void assignDonorToRequest(void);
void requesterConfirmDonation(void);
void adminVerifyDonation(void);

void showDonorSummary(void);
void showRequestSummary(void);
void exportDonorReportToTXT(void);
void exportRequestReportToTXT(void);
void exportDonationReportToTXT(void);
void viewDonationRecords(void);
void viewPublicBloodGroupAvailability(void);
void viewActivityLog(void);
void writeActivityLog(const char *action);
int donationRecordExistsForRequest(int requestId);
void createDonationRecord(const Request *request);

int main(void)
{
    int choice;
    int donorId;
    int adminId;

    initializeDefaultAdmin();
    srand((unsigned int)time(NULL));

    while (1)
    {
        showAccessMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            adminId = adminLogin();
            if (adminId != 0)
            {
                adminMenu(adminId);
            }
            break;
        case 2:
            donorId = donorLogin();
            if (donorId != 0)
            {
                donorMenu(donorId);
            }
            break;
        case 3:
            requesterAccess();
            break;
        case 4:
            guestMenu();
            break;
        case 5:
            printStatusMessage("SUCCESS", "Thank you for using the system.");
            printf("Program closed successfully.\n");
            return 0;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void showAccessMenu(void)
{
    printf("\n");
    printSectionHeader("NSU Blood Donor and Emergency Request Management System");
    printf("  1. Administrator Login\n");
    printf("  2. Donor Login\n");
    printf("  3. Request Tracking / Requester Access\n");
    printf("  4. Guest Access\n");
    printf("  5. Exit\n");
    printLine('=', 63);
}

void showMainMenu(void)
{
    printf("\n");
    printSectionHeader("NSU Blood Donor and Emergency Request Management System");
    printf("  1. Donor Management\n");
    printf("  2. Emergency Request Management\n");
    printf("  3. Reports and Activity Log\n");
    printf("  0. Exit\n");
    printLine('=', 63);
}

void showAdminMenu(void)
{
    printf("\n");
    printSectionHeader("Admin Dashboard");
    printf("  1. Donor Management\n");
    printf("  2. Emergency Request Management\n");
    printf("  3. Reports and Activity Log\n");
    printf("  4. Change Admin Password\n");
    printf("  0. Logout\n");
    printLine('=', 63);
}

void showDonorMenu(void)
{
    printSectionHeader("Donor Management");
    printf("  1. Add Donor\n");
    printf("  2. View All Donors\n");
    printf("  3. Search Donor by ID\n");
    printf("  4. Update Donor\n");
    printf("  5. Delete Donor\n");
    printf("  6. Change Donor Availability\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 63);
}

void showRequestMenu(void)
{
    printSectionHeader("Emergency Request Management");
    printf("  1. Add Emergency Request\n");
    printf("  2. View All Requests\n");
    printf("  3. Search Request by ID\n");
    printf("  4. Update Request Status\n");
    printf("  5. Delete Request\n");
    printf("  6. Match Donors by Blood Group\n");
    printf("  7. Match Donors by Request ID\n");
    printf("  8. Assign Donor to Request\n");
    printf("  9. Admin Verify Donation\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 63);
}

void showReportMenu(void)
{
    printSectionHeader("Reports and Activity Log");
    printf("  1. Show Donor Summary\n");
    printf("  2. Show Request Summary\n");
    printf("  3. Export Donor Report to TXT\n");
    printf("  4. Export Request Report to TXT\n");
    printf("  5. Export Donation Report to TXT\n");
    printf("  6. View Donation Records\n");
    printf("  7. View Activity Log\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 63);
}

void showGuestMenu(void)
{
    printSectionHeader("Guest Access");
    printf("  1. Create Emergency Blood Request\n");
    printf("  2. Search Donor by Blood Group\n");
    printf("  3. View Public Blood Group Availability\n");
    printf("  4. Back\n");
    printLine('-', 63);
}

void adminMenu(int adminId)
{
    int choice;

    while (1)
    {
        showAdminMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            adminDonorMenu();
            break;
        case 2:
            requestMenu();
            break;
        case 3:
            reportMenu();
            break;
        case 4:
            changeAdminPassword(adminId);
            break;
        case 0:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void adminDonorMenu(void)
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
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
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
        case 8:
            assignDonorToRequest();
            break;
        case 9:
            adminVerifyDonation();
            break;
        case 0:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
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
            exportDonationReportToTXT();
            break;
        case 6:
            viewDonationRecords();
            break;
        case 7:
            viewActivityLog();
            break;
        case 0:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void guestMenu(void)
{
    int choice;

    while (1)
    {
        showGuestMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            addRequest();
            break;
        case 2:
            matchDonorsByBloodGroup();
            break;
        case 3:
            viewPublicBloodGroupAvailability();
            break;
        case 4:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void requesterAccess(void)
{
    printSectionHeader("Requester Access");
    printStatusMessage("INFO", "Requester access will be implemented later.");
    pauseScreen();
}

void initializeDefaultAdmin(void)
{
    FILE *file;
    Admin admin;

    file = fopen(ADMIN_FILE, "rb");

    if (file != NULL)
    {
        fclose(file);
        return;
    }

    file = fopen(ADMIN_FILE, "wb");

    if (file == NULL)
    {
        return;
    }

    admin.adminId = 1001;
    strcpy(admin.password, "admin123");
    strcpy(admin.name, "System Admin");
    fwrite(&admin, sizeof(Admin), 1, file);
    fclose(file);
}

int adminLogin(void)
{
    FILE *file;
    Admin admin;
    int inputId;
    char inputPassword[30];
    int loginSuccess = 0;

    printSectionHeader("Admin Login");

    file = fopen(ADMIN_FILE, "rb");

    if (file == NULL)
    {
        initializeDefaultAdmin();
        file = fopen(ADMIN_FILE, "rb");
    }

    if (file == NULL)
    {
        printStatusMessage("ERROR", "Unable to open admin file.");
        pauseScreen();
        return 0;
    }

    printf("Enter Admin ID: ");
    while (scanf("%d", &inputId) != 1)
    {
        printf("Invalid input. Enter Admin ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter Password: ", inputPassword, sizeof(inputPassword));

    while (fread(&admin, sizeof(Admin), 1, file) == 1)
    {
        if (admin.adminId == inputId && strcmp(admin.password, inputPassword) == 0)
        {
            loginSuccess = 1;
            break;
        }
    }

    fclose(file);

    if (loginSuccess)
    {
        printf("\nWelcome, %s!\n", admin.name);
        printStatusMessage("SUCCESS", "Admin login successful.");
        writeActivityLog("Admin logged in.");
        pauseScreen();
        return admin.adminId;
    }

    printStatusMessage("ERROR", "Invalid Admin ID or password.");
    writeActivityLog("Failed admin login attempt.");
    pauseScreen();
    return 0;
}

void changeAdminPassword(int adminId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Admin admin;
    char currentPassword[30];
    char newPassword[30];
    char confirmPassword[30];
    int found = 0;
    int passwordChanged = 0;

    printSectionHeader("Change Admin Password");
    getTextInput("Enter current password: ", currentPassword, sizeof(currentPassword));
    getTextInput("Enter new password: ", newPassword, sizeof(newPassword));
    getTextInput("Confirm new password: ", confirmPassword, sizeof(confirmPassword));

    if (strlen(newPassword) == 0)
    {
        printStatusMessage("ERROR", "Password cannot be empty.");
        pauseScreen();
        return;
    }

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printStatusMessage("ERROR", "New passwords do not match.");
        pauseScreen();
        return;
    }

    sourceFile = fopen(ADMIN_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("ERROR", "Unable to read admin account.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_ADMIN_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary admin file.");
        pauseScreen();
        return;
    }

    while (fread(&admin, sizeof(Admin), 1, sourceFile) == 1)
    {
        if (admin.adminId == adminId)
        {
            found = 1;

            if (strcmp(currentPassword, admin.password) == 0)
            {
                strcpy(admin.password, newPassword);
                passwordChanged = 1;
            }
        }

        if (fwrite(&admin, sizeof(Admin), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ADMIN_FILE);
            printStatusMessage("ERROR", "Failed to update admin password.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_ADMIN_FILE);
        printStatusMessage("ERROR", "Admin account was not found.");
        pauseScreen();
        return;
    }

    if (!passwordChanged)
    {
        remove(TEMP_ADMIN_FILE);
        printStatusMessage("ERROR", "Current password is incorrect.");
        pauseScreen();
        return;
    }

    if (remove(ADMIN_FILE) != 0 || rename(TEMP_ADMIN_FILE, ADMIN_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace admin file after password change.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Admin password changed successfully.");
    writeActivityLog("Admin password changed.");
    pauseScreen();
}

int donorLogin(void)
{
    FILE *file;
    Donor donor;
    int inputId;
    char inputPassword[30];
    int loginSuccess = 0;

    printSectionHeader("Donor Login");

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return 0;
    }

    printf("Enter Donor ID: ");
    while (scanf("%d", &inputId) != 1)
    {
        printf("Invalid input. Enter Donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter Password: ", inputPassword, sizeof(inputPassword));

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.donorId == inputId && strcmp(donor.password, inputPassword) == 0)
        {
            loginSuccess = 1;
            break;
        }
    }

    fclose(file);

    if (loginSuccess)
    {
        printf("\nWelcome, %s!\n", donor.name);
        printStatusMessage("SUCCESS", "Donor login successful.");
        writeActivityLog("Donor logged in.");
        pauseScreen();
        return inputId;
    }

    printStatusMessage("ERROR", "Invalid Donor ID or password.");
    writeActivityLog("Failed donor login attempt.");
    pauseScreen();
    return 0;
}

void donorMenu(int donorId)
{
    int choice;

    while (1)
    {
        printSectionHeader("Donor Portal");
        printf("  1. View My Profile\n");
        printf("  2. Change Password\n");
        printf("  0. Logout\n");
        printLine('-', 63);

        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            viewOwnDonorProfile(donorId);
            break;
        case 2:
            changeDonorPassword(donorId);
            break;
        case 0:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void viewOwnDonorProfile(int donorId)
{
    FILE *file;
    Donor donor;
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.donorId == donorId)
        {
            printStatusMessage("SUCCESS", "Donor profile found.");
            displayDonor(&donor);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "Donor profile was not found.");
    }

    pauseScreen();
}

void changeOwnDonorAvailability(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int found = 0;

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary donor file.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            printSectionHeader("Current Donor Information");
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
            printStatusMessage("ERROR", "Failed to change donor availability.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Donor profile was not found.");
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after availability change.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Availability updated successfully.");
    writeActivityLog("Donor changed own availability status.");
    pauseScreen();
}

void changeDonorPassword(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    char currentPassword[30];
    char newPassword[30];
    char confirmPassword[30];
    int found = 0;
    int passwordChanged = 0;

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary donor file.");
        pauseScreen();
        return;
    }

    printSectionHeader("Change Donor Password");
    getTextInput("Enter current password: ", currentPassword, sizeof(currentPassword));
    getTextInput("Enter new password: ", newPassword, sizeof(newPassword));
    getTextInput("Confirm new password: ", confirmPassword, sizeof(confirmPassword));

    if (strlen(newPassword) == 0)
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("ERROR", "Password cannot be empty.");
        pauseScreen();
        return;
    }

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("ERROR", "New passwords do not match.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;

            if (strcmp(currentPassword, donor.password) == 0)
            {
                strcpy(donor.password, newPassword);
                passwordChanged = 1;
            }
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printStatusMessage("ERROR", "Failed to change donor password.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Donor profile was not found.");
        pauseScreen();
        return;
    }

    if (!passwordChanged)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("ERROR", "Current password is incorrect.");
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after password change.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor password changed successfully.");
    writeActivityLog("Donor password changed.");
    pauseScreen();
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
        printf("[ERROR] Invalid input. Please enter a number: ");
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

void printLine(char symbol, int count)
{
    int i;

    for (i = 0; i < count; i++)
    {
        putchar(symbol);
    }

    putchar('\n');
}

void printSectionHeader(const char *title)
{
    printf("\n");
    printLine('=', 63);
    printf("%31s\n", title);
    printLine('=', 63);
}

void printStatusMessage(const char *type, const char *message)
{
    printf("\n[%s] %s\n", type, message);
}

const char *getRequestStatusText(int status)
{
    switch (status)
    {
    case REQUEST_PENDING:
        return "Pending";
    case REQUEST_MATCHED:
        return "Matched";
    case REQUEST_WAITING_VERIFICATION:
        return "Donation Done, Waiting Verification";
    case REQUEST_FULFILLED:
        return "Fulfilled / Verified";
    default:
        return "Unknown";
    }
}

const char *getAvailabilityText(const Donor *donor)
{
    return isDonorAvailable(donor) ? "Available" : "Unavailable";
}

void generateRequestPin(char *pin, int size)
{
    int value;

    if (size < 5)
    {
        if (size > 0)
        {
            pin[0] = '\0';
        }
        return;
    }

    value = 1000 + rand() % 9000;
    snprintf(pin, size, "%04d", value);
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

int generateNextDonationId(void)
{
    FILE *file;
    DonationRecord record;
    int lastId = 0;

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        return 1;
    }

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        lastId = record.donationId;
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
           getAvailabilityText(donor));
}

void displayDonor(const Donor *donor)
{
    printLine('-', 57);
    printf("%-20s : %d\n", "Donor ID", donor->donorId);
    printf("%-20s : %s\n", "Name", donor->name);
    printf("%-20s : %d\n", "Age", donor->age);
    printf("%-20s : %s\n", "Gender", donor->gender);
    printf("%-20s : %s\n", "Blood Group", donor->bloodGroup);
    printf("%-20s : %s\n", "Phone", donor->phone);
    printf("%-20s : %s\n", "Address", donor->address);
    printf("%-20s : %s\n", "Last Donation Date", donor->lastDonationDate);
    printf("%-20s : %s\n", "Availability", getAvailabilityText(donor));
    printf("%-20s : %d\n", "Donation Count", donor->donationCount);
    printLine('-', 57);
}

void displayRequest(const Request *request)
{
    printLine('-', 57);
    printf("%-20s : %d\n", "Request ID", request->requestId);
    printf("%-20s : %s\n", "Patient Name", request->patientName);
    printf("%-20s : %s\n", "Blood Group Needed", request->bloodGroupNeeded);
    printf("%-20s : %d\n", "Units Needed", request->unitsNeeded);
    printf("%-20s : %s\n", "Hospital Name", request->hospitalName);
    printf("%-20s : %s\n", "Location", request->location);
    printf("%-20s : %s\n", "Contact Number", request->contactNumber);
    printf("%-20s : %s\n", "Urgency Level", request->urgencyLevel);
    printf("%-20s : %s\n", "Tracking PIN", request->trackingPIN);
    printf("%-20s : %d\n", "Assigned Donor ID", request->assignedDonorId);
    printf("%-20s : %s\n", "Requester Confirmed", request->requesterConfirmed ? "Yes" : "No");
    printf("%-20s : %s\n", "Admin Verified", request->adminVerified ? "Yes" : "No");
    printf("%-20s : %s\n", "Request Status", getRequestStatusText(request->requestStatus));
    printLine('-', 57);
}

void displayDonationRecord(const DonationRecord *record)
{
    printLine('-', 57);
    printf("%-24s : %d\n", "Donation ID", record->donationId);
    printf("%-24s : %d\n", "Request ID", record->requestId);
    printf("%-24s : %d\n", "Donor ID", record->donorId);
    printf("%-24s : %s\n", "Blood Group", record->bloodGroup);
    printf("%-24s : %d\n", "Units Donated", record->unitsDonated);
    printf("%-24s : %s\n", "Donation Date", record->donationDate);
    printf("%-24s : %s\n", "Requester Confirmed", record->requesterConfirmed ? "Yes" : "No");
    printf("%-24s : %s\n", "Admin Verified", record->adminVerified ? "Yes" : "No");
    printLine('-', 57);
}

void addDonor(void)
{
    FILE *file;
    Donor donor;

    memset(&donor, 0, sizeof(Donor));
    donor.donorId = generateNextDonorId();
    file = fopen(DONOR_FILE, "ab");

    if (file == NULL)
    {
        printStatusMessage("ERROR", "Unable to open donor file.");
        pauseScreen();
        return;
    }

    printSectionHeader("Add Donor");
    printf("Assigned Donor ID : %d\n", donor.donorId);

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

    do
    {
        getTextInput("Create initial donor password: ", donor.password, sizeof(donor.password));

        if (strlen(donor.password) == 0)
        {
            printStatusMessage("ERROR", "Password cannot be empty.");
        }
    } while (strlen(donor.password) == 0);

    donor.donationCount = 0;

    /* Save the whole donor record in binary format. */
    if (fwrite(&donor, sizeof(Donor), 1, file) != 1)
    {
        printStatusMessage("ERROR", "Failed to save donor information.");
        fclose(file);
        pauseScreen();
        return;
    }

    fclose(file);

    printStatusMessage("SUCCESS", "Donor added successfully.");
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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("All Donors");

    /* Read each donor one by one from the binary file. */
    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        displayDonor(&donor);
        found = 1;
    }

    if (!found)
    {
        printStatusMessage("INFO", "No donor records found.");
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
        printStatusMessage("INFO", "No donor records found yet.");
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
            printStatusMessage("SUCCESS", "Donor found.");
            displayDonor(&donor);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\n[INFO] No donor found with ID %d.\n", searchId);
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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary donor file.");
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

            printSectionHeader("Current Donor Information");
            displayDonor(&donor);
            printStatusMessage("INFO", "Enter the updated donor information below.");

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
            printStatusMessage("ERROR", "Failed to update donor information.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\n[INFO] No donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after update.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor updated successfully.");
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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary donor file.");
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
            printStatusMessage("ERROR", "Failed to delete donor information.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\n[INFO] No donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after deletion.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor deleted successfully.");
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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary donor file.");
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

            printSectionHeader("Current Donor Information");
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
            printStatusMessage("ERROR", "Failed to change donor availability.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\n[INFO] No donor found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after availability change.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor availability updated successfully.");
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
        printStatusMessage("ERROR", "Unable to open request file.");
        pauseScreen();
        return;
    }

    printSectionHeader("Add Emergency Request");
    printf("Assigned Request ID : %d\n", request.requestId);

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
    generateRequestPin(request.trackingPIN, sizeof(request.trackingPIN));
    request.assignedDonorId = 0;
    request.requesterConfirmed = 0;
    request.adminVerified = 0;
    request.requestStatus = REQUEST_PENDING;

    /* Save the whole request record in binary format. */
    if (fwrite(&request, sizeof(Request), 1, file) != 1)
    {
        printStatusMessage("ERROR", "Failed to save request information.");
        fclose(file);
        pauseScreen();
        return;
    }

    fclose(file);

    printStatusMessage("SUCCESS", "Emergency request added successfully.");
    printf("\nRequest ID   : %d\n", request.requestId);
    printf("Tracking PIN : %s\n", request.trackingPIN);
    printf("Please save the Request ID and Tracking PIN for tracking and verification.\n");
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
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("All Emergency Requests");

    /* Read each request one by one from the binary file. */
    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        displayRequest(&request);
        found = 1;
    }

    if (!found)
    {
        printStatusMessage("INFO", "No request records found.");
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
        printStatusMessage("INFO", "No request records found yet.");
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
            printStatusMessage("SUCCESS", "Request found.");
            displayRequest(&request);
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("\n[INFO] No request found with ID %d.\n", searchId);
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
    int newStatus;
    int found = 0;

    sourceFile = fopen(REQUEST_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary request file.");
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

            printSectionHeader("Current Request Information");
            displayRequest(&request);

            printf("Enter new request status\n");
            printf("  0. Pending\n");
            printf("  1. Matched\n");
            printf("  2. Donation Done, Waiting Verification\n");
            printf("  3. Fulfilled / Verified\n");
            printf("Status choice: ");
            while (scanf("%d", &newStatus) != 1 ||
                   newStatus < REQUEST_PENDING ||
                   newStatus > REQUEST_FULFILLED)
            {
                printf("Invalid status. Enter 0, 1, 2, or 3: ");
                clearInputBuffer();
            }
            clearInputBuffer();
            request.requestStatus = newStatus;
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to update request status.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\n[INFO] No request found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after status update.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request status updated successfully.");
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
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary request file.");
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
            printStatusMessage("ERROR", "Failed to delete request information.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\n[INFO] No request found with ID %d.\n", searchId);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after deletion.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request deleted successfully.");
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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    getTextInput("\nEnter blood group to match: ", bloodGroup, sizeof(bloodGroup));

    printSectionHeader("Matched Available Donors");
    printf("%-18s : %s\n", "Blood Group Filter", bloodGroup);
    printf("%-18s : %s\n", "Availability Rule", "Only donors with status 1 are shown");
    printLine('-', 76);
    printf("%-8s %-22s %-10s %-16s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Status");
    printLine('-', 76);

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
        printf("[INFO] No available donors found for blood group %s.\n", bloodGroup);
    }

    printLine('=', 76);
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
        printStatusMessage("INFO", "No request records found yet.");
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
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    donorFile = fopen(DONOR_FILE, "rb");

    if (donorFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donor Match Result");
    printf("%-18s : %d\n", "Request ID", request.requestId);
    printf("%-18s : %s\n", "Patient Name", request.patientName);
    printf("%-18s : %s\n", "Blood Group Needed", request.bloodGroupNeeded);
    printf("%-18s : %d\n", "Units Needed", request.unitsNeeded);
    printf("%-18s : %s\n", "Urgency Level", request.urgencyLevel);
    printf("%-18s : %s\n", "Request Status", getRequestStatusText(request.requestStatus));
    printLine('-', 76);
    printf("%-8s %-22s %-10s %-16s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Status");
    printLine('-', 76);

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
        printf("[INFO] No available donors found for request ID %d.\n", request.requestId);
    }

    printLine('=', 76);
    fclose(donorFile);
    writeActivityLog("Matched donors by request ID.");
    pauseScreen();
}

void trackRequestByIdAndPin(void)
{
    FILE *file;
    Request request;
    int requestId;
    char pin[10];
    int idFound = 0;
    int verified = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Track Request");
    printf("Enter request ID: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter tracking PIN: ", pin, sizeof(pin));

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestId == requestId)
        {
            idFound = 1;

            if (strcmp(request.trackingPIN, pin) == 0)
            {
                printStatusMessage("SUCCESS", "Request found.");
                displayRequest(&request);
                verified = 1;
            }

            break;
        }
    }

    fclose(file);

    if (!idFound)
    {
        printf("\n[INFO] No request found with ID %d.\n", requestId);
    }
    else if (!verified)
    {
        printStatusMessage("ERROR", "Invalid tracking PIN for this request.");
    }

    pauseScreen();
}

void assignDonorToRequest(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    FILE *donorFile;
    Request request;
    Donor donor;
    int requestId;
    int donorId;
    int requestFound = 0;
    int donorFound = 0;
    int validDonor = 0;
    int assigned = 0;

    sourceFile = fopen(REQUEST_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary request file.");
        pauseScreen();
        return;
    }

    printf("\nEnter request ID to assign donor: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId)
        {
            requestFound = 1;

            printSectionHeader("Request For Assignment");
            displayRequest(&request);

            if (request.requestStatus != REQUEST_PENDING)
            {
                printStatusMessage("ERROR", "Only pending requests can be assigned a donor.");
            }
            else
            {
                donorFile = fopen(DONOR_FILE, "rb");

                if (donorFile == NULL)
                {
                    printStatusMessage("INFO", "No donor records found yet.");
                }
                else
                {
                    printSectionHeader("Available Matching Donors");
                    printf("%-8s %-22s %-10s %-16s %-12s\n",
                           "ID",
                           "Name",
                           "Group",
                           "Phone",
                           "Status");
                    printLine('-', 76);

                    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
                    {
                        if (strcmp(donor.bloodGroup, request.bloodGroupNeeded) == 0 &&
                            isDonorAvailable(&donor))
                        {
                            printMatchedDonorRow(&donor);
                        }
                    }

                    fclose(donorFile);
                }

                printf("\nEnter donor ID to assign: ");
                while (scanf("%d", &donorId) != 1)
                {
                    printf("Invalid input. Enter donor ID again: ");
                    clearInputBuffer();
                }
                clearInputBuffer();

                donorFile = fopen(DONOR_FILE, "rb");

                if (donorFile != NULL)
                {
                    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
                    {
                        if (donor.donorId == donorId)
                        {
                            donorFound = 1;

                            if (strcmp(donor.bloodGroup, request.bloodGroupNeeded) == 0 &&
                                isDonorAvailable(&donor))
                            {
                                validDonor = 1;
                            }

                            break;
                        }
                    }

                    fclose(donorFile);
                }

                if (validDonor)
                {
                    request.assignedDonorId = donorId;
                    request.requestStatus = REQUEST_MATCHED;
                    assigned = 1;
                }
                else if (!donorFound)
                {
                    printStatusMessage("ERROR", "Donor ID was not found.");
                }
                else
                {
                    printStatusMessage("ERROR", "Donor is unavailable or blood group does not match.");
                }
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to assign donor.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!requestFound)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    if (!assigned)
    {
        remove(TEMP_REQUEST_FILE);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after donor assignment.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor assigned successfully.");
    writeActivityLog("Assigned donor to emergency request.");
    pauseScreen();
}

int donationRecordExistsForRequest(int requestId)
{
    FILE *file;
    DonationRecord record;

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        if (record.requestId == requestId)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void createDonationRecord(const Request *request)
{
    FILE *file;
    DonationRecord record;
    char dateTime[30];

    if (donationRecordExistsForRequest(request->requestId))
    {
        return;
    }

    file = fopen(DONATION_FILE, "ab");

    if (file == NULL)
    {
        printStatusMessage("ERROR", "Unable to open donation file.");
        return;
    }

    record.donationId = generateNextDonationId();
    record.requestId = request->requestId;
    record.donorId = request->assignedDonorId;
    strcpy(record.bloodGroup, request->bloodGroupNeeded);
    record.unitsDonated = request->unitsNeeded;
    getCurrentDateTime(dateTime, sizeof(dateTime));
    strncpy(record.donationDate, dateTime, sizeof(record.donationDate) - 1);
    record.donationDate[sizeof(record.donationDate) - 1] = '\0';
    record.requesterConfirmed = 1;
    record.adminVerified = 0;

    fwrite(&record, sizeof(DonationRecord), 1, file);
    fclose(file);
}

void requesterConfirmDonation(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    Request confirmedRequest;
    int requestId;
    char pin[10];
    int requestFound = 0;
    int pinMatched = 0;
    int confirmed = 0;

    sourceFile = fopen(REQUEST_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        printStatusMessage("ERROR", "Unable to open temporary request file.");
        pauseScreen();
        return;
    }

    printSectionHeader("Requester Donation Confirmation");
    printf("Enter request ID: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter tracking PIN: ", pin, sizeof(pin));

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId)
        {
            requestFound = 1;

            if (strcmp(request.trackingPIN, pin) == 0)
            {
                pinMatched = 1;

                if (request.requestStatus == REQUEST_MATCHED && request.assignedDonorId != 0)
                {
                    request.requestStatus = REQUEST_WAITING_VERIFICATION;
                    request.requesterConfirmed = 1;
                    confirmedRequest = request;
                    confirmed = 1;
                }
                else
                {
                    printStatusMessage("ERROR", "Only matched requests with an assigned donor can be confirmed.");
                }
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to confirm donation.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!requestFound)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    if (!pinMatched)
    {
        remove(TEMP_REQUEST_FILE);
        printStatusMessage("ERROR", "Invalid tracking PIN for this request.");
        pauseScreen();
        return;
    }

    if (!confirmed)
    {
        remove(TEMP_REQUEST_FILE);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after confirmation.");
        pauseScreen();
        return;
    }

    createDonationRecord(&confirmedRequest);
    printStatusMessage("SUCCESS", "Donation confirmation submitted for admin verification.");
    writeActivityLog("Requester confirmed donation completion.");
    pauseScreen();
}

void viewDonationRecords(void)
{
    FILE *file;
    DonationRecord record;
    int found = 0;

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donation records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donation Records");

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        displayDonationRecord(&record);
        found = 1;
    }

    if (!found)
    {
        printStatusMessage("INFO", "No donation records found.");
    }

    fclose(file);
    pauseScreen();
}

void adminVerifyDonation(void)
{
    FILE *requestFile;
    FILE *tempRequestFile;
    FILE *donationFile;
    FILE *tempDonationFile;
    FILE *donorFile;
    FILE *tempDonorFile;
    Request request;
    DonationRecord record;
    Donor donor;
    int requestId;
    int requestFound = 0;
    int validRequest = 0;
    int donationUpdated = 0;
    int donorUpdated = 0;
    int donorId = 0;
    char dateTime[30];

    printSectionHeader("Admin Final Verification");
    printf("Enter request ID to verify: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    requestFile = fopen(REQUEST_FILE, "rb");

    if (requestFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, requestFile) == 1)
    {
        if (request.requestId == requestId)
        {
            requestFound = 1;
            donorId = request.assignedDonorId;

            if (request.requestStatus == REQUEST_WAITING_VERIFICATION &&
                donationRecordExistsForRequest(requestId))
            {
                validRequest = 1;
            }

            break;
        }
    }

    fclose(requestFile);

    if (!requestFound)
    {
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    if (!validRequest)
    {
        printStatusMessage("ERROR", "Request must be waiting for verification and have a donation record.");
        pauseScreen();
        return;
    }

    donationFile = fopen(DONATION_FILE, "rb");
    tempDonationFile = fopen(TEMP_DONATION_FILE, "wb");

    if (donationFile == NULL || tempDonationFile == NULL)
    {
        if (donationFile != NULL)
        {
            fclose(donationFile);
        }
        if (tempDonationFile != NULL)
        {
            fclose(tempDonationFile);
        }

        printStatusMessage("ERROR", "Unable to open donation files.");
        pauseScreen();
        return;
    }

    while (fread(&record, sizeof(DonationRecord), 1, donationFile) == 1)
    {
        if (record.requestId == requestId)
        {
            record.adminVerified = 1;
            donationUpdated = 1;
        }

        if (fwrite(&record, sizeof(DonationRecord), 1, tempDonationFile) != 1)
        {
            fclose(donationFile);
            fclose(tempDonationFile);
            remove(TEMP_DONATION_FILE);
            printStatusMessage("ERROR", "Failed to update donation record.");
            pauseScreen();
            return;
        }
    }

    fclose(donationFile);
    fclose(tempDonationFile);

    if (!donationUpdated)
    {
        remove(TEMP_DONATION_FILE);
        printStatusMessage("ERROR", "Matching donation record was not found.");
        pauseScreen();
        return;
    }

    if (remove(DONATION_FILE) != 0 || rename(TEMP_DONATION_FILE, DONATION_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donation file after verification.");
        pauseScreen();
        return;
    }

    requestFile = fopen(REQUEST_FILE, "rb");
    tempRequestFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (requestFile == NULL || tempRequestFile == NULL)
    {
        if (requestFile != NULL)
        {
            fclose(requestFile);
        }
        if (tempRequestFile != NULL)
        {
            fclose(tempRequestFile);
        }

        printStatusMessage("ERROR", "Unable to open request files.");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, requestFile) == 1)
    {
        if (request.requestId == requestId)
        {
            request.requestStatus = REQUEST_FULFILLED;
            request.adminVerified = 1;
        }

        if (fwrite(&request, sizeof(Request), 1, tempRequestFile) != 1)
        {
            fclose(requestFile);
            fclose(tempRequestFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to update request verification status.");
            pauseScreen();
            return;
        }
    }

    fclose(requestFile);
    fclose(tempRequestFile);

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after verification.");
        pauseScreen();
        return;
    }

    donorFile = fopen(DONOR_FILE, "rb");
    tempDonorFile = fopen(TEMP_DONOR_FILE, "wb");

    if (donorFile != NULL && tempDonorFile != NULL)
    {
        getCurrentDateTime(dateTime, sizeof(dateTime));

        while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
        {
            if (donor.donorId == donorId)
            {
                strcpy(donor.availabilityStatus, "0");
                strncpy(donor.lastDonationDate, dateTime, sizeof(donor.lastDonationDate) - 1);
                donor.lastDonationDate[sizeof(donor.lastDonationDate) - 1] = '\0';
                donor.donationCount++;
                donorUpdated = 1;
            }

            if (fwrite(&donor, sizeof(Donor), 1, tempDonorFile) != 1)
            {
                fclose(donorFile);
                fclose(tempDonorFile);
                remove(TEMP_DONOR_FILE);
                printStatusMessage("ERROR", "Failed to update donor after verification.");
                pauseScreen();
                return;
            }
        }

        fclose(donorFile);
        fclose(tempDonorFile);

        if (donorUpdated)
        {
            if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
            {
                printStatusMessage("ERROR", "Failed to replace donor file after verification.");
                pauseScreen();
                return;
            }
        }
        else
        {
            remove(TEMP_DONOR_FILE);
        }
    }
    else
    {
        if (donorFile != NULL)
        {
            fclose(donorFile);
        }
        if (tempDonorFile != NULL)
        {
            fclose(tempDonorFile);
        }
    }

    printStatusMessage("SUCCESS", "Donation verified and request fulfilled.");
    writeActivityLog("Admin verified donation and fulfilled request.");
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
        printStatusMessage("INFO", "No donor records found yet.");
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

    printSectionHeader("Donor Summary Report");
    printf("%-22s : %d\n", "Total Donors", totalDonors);
    printf("%-22s : %d\n", "Available Donors", availableDonors);
    printf("%-22s : %d\n", "Unavailable Donors", unavailableDonors);
    printLine('=', 63);

    writeActivityLog("Viewed donor summary report.");
    pauseScreen();
}

void viewPublicBloodGroupAvailability(void)
{
    FILE *file;
    Donor donor;
    int aPositive = 0;
    int aNegative = 0;
    int bPositive = 0;
    int bNegative = 0;
    int abPositive = 0;
    int abNegative = 0;
    int oPositive = 0;
    int oNegative = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (isDonorAvailable(&donor))
        {
            if (strcmp(donor.bloodGroup, "A+") == 0)
            {
                aPositive++;
            }
            else if (strcmp(donor.bloodGroup, "A-") == 0)
            {
                aNegative++;
            }
            else if (strcmp(donor.bloodGroup, "B+") == 0)
            {
                bPositive++;
            }
            else if (strcmp(donor.bloodGroup, "B-") == 0)
            {
                bNegative++;
            }
            else if (strcmp(donor.bloodGroup, "AB+") == 0)
            {
                abPositive++;
            }
            else if (strcmp(donor.bloodGroup, "AB-") == 0)
            {
                abNegative++;
            }
            else if (strcmp(donor.bloodGroup, "O+") == 0)
            {
                oPositive++;
            }
            else if (strcmp(donor.bloodGroup, "O-") == 0)
            {
                oNegative++;
            }
        }
    }

    fclose(file);

    printSectionHeader("Public Blood Group Availability");
    printf("%-15s : %d\n", "A+", aPositive);
    printf("%-15s : %d\n", "A-", aNegative);
    printf("%-15s : %d\n", "B+", bPositive);
    printf("%-15s : %d\n", "B-", bNegative);
    printf("%-15s : %d\n", "AB+", abPositive);
    printf("%-15s : %d\n", "AB-", abNegative);
    printf("%-15s : %d\n", "O+", oPositive);
    printf("%-15s : %d\n", "O-", oNegative);
    printLine('=', 63);

    writeActivityLog("Guest viewed public blood group availability.");
    pauseScreen();
}

void showRequestSummary(void)
{
    FILE *file;
    Request request;
    int totalRequests = 0;
    int pendingRequests = 0;
    int matchedRequests = 0;
    int waitingRequests = 0;
    int fulfilledRequests = 0;
    int otherRequests = 0;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        totalRequests++;

        if (request.requestStatus == REQUEST_PENDING)
        {
            pendingRequests++;
        }
        else if (request.requestStatus == REQUEST_MATCHED)
        {
            matchedRequests++;
        }
        else if (request.requestStatus == REQUEST_WAITING_VERIFICATION)
        {
            waitingRequests++;
        }
        else if (request.requestStatus == REQUEST_FULFILLED)
        {
            fulfilledRequests++;
        }
        else
        {
            otherRequests++;
        }
    }

    fclose(file);

    printSectionHeader("Request Summary Report");
    printf("%-22s : %d\n", "Total Requests", totalRequests);
    printf("%-22s : %d\n", "Pending Requests", pendingRequests);
    printf("%-22s : %d\n", "Matched Requests", matchedRequests);
    printf("%-22s : %d\n", "Waiting Verification", waitingRequests);
    printf("%-22s : %d\n", "Fulfilled Requests", fulfilledRequests);
    printf("%-22s : %d\n", "Other Status", otherRequests);
    printLine('=', 63);

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
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    reportFile = fopen(DONOR_REPORT_FILE, "w");

    if (reportFile == NULL)
    {
        fclose(dataFile);
        printStatusMessage("ERROR", "Unable to create donor report file.");
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
        fprintf(reportFile, "Availability       : %s\n", getAvailabilityText(&donor));
        fprintf(reportFile, "Donation Count     : %d\n", donor.donationCount);
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Donors       : %d\n", totalDonors);
    fprintf(reportFile, "Available Donors   : %d\n", availableDonors);
    fprintf(reportFile, "Unavailable Donors : %d\n", unavailableDonors);

    fclose(dataFile);
    fclose(reportFile);

    printf("\n[SUCCESS] Donor report exported to %s\n", DONOR_REPORT_FILE);
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
    int matchedRequests = 0;
    int waitingRequests = 0;
    int fulfilledRequests = 0;
    int otherRequests = 0;
    char dateTime[30];

    dataFile = fopen(REQUEST_FILE, "rb");

    if (dataFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    reportFile = fopen(REQUEST_REPORT_FILE, "w");

    if (reportFile == NULL)
    {
        fclose(dataFile);
        printStatusMessage("ERROR", "Unable to create request report file.");
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

        if (request.requestStatus == REQUEST_PENDING)
        {
            pendingRequests++;
        }
        else if (request.requestStatus == REQUEST_MATCHED)
        {
            matchedRequests++;
        }
        else if (request.requestStatus == REQUEST_WAITING_VERIFICATION)
        {
            waitingRequests++;
        }
        else if (request.requestStatus == REQUEST_FULFILLED)
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
        fprintf(reportFile, "Tracking PIN       : %s\n", request.trackingPIN);
        fprintf(reportFile, "Assigned Donor ID  : %d\n", request.assignedDonorId);
        fprintf(reportFile, "Requester Confirmed: %s\n", request.requesterConfirmed ? "Yes" : "No");
        fprintf(reportFile, "Admin Verified     : %s\n", request.adminVerified ? "Yes" : "No");
        fprintf(reportFile, "Request Status     : %s\n", getRequestStatusText(request.requestStatus));
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Requests     : %d\n", totalRequests);
    fprintf(reportFile, "Pending Requests   : %d\n", pendingRequests);
    fprintf(reportFile, "Matched Requests   : %d\n", matchedRequests);
    fprintf(reportFile, "Waiting Verify     : %d\n", waitingRequests);
    fprintf(reportFile, "Fulfilled Requests : %d\n", fulfilledRequests);
    fprintf(reportFile, "Other Status       : %d\n", otherRequests);

    fclose(dataFile);
    fclose(reportFile);

    printf("\n[SUCCESS] Request report exported to %s\n", REQUEST_REPORT_FILE);
    writeActivityLog("Exported request report to TXT.");
    pauseScreen();
}

void exportDonationReportToTXT(void)
{
    FILE *dataFile;
    FILE *reportFile;
    DonationRecord record;
    int totalRecords = 0;
    int requesterConfirmed = 0;
    int adminVerified = 0;
    char dateTime[30];

    dataFile = fopen(DONATION_FILE, "rb");

    if (dataFile == NULL)
    {
        printStatusMessage("INFO", "No donation records found yet.");
        pauseScreen();
        return;
    }

    reportFile = fopen(DONATION_REPORT_FILE, "w");

    if (reportFile == NULL)
    {
        fclose(dataFile);
        printStatusMessage("ERROR", "Unable to create donation report file.");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    fprintf(reportFile, "NSU Blood Donor Management System\n");
    fprintf(reportFile, "Donation Report\n");
    fprintf(reportFile, "Generated On: %s\n", dateTime);
    fprintf(reportFile, "============================================================\n\n");

    while (fread(&record, sizeof(DonationRecord), 1, dataFile) == 1)
    {
        totalRecords++;

        if (record.requesterConfirmed)
        {
            requesterConfirmed++;
        }

        if (record.adminVerified)
        {
            adminVerified++;
        }

        fprintf(reportFile, "Donation ID         : %d\n", record.donationId);
        fprintf(reportFile, "Request ID          : %d\n", record.requestId);
        fprintf(reportFile, "Donor ID            : %d\n", record.donorId);
        fprintf(reportFile, "Blood Group         : %s\n", record.bloodGroup);
        fprintf(reportFile, "Units Donated       : %d\n", record.unitsDonated);
        fprintf(reportFile, "Donation Date       : %s\n", record.donationDate);
        fprintf(reportFile, "Requester Confirmed : %s\n", record.requesterConfirmed ? "Yes" : "No");
        fprintf(reportFile, "Admin Verified      : %s\n", record.adminVerified ? "Yes" : "No");
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Records       : %d\n", totalRecords);
    fprintf(reportFile, "Requester Confirmed : %d\n", requesterConfirmed);
    fprintf(reportFile, "Admin Verified      : %d\n", adminVerified);

    fclose(dataFile);
    fclose(reportFile);

    printf("\n[SUCCESS] Donation report exported to %s\n", DONATION_REPORT_FILE);
    writeActivityLog("Exported donation report to TXT.");
    pauseScreen();
}

void viewActivityLog(void)
{
    FILE *logFile;
    char line[200];

    logFile = fopen(ACTIVITY_LOG_FILE, "r");

    if (logFile == NULL)
    {
        printStatusMessage("INFO", "No activity log found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Activity Log");

    while (fgets(line, sizeof(line), logFile) != NULL)
    {
        printf("%s", line);
    }

    printLine('=', 63);
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
