#include "tpb.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "linked_list.h"
#define FRAME_NUMBER 10

void free_scoreboard(LinkedList *scoreboard) {
    Node *current;
    
    if (scoreboard == NULL) {
        return;
    }
    current = scoreboard->head;
    
    while (current != NULL) {
        void *temp = current->data;
        current = current->next;
        free(temp);
    }
    
    free_linked_list(scoreboard);
}

LinkedList *bowling_score_parser(const char *game_characters, int *err_position) {
    LinkedList *rolls_list;
    LinkedList *scoreboard;
    int rolls_array[21];
    int i;
    int j;
    int current_roll_score;
    int cumulative_score;
    int frame_count;
    Node *current;
    Node *current_roll;
    int frame_score;
    int first_roll;
    int second_roll;
    int *frame_total;
    
    j = 0;
    cumulative_score = 0;
    frame_count = 0;
    
    for (i = 0; i < 21; i++) {
        rolls_array[i] = 0;
    }
    
    if (game_characters == NULL || strlen(game_characters) == 0) {
        if (err_position != NULL) {
            *err_position = 0;
        }
        return NULL;
    }

    rolls_list = initialise_linked_list();
    scoreboard = initialise_linked_list();
    
    for (i = 0; game_characters[i] != '\0'; i++) {
        if (j >= 21) { 
            break;
        }

        if (game_characters[i] >= '1' && game_characters[i] <= '9') {
            current_roll_score = game_characters[i] - '0';
        } 
        
        else if (game_characters[i] == 'X' || game_characters[i] == 'x') {
            current_roll_score = 10;
        } 
        
        else if (game_characters[i] == '/') {
            if (j > 0) {
                current_roll_score = 10 - rolls_array[j - 1];
            } 
            
            else {
                current_roll_score = 10;
            }
        } 
        
        else if (game_characters[i] == '-' || game_characters[i] == 'f' || 
                 game_characters[i] == 'F') {
            current_roll_score = 0;
        }
        
        else {
            continue; 
        }
        
        rolls_array[j] = current_roll_score;
        j++;
    }

    for (i = 0; i < j; i++) {
        int *roll_value;
        roll_value = (int *)malloc(sizeof(int));
        if (roll_value == NULL) {
            free_scoreboard(scoreboard);
            current = rolls_list->head;
        
            while (current != NULL) {
                free(current->data);
                current = current->next;
            }
            free_linked_list(rolls_list);
            return NULL;
        }
        *roll_value = rolls_array[i];
        append_linked_list(rolls_list, roll_value);
    }

    current_roll = rolls_list->head;
    while (current_roll != NULL && frame_count < FRAME_NUMBER) {
        frame_score = 0;

        if (*(int*)current_roll->data == 10) {
            frame_score = 10;
        
            if (current_roll->next != NULL) {
                frame_score += *(int*)current_roll->next->data;
        
                if (current_roll->next->next != NULL) {
                    frame_score += *(int*)current_roll->next->next->data;
                }
            }
            current_roll = current_roll->next;
        }
    
       else if (current_roll->next != NULL) {
    first_roll = *(int*)current_roll->data;
    second_roll = *(int*)current_roll->next->data;
    
    if (first_roll + second_roll == 10) { 
        frame_score = 10;
        if (current_roll->next->next != NULL) {
            frame_score += *(int*)current_roll->next->next->data;
        }
    } 
    
    else {
        frame_score = first_roll + second_roll;
    }
    current_roll = current_roll->next->next;
} 

else if (current_roll != NULL) {
    frame_score = *(int*)current_roll->data;
    current_roll = current_roll->next;
}

else {
    break;
}
        cumulative_score += frame_score;
        frame_total = (int *)malloc(sizeof(int));
        if (frame_total == NULL) {
            free_scoreboard(scoreboard);
            current = rolls_list->head;
            
            while (current != NULL) {
                free(current->data);
                current = current->next;
            }
            free_linked_list(rolls_list);
            return NULL;
        }
        *frame_total = cumulative_score;
        append_linked_list(scoreboard, frame_total);
        frame_count++;
    }

    current = rolls_list->head;
    while (current != NULL) {
        free(current->data);
        current = current->next;
    }
    free_linked_list(rolls_list);

    printf("%s: ", game_characters);

    return scoreboard;
}

void print_scoreboard(LinkedList *scoreboard) {
    Node *current = scoreboard->head;

    if (current == NULL) {
        return;
    }

    while (current != NULL) {
        printf("%d", *(int *)current->data);
        current = current->next;
        
        if (current != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}
