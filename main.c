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

   
    saveData(students, student_count);
    printf("Data saved. Exiting...\n");
    return 0;
}

// ... [All other functions remain unchanged] ...

// Add these new functions:

void saveData(struct Student students[], int count) {
    FILE *file = fopen("student_data.txt", "w");
    if (!file) {
        printf("Error saving data!\n");
        return;
    }

    fprintf(file, "%d\n", count);
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n%s\n%d\n", 
               students[i].name,
               students[i].id,
               students[i].dept,
               students[i].email,
               students[i].course_count);
        
        for (int j = 0; j < students[i].course_count; j++) {
            fprintf(file, "%s\n%.1f\n%.1f\n%.1f\n%.1f\n%c\n",
                   students[i].courses[j].code,
                   students[i].courses[j].quiz,
                   students[i].courses[j].midterm,
                   students[i].courses[j].final,
                   students[i].courses[j].total,
                   students[i].courses[j].grade);
        }
    }
    fclose(file);
    printf("Data saved successfully!\n");
}

int loadData(struct Student students[], int *count) {
    FILE *file = fopen("student_data.txt", "r");
    if (!file) {
        return 0;  // File doesn't exist (first run)
    }

    if (fscanf(file, "%d", count) != 1) {
        fclose(file);
        return 0;
    }
    
    // Prevent overflow
    if (*count > MAX_STUDENTS) *count = MAX_STUDENTS;
    
    for (int i = 0; i < *count; i++) {
        fscanf(file, " %[^\n]", students[i].name);
        fscanf(file, " %[^\n]", students[i].id);
        fscanf(file, " %[^\n]", students[i].dept);
        fscanf(file, " %[^\n]", students[i].email);
        fscanf(file, "%d", &students[i].course_count);
        
        // Prevent course overflow
        if (students[i].course_count > MAX_COURSES) {
            students[i].course_count = MAX_COURSES;
        }
        
        for (int j = 0; j < students[i].course_count; j++) {
            fscanf(file, " %[^\n]", students[i].courses[j].code);
            fscanf(file, "%f", &students[i].courses[j].quiz);
            fscanf(file, "%f", &students[i].courses[j].midterm);
            fscanf(file, "%f", &students[i].courses[j].final);
            fscanf(file, "%f", &students[i].courses[j].total);
            fscanf(file, " %c", &students[i].courses[j].grade);
        }
    }
    fclose(file);
    return 1;
}
void addStudent(struct Student students[], int *count) {
    if(*count >= MAX_STUDENTS) {
        printf("Maximum students reached!\n");
        return;
    }

    struct Student new_student;
    
    printf("\nEnter student name: ");
    scanf(" %[^\n]s", new_student.name);
    
    printf("Enter student ID: ");
    scanf("%s", new_student.id);
    
    printf("Enter department: ");
    scanf(" %[^\n]s", new_student.dept);
    
    printf("Enter email: ");
    scanf("%s", new_student.email);
    
    new_student.course_count = 0;
    students[*count] = new_student;
    (*count)++;
    
    printf("Student added successfully!\n");
}

void registerCourses(struct Student students[], int count) {
    char id[MAX_ID_LEN];
    printf("Enter student ID: ");
    scanf("%s", id);

    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].id, id) == 0) {
            if(students[i].course_count >= MAX_COURSES) {
                printf("Maximum courses reached!\n");
                return;
            }
            
            printf("Enter course code: ");
            scanf("%s", students[i].courses[students[i].course_count].code);
            students[i].course_count++;
            printf("Course registered!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

int validateMarks(float mark) {
    return (mark >= 0 && mark <= 100);
}

char calculateLetterGrade(float total) {
    if(total >= 90) return 'A';
    if(total >= 80) return 'B';
    if(total >= 70) return 'C';
    if(total >= 60) return 'D';
    return 'F';
}

void calculateGrade(struct Student *student) {
    for(int i = 0; i < student->course_count; i++) {
        student->courses[i].total = 
            student->courses[i].quiz +
            student->courses[i].midterm +
            student->courses[i].final;
            
        student->courses[i].grade = 
            calculateLetterGrade(student->courses[i].total);
    }
}

void inputMarks(struct Student students[], int count) {
    char id[MAX_ID_LEN], course[MAX_COURSE_LEN];
    printf("Enter student ID: ");
    scanf("%s", id);
    
    for(int i = 0; i < count; i++) {
        if(strcmp(students[i].id, id) == 0) {
            printf("Enter course code: ");
            scanf("%s", course);
            
            for(int j = 0; j < students[i].course_count; j++) {
                if(strcmp(students[i].courses[j].code, course) == 0) {
                    do {
                        printf("Enter quiz marks (0-20): ");
                        scanf("%f", &students[i].courses[j].quiz);
                    } while(!validateMarks(students[i].courses[j].quiz));
                    
                    do {
                        printf("Enter midterm marks (0-30): ");
                        scanf("%f", &students[i].courses[j].midterm);
                    } while(!validateMarks(students[i].courses[j].midterm));
                    
                    do {
                        printf("Enter final marks (0-50): ");
                        scanf("%f", &students[i].courses[j].final);
                    } while(!validateMarks(students[i].courses[j].final));
                    
                    calculateGrade(&students[i]);
                    return;
                }
            }
            printf("Course not found!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

void displayStudent(struct Student student) {
    printf("\nName: %s\nID: %s\nDept: %s\nEmail: %s\n", 
           student.name, student.id, student.dept, student.email);
    
    printf("\nCourses:\n");
    for(int i = 0; i < student.course_count; i++) {
        printf("  %s: Quiz=%.1f, Mid=%.1f, Final=%.1f, Total=%.1f, Grade=%c\n",
               student.courses[i].code,
               student.courses[i].quiz,
               student.courses[i].midterm,
               student.courses[i].final,
               student.courses[i].total,
               student.courses[i].grade);
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
    char filename[100];
    printf("Enter output filename: ");
    scanf("%s", filename);
    
    FILE *file = fopen(filename, "w");
    if(!file) {
        printf("Error opening file!\n");
        return;
    }
    
    for(int i = 0; i < count; i++) {
        fprintf(file, "Name: %s\nID: %s\nDepartment: %s\nEmail: %s\n",
                students[i].name, students[i].id, 
                students[i].dept, students[i].email);
        
        for(int j = 0; j < students[i].course_count; j++) {
            fprintf(file, "  Course: %s, Total: %.1f, Grade: %c\n",
                    students[i].courses[j].code,
                    students[i].courses[j].total,
                    students[i].courses[j].grade);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Data exported successfully!\n");
}