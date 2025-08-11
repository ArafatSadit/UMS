#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 1000
#define MAX_COURSES 5
#define MAX_NAME_LEN 50
#define MAX_DEPT_LEN 30
#define MAX_EMAIL_LEN 50
#define MAX_ID_LEN 15
#define MAX_COURSE_LEN 10
#define MAX_PASSWORD_LEN 30
#define MAX_FACULTY 50

// ROLES
#define ROLE_ADMIN 0
#define ROLE_FACULTY 1
#define ROLE_STUDENT 2
#define ROLE_INVALID -1

struct Course {
    char code[MAX_COURSE_LEN];
    float quiz;
    float midterm;
    float final;
    float total;
    char grade;
    float gp;
};

struct Student {
    char name[MAX_NAME_LEN];
    char id[MAX_ID_LEN];
    char dept[MAX_DEPT_LEN];
    char email[MAX_EMAIL_LEN];
    char password[MAX_PASSWORD_LEN];
    struct Course courses[MAX_COURSES];
    int course_count;
    float cgpa;
};

struct Faculty {
    char name[MAX_NAME_LEN];
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];
};

// Function prototypes
void toLowerStr(char *dest, const char *src);
void displayStudent(struct Student student);
void partialSearch(struct Student students[MAX_STUDENTS], int count);

void registerCourses(struct Student students[], int count);
void studentRegisterCourse(struct Student *student);

int adminLogin();
int facultyLogin(struct Faculty faculty[], int faculty_count, char logged_in_id[]);
int studentLogin(struct Student student[], int student_count, char logged_in_id[]);

void addStudent(struct Student students[], int *count);
void addFaculty(struct Faculty faculty[], int *count);

void inputMarks(struct Student students[], int count);
void calculateGrade(struct Student *student);
void exportToFile(struct Student students[], int count);
int validateMarks(float mark, float max);
char calculateLetterGrade(float total);
float letterGradeToGP(char grade);

int loadFaculty(struct Faculty faculty[], int *count);
void saveFaculty(struct Faculty faculty[], int count);
int loadData(struct Student students[], int *count);
void saveData(struct Student students[], int count);

int main() {
    struct Student students[MAX_STUDENTS];
    int student_count = 0;
    struct Faculty faculty[MAX_FACULTY];
    int faculty_count = 0;
    int role = ROLE_INVALID;
    char user_id[MAX_ID_LEN] = {0};

    loadFaculty(faculty, &faculty_count);
    loadData(students, &student_count);

    while (1) {
        while (role == ROLE_INVALID) {
            int login_choice;
            printf("\n--- University Login Portal ---\n");
            printf("1. Student Login\n2. Faculty Login\n3. Admin Login\n4. Exit\n");
            printf("Select your role: ");
            scanf("%d", &login_choice);
            getchar();

            switch (login_choice) {
                case 1:
                    if (studentLogin(students, student_count, user_id)) role = ROLE_STUDENT;
                    break;
                case 2:
                    if (facultyLogin(faculty, faculty_count, user_id)) role = ROLE_FACULTY;
                    break;
                case 3:
                    if (adminLogin()) {
                        role = ROLE_ADMIN;
                        strcpy(user_id, "admin");
                    }
                    break;
                case 4:
                    printf("Exiting...\n");
                    saveData(students, student_count);
                    saveFaculty(faculty, faculty_count);
                    return 0;
                default:
                    printf("Invalid choice.\n");
            }
        }

        int choice;
        do {
            printf("\n--- University Academic Management System ---\n");
            if (role == ROLE_ADMIN) {
                printf("1. Add Student\n2. Add Faculty\n3. Register Courses\n4. Input Marks\n5. Search Student\n6. Export Report\n7. Logout\n8. Exit\n");
            } else if (role == ROLE_FACULTY) {
                printf("1. Input Marks\n2. Search Student\n3. Export Report\n4. Logout\n5. Exit\n");
            } else if (role == ROLE_STUDENT) {
                printf("1. Display My Info\n2. Register Course\n3. Logout\n4. Exit\n");
            }

            printf("Enter choice: ");
            scanf("%d", &choice);
            getchar();

            if (role == ROLE_ADMIN) {
                switch (choice) {
                    case 1: addStudent(students, &student_count); saveData(students, student_count); break;
                    case 2: addFaculty(faculty, &faculty_count); saveFaculty(faculty, faculty_count); break;
                    case 3: registerCourses(students, student_count); break;
                    case 4: inputMarks(students, student_count); break;
                    case 5: partialSearch(students, student_count); break;
                    case 6: exportToFile(students, student_count); break;
                    case 7: role = ROLE_INVALID; break;
                    case 8: saveData(students, student_count); saveFaculty(faculty, faculty_count); return 0; break;
                    default: printf("Invalid choice!\n");
                }
            } else if (role == ROLE_FACULTY) {
                switch (choice) {
                    case 1: inputMarks(students, student_count); saveData(students, student_count); break;
                    case 2: partialSearch(students, student_count); break;
                    case 3: exportToFile(students, student_count); break;
                    case 4: role = ROLE_INVALID; break;
                    case 5: saveData(students, student_count); return 0; break;
                    default: printf("Invalid choice!\n");
                }
            } else if (role == ROLE_STUDENT) {
                int found = 0;
                switch (choice) {
                    case 1:
                        for (int i = 0; i < student_count; i++) {
                            if (strcmp(students[i].id, user_id) == 0) {
                                displayStudent(students[i]);
                                found = 1;
                                break;
                            }
                        }
                        if (!found) printf("Error: Could not find your student record.\n");
                        break;
                    case 2:
                        for (int i = 0; i < student_count; i++) {
                            if (strcmp(students[i].id, user_id) == 0) {
                                studentRegisterCourse(&students[i]);
                                found = 1;
                                break;
                            }
                        }
                        if (!found) printf("Error: Could not find your student record.\n");
                        break;
                    case 3: role = ROLE_INVALID; break;
                    case 4: saveData(students, student_count); return 0; break;
                    default: printf("Invalid choice!\n");
                }
            }
        } while (role != ROLE_INVALID);
    }

    saveData(students, student_count);
    saveFaculty(faculty, faculty_count);

    return 0;
}

