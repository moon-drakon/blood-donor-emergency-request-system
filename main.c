#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define DONOR_FILE "donors.dat"
#define TEMP_DONOR_FILE "temp_donors.dat"
#define REQUEST_FILE "requests.dat"
#define TEMP_REQUEST_FILE "temp_requests.dat"
#define ASSIGNMENT_FILE "assignments.dat"
#define TEMP_ASSIGNMENT_FILE "temp_assignments.dat"
#define ADMIN_FILE "admins.dat"
#define TEMP_ADMIN_FILE "temp_admins.dat"
#define DONATION_FILE "donations.dat"
#define DONOR_REPORT_FILE "donor_report.txt"
#define REQUEST_REPORT_FILE "request_report.txt"
#define DONATION_REPORT_FILE "donation_report.txt"
#define ACTIVITY_LOG_FILE "activity_log.txt"

#define REQUEST_PENDING 0
#define REQUEST_DONOR_INTERESTED 1
#define REQUEST_PARTIALLY_MATCHED 2
#define REQUEST_FULLY_MATCHED 3
#define REQUEST_PARTIALLY_FULFILLED 4
#define REQUEST_FULFILLED 5
#define REQUEST_CANCELLED 6

#define ASSIGNMENT_INTERESTED 0
#define ASSIGNMENT_ASSIGNED 1
#define ASSIGNMENT_WAITING_VERIFICATION 2
#define ASSIGNMENT_VERIFIED 3
#define ASSIGNMENT_REPLACED 4
#define ASSIGNMENT_CANCELLED 5

#define DONOR_PENDING_APPROVAL 0
#define DONOR_APPROVED 1
#define DONOR_REJECTED 2
#define DONOR_BLOCKED 3

typedef struct
{
    int donorId;
    char name[50];
    int age;
    char gender[10];
    char bloodGroup[5];
    char phone[20];
    char email[50];
    char address[100];
    char lastDonationDate[20];
    char medicalNote[100];
    char securityQuestion[100];
    char securityAnswer[50];
    char availabilityStatus[20];
    char password[30];
    int donationCount;
    int approvalStatus;
} Donor;

typedef struct
{
    int requestId;
    char requesterName[50];
    char patientName[50];
    char bloodGroupNeeded[5];
    int requiredBags;
    int verifiedBags;
    char hospitalName[60];
    char location[100];
    char contactNumber[20];
    char urgencyLevel[20];
    char trackingPIN[10];
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
    char donorName[50];
    char requesterName[50];
    char patientName[50];
    char bloodGroup[5];
    int unitsDonated;
    char donationDate[20];
    int requesterConfirmed;
    int adminVerified;
} DonationRecord;

typedef struct
{
    int assignmentId;
    int requestId;
    int donorId;
    int status;
    int bags;
    int requesterConfirmed;
    int adminVerified;
    char interestedDate[20];
    char assignedDate[20];
    char donationDoneDate[20];
    char verifiedDate[20];
} DonorAssignment;

char currentActivityActor[20] = "SYSTEM";
int currentActivityActorId = 0;

void showAccessMenu(void);
void showAdminMenu(void);
void showAdminDonorMenu(void);
void showRequestMenu(void);
void showAssignmentMenu(void);
void showReportMenu(void);
void showGuestMenu(void);
void adminMenu(int adminId);
void adminDonorMenu(void);
void assignmentMenu(void);
void donorMenu(int donorId);
void requestMenu(void);
void reportMenu(void);
void requesterAccess(void);
void requesterMenu(int requestId, const char *trackingPIN);
void viewRequesterRequest(int requestId, const char *trackingPIN);
void updateRequiredBagsByRequester(int requestId, const char *trackingPIN);
void viewAssignedDonorForRequest(int requestId, const char *trackingPIN);
int printRequesterConfirmableAssignments(int requestId);
void confirmRequesterDonationByCredentials(int requestId, const char *trackingPIN);
void cancelRequesterRequest(int requestId, const char *trackingPIN);
void guestMenu(void);
void guestSearchDonorAvailabilityByBloodGroup(void);
void viewEmergencyBloodRequestsPublic(void);
void clearInputBuffer(void);
int getMenuChoice(void);
int readIntOrBack(const char *prompt, int *value);
int getTextInputOrBack(const char *prompt, char *buffer, int size);
int inputBloodGroupOrBack(const char *prompt, char *bloodGroup, int size);
int readIntInRangeOrBack(const char *prompt, int *value, int min, int max);
int inputAgeOrBack(int *age);
int inputRequiredBagsOrBack(int *bags);
int inputPhoneOrBack(const char *prompt, char *phone, int size);
int inputEmailOrBack(const char *prompt, char *email, int size);
int inputDateOrBack(const char *prompt, char *dateText, int size);
int inputAvailabilityOrBack(const char *prompt, char *availability, int size);
int inputPasswordOrBack(const char *prompt, char *password, int size);
void pauseScreen(void);
void getTextInput(const char *prompt, char *buffer, int size);
void printLine(char symbol, int count);
void printSectionHeader(const char *title);
void printStatusMessage(const char *type, const char *message);
int generateNextDonorId(void);
int generateNextRequestId(void);
int generateNextDonationId(void);
int generateNextAssignmentId(void);
int isDonorAvailable(const Donor *donor);
int isDonorApproved(const Donor *donor);
int isDonorEligibleForAssignment(const Donor *donor);
int isValidBloodGroup(const char *bloodGroup);
int isValidPhone(const char *phone);
int isValidEmail(const char *email);
int isValidDateText(const char *dateText);
int isValidAvailabilityInput(const char *availability);
int isValidPassword(const char *password);
void inputBloodGroup(const char *prompt, char *bloodGroup, int size);
int isCompatible(const char donorBG[], const char requestBG[]);
int isValidUrgencyChoice(int urgency);
const char *getUrgencyText(const char *urgencyLevel);
int isDuplicateDonorPhoneOrEmail(const char *phone, const char *email, int ignoreDonorId);
void printMatchedDonorRow(const Donor *donor);
int printDonorSelectionList(const char *title, int approvalStatusFilter);
void printBloodGroupOptions(void);
void printDonorAreaOptions(void);
void getCurrentDateTime(char *buffer, int size);
const char *getRequestStatusText(int status);
const char *getRequestStatusShortText(int status);
const char *getAvailabilityText(const Donor *donor);
const char *getDonorApprovalStatusText(int status);
const char *getAssignmentStatusText(int status);
const char *getAssignmentStatusShortText(int status);
void generateRequestPin(char *pin, int size);
void displayDonor(const Donor *donor);
void displayRequest(const Request *request);
int printRequestSelectionList(const char *title, int includeClosedRequests);
int printRequestsWithInterestedDonors(const char *title);
void displayDonationRecord(const DonationRecord *record);
void displayAssignment(const DonorAssignment *assignment);
int printAssignmentSelectionList(const char *title, int statusFilter);
int findDonorById(int donorId, Donor *foundDonor);
int findRequestById(int requestId, Request *foundRequest);
int saveDonationRecordForAssignment(const Request *request, const Donor *donor, int bags, int requesterVerified, int adminVerified);
int updateDonorAfterVerifiedDonation(int donorId);

void initializeDefaultAdmin(void);
int adminLogin(void);
void changeAdminPassword(int adminId);
void adminResetDonorPassword(void);
void viewDonorLoginInfoForAdmin(void);
int donorLogin(void);
void donorSelfRegistration(void);
void forgotPassword(void);
void viewOwnDonorProfile(int donorId);
void updateOwnDonorProfile(int donorId);
void changeDonorPassword(int donorId);
void viewMatchingBloodRequests(int donorId);
int printMatchingRequestSelectionForDonor(int donorId, const char *title);
void showInterestInRequest(int donorId);
void viewMyInterestedRequests(int donorId);
void viewMyAssignedRequests(int donorId);
int printDonorAssignmentSelectionList(int donorId, const char *title, int statusFilter);
void markDonationDone(int donorId);

void addDonor(void);
void viewAllDonors(void);
void searchDonorById(void);
void searchDonorByBloodGroupAdmin(void);
void searchDonorByArea(void);
void searchDonorByApprovalStatus(void);
int updateDonorApprovalStatusById(int donorId, int newStatus);
void updateDonor(void);
void deleteDonor(void);
void changeDonorAvailability(void);
void viewPendingDonorRegistrations(void);
void approveOrRejectDonorRegistration(void);

void addRequest(void);
void viewAllRequests(void);
void searchRequestById(void);
void updateRequestStatus(void);
void deleteRequest(void);
void cancelRequestByAdmin(void);
void matchDonorsByBloodGroup(void);
void matchDonorsByRequestId(void);
void trackRequestByIdAndPin(void);
int printMatchingDonorsForRequest(const Request *request);
int printMatchingRequestsForDonor(const Donor *donor);
int assignDonorToRequestByIds(int requestId, int donorId);
void offerAssignmentAfterMatch(int requestId);
void assignDonorToRequest(void);
void viewAllAssignments(void);
void viewAssignmentsByRequest(void);
void viewInterestedDonors(void);
int updateInterestedAssignmentStatus(int assignmentId, int requestId, int newStatus);
void assignMultipleDonorsToRequest(void);
void changeAssignedDonor(void);
void addBackupDonor(void);
void verifyDonationByAssignment(void);

void showDonorSummary(void);
void showRequestSummary(void);
void viewCriticalRequests(void);
void viewHighPriorityRequests(void);
void searchRequestByUrgency(void);
void searchRequestByStatus(void);
void exportDonorReportToTXT(void);
void exportRequestReportToTXT(void);
void exportDonationReport(void);
void viewDonationRecords(void);
void searchDonationHistoryByDonorId(void);
void searchDonationHistoryByRequestId(void);
void viewMyDonationHistory(int donorId);
void viewMyDonationCount(int donorId);
void viewPublicBloodGroupAvailability(void);
void viewActivityLog(void);
void setActivityActor(const char *actorType, int actorId);
void clearActivityActor(void);
void writeActivityLog(const char *action);
void initializeDemoDataIfEmpty(void);
void topUpDemoDataForFinalSetup(void);
int isDonorAssignedToActiveRequest(int donorId, int currentRequestId);
int donorInterestExists(int requestId, int donorId);
int createOrUpdateAssignment(int requestId, int donorId, int status);
int countRequestAssignmentsByStatus(int requestId, int status);
int countRequestActiveAssigned(int requestId);
int updateRequestStatusAfterAssignment(int requestId);
int updateRequestStatusAfterVerification(int requestId);
int cancelAssignmentsForRequest(int requestId);
int cancelAssignmentsForDonor(int donorId);

int main(void)
{
    int choice;
    int donorId;
    int adminId;

    initializeDefaultAdmin();
    initializeDemoDataIfEmpty();
    topUpDemoDataForFinalSetup();
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
                writeActivityLog("Logged out.");
                clearActivityActor();
            }
            break;
        case 2:
            donorId = donorLogin();
            if (donorId != 0)
            {
                donorMenu(donorId);
                writeActivityLog("Logged out.");
                clearActivityActor();
            }
            break;
        case 3:
            setActivityActor("GUEST", 0);
            donorSelfRegistration();
            clearActivityActor();
            break;
        case 4:
            setActivityActor("GUEST", 0);
            guestMenu();
            clearActivityActor();
            break;
        case 5:
            requesterAccess();
            break;
        case 6:
            forgotPassword();
            break;
        case 7:
            printStatusMessage("SUCCESS", "Thank you for using the system.");
            printf("Program closed successfully.\n");
            return 0;
        default:
            printStatusMessage("ERROR", "Please choose a valid menu option.");
            pauseScreen();
        }
    }
}

void showAccessMenu(void)
{
    printf("\n");
    printSectionHeader("NSU Blood Donor System - Access Menu");
    /* Donor Login is for approved donors who already have a Donor ID and Password. */
    printf("  1. Administrator Login\n");
    printf("  2. Donor Login\n");
    /* Donor Registration is for new donors who want to create an account. */
    printf("  3. Donor Registration\n");
    printf("  4. Guest Blood Request Services\n");
    printf("  5. Requester Access / Track Request\n");
    printf("  6. Forgot Password\n");
    printf("  7. Exit\n");
    printLine('=', 72);
}

void showAdminMenu(void)
{
    printf("\n");
    printSectionHeader("Admin Menu");
    printf("  1. Dashboard\n");
    printf("  2. Donor Management\n");
    printf("  3. Request Management\n");
    printf("  4. Assignment Management\n");
    printf("  5. Reports and Activity Log\n");
    printf("  6. Change Admin Password\n");
    printf("  0. Logout\n");
    printLine('=', 72);
}

