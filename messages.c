#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "chat_system.h"

//message ui
void manage_messages(char *current_user)
{
    int choice;
    while (1) {
        printf("\n=================== Manage Messages ===================\n");
        printf("1. Send a message\n");
        printf("2. Read messages\n");
        printf("3. Delete messages\n");
        printf("4. Back\n");
        printf("======================================================\n");
        printf("Choose an option (1-4): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid instruction\n");
            while (getchar() != '\n');
            continue;
        }
        switch (choice) {
            case 1:
                send_message(current_user);
                break;
            case 2:
                read_messages(current_user);
                break;
            case 3:
                delete_messages(current_user);
                break;
            case 4:
                return;
            default:
                printf("Invalid instruction\n");
        }
    }
}

void send_message(char *current_user)
{
    char filename[FILENAME_LENGTH];
    User current_user_data;
    char message[MAX_MESSAGE_LENGTH];
    int choices[50];
    int choice_count = 0;
    int i;
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
    printf("%d. All\n", current_user_data.friend_count + 1);
    printf("%d. Back\n", current_user_data.friend_count + 2);
    printf("Enter friend numbers (separated by space), press Enter to finish: ");
    char input[256];
    getchar();
    fgets(input, sizeof(input), stdin);
    //seperate your input
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
    //if go back
    for (i = 0; i < choice_count; i++) {
        if (choices[i] == current_user_data.friend_count + 2) {
            return;
        }
    }
    //input message
    printf("Enter message (max 255 chars), press Enter to finish: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;
    //check whether send to all
    int send_to_all = 0;
    for (i = 0; i < choice_count; i++) {
        if (choices[i] == current_user_data.friend_count + 1) {
            send_to_all = 1;
            break;
        }
    }
    if (send_to_all) {
        //send message to every one
        for (i = 0; i < current_user_data.friend_count; i++) {
            send_single_message(current_user, current_user_data.friends[i], message);
        }
        printf("Message sent to All\n");
    } else {
        //send message to the chosen friends
        for (i = 0; i < choice_count; i++) {
            int choice = choices[i] - 1;
            if (choice >= 0 && choice < current_user_data.friend_count) {
                send_single_message(current_user, current_user_data.friends[choice], message);
                printf("Message sent to %s\n", current_user_data.friends[choice]);
            }
        }
    }
}

void send_single_message(char *sender, char *receiver, char *message)
{
    char filename[FILENAME_LENGTH];
    char time_str[20];
    FILE *file;
    //read current time
    get_current_time(time_str);
    //write message into the friend's file
    snprintf(filename, sizeof(filename), "%s.txt", receiver);
    file = fopen(filename, "a");
    if (file) {
        fprintf(file, "Time: %s", time_str);
        fprintf(file, "\nSender: %s", sender);
        fprintf(file, "\nContent: %s",message);
        fprintf(file, "\nunread status\n");
        fprintf(file, "------------------------\n");
        fclose(file);
    }
}

void read_messages(char *current_user)
{
    char filename[FILENAME_LENGTH];
    FILE *file;
    char line[256];
    int choice;
    if (choice == 3) return;
    snprintf(filename, sizeof(filename), "%s.txt", current_user);
    file = fopen(filename, "r");
    //check whether have email
    if (!file) {
        printf("No messages found.\n");
        return;
    }
    if (!fgets(line, sizeof(line), file)) {
        printf("No messages found.\n");
        return;
    }
    fseek(file, 0, SEEK_SET);
    //ui
    printf("\n1. Read all messages\n");
    printf("2. Read unread messages only\n");
    printf("3. Back\n");
    printf("Choose an option (1-3): ");
    //check instruction
    if (scanf("%d", &choice) != 1) {
        printf("Invalid instruction\n");
        return;
    }
    //back
    //read emails
    if (choice == 1 || choice == 2) {
        mark_messages_as_read(current_user, choice);
    }
    fclose(file);
}

void mark_messages_as_read(char *current_user, int choice)
{
    char filename[FILENAME_LENGTH];
    char temp_filename[FILENAME_LENGTH];
    FILE *file, *temp_file;
    char line[256];
    char buffer[MAX_MESSAGE_LENGTH] = "\0";
    int unread = 0, have_message = 0;
    snprintf(filename, sizeof(filename), "%s.txt", current_user);
    snprintf(temp_filename, sizeof(temp_filename), "temp_%s.txt", current_user);
    file = fopen(filename, "r");
    if (!file) return;
    temp_file = fopen(temp_filename, "w");
    if (!temp_file) {
        fclose(file);
        return;
    }
    //load single message into the buffer
    while (fgets(line, sizeof(line), file)) {
        strcat(buffer, line);
        //check status
        if (strstr(line, "unread status")) {
            //rewrite status
            fputs("already read\n", temp_file);
            unread = 1;
        } else {
            fputs(line, temp_file);
        }
        //check whether the end of the message
        if (strstr(line, "------------------------")) {
            //print unread message
            if (choice == 2 && unread) {
                have_message = 1;
                printf("%s", buffer);
            //print all messages
            } else if (choice == 1) {
                have_message = 1;
                printf("%s", buffer);
            }
            unread = 0;
            buffer[0] = 0;
        }
    }
    if(!have_message) {
        printf("No messages found.\n");
    }
    fclose(file);
    fclose(temp_file);
    //renew the file
    remove(filename);
    rename(temp_filename, filename);
}

void delete_messages(char *current_user)
{
    char filename[FILENAME_LENGTH];
    char temp_filename[FILENAME_LENGTH];
    char start_date[11], end_date[11], account_name[MAX_NAME_LENGTH];
    FILE *file, *temp_file;
    char line[256];
    char current_time[20], current_sender[MAX_NAME_LENGTH];
    int in_message = 0, should_delete = 0, cnt = 0;
    snprintf(filename, sizeof(filename), "%s.txt", current_user);
    file = fopen(filename, "r");
    if (!file) {
        printf("No messages found.\n");
        return;
    }
    if (!fgets(line, sizeof(line), file)) {
        printf("No messages found.\n");
        return;
    }
    fseek(file, 0, SEEK_SET);
    //input date
    printf("Start date (dd/mm/yyyy): ");
    scanf("%s", start_date);
    printf("End date (dd/mm/yyyy): ");
    scanf("%s", end_date);
    printf("Account name (or 'all'): ");
    scanf("%s", account_name);
    snprintf(temp_filename, sizeof(temp_filename), "temp_%s.txt", current_user);
    //check whether you have email
    //failed to create new file
    temp_file = fopen(temp_filename, "w");
    if (!temp_file) {
        fclose(file);
        printf("No messages found.\n");
        return;
    }
    //delete emails
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Time: ")) {
            in_message = 1;
            should_delete = 0;
            sscanf(line, "Time: %10s", current_time);
            if (fgets(line, sizeof(line), file)) {
                if (strstr(line, "Sender: ")) {
                    sscanf(line, "Sender: %s", current_sender);
                    if (strcmp(account_name, "all") == 0
                        || strcmp(account_name, current_sender) == 0) {
                        //check whether the time is in the input period
                        if (compare_dates(current_time, start_date) >= 0
                            && compare_dates(current_time, end_date) <= 0) {
                            should_delete = 1;
                        }
                    }
                }
            }
            //write other messages into the new file
            if (!should_delete) {
                fputs("Time: ", temp_file);
                fputs(current_time, temp_file);
                fputs("\n", temp_file);
                fputs("Sender: ", temp_file);
                fputs(current_sender, temp_file);
                fputs("\n", temp_file);
            } else {
                cnt++;
            }
        } else if (strstr(line, "------------------------")) {
            in_message = 0;
            if (!should_delete) {
                fputs("------------------------\n", temp_file);
            }
        } else {
            if (!should_delete && in_message) {
                fputs(line, temp_file);
            }
        }
    }
    if (strcmp(account_name, "all") == 0) {
        printf("Removed %d message(s) from all in period %s - %s.\n",
        cnt, start_date, end_date);
    } else {
        printf("Removed %d message(s) from %s in period %s - %s.\n",
        cnt, account_name, start_date, end_date);
    }
    fclose(file);
    fclose(temp_file);
    //store new data
    remove(filename);
    rename(temp_filename, filename);
}