void toLowerStr(char *dest, const char *src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower(src[i]);
        i++;
    }
    dest[i] = '\0';
}

void displayStudent(struct Student student) {
    printf("\nName: %s\nID: %s\nDept: %s\nEmail: %s\n",
           student.name, student.id, student.dept, student.email);

    printf("\nCourses:\n");
    for (int i = 0; i < student.course_count; i++) {
        printf("  %s: Quiz=%.1f, Mid=%.1f, Final=%.1f, Total=%.1f, Grade=%c, GP=%.2f\n",
               student.courses[i].code,
               student.courses[i].quiz,
               student.courses[i].midterm,
               student.courses[i].final,
               student.courses[i].total,
               student.courses[i].grade,
               student.courses[i].gp);
    }
    printf("CGPA: %.2f\n", student.cgpa);
}

void partialSearch(struct Student students[MAX_STUDENTS], int count) {
    int i, found = 0;
    char search_term[MAX_NAME_LEN];
    char search_term_lower[MAX_NAME_LEN];
    char name_lower[MAX_NAME_LEN];
    char id_lower[MAX_ID_LEN];

    printf("Enter Student Name or ID : ");
    scanf(" %[^\n]", search_term);

    toLowerStr(search_term_lower, search_term);

    for (i = 0; i < count; i++) {
        toLowerStr(name_lower, students[i].name);
        toLowerStr(id_lower, students[i].id);

        if (strstr(name_lower, search_term_lower) || strstr(id_lower, search_term_lower)) {
            displayStudent(students[i]);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("No matches found\n");
}

void registerCourses(struct Student students[], int count) {
    char id[MAX_ID_LEN];
    printf("Enter student ID: ");
    scanf("%s", id);

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            if (students[i].course_count >= MAX_COURSES) {
                printf("Maximum courses reached!\n");
                return;
            }

            printf("Enter course code: ");
            scanf("%s", students[i].courses[students[i].course_count].code);
            students[i].course_count++;
            calculateGrade(&students[i]);
            printf("Course registered!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

void studentRegisterCourse(struct Student *student) {
    if ((*student).course_count >= MAX_COURSES) {
        printf("Maximum courses reached!\n");
        return;
    }

    printf("Enter course code: ");
    scanf("%s", (*student).courses[(*student).course_count].code);
    (*student).course_count++;

    calculateGrade(student);

    printf("Course registered!\n");
}

int adminLogin() {
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];

    printf("\n--- Admin Login ---\n");
    printf("Enter Admin ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    if (strcmp(id, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("Admin login successful!\n");
        return 1;
    }

    printf("Invalid credentials!\n");
    return 0;
}

int facultyLogin(struct Faculty faculty[], int faculty_count, char logged_in_id[]) {
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];

    printf("\n--- Faculty Login ---\n");
    printf("Enter Faculty ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    for (int i = 0; i < faculty_count; i++) {
        if (strcmp(faculty[i].id, id) == 0 && strcmp(faculty[i].password, password) == 0) {
            printf("Faculty login successful!\n");
            strcpy(logged_in_id, id);
            return 1;
        }
    }

    printf("Invalid credentials!\n");
    return 0;
}

int studentLogin(struct Student students[], int student_count, char logged_in_id[]) {
    char id[MAX_ID_LEN];
    char password[MAX_PASSWORD_LEN];

    printf("\n--- Student Login ---\n");
    printf("Enter Student ID: ");
    scanf("%s", id);
    printf("Enter Password: ");
    scanf("%s", password);

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].id, id) == 0 && strcmp(students[i].password, password) == 0) {
            printf("Student login successful!\n");
            strcpy(logged_in_id, id);
            return 1;
        }
    }

    printf("Invalid credentials!\n");
    return 0;
}

