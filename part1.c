#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "project.h"

int have_account = 0;

int main()
{
    show_login_menu();
    return 0;
}

//login ui
void show_login_menu()
{
    int choice;
    while (1) {
        printf("\n==================== Login  ====================\n");
        printf("1. Login\n");
        printf("2. Register (If you do not have an account)\n");
        printf("3. Exit\n");
        printf("===============================================\n");
        printf("Choose an option (1-3): ");
        //choose instruction
        if (scanf("%d", &choice) != 1) {
            printf("Invalid instruction\n");
            while (getchar() != '\n') {
                ;
            }
            continue;
        }
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                register_account();
                break;
            case 3:
                printf("Byebye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please enter a number between 1 and 3.\n");
        }
    }
}

void login()
{
    char account_name[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char filename[FILENAME_LENGTH];
    User user;
    printf("Please input your account name: ");
    scanf("%s", account_name);
    printf("and password: ");
    scanf("%s", password);
    if (!have_account) {
        printf("No user in this system, please register one.\n");
        return;
    }
    //check whether the user exists
    if (!user_exists(account_name)) {
        printf("Warning! Account name not found.\n");
        return;
    }
    //load account info
    snprintf(filename, sizeof(filename), "%s_user.dat", account_name);
    load_user_data(&user, filename);
    //check password
    if (strcmp(user.password, password) != 0) {
        printf("Warning! Incorrect password.\n");
        return;
    }
    //login successfully
    printf("Login successful. Welcome %s!\n", account_name);
    main_service_menu(account_name);
}

void register_account()
{
    char account_name[MAX_NAME_LENGTH];
    char password1[MAX_NAME_LENGTH];
    char password2[MAX_NAME_LENGTH];
    char filename[FILENAME_LENGTH];
    User new_user;
    FILE *file;
    //input account name
    printf("Enter account name: ");
    scanf("%s", account_name);
    //check whether account exists
    if (user_exists(account_name)) {
        printf("Account name already exists.\n");
        return;
    }
    //create password
    printf("Enter password: ");
    scanf("%s", password1);
    printf("Confirm password: ");
    scanf("%s", password2);
    //confirm password
    if (strcmp(password1, password2) != 0) {
        printf("Passwords do not match. Registration failed.\n");
        return;
    }
    //initialize the account
    strcpy(new_user.account_name, account_name);
    strcpy(new_user.password, password1);
    new_user.friend_count = 0;
    new_user.waiting_count = 0;
    //save account info
    snprintf(filename, sizeof(filename), "%s_user.dat", account_name);
    save_user_data(&new_user, filename);
    snprintf(filename, sizeof(filename), "%s.txt", account_name);
    file = fopen(filename, "w");
    if (file) fclose(file);
    printf("Registration successful! You can now login.\n");
    have_account = 1;
}

//check whether account exists
int user_exists(const char *account_name)
{
    char filename[FILENAME_LENGTH];
    FILE *file;
    //open the file named after acount name
    snprintf(filename, sizeof(filename), "%s_user.dat", account_name);
    file = fopen(filename, "rb");
    //if file exists
    if (file) {
        fclose(file);
        return 1;
    }
    //does not exist
    return 0;
}

//load account info to the user pointer
void load_user_data(User *user, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file) {
        fread(user, sizeof(User), 1, file);
        fclose(file);
    }
}

//save data
//rewrite the account info
void save_user_data(User *user, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file) {
        fwrite(user, sizeof(User), 1, file);
        fclose(file);
    }
}