void showAdminDonorMenu(void)
{
    printSectionHeader("Admin Donor Management");
    printf("  1. Add Donor\n");
    printf("  2. View All Donors\n");
    printf("  3. Search Donor by ID\n");
    printf("  4. Update Donor\n");
    printf("  5. Delete Donor\n");
    printf("  6. Change Donor Availability\n");
    printf("  7. View Pending Donor Registrations\n");
    printf("  8. Manage Donor Approval Status\n");
    printf("  9. Search Donor by Blood Group\n");
    printf(" 10. Search Donor by Area\n");
    printf(" 11. Search Donor by Approval Status\n");
    printf(" 12. Reset / Change Donor Password\n");
    printf(" 13. View Donor Login Info\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 72);
}

void showRequestMenu(void)
{
    printSectionHeader("Admin Request Management");
    printf("  1. Add Emergency Request\n");
    printf("  2. View All Requests\n");
    printf("  3. Search Request by ID\n");
    printf("  4. Update Request Status\n");
    printf("  5. Delete Request\n");
    printf("  6. Cancel Request\n");
    printf("  7. View Critical Requests\n");
    printf("  8. View High Priority Requests\n");
    printf("  9. Search Request by Urgency\n");
    printf(" 10. Search Request by Status\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 72);
}

void showAssignmentMenu(void)
{
    printSectionHeader("Admin Assignment Management");
    printf("  1. Match Donors by Blood Group\n");
    printf("  2. Match Donors by Request ID\n");
    printf("  3. View All Assignments\n");
    printf("  4. View Assignments by Request\n");
    printf("  5. View Interested Donors\n");
    printf("  6. Assign Donor\n");
    printf("  7. Assign Multiple Donors\n");
    printf("  8. Change Assigned Donor\n");
    printf("  9. Add Backup Donor\n");
    printf(" 10. Verify Donation\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 72);
}

void showReportMenu(void)
{
    printSectionHeader("Reports, History, and Activity Log");
    printf("  1. Show Donor Summary\n");
    printf("  2. Show Request Summary\n");
    printf("  3. Export Donor Report to TXT\n");
    printf("  4. Export Request Report to TXT\n");
    printf("  5. Export Donation Report to TXT\n");
    printf("  6. View All Donation History\n");
    printf("  7. Search Donation History by Donor ID\n");
    printf("  8. Search Donation History by Request ID\n");
    printf("  9. View Activity Log\n");
    printf("  0. Back to Main Menu\n");
    printLine('-', 72);
}

void showGuestMenu(void)
{
    printSectionHeader("Guest Blood Request Services");
    printf("  1. Create Emergency Blood Request\n");
    printf("  2. Search Donor by Blood Group\n");
    printf("  3. View Public Blood Group Availability\n");
    printf("  4. View Open Emergency Requests\n");
    printf("  5. Track Request Status\n");
    printf("  6. Back\n");
    printLine('-', 72);
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
            showDonorSummary();
            showRequestSummary();
            break;
        case 2:
            adminDonorMenu();
            break;
        case 3:
            requestMenu();
            break;
        case 4:
            assignmentMenu();
            break;
        case 5:
            reportMenu();
            break;
        case 6:
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
        showAdminDonorMenu();
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
        case 7:
            viewPendingDonorRegistrations();
            break;
        case 8:
            approveOrRejectDonorRegistration();
            break;
        case 9:
            searchDonorByBloodGroupAdmin();
            break;
        case 10:
            searchDonorByArea();
            break;
        case 11:
            searchDonorByApprovalStatus();
            break;
        case 12:
            adminResetDonorPassword();
            break;
        case 13:
            viewDonorLoginInfoForAdmin();
            break;
        case 0:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void assignmentMenu(void)
{
    int choice;

    while (1)
    {
        showAssignmentMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            matchDonorsByBloodGroup();
            break;
        case 2:
            matchDonorsByRequestId();
            break;
        case 3:
            viewAllAssignments();
            break;
        case 4:
            viewAssignmentsByRequest();
            break;
        case 5:
            viewInterestedDonors();
            break;
        case 6:
            assignDonorToRequest();
            break;
        case 7:
            assignMultipleDonorsToRequest();
            break;
        case 8:
            changeAssignedDonor();
            break;
        case 9:
            addBackupDonor();
            break;
        case 10:
            verifyDonationByAssignment();
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
            cancelRequestByAdmin();
            break;
        case 7:
            viewCriticalRequests();
            break;
        case 8:
            viewHighPriorityRequests();
            break;
        case 9:
            searchRequestByUrgency();
            break;
        case 10:
            searchRequestByStatus();
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
            exportDonationReport();
            break;
        case 6:
            viewDonationRecords();
            break;
        case 7:
            searchDonationHistoryByDonorId();
            break;
        case 8:
            searchDonationHistoryByRequestId();
            break;
        case 9:
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
            guestSearchDonorAvailabilityByBloodGroup();
            break;
        case 3:
            viewPublicBloodGroupAvailability();
            break;
        case 4:
            viewEmergencyBloodRequestsPublic();
            break;
        case 5:
            trackRequestByIdAndPin();
            break;
        case 6:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void viewEmergencyBloodRequestsPublic(void)
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

    printSectionHeader("Open Emergency Blood Requests");
    printf("%-8s %-18s %-8s %-8s %-8s %-15s %-18s\n",
           "Req ID", "Patient", "Group", "Need", "Done", "Urgency", "Status");
    printLine('-', 92);

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestStatus != REQUEST_FULFILLED &&
            request.requestStatus != REQUEST_CANCELLED)
        {
            printf("%-8d %-18s %-8s %-8d %-8d %-15s %-18s\n",
                   request.requestId,
                   request.patientName,
                   request.bloodGroupNeeded,
                   request.requiredBags,
                   request.verifiedBags,
                   getUrgencyText(request.urgencyLevel),
                   getRequestStatusText(request.requestStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No open emergency blood requests right now.");
    }

    pauseScreen();
}

void guestSearchDonorAvailabilityByBloodGroup(void)
{
    FILE *donorFile;
    Donor donor;
    char bloodGroup[5];
    int found = 0;
    int count = 0;

    donorFile = fopen(DONOR_FILE, "rb");

    if (donorFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    inputBloodGroup("\nEnter required blood group: ", bloodGroup, sizeof(bloodGroup));

    printSectionHeader("Public Donor Availability");
    printf("Guest privacy mode: names, passwords, full phone numbers, and history are hidden.\n");
    printf("%-15s : %s\n", "Needed Group", bloodGroup);
    printLine('-', 72);
    printf("%-15s %-30s %-15s\n", "Blood Group", "Area", "Status");
    printLine('-', 72);

    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
    {
        if (isCompatible(donor.bloodGroup, bloodGroup) && isDonorEligibleForAssignment(&donor))
        {
            printf("%-15s %-30s %-15s\n",
                   donor.bloodGroup,
                   donor.address,
                   "Available");
            found = 1;
            count++;
        }
    }

    fclose(donorFile);

    printLine('-', 72);
    printf("%-15s : %d\n", "Total Found", count);

    if (!found)
    {
        printStatusMessage("INFO", "No public donor availability found for this blood group.");
    }

    writeActivityLog("Guest searched public donor availability by blood group.");
    pauseScreen();
}

void requesterAccess(void)
{
    FILE *file;
    Request request;
    int requestId;
    char trackingPIN[10];
    int idFound = 0;
    int pinMatched = 0;

    printSectionHeader("Requester Access");

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    printf("Enter Request ID: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter Request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    getTextInput("Enter Tracking PIN: ", trackingPIN, sizeof(trackingPIN));

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestId == requestId)
        {
            idFound = 1;

            if (strcmp(request.trackingPIN, trackingPIN) == 0)
            {
                pinMatched = 1;
            }

            break;
        }
    }

    fclose(file);

    if (!idFound)
    {
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    if (!pinMatched)
    {
        printStatusMessage("ERROR", "Request ID found, but the Tracking PIN does not match.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request ID and Tracking PIN verified.");
    setActivityActor("REQUESTER REQUEST", requestId);
    writeActivityLog("Logged in.");
    pauseScreen();
    requesterMenu(requestId, trackingPIN);
    writeActivityLog("Logged out.");
    clearActivityActor();
}

void requesterMenu(int requestId, const char *trackingPIN)
{
    int choice;

    while (1)
    {
        printSectionHeader("Requester Menu");
        printf("  1. Create Another Blood Request\n");
        printf("  2. View This Request\n");
        printf("  3. View Assigned Donor Details\n");
        printf("  4. Increase Required Blood Bags\n");
        printf("  5. Confirm Completed Donation\n");
        printf("  6. Cancel This Request\n");
        printf("  7. Logout\n");
        printLine('-', 72);

        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            addRequest();
            break;
        case 2:
            viewRequesterRequest(requestId, trackingPIN);
            break;
        case 3:
            viewAssignedDonorForRequest(requestId, trackingPIN);
            break;
        case 4:
            updateRequiredBagsByRequester(requestId, trackingPIN);
            break;
        case 5:
            confirmRequesterDonationByCredentials(requestId, trackingPIN);
            break;
        case 6:
            cancelRequesterRequest(requestId, trackingPIN);
            break;
        case 7:
            return;
        default:
            printStatusMessage("ERROR", "Invalid menu choice. Please try again.");
            pauseScreen();
        }
    }
}

void viewRequesterRequest(int requestId, const char *trackingPIN)
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

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestId == requestId &&
            strcmp(request.trackingPIN, trackingPIN) == 0)
        {
            printStatusMessage("SUCCESS", "Request found.");
            displayRequest(&request);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("ERROR", "Unable to verify this request.");
    }

    pauseScreen();
}

void updateRequiredBagsByRequester(int requestId, const char *trackingPIN)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int found = 0;
    int updated = 0;
    int currentBags;
    int newBags;
    int minimumAllowed;

    printSectionHeader("Increase Required Blood Bags");
    printf("Enter 0 to go back. Required bags can only be increased.\n");

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

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId &&
            strcmp(request.trackingPIN, trackingPIN) == 0)
        {
            found = 1;
            currentBags = request.requiredBags;
            minimumAllowed = currentBags + 1;

            if (request.verifiedBags >= minimumAllowed)
            {
                minimumAllowed = request.verifiedBags + 1;
            }

            displayRequest(&request);

            if (request.requestStatus == REQUEST_CANCELLED ||
                request.requestStatus == REQUEST_FULFILLED)
            {
                printStatusMessage("ERROR", "Fulfilled or cancelled requests cannot be updated.");
            }
            else if (minimumAllowed > 20)
            {
                printStatusMessage("ERROR", "Required bags cannot be increased without exceeding the maximum of 20.");
            }
            else if (readIntInRangeOrBack("Enter new required blood bags: ",
                                          &newBags,
                                          minimumAllowed,
                                          20))
            {
                request.requiredBags = newBags;
                updated = 1;
            }
            else
            {
                fclose(sourceFile);
                fclose(tempFile);
                remove(TEMP_REQUEST_FILE);
                printStatusMessage("INFO", "Blood bag increase cancelled. No information was saved.");
                pauseScreen();
                return;
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to update required blood bags.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printStatusMessage("ERROR", "Unable to verify this request.");
        pauseScreen();
        return;
    }

    if (!updated)
    {
        remove(TEMP_REQUEST_FILE);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save required blood bag update.");
        pauseScreen();
        return;
    }

    if (countRequestAssignmentsByStatus(requestId, ASSIGNMENT_VERIFIED) > 0)
    {
        updateRequestStatusAfterVerification(requestId);
    }
    else
    {
        updateRequestStatusAfterAssignment(requestId);
    }

    printStatusMessage("SUCCESS", "Required blood bags increased successfully.");
    writeActivityLog("Requester increased required blood bags.");
    pauseScreen();
}

void viewAssignedDonorForRequest(int requestId, const char *trackingPIN)
{
    FILE *requestFile;
    FILE *assignmentFile;
    FILE *donorFile;
    Request request;
    DonorAssignment assignment;
    Donor donor;
    int requestFound = 0;
    int donorFound = 0;

    requestFile = fopen(REQUEST_FILE, "rb");

    if (requestFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, requestFile) == 1)
    {
        if (request.requestId == requestId &&
            strcmp(request.trackingPIN, trackingPIN) == 0)
        {
            requestFound = 1;
            break;
        }
    }

    fclose(requestFile);

    if (!requestFound)
    {
        printStatusMessage("ERROR", "Unable to verify this request.");
        pauseScreen();
        return;
    }

    assignmentFile = fopen(ASSIGNMENT_FILE, "rb");

    if (assignmentFile == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Assigned Donors for Request");

    while (fread(&assignment, sizeof(DonorAssignment), 1, assignmentFile) == 1)
    {
        if (assignment.requestId == request.requestId &&
            (assignment.status == ASSIGNMENT_ASSIGNED ||
             assignment.status == ASSIGNMENT_WAITING_VERIFICATION ||
             assignment.status == ASSIGNMENT_VERIFIED))
        {
            displayAssignment(&assignment);
            printf("%-24s : %d\n", "Request Required Bags", request.requiredBags);
            printf("%-24s : %d\n", "Request Verified Bags", request.verifiedBags);
            donorFile = fopen(DONOR_FILE, "rb");
            if (donorFile != NULL)
            {
                while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
                {
                    if (donor.donorId == assignment.donorId)
                    {
                        printf("%-24s : %s\n", "Name", donor.name);
                        printf("%-24s : %s\n", "Blood Group", donor.bloodGroup);
                        printf("%-24s : %s\n", "Phone", donor.phone);
                        printf("%-24s : %s\n", "Area", donor.address);
                        printLine('-', 72);
                        donorFound = 1;
                        break;
                    }
                }
                fclose(donorFile);
            }
        }
    }

    fclose(assignmentFile);

    if (!donorFound)
    {
        printStatusMessage("INFO", "No assigned donor records were found.");
    }

    pauseScreen();
}

int printRequesterConfirmableAssignments(int requestId)
{
    FILE *assignmentFile;
    DonorAssignment assignment;
    Donor donor;
    int found = 0;

    assignmentFile = fopen(ASSIGNMENT_FILE, "rb");

    if (assignmentFile == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        return 0;
    }

    printSectionHeader("Assignments Ready For Confirmation");
    printf("%-8s %-8s %-18s %-6s %-15s %-14s %-10s %-8s\n",
           "Assign",
           "Donor",
           "Donor Name",
           "Bags",
           "Phone",
           "Status",
           "Requester",
           "Admin");
    printLine('-', 95);

    while (fread(&assignment, sizeof(DonorAssignment), 1, assignmentFile) == 1)
    {
        if (assignment.requestId == requestId &&
            (assignment.status == ASSIGNMENT_WAITING_VERIFICATION ||
             assignment.status == ASSIGNMENT_VERIFIED))
        {
            if (!findDonorById(assignment.donorId, &donor))
            {
                memset(&donor, 0, sizeof(Donor));
                strcpy(donor.name, "Unknown");
                strcpy(donor.phone, "-");
            }

            printf("%-8d %-8d %-18s %-6d %-15s %-14s %-10s %-8s\n",
                   assignment.assignmentId,
                   assignment.donorId,
                   donor.name,
                   assignment.bags > 0 ? assignment.bags : 1,
                   donor.phone,
                   getAssignmentStatusShortText(assignment.status),
                   assignment.requesterConfirmed ? "Yes" : "No",
                   assignment.adminVerified ? "Yes" : "No");
            found = 1;
        }
    }

    fclose(assignmentFile);

    if (!found)
    {
        printStatusMessage("INFO", "No donation-done assignments are ready for requester confirmation.");
    }

    return found;
}

void confirmRequesterDonationByCredentials(int requestId, const char *trackingPIN)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    DonorAssignment assignment;
    DonorAssignment finalAssignment;
    Donor donor;
    int assignmentId;
    int found = 0;
    int confirmed = 0;
    int finalized = 0;
    char dateTime[30];

    printSectionHeader("Requester Confirm Donation");

    if (!findRequestById(requestId, &request) ||
        strcmp(request.trackingPIN, trackingPIN) != 0)
    {
        printStatusMessage("ERROR", "Unable to verify this requester session.");
        pauseScreen();
        return;
    }

    printf("%-24s : %d\n", "Request ID", request.requestId);
    printf("%-24s : %s\n", "Patient Name", request.patientName);
    printf("%-24s : %d\n", "Required Bags", request.requiredBags);
    printf("%-24s : %d\n", "Verified Bags", request.verifiedBags);

    if (!printRequesterConfirmableAssignments(requestId))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter assignment ID to confirm (0 to back): ", &assignmentId))
    {
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");
    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == assignmentId &&
            assignment.requestId == requestId)
        {
            found = 1;

            if (assignment.status != ASSIGNMENT_WAITING_VERIFICATION)
            {
                /* Only donation-done assignments can be confirmed by the requester. */
            }
            else if (assignment.requesterConfirmed)
            {
                printStatusMessage("INFO", "Requester confirmation was already saved for this assignment.");
            }
            else if (findDonorById(assignment.donorId, &donor))
            {
                assignment.requesterConfirmed = 1;
                confirmed = 1;

                if (assignment.adminVerified)
                {
                    assignment.status = ASSIGNMENT_VERIFIED;
                    strncpy(assignment.verifiedDate, dateTime, sizeof(assignment.verifiedDate) - 1);
                    finalAssignment = assignment;
                    finalized = 1;
                }
            }
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            printStatusMessage("ERROR", "Failed to verify donation.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Assignment does not belong to your request.");
        pauseScreen();
        return;
    }

    if (!confirmed)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Assignment must be Donation Done, Waiting Verification and not already requester-confirmed.");
        pauseScreen();
        return;
    }

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save requester verification.");
        pauseScreen();
        return;
    }

    if (finalized)
    {
        if (!updateDonorAfterVerifiedDonation(donor.donorId) ||
            !saveDonationRecordForAssignment(&request, &donor, finalAssignment.bags, 1, 1))
        {
            printStatusMessage("ERROR", "Donation was confirmed, but donor history could not be fully updated.");
            writeActivityLog("Requester confirmation completed with donor history update error.");
            pauseScreen();
            return;
        }

        updateRequestStatusAfterVerification(requestId);
        printStatusMessage("SUCCESS", "Requester confirmed donation. Admin verification was already complete, so the donation is now finalized.");
        writeActivityLog("Requester confirmed and finalized a donation assignment.");
    }
    else
    {
        printStatusMessage("SUCCESS", "Requester confirmation saved. Waiting for admin verification before final completion.");
        writeActivityLog("Requester confirmed a donation assignment.");
    }

    pauseScreen();
}

void cancelRequesterRequest(int requestId, const char *trackingPIN)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int found = 0;
    int cancelled = 0;

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

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId &&
            strcmp(request.trackingPIN, trackingPIN) == 0)
        {
            found = 1;

            if (request.requestStatus != REQUEST_FULFILLED &&
                request.requestStatus != REQUEST_CANCELLED)
            {
                request.requestStatus = REQUEST_CANCELLED;
                cancelled = 1;
            }
            else
            {
                printStatusMessage("ERROR", "Fulfilled or already cancelled requests cannot be cancelled.");
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to cancel request.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printStatusMessage("ERROR", "Unable to verify this request.");
        pauseScreen();
        return;
    }

    if (!cancelled)
    {
        remove(TEMP_REQUEST_FILE);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after cancellation.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request cancelled successfully.");
    cancelAssignmentsForRequest(requestId);
    writeActivityLog("Requester cancelled an emergency request.");
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

void initializeDemoDataIfEmpty(void)
{
    FILE *donorFile;
    FILE *requestFile;
    FILE *assignmentFile;
    Donor donor;
    Request request;
    const char *firstNames[] = {
        "Arafat", "Nusrat", "Tanvir", "Sadia", "Rafi", "Maliha", "Shuvo", "Farhana", "Imran", "Nabila"
    };
    const char *lastNames[] = {
        "Rahman", "Ahmed", "Islam", "Chowdhury", "Hossain", "Karim", "Hasan", "Akter", "Mahmud", "Sultana"
    };
    const char *bloodGroups[] = {
        "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    };
    const char *areas[] = {
        "Bashundhara", "Mirpur", "Dhanmondi", "Uttara", "Banani",
        "Gulshan", "Mohammadpur", "Badda", "Farmgate", "Rampura"
    };
    const char *hospitals[] = {
        "Evercare Hospital Dhaka", "Square Hospital", "United Hospital", "Labaid Hospital",
        "Ibn Sina Hospital", "Popular Medical College Hospital", "Dhaka Medical College Hospital",
        "Kurmitola General Hospital", "BIRDEM General Hospital", "National Heart Foundation"
    };
    const char *requesters[] = {
        "Mahin Rahman", "Samiha Karim", "Fahim Ahmed", "Nafisa Islam", "Rashed Hasan",
        "Tania Akter", "Sakib Chowdhury", "Moumita Sultana", "Arman Hossain", "Jannat Mahmud"
    };
    const char *patients[] = {
        "Kamal Uddin", "Sharmin Akter", "Rezaul Karim", "Mithila Rahman", "Abdul Kader",
        "Salma Begum", "Hasib Hasan", "Rokeya Sultana", "Anika Islam", "Mizanur Rahman"
    };
    int donorsHaveData = 0;
    int requestsHaveData = 0;
    int i;
    int id;

    donorFile = fopen(DONOR_FILE, "rb");
    if (donorFile != NULL)
    {
        donorsHaveData = fread(&donor, sizeof(Donor), 1, donorFile) == 1;
        fclose(donorFile);
    }

    requestFile = fopen(REQUEST_FILE, "rb");
    if (requestFile != NULL)
    {
        requestsHaveData = fread(&request, sizeof(Request), 1, requestFile) == 1;
        fclose(requestFile);
    }

    if (donorsHaveData || requestsHaveData)
    {
        return;
    }

    donorFile = fopen(DONOR_FILE, "wb");
    requestFile = fopen(REQUEST_FILE, "wb");

    if (donorFile == NULL || requestFile == NULL)
    {
        if (donorFile != NULL)
        {
            fclose(donorFile);
        }
        if (requestFile != NULL)
        {
            fclose(requestFile);
        }
        return;
    }

    for (i = 0; i < 55; i++)
    {
        id = i + 1;
        memset(&donor, 0, sizeof(Donor));

        donor.donorId = id;
        snprintf(donor.name, sizeof(donor.name), "%s %s", firstNames[i % 10], lastNames[(i / 10 + i) % 10]);
        donor.age = 18 + ((i * 7) % 43);
        snprintf(donor.gender, sizeof(donor.gender), "%s", (i % 2 == 0) ? "Male" : "Female");
        snprintf(donor.bloodGroup, sizeof(donor.bloodGroup), "%s", bloodGroups[i % 8]);
        snprintf(donor.phone, sizeof(donor.phone), "01710%06d", id);
        snprintf(donor.email, sizeof(donor.email), "donor%d@nsublood.demo", id);
        snprintf(donor.address, sizeof(donor.address), "%s, Dhaka", areas[i % 10]);

        if (i % 6 == 0)
        {
            snprintf(donor.lastDonationDate, sizeof(donor.lastDonationDate), "Never");
        }
        else
        {
            snprintf(donor.lastDonationDate,
                     sizeof(donor.lastDonationDate),
                     "2025-%02d-%02d",
                     (i % 12) + 1,
                     (i % 28) + 1);
        }

        snprintf(donor.medicalNote, sizeof(donor.medicalNote), "No major issue");
        snprintf(donor.securityQuestion, sizeof(donor.securityQuestion), "What is your donor code?");
        snprintf(donor.securityAnswer, sizeof(donor.securityAnswer), "donor%d", id);
        snprintf(donor.availabilityStatus, sizeof(donor.availabilityStatus), "%s", (id % 11 == 0) ? "0" : "1");
        snprintf(donor.password, sizeof(donor.password), "donor%d", id);
        donor.donationCount = i % 6;
        donor.approvalStatus = DONOR_APPROVED;

        if (fwrite(&donor, sizeof(Donor), 1, donorFile) != 1)
        {
            fclose(donorFile);
            fclose(requestFile);
            return;
        }
    }

    for (i = 0; i < 30; i++)
    {
        id = i + 1;
        memset(&request, 0, sizeof(Request));

        request.requestId = id;
        snprintf(request.requesterName, sizeof(request.requesterName), "%s", requesters[i % 10]);
        snprintf(request.patientName, sizeof(request.patientName), "%s", patients[(i * 3) % 10]);
        snprintf(request.bloodGroupNeeded, sizeof(request.bloodGroupNeeded), "%s", bloodGroups[(i + 2) % 8]);
        request.requiredBags = (i % 5) + 1;
        request.verifiedBags = 0;
        snprintf(request.hospitalName, sizeof(request.hospitalName), "%s", hospitals[i % 10]);
        snprintf(request.location, sizeof(request.location), "%s, Dhaka", areas[(i + 4) % 10]);
        snprintf(request.contactNumber, sizeof(request.contactNumber), "01820%06d", id);
        snprintf(request.urgencyLevel, sizeof(request.urgencyLevel), "%d", (i % 4) + 1);
        snprintf(request.trackingPIN, sizeof(request.trackingPIN), "%d", 1000 + id);
        request.requestStatus = REQUEST_PENDING;

        if (fwrite(&request, sizeof(Request), 1, requestFile) != 1)
        {
            fclose(donorFile);
            fclose(requestFile);
            return;
        }
    }

    fclose(donorFile);
    fclose(requestFile);

    assignmentFile = fopen(ASSIGNMENT_FILE, "wb");
    if (assignmentFile != NULL)
    {
        fclose(assignmentFile);
    }
}

void topUpDemoDataForFinalSetup(void)
{
    FILE *file;
    Donor donor;
    Request request;
    int donorCount = 0;
    int requestCount = 0;
    int maxDonorId = 0;
    int maxRequestId = 0;
    int id;
    const int targetDonors = 139;
    const int targetRequests = 73;
    const char *firstNames[] = {
        "Arafat", "Nusrat", "Tanvir", "Sadia", "Rafi", "Maliha", "Shuvo", "Farhana", "Imran", "Nabila",
        "Rakib", "Sumaiya", "Mehedi", "Tasnim", "Sabbir", "Mim", "Nahid", "Raisa", "Foysal", "Jerin"
    };
    const char *lastNames[] = {
        "Rahman", "Ahmed", "Islam", "Chowdhury", "Hossain", "Karim", "Hasan", "Akter", "Mahmud", "Sultana",
        "Khan", "Mollah", "Sarker", "Begum", "Alam", "Mitra", "Kabir", "Nahar", "Uddin", "Jahan"
    };
    const char *bloodGroups[] = {
        "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
    };
    const char *areas[] = {
        "Bashundhara", "Mirpur", "Dhanmondi", "Uttara", "Banani",
        "Gulshan", "Mohammadpur", "Badda", "Farmgate", "Rampura",
        "Mohakhali", "Khilgaon", "Motijheel", "Wari", "Shyamoli",
        "Tejgaon", "Malibagh", "Lalbagh", "Jatrabari", "Khilkhet"
    };
    const char *hospitals[] = {
        "Evercare Hospital Dhaka", "Square Hospital", "United Hospital", "Labaid Hospital",
        "Ibn Sina Hospital", "Popular Medical College Hospital", "Dhaka Medical College Hospital",
        "Kurmitola General Hospital", "BIRDEM General Hospital", "National Heart Foundation",
        "Anwer Khan Modern Hospital", "Holy Family Red Crescent Hospital", "Bangladesh Specialized Hospital",
        "Ahsania Mission Cancer Hospital", "Mugda Medical College Hospital"
    };
    const char *requesters[] = {
        "Mahin Rahman", "Samiha Karim", "Fahim Ahmed", "Nafisa Islam", "Rashed Hasan",
        "Tania Akter", "Sakib Chowdhury", "Moumita Sultana", "Arman Hossain", "Jannat Mahmud",
        "Raihan Kabir", "Nadia Jahan", "Sohan Uddin", "Ishrat Nahar", "Adnan Alam"
    };
    const char *patients[] = {
        "Kamal Uddin", "Sharmin Akter", "Rezaul Karim", "Mithila Rahman", "Abdul Kader",
        "Salma Begum", "Hasib Hasan", "Rokeya Sultana", "Anika Islam", "Mizanur Rahman",
        "Tahmid Hasan", "Mariya Islam", "Saiful Alam", "Nusrat Jahan", "Farhan Kabir"
    };

    file = fopen(DONOR_FILE, "rb");
    if (file != NULL)
    {
        while (fread(&donor, sizeof(Donor), 1, file) == 1)
        {
            donorCount++;
            if (donor.donorId > maxDonorId)
            {
                maxDonorId = donor.donorId;
            }
        }
        fclose(file);
    }

    if (donorCount < targetDonors)
    {
        file = fopen(DONOR_FILE, "ab");
        if (file != NULL)
        {
            while (donorCount < targetDonors)
            {
                id = ++maxDonorId;
                memset(&donor, 0, sizeof(Donor));

                donor.donorId = id;
                snprintf(donor.name, sizeof(donor.name), "%s %s", firstNames[id % 20], lastNames[(id * 3) % 20]);
                donor.age = 18 + ((id * 5) % 43);
                snprintf(donor.gender, sizeof(donor.gender), "%s", (id % 2 == 0) ? "Female" : "Male");
                snprintf(donor.bloodGroup, sizeof(donor.bloodGroup), "%s", bloodGroups[(id + 3) % 8]);
                snprintf(donor.phone, sizeof(donor.phone), "01710%06d", id);
                snprintf(donor.email, sizeof(donor.email), "donor%d@nsublood.demo", id);
                snprintf(donor.address, sizeof(donor.address), "%s, Dhaka", areas[id % 20]);

                if (id % 7 == 0)
                {
                    snprintf(donor.lastDonationDate, sizeof(donor.lastDonationDate), "Never");
                }
                else
                {
                    snprintf(donor.lastDonationDate,
                             sizeof(donor.lastDonationDate),
                             "2025-%02d-%02d",
                             (id % 12) + 1,
                             (id % 28) + 1);
                }

                snprintf(donor.medicalNote, sizeof(donor.medicalNote), "No major issue");
                snprintf(donor.securityQuestion, sizeof(donor.securityQuestion), "What is your donor code?");
                snprintf(donor.securityAnswer, sizeof(donor.securityAnswer), "donor%d", id);
                snprintf(donor.availabilityStatus, sizeof(donor.availabilityStatus), "%s", (id % 13 == 0) ? "0" : "1");
                snprintf(donor.password, sizeof(donor.password), "donor%d", id);
                donor.donationCount = id % 6;
                donor.approvalStatus = DONOR_APPROVED;

                if (fwrite(&donor, sizeof(Donor), 1, file) != 1)
                {
                    break;
                }

                donorCount++;
            }

            fclose(file);
        }
    }

    file = fopen(REQUEST_FILE, "rb");
    if (file != NULL)
    {
        while (fread(&request, sizeof(Request), 1, file) == 1)
        {
            requestCount++;
            if (request.requestId > maxRequestId)
            {
                maxRequestId = request.requestId;
            }
        }
        fclose(file);
    }

    if (requestCount < targetRequests)
    {
        file = fopen(REQUEST_FILE, "ab");
        if (file != NULL)
        {
            while (requestCount < targetRequests)
            {
                id = ++maxRequestId;
                memset(&request, 0, sizeof(Request));

                request.requestId = id;
                snprintf(request.requesterName, sizeof(request.requesterName), "%s", requesters[id % 15]);
                snprintf(request.patientName, sizeof(request.patientName), "%s", patients[(id * 2) % 15]);
                snprintf(request.bloodGroupNeeded, sizeof(request.bloodGroupNeeded), "%s", bloodGroups[(id + 5) % 8]);
                request.requiredBags = (id % 5) + 1;
                request.verifiedBags = 0;
                snprintf(request.hospitalName, sizeof(request.hospitalName), "%s", hospitals[id % 15]);
                snprintf(request.location, sizeof(request.location), "%s, Dhaka", areas[(id + 6) % 20]);
                snprintf(request.contactNumber, sizeof(request.contactNumber), "01820%06d", id);
                snprintf(request.urgencyLevel, sizeof(request.urgencyLevel), "%d", (id % 4) + 1);
                snprintf(request.trackingPIN, sizeof(request.trackingPIN), "%d", 1000 + id);
                request.requestStatus = REQUEST_PENDING;

                if (fwrite(&request, sizeof(Request), 1, file) != 1)
                {
                    break;
                }

                requestCount++;
            }

            fclose(file);
        }
    }
}

int adminLogin(void)
{
    FILE *file;
    Admin admin;
    int inputId;
    char inputPassword[30];
    char logMessage[100];
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
        printStatusMessage("ERROR", "Unable to open the admin account file.");
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
        printStatusMessage("SUCCESS", "Administrator login successful.");
        setActivityActor("ADMIN", admin.adminId);
        writeActivityLog("Logged in.");
        pauseScreen();
        return admin.adminId;
    }

    printStatusMessage("ERROR", "Invalid Admin ID or password. Returning to access menu.");
    snprintf(logMessage, sizeof(logMessage), "Failed admin login attempt for ID %d.", inputId);
    writeActivityLog(logMessage);
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
    if (!inputPasswordOrBack("Enter new password: ", newPassword, sizeof(newPassword)))
    {
        printStatusMessage("INFO", "Password change cancelled.");
        pauseScreen();
        return;
    }
    getTextInput("Confirm new password: ", confirmPassword, sizeof(confirmPassword));

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
    char logMessage[100];
    int accountFound = 0;
    int passwordMatched = 0;
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
        if (donor.donorId == inputId)
        {
            accountFound = 1;

            if (strcmp(donor.password, inputPassword) == 0)
            {
                passwordMatched = 1;

                if (isDonorApproved(&donor))
                {
                    loginSuccess = 1;
                }
            }

            break;
        }
    }

    fclose(file);

    if (loginSuccess)
    {
        printf("\nWelcome, %s!\n", donor.name);
        printStatusMessage("SUCCESS", "Donor login successful.");
        setActivityActor("DONOR", inputId);
        writeActivityLog("Logged in.");
        pauseScreen();
        return inputId;
    }

    if (accountFound && passwordMatched)
    {
        printf("\nAccount Status : %s\n", getDonorApprovalStatusText(donor.approvalStatus));

        if (donor.approvalStatus == DONOR_PENDING_APPROVAL)
        {
            printStatusMessage("INFO", "Your account is waiting for admin approval.");
        }
        else if (donor.approvalStatus == DONOR_REJECTED)
        {
            printStatusMessage("ERROR", "Your registration request was rejected.");
        }
        else if (donor.approvalStatus == DONOR_BLOCKED)
        {
            printStatusMessage("ERROR", "Your account is blocked.");
        }
        else
        {
            printStatusMessage("ERROR", "Your donor account is not approved for login.");
        }

        setActivityActor("DONOR", inputId);
        writeActivityLog("Login blocked due to account status.");
        clearActivityActor();
        pauseScreen();
        return 0;
    }

    printStatusMessage("ERROR", "Invalid Donor ID or password. Returning to access menu.");
    snprintf(logMessage, sizeof(logMessage), "Failed donor login attempt for ID %d.", inputId);
    writeActivityLog(logMessage);
    pauseScreen();
    return 0;
}

void donorSelfRegistration(void)
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

    printSectionHeader("Donor Self Registration");
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");
    printf("Your generated Donor ID : %d\n", donor.donorId);

    if (!getTextInputOrBack("Enter donor name: ", donor.name, sizeof(donor.name)) ||
        !inputAgeOrBack(&donor.age) ||
        !getTextInputOrBack("Enter gender: ", donor.gender, sizeof(donor.gender)) ||
        !inputBloodGroupOrBack("Enter blood group: ", donor.bloodGroup, sizeof(donor.bloodGroup)))
    {
        fclose(file);
        printStatusMessage("INFO", "Registration cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    do
    {
        if (!inputPhoneOrBack("Enter phone number: ", donor.phone, sizeof(donor.phone)) ||
            !inputEmailOrBack("Enter email: ", donor.email, sizeof(donor.email)))
        {
            fclose(file);
            printStatusMessage("INFO", "Registration cancelled. No information was saved.");
            pauseScreen();
            return;
        }

        if (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, 0))
        {
            printStatusMessage("ERROR", "Phone or email already exists. Please use unique contact information.");
        }
    } while (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, 0));

    if (!getTextInputOrBack("Enter address/area: ", donor.address, sizeof(donor.address)) ||
        !inputDateOrBack("Enter last donation date (YYYY-MM-DD or Never): ", donor.lastDonationDate, sizeof(donor.lastDonationDate)) ||
        !getTextInputOrBack("Enter medical note: ", donor.medicalNote, sizeof(donor.medicalNote)) ||
        !getTextInputOrBack("Enter security question: ", donor.securityQuestion, sizeof(donor.securityQuestion)) ||
        !getTextInputOrBack("Enter security answer: ", donor.securityAnswer, sizeof(donor.securityAnswer)))
    {
        fclose(file);
        printStatusMessage("INFO", "Registration cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    strcpy(donor.availabilityStatus, "1");

    do
    {
        if (!inputPasswordOrBack("Create donor password: ", donor.password, sizeof(donor.password)))
        {
            fclose(file);
            printStatusMessage("INFO", "Registration cancelled. No information was saved.");
            pauseScreen();
            return;
        }
    } while (!isValidPassword(donor.password));

    donor.donationCount = 0;
    donor.approvalStatus = DONOR_PENDING_APPROVAL;

    if (fwrite(&donor, sizeof(Donor), 1, file) != 1)
    {
        fclose(file);
        printStatusMessage("ERROR", "Failed to save donor registration.");
        pauseScreen();
        return;
    }

    fclose(file);

    printStatusMessage("SUCCESS", "Registration submitted successfully.");
    printf("Save your Donor ID. You can login after admin approval.\n");
    printf("%-24s : %d\n", "Donor ID", donor.donorId);
    printf("%-24s : %s\n", "Account Status", getDonorApprovalStatusText(donor.approvalStatus));
    writeActivityLog("New donor self-registration submitted.");
    pauseScreen();
}

void donorMenu(int donorId)
{
    int choice;

    while (1)
    {
        printSectionHeader("Donor Portal");
        printf("  1. View My Profile\n");
        printf("  2. Update My Profile\n");
        printf("  3. Change Password\n");
        printf("  4. View Matching Blood Requests\n");
        printf("  5. Show Interest in a Request\n");
        printf("  6. View My Interested Requests\n");
        printf("  7. View My Assigned Requests\n");
        printf("  8. Mark Donation Done\n");
        printf("  9. View Donation History\n");
        printf(" 10. View Donation Count\n");
        printf(" 11. Logout\n");
        printLine('-', 72);

        choice = getMenuChoice();

        switch (choice)
        {
        case 1:
            viewOwnDonorProfile(donorId);
            break;
        case 2:
            updateOwnDonorProfile(donorId);
            break;
        case 3:
            changeDonorPassword(donorId);
            break;
        case 4:
            viewMatchingBloodRequests(donorId);
            break;
        case 5:
            showInterestInRequest(donorId);
            break;
        case 6:
            viewMyInterestedRequests(donorId);
            break;
        case 7:
            viewMyAssignedRequests(donorId);
            break;
        case 8:
            markDonationDone(donorId);
            break;
        case 9:
            viewMyDonationHistory(donorId);
            break;
        case 10:
            viewMyDonationCount(donorId);
            break;
        case 11:
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
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");

    if (!getTextInputOrBack("Enter current password: ", currentPassword, sizeof(currentPassword)))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Password change cancelled.");
        pauseScreen();
        return;
    }

    if (!inputPasswordOrBack("Enter new password: ", newPassword, sizeof(newPassword)))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Password change cancelled.");
        pauseScreen();
        return;
    }

    if (!getTextInputOrBack("Confirm new password: ", confirmPassword, sizeof(confirmPassword)))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Password change cancelled.");
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

void adminResetDonorPassword(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int donorId;
    char newPassword[30];
    char confirmPassword[30];
    int found = 0;

    printSectionHeader("Reset Donor Password");
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");

    if (!printDonorSelectionList("Choose Donor For Password Reset", -1))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
    {
        return;
    }

    if (!inputPasswordOrBack("Enter new password: ", newPassword, sizeof(newPassword)))
    {
        printStatusMessage("INFO", "Password reset cancelled.");
        pauseScreen();
        return;
    }

    if (!getTextInputOrBack("Confirm new password: ", confirmPassword, sizeof(confirmPassword)))
    {
        printStatusMessage("INFO", "Password reset cancelled.");
        pauseScreen();
        return;
    }

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printStatusMessage("ERROR", "New passwords do not match.");
        pauseScreen();
        return;
    }

    sourceFile = fopen(DONOR_FILE, "rb");
    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open donor files.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            strncpy(donor.password, newPassword, sizeof(donor.password) - 1);
            donor.password[sizeof(donor.password) - 1] = '\0';
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printStatusMessage("ERROR", "Failed to reset donor password.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "No donor found with that ID.");
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save donor password reset.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor password reset successfully.");
    writeActivityLog("Admin reset donor password.");
    pauseScreen();
}

void viewDonorLoginInfoForAdmin(void)
{
    FILE *file;
    Donor donor;
    int choice;
    int donorId;
    int found = 0;

    printSectionHeader("Admin Donor Login Info");
    printf("Demo/admin use only. In real systems, passwords should be reset, not viewed.\n");
    printf("  1. View All Donor Login Info\n");
    printf("  2. View One Donor Login Info\n");
    printf("  0. Back\n");
    printLine('-', 72);

    if (!readIntInRangeOrBack("Enter choice: ", &choice, 0, 2))
    {
        return;
    }

    if (choice == 0)
    {
        return;
    }

    if (choice == 2)
    {
        if (!printDonorSelectionList("Choose Donor To View Login Info", -1))
        {
            pauseScreen();
            return;
        }

        if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
        {
            return;
        }
    }

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader(choice == 1 ? "All Donor Login Info" : "Selected Donor Login Info");
    printf("%-8s %-22s %-18s %-18s %-18s\n",
           "ID",
           "Name",
           "Password",
           "Approval",
           "Available");
    printLine('-', 90);

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (choice == 1 || donor.donorId == donorId)
        {
            printf("%-8d %-22s %-18s %-18s %-18s\n",
                   donor.donorId,
                   donor.name,
                   donor.password,
                   getDonorApprovalStatusText(donor.approvalStatus),
                   getAvailabilityText(&donor));
            found = 1;

            if (choice == 2)
            {
                break;
            }
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donor login information found for that choice.");
    }

    writeActivityLog("Admin viewed donor login information.");
    pauseScreen();
}

void forgotPassword(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int donorId;
    char phoneOrEmail[50];
    char answer[50];
    char newPassword[30];
    int found = 0;
    int verified = 0;

    printSectionHeader("Forgot Password");
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");

    if (!readIntOrBack("Enter donor ID: ", &donorId) ||
        !getTextInputOrBack("Enter registered phone or email: ", phoneOrEmail, sizeof(phoneOrEmail)))
    {
        return;
    }

    sourceFile = fopen(DONOR_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId &&
            (strcmp(donor.phone, phoneOrEmail) == 0 || strcmp(donor.email, phoneOrEmail) == 0))
        {
            found = 1;
            break;
        }
    }

    fclose(sourceFile);

    if (!found)
    {
        printStatusMessage("ERROR", "Donor ID and phone/email did not match.");
        pauseScreen();
        return;
    }

    if (strlen(donor.securityQuestion) == 0 || strlen(donor.securityAnswer) == 0)
    {
        printStatusMessage("ERROR", "No security question is saved for this account. Contact admin.");
        pauseScreen();
        return;
    }

    printf("Security Question: %s\n", donor.securityQuestion);
    if (!getTextInputOrBack("Enter answer: ", answer, sizeof(answer)))
    {
        return;
    }

    if (strcmp(donor.securityAnswer, answer) != 0)
    {
        printStatusMessage("ERROR", "Security answer is incorrect.");
        pauseScreen();
        return;
    }

    verified = 1;
    if (!inputPasswordOrBack("Enter new password: ", newPassword, sizeof(newPassword)))
    {
        printStatusMessage("INFO", "Password reset cancelled.");
        pauseScreen();
        return;
    }

    sourceFile = fopen(DONOR_FILE, "rb");
    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open donor files.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId && verified)
        {
            strncpy(donor.password, newPassword, sizeof(donor.password) - 1);
            donor.password[sizeof(donor.password) - 1] = '\0';
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printStatusMessage("ERROR", "Failed to reset password.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save new password.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Password reset successfully.");
    writeActivityLog("Donor reset password with security question.");
    pauseScreen();
}

void updateOwnDonorProfile(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    Donor updatedDonor;
    int found = 0;
    int cancelled = 0;

    sourceFile = fopen(DONOR_FILE, "rb");
    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open donor files.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            updatedDonor = donor;
            printSectionHeader("Update My Profile");
            printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");
            displayDonor(&donor);

            if (!getTextInputOrBack("Enter name: ", updatedDonor.name, sizeof(updatedDonor.name)) ||
                !inputPhoneOrBack("Enter phone: ", updatedDonor.phone, sizeof(updatedDonor.phone)) ||
                !inputEmailOrBack("Enter email: ", updatedDonor.email, sizeof(updatedDonor.email)))
            {
                cancelled = 1;
            }
            else if (isDuplicateDonorPhoneOrEmail(updatedDonor.phone, updatedDonor.email, donorId))
            {
                printStatusMessage("ERROR", "Phone or email already belongs to another donor. No information was saved.");
                cancelled = 1;
            }
            else if (!getTextInputOrBack("Enter address/area: ", updatedDonor.address, sizeof(updatedDonor.address)) ||
                !inputAvailabilityOrBack("Enter availability (1=available, 0=unavailable): ", updatedDonor.availabilityStatus, sizeof(updatedDonor.availabilityStatus)) ||
                !inputDateOrBack("Enter last donation date (YYYY-MM-DD or Never): ", updatedDonor.lastDonationDate, sizeof(updatedDonor.lastDonationDate)) ||
                !getTextInputOrBack("Enter medical note: ", updatedDonor.medicalNote, sizeof(updatedDonor.medicalNote)))
            {
                cancelled = 1;
            }

            if (!cancelled)
            {
                donor = updatedDonor;
            }
        }

        if (!cancelled && fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printStatusMessage("ERROR", "Failed to update profile.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (cancelled)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Profile update cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Donor profile was not found.");
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save profile update.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Profile updated successfully.");
    writeActivityLog("Donor updated own profile.");
    pauseScreen();
}

void viewMatchingBloodRequests(int donorId)
{
    printMatchingRequestSelectionForDonor(donorId, "Matching Open Blood Requests");
    pauseScreen();
}

int printMatchingRequestSelectionForDonor(int donorId, const char *title)
{
    FILE *file;
    Donor donor;
    Request request;
    int found = 0;

    if (!findDonorById(donorId, &donor))
    {
        printStatusMessage("ERROR", "Donor profile was not found.");
        return 0;
    }

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-18s : %s\n", "My Blood Group", donor.bloodGroup);
    printf("%-18s : %s\n", "My Area", donor.address);
    printLine('-', 100);
    printf("%-8s %-18s %-8s %-6s %-6s %-22s %-14s\n",
           "Req ID",
           "Patient",
           "Group",
           "Need",
           "Done",
           "Request Location",
           "Status");
    printLine('-', 100);

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (isCompatible(donor.bloodGroup, request.bloodGroupNeeded) &&
            request.requestStatus != REQUEST_FULFILLED &&
            request.requestStatus != REQUEST_CANCELLED)
        {
            printf("%-8d %-18s %-8s %-6d %-6d %-22s %-14s\n",
                   request.requestId,
                   request.patientName,
                   request.bloodGroupNeeded,
                   request.requiredBags,
                   request.verifiedBags,
                   request.location,
                   getRequestStatusShortText(request.requestStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No open matching requests found.");
    }

    return found;
}

void showInterestInRequest(int donorId)
{
    Donor donor;
    Request request;
    int requestId;

    printSectionHeader("Show Interest in a Request");
    printf("Enter 0 to go back. Nothing will be saved.\n");

    if (!findDonorById(donorId, &donor))
    {
        printStatusMessage("ERROR", "Donor profile was not found.");
        pauseScreen();
        return;
    }

    if (!isDonorAvailable(&donor))
    {
        printStatusMessage("ERROR", "Unavailable donors cannot show interest.");
        pauseScreen();
        return;
    }

    if (!printMatchingRequestSelectionForDonor(donorId, "Choose Matching Request To Show Interest"))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to back): ", &requestId))
    {
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "No request found with that ID.");
        pauseScreen();
        return;
    }

    if (request.requestStatus == REQUEST_CANCELLED || request.requestStatus == REQUEST_FULFILLED)
    {
        printStatusMessage("ERROR", "This request is not open for donor interest.");
        pauseScreen();
        return;
    }

    if (!isCompatible(donor.bloodGroup, request.bloodGroupNeeded))
    {
        printStatusMessage("ERROR", "Your blood group does not match this request.");
        pauseScreen();
        return;
    }

    if (donorInterestExists(requestId, donorId))
    {
        printStatusMessage("ERROR", "You already showed interest in this request.");
        pauseScreen();
        return;
    }

    if (!createOrUpdateAssignment(requestId, donorId, ASSIGNMENT_INTERESTED))
    {
        printStatusMessage("ERROR", "Failed to save donor interest.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Interest saved. Admin can now assign you if needed.");
    writeActivityLog("Donor showed interest in a blood request.");
    pauseScreen();
}

void viewMyInterestedRequests(int donorId)
{
    FILE *file;
    DonorAssignment assignment;
    Request request;
    int found = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No interest records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("My Interested Requests");

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.donorId == donorId &&
            assignment.status == ASSIGNMENT_INTERESTED)
        {
            displayAssignment(&assignment);
            if (findRequestById(assignment.requestId, &request))
            {
                displayRequest(&request);
            }
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "You have not shown interest in any request yet.");
    }

    pauseScreen();
}

void viewMyAssignedRequests(int donorId)
{
    FILE *file;
    DonorAssignment assignment;
    Request request;
    int found = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("My Assigned Requests");

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.donorId == donorId &&
            (assignment.status == ASSIGNMENT_ASSIGNED ||
             assignment.status == ASSIGNMENT_WAITING_VERIFICATION ||
             assignment.status == ASSIGNMENT_VERIFIED))
        {
            displayAssignment(&assignment);
            if (findRequestById(assignment.requestId, &request))
            {
                displayRequest(&request);
            }
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No assigned requests found for your account.");
    }

    pauseScreen();
}

int printDonorAssignmentSelectionList(int donorId, const char *title, int statusFilter)
{
    FILE *file;
    DonorAssignment assignment;
    Request request;
    int found = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-8s %-8s %-18s %-22s %-14s %-10s %-8s\n",
           "Assign",
           "Req ID",
           "Patient",
           "Location",
           "Status",
           "Requester",
           "Admin");
    printLine('-', 95);

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.donorId == donorId &&
            (statusFilter == -1 || assignment.status == statusFilter))
        {
            if (!findRequestById(assignment.requestId, &request))
            {
                memset(&request, 0, sizeof(Request));
                strcpy(request.patientName, "Unknown");
                strcpy(request.location, "Unknown");
            }

            printf("%-8d %-8d %-18s %-22s %-14s %-10s %-8s\n",
                   assignment.assignmentId,
                   assignment.requestId,
                   request.patientName,
                   request.location,
                   getAssignmentStatusShortText(assignment.status),
                   assignment.requesterConfirmed ? "Yes" : "No",
                   assignment.adminVerified ? "Yes" : "No");
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No matching assignments found for your account.");
    }

    return found;
}

void markDonationDone(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;
    int assignmentId;
    int found = 0;
    int updated = 0;
    char dateTime[30];

    printSectionHeader("Mark Donation Done");
    printf("Enter 0 to go back. Only your own assigned donations can be updated.\n");

    if (!printDonorAssignmentSelectionList(donorId, "Choose Assigned Donation To Mark Done", ASSIGNMENT_ASSIGNED))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter assignment ID (0 to back): ", &assignmentId))
    {
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");
    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == assignmentId && assignment.donorId == donorId)
        {
            found = 1;
            if (assignment.status == ASSIGNMENT_ASSIGNED)
            {
                assignment.status = ASSIGNMENT_WAITING_VERIFICATION;
                assignment.requesterConfirmed = 0;
                assignment.adminVerified = 0;
                strncpy(assignment.donationDoneDate, dateTime, sizeof(assignment.donationDoneDate) - 1);
                updated = 1;
            }
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            printStatusMessage("ERROR", "Failed to update assignment.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Assignment not found for your donor account.");
        pauseScreen();
        return;
    }

    if (!updated)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Only assigned donations can be marked done.");
        pauseScreen();
        return;
    }

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save donation done status.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donation marked done. Waiting for requester/admin verification.");
    writeActivityLog("Donor marked assigned donation done.");
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