void addStudent(struct Student students[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("Maximum students reached!\n");
        return;
    }
    struct Student new_student;

    printf("\nEnter student name: ");
    scanf(" %[^\n]", new_student.name);

    printf("Enter student ID: ");
    scanf("%s", new_student.id);

    printf("Set password for student login: ");
    scanf("%s", new_student.password);

    printf("Enter department: ");
    scanf(" %[^\n]", new_student.dept);

    printf("Enter email: ");
    scanf("%s", new_student.email);

    new_student.course_count = 0;
    new_student.cgpa = 0.0f;
    students[*count] = new_student;
    (*count)++;

    printf("Student added successfully!\n");
}

void addFaculty(struct Faculty faculty[], int *count) {
    if (*count >= MAX_FACULTY) {
        printf("Maximum faculty reached!\n");
        return;
    }
    printf("Enter faculty name: ");
    scanf(" %[^\n]", faculty[*count].name);
    printf("Enter faculty ID: ");
    scanf("%s", faculty[*count].id);
    printf("Set password: ");
    scanf("%s", faculty[*count].password);
    (*count)++;
    printf("Faculty added successfully!\n");
}

void inputMarks(struct Student students[], int count) {
    char id[MAX_ID_LEN], course[MAX_COURSE_LEN];
    printf("Enter student ID: ");
    scanf("%s", id);

    int studentIndex = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].id, id) == 0) {
            studentIndex = i;
            break;
        }
    }
    if (studentIndex == -1) {
        printf("Student not found!\n");
        return;
    }

    printf("Enter course code: ");
    scanf("%s", course);

    int courseIndex = -1;
    for (int j = 0; j < students[studentIndex].course_count; j++) {
        if (strcmp(students[studentIndex].courses[j].code, course) == 0) {
            courseIndex = j;
            break;
        }
    }
    if (courseIndex == -1) {
        printf("Course not found!\n");
        return;
    }

    float q, m, f;

    do {
        printf("Enter quiz marks (0-20): ");
        if (scanf("%f", &q) != 1) {
            while (getchar() != '\n');
            q = -1;
        }
    } while (!validateMarks(q, 20));

    do {
        printf("Enter midterm marks (0-30): ");
        if (scanf("%f", &m) != 1) {
            while (getchar() != '\n');
            m = -1;
        }
    } while (!validateMarks(m, 30));

    do {
        printf("Enter final marks (0-50): ");
        if (scanf("%f", &f) != 1) {
            while (getchar() != '\n');
            f = -1;
        }
    } while (!validateMarks(f, 50));

    students[studentIndex].courses[courseIndex].quiz = q;
    students[studentIndex].courses[courseIndex].midterm = m;
    students[studentIndex].courses[courseIndex].final = f;

    calculateGrade(&students[studentIndex]);

    printf("Marks updated successfully.\n");
}

void calculateGrade(struct Student *student) {
    float totalGP = 0.0f;
    for (int i = 0; i < student->course_count; i++) {
        student->courses[i].total =
            student->courses[i].quiz +
            student->courses[i].midterm +
            student->courses[i].final;

        student->courses[i].grade = calculateLetterGrade(student->courses[i].total);

        student->courses[i].gp = letterGradeToGP(student->courses[i].grade);

        totalGP += student->courses[i].gp;
    }

    if (student->course_count > 0)
        student->cgpa = totalGP / student->course_count;
    else
        student->cgpa = 0.0f;
}

