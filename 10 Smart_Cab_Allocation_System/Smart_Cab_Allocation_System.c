#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_STRING_LEN 50
#define MAX_CABS 100
#define MAX_USERS 100


typedef struct {
    double x;
    double y;
} Location;

typedef struct {
    int cabId;
    Location location;
    int available; 
} Cab;

typedef struct {
    char username[MAX_STRING_LEN];
    char password[MAX_STRING_LEN];
    Cab cabs[MAX_CABS];
    int cab_count;
} Admin;

// Represents a User
typedef struct {
    char name[MAX_STRING_LEN];
    char password[MAX_STRING_LEN];
} User;

// Stores user credentials
typedef struct {
    char username[MAX_STRING_LEN];
    unsigned long hashedPassword;
} UserCredential;

// Manages all registered users
typedef struct {
    UserCredential users[MAX_USERS];
    int user_count;
} UserManager;

// Forward declaration for the main system struct
struct SmartCabAllocSystem;

// --- Function Prototypes ---
void clean_input_buffer();
unsigned long hash_password(const char* str);

// Cab functions
void cab_init(Cab* cab, int id, double x, double y);

// Admin functions
void admin_init(Admin* admin);
int admin_get_authorized(Admin* admin, const char* enteredName, const char* enteredPassword);
void admin_add_cab(Admin* admin, Cab cab);

// UserManager functions
void user_manager_init(UserManager* manager);
void user_manager_register(UserManager* manager, const char* username, const char* password);
int user_manager_authenticate(UserManager* manager, const char* username, const char* password);

// Main System functions (combines Auth and SmartCabAllocSystem)
void system_init(struct SmartCabAllocSystem* system);
void system_home(struct SmartCabAllocSystem* system);
void system_admin_route(struct SmartCabAllocSystem* system);
void system_user_route(struct SmartCabAllocSystem* system);
void system_login_util(struct SmartCabAllocSystem* system);
void system_register_util(struct SmartCabAllocSystem* system);
void system_admin_functionality(struct SmartCabAllocSystem* system);
void system_user_functionality(struct SmartCabAllocSystem* system);
void system_get_started(struct SmartCabAllocSystem* system);


typedef struct SmartCabAllocSystem {
    UserManager user_manager;
    Admin admin;
    User current_user;
    int is_admin_logged_in;
} SmartCabAllocSystem;


int main() {
    SmartCabAllocSystem system;
    system_get_started(&system);
    return 0;
}


void system_init(SmartCabAllocSystem* system) {
    user_manager_init(&system->user_manager);
    admin_init(&system->admin);
    strcpy(system->current_user.name, ""); 
    system->is_admin_logged_in = 0;
}

void system_get_started(SmartCabAllocSystem* system) {
    system_init(system); 
    while (1) {
        system_home(system);

        if (system->is_admin_logged_in) {
            system_admin_functionality(system);
        } else if (strlen(system->current_user.name) > 0) {
            system_user_functionality(system);
        } else {

        }
    }
}

