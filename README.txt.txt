NSU Blood Donor and Emergency Request Management System
=======================================================

This file is kept as a plain-text companion for local lab submission.
For the GitHub version, open README.md.


Project Type
------------
Console-based C project for CSE 115 Lab, North South University.


Project Overview
----------------
This project manages blood donors, emergency blood requests, donor matching,
assignment, requester confirmation, admin verification, donation history,
reports, and activity logging.

The system is menu-driven and stores records in binary files. Reports and logs
are exported as readable text files.


Main Files
----------
- main.c
- README.md
- PROJECT_DEMO_GUIDE.txt
- DEMO_LOGIN_INFO.txt


Compile and Run
---------------
PowerShell:

gcc main.c -o blood_system.exe
.\blood_system.exe

Command Prompt:

gcc main.c -o blood_system.exe
blood_system.exe


Default Admin Login
-------------------
Admin ID : 1001
Password : admin123


Main Workflow
-------------
1. Donor registers.
2. Admin approves the donor.
3. Requester creates a blood request and saves the Request ID + Tracking PIN.
4. Donor views matching requests and shows interest.
5. Admin assigns compatible donors.
6. Donor marks assigned donation as done.
7. Requester confirms the completed donation.
8. Admin verifies the donation.
9. The donation is saved to history and request progress is updated.


Status References
-----------------
Request statuses:
0 = Pending
1 = Donor Interested
2 = Partially Matched
3 = Fully Matched
4 = Partially Fulfilled
5 = Fulfilled / Verified
6 = Cancelled

Assignment statuses:
0 = Interested
1 = Assigned
2 = Donation Done, Waiting Verification
3 = Verified / Completed
4 = Replaced
5 = Cancelled

Donor approval statuses:
0 = Pending Approval
1 = Approved
2 = Rejected
3 = Blocked


Generated Local Files
---------------------
Runtime data:
- admins.dat
- donors.dat
- requests.dat
- assignments.dat
- donations.dat

Reports and logs:
- donor_report.txt
- request_report.txt
- donation_report.txt
- activity_log.txt


Notes
-----
- This is an academic learning project, not a production medical system.
- Passwords and PINs are stored as plain text for simplicity.
- Binary .dat files depend on the current struct layout.
- See PROJECT_DEMO_GUIDE.txt for a step-by-step presentation script.


Copyright and Usage
-------------------
Copyright (c) 2026 moon-drakon. All rights reserved.

This repository is shared for portfolio review, academic demonstration, and
learning reference only. Do not copy, redistribute, resubmit, sell, modify, or
present this project as your own work without written permission.


End of Document
