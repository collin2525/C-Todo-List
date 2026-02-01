#include<stdio.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_LENGTH 100

char tasks[MAX_TASKS][MAX_LENGTH];
int task_count = 0;

void add_task(void);
void view_tasks(void);
void delete_task(void);

int main(void){
    printf("Hello\n");
    fflush(stdout);
    while(1){
        printf("\n1) Add task\n");
        printf("2) View tasks\n");
        printf("3) Delete task\n");
        printf("4) Quit\n");
        printf("> ");

        int choice;
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice)
        {
        case 1: add_task(); break;
        case 2: view_tasks(); break;
        case 3: delete_task(); break;
        case 4: return 0;
        
        default: printf("Invalid choice\n");
        }
    }
    return 0;
}

void add_task() {
    if (task_count >= MAX_TASKS){
        printf("Task List Full!\n");
        return;
    }

    printf("Enter task: ");
    fgets(tasks[task_count], MAX_LENGTH, stdin);
    tasks[task_count][strcspn(tasks[task_count], "\n")] = 0;

    task_count++;
}

void view_tasks() {
    if (task_count == 0){
        printf("No Tasks.\n");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        printf("%d. %s\n", i + 1, tasks[i]);
    }
}

void delete_task() {
    int index;
    printf("Enter task number to delete: ");
    scanf("%d", &index);
    getchar();

    if (index < 1 || index > task_count) {
        printf("Invalid task number.\n");
        return;
    }

    for (int i = index - 1; i < task_count - 1; i++) {
        strcpy(tasks[i], tasks[i + 1]);
    }

    task_count--;
}