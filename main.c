#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_COURSES 5
#define MAX_NAME_LEN 50
#define MAX_DEPT_LEN 30
#define MAX_EMAIL_LEN 50
#define MAX_ID_LEN 15
#define MAX_COURSE_LEN 10

struct Course {
    char code[MAX_COURSE_LEN];
    float quiz;
    float midterm;
    float final;
    float total;
    char grade;
};

struct Student {
    char name[MAX_NAME_LEN];
    char id[MAX_ID_LEN];
    char dept[MAX_DEPT_LEN];
    char email[MAX_EMAIL_LEN];
    struct Course courses[MAX_COURSES];
    int course_count;
};

// Function prototypes
void addStudent(struct Student students[], int *count);
void registerCourses(struct Student students[], int count);
void inputMarks(struct Student students[], int count);
void calculateGrade(struct Student *student);
void displayStudent(struct Student student);
void searchStudent(struct Student students[], int count);
void exportToFile(struct Student students[], int count);
int validateMarks(float mark);
char calculateLetterGrade(float total);
void saveData(struct Student students[], int count);
int loadData(struct Student students[], int *count);

int main() {
    struct Student students[MAX_STUDENTS];
    int student_count = 0;
    int choice;

    // Load existing data
    if (loadData(students, &student_count)) {
        printf("Loaded %d student records\n", student_count);
    }

    do {
        printf("\n--- Student Academic Management System ---\n");
        printf("1. Add Student\n");
        printf("2. Register Courses\n");
        printf("3. Input Marks\n");
        printf("4. Display Student Record\n");
        printf("5. Export Report to File\n");
        printf("6. Save Data\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(students, &student_count); break;
            case 2: registerCourses(students, student_count); break;
            case 3: inputMarks(students, student_count); break;
            case 4: searchStudent(students, student_count); break;
            case 5: exportToFile(students, student_count); break;
            case 6: saveData(students, student_count); break;
            case 7: break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 7);

    // Save before exit
    saveData(students, student_count);
    printf("Data saved. Exiting...\n");
    return 0;
}



// Actual function defination to be added later
//Dummy functions bellow
void addStudent(struct Student students[], int *count) {
    if(*count >= MAX_STUDENTS) {
        printf("Maximum students reached!\n");
        return;
    }

    struct Student new_student;
    
    printf("\nEnter student name: ");
    scanf(" %[^\n]", new_student.name);
    
    printf("Enter student ID: ");
    scanf("%s", new_student.id);
    
    printf("Enter department: ");
    scanf(" %[^\n]", new_student.dept);
    
    printf("Enter email: ");
    scanf("%s", new_student.email);
    new_student.course_count = 0;
    students[*count] = new_student;
    (*count)++;
    
    printf("Student added successfully!\n");
}

void registerCourses(struct Student students[], int count) {
    printf("registerCourses() called\n");
}

void inputMarks(struct Student students[], int count) {
    printf("inputMarks() called\n");
}

void calculateGrade(struct Student *student) {
    printf("calculateGrade() called\n");
}

void displayStudent(struct Student student) {
    printf("\nName: %s\nID: %s\nDept: %s\nEmail: %s\n", 
           student.name, student.id, student.dept, student.email);
    
    printf("\nCourses:\n");
    for(int i = 0; i < student.course_count; i++) {
        printf("  %s: Quiz=%.2f, Mid=%.2f, Final=%.2f, Total=%.2f, Grade=%c\n",
               student.courses[i].code,
               student.courses[i].quiz,
               student.courses[i].midterm,
               student.courses[i].final,
               student.courses[i].total,
               student.courses[i].grade)
            ;
    }
}

void searchStudent(struct Student students[], int count) {
    char id[MAX_ID_LEN];
    printf("Enter student ID: ");
    scanf("%s", id);
    
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].id, id) == 0) {
            displayStudent(students[i]);
            return;
        }
    }
    printf("Student not found!\n");
}


void exportToFile(struct Student students[], int count) {
    printf("exportToFile() called\n");
}

int validateMarks(float mark) {
    return 1;
}

char calculateLetterGrade(float total) {
    return 'A';
}

void saveData(struct Student students[], int count) {
    printf("saveData() called\n");
}

int loadData(struct Student students[], int *count) {
    printf("loadData() called\n");
    return 0;
}

