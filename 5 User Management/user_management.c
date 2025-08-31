#include <stdio.h>
#include <string.h>

#ifdef _WIN32
    // For Windows-specific 
    #include <conio.h> 
#else
    // For UNIX/Linux/macOS 
    #include <unistd.h>
    #include <termios.h>
#endif

#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30

typedef struct {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();
void fix_fgets_input(char*);
void input_credentials(char* username, char* password);

int main() {
    int option;
    int user_index;
    while (1) {
        printf ("\nWelcome to User Management");
        printf ("\n1. Register");
        printf ("\n2. Login");
        printf ("\n3. Exit");
        printf ("\nSelect an option: ");
        scanf("%d", &option);
        getchar(); 

        switch (option)
        {
        case 1:
            register_user();
            break;
        case 2:
            user_index = login_user();
            if (user_index >= 0) {
                printf("\nLogin successful! Welcome, %s!\n",
                    users[user_index].username);
            } else {
                printf("\nLogin failed! Incorrect username or password.\n");
            }
            break;
        case 3:
            printf("\nExiting Program.\n");
            return 0;
            break;
        default:
            printf("\nInvalid option. Please try again.\n");
            break;
        }
    }
    return 0;
}

void register_user() {
    if (user_count == MAX_USERS) {
        printf("\nMaximum %d users are supported! No more registrations Allowed!!!!\n", MAX_USERS);
        return;
    }

    int new_index = user_count;
    printf("\nRegister a new user");
    input_credentials(users[new_index].username, users[new_index].password);
    user_count++;
    printf("\nRegistration successful!\n");
}

int login_user() {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];

    input_credentials(username, password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 && 
            strcmp(password, users[i].password) == 0) {
            return i;
        }
    }

    return -1;
}

void input_credentials(char* username, char* password) {
    printf("\nEnter username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_input(username);
    printf("Enter password (masking enabled): ");
    fflush(stdout);

    char ch;
    int i = 0;

// --- WINDOWS-SPECIFIC PASSWORD INPUT ---
#ifdef _WIN32
    while ((ch = _getch()) != '\r' && i < CREDENTIAL_LENGTH - 1) { 
        if (ch == '\b') { 
            if (i > 0) {
                i--;
                printf("\b \b"); 
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
// --- UNIX/LINUX/MACOS-SPECIFIC PASSWORD INPUT ---
#else
    struct termios old_props, new_props;
    tcgetattr(STDIN_FILENO, &old_props);
    new_props = old_props;
    new_props.c_lflag &= ~(ECHO | ICANON); 
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);

    while ((ch = getchar()) != '\n' && ch != EOF && i < CREDENTIAL_LENGTH - 1) {
        if (ch == '\b' || ch == 127) { 
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
#endif

    password[i] = '\0';
    printf("\n"); 
}

void fix_fgets_input(char* string) {
    int index = strcspn(string, "\n");
    string[index] = '\0';
}