void system_home(SmartCabAllocSystem* system) {
    int main_option;
    printf("Welcome to Smart Cab Allocation System\n");
    printf("Enter one from below:\n\t1. Admin login\n\t2. User login/register\n\t-1. Exit from the system\n:");
    scanf("%d", &main_option);
    clean_input_buffer();

    switch (main_option) {
        case 1:
            system_admin_route(system);
            break;
        case 2:
            system_user_route(system);
            break;
        case -1:
            printf("Exiting system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
}

void system_admin_route(SmartCabAllocSystem* system) {
    char username[MAX_STRING_LEN], password[MAX_STRING_LEN];
    printf("Admin please authenticate yourself first\n");
    printf("\nEnter your username:\t");
    scanf("%s", username);
    clean_input_buffer();
    printf("Enter your password:\t");
    scanf("%s", password);
    clean_input_buffer();

    if (admin_get_authorized(&system->admin, username, password)) {
        printf("\nWelcome Admin!\n");
        system->is_admin_logged_in = 1;
    } else {
        printf("\nAdmin login failed. Please retry.\n");
        system->is_admin_logged_in = 0;
    }
}

void system_user_route(SmartCabAllocSystem* system) {
    int option = 0;
    while (option != -1) {
        printf("\nPlease choose an option:\n\t1. Register\n\t2. Login\n\t-1. Back to main menu\n:");
        scanf("%d", &option);
        clean_input_buffer();

        if (option == 1) {
            system_register_util(system);
        } else if (option == 2) {
            system_login_util(system);
            if (strlen(system->current_user.name) > 0) {
                break; // Exit loop on successful login
            }
        } else if (option == -1) {
            printf("\nReturning to the main menu...\n");
        } else {
            printf("Invalid option. Please try again.\n");
        }
    }
}

void system_login_util(SmartCabAllocSystem* system) {
    char username[MAX_STRING_LEN], password[MAX_STRING_LEN];
    printf("\nEnter your username:\t");
    scanf("%s", username);
    clean_input_buffer();
    printf("Enter your password:\t");
    scanf("%s", password);
    clean_input_buffer();

    if (user_manager_authenticate(&system->user_manager, username, password)) {
        printf("\nAuthentication successful!\n");
        strcpy(system->current_user.name, username);
    } else {
        printf("\nAuthentication failed. Please register first or check your credentials.\n");
        strcpy(system->current_user.name, "");
    }
}

void system_register_util(SmartCabAllocSystem* system) {
    char username[MAX_STRING_LEN], password[MAX_STRING_LEN];
    printf("\nEnter a username:\t");
    scanf("%s", username);
    clean_input_buffer();
    printf("Enter a password:\t");
    scanf("%s", password);
    clean_input_buffer();
    user_manager_register(&system->user_manager, username, password);
    printf("Now login to access our features.\n");
}


void system_admin_functionality(SmartCabAllocSystem* system) {
    int option = 0;
    while (option != -1) {
        printf("\nAdmin Menu:\n\t1. Add Cab\n\t2. Get all cabs details\n\t-1. Logout\n:");
        scanf("%d", &option);
        clean_input_buffer();

        if (option == 1) {
            int id;
            double x, y;
            printf("Enter the cab id:\t");
            scanf("%d", &id);
            clean_input_buffer();
            printf("Enter the cab location (x y):\t");
            scanf("%lf %lf", &x, &y);
            clean_input_buffer();
            
            Cab new_cab;
            cab_init(&new_cab, id, x, y);
            admin_add_cab(&system->admin, new_cab);
            printf("Cab added successfully!\n");
        } else if (option == 2) {
            if (system->admin.cab_count == 0) {
                printf("No cabs in the system yet.\n");
            } else {
                printf("List of all cabs:\n");
                for (int i = 0; i < system->admin.cab_count; i++) {
                    Cab cab = system->admin.cabs[i];
                    printf("Cab ID: %d, Location: (%.2f, %.2f), Available: %s\n",
                           cab.cabId, cab.location.x, cab.location.y, cab.available ? "Yes" : "No");
                }
            }
        } else if (option == -1) {
            system->is_admin_logged_in = 0;
            printf("Admin logged out.\n");
        } else {
            printf("Invalid option.\n");
        }
    }
}

void system_user_functionality(SmartCabAllocSystem* system) {
     printf("\nWelcome %s! Hope you like our services.\n", system->current_user.name);
     printf("(User functionality is not yet implemented.)\n");
     printf("Logging out...\n\n");
     strcpy(system->current_user.name, ""); 
}


void cab_init(Cab* cab, int id, double x, double y) {
    cab->cabId = id;
    cab->location.x = x;
    cab->location.y = y;
    cab->available = 1;
}

void admin_init(Admin* admin) {
    strcpy(admin->username, "admin"); // adimin username
    strcpy(admin->password, "admin123"); // admin password
    admin->cab_count = 0;
}

int admin_get_authorized(Admin* admin, const char* enteredName, const char* enteredPassword) {
    return strcmp(admin->username, enteredName) == 0 && strcmp(admin->password, enteredPassword) == 0;
}

void admin_add_cab(Admin* admin, Cab cab) {
    if (admin->cab_count < MAX_CABS) {
        admin->cabs[admin->cab_count++] = cab;
    } else {
        printf("Cannot add more cabs. System is full.\n");
    }
}

void user_manager_init(UserManager* manager) {
    manager->user_count = 0;
}

void user_manager_register(UserManager* manager, const char* username, const char* password) {
    if (manager->user_count < MAX_USERS) {
        strcpy(manager->users[manager->user_count].username, username);
        manager->users[manager->user_count].hashedPassword = hash_password(password);
        manager->user_count++;
        printf("\nUser registered successfully!\n");
    } else {
        printf("Cannot register more users. System is full.\n");
    }
}

int user_manager_authenticate(UserManager* manager, const char* username, const char* password) {
    unsigned long hashedPassword = hash_password(password);
    for (int i = 0; i < manager->user_count; i++) {
        if (strcmp(manager->users[i].username, username) == 0 && manager->users[i].hashedPassword == hashedPassword) {
            return 1; 
        }
    }
    return 0; 
}

unsigned long hash_password(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; 
    }
    return hash;
}

void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
