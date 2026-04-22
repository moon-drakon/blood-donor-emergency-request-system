NSU Blood Donor and Emergency Request Management System
=======================================================

Project Type:
Console-Based C Project

Course:
CSE 115 Lab
North South University

Project Overview
----------------
This project is a console-based Blood Donor and Emergency Request Management
System developed in the C programming language. The system is designed to help
manage donor records, emergency blood requests, donor-request matching,
report generation, and activity logging in a clear and organized way.

The project uses:
- struct
- functions
- binary file handling
- text file report export
- menu-driven console navigation


Main Features
-------------
1. Donor Management
   - Add donor
   - View all donors
   - Search donor by ID
   - Update donor
   - Delete donor
   - Change donor availability

2. Emergency Request Management
   - Add request
   - View all requests
   - Search request by ID
   - Update request status
   - Delete request

3. Matching System
   - Match donors by blood group
   - Match donors by request ID
   - Only shows donors with matching blood group
   - Only shows donors whose availability status is 1

4. Reports and Logging
   - Show donor summary
   - Show request summary
   - Export donor report to TXT
   - Export request report to TXT
   - View activity log


Files Used in the Project
-------------------------
Source File:
- main.c

Binary Data Files:
- donors.dat
- requests.dat

Readable Text Files:
- donor_report.txt
- request_report.txt
- activity_log.txt

Temporary Files:
- temp_donors.dat
- temp_requests.dat


Concepts Used
-------------
- C structures for storing donor and request data
- Modular functions for menu operations and features
- Binary file handling using fread() and fwrite()
- Temporary file technique for update and delete operations
- Text file generation using fprintf()
- Activity logging with date and time


Data Structures
---------------
Donor record stores:
- donor ID
- name
- age
- gender
- blood group
- phone number
- address
- last donation date
- availability status

Request record stores:
- request ID
- patient name
- blood group needed
- units needed
- hospital name
- location
- contact number
- urgency level
- request status


How to Compile and Run
----------------------
Using GCC:

gcc main.c -o blood_donor_system.exe
./blood_donor_system.exe

If you are using Windows Command Prompt:

gcc main.c -o blood_donor_system.exe
blood_donor_system.exe


How to Run in Code::Blocks
--------------------------
In your lab, you can run this project in Code::Blocks using the following steps:

1. Open Code::Blocks
2. Click File -> New -> Project
3. Choose Console Application
4. Select C as the language
5. Give the project a name such as:
   NSU_Blood_Donor_Project
6. Finish creating the project
7. In the project panel, open the generated source file
8. Delete the default sample code
9. Copy the code from main.c and paste it there
10. Save the file
11. Build and run the project using:
    F9

Alternative method:
1. Open Code::Blocks
2. Click File -> Open
3. Open the main.c file directly
4. Build and run using F9

Important Notes for Code::Blocks:
- Keep main.c in the same folder as the data files
- donors.dat and requests.dat will be created automatically after data is added
- donor_report.txt, request_report.txt, and activity_log.txt will also be created in the project folder
- If the console closes too quickly, run the program again from Code::Blocks and check the output carefully
- Make sure your compiler is set correctly in Code::Blocks before running

Recommended Setup in Code::Blocks:
- Compiler: GNU GCC Compiler
- Project Type: Console Application
- Language: C


Menu Structure
--------------
Main Menu:
- Donor Management
- Emergency Request Management
- Reports and Activity Log

Donor Menu:
- Add Donor
- View All Donors
- Search Donor by ID
- Update Donor
- Delete Donor
- Change Donor Availability

Request Menu:
- Add Emergency Request
- View All Requests
- Search Request by ID
- Update Request Status
- Delete Request
- Match Donors by Blood Group
- Match Donors by Request ID

Report Menu:
- Show Donor Summary
- Show Request Summary
- Export Donor Report to TXT
- Export Request Report to TXT
- View Activity Log


Project Strengths
-----------------
- Beginner-friendly code structure
- Professional menu-driven console interface
- Real binary file storage
- Clean donor matching feature
- Readable report export
- Practical activity logging


Notes
-----
- Donor availability uses:
  1 = Available
  0 = Unavailable

- The project is designed for learning purposes and academic lab submission.
- The current system focuses on clarity, file handling, and console usability.


End of Document