int readIntOrBack(const char *prompt, int *value)
{
    printf("%s", prompt);

    while (scanf("%d", value) != 1)
    {
        printf("[ERROR] Invalid input. Enter a number, or 0 to go back: ");
        clearInputBuffer();
    }

    clearInputBuffer();
    return *value != 0;
}

int getTextInputOrBack(const char *prompt, char *buffer, int size)
{
    getTextInput(prompt, buffer, size);
    return strcmp(buffer, "0") != 0;
}

int inputBloodGroupOrBack(const char *prompt, char *bloodGroup, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, bloodGroup, size))
        {
            return 0;
        }

        if (isValidBloodGroup(bloodGroup))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Invalid blood group. Use A+, A-, B+, B-, AB+, AB-, O+, or O-.");
    }
}

int readIntInRangeOrBack(const char *prompt, int *value, int min, int max)
{
    while (1)
    {
        if (!readIntOrBack(prompt, value))
        {
            return 0;
        }

        if (*value >= min && *value <= max)
        {
            return 1;
        }

        printf("[ERROR] Value must be between %d and %d.\n", min, max);
    }
}

int inputAgeOrBack(int *age)
{
    return readIntInRangeOrBack("Enter age (18-65): ", age, 18, 65);
}

int inputRequiredBagsOrBack(int *bags)
{
    return readIntInRangeOrBack("Enter required blood bags (1-20): ", bags, 1, 20);
}

int inputPhoneOrBack(const char *prompt, char *phone, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, phone, size))
        {
            return 0;
        }

        if (isValidPhone(phone))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Phone must contain 7-15 digits and may include +, spaces, or hyphens.");
    }
}

int inputEmailOrBack(const char *prompt, char *email, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, email, size))
        {
            return 0;
        }

        if (isValidEmail(email))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Email must contain @, a dot after @, and no spaces.");
    }
}

int inputDateOrBack(const char *prompt, char *dateText, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, dateText, size))
        {
            return 0;
        }

        if (isValidDateText(dateText))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Date must be a real calendar date in YYYY-MM-DD format, or write Never.");
    }
}

int inputAvailabilityOrBack(const char *prompt, char *availability, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, availability, size))
        {
            return 0;
        }

        if (isValidAvailabilityInput(availability))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Availability must be 1 for available or 0 for unavailable.");
    }
}

int inputPasswordOrBack(const char *prompt, char *password, int size)
{
    while (1)
    {
        if (!getTextInputOrBack(prompt, password, size))
        {
            return 0;
        }

        if (isValidPassword(password))
        {
            return 1;
        }

        printStatusMessage("ERROR", "Password must be at least 6 characters and include at least one letter and one number.");
    }
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
    printLine('=', 72);
    printf("  %s\n", title);
    printLine('=', 72);
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
    case REQUEST_DONOR_INTERESTED:
        return "Donor Interested";
    case REQUEST_PARTIALLY_MATCHED:
        return "Partially Matched";
    case REQUEST_FULLY_MATCHED:
        return "Fully Matched";
    case REQUEST_PARTIALLY_FULFILLED:
        return "Partially Fulfilled";
    case REQUEST_FULFILLED:
        return "Fulfilled / Verified";
    case REQUEST_CANCELLED:
        return "Cancelled";
    default:
        return "Unknown Status";
    }
}

const char *getRequestStatusShortText(int status)
{
    switch (status)
    {
    case REQUEST_PENDING:
        return "Pending";
    case REQUEST_DONOR_INTERESTED:
        return "Interested";
    case REQUEST_PARTIALLY_MATCHED:
        return "Part Matched";
    case REQUEST_FULLY_MATCHED:
        return "Full Matched";
    case REQUEST_PARTIALLY_FULFILLED:
        return "Part Done";
    case REQUEST_FULFILLED:
        return "Fulfilled";
    case REQUEST_CANCELLED:
        return "Cancelled";
    default:
        return "Unknown";
    }
}

const char *getAssignmentStatusText(int status)
{
    switch (status)
    {
    case ASSIGNMENT_INTERESTED:
        return "Interested";
    case ASSIGNMENT_ASSIGNED:
        return "Assigned";
    case ASSIGNMENT_WAITING_VERIFICATION:
        return "Donation Done, Waiting Verification";
    case ASSIGNMENT_VERIFIED:
        return "Verified / Completed";
    case ASSIGNMENT_REPLACED:
        return "Replaced";
    case ASSIGNMENT_CANCELLED:
        return "Cancelled";
    default:
        return "Unknown";
    }
}

const char *getAssignmentStatusShortText(int status)
{
    switch (status)
    {
    case ASSIGNMENT_INTERESTED:
        return "Interested";
    case ASSIGNMENT_ASSIGNED:
        return "Assigned";
    case ASSIGNMENT_WAITING_VERIFICATION:
        return "Waiting Verify";
    case ASSIGNMENT_VERIFIED:
        return "Completed";
    case ASSIGNMENT_REPLACED:
        return "Replaced";
    case ASSIGNMENT_CANCELLED:
        return "Cancelled";
    default:
        return "Unknown";
    }
}

const char *getAvailabilityText(const Donor *donor)
{
    return isDonorAvailable(donor) ? "Available" : "Unavailable";
}

const char *getDonorApprovalStatusText(int status)
{
    switch (status)
    {
    case DONOR_PENDING_APPROVAL:
        return "Pending Approval";
    case DONOR_APPROVED:
        return "Approved";
    case DONOR_REJECTED:
        return "Rejected";
    case DONOR_BLOCKED:
        return "Blocked";
    default:
        return "Unknown";
    }
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

int generateNextAssignmentId(void)
{
    FILE *file;
    DonorAssignment assignment;
    int lastId = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        return 1;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.assignmentId > lastId)
        {
            lastId = assignment.assignmentId;
        }
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

int isDonorApproved(const Donor *donor)
{
    return donor->approvalStatus == DONOR_APPROVED;
}

int isDonorEligibleForAssignment(const Donor *donor)
{
    return isDonorApproved(donor) && isDonorAvailable(donor);
}

int isValidBloodGroup(const char *bloodGroup)
{
    return strcmp(bloodGroup, "A+") == 0 ||
           strcmp(bloodGroup, "A-") == 0 ||
           strcmp(bloodGroup, "B+") == 0 ||
           strcmp(bloodGroup, "B-") == 0 ||
           strcmp(bloodGroup, "AB+") == 0 ||
           strcmp(bloodGroup, "AB-") == 0 ||
           strcmp(bloodGroup, "O+") == 0 ||
           strcmp(bloodGroup, "O-") == 0;
}

int isValidPhone(const char *phone)
{
    int i;
    int digits = 0;

    if (phone == NULL || strlen(phone) == 0)
    {
        return 0;
    }

    for (i = 0; phone[i] != '\0'; i++)
    {
        if (isdigit((unsigned char)phone[i]))
        {
            digits++;
        }
        else if (phone[i] != '+' && phone[i] != '-' && phone[i] != ' ')
        {
            return 0;
        }
    }

    return digits >= 7 && digits <= 15;
}

int isValidEmail(const char *email)
{
    const char *at;
    const char *dotAfterAt;

    if (email == NULL || strlen(email) < 5 || strchr(email, ' ') != NULL)
    {
        return 0;
    }

    at = strchr(email, '@');
    if (at == NULL || at == email || strchr(at + 1, '@') != NULL)
    {
        return 0;
    }

    dotAfterAt = strchr(at + 1, '.');
    return dotAfterAt != NULL && dotAfterAt[1] != '\0';
}

int isValidDateText(const char *dateText)
{
    int year;
    int month;
    int day;
    int i;
    int daysInMonth;
    int leapYear;

    if (dateText == NULL || strlen(dateText) == 0)
    {
        return 0;
    }

    if (strcmp(dateText, "Never") == 0 || strcmp(dateText, "never") == 0)
    {
        return 1;
    }

    if (strlen(dateText) != 10 ||
        dateText[4] != '-' ||
        dateText[7] != '-')
    {
        return 0;
    }

    for (i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            continue;
        }

        if (!isdigit((unsigned char)dateText[i]))
        {
            return 0;
        }
    }

    if (sscanf(dateText, "%4d-%2d-%2d", &year, &month, &day) != 3)
    {
        return 0;
    }

    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1)
    {
        return 0;
    }

    daysInMonth = 31;

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        daysInMonth = 30;
    }
    else if (month == 2)
    {
        leapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
        daysInMonth = leapYear ? 29 : 28;
    }

    return day <= daysInMonth;
}

int isValidAvailabilityInput(const char *availability)
{
    return strcmp(availability, "1") == 0 || strcmp(availability, "0") == 0;
}

int isValidPassword(const char *password)
{
    int i;
    int hasLetter = 0;
    int hasDigit = 0;

    if (password == NULL || strlen(password) < 6)
    {
        return 0;
    }

    for (i = 0; password[i] != '\0'; i++)
    {
        if (isalpha((unsigned char)password[i]))
        {
            hasLetter = 1;
        }
        else if (isdigit((unsigned char)password[i]))
        {
            hasDigit = 1;
        }
    }

    return hasLetter && hasDigit;
}

void inputBloodGroup(const char *prompt, char *bloodGroup, int size)
{
    do
    {
        getTextInput(prompt, bloodGroup, size);

        if (!isValidBloodGroup(bloodGroup))
        {
            printStatusMessage("ERROR", "Invalid blood group. Use A+, A-, B+, B-, AB+, AB-, O+, or O-.");
        }
    } while (!isValidBloodGroup(bloodGroup));
}

int isCompatible(const char donorBG[], const char requestBG[])
{
    if (strcmp(donorBG, "O-") == 0)
    {
        return 1;
    }
    if (strcmp(donorBG, "O+") == 0)
    {
        return strcmp(requestBG, "O+") == 0 ||
               strcmp(requestBG, "A+") == 0 ||
               strcmp(requestBG, "B+") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "A-") == 0)
    {
        return strcmp(requestBG, "A-") == 0 ||
               strcmp(requestBG, "A+") == 0 ||
               strcmp(requestBG, "AB-") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "A+") == 0)
    {
        return strcmp(requestBG, "A+") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "B-") == 0)
    {
        return strcmp(requestBG, "B-") == 0 ||
               strcmp(requestBG, "B+") == 0 ||
               strcmp(requestBG, "AB-") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "B+") == 0)
    {
        return strcmp(requestBG, "B+") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "AB-") == 0)
    {
        return strcmp(requestBG, "AB-") == 0 ||
               strcmp(requestBG, "AB+") == 0;
    }
    if (strcmp(donorBG, "AB+") == 0)
    {
        return strcmp(requestBG, "AB+") == 0;
    }

    return 0;
}

int isValidUrgencyChoice(int urgency)
{
    return urgency >= 1 && urgency <= 4;
}

const char *getUrgencyText(const char *urgencyLevel)
{
    if (strcmp(urgencyLevel, "1") == 0)
    {
        return "Low";
    }
    if (strcmp(urgencyLevel, "2") == 0)
    {
        return "Medium";
    }
    if (strcmp(urgencyLevel, "3") == 0)
    {
        return "High";
    }
    if (strcmp(urgencyLevel, "4") == 0)
    {
        return "Critical";
    }

    return urgencyLevel;
}

int isDuplicateDonorPhoneOrEmail(const char *phone, const char *email, int ignoreDonorId)
{
    FILE *file;
    Donor donor;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.donorId != ignoreDonorId &&
            (strcmp(donor.phone, phone) == 0 || strcmp(donor.email, email) == 0))
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void printMatchedDonorRow(const Donor *donor)
{
    printf("%-8d %-20s %-8s %-15s %-24s %-12s\n",
           donor->donorId,
           donor->name,
           donor->bloodGroup,
           donor->phone,
           donor->address,
           getAvailabilityText(donor));
}

int printDonorSelectionList(const char *title, int approvalStatusFilter)
{
    FILE *file;
    Donor donor;
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-6s %-22s %-8s %-16s %-13s %-18s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Available",
           "Approval");
    printLine('-', 90);

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (approvalStatusFilter == -1 || donor.approvalStatus == approvalStatusFilter)
        {
            printf("%-6d %-22s %-8s %-16s %-13s %-18s\n",
                   donor.donorId,
                   donor.name,
                   donor.bloodGroup,
                   donor.phone,
                   getAvailabilityText(&donor),
                   getDonorApprovalStatusText(donor.approvalStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No matching donor records found.");
    }

    return found;
}

void printBloodGroupOptions(void)
{
    printSectionHeader("Available Blood Groups");
    printf("A+   A-   B+   B-   AB+   AB-   O+   O-\n");
    printLine('-', 72);
}

void printDonorAreaOptions(void)
{
    FILE *file;
    Donor donor;
    char areas[200][100];
    int areaCount = 0;
    int exists;
    int i;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        exists = 0;

        for (i = 0; i < areaCount; i++)
        {
            if (strcmp(areas[i], donor.address) == 0)
            {
                exists = 1;
                break;
            }
        }

        if (!exists && areaCount < 200)
        {
            strncpy(areas[areaCount], donor.address, sizeof(areas[areaCount]) - 1);
            areas[areaCount][sizeof(areas[areaCount]) - 1] = '\0';
            areaCount++;
        }
    }

    fclose(file);

    printSectionHeader("Available Donor Areas");

    if (areaCount == 0)
    {
        printStatusMessage("INFO", "No donor areas found.");
        return;
    }

    for (i = 0; i < areaCount; i++)
    {
        printf("%2d. %s\n", i + 1, areas[i]);
    }

    printLine('-', 72);
}

void displayDonor(const Donor *donor)
{
    printLine('-', 72);
    printf("%-24s : %d\n", "Donor ID", donor->donorId);
    printf("%-24s : %s\n", "Name", donor->name);
    printf("%-24s : %d\n", "Age", donor->age);
    printf("%-24s : %s\n", "Gender", donor->gender);
    printf("%-24s : %s\n", "Blood Group", donor->bloodGroup);
    printf("%-24s : %s\n", "Phone", donor->phone);
    printf("%-24s : %s\n", "Email", donor->email);
    printf("%-24s : %s\n", "Address", donor->address);
    printf("%-24s : %s\n", "Last Donation Date", donor->lastDonationDate);
    printf("%-24s : %s\n", "Medical Note", donor->medicalNote);
    printf("%-24s : %s\n", "Availability", getAvailabilityText(donor));
    printf("%-24s : %s\n", "Account Status", getDonorApprovalStatusText(donor->approvalStatus));
    printf("%-24s : %d\n", "Donation Count", donor->donationCount);
    printLine('-', 72);
}

void displayRequest(const Request *request)
{
    printLine('-', 72);
    printf("%-24s : %d\n", "Request ID", request->requestId);
    printf("%-24s : %s\n", "Requester Name", request->requesterName);
    printf("%-24s : %s\n", "Patient Name", request->patientName);
    printf("%-24s : %s\n", "Blood Group Needed", request->bloodGroupNeeded);
    printf("%-24s : %d\n", "Required Bags", request->requiredBags);
    printf("%-24s : %d\n", "Verified Bags", request->verifiedBags);
    printf("%-24s : %s\n", "Hospital Name", request->hospitalName);
    printf("%-24s : %s\n", "Location", request->location);
    printf("%-24s : %s\n", "Contact Number", request->contactNumber);
    printf("%-24s : %s\n", "Urgency Level", getUrgencyText(request->urgencyLevel));
    printf("%-24s : %s\n", "Tracking PIN", request->trackingPIN);
    printf("%-24s : %s\n", "Request Status", getRequestStatusText(request->requestStatus));
    printLine('-', 72);
}

