#ifndef __PROJECT__
#define __PROJECT__

#define MAX_ACCOUNTS 100
#define MAX_FRIENDS 50
#define MAX_MESSAGE_LENGTH 255
#define MAX_WAITING 50
#define FILENAME_LENGTH 50
#define MAX_NAME_LENGTH 30

typedef struct {
    char account_name[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char friends[MAX_FRIENDS][MAX_NAME_LENGTH];
    char waiting_list[MAX_WAITING][MAX_NAME_LENGTH];
    int friend_count;
    int waiting_count;
} User;

void show_login_menu();
void login();
void register_account();
void main_service_menu(char *current_user);
void manage_friends(char *current_user);
void manage_messages(char *current_user);
void add_friends(char *current_user);
void accept_friends(char *current_user);
void delete_friends(char *current_user);
void show_friends(char *current_user);
void send_message(char *current_user);
void read_messages(char *current_user);
void delete_messages(char *current_user);
void load_user_data(User *user, const char *filename);
void save_user_data(User *user, const char *filename);
int user_exists(const char *account_name);
void get_current_time(char *time_str);
void send_single_message(char *sender, char *receiver, char *message);
void mark_messages_as_read(char *current_user, int choice);
int compare_dates(char *date1, char *date2);

#endif
