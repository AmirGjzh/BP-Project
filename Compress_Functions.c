#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree {
    int data;
    int freq;
    struct tree *left;
    struct tree *right;
} Tree;

typedef struct queue {
    Tree *main;
    struct queue *next;
} Queue;

typedef struct hf_code {
    char code[100];
    int data;
    struct hf_code *next;
} Hf_code;

typedef enum{
    RED = 4, WHITE = 7, BLUE = 2, GREEN = 10, LIGHT_BLUE = 3
} ConsoleColors;


void SetColor(ConsoleColors col) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    WORD color = (bufferInfo.wAttributes & 0xF0) + (col & 0x0F);
    SetConsoleTextAttribute(consoleHandle, color);
}


Queue * add_to_queue(Queue *head1, int w) {
    Queue *copy_head = head1;
    while (head1 != NULL) {
        if (head1->main->data == w) {
            head1->main->freq++;
            return copy_head;
        }
        head1 = head1->next;
    }

    Queue *new = (Queue *) malloc(sizeof(Queue));
    //--- Error message!
    if (new == NULL) {
        SetColor(RED);
        printf("%s", "No memory to allocate!");
        SetColor(WHITE);
        return head1;
    }
    new->main = (Tree *) malloc(sizeof(Tree));
    //--- Error message!
    if (new->main == NULL) {
        SetColor(RED);
        printf("%s", "No memory to allocate!");
        SetColor(WHITE);
        return head1;
    }

    new->main->left = NULL;
    new->main->right = NULL;
    new->main->data = w;
    new->main->freq = 1;
    new->next = copy_head;
    copy_head = new;

    return copy_head;
}


Queue * delete_queue(Queue *head2) {
    Queue *temp = head2->next;
    free(head2);
    return temp;
}


void sort_queue(Queue *head1) {
    while (head1 != NULL) {
        Queue *new = head1;
        while (new != NULL) {

            if (new->main->freq < head1->main->freq) {
                Tree *hold = new->main;
                new->main = head1->main;
                head1->main = hold;
            }
            new = new->next;
        }
        head1 = head1->next;
    }
}


Queue * build_huffman_tree(Queue *head1) {
    if (head1->next == NULL) {
        return head1;
    }
    Queue *new = (Queue *) malloc(sizeof(Queue));
    //--- Error message!
    if (new == NULL) {
        SetColor(RED);
        printf("%s", "No memory to allocate!");
        SetColor(WHITE);
        return head1;
    }
    new->main = (Tree *) malloc(sizeof(Tree));
    //--- Error message!
    if (new->main == NULL) {
        SetColor(RED);
        printf("%s", "No memory to allocate!");
        SetColor(WHITE);
        return head1;
    }

    new->main->left = head1->main;
    new->main->right = head1->next->main;
    new->main->data = -1;
    new->main->freq = new->main->left->freq + new->main->right->freq;

    head1 = delete_queue(head1);
    head1 = delete_queue(head1);

    new->next = head1;
    head1 = new;

    sort_queue(head1);

    build_huffman_tree(head1);
}


void show_huffman_codes(Tree *main, char *code, int index, FILE *file) {
    if (main->left == NULL && main->right == NULL) {
        fprintf(file, "%s %d\n", code, main->data);
        return;
    }
    code[index] = '1'; code[index + 1] = '\0';
    show_huffman_codes(main->right, code, index + 1, file);
    code[index] = '0'; code[index + 1] = '\0';
    show_huffman_codes(main->left, code, index + 1, file);
}


Hf_code * add_to_hf_queue(Hf_code *node1, char *code, int val) {
    Hf_code *new = (Hf_code *) malloc(sizeof(Hf_code));
    //--- Error message!
    if (new == NULL) {
        SetColor(RED);
        printf("%s", "No memory to allocate!");
        SetColor(WHITE);
        return node1;
    }

    strcpy(new->code, code);
    new->data = val;
    new->next = node1;
    node1 = new;

    return node1;
}


char * search_hf_queue(Hf_code *node1, int val) {
    while (1) {
        if (node1->data == val) {
            return node1->code;
        }
        node1 = node1->next;
    }
}


void free_tree(Tree *main1) {
    if (main1 == NULL) {
        return;
    }
    free_tree(main1->right);
    free_tree(main1->left);

    free(main1);
}


void free_queue(Queue *head1) {
    Queue *temp;
    while (head1 != NULL) {
        temp = head1->next;
        free_tree(head1->main);
        free(head1);
        head1 = temp;
    }
}


void free_hf_queue(Hf_code *node1) {
    Hf_code *temp;
    while (node1 != NULL) {
        temp = node1->next;
        free(node1);
        node1 = temp;
    }
}


void binary(int n, FILE *file) {
    for (int i = 64; i >= 1; i /= 2) {
        fprintf(file, "%d", n / i);
        n %= i;
    }
}


Tree * add_to_tree(Tree *main, char *s, int data) {
    Tree *copy = main;

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '1') {
            if (copy->right != NULL) {
                copy = copy->right;
            }
            else {
                copy->right = (Tree *) malloc(sizeof(Tree));
                //--- Error message!
                if (copy->right == NULL) {
                    SetColor(RED);
                    printf("%s", "No memory to allocate!");
                    SetColor(WHITE);
                }

                copy->right->right = NULL;
                copy->right->left = NULL;
                copy = copy->right;
            }
        } 
        else {
            if (copy->left != NULL) {
                copy = copy->left;
            }
            else {
                copy->left = (Tree *) malloc(sizeof(Tree));
                //--- Error message!
                if (copy->left == NULL) {
                    SetColor(RED);
                    printf("%s", "No memory to allocate!");
                    SetColor(WHITE);
                }

                copy->left->right = NULL;
                copy->left->left = NULL;
                copy = copy->left;
            }
        }
    }
    copy->data = data;

    return main;
}