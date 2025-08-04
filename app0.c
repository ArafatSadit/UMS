#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 20000
#define MAX_COURSES 3
#define MAX_NAME_LEN 50
#define MAX_DEPT_LEN 30
#define MAX_EMAIL_LEN 50
#define MAX_ID_LEN 15
#define MAX_COURSE_LEN 10


struct Courses
{

};

struct Students
{
    char name[MAX_NAME_LEN];
    int id[MAX_ID_LEN];
    char email[MAX_EMAIL_LEN] ;
    char dept[MAX_DEPT_LEN];
    
    
}