int printRequestSelectionList(const char *title, int includeClosedRequests)
{
    FILE *file;
    Request request;
    int found = 0;
    int assignedBags;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-8s %-18s %-8s %-6s %-8s %-6s %-10s %-14s\n",
           "Req ID",
           "Patient",
           "Group",
           "Need",
           "Assigned",
           "Done",
           "Urgency",
           "Status");
    printLine('-', 90);

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (includeClosedRequests ||
            (request.requestStatus != REQUEST_CANCELLED &&
             request.requestStatus != REQUEST_FULFILLED))
        {
            assignedBags = countRequestActiveAssigned(request.requestId);
            printf("%-8d %-18s %-8s %-6d %-8d %-6d %-10s %-14s\n",
                   request.requestId,
                   request.patientName,
                   request.bloodGroupNeeded,
                   request.requiredBags,
                   assignedBags,
                   request.verifiedBags,
                   getUrgencyText(request.urgencyLevel),
                   getRequestStatusShortText(request.requestStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No matching request records found.");
    }

    return found;
}

int printRequestsWithInterestedDonors(const char *title)
{
    FILE *file;
    Request request;
    int found = 0;
    int interestedCount;
    int activeCount;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-8s %-18s %-8s %-6s %-10s %-6s %-22s %-14s\n",
           "Req ID",
           "Patient",
           "Group",
           "Need",
           "Interested",
           "Active",
           "Location",
           "Status");
    printLine('-', 100);

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        interestedCount = countRequestAssignmentsByStatus(request.requestId, ASSIGNMENT_INTERESTED);
        activeCount = countRequestActiveAssigned(request.requestId);

        if (interestedCount > 0)
        {
            printf("%-8d %-18s %-8s %-6d %-10d %-6d %-22s %-14s\n",
                   request.requestId,
                   request.patientName,
                   request.bloodGroupNeeded,
                   request.requiredBags,
                   interestedCount,
                   activeCount,
                   request.location,
                   getRequestStatusShortText(request.requestStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No requests currently have interested donors.");
    }

    return found;
}

void displayDonationRecord(const DonationRecord *record)
{
    printLine('-', 72);
    printf("%-24s : %d\n", "Donation ID", record->donationId);
    printf("%-24s : %d\n", "Request ID", record->requestId);
    printf("%-24s : %d\n", "Donor ID", record->donorId);
    printf("%-24s : %s\n", "Donor Name", record->donorName);
    printf("%-24s : %s\n", "Requester Name", record->requesterName);
    printf("%-24s : %s\n", "Patient Name", record->patientName);
    printf("%-24s : %s\n", "Blood Group", record->bloodGroup);
    printf("%-24s : %d\n", "Units Donated", record->unitsDonated);
    printf("%-24s : %s\n", "Donation Date", record->donationDate);
    printf("%-24s : %s\n", "Requester Confirmed", record->requesterConfirmed ? "Yes" : "No");
    printf("%-24s : %s\n", "Admin Verified", record->adminVerified ? "Yes" : "No");
    printLine('-', 72);
}

void displayAssignment(const DonorAssignment *assignment)
{
    printLine('-', 72);
    printf("%-24s : %d\n", "Assignment ID", assignment->assignmentId);
    printf("%-24s : %d\n", "Request ID", assignment->requestId);
    printf("%-24s : %d\n", "Donor ID", assignment->donorId);
    printf("%-24s : %s\n", "Status", getAssignmentStatusText(assignment->status));
    printf("%-24s : %d\n", "Bags", assignment->bags);
    printf("%-24s : %s\n", "Requester Confirmed", assignment->requesterConfirmed ? "Yes" : "No");
    printf("%-24s : %s\n", "Admin Verified", assignment->adminVerified ? "Yes" : "No");
    printf("%-24s : %s\n", "Interested At", assignment->interestedDate);
    printf("%-24s : %s\n", "Assigned At", assignment->assignedDate);
    printf("%-24s : %s\n", "Donation Done At", assignment->donationDoneDate);
    printf("%-24s : %s\n", "Verified At", assignment->verifiedDate);
    printLine('-', 72);
}

int printAssignmentSelectionList(const char *title, int statusFilter)
{
    FILE *file;
    DonorAssignment assignment;
    Donor donor;
    Request request;
    int found = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        return 0;
    }

    printSectionHeader(title);
    printf("%-8s %-7s %-7s %-18s %-6s %-14s %-9s %-7s %-14s\n",
           "Assign",
           "Req ID",
           "Donor",
           "Name",
           "Group",
           "Assign Status",
           "Requester",
           "Admin",
           "Request Status");
    printLine('-', 100);

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (statusFilter == -1 || assignment.status == statusFilter)
        {
            if (!findDonorById(assignment.donorId, &donor))
            {
                memset(&donor, 0, sizeof(Donor));
                strcpy(donor.name, "Unknown");
                strcpy(donor.bloodGroup, "-");
            }

            if (!findRequestById(assignment.requestId, &request))
            {
                memset(&request, 0, sizeof(Request));
                request.requestStatus = REQUEST_PENDING;
            }

            printf("%-8d %-7d %-7d %-18s %-6s %-14s %-9s %-7s %-14s\n",
                   assignment.assignmentId,
                   assignment.requestId,
                   assignment.donorId,
                   donor.name,
                   donor.bloodGroup,
                   getAssignmentStatusShortText(assignment.status),
                   assignment.requesterConfirmed ? "Yes" : "No",
                   assignment.adminVerified ? "Yes" : "No",
                   getRequestStatusShortText(request.requestStatus));
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No matching assignment records found.");
    }

    return found;
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
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");
    printf("Assigned Donor ID : %d\n", donor.donorId);

    if (!getTextInputOrBack("Enter donor name: ", donor.name, sizeof(donor.name)) ||
        !inputAgeOrBack(&donor.age) ||
        !getTextInputOrBack("Enter gender: ", donor.gender, sizeof(donor.gender)) ||
        !inputBloodGroupOrBack("Enter blood group: ", donor.bloodGroup, sizeof(donor.bloodGroup)))
    {
        fclose(file);
        printStatusMessage("INFO", "Add donor cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    do
    {
        if (!inputPhoneOrBack("Enter phone number: ", donor.phone, sizeof(donor.phone)) ||
            !inputEmailOrBack("Enter email: ", donor.email, sizeof(donor.email)))
        {
            fclose(file);
            printStatusMessage("INFO", "Add donor cancelled. No information was saved.");
            pauseScreen();
            return;
        }

        if (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, 0))
        {
            printStatusMessage("ERROR", "Phone or email already exists. Please use unique contact information.");
        }
    } while (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, 0));

    if (!getTextInputOrBack("Enter address/area: ", donor.address, sizeof(donor.address)) ||
        !inputDateOrBack("Enter last donation date (YYYY-MM-DD or Never): ", donor.lastDonationDate, sizeof(donor.lastDonationDate)) ||
        !getTextInputOrBack("Enter medical note: ", donor.medicalNote, sizeof(donor.medicalNote)) ||
        !getTextInputOrBack("Enter security question: ", donor.securityQuestion, sizeof(donor.securityQuestion)) ||
        !getTextInputOrBack("Enter security answer: ", donor.securityAnswer, sizeof(donor.securityAnswer)) ||
        !inputAvailabilityOrBack("Enter availability status (1 for available, 0 for unavailable): ",
                            donor.availabilityStatus,
                            sizeof(donor.availabilityStatus)))
    {
        fclose(file);
        printStatusMessage("INFO", "Add donor cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    do
    {
        if (!inputPasswordOrBack("Create initial donor password: ", donor.password, sizeof(donor.password)))
        {
            fclose(file);
            printStatusMessage("INFO", "Add donor cancelled. No information was saved.");
            pauseScreen();
            return;
        }
    } while (!isValidPassword(donor.password));

    donor.donationCount = 0;
    donor.approvalStatus = DONOR_APPROVED;

    /* Save the whole donor record in binary format. */
    if (fwrite(&donor, sizeof(Donor), 1, file) != 1)
    {
        printStatusMessage("ERROR", "Failed to save donor information.");
        fclose(file);
        pauseScreen();
        return;
    }

    fclose(file);

    printStatusMessage("SUCCESS", "Donor account created successfully.");
    displayDonor(&donor);
    writeActivityLog("Added a new donor record.");
    pauseScreen();
}

void viewAllDonors(void)
{
    printDonorSelectionList("All Donors", -1);
    pauseScreen();
}

void searchDonorById(void)
{
    FILE *file;
    Donor donor;
    int searchId;
    int found = 0;

    if (!printDonorSelectionList("Choose Donor To Search", -1))
    {
        pauseScreen();
        return;
    }

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    if (!readIntOrBack("\nEnter donor ID to search (0 to back): ", &searchId))
    {
        fclose(file);
        printStatusMessage("INFO", "Donor search cancelled.");
        pauseScreen();
        return;
    }

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

void searchDonorByBloodGroupAdmin(void)
{
    FILE *file;
    Donor donor;
    char bloodGroup[5];
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    printBloodGroupOptions();

    if (!inputBloodGroupOrBack("\nEnter donor blood group to search (0 to back): ", bloodGroup, sizeof(bloodGroup)))
    {
        fclose(file);
        printStatusMessage("INFO", "Blood group search cancelled.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donors By Blood Group");

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (strcmp(donor.bloodGroup, bloodGroup) == 0)
        {
            displayDonor(&donor);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donors found for this blood group.");
    }

    pauseScreen();
}

void searchDonorByArea(void)
{
    FILE *file;
    Donor donor;
    char area[100];
    int found = 0;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        pauseScreen();
        return;
    }

    printDonorAreaOptions();

    if (!getTextInputOrBack("\nEnter area/location to search (0 to back): ", area, sizeof(area)))
    {
        fclose(file);
        printStatusMessage("INFO", "Area search cancelled.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donors By Area");

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (strstr(donor.address, area) != NULL)
        {
            displayDonor(&donor);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donors found for this area.");
    }

    pauseScreen();
}

void searchDonorByApprovalStatus(void)
{
    int status;
    int changeChoice;
    int donorId;
    int newStatus;

    printSectionHeader("Approval Status Options");
    printf("  0. Pending Approval\n");
    printf("  1. Approved\n");
    printf("  2. Rejected\n");
    printf("  3. Blocked\n");
    printf("  4. All Statuses\n");
    printf("  9. Back\n");
    printLine('-', 72);

    printf("Enter approval status to search: ");
    while (scanf("%d", &status) != 1 ||
           !((status >= DONOR_PENDING_APPROVAL && status <= 4) || status == 9))
    {
        printf("Invalid status. Enter 0, 1, 2, 3, 4, or 9 to back: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    if (status == 9)
    {
        printStatusMessage("INFO", "Approval status search cancelled.");
        pauseScreen();
        return;
    }

    if (status == 4)
    {
        if (!printDonorSelectionList("All Donors By Approval Status", -1))
        {
            pauseScreen();
            return;
        }
    }
    else
    {
        printf("%-24s : %s\n", "Status Filter", getDonorApprovalStatusText(status));
        if (!printDonorSelectionList("Donors By Approval Status", status))
        {
            pauseScreen();
            return;
        }
    }

    if (!readIntInRangeOrBack("Change approval status now? (1=Yes, 0=Back): ", &changeChoice, 0, 1))
    {
        return;
    }

    if (changeChoice == 0)
    {
        return;
    }

    if (!readIntOrBack("Enter donor ID to change status (0 to back): ", &donorId))
    {
        return;
    }

    printSectionHeader("New Approval Status");
    printf("  0. Pending Approval\n");
    printf("  1. Approved\n");
    printf("  2. Rejected\n");
    printf("  3. Blocked\n");
    printf("  9. Back\n");
    printLine('-', 72);

    printf("Enter new approval status: ");
    while (scanf("%d", &newStatus) != 1 ||
           !((newStatus >= DONOR_PENDING_APPROVAL && newStatus <= DONOR_BLOCKED) || newStatus == 9))
    {
        printf("Invalid status. Enter 0, 1, 2, 3, or 9 to back: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    if (newStatus == 9)
    {
        printStatusMessage("INFO", "Approval status change cancelled.");
        pauseScreen();
        return;
    }

    if (updateDonorApprovalStatusById(donorId, newStatus))
    {
        printStatusMessage("SUCCESS", "Donor approval status changed successfully.");
    }
    else
    {
        printStatusMessage("ERROR", "Failed to change donor approval status.");
    }

    pauseScreen();
}

int updateDonorApprovalStatusById(int donorId, int newStatus)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int found = 0;
    char logMessage[120];

    sourceFile = fopen(DONOR_FILE, "rb");
    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        return 0;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            donor.approvalStatus = newStatus;

            if (newStatus == DONOR_APPROVED)
            {
                strcpy(donor.availabilityStatus, "1");
            }
            else
            {
                strcpy(donor.availabilityStatus, "0");
            }
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        return 0;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        return 0;
    }

    snprintf(logMessage,
             sizeof(logMessage),
             "Changed donor ID %d approval status to %s.",
             donorId,
             getDonorApprovalStatusText(newStatus));
    writeActivityLog(logMessage);
    return 1;
}

void updateDonor(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int searchId;
    int found = 0;

    if (!printDonorSelectionList("Choose Donor To Update", -1))
    {
        pauseScreen();
        return;
    }

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

    if (!readIntOrBack("\nEnter donor ID to update (0 to back): ", &searchId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Donor update cancelled.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == searchId)
        {
            found = 1;

            printSectionHeader("Current Donor Information");
            displayDonor(&donor);
            printStatusMessage("INFO", "Enter the updated donor information below.");

            getTextInput("Enter donor name: ", donor.name, sizeof(donor.name));

            if (!inputAgeOrBack(&donor.age))
            {
                fclose(sourceFile);
                fclose(tempFile);
                remove(TEMP_DONOR_FILE);
                printStatusMessage("INFO", "Donor update cancelled. No information was saved.");
                pauseScreen();
                return;
            }

            getTextInput("Enter gender: ", donor.gender, sizeof(donor.gender));
            inputBloodGroup("Enter blood group: ", donor.bloodGroup, sizeof(donor.bloodGroup));
            do
            {
                if (!inputPhoneOrBack("Enter phone number: ", donor.phone, sizeof(donor.phone)) ||
                    !inputEmailOrBack("Enter email: ", donor.email, sizeof(donor.email)))
                {
                    fclose(sourceFile);
                    fclose(tempFile);
                    remove(TEMP_DONOR_FILE);
                    printStatusMessage("INFO", "Donor update cancelled. No information was saved.");
                    pauseScreen();
                    return;
                }

                if (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, donor.donorId))
                {
                    printStatusMessage("ERROR", "Phone or email already exists. Please use unique contact information.");
                }
            } while (isDuplicateDonorPhoneOrEmail(donor.phone, donor.email, donor.donorId));
            getTextInput("Enter address: ", donor.address, sizeof(donor.address));
            if (!inputDateOrBack("Enter last donation date (YYYY-MM-DD or Never): ", donor.lastDonationDate, sizeof(donor.lastDonationDate)))
            {
                fclose(sourceFile);
                fclose(tempFile);
                remove(TEMP_DONOR_FILE);
                printStatusMessage("INFO", "Donor update cancelled. No information was saved.");
                pauseScreen();
                return;
            }
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

    printStatusMessage("SUCCESS", "Donor information updated successfully.");
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

    if (!printDonorSelectionList("Choose Donor To Delete", -1))
    {
        pauseScreen();
        return;
    }

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

    if (!readIntOrBack("\nEnter donor ID to delete (0 to back): ", &searchId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Donor delete cancelled.");
        pauseScreen();
        return;
    }

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

    if (!cancelAssignmentsForDonor(searchId))
    {
        printStatusMessage("ERROR", "Donor was deleted, but related assignment cleanup failed.");
        writeActivityLog("Deleted donor with assignment cleanup error.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Donor record deleted successfully.");
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

    if (!printDonorSelectionList("Choose Donor For Availability Change", -1))
    {
        pauseScreen();
        return;
    }

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

    if (!readIntOrBack("\nEnter donor ID to change availability (0 to back): ", &searchId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Availability update cancelled.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == searchId)
        {
            found = 1;

            printSectionHeader("Current Donor Information");
            displayDonor(&donor);
            if (!inputAvailabilityOrBack("Enter new availability status (1 for available, 0 for unavailable): ",
                                         donor.availabilityStatus,
                                         sizeof(donor.availabilityStatus)))
            {
                fclose(sourceFile);
                fclose(tempFile);
                remove(TEMP_DONOR_FILE);
                printStatusMessage("INFO", "Availability update cancelled. No information was saved.");
                pauseScreen();
                return;
            }
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

void viewPendingDonorRegistrations(void)
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

    printSectionHeader("Pending Donor Registrations");

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.approvalStatus == DONOR_PENDING_APPROVAL)
        {
            displayDonor(&donor);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No pending donor registrations found.");
    }

    pauseScreen();
}

void approveOrRejectDonorRegistration(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int donorId;
    int choice;
    int found = 0;
    int updated = 0;

    if (!printDonorSelectionList("Choose Donor For Approval Status", -1))
    {
        pauseScreen();
        return;
    }

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

    printSectionHeader("Manage Donor Approval Status");
    if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_DONOR_FILE);
        printStatusMessage("INFO", "Approval update cancelled.");
        pauseScreen();
        return;
    }

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            displayDonor(&donor);

            printf("  1. Approve\n");
            printf("  2. Reject\n");
            printf("  3. Block\n");
            printf("  4. Unblock / Approve\n");
            if (!readIntInRangeOrBack("Enter choice (0 to back): ", &choice, 1, 4))
            {
                fclose(sourceFile);
                fclose(tempFile);
                remove(TEMP_DONOR_FILE);
                printStatusMessage("INFO", "Approval update cancelled.");
                pauseScreen();
                return;
            }

            if (choice == 1)
            {
                donor.approvalStatus = DONOR_APPROVED;
                printStatusMessage("SUCCESS", "Donor registration approved.");
                writeActivityLog("Admin approved a donor registration.");
            }
            else if (choice == 2)
            {
                donor.approvalStatus = DONOR_REJECTED;
                strcpy(donor.availabilityStatus, "0");
                printStatusMessage("SUCCESS", "Donor registration rejected.");
                writeActivityLog("Admin rejected a donor registration.");
            }
            else if (choice == 3)
            {
                donor.approvalStatus = DONOR_BLOCKED;
                strcpy(donor.availabilityStatus, "0");
                printStatusMessage("SUCCESS", "Donor account blocked.");
                writeActivityLog("Admin blocked a donor account.");
            }
            else
            {
                donor.approvalStatus = DONOR_APPROVED;
                printStatusMessage("SUCCESS", "Donor account unblocked and approved.");
                writeActivityLog("Admin unblocked a donor account.");
            }

            updated = 1;
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            printStatusMessage("ERROR", "Failed to update donor registration status.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        printf("\n[INFO] No donor found with ID %d.\n", donorId);
        pauseScreen();
        return;
    }

    if (!updated)
    {
        remove(TEMP_DONOR_FILE);
        pauseScreen();
        return;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace donor file after approval update.");
        pauseScreen();
        return;
    }

    writeActivityLog("Updated donor registration approval status.");
    pauseScreen();
}

void addRequest(void)
{
    FILE *file;
    Request request;
    int urgencyChoice;

    request.requestId = generateNextRequestId();
    file = fopen(REQUEST_FILE, "ab");

    if (file == NULL)
    {
        printStatusMessage("ERROR", "Unable to open request file.");
        pauseScreen();
        return;
    }

    printSectionHeader("Create Emergency Blood Request");
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");
    printf("Assigned Request ID : %d\n", request.requestId);

    if (!getTextInputOrBack("Enter requester name: ", request.requesterName, sizeof(request.requesterName)) ||
        !getTextInputOrBack("Enter patient name: ", request.patientName, sizeof(request.patientName)) ||
        !inputBloodGroupOrBack("Enter blood group needed: ",
                               request.bloodGroupNeeded,
                               sizeof(request.bloodGroupNeeded)) ||
        !inputRequiredBagsOrBack(&request.requiredBags))
    {
        fclose(file);
        printStatusMessage("INFO", "Blood request cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    if (!getTextInputOrBack("Enter hospital name: ", request.hospitalName, sizeof(request.hospitalName)) ||
        !getTextInputOrBack("Enter location: ", request.location, sizeof(request.location)) ||
        !inputPhoneOrBack("Enter contact number: ", request.contactNumber, sizeof(request.contactNumber)))
    {
        fclose(file);
        printStatusMessage("INFO", "Blood request cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter urgency level (1=Low, 2=Medium, 3=High, 4=Critical): ", &urgencyChoice))
    {
        fclose(file);
        printStatusMessage("INFO", "Blood request cancelled. No information was saved.");
        pauseScreen();
        return;
    }

    while (!isValidUrgencyChoice(urgencyChoice))
    {
        if (!readIntOrBack("Invalid urgency. Enter 1, 2, 3, 4, or 0 to go back: ", &urgencyChoice))
        {
            fclose(file);
            printStatusMessage("INFO", "Blood request cancelled. No information was saved.");
            pauseScreen();
            return;
        }
    }
    snprintf(request.urgencyLevel, sizeof(request.urgencyLevel), "%d", urgencyChoice);
    generateRequestPin(request.trackingPIN, sizeof(request.trackingPIN));
    request.verifiedBags = 0;
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

    printStatusMessage("SUCCESS", "Emergency blood request saved successfully.");
    printLine('-', 72);
    printf("%-24s : %d\n", "Request ID", request.requestId);
    printf("%-24s : %s\n", "Tracking PIN", request.trackingPIN);
    printLine('-', 72);
    printf("Save both values. You need them to track the request and confirm donation.\n");
    displayRequest(&request);
    writeActivityLog("Added a new emergency request.");
    pauseScreen();
}

void viewAllRequests(void)
{
    printRequestSelectionList("All Emergency Requests", 1);
    pauseScreen();
}

void searchRequestById(void)
{
    FILE *file;
    Request request;
    int searchId;
    int found = 0;

    if (!printRequestSelectionList("Choose Request To Search", 1))
    {
        pauseScreen();
        return;
    }

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    if (!readIntOrBack("\nEnter request ID to search (0 to back): ", &searchId))
    {
        fclose(file);
        printStatusMessage("INFO", "Request search cancelled.");
        pauseScreen();
        return;
    }

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
    Request request;
    int searchId;

    if (!printRequestSelectionList("Choose Request To Recalculate", 1))
    {
        pauseScreen();
        return;
    }

    printSectionHeader("Recalculate Request Status");
    printf("Request status is now calculated from donor assignments and verified bags.\n");

    if (!readIntOrBack("Enter request ID (0 to go back): ", &searchId))
    {
        return;
    }

    if (!findRequestById(searchId, &request))
    {
        printStatusMessage("INFO", "No request found with that ID.");
        pauseScreen();
        return;
    }

    displayRequest(&request);

    if (request.requestStatus == REQUEST_CANCELLED)
    {
        printStatusMessage("INFO", "Cancelled requests are not recalculated.");
        pauseScreen();
        return;
    }

    if (countRequestAssignmentsByStatus(searchId, ASSIGNMENT_VERIFIED) > 0)
    {
        updateRequestStatusAfterVerification(searchId);
    }
    else
    {
        updateRequestStatusAfterAssignment(searchId);
    }

    printStatusMessage("SUCCESS", "Request status recalculated from assignment records.");
    writeActivityLog("Recalculated request status from assignment records.");
    pauseScreen();
}

void deleteRequest(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int searchId;
    int found = 0;

    if (!printRequestSelectionList("Choose Request To Delete", 1))
    {
        pauseScreen();
        return;
    }

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

    if (!readIntOrBack("\nEnter request ID to delete (0 to back): ", &searchId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_REQUEST_FILE);
        printStatusMessage("INFO", "Request delete cancelled.");
        pauseScreen();
        return;
    }

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

    if (!cancelAssignmentsForRequest(searchId))
    {
        printStatusMessage("ERROR", "Request was deleted, but related assignment cleanup failed.");
        writeActivityLog("Deleted request with assignment cleanup error.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request deleted successfully.");
    writeActivityLog("Deleted an emergency request.");
    pauseScreen();
}

void cancelRequestByAdmin(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int requestId;
    int found = 0;
    int cancelled = 0;

    if (!printRequestSelectionList("Choose Request To Cancel", 0))
    {
        pauseScreen();
        return;
    }

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

    if (!readIntOrBack("\nEnter request ID to cancel (0 to back): ", &requestId))
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_REQUEST_FILE);
        printStatusMessage("INFO", "Request cancellation cancelled.");
        pauseScreen();
        return;
    }

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId)
        {
            found = 1;

            if (request.requestStatus != REQUEST_FULFILLED)
            {
                request.requestStatus = REQUEST_CANCELLED;
                cancelled = 1;
            }
            else
            {
                printStatusMessage("ERROR", "Fulfilled request cannot be cancelled.");
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            printStatusMessage("ERROR", "Failed to cancel request.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        printf("\n[INFO] No request found with ID %d.\n", requestId);
        pauseScreen();
        return;
    }

    if (!cancelled)
    {
        remove(TEMP_REQUEST_FILE);
        pauseScreen();
        return;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to replace request file after cancellation.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Request cancelled successfully.");
    cancelAssignmentsForRequest(requestId);
    writeActivityLog("Admin cancelled an emergency request.");
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

    printBloodGroupOptions();

    if (!inputBloodGroupOrBack("\nEnter blood group to match (0 to back): ", bloodGroup, sizeof(bloodGroup)))
    {
        fclose(donorFile);
        printStatusMessage("INFO", "Blood group match cancelled.");
        pauseScreen();
        return;
    }

    printSectionHeader("Matched Available Donors");
    printf("%-18s : %s\n", "Blood Group Filter", bloodGroup);
    printf("%-18s : %s\n", "Eligibility Rule", "Approved and available donors only");
    printLine('-', 100);
    printf("%-8s %-20s %-8s %-15s %-24s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Donor Area",
           "Status");
    printLine('-', 100);

    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
    {
        if (isCompatible(donor.bloodGroup, bloodGroup) && isDonorEligibleForAssignment(&donor))
        {
            printMatchedDonorRow(&donor);
            found = 1;
        }
    }

    if (!found)
    {
        printf("[INFO] No available donors found for blood group %s.\n", bloodGroup);
    }

    printLine('=', 100);
    fclose(donorFile);
    writeActivityLog("Matched donors by blood group.");

    if (found)
    {
        offerAssignmentAfterMatch(0);
    }

    pauseScreen();
}

void matchDonorsByRequestId(void)
{
    FILE *requestFile;
    Request request;
    int requestId;
    int requestFound = 0;

    if (!printRequestSelectionList("Choose Request To Match Donors", 0))
    {
        pauseScreen();
        return;
    }

    requestFile = fopen(REQUEST_FILE, "rb");

    if (requestFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    if (!readIntOrBack("\nEnter request ID to find matching donors (0 to back): ", &requestId))
    {
        fclose(requestFile);
        printStatusMessage("INFO", "Request match cancelled.");
        pauseScreen();
        return;
    }

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

    printSectionHeader("Donor Match Result");
    printf("%-18s : %d\n", "Request ID", request.requestId);
    printf("%-18s : %s\n", "Patient Name", request.patientName);
    printf("%-18s : %s\n", "Blood Group Needed", request.bloodGroupNeeded);
    printf("%-18s : %d\n", "Required Bags", request.requiredBags);
    printf("%-18s : %s\n", "Urgency Level", getUrgencyText(request.urgencyLevel));
    printf("%-18s : %s\n", "Request Status", getRequestStatusText(request.requestStatus));

    if (printMatchingDonorsForRequest(&request))
    {
        offerAssignmentAfterMatch(request.requestId);
    }

    printLine('=', 76);
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

int printMatchingDonorsForRequest(const Request *request)
{
    FILE *donorFile;
    Donor donor;
    int donorFound = 0;

    if (request == NULL)
    {
        return 0;
    }

    donorFile = fopen(DONOR_FILE, "rb");

    if (donorFile == NULL)
    {
        printStatusMessage("INFO", "No donor records found yet.");
        return 0;
    }

    printf("%-18s : %s\n", "Hospital", request->hospitalName);
    printf("%-18s : %s\n", "Request Location", request->location);
    printf("%-18s : %s\n", "Nearby Tip", "Compare Request Location with Donor Area");
    printLine('-', 100);
    printf("%-8s %-20s %-8s %-15s %-24s %-12s\n",
           "ID",
           "Name",
           "Group",
           "Phone",
           "Donor Area",
           "Status");
    printLine('-', 100);

    while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
    {
        if (isCompatible(donor.bloodGroup, request->bloodGroupNeeded) &&
            isDonorEligibleForAssignment(&donor))
        {
            printMatchedDonorRow(&donor);
            donorFound = 1;
        }
    }

    fclose(donorFile);

    if (!donorFound)
    {
        printf("[INFO] No available donors found for request ID %d.\n", request->requestId);
    }

    return donorFound;
}

int printMatchingRequestsForDonor(const Donor *donor)
{
    FILE *requestFile;
    Request request;
    int found = 0;
    int assignedBags;

    if (donor == NULL)
    {
        return 0;
    }

    requestFile = fopen(REQUEST_FILE, "rb");

    if (requestFile == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        return 0;
    }

    printSectionHeader("Matching Requests for Selected Donor");
    printf("%-18s : %d\n", "Donor ID", donor->donorId);
    printf("%-18s : %s\n", "Donor Name", donor->name);
    printf("%-18s : %s\n", "Blood Group", donor->bloodGroup);
    printf("%-18s : %s\n", "Donor Area", donor->address);
    printLine('-', 105);
    printf("%-8s %-18s %-8s %-6s %-8s %-6s %-22s %-14s\n",
           "Req ID",
           "Patient",
           "Needed",
           "Need",
           "Assigned",
           "Done",
           "Request Location",
           "Status");
    printLine('-', 105);

    while (fread(&request, sizeof(Request), 1, requestFile) == 1)
    {
        if (isCompatible(donor->bloodGroup, request.bloodGroupNeeded) &&
            request.requestStatus != REQUEST_CANCELLED &&
            request.requestStatus != REQUEST_FULFILLED)
        {
            assignedBags = countRequestActiveAssigned(request.requestId);

            if (assignedBags < request.requiredBags)
            {
                printf("%-8d %-18s %-8s %-6d %-8d %-6d %-22s %-14s\n",
                       request.requestId,
                       request.patientName,
                       request.bloodGroupNeeded,
                       request.requiredBags,
                       assignedBags,
                       request.verifiedBags,
                       request.location,
                       getRequestStatusShortText(request.requestStatus));
                found = 1;
            }
        }
    }

    fclose(requestFile);

    if (!found)
    {
        printStatusMessage("INFO", "No open compatible requests found for this donor.");
    }

    return found;
}

int findDonorById(int donorId, Donor *foundDonor)
{
    FILE *file;
    Donor donor;

    file = fopen(DONOR_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&donor, sizeof(Donor), 1, file) == 1)
    {
        if (donor.donorId == donorId)
        {
            if (foundDonor != NULL)
            {
                *foundDonor = donor;
            }

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int findRequestById(int requestId, Request *foundRequest)
{
    FILE *file;
    Request request;

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestId == requestId)
        {
            if (foundRequest != NULL)
            {
                *foundRequest = request;
            }

            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int isDonorAssignedToActiveRequest(int donorId, int currentRequestId)
{
    FILE *file;
    DonorAssignment assignment;
    Request request;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.donorId == donorId &&
            assignment.requestId != currentRequestId &&
            (assignment.status == ASSIGNMENT_ASSIGNED ||
             assignment.status == ASSIGNMENT_WAITING_VERIFICATION) &&
            findRequestById(assignment.requestId, &request) &&
            request.requestStatus != REQUEST_FULFILLED &&
            request.requestStatus != REQUEST_CANCELLED)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int donorInterestExists(int requestId, int donorId)
{
    FILE *file;
    DonorAssignment assignment;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.requestId == requestId &&
            assignment.donorId == donorId &&
            assignment.status != ASSIGNMENT_REPLACED &&
            assignment.status != ASSIGNMENT_CANCELLED)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int countRequestAssignmentsByStatus(int requestId, int status)
{
    FILE *file;
    DonorAssignment assignment;
    int count = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.requestId == requestId && assignment.status == status)
        {
            count += assignment.bags > 0 ? assignment.bags : 1;
        }
    }

    fclose(file);
    return count;
}

int countRequestActiveAssigned(int requestId)
{
    FILE *file;
    DonorAssignment assignment;
    int count = 0;

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.requestId == requestId &&
            (assignment.status == ASSIGNMENT_ASSIGNED ||
             assignment.status == ASSIGNMENT_WAITING_VERIFICATION ||
             assignment.status == ASSIGNMENT_VERIFIED))
        {
            count += assignment.bags > 0 ? assignment.bags : 1;
        }
    }

    fclose(file);
    return count;
}

int updateRequestStatusAfterAssignment(int requestId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int found = 0;
    int assignedBags;
    int interestedCount;
    int requiredBags;
    int verifiedBags;

    sourceFile = fopen(REQUEST_FILE, "rb");
    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        return 0;
    }

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId)
        {
            found = 1;
            requiredBags = request.requiredBags;
            verifiedBags = countRequestAssignmentsByStatus(requestId, ASSIGNMENT_VERIFIED);
            assignedBags = countRequestActiveAssigned(requestId);
            interestedCount = countRequestAssignmentsByStatus(requestId, ASSIGNMENT_INTERESTED);
            request.verifiedBags = verifiedBags;

            if (request.requestStatus != REQUEST_CANCELLED)
            {
                if (verifiedBags >= requiredBags)
                {
                    request.requestStatus = REQUEST_FULFILLED;
                }
                else if (verifiedBags > 0)
                {
                    request.requestStatus = REQUEST_PARTIALLY_FULFILLED;
                }
                else if (assignedBags >= requiredBags)
                {
                    request.requestStatus = REQUEST_FULLY_MATCHED;
                }
                else if (assignedBags > 0)
                {
                    request.requestStatus = REQUEST_PARTIALLY_MATCHED;
                }
                else if (interestedCount > 0)
                {
                    request.requestStatus = REQUEST_DONOR_INTERESTED;
                }
                else
                {
                    request.requestStatus = REQUEST_PENDING;
                }
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        return 0;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

int updateRequestStatusAfterVerification(int requestId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Request request;
    int found = 0;
    int requiredBags;
    int verifiedBags;

    sourceFile = fopen(REQUEST_FILE, "rb");
    tempFile = fopen(TEMP_REQUEST_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        return 0;
    }

    while (fread(&request, sizeof(Request), 1, sourceFile) == 1)
    {
        if (request.requestId == requestId)
        {
            found = 1;
            requiredBags = request.requiredBags;
            verifiedBags = countRequestAssignmentsByStatus(requestId, ASSIGNMENT_VERIFIED);
            request.verifiedBags = verifiedBags;

            if (request.requestStatus != REQUEST_CANCELLED)
            {
                if (verifiedBags >= requiredBags)
                {
                    request.requestStatus = REQUEST_FULFILLED;
                }
                else if (verifiedBags > 0)
                {
                    request.requestStatus = REQUEST_PARTIALLY_FULFILLED;
                }
                else
                {
                    request.requestStatus = REQUEST_PENDING;
                }
            }
        }

        if (fwrite(&request, sizeof(Request), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_REQUEST_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_REQUEST_FILE);
        return 0;
    }

    if (remove(REQUEST_FILE) != 0 || rename(TEMP_REQUEST_FILE, REQUEST_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

int cancelAssignmentsForRequest(int requestId)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");

    if (sourceFile == NULL)
    {
        return 1;
    }

    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.requestId == requestId &&
            assignment.status != ASSIGNMENT_VERIFIED &&
            assignment.status != ASSIGNMENT_REPLACED)
        {
            assignment.status = ASSIGNMENT_CANCELLED;
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

int cancelAssignmentsForDonor(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");

    if (sourceFile == NULL)
    {
        return 1;
    }

    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.donorId == donorId &&
            assignment.status != ASSIGNMENT_VERIFIED &&
            assignment.status != ASSIGNMENT_REPLACED)
        {
            assignment.status = ASSIGNMENT_CANCELLED;
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        return 0;
    }

    return 1;
}

int createOrUpdateAssignment(int requestId, int donorId, int status)
{
    FILE *sourceFile;
    FILE *tempFile;
    FILE *appendFile;
    DonorAssignment assignment;
    int found = 0;
    char dateTime[30];

    getCurrentDateTime(dateTime, sizeof(dateTime));
    sourceFile = fopen(ASSIGNMENT_FILE, "rb");

    if (sourceFile == NULL)
    {
        appendFile = fopen(ASSIGNMENT_FILE, "ab");

        if (appendFile == NULL)
        {
            return 0;
        }

        memset(&assignment, 0, sizeof(DonorAssignment));
        assignment.assignmentId = generateNextAssignmentId();
        assignment.requestId = requestId;
        assignment.donorId = donorId;
        assignment.status = status;
        assignment.bags = 1;
        assignment.requesterConfirmed = 0;
        assignment.adminVerified = 0;
        strncpy(assignment.interestedDate, dateTime, sizeof(assignment.interestedDate) - 1);
        if (status == ASSIGNMENT_ASSIGNED)
        {
            strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
        }
        else if (status == ASSIGNMENT_VERIFIED)
        {
            assignment.requesterConfirmed = 1;
            assignment.adminVerified = 1;
            strncpy(assignment.verifiedDate, dateTime, sizeof(assignment.verifiedDate) - 1);
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, appendFile) != 1)
        {
            fclose(appendFile);
            return 0;
        }
        fclose(appendFile);
        updateRequestStatusAfterAssignment(requestId);
        return 1;
    }

    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (tempFile == NULL)
    {
        fclose(sourceFile);
        return 0;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.requestId == requestId &&
            assignment.donorId == donorId &&
            assignment.status != ASSIGNMENT_REPLACED &&
            assignment.status != ASSIGNMENT_CANCELLED)
        {
            found = 1;
            assignment.status = status;
            if (assignment.bags <= 0)
            {
                assignment.bags = 1;
            }
            if (status == ASSIGNMENT_ASSIGNED)
            {
                assignment.requesterConfirmed = 0;
                assignment.adminVerified = 0;
                strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
            }
            else if (status == ASSIGNMENT_WAITING_VERIFICATION)
            {
                assignment.requesterConfirmed = 0;
                assignment.adminVerified = 0;
                strncpy(assignment.donationDoneDate, dateTime, sizeof(assignment.donationDoneDate) - 1);
            }
            else if (status == ASSIGNMENT_VERIFIED)
            {
                assignment.requesterConfirmed = 1;
                assignment.adminVerified = 1;
                strncpy(assignment.verifiedDate, dateTime, sizeof(assignment.verifiedDate) - 1);
            }
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            return 0;
        }
    }

    if (!found)
    {
        memset(&assignment, 0, sizeof(DonorAssignment));
        assignment.assignmentId = generateNextAssignmentId();
        assignment.requestId = requestId;
        assignment.donorId = donorId;
        assignment.status = status;
        assignment.bags = 1;
        assignment.requesterConfirmed = 0;
        assignment.adminVerified = 0;
        strncpy(assignment.interestedDate, dateTime, sizeof(assignment.interestedDate) - 1);
        if (status == ASSIGNMENT_ASSIGNED)
        {
            strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
        }
        else if (status == ASSIGNMENT_VERIFIED)
        {
            assignment.requesterConfirmed = 1;
            assignment.adminVerified = 1;
            strncpy(assignment.verifiedDate, dateTime, sizeof(assignment.verifiedDate) - 1);
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        return 0;
    }

    updateRequestStatusAfterAssignment(requestId);
    return 1;
}

int assignDonorToRequestByIds(int requestId, int donorId)
{
    Request request;
    Donor donor;

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "No request found with that ID.");
        return 0;
    }

    if (request.requestStatus == REQUEST_CANCELLED || request.requestStatus == REQUEST_FULFILLED)
    {
        printStatusMessage("ERROR", "Cancelled or fulfilled requests cannot receive new assignments.");
        return 0;
    }

    if (!findDonorById(donorId, &donor))
    {
        printStatusMessage("ERROR", "No donor record exists with that ID.");
        return 0;
    }

    if (!isDonorEligibleForAssignment(&donor))
    {
        printStatusMessage("ERROR", "Donor must be approved and available.");
        return 0;
    }

    if (!isCompatible(donor.bloodGroup, request.bloodGroupNeeded))
    {
        printStatusMessage("ERROR", "Donor blood group does not match this request.");
        return 0;
    }

    if (isDonorAssignedToActiveRequest(donorId, requestId))
    {
        printStatusMessage("ERROR", "This donor is already assigned to another active request.");
        return 0;
    }

    if (countRequestActiveAssigned(requestId) >= request.requiredBags)
    {
        printStatusMessage("ERROR", "This request already has enough assigned donors. Add backup donor instead.");
        return 0;
    }

    if (!createOrUpdateAssignment(requestId, donorId, ASSIGNMENT_ASSIGNED))
    {
        printStatusMessage("ERROR", "Failed to save donor assignment.");
        return 0;
    }

    printStatusMessage("SUCCESS", "Donor assigned successfully. Request matching status updated.");
    writeActivityLog("Admin assigned donor to blood request.");
    return 1;
}

void offerAssignmentAfterMatch(int requestId)
{
    int choice;
    int donorId;
    Donor donor;

    if (!readIntInRangeOrBack("Assign donor now? (1=Yes, 0=No): ", &choice, 0, 1) || choice == 0)
    {
        return;
    }

    if (requestId == 0)
    {
        if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
        {
            return;
        }

        if (!findDonorById(donorId, &donor))
        {
            printStatusMessage("ERROR", "No donor record exists with that ID.");
            return;
        }

        if (!isDonorEligibleForAssignment(&donor))
        {
            printStatusMessage("ERROR", "Donor must be approved and available.");
            return;
        }

        if (!printMatchingRequestsForDonor(&donor))
        {
            return;
        }

        if (!readIntOrBack("Enter request ID (0 to back): ", &requestId))
        {
            return;
        }
    }
    else if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
    {
        return;
    }

    assignDonorToRequestByIds(requestId, donorId);
}

void assignDonorToRequest(void)
{
    Request request;
    int requestId;
    int donorId;

    printSectionHeader("Assign Donor");
    printf("Enter 0 at any prompt to go back. Nothing will be saved.\n");

    if (!printRequestSelectionList("Choose Request To Assign Donor", 0))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to back): ", &requestId))
    {
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "No request found with that ID.");
        pauseScreen();
        return;
    }

    printf("%-18s : %d\n", "Request ID", request.requestId);
    printf("%-18s : %s\n", "Patient Name", request.patientName);
    printf("%-18s : %s\n", "Blood Group Needed", request.bloodGroupNeeded);
    printf("%-18s : %d\n", "Required Bags", request.requiredBags);
    printf("%-18s : %d\n", "Assigned Bags", countRequestActiveAssigned(requestId));
    printf("%-18s : %s\n", "Request Status", getRequestStatusText(request.requestStatus));
    printMatchingDonorsForRequest(&request);

    if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
    {
        return;
    }

    assignDonorToRequestByIds(requestId, donorId);
    pauseScreen();
}

void viewAllAssignments(void)
{
    printAssignmentSelectionList("All Assignments", -1);
    pauseScreen();
}

void viewAssignmentsByRequest(void)
{
    FILE *file;
    DonorAssignment assignment;
    Donor donor;
    Request request;
    int requestId;
    int found = 0;

    printSectionHeader("Assignments by Request");

    if (!printRequestSelectionList("Choose Request To View Assignments", 1))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to go back): ", &requestId))
    {
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("INFO", "No request found with that ID.");
        pauseScreen();
        return;
    }

    displayRequest(&request);

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No assignment records found yet.");
        pauseScreen();
        return;
    }

    printf("%-8s %-8s %-18s %-6s %-14s %-5s %-5s %-5s %-9s %-7s\n",
           "Assign", "Donor", "Donor Name", "Group", "Status", "Bags", "Need", "Done", "Requester", "Admin");
    printLine('-', 92);

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.requestId == requestId)
        {
            if (findDonorById(assignment.donorId, &donor))
            {
                printf("%-8d %-8d %-18s %-6s %-14s %-5d %-5d %-5d %-9s %-7s\n",
                       assignment.assignmentId,
                       assignment.donorId,
                       donor.name,
                       donor.bloodGroup,
                       getAssignmentStatusShortText(assignment.status),
                       assignment.bags > 0 ? assignment.bags : 1,
                       request.requiredBags,
                       request.verifiedBags,
                       assignment.requesterConfirmed ? "Yes" : "No",
                       assignment.adminVerified ? "Yes" : "No");
            }
            else
            {
                printf("%-8d %-8d %-18s %-6s %-14s %-5d %-5d %-5d %-9s %-7s\n",
                       assignment.assignmentId,
                       assignment.donorId,
                       "Unknown",
                       "Unknown",
                       getAssignmentStatusShortText(assignment.status),
                       assignment.bags > 0 ? assignment.bags : 1,
                       request.requiredBags,
                       request.verifiedBags,
                       assignment.requesterConfirmed ? "Yes" : "No",
                       assignment.adminVerified ? "Yes" : "No");
            }

            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No assignments found for this request.");
    }

    pauseScreen();
}

void viewInterestedDonors(void)
{
    FILE *file;
    DonorAssignment assignment;
    Donor donor;
    Request request;
    int requestId;
    int found = 0;
    int actionChoice;
    int assignmentId;

    printSectionHeader("View Interested Donors");

    if (!printRequestsWithInterestedDonors("Choose Request With Interested Donors"))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to go back): ", &requestId))
    {
        return;
    }

    file = fopen(ASSIGNMENT_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donor interest records found yet.");
        pauseScreen();
        return;
    }

    if (findRequestById(requestId, &request))
    {
        printf("%-18s : %d\n", "Request ID", request.requestId);
        printf("%-18s : %s\n", "Patient Name", request.patientName);
        printf("%-18s : %s\n", "Blood Group Needed", request.bloodGroupNeeded);
        printf("%-18s : %d\n", "Required Bags", request.requiredBags);
        printf("%-18s : %d\n", "Assigned Bags", countRequestActiveAssigned(requestId));
        printf("%-18s : %s\n", "Location", request.location);
        printf("%-18s : %s\n", "Request Status", getRequestStatusText(request.requestStatus));
    }

    printLine('-', 104);
    printf("%-8s %-8s %-18s %-6s %-14s %-20s %-14s %-12s\n",
           "Assign",
           "Donor",
           "Name",
           "Group",
           "Phone",
           "Area",
           "Status",
           "Action");
    printLine('-', 104);

    while (fread(&assignment, sizeof(DonorAssignment), 1, file) == 1)
    {
        if (assignment.requestId == requestId &&
            (assignment.status == ASSIGNMENT_INTERESTED ||
             assignment.status == ASSIGNMENT_ASSIGNED ||
             assignment.status == ASSIGNMENT_WAITING_VERIFICATION))
        {
            if (findDonorById(assignment.donorId, &donor))
            {
                printf("%-8d %-8d %-18s %-6s %-14s %-20s %-14s %-12s\n",
                       assignment.assignmentId,
                       assignment.donorId,
                       donor.name,
                       donor.bloodGroup,
                       donor.phone,
                       donor.address,
                       getAssignmentStatusShortText(assignment.status),
                       assignment.status == ASSIGNMENT_INTERESTED ? "Action OK" : "View Only");
            }
            else
            {
                printf("%-8d %-8d %-18s %-6s %-14s %-20s %-14s %-12s\n",
                       assignment.assignmentId,
                       assignment.donorId,
                       "Unknown",
                       "-",
                       "-",
                       "-",
                       getAssignmentStatusShortText(assignment.status),
                       assignment.status == ASSIGNMENT_INTERESTED ? "Action OK" : "View Only");
            }
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No interested or active donors found for this request.");
        pauseScreen();
        return;
    }

    printLine('-', 104);
    printf("  1. Assign an interested donor\n");
    printf("  2. Reject / cancel an interested donor\n");
    printf("  0. Back\n");

    if (!readIntInRangeOrBack("Enter action: ", &actionChoice, 0, 2))
    {
        return;
    }

    if (actionChoice == 0)
    {
        return;
    }

    if (!readIntOrBack("Enter assignment ID: ", &assignmentId))
    {
        return;
    }

    if (actionChoice == 1)
    {
        if (updateInterestedAssignmentStatus(assignmentId, requestId, ASSIGNMENT_ASSIGNED))
        {
            printStatusMessage("SUCCESS", "Interested donor assigned successfully.");
            writeActivityLog("Admin assigned an interested donor.");
        }
        else
        {
            printStatusMessage("ERROR", "Only valid interested donor records can be assigned.");
        }
    }
    else
    {
        if (updateInterestedAssignmentStatus(assignmentId, requestId, ASSIGNMENT_CANCELLED))
        {
            printStatusMessage("SUCCESS", "Interested donor rejected/cancelled successfully.");
            writeActivityLog("Admin rejected an interested donor.");
        }
        else
        {
            printStatusMessage("ERROR", "Only valid interested donor records can be rejected.");
        }
    }

    pauseScreen();
}

int updateInterestedAssignmentStatus(int assignmentId, int requestId, int newStatus)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;
    Request request;
    Donor donor;
    int found = 0;
    int updated = 0;
    char dateTime[30];

    if (newStatus != ASSIGNMENT_ASSIGNED && newStatus != ASSIGNMENT_CANCELLED)
    {
        return 0;
    }

    if (!findRequestById(requestId, &request))
    {
        return 0;
    }

    if (request.requestStatus == REQUEST_CANCELLED || request.requestStatus == REQUEST_FULFILLED)
    {
        return 0;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");
    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        return 0;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == assignmentId &&
            assignment.requestId == requestId &&
            assignment.status == ASSIGNMENT_INTERESTED)
        {
            found = 1;

            if (newStatus == ASSIGNMENT_ASSIGNED)
            {
                if (countRequestActiveAssigned(requestId) >= request.requiredBags ||
                    !findDonorById(assignment.donorId, &donor) ||
                    !isDonorEligibleForAssignment(&donor) ||
                    !isCompatible(donor.bloodGroup, request.bloodGroupNeeded) ||
                    isDonorAssignedToActiveRequest(assignment.donorId, requestId))
                {
                    fclose(sourceFile);
                    fclose(tempFile);
                    remove(TEMP_ASSIGNMENT_FILE);
                    return 0;
                }

                assignment.status = ASSIGNMENT_ASSIGNED;
                assignment.bags = assignment.bags > 0 ? assignment.bags : 1;
                assignment.requesterConfirmed = 0;
                assignment.adminVerified = 0;
                strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
                assignment.assignedDate[sizeof(assignment.assignedDate) - 1] = '\0';
                updated = 1;
            }
            else
            {
                assignment.status = ASSIGNMENT_CANCELLED;
                updated = 1;
            }
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found || !updated)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        return 0;
    }

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        return 0;
    }

    updateRequestStatusAfterAssignment(requestId);
    return 1;
}

void assignMultipleDonorsToRequest(void)
{
    Request request;
    int requestId;
    int donorId;
    int requiredBags;

    printSectionHeader("Assign Multiple Donors");
    printf("Enter 0 to stop assigning and go back.\n");

    if (!printRequestSelectionList("Choose Request For Multiple Donors", 0))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to back): ", &requestId))
    {
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "No request found with that ID.");
        pauseScreen();
        return;
    }

    requiredBags = request.requiredBags;

    while (countRequestActiveAssigned(requestId) < requiredBags)
    {
        printf("\nAssigned bags: %d / %d\n", countRequestActiveAssigned(requestId), requiredBags);

        if (!printMatchingDonorsForRequest(&request))
        {
            break;
        }

        if (!readIntOrBack("Enter donor ID to assign (0 to back): ", &donorId))
        {
            break;
        }

        if (donorId == 0)
        {
            break;
        }

        if (!findDonorById(donorId, NULL))
        {
            printStatusMessage("ERROR", "No donor record exists with that ID.");
            continue;
        }

        {
            Donor donor;
            findDonorById(donorId, &donor);
            if (!isDonorEligibleForAssignment(&donor))
            {
                printStatusMessage("ERROR", "Donor must be approved and available.");
                continue;
            }
            if (!isCompatible(donor.bloodGroup, request.bloodGroupNeeded))
            {
                printStatusMessage("ERROR", "Donor blood group does not match this request.");
                continue;
            }
            if (isDonorAssignedToActiveRequest(donorId, requestId))
            {
                printStatusMessage("ERROR", "Donor is already assigned to another active request.");
                continue;
            }
        }

        if (createOrUpdateAssignment(requestId, donorId, ASSIGNMENT_ASSIGNED))
        {
            printStatusMessage("SUCCESS", "Donor assigned.");
        }
        else
        {
            printStatusMessage("ERROR", "Failed to assign donor.");
        }
    }

    updateRequestStatusAfterAssignment(requestId);
    printStatusMessage("INFO", "Multiple assignment flow finished.");
    writeActivityLog("Admin assigned multiple donors to a request.");
    pauseScreen();
}

void changeAssignedDonor(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;
    Request request;
    Donor newDonor;
    int oldAssignmentId;
    int newDonorId;
    int requestId = 0;
    int oldDonorId = 0;
    int found = 0;
    int replaceable = 0;
    int newDonorRecordFound = 0;
    int newDonorBlocked = 0;
    int newAssignmentWritten = 0;
    int nextAssignmentId;
    char dateTime[30];

    printSectionHeader("Change Assigned Donor");
    printf("Only admin can change assigned donors. Enter 0 at any prompt to go back.\n");

    if (!printAssignmentSelectionList("Choose Assigned Record To Replace", ASSIGNMENT_ASSIGNED))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter old assignment ID (0 to back): ", &oldAssignmentId))
    {
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == oldAssignmentId &&
            assignment.status == ASSIGNMENT_ASSIGNED)
        {
            found = 1;
            replaceable = 1;
            requestId = assignment.requestId;
            oldDonorId = assignment.donorId;
            break;
        }
        else if (assignment.assignmentId == oldAssignmentId)
        {
            found = 1;
        }
    }

    fclose(sourceFile);

    if (!found || !replaceable)
    {
        printStatusMessage("ERROR", "Only currently assigned donor records can be replaced.");
        pauseScreen();
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "Request record was not found.");
        pauseScreen();
        return;
    }

    if (request.requestStatus == REQUEST_CANCELLED ||
        request.requestStatus == REQUEST_FULFILLED)
    {
        printStatusMessage("ERROR", "Cancelled or fulfilled requests cannot have assigned donors changed.");
        pauseScreen();
        return;
    }

    printf("%-18s : %d\n", "Request ID", request.requestId);
    printf("%-18s : %s\n", "Patient Name", request.patientName);
    printf("%-18s : %s\n", "Blood Group Needed", request.bloodGroupNeeded);
    printf("%-18s : %d\n", "Required Bags", request.requiredBags);
    printf("%-18s : %s\n", "Request Status", getRequestStatusText(request.requestStatus));

    if (!printMatchingDonorsForRequest(&request))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter new donor ID (0 to back): ", &newDonorId))
    {
        return;
    }

    if (oldDonorId == newDonorId)
    {
        printStatusMessage("ERROR", "New donor must be different from the currently assigned donor.");
        pauseScreen();
        return;
    }

    if (!findDonorById(newDonorId, &newDonor))
    {
        printStatusMessage("ERROR", "New donor record was not found.");
        pauseScreen();
        return;
    }

    if (!isDonorEligibleForAssignment(&newDonor) ||
        !isCompatible(newDonor.bloodGroup, request.bloodGroupNeeded) ||
        isDonorAssignedToActiveRequest(newDonorId, requestId))
    {
        printStatusMessage("ERROR", "New donor must be approved, available, unassigned, and blood-group matched.");
        pauseScreen();
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");

    if (sourceFile == NULL)
    {
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.requestId == requestId &&
            assignment.donorId == newDonorId &&
            assignment.status != ASSIGNMENT_REPLACED &&
            assignment.status != ASSIGNMENT_CANCELLED)
        {
            if (assignment.status == ASSIGNMENT_INTERESTED)
            {
                newDonorRecordFound = 1;
            }
            else
            {
                newDonorBlocked = 1;
            }
        }
    }

    fclose(sourceFile);

    if (newDonorBlocked)
    {
        printStatusMessage("ERROR", "New donor already has an active assignment for this request.");
        pauseScreen();
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");
    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));
    nextAssignmentId = generateNextAssignmentId();

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == oldAssignmentId)
        {
            assignment.status = ASSIGNMENT_REPLACED;
        }
        else if (assignment.requestId == requestId &&
                 assignment.donorId == newDonorId &&
                 assignment.status == ASSIGNMENT_INTERESTED &&
                 !newAssignmentWritten)
        {
            assignment.status = ASSIGNMENT_ASSIGNED;
            assignment.bags = assignment.bags > 0 ? assignment.bags : 1;
            assignment.requesterConfirmed = 0;
            assignment.adminVerified = 0;
            strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
            assignment.assignedDate[sizeof(assignment.assignedDate) - 1] = '\0';
            newAssignmentWritten = 1;
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            printStatusMessage("ERROR", "Failed to save assigned donor change.");
            pauseScreen();
            return;
        }
    }

    if (newDonorRecordFound && !newAssignmentWritten)
    {
        fclose(sourceFile);
        fclose(tempFile);
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "New donor interest record changed before it could be assigned.");
        pauseScreen();
        return;
    }

    if (!newDonorRecordFound)
    {
        memset(&assignment, 0, sizeof(DonorAssignment));
        assignment.assignmentId = nextAssignmentId;
        assignment.requestId = requestId;
        assignment.donorId = newDonorId;
        assignment.status = ASSIGNMENT_ASSIGNED;
        assignment.bags = 1;
        assignment.requesterConfirmed = 0;
        assignment.adminVerified = 0;
        strncpy(assignment.interestedDate, dateTime, sizeof(assignment.interestedDate) - 1);
        assignment.interestedDate[sizeof(assignment.interestedDate) - 1] = '\0';
        strncpy(assignment.assignedDate, dateTime, sizeof(assignment.assignedDate) - 1);
        assignment.assignedDate[sizeof(assignment.assignedDate) - 1] = '\0';

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            printStatusMessage("ERROR", "Failed to save new donor assignment.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save assigned donor change.");
        pauseScreen();
        return;
    }

    updateRequestStatusAfterAssignment(requestId);
    printStatusMessage("SUCCESS", "Assigned donor changed. Old assignment kept as Replaced.");
    writeActivityLog("Admin changed an assigned donor.");
    pauseScreen();
}