char calculateLetterGrade(float total) {
    if (total >= 80)
        return 'A';
    else if (total >= 70)
        return 'B';
    else if (total >= 60)
        return 'C';
    else if (total >= 50)
        return 'D';
    else
        return 'F';
}

float letterGradeToGP(char grade) {
    switch (grade) {
        case 'A': return 4.0f;
        case 'B': return 3.0f;
        case 'C': return 2.0f;
        case 'D': return 1.0f;
        default:  return 0.0f;
    }
}

void exportToFile(struct Student students[], int count) {
    FILE *file = fopen("report.txt", "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Name: %s\nID: %s\nDept: %s\nEmail: %s\n",
                students[i].name, students[i].id, students[i].dept, students[i].email);

        fprintf(file, "Courses:\n");
        for (int j = 0; j < students[i].course_count; j++) {
            fprintf(file, "  %s: Quiz=%.1f, Mid=%.1f, Final=%.1f, Total=%.1f, Grade=%c, GP=%.2f\n",
                    students[i].courses[j].code,
                    students[i].courses[j].quiz,
                    students[i].courses[j].midterm,
                    students[i].courses[j].final,
                    students[i].courses[j].total,
                    students[i].courses[j].grade,
                    students[i].courses[j].gp);
        }
        fprintf(file, "CGPA: %.2f\n\n", students[i].cgpa);
    }

    fclose(file);
    printf("Report exported to report.txt\n");
}

int validateMarks(float mark, float max) {
    if (mark < 0 || mark > max) {
        printf("Invalid marks, must be between 0 and %.1f\n", max);
        return 0;
    }
    return 1;
}

int loadFaculty(struct Faculty faculty[], int *count) {
    FILE *file = fopen("faculty_data.txt", "r");
    if (!file) return 0;

    *count = 0;
    while (fscanf(file, " %[^\n]", faculty[*count].name) == 1) {
        fscanf(file, "%s", faculty[*count].id);
        fscanf(file, "%s", faculty[*count].password);
        (*count)++;
        if (*count >= MAX_FACULTY) break;
    }

    fclose(file);
    return 1;
}

void saveFaculty(struct Faculty faculty[], int count) {
    FILE *file = fopen("faculty_data.txt", "w");
    if (!file) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n",
                faculty[i].name,
                faculty[i].id,
                faculty[i].password);
    }

    fclose(file);
}

int loadData(struct Student students[], int *count) {
    FILE *file = fopen("student_data.txt", "r");
    if (!file) return 0;

    *count = 0;
    while (fscanf(file, " %[^\n]", students[*count].name) == 1) {
        fscanf(file, "%s", students[*count].id);
        fscanf(file, " %[^\n]", students[*count].dept);
        fscanf(file, "%s", students[*count].email);
        fscanf(file, "%s", students[*count].password);
        fscanf(file, "%d", &students[*count].course_count);

        if (students[*count].course_count > MAX_COURSES) {
            students[*count].course_count = MAX_COURSES;
        }

        fscanf(file, "%f", &students[*count].cgpa);

        for (int j = 0; j < students[*count].course_count; j++) {
            fscanf(file, " %[^\n]", students[*count].courses[j].code);
            fscanf(file, "%f", &students[*count].courses[j].quiz);
            fscanf(file, "%f", &students[*count].courses[j].midterm);
            fscanf(file, "%f", &students[*count].courses[j].final);
            fscanf(file, "%f", &students[*count].courses[j].total);
            fscanf(file, " %c", &students[*count].courses[j].grade);
            fscanf(file, "%f", &students[*count].courses[j].gp);
        }

        (*count)++;
        if (*count >= MAX_STUDENTS) break;
    }

    fclose(file);
    return 1;
}

void saveData(struct Student students[], int count) {
    FILE *file = fopen("student_data.txt", "w");
    if (!file) return;

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s\n%s\n%s\n%s\n%s\n%d\n%.2f\n",
                students[i].name,
                students[i].id,
                students[i].dept,
                students[i].email,
                students[i].password,
                students[i].course_count,
                students[i].cgpa);

        for (int j = 0; j < students[i].course_count; j++) {
            fprintf(file, "%s\n%.1f\n%.1f\n%.1f\n%.1f\n%c\n%.2f\n",
                    students[i].courses[j].code,
                    students[i].courses[j].quiz,
                    students[i].courses[j].midterm,
                    students[i].courses[j].final,
                    students[i].courses[j].total,
                    students[i].courses[j].grade,
                    students[i].courses[j].gp);
        }
    }

    fclose(file);
}
