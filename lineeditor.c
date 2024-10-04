#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// Data structure to store file dataaaaaaaaaaaaaaaa
typedef struct storage_node {
    char data[100];
    struct storage_node *next;
} storage_node;
//variables declaration
storage_node *head = NULL;
storage_node *tail = NULL;
#define SIZE 5
FILE *fp ;
int stack[SIZE] = {0, 0, 0, 0, 0};
int top = -1;
int current_state = 0;

//functions.................................................
void undo() {
    int prev_state;
if (current_state == 1) {
    prev_state = SIZE;
} else {
    prev_state = current_state - 1;
}
    char filename[20];
    snprintf(filename, sizeof(filename), "state%d.txt", prev_state);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No previous state to undo to.\n");
        return;
    }
    storage_node *current = head;
    while (current != NULL) {
        storage_node *next = current->next;
        free(current);
        current = next;
    }
    head = tail = NULL;
    storing_data(fp);
    fclose(fp);
    current_state = prev_state;
}

void redo() {
    int next_state;
if (current_state == SIZE) {
    next_state = 1;
} else {
    next_state = current_state + 1;
}
    char filename[20];
    snprintf(filename, sizeof(filename), "state%d.txt", next_state);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("No next state to redo to.\n");
        return;
    }
    // Clear current list
    storage_node *current = head;
    while (current != NULL) {
        storage_node *next = current->next;
        free(current);
        current = next;
    }
    head = tail = NULL;
    storing_data(fp);
    fclose(fp);
    current_state = next_state;
}

void push(int x) {

    if (top == SIZE - 1) {
        printf("Undo stack is full.\n");
    } else {
        top = top + 1;
        stack[top] = x;
        redo();
    }
}

void pop() {
    if (top == -1) {
        printf("Cannot do any more undo operation!\n");
    } else {
        top = top - 1;
        undo();
    }
}

void menu() {

    printf("\n1 - Create a new node (beginning or end)\n");
    printf("2 - Delete a node\n");
    printf("3 - Replace\n");
    printf("4 - Display\n");
    printf("5 - Replace a specific word\n");
    printf("6 - Undo\n");
    printf("7 - Redo\n");
    printf("8 - Clear all data\n");
    printf("9 - Exit\n");
}

void clearall() {
    fp = fopen("file.txt", "w+");
    fp = fopen("state1.txt", "w+");
    fp = fopen("state2.txt", "w+");
    fp = fopen("state3.txt", "w+");
    fp = fopen("state4.txt", "w+");
    fp = fopen("state5.txt", "w+");
}

void storing_data(FILE *fp) {
    char ch[100];
    while (fgets(ch, sizeof(ch), fp) != NULL) {
        storage_node *newnode = (storage_node *)malloc(sizeof(storage_node));
        newnode->next = NULL;
        strcpy(newnode->data, ch);

        if (head == NULL) {
            head = newnode;
            tail = head;
        } else {
            tail->next = newnode;
            tail = newnode;
        }
    }
}

void save_state(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("failed to open savemode\n");
        return;
    }
    storage_node *current = head;
    while (current != NULL) {
        fprintf(fp, "%s", current->data);
        current = current->next;
    }
    fclose(fp);
}

void make_copy() {
    current_state++;
    current_state = (current_state - 1) % SIZE + 1;
    char filename[20];
    snprintf(filename, sizeof(filename), "state%d.txt", current_state);
    save_state(filename);
}

void close_program() {
    FILE *fp = fopen("file.txt", "w+");
    storage_node *current = head;
    while (current != NULL) {
        fprintf(fp, "%s", current->data);
        current = current->next;
    }
    fclose(fp);
    exit(0);
}

void display() {
    system("cls");
    printf(".....................................................................\n");
    storage_node *current = head;
    while (current != NULL) {
        printf("%s", current->data);
        current = current->next;
    }
}