void addBackupDonor(void)
{
    Request request;
    Donor donor;
    int requestId;
    int donorId;

    printSectionHeader("Add Backup Donor");
    printf("Backup donors stay Interested and do not count as assigned bags.\n");

    if (!printRequestSelectionList("Choose Request For Backup Donor", 0))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter request ID (0 to back): ", &requestId))
    {
        return;
    }

    if (!findRequestById(requestId, &request))
    {
        printStatusMessage("ERROR", "Request was not found.");
        pauseScreen();
        return;
    }

    if (!printMatchingDonorsForRequest(&request))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter donor ID (0 to back): ", &donorId))
    {
        return;
    }

    if (!findDonorById(donorId, &donor))
    {
        printStatusMessage("ERROR", "Donor was not found.");
        pauseScreen();
        return;
    }

    if (!isDonorEligibleForAssignment(&donor) ||
        !isCompatible(donor.bloodGroup, request.bloodGroupNeeded))
    {
        printStatusMessage("ERROR", "Backup donor must be approved, available, and blood-group matched.");
        pauseScreen();
        return;
    }

    if (donorInterestExists(requestId, donorId))
    {
        printStatusMessage("ERROR", "This donor already has an interest/assignment record for this request.");
        pauseScreen();
        return;
    }

    if (!createOrUpdateAssignment(requestId, donorId, ASSIGNMENT_INTERESTED))
    {
        printStatusMessage("ERROR", "Failed to add backup donor.");
        pauseScreen();
        return;
    }

    printStatusMessage("SUCCESS", "Backup donor added as Interested.");
    writeActivityLog("Admin added backup donor.");
    pauseScreen();
}

