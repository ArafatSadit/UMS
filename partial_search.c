#include <stdio.h>
#include <string.h>

#define MAX 100

struct Student {
    char name[50];
    int id;
};

int main() {
    struct Student students[MAX] = {
        {"Alice Johnson", 1},
        {"Bob Smith", 2},
        {"Charlie Brown", 3},
        {"Alicia Keys", 4},
        {"Albert Einstein", 5}
    };
    int n = 5;

    char search_term[50];
    printf("Enter partial name to search: ");
    scanf("%s", search_term); // you can use scanf("%49[^\n]", search_term); to allow spaces

    printf("\nResults:\n");
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (strstr(students[i].name, search_term)) {
            printf("ID: %d, Name: %s\n", students[i].id, students[i].name);
            found = 1;
        }
    }

    if (!found)
        printf("No match found.\n");

    return 0;
}
//will implement this in the main code somehow

