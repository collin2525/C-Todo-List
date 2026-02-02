#include <stdio.h>
#include <string.h>
#include "string.h"
#include <stdbool.h>
#include <stdlib.h>


#define MAX_TASKS 100
#define MAX_LENGTH 256
StringArray tasks;

void run_command(String command);
void print_help(void);
void add_task(String task);
void view_tasks(StringArray tasks);
void delete_task(String task);
void save_tasks(void);
void load_tasks(void);
int find_task(String task);
StringArray find_tasts_by_content(String content);
void clear_tasks(void);

int main(void){
    tasks.items = malloc(sizeof(String) * MAX_TASKS);
    String welcome = string_create("Welcome to the Todo List Application!");
    printf("%s\n", welcome.data);
    print_help();
    string_free(&welcome);
    load_tasks();
    fflush(stdout);
    while(1){
        char input_buffer[MAX_LENGTH];
        printf("\n> ");
        if (fgets(input_buffer, MAX_LENGTH, stdin) == NULL){
            printf("Error reading input. Exiting.\n");
            break;
        }
        input_buffer[strcspn(input_buffer, "\n")] = '\0';
        String command = string_create(input_buffer);
        if (string_equals_cstr(&command, "exit")){
            string_free(&command);
            printf("Exiting...\n");
            save_tasks();
            break;
        }
        run_command(command);
        string_free(&command);
    }
    clear_tasks();
    free(tasks.items);
    return 0;
}

void run_command(String command){
    StringArray parts = string_split(&command, ':');
    if (parts.count == 0){
        string_array_free(&parts);
        return;
    }
    if (string_equals_cstr(&parts.items[0], "add")){
        if (parts.count < 2){
            printf("Usage: add:[task here]\n");
        } else {
            add_task(parts.items[1]);
        }
    } else if (string_equals_cstr(&parts.items[0], "view")){
        printf(tasks.count == 0 ? "No Tasks 0.\n" : "Tasks:\n");
        view_tasks(tasks);
    } else if (string_equals_cstr(&parts.items[0], "delete")){
        if (parts.count < 2){
            printf("Usage: delete:[task here]\n");
        } else {
            delete_task(parts.items[1]);
        }
    } else if (string_equals_cstr(&parts.items[0], "save")){
        save_tasks();
    } else if (string_equals_cstr(&parts.items[0], "find")){
        if (parts.count < 2){
            printf("Usage: find:[part task name here]\n");
        } else {
            view_tasks(find_tasts_by_content(parts.items[1]));
        }
    } else if (string_equals_cstr(&parts.items[0], "clear")){
        clear_tasks();
    } else if (string_equals_cstr(&parts.items[0], "help")){
        print_help();
    } else if (string_equals_cstr(&parts.items[0], "exit")){
        printf("Exiting...\n");
        save_tasks();
    } else {
        printf("Unknown command. Type 'help' for a list of commands.\n");
    }
    
    string_array_free(&parts);
}

void print_help() {
    printf("Todo List Application Commands:\n");
    printf("add:[task here]- Add a new task\n");
    printf("view - View all tasks\n");
    printf("delete:[task here] - Delete a task\n");
    printf("save - Save tasks to file\n");
    printf("find:[part task name here] - Find tasks by its content\n");
    printf("clear - Clear all tasks\n");
    printf("help - Show this help message\n");
    printf("exit - Exit the application\n");
}

void add_task(String task) {
    if (tasks.count >= MAX_TASKS){
        printf("Task List Full!\n");
        return;
    }
    // Duplicate the string to store
    add_string_to_array(&tasks, string_create(task.data));
}

void view_tasks(StringArray tasks) {
    if (tasks.count == 0){
        printf("No Tasks.\n");
        return;
    }

    for (int i = 0; i < tasks.count; i++) {
        printf("%d. %s\n", i + 1, tasks.items[i].data);
    }
}

void delete_task(String task) {
    int index = find_task(task);
    if (index == -1) {
        return;
    }

    remove_string_from_array(&tasks, index);
    printf("Task deleted: %s\n", task.data);

    string_free(&tasks.items[tasks.count - 1]);
    tasks.count--;
}

void save_tasks() {
    FILE *file = fopen("tasks.txt", "w");

    if (file == NULL){
        printf("Failed to open file for writing.\n");
        return;
    }
    for (int i = 0; i < tasks.count; i++){
        fprintf(file, "%s\n", tasks.items[i].data);
    }

    fclose(file);
    printf("Tasks saved.\n");
}

void load_tasks(){
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        printf("No tasks.txt file found.\n");
        return;
    }
    char buffer[MAX_LENGTH];
    while (fgets(buffer, MAX_LENGTH, file)){
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > 0) {  // Only add non-empty lines
            add_string_to_array(&tasks, string_create(buffer));
            printf("Loaded task: %s\n", buffer);
        }
        if (tasks.count >= MAX_TASKS){
            break;
        }
    }
    fclose(file);
    printf("Total tasks loaded: %d\n", tasks.count);
}

int find_task(String task) {
    for (int i = 0; i < tasks.count; i++) {
        if (string_equals(&tasks.items[i], &task)) {
            printf("Task found at index %d: %s\n", i + 1, tasks.items[i].data);
            return i;
        }
    }
    printf("Task not found.\n");
    return -1;
}

StringArray find_tasts_by_content(String content) {
    StringArray results;
    results.count = 0;
    results.items = NULL;

    for (int i = 0; i < tasks.count; i++) {
        if (string_contains(&tasks.items[i], &content)) {
            results.items = (String *)realloc(results.items, sizeof(String) * (results.count + 1));
            results.items[results.count] = string_create(tasks.items[i].data);
            results.count++;
        }
    }

    return results;
}

void clear_tasks() {
    string_array_free(&tasks);
    printf("All tasks cleared.\n");
}
