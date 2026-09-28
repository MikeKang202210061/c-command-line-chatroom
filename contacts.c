#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "chat_system.h"

//main service ui
void main_service_menu(char *current_user)
{
    int choice;
    while (1) {
        printf("\n=================== Main Service ===================\n");
        printf("1. Manage friends\n");
        printf("2. Manage messages\n");
        printf("3. Back\n");
        printf("======================================================\n");
        printf("Choose an option (1-3): ");
        //check instruction
        if (scanf("%d", &choice) != 1) {
            printf("Invalid instruction\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                manage_friends(current_user);
                break;
            case 2:
                manage_messages(current_user);
                break;
            case 3:
                return;
            default:
                printf("Invalid instruction\n");
        }
    }
}

//manage friend ui
void manage_friends(char *current_user)
{
    int choice;
    while (1) {
        printf("\n=================== Manage Friends ===================\n");
        printf("1. Add friends\n");
        printf("2. Accept friends\n");
        printf("3. Delete friends\n");
        printf("4. Show current friends\n");
        printf("5. Back\n");
        printf("======================================================\n");
        printf("Choose an option (1-5): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid instruction\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                add_friends(current_user);
                break;
            case 2:
                accept_friends(current_user);
                break;
            case 3:
                delete_friends(current_user);
                break;
            case 4:
                show_friends(current_user);
                break;
            case 5:
                return;
            default:
                printf("Invalid instruction\n");
        }
    }
}

//add friend
void add_friends(char *current_user)
{
    char input[256];
    char friend_name[MAX_NAME_LENGTH];
    char filename[FILENAME_LENGTH];
    User current_user_data, friend_user_data;
    int i, failure = 0;
    //load current user info
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    load_user_data(&current_user_data, filename);
    //input account name
    printf("Enter usernames to add (in one line separated by space): ");
    getchar();
    fgets(input, sizeof(input), stdin);
    //seperate the input
    char *token = strtok(input, " \n");
    while (token != NULL) {
        failure = 0;
        //add token into names
        strcpy(friend_name, token);
        //check account name
        if (strcmp(friend_name, current_user) == 0) {
            printf("You cannot send a friend request to yourself.\n");
            failure = 1;
        }
        if (!user_exists(friend_name)) {
            printf("Account %s does not exist.\n", friend_name);
            failure = 1;
        }
        int is_friend = 0;
        for (i = 0; i < current_user_data.friend_count; i++) {
            if (strcmp(current_user_data.friends[i], friend_name) == 0) {
                printf("%s has already been your friend.\n", friend_name);
                failure = 1;
            }
        }
        for (i = 0; i < current_user_data.waiting_count; i++) {
            if (strcmp(current_user_data.waiting_list[i], friend_name)
                == 0) {
                printf
                ("%s has sent friend request to you.\n",
                friend_name);
                failure = 1;
            }
        }
        if (failure) {
            token = strtok(NULL, " \n");
            continue;
        }
        snprintf
        (filename, sizeof(filename), "%s_user.dat", friend_name);
        if (user_exists(friend_name)) {
            load_user_data(&friend_user_data, filename);
            //check whether you can add the account
            for (i = 0; i < friend_user_data.waiting_count; i++) {
                 if (strcmp
                 (friend_user_data.waiting_list[i],
                 current_user)
                 == 0) {
                 printf
                 ("Friend request to %s is already pending.\n",
                 friend_name);
                 failure = 1;
                 }
            }
            if (failure) {
                token = strtok(NULL, " \n");
                continue;
            }
            //add current user into the waiting list of the account
            strcpy
            (friend_user_data.waiting_list[friend_user_data.waiting_count],
            current_user);
            friend_user_data.waiting_count++;
            save_user_data(&friend_user_data, filename);
            printf("Friend request sent to %s.\n", friend_name);
        }
        token = strtok(NULL, " \n");
    }
}

void accept_friends(char *current_user)
{
    char filename[FILENAME_LENGTH];
    User current_user_data, friend_user_data;
    int choices[50];
    int choice_count = 0;
    int i, j;
    //load current user info
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    load_user_data(&current_user_data, filename);
    //check len of waiting list
    if (current_user_data.waiting_count == 0) {
        printf("No pending friend requests for %s.\n", current_user);
        return;
    }
    //print the list
    printf("\nPending friend requests for %s:\n", current_user);
    for (i = 0; i < current_user_data.waiting_count; i++) {
        printf("%d. %s\n", i + 1, current_user_data.waiting_list[i]);
    }
    printf("%d. All\n", current_user_data.waiting_count + 1);
    printf("%d. Back\n", current_user_data.waiting_count + 2);
    printf("Enter indices (space separated), press Enter to finish: ");
    char input[256];
    getchar();
    fgets(input, sizeof(input), stdin);
    //seperate the input
    char *token = strtok(input, " \n");
    //store the chosen friends
    while (token != NULL && choice_count < 50) {
        int choice = atoi(token);
        if (choice >= 1 && choice <= current_user_data.waiting_count + 2) {
            choices[choice_count++] = choice;
        }
        //read next choice
        token = strtok(NULL, " \n");
    }
    int accept_all = 0;
    for (i = 0; i < choice_count; i++) {
        if (choices[i] == current_user_data.waiting_count + 1) {
            accept_all = 1;
        } else if (choices[i] == current_user_data.waiting_count + 2) {
            return;
        }
    }
    //choose all
    if (accept_all) {
        for (i = 0; i < current_user_data.waiting_count; i++) {
            char *friend_name = current_user_data.waiting_list[i];
            strcpy(current_user_data.friends[current_user_data.friend_count], friend_name);
            current_user_data.friend_count++;
            //add you into these friends' lists
            snprintf(filename, sizeof(filename), "%s_user.dat", friend_name);
            load_user_data(&friend_user_data, filename);
            strcpy(friend_user_data.friends[friend_user_data.friend_count], current_user);
            friend_user_data.friend_count++;
            save_user_data(&friend_user_data, filename);
        }
        //no account in your waiting list
        current_user_data.waiting_count = 0;
        printf("Friend requests updated for all.\n");
    } else {
        char accepted_friends[MAX_WAITING][MAX_NAME_LENGTH];
        int accepted_count = 0;
        //store the account to accepted friends list
        for (i = 0; i < choice_count; i++) {
            int choice = choices[i] - 1;
            if (choice >= 0 && choice < current_user_data.waiting_count) {
                strcpy(accepted_friends[accepted_count], current_user_data.waiting_list[choice]);
                accepted_count++;
            }
        }
        //add accounts into your friends list
        for (i = 0; i < accepted_count; i++) {
            char *friend_name = accepted_friends[i];
            strcpy(current_user_data.friends[current_user_data.friend_count], friend_name);
            current_user_data.friend_count++;
            snprintf(filename, sizeof(filename), "%s_user.dat", friend_name);
            load_user_data(&friend_user_data, filename);
            strcpy(friend_user_data.friends[friend_user_data.friend_count], current_user);
            friend_user_data.friend_count++;
            save_user_data(&friend_user_data, filename);
            printf("Friend requests updated for %s.\n", friend_name);
            //delete these accounts in your waiting list
            for (j = 0; j < current_user_data.waiting_count; j++) {
                if (strcmp(current_user_data.waiting_list[j], friend_name) == 0) {
                    for (int k = j; k < current_user_data.waiting_count - 1; k++) {
                        strcpy(current_user_data.waiting_list[k], current_user_data.waiting_list[k + 1]);
                    }
                    current_user_data.waiting_count--;
                    break;
                }
            }
        }
    }
    //store your data
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    save_user_data(&current_user_data, filename);
}

//print your friends
void show_friends(char *current_user)
{
    char filename[FILENAME_LENGTH];
    User current_user_data;
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    load_user_data(&current_user_data, filename);
    if (current_user_data.friend_count == 0) {
        printf("You have no friends.\n");
        return;
    }
    printf("\nYour friends:\n");
    for (int i = 0; i < current_user_data.friend_count; i++) {
        printf("%d. %s\n", i + 1, current_user_data.friends[i]);
    }
}

void delete_friends(char *current_user)
{
    char filename[FILENAME_LENGTH];
    User current_user_data, friend_user_data;
    int choices[50];
    int choice_count = 0;
    int i, j, k;
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    load_user_data(&current_user_data, filename);
    //check whether you have friend
    if (current_user_data.friend_count == 0) {
        printf("You have no friends.\n");
        return;
    }
    //print friends list
    printf("\nYour friends:\n");
    for (i = 0; i < current_user_data.friend_count; i++) {
        printf("%d. %s\n", i + 1, current_user_data.friends[i]);
    }
    printf("%d. ALL\n", current_user_data.friend_count + 1);
    printf("%d. Back\n", current_user_data.friend_count + 2);
    printf("Enter friend numbers (separated by space), press Enter to finish: ");
    char input[256];
    getchar();
    fgets(input, sizeof(input), stdin);
    //seperate your input number
    char *token = strtok(input, " \n");
    while (token != NULL && choice_count < 50) {
        int choice = atoi(token);
        //store your choice
        if (choice >= 1 && choice <= current_user_data.friend_count + 2) {
            choices[choice_count++] = choice;
        }
        //read next
        token = strtok(NULL, " \n");
    }
    int delete_all = 0;
    for (i = 0; i < choice_count; i++) {
        if (choices[i] == current_user_data.friend_count + 1) {
            delete_all = 1;
        } else if (choices[i] == current_user_data.friend_count + 2) {
            return;
        }
    }
    //delete all friends
    if (delete_all) {
        for (i = 0; i < current_user_data.friend_count; i++) {
            char *friend_name = current_user_data.friends[i];
            snprintf(filename, sizeof(filename), "%s_user.dat", friend_name);
            if (user_exists(friend_name)) {
                load_user_data(&friend_user_data, filename);
                //renew your friends' friends list
                for (j = 0; j < friend_user_data.friend_count; j++) {
                    if (strcmp(friend_user_data.friends[j], current_user) == 0) {
                        for (k = j; k < friend_user_data.friend_count - 1; k++) {
                            strcpy(friend_user_data.friends[k], friend_user_data.friends[k + 1]);
                        }
                        friend_user_data.friend_count--;
                        break;
                    }
                }
                //save data
                save_user_data(&friend_user_data, filename);
            }
        }
        current_user_data.friend_count = 0;
        printf("Deleting all...");
    //delete some friends
    } else {
        char deleted_friends[MAX_FRIENDS][MAX_NAME_LENGTH];
        int deleted_count = 0;
        //store deleted friends
        for (i = 0; i < choice_count; i++) {
            int choice = choices[i] - 1;
            if (choice >= 0 && choice < current_user_data.friend_count) {
                strcpy(deleted_friends[deleted_count], current_user_data.friends[choice]);
                deleted_count++;
            }
        }
        //renew your friends' friends list
        for (i = 0; i < deleted_count; i++) {
            char *friend_name = deleted_friends[i];
            snprintf(filename, sizeof(filename), "%s_user.dat", friend_name);
            if (user_exists(friend_name)) {
                load_user_data(&friend_user_data, filename);
                for (j = 0; j < friend_user_data.friend_count; j++) {
                    if (strcmp(friend_user_data.friends[j], current_user) == 0) {
                        for (k = j; k < friend_user_data.friend_count - 1; k++) {
                            strcpy(friend_user_data.friends[k], friend_user_data.friends[k + 1]);
                        }
                        friend_user_data.friend_count--;
                        break;
                    }
                }
                save_user_data(&friend_user_data, filename);
            }
            //renew your friends list
            for (j = 0; j < current_user_data.friend_count; j++) {
                if (strcmp(current_user_data.friends[j], friend_name) == 0) {
                    for (k = j; k < current_user_data.friend_count - 1; k++) {
                        strcpy(current_user_data.friends[k], current_user_data.friends[k + 1]);
                    }
                    current_user_data.friend_count--;
                    break;
                }
            }
            printf("Deleting %s\n", friend_name);
        }
    }
    //save data
    snprintf(filename, sizeof(filename), "%s_user.dat", current_user);
    save_user_data(&current_user_data, filename);
    printf("Friend list updated.\n");
}
