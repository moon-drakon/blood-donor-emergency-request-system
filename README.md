# NSU Blood Donor and Emergency Request Management System

A console-based C project for managing blood donors, emergency blood requests,
donor matching, assignments, donation verification, reports, and activity logs.

This project was developed as an academic C programming project for CSE 115 Lab
at North South University.

## Project Overview

The system simulates a complete emergency blood donation workflow with three
main user roles:

- **Administrator**: manages donors, requests, assignments, reports, and final
  donation verification.
- **Donor**: registers, waits for approval, views matching requests, shows
  interest, and marks assigned donations as done.
- **Requester / Guest**: creates emergency blood requests, tracks requests with
  a PIN, views assigned donor details, confirms completed donations, and uses
  public donor availability features.

Records are stored using binary files, while reports and activity logs are
exported as readable text files.

## Features

- Admin login with default admin account creation
- Donor self-registration with admin approval
- Donor login, profile update, password change, and password recovery
- Emergency blood request creation with Request ID and Tracking PIN
- Public guest search for donor availability by blood group
- Blood-group compatibility matching
- Donor interest workflow for matching requests
- Admin assignment, multiple donor assignment, backup donor, and donor
  replacement workflow
- Donor completion marking after assigned donation
- Two-step finalization with requester confirmation and admin verification
- Donation history and donor donation count tracking
- Request status recalculation from assignments and verified bags
- Donor, request, and donation reports exported to text files
- Activity log with actor information
- Demo data auto-generation for easier testing and presentation

## Tech Stack

- C
- C structs
- Binary file handling with `fread` and `fwrite`
- Text report generation with `fprintf`
- Menu-driven console interface
- Modular function-based design

## How to Compile and Run

### Windows PowerShell

```powershell
gcc main.c -o blood_system.exe
.\blood_system.exe
```

### Windows Command Prompt

```bat
gcc main.c -o blood_system.exe
blood_system.exe
```

### Linux / macOS

```bash
gcc main.c -o blood_system
./blood_system
```

## Default Login

The program creates a default admin account automatically if `admins.dat` does
not exist.

```text
Admin ID : 1001
Password : admin123
```

## Demo Data

If `donors.dat` and `requests.dat` are missing or empty, the program creates
demo records automatically. The current final setup tops up the dataset to:

- 139 approved donors
- 73 emergency requests

Example demo credentials:

```text
Donor ID 1  / password donor1
Donor ID 2  / password donor2
Donor ID 10 / password donor10

Request ID 1 / PIN 1001
Request ID 2 / PIN 1002
```

For a full presentation walkthrough, see
[PROJECT_DEMO_GUIDE.txt](PROJECT_DEMO_GUIDE.txt).

## Main Workflow

1. Donor registers from the access menu.
2. Admin approves the donor account.
3. Requester creates an emergency blood request and saves the Request ID and
   Tracking PIN.
4. Donor views compatible requests and shows interest.
5. Admin assigns one or more compatible donors.
6. Donor marks the assigned donation as done.
7. Requester confirms the completed donation using Request ID and Tracking PIN.
8. Admin verifies the donation separately.
9. After both confirmations, the donation is finalized, saved to history, and
   the request progress is updated.

## Request Status Values

| Value | Meaning |
| --- | --- |
| 0 | Pending |
| 1 | Donor Interested |
| 2 | Partially Matched |
| 3 | Fully Matched |
| 4 | Partially Fulfilled |
| 5 | Fulfilled / Verified |
| 6 | Cancelled |

## Assignment Status Values

| Value | Meaning |
| --- | --- |
| 0 | Interested |
| 1 | Assigned |
| 2 | Donation Done, Waiting Verification |
| 3 | Verified / Completed |
| 4 | Replaced |
| 5 | Cancelled |

## Donor Approval Status Values

| Value | Meaning |
| --- | --- |
| 0 | Pending Approval |
| 1 | Approved |
| 2 | Rejected |
| 3 | Blocked |

## Files Used by the Program

### Source and documentation

- `main.c`
- `README.md`
- `PROJECT_DEMO_GUIDE.txt`
- `DEMO_LOGIN_INFO.txt`

### Runtime data files

These are generated locally and ignored by Git:

- `admins.dat`
- `donors.dat`
- `requests.dat`
- `assignments.dat`
- `donations.dat`
- `temp_*.dat`

### Generated text files

These are generated locally and ignored by Git:

- `donor_report.txt`
- `request_report.txt`
- `donation_report.txt`
- `activity_log.txt`

## Validation Rules

- Donor age must be between 18 and 65.
- Blood group must be one of `A+`, `A-`, `B+`, `B-`, `AB+`, `AB-`, `O+`, `O-`.
- Phone number and email must be valid.
- Donor phone and email must be unique.
- Last donation date must be `YYYY-MM-DD` or `Never`.
- Availability must be `1` for available or `0` for unavailable.
- Password must be at least 6 characters and include a letter and a number.
- Required blood bags must be between 1 and 20.
- Urgency must be between 1 and 4.

## Notes

- This is an academic learning project, not a production medical system.
- Passwords and tracking PINs are stored in plain text for simplicity.
- Binary `.dat` files depend on the current C struct layout.
- If struct definitions change, old `.dat` files may need to be deleted and
  regenerated.
- Completed donation history is preserved when donors or requests are deleted;
  active non-verified assignments are cancelled.

## Suggested LinkedIn Description

```text
Built a console-based Blood Donor and Emergency Request Management System in C.
The system manages donor registration, admin approval, emergency blood requests,
donor matching, assignment tracking, requester confirmation, admin verification,
donation history, report generation, and activity logging using file-based data
storage.
```
