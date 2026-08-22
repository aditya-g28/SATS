#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define CLASS_SIZE 20

// ============================================================
// STUDENT STRUCTURE
// Stores the permanent information about each student.
// ============================================================

struct Student
{
    char name[NAME_SIZE];
    int id;
    char class[CLASS_SIZE];

    // These will be used later for attendance calculations.
    int total_days;
    int present_days;
    int absent_days;
};


// ============================================================
// ATTENDANCE STRUCTURE
// Stores one attendance record for one student on one day.
// ============================================================

struct Attendance
{
    int student_id;

    int day;
    int month;
    int year;

    int status;          // 1 = Present, 0 = Absent

    // Unique key generated for this attendance record.
    int attendance_id;
};


int main()
{
    // --------------------------------------------------------
    // ARRAY TO STORE ALL STUDENTS READ FROM students.txt
    // --------------------------------------------------------

    struct Student students[MAX_STUDENTS];

    // One Attendance variable is enough because we save
    // each record to the file immediately.
    struct Attendance attendance;

    int student_count = 0;

    FILE *fp;


    // ========================================================
    // INITIALIZE RANDOM NUMBER GENERATOR
    // ========================================================

    // This makes rand() generate different numbers on
    // different program runs.
    srand(time(NULL));


    // ========================================================
    // READ STUDENTS FROM students.txt
    // ========================================================

    fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        printf("Error: students.txt not found.\n");
        return 1;
    }

    /*
        Each line in students.txt should look like:

        Aditya 1001 CSE-A
        Rahul  1002 CSE-A
        Jay    1003 CSE-B

        fscanf() reads:
        name → ID → class

        The loop continues until there are no more students.
    */

    while (student_count < MAX_STUDENTS &&
           fscanf(fp, "%49s %d %19s",
                  students[student_count].name,
                  &students[student_count].id,
                  students[student_count].class) == 3)
    {
        // Initialize attendance counters.
        students[student_count].total_days = 0;
        students[student_count].present_days = 0;
        students[student_count].absent_days = 0;

        student_count++;
    }

    fclose(fp);


    // ========================================================
    // CHECK WHETHER STUDENTS WERE LOADED
    // ========================================================

    if (student_count == 0)
    {
        printf("No students found in students.txt.\n");
        return 1;
    }

    printf("\n====================================\n");
    printf("        SATS ATTENDANCE SYSTEM\n");
    printf("====================================\n");

    printf("Students Loaded: %d\n", student_count);


    // ========================================================
    // ATTENDANCE LOOP
    // ========================================================

    int continue_attendance = 1;

    while (continue_attendance == 1)
    {
        int search_id;
        int found = 0;


        // ----------------------------------------------------
        // ASK FOR STUDENT ID
        // ----------------------------------------------------

        printf("\n------------------------------------\n");
        printf("Enter Student ID: ");
        scanf("%d", &search_id);


        // ----------------------------------------------------
        // SEARCH FOR THE STUDENT
        // ----------------------------------------------------

        for (int i = 0; i < student_count; i++)
        {
            if (students[i].id == search_id)
            {
                printf("\nStudent Found!\n");

                printf("Name  : %s\n", students[i].name);
                printf("ID    : %d\n", students[i].id);
                printf("Class : %s\n", students[i].class);

                found = 1;


                // ============================================
                // ASK FOR ATTENDANCE STATUS
                // ============================================

                printf("\nAttendance:\n");
                printf("1. Present\n");
                printf("0. Absent\n");
                printf("Enter choice: ");

                scanf("%d", &attendance.status);


                // ------------------------------------------------
                // VALIDATE ATTENDANCE INPUT
                // ------------------------------------------------

                while (attendance.status != 0 &&
                       attendance.status != 1)
                {
                    printf("Invalid choice!\n");
                    printf("Enter 1 for Present or 0 for Absent: ");
                    scanf("%d", &attendance.status);
                }


                // ============================================
                // STORE STUDENT ID IN ATTENDANCE RECORD
                // ============================================

                attendance.student_id = students[i].id;


                // ============================================
                // GET CURRENT DATE
                // ============================================

                time_t current_time;
                struct tm *current_date;

                time(&current_time);

                current_date = localtime(&current_time);

                attendance.day = current_date->tm_mday;
                attendance.month = current_date->tm_mon + 1;
                attendance.year = current_date->tm_year + 1900;


                // ============================================
                // GENERATE RANDOM ATTENDANCE KEY
                // ============================================

                /*
                    Generate a 6-digit number.

                    Example:

                    583921

                    This key identifies THIS particular
                    attendance record.
                */

                attendance.attendance_id =
                    rand() % 900000 + 100000;


                // ============================================
                // SAVE ATTENDANCE TO attendance.txt
                // ============================================

                fp = fopen("attendance.txt", "a");

                if (fp == NULL)
                {
                    printf("Error opening attendance.txt\n");
                    return 1;
                }

                /*
                    File format:

                    StudentID Day Month Year Status AttendanceID

                    Example:

                    1001 22 8 2026 1 583921

                    1 = Present
                    0 = Absent
                */

                fprintf(fp, "%d %d %d %d %d %d\n",
                        attendance.student_id,
                        attendance.day,
                        attendance.month,
                        attendance.year,
                        attendance.status,
                        attendance.attendance_id);

                fclose(fp);


                // ============================================
                // DISPLAY SUCCESS MESSAGE
                // ============================================

                printf("\n====================================\n");
                printf("Attendance Recorded Successfully!\n");
                printf("====================================\n");

                printf("Student : %s\n", students[i].name);
                printf("ID      : %d\n", students[i].id);

                if (attendance.status == 1)
                {
                    printf("Status  : PRESENT\n");
                }
                else
                {
                    printf("Status  : ABSENT\n");
                }

                printf("Date    : %02d/%02d/%d\n",
                       attendance.day,
                       attendance.month,
                       attendance.year);

                printf("Key     : %d\n",
                       attendance.attendance_id);

                break;
            }
        }


        // ----------------------------------------------------
        // STUDENT NOT FOUND
        // ----------------------------------------------------

        if (found == 0)
        {
            printf("\nStudent ID %d was not found.\n",
                   search_id);
        }


        // ====================================================
        // ASK WHETHER TO RECORD ANOTHER STUDENT
        // ====================================================

        printf("\n------------------------------------\n");
        printf("Record another student?\n");
        printf("1. Yes\n");
        printf("0. No\n");
        printf("Choice: ");

        scanf("%d", &continue_attendance);


        while (continue_attendance != 0 &&
               continue_attendance != 1)
        {
            printf("Invalid choice!\n");
            printf("Enter 1 for Yes or 0 for No: ");

            scanf("%d", &continue_attendance);
        }
    }


    // ========================================================
    // PROGRAM END
    // ========================================================

    printf("\n====================================\n");
    printf("Attendance session completed.\n");
    printf("====================================\n");

    return 0;
}