void createnode() {
    int ch;
    char line[100];
    printf("1 - At the end 2 - At the start: ");
    scanf("%d", &ch);
    getchar();

    printf("Enter the line you want to add here: ");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = '\0';

    storage_node *newnode = (storage_node *)malloc(sizeof(storage_node));
    strcpy(newnode->data, line);
    strcat(newnode->data, "\n");
    newnode->next = NULL;

    if (ch == 1) {
        if (head == NULL) {
            head = newnode;
            tail = head;
        } else {
            tail->next = newnode;
            tail = newnode;
        }
    } else if (ch == 2) {
        newnode->next = head;
        head = newnode;
        if (tail == NULL) {
            tail = newnode;
        }
    } else {
        printf("Wrong input! ");
        free(newnode);
        return;
    }

    display();
    make_copy(); // Save the new state
}

void deletenode() {
    int ch;
    printf("1 - Delete from start 2 - Delete from end: ");
    scanf("%d", &ch);
    getchar();

    if (ch == 1) {
        if (head == NULL) {
            printf("List is empty!\n");
            return;
        }
        storage_node *temp = head;
        head = head->next;
        if (head == NULL) {
            tail = NULL;
        }
        free(temp);
    } else if (ch == 2) {
        if (head == NULL) {
            printf("List is empty!\n");
            return;
        }
        if (head->next == NULL) {
            free(head);
            head = tail = NULL;
        } else {
            storage_node *current = head;
            while (current->next->next != NULL) {
                current = current->next;
            }
            free(tail);
            tail = current;
            tail->next = NULL;
        }
    } else {
        printf("Wrong choice!\n");
        return;
    }

    display();
    make_copy(); // Save the new state
}

void update_line() {
    int position;
    char newtext[100];
    storage_node *current = head;

    printf("Node number: ");
    scanf("%d", &position);
    getchar();

    printf("Enter the new text here: ");
    fgets(newtext, sizeof(newtext), stdin);

    int len = strlen(newtext);
    if (len > 0 && newtext[len - 1] == '\n') {
        newtext[len - 1] = '\0';
    }
    for (int i = 1; i < position && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Position out of range!\n");
        return;
    }
    strcpy(current->data, newtext);
    strcat(current->data, "\n");

    display();
    make_copy(); // Save the new state
}

void update_content_in_line() {
    int position;
    char oldword[50], newword[50];
    storage_node *current = head;

    printf("Node number: ");
    scanf("%d", &position);
    getchar();

    printf("Enter the word you want to replace: ");
    scanf("%s", oldword);
    getchar();
    
    printf("Enter the new word: ");
    scanf("%s", newword);
    getchar();

    for (int i = 1; i < position && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Position out of range!\n");
        return;
    }

    char *pos = strstr(current->data, oldword);
    if (pos != NULL) {
        char newstr[100];
        int orgnl = pos - current->data;
        int old_len = strlen(oldword);
        int new_len = strlen(newword);

        strncpy(newstr, current->data, orgnl);
        newstr[orgnl] = '\0';
        strcat(newstr, newword);
        strcat(newstr, pos + old_len);

        strcpy(current->data, newstr);
    } else {
        printf("Word not found.\n");
    }

    display();
    make_copy(); // Save the new state
}

int main(int argc, char *argv[]) {
    FILE *fp;
    if (argc == 1) {
        fp = fopen("file.txt", "w+");
    } else if (argc == 2) {
        fp = fopen(argv[1], "r+");
        if (fp != NULL) {
            storing_data(fp);
        } else {
            fp = fopen(argv[1], "w+"); 
            printf("New file created successfully\n");
        }
    } else if (argc == 3) {
        char newfile[100];
        strcpy(newfile, argv[2]);
        strcat(newfile, argv[1]);
        fp = fopen(newfile, "w+");
    } else {
        printf("Wrong number/order of arguments!\n");
        exit(0);
    }

    if (fp == NULL) {
        printf("Failed to open file!\n");
        exit(0);
    }
    storing_data(fp);
    fclose(fp);

    clearall();
    
    int choice;
    do {
        menu();
        printf("Enter your choice here: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                createnode();
                break;
            case 2:
                deletenode();
                break;
            case 3:
                update_line();
                break;
            case 4:
                display();
                break;
            case 5:
                update_content_in_line();
                break;
            case 6:
                undo();
                break;
            case 7:
                redo();
                break;
            case 8:
                clearall();
                break;
            case 9:
                close_program();
                break;
            default:
                printf("Wrong input\n");
                break;
        }
    } while (choice != 9);
    return 0;
}