void verifyDonationByAssignment(void)
{
    FILE *sourceFile;
    FILE *tempFile;
    DonorAssignment assignment;
    DonorAssignment verifiedAssignment;
    Request request;
    Donor donor;
    int assignmentId;
    int found = 0;
    int adminMarked = 0;
    int finalized = 0;
    char dateTime[30];

    printSectionHeader("Admin Verify Donation");
    printf("Requester confirmation and admin verification are saved separately.\n");

    if (!printAssignmentSelectionList("Choose Donation Done Assignment To Verify", ASSIGNMENT_WAITING_VERIFICATION))
    {
        pauseScreen();
        return;
    }

    if (!readIntOrBack("Enter assignment ID (0 to back): ", &assignmentId))
    {
        return;
    }

    sourceFile = fopen(ASSIGNMENT_FILE, "rb");
    tempFile = fopen(TEMP_ASSIGNMENT_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        printStatusMessage("ERROR", "Unable to open assignment files.");
        pauseScreen();
        return;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    while (fread(&assignment, sizeof(DonorAssignment), 1, sourceFile) == 1)
    {
        if (assignment.assignmentId == assignmentId)
        {
            found = 1;

            if (assignment.status != ASSIGNMENT_WAITING_VERIFICATION)
            {
                /* Only donation-done assignments can receive admin verification. */
            }
            else if (assignment.adminVerified)
            {
                printStatusMessage("INFO", "Admin verification was already saved for this assignment.");
            }
            else if (findRequestById(assignment.requestId, &request) &&
                findDonorById(assignment.donorId, &donor))
            {
                assignment.adminVerified = 1;
                adminMarked = 1;

                if (assignment.requesterConfirmed)
                {
                    assignment.status = ASSIGNMENT_VERIFIED;
                    strncpy(assignment.verifiedDate, dateTime, sizeof(assignment.verifiedDate) - 1);
                    finalized = 1;
                }

                verifiedAssignment = assignment;
            }
        }

        if (fwrite(&assignment, sizeof(DonorAssignment), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_ASSIGNMENT_FILE);
            printStatusMessage("ERROR", "Failed to verify donation.");
            pauseScreen();
            return;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Assignment was not found.");
        pauseScreen();
        return;
    }

    if (!adminMarked)
    {
        remove(TEMP_ASSIGNMENT_FILE);
        printStatusMessage("ERROR", "Assignment must be Donation Done, Waiting Verification and not already admin-verified.");
        pauseScreen();
        return;
    }

    if (remove(ASSIGNMENT_FILE) != 0 || rename(TEMP_ASSIGNMENT_FILE, ASSIGNMENT_FILE) != 0)
    {
        printStatusMessage("ERROR", "Failed to save verification.");
        pauseScreen();
        return;
    }

    if (finalized)
    {
        if (!updateDonorAfterVerifiedDonation(donor.donorId) ||
            !saveDonationRecordForAssignment(&request, &donor, verifiedAssignment.bags, 1, 1))
        {
            printStatusMessage("ERROR", "Donation was verified, but donor history could not be fully updated.");
            writeActivityLog("Admin verification completed with donor history update error.");
            pauseScreen();
            return;
        }

        updateRequestStatusAfterVerification(request.requestId);
        printStatusMessage("SUCCESS", "Admin verification saved. Requester had already confirmed, so the donation is now finalized.");
        writeActivityLog("Admin verified and finalized a donation assignment.");
    }
    else
    {
        printStatusMessage("SUCCESS", "Admin verification saved. Waiting for requester confirmation before final completion.");
        writeActivityLog("Admin verified a donation assignment.");
    }

    pauseScreen();
}

int saveDonationRecordForAssignment(const Request *request, const Donor *donor, int bags, int requesterVerified, int adminVerified)
{
    FILE *file;
    DonationRecord record;
    char dateTime[30];

    file = fopen(DONATION_FILE, "ab");

    if (file == NULL)
    {
        return 0;
    }

    memset(&record, 0, sizeof(DonationRecord));
    record.donationId = generateNextDonationId();
    record.requestId = request->requestId;
    record.donorId = donor->donorId;
    strncpy(record.donorName, donor->name, sizeof(record.donorName) - 1);
    strncpy(record.requesterName, request->requesterName, sizeof(record.requesterName) - 1);
    strncpy(record.patientName, request->patientName, sizeof(record.patientName) - 1);
    strncpy(record.bloodGroup, request->bloodGroupNeeded, sizeof(record.bloodGroup) - 1);
    record.unitsDonated = bags > 0 ? bags : 1;
    getCurrentDateTime(dateTime, sizeof(dateTime));
    strncpy(record.donationDate, dateTime, sizeof(record.donationDate) - 1);
    record.requesterConfirmed = requesterVerified;
    record.adminVerified = adminVerified;

    if (fwrite(&record, sizeof(DonationRecord), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int updateDonorAfterVerifiedDonation(int donorId)
{
    FILE *sourceFile;
    FILE *tempFile;
    Donor donor;
    int found = 0;
    char dateTime[30];

    sourceFile = fopen(DONOR_FILE, "rb");
    tempFile = fopen(TEMP_DONOR_FILE, "wb");

    if (sourceFile == NULL || tempFile == NULL)
    {
        if (sourceFile != NULL)
        {
            fclose(sourceFile);
        }
        if (tempFile != NULL)
        {
            fclose(tempFile);
        }
        return 0;
    }

    getCurrentDateTime(dateTime, sizeof(dateTime));

    while (fread(&donor, sizeof(Donor), 1, sourceFile) == 1)
    {
        if (donor.donorId == donorId)
        {
            found = 1;
            donor.donationCount++;
            strcpy(donor.availabilityStatus, "0");
            strncpy(donor.lastDonationDate, dateTime, sizeof(donor.lastDonationDate) - 1);
        }

        if (fwrite(&donor, sizeof(Donor), 1, tempFile) != 1)
        {
            fclose(sourceFile);
            fclose(tempFile);
            remove(TEMP_DONOR_FILE);
            return 0;
        }
    }

    fclose(sourceFile);
    fclose(tempFile);

    if (!found)
    {
        remove(TEMP_DONOR_FILE);
        return 0;
    }

    if (remove(DONOR_FILE) != 0 || rename(TEMP_DONOR_FILE, DONOR_FILE) != 0)
    {
        return 0;
    }

    return 1;
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
    printf("%-8s %-8s %-8s %-18s %-18s %-8s %-12s %-10s %-8s\n",
           "Don ID",
           "Req ID",
           "Donor",
           "Donor Name",
           "Patient",
           "Bags",
           "Date",
           "Requester",
           "Admin");
    printLine('-', 104);

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        printf("%-8d %-8d %-8d %-18s %-18s %-8d %-12s %-10s %-8s\n",
               record.donationId,
               record.requestId,
               record.donorId,
               record.donorName,
               record.patientName,
               record.unitsDonated,
               record.donationDate,
               record.requesterConfirmed ? "Yes" : "No",
               record.adminVerified ? "Yes" : "No");
        found = 1;
    }

    if (!found)
    {
        printStatusMessage("INFO", "No donation records found.");
    }

    fclose(file);
    pauseScreen();
}

void searchDonationHistoryByDonorId(void)
{
    FILE *file;
    DonationRecord record;
    int donorId;
    int found = 0;

    printf("\nEnter donor ID to search donation history: ");
    while (scanf("%d", &donorId) != 1)
    {
        printf("Invalid input. Enter donor ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donation records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donation History by Donor ID");
    printf("%-24s : %d\n", "Donor ID", donorId);

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        if (record.donorId == donorId)
        {
            displayDonationRecord(&record);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donation history found for this donor.");
    }

    pauseScreen();
}

void searchDonationHistoryByRequestId(void)
{
    FILE *file;
    DonationRecord record;
    int requestId;
    int found = 0;

    printf("\nEnter request ID to search donation history: ");
    while (scanf("%d", &requestId) != 1)
    {
        printf("Invalid input. Enter request ID again: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donation records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Donation History by Request ID");
    printf("%-24s : %d\n", "Request ID", requestId);

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        if (record.requestId == requestId)
        {
            displayDonationRecord(&record);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donation history found for this request.");
    }

    pauseScreen();
}

void viewMyDonationHistory(int donorId)
{
    FILE *file;
    DonationRecord record;
    int found = 0;

    file = fopen(DONATION_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No donation history found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("My Donation History");
    printf("%-24s : %d\n", "Donor ID", donorId);

    while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
    {
        if (record.donorId == donorId)
        {
            displayDonationRecord(&record);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No donation history found for your account.");
    }

    pauseScreen();
}

void viewMyDonationCount(int donorId)
{
    FILE *file;
    DonationRecord record;
    int totalDonations = 0;

    file = fopen(DONATION_FILE, "rb");

    if (file != NULL)
    {
        while (fread(&record, sizeof(DonationRecord), 1, file) == 1)
        {
            if (record.donorId == donorId)
            {
                totalDonations++;
            }
        }

        fclose(file);
    }

    printSectionHeader("My Total Donation Count");
    printf("%-24s : %d\n", "Donor ID", donorId);
    printf("%-24s : %d\n", "Total Donations", totalDonations);
    printLine('=', 72);
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

        if (isDonorEligibleForAssignment(&donor))
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
    printf("%-22s : %d\n", "Approved Available", availableDonors);
    printf("%-22s : %d\n", "Not Assignable", unavailableDonors);
    printLine('=', 72);

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
        if (isDonorEligibleForAssignment(&donor))
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
    printf("Only approved and available donors are counted.\n");
    printf("%-15s : %d\n", "A+", aPositive);
    printf("%-15s : %d\n", "A-", aNegative);
    printf("%-15s : %d\n", "B+", bPositive);
    printf("%-15s : %d\n", "B-", bNegative);
    printf("%-15s : %d\n", "AB+", abPositive);
    printf("%-15s : %d\n", "AB-", abNegative);
    printf("%-15s : %d\n", "O+", oPositive);
    printf("%-15s : %d\n", "O-", oNegative);
    printLine('=', 72);

    writeActivityLog("Guest viewed public blood group availability.");
    pauseScreen();
}

void showRequestSummary(void)
{
    FILE *file;
    Request request;
    int totalRequests = 0;
    int pendingRequests = 0;
    int interestedRequests = 0;
    int partiallyMatchedRequests = 0;
    int fullyMatchedRequests = 0;
    int partiallyFulfilledRequests = 0;
    int fulfilledRequests = 0;
    int cancelledRequests = 0;
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
        else if (request.requestStatus == REQUEST_DONOR_INTERESTED)
        {
            interestedRequests++;
        }
        else if (request.requestStatus == REQUEST_PARTIALLY_MATCHED)
        {
            partiallyMatchedRequests++;
        }
        else if (request.requestStatus == REQUEST_FULLY_MATCHED)
        {
            fullyMatchedRequests++;
        }
        else if (request.requestStatus == REQUEST_PARTIALLY_FULFILLED)
        {
            partiallyFulfilledRequests++;
        }
        else if (request.requestStatus == REQUEST_FULFILLED)
        {
            fulfilledRequests++;
        }
        else if (request.requestStatus == REQUEST_CANCELLED)
        {
            cancelledRequests++;
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
    printf("%-22s : %d\n", "Donor Interested", interestedRequests);
    printf("%-22s : %d\n", "Partially Matched", partiallyMatchedRequests);
    printf("%-22s : %d\n", "Fully Matched", fullyMatchedRequests);
    printf("%-22s : %d\n", "Partially Fulfilled", partiallyFulfilledRequests);
    printf("%-22s : %d\n", "Fulfilled Requests", fulfilledRequests);
    printf("%-22s : %d\n", "Cancelled Requests", cancelledRequests);
    printf("%-22s : %d\n", "Other Status", otherRequests);
    printLine('=', 72);

    writeActivityLog("Viewed request summary report.");
    pauseScreen();
}

void displayRequestsByUrgencyCode(const char *urgencyCode, const char *title)
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

    printSectionHeader(title);

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (strcmp(request.urgencyLevel, urgencyCode) == 0)
        {
            displayRequest(&request);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No requests found for this urgency.");
    }

    pauseScreen();
}

void viewCriticalRequests(void)
{
    displayRequestsByUrgencyCode("4", "Critical Requests");
}

void viewHighPriorityRequests(void)
{
    displayRequestsByUrgencyCode("3", "High Priority Requests");
}

void searchRequestByUrgency(void)
{
    int urgency;
    char urgencyCode[3];

    printf("\nEnter urgency (1=Low, 2=Medium, 3=High, 4=Critical): ");
    while (scanf("%d", &urgency) != 1 || !isValidUrgencyChoice(urgency))
    {
        printf("Invalid urgency. Enter 1, 2, 3, or 4: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    snprintf(urgencyCode, sizeof(urgencyCode), "%d", urgency);
    displayRequestsByUrgencyCode(urgencyCode, "Requests By Urgency");
}

void searchRequestByStatus(void)
{
    FILE *file;
    Request request;
    int status;
    int found = 0;

    printSectionHeader("Request Status Options");
    printf("  0. Pending              : No donor interest or assignment yet.\n");
    printf("  1. Donor Interested     : Donor showed interest, admin has not assigned enough donors yet.\n");
    printf("  2. Partially Matched    : Some donors assigned, but assigned bags are less than required bags.\n");
    printf("  3. Fully Matched        : Assigned bags reached required bags, but donation is not fully verified yet.\n");
    printf("  4. Partially Fulfilled  : Some bags are admin verified, but verified bags are less than required bags.\n");
    printf("  5. Fulfilled            : Verified bags reached required bags.\n");
    printf("  6. Cancelled            : Request was cancelled.\n");
    printf("  9. Back\n");
    printLine('-', 100);

    printf("Enter status to search: ");
    while (scanf("%d", &status) != 1 ||
           !((status >= REQUEST_PENDING && status <= REQUEST_CANCELLED) || status == 9))
    {
        printf("Invalid status. Enter 0 to 6, or 9 to back: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    if (status == 9)
    {
        printStatusMessage("INFO", "Request status search cancelled.");
        pauseScreen();
        return;
    }

    file = fopen(REQUEST_FILE, "rb");

    if (file == NULL)
    {
        printStatusMessage("INFO", "No request records found yet.");
        pauseScreen();
        return;
    }

    printSectionHeader("Requests By Status");
    printf("%-24s : %s\n", "Status Filter", getRequestStatusText(status));

    while (fread(&request, sizeof(Request), 1, file) == 1)
    {
        if (request.requestStatus == status)
        {
            displayRequest(&request);
            found = 1;
        }
    }

    fclose(file);

    if (!found)
    {
        printStatusMessage("INFO", "No requests found for this status.");
    }

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

        if (isDonorEligibleForAssignment(&donor))
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
        fprintf(reportFile, "Account Status     : %s\n", getDonorApprovalStatusText(donor.approvalStatus));
        fprintf(reportFile, "Donation Count     : %d\n", donor.donationCount);
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Donors       : %d\n", totalDonors);
    fprintf(reportFile, "Approved Available : %d\n", availableDonors);
    fprintf(reportFile, "Not Assignable     : %d\n", unavailableDonors);

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
    int interestedRequests = 0;
    int partiallyMatchedRequests = 0;
    int fullyMatchedRequests = 0;
    int partiallyFulfilledRequests = 0;
    int fulfilledRequests = 0;
    int cancelledRequests = 0;
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
        else if (request.requestStatus == REQUEST_DONOR_INTERESTED)
        {
            interestedRequests++;
        }
        else if (request.requestStatus == REQUEST_PARTIALLY_MATCHED)
        {
            partiallyMatchedRequests++;
        }
        else if (request.requestStatus == REQUEST_FULLY_MATCHED)
        {
            fullyMatchedRequests++;
        }
        else if (request.requestStatus == REQUEST_PARTIALLY_FULFILLED)
        {
            partiallyFulfilledRequests++;
        }
        else if (request.requestStatus == REQUEST_FULFILLED)
        {
            fulfilledRequests++;
        }
        else if (request.requestStatus == REQUEST_CANCELLED)
        {
            cancelledRequests++;
        }
        else
        {
            otherRequests++;
        }

        fprintf(reportFile, "Request ID         : %d\n", request.requestId);
        fprintf(reportFile, "Patient Name       : %s\n", request.patientName);
        fprintf(reportFile, "Blood Group Needed : %s\n", request.bloodGroupNeeded);
        fprintf(reportFile, "Required Bags      : %d\n", request.requiredBags);
        fprintf(reportFile, "Verified Bags      : %d\n", request.verifiedBags);
        fprintf(reportFile, "Hospital Name      : %s\n", request.hospitalName);
        fprintf(reportFile, "Location           : %s\n", request.location);
        fprintf(reportFile, "Contact Number     : %s\n", request.contactNumber);
        fprintf(reportFile, "Urgency Level      : %s\n", getUrgencyText(request.urgencyLevel));
        fprintf(reportFile, "Tracking PIN       : %s\n", request.trackingPIN);
        fprintf(reportFile, "Request Status     : %s\n", getRequestStatusText(request.requestStatus));
        fprintf(reportFile, "------------------------------------------------------------\n");
    }

    fprintf(reportFile, "\nSummary\n");
    fprintf(reportFile, "Total Requests     : %d\n", totalRequests);
    fprintf(reportFile, "Pending Requests   : %d\n", pendingRequests);
    fprintf(reportFile, "Donor Interested   : %d\n", interestedRequests);
    fprintf(reportFile, "Partially Matched  : %d\n", partiallyMatchedRequests);
    fprintf(reportFile, "Fully Matched      : %d\n", fullyMatchedRequests);
    fprintf(reportFile, "Partially Fulfilled: %d\n", partiallyFulfilledRequests);
    fprintf(reportFile, "Fulfilled Requests : %d\n", fulfilledRequests);
    fprintf(reportFile, "Cancelled Requests : %d\n", cancelledRequests);
    fprintf(reportFile, "Other Status       : %d\n", otherRequests);

    fclose(dataFile);
    fclose(reportFile);

    printf("\n[SUCCESS] Request report exported to %s\n", REQUEST_REPORT_FILE);
    writeActivityLog("Exported request report to TXT.");
    pauseScreen();
}

void exportDonationReport(void)
{
    FILE *dataFile;
    FILE *reportFile;
    FILE *donorFile;
    FILE *requestFile;
    DonationRecord record;
    Donor donor;
    Request request;
    int totalRecords = 0;
    int requesterConfirmed = 0;
    int adminVerified = 0;
    int donorFound;
    int requestFound;
    char dateTime[30];
    char donorName[50];
    char requesterName[50];
    char patientName[50];
    char finalStatus[50];

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
        donorFound = 0;
        requestFound = 0;
        strcpy(donorName, strlen(record.donorName) > 0 ? record.donorName : "Not Found");
        strcpy(requesterName, strlen(record.requesterName) > 0 ? record.requesterName : "Not Found");
        strcpy(patientName, strlen(record.patientName) > 0 ? record.patientName : "Not Found");
        strcpy(finalStatus, "Unknown");

        if (record.requesterConfirmed)
        {
            requesterConfirmed++;
        }

        if (record.adminVerified)
        {
            adminVerified++;
        }

        donorFile = fopen(DONOR_FILE, "rb");

        if (donorFile != NULL)
        {
            while (fread(&donor, sizeof(Donor), 1, donorFile) == 1)
            {
                if (donor.donorId == record.donorId)
                {
                    strcpy(donorName, donor.name);
                    donorFound = 1;
                    break;
                }
            }

            fclose(donorFile);
        }

        requestFile = fopen(REQUEST_FILE, "rb");

        if (requestFile != NULL)
        {
            while (fread(&request, sizeof(Request), 1, requestFile) == 1)
            {
                if (request.requestId == record.requestId)
                {
                    strcpy(requesterName, request.requesterName);
                    strcpy(patientName, request.patientName);
                    strcpy(finalStatus, getRequestStatusText(request.requestStatus));
                    requestFound = 1;
                    break;
                }
            }

            fclose(requestFile);
        }

        if (donorFound == 0)
        {
            strcpy(donorName, "Unknown Donor");
        }

        if (requestFound == 0)
        {
            strcpy(requesterName, "Unknown Requester");
            strcpy(patientName, "Unknown Patient");
        }

        fprintf(reportFile, "Donation ID            : %d\n", record.donationId);
        fprintf(reportFile, "Request ID             : %d\n", record.requestId);
        fprintf(reportFile, "Donor ID               : %d\n", record.donorId);
        fprintf(reportFile, "Donor Name             : %s\n", donorName);
        fprintf(reportFile, "Requester Name         : %s\n", requesterName);
        fprintf(reportFile, "Patient Name           : %s\n", patientName);
        fprintf(reportFile, "Blood Group            : %s\n", record.bloodGroup);
        fprintf(reportFile, "Donation Date          : %s\n", record.donationDate);
        fprintf(reportFile, "Requester Confirmation : %s\n", record.requesterConfirmed ? "Yes" : "No");
        fprintf(reportFile, "Admin Verification     : %s\n", record.adminVerified ? "Yes" : "No");
        fprintf(reportFile, "Final Status           : %s\n", finalStatus);
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

    printLine('=', 72);
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

    if (currentActivityActorId > 0)
    {
        fprintf(logFile, "[%s] [%s ID %d] %s\n",
                dateTime,
                currentActivityActor,
                currentActivityActorId,
                action);
    }
    else
    {
        fprintf(logFile, "[%s] [%s] %s\n",
                dateTime,
                currentActivityActor,
                action);
    }

    fclose(logFile);
}

void setActivityActor(const char *actorType, int actorId)
{
    if (actorType == NULL || strlen(actorType) == 0)
    {
        strcpy(currentActivityActor, "SYSTEM");
        currentActivityActorId = 0;
        return;
    }

    strncpy(currentActivityActor, actorType, sizeof(currentActivityActor) - 1);
    currentActivityActor[sizeof(currentActivityActor) - 1] = '\0';
    currentActivityActorId = actorId;
}

void clearActivityActor(void)
{
    strcpy(currentActivityActor, "SYSTEM");
    currentActivityActorId = 0;
}
