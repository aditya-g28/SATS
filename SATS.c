#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STUDENTS 100
#define NAME_SIZE 50
#define CLASS_SIZE 20

struct Student
{
    char name[NAME_SIZE];
    int id;
    char class[CLASS_SIZE];

    int total_days;
    int present_days;
    int absent_days;
};

struct Attendance
{
    int student_id;

    int day;
    int month;
    int year;

    int status;          // 1 = Present, 0 = Absent

    int attendance_id;
};

int main()
{
    struct Student students[MAX_STUDENTS];
    struct Attendance attendance;

    int student_count = 0;
    int search_id;
    int found = 0;

    FILE *fp;

    // --------------------------------
    // READ STUDENTS FROM students.txt
    // --------------------------------

    fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        printf("Error: students.txt not found.\n");
        return 1;
    }

    while (student_count < MAX_STUDENTS &&
           fscanf(fp, "%49s %d %19s",
                  students[student_count].name,
                  &students[student_count].id,
                  students[student_count].class) == 3)
    {
        students[student_count].total_days = 0;
        students[student_count].present_days = 0;
        students[student_count].absent_days = 0;

        student_count++;
    }

    fclose(fp);

    // --------------------------------
    // SEARCH STUDENT
    // --------------------------------

    printf("\n========== SATS ==========\n");
    printf("Total Students: %d\n", student_count);

    printf("\nEnter Student ID: ");
    scanf("%d", &search_id);

    for (int i = 0; i < student_count; i++)
    {
        if (students[i].id == search_id)
        {
            printf("\nStudent Found!\n");
            printf("Name  : %s\n", students[i].name);
            printf("ID    : %d\n", students[i].id);
            printf("Class : %s\n", students[i].class);

            found = 1;

            // -----------------------------
            // ATTENDANCE STATUS
            // -----------------------------

            printf("\nEnter Attendance:\n");
            printf("1. Present\n");
            printf("0. Absent\n");
            printf("Choice: ");
            scanf("%d", &attendance.status);

            attendance.student_id = students[i].id;

            // -----------------------------
            // GET DATE
            // -----------------------------

            time_t t;
            struct tm *date;

            time(&t);
            date = localtime(&t);

            attendance.day = date->tm_mday;
            attendance.month = date->tm_mon + 1;
            attendance.year = date->tm_year + 1900;

            // -----------------------------
            // GENERATE ATTENDANCE ID
            // -----------------------------

            srand(time(NULL));

            attendance.attendance_id = rand() % 900000 + 100000;

            // -----------------------------
            // SAVE ATTENDANCE
            // -----------------------------

            fp = fopen("attendance.txt", "a");

            if (fp == NULL)
            {
                printf("Error opening attendance.txt\n");
                return 1;
            }

            fprintf(fp, "%d %d %d %d %d %d\n",
                    attendance.student_id,
                    attendance.day,
                    attendance.month,
                    attendance.year,
                    attendance.status,
                    attendance.attendance_id);

            fclose(fp);

            printf("\nAttendance saved successfully!\n");
            printf("Attendance ID: %d\n", attendance.attendance_id);

            break;
        }
    }

    if (found == 0)
    {
        printf("\nStudent ID not found.\n");
    }

    return 0;
}