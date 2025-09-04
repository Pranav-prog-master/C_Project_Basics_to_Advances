#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int who;
    int l_no;
    char name[100];
    int registration_no;
    int hostel_no;
    int room_no;
    char block;
    long int mobile_no;
    int password;
} Registration;

void input(Registration* r);
void get_register(Registration* r);
void set_pass(Registration* r);
int check_credentials(const Registration* r, int num, int pin);
int login_user();
void clean_input_buffer();
void strip_newline(char* str);

int main() {
    int x;
    printf("Welcome To Laundry Management System\n");
    printf("Press 1 for Login\n");
    printf("Press 2 for Registration\n");
    
    while (scanf("%d", &x) != 1 || (x != 1 && x != 2)) {
        printf("Invalid input. Please enter 1 or 2: ");
        clean_input_buffer();
    }
    clean_input_buffer(); 

    if (x == 1) { // Login
        if (login_user()) {
            printf("\nLogin Successful!\n");
        } else {
            printf("\nLogin Failed: Invalid Laundry Number or Password.\n");
        }
    } else if (x == 2) { // Registration
        Registration s1;
        input(&s1);
        set_pass(&s1);
        get_register(&s1);
    }

    return 0;
}

//  Function to get user details for registration 
void input(Registration* r) {
    printf("Enter the following details for registration\n");
    printf("Press 1 If you are a Student\n");
    printf("Press 2 If you are a Laundry Man\n");
    scanf("%d", &r->who);
    clean_input_buffer();

    if (r->who == 1) {
        printf("Registration No.: ");
        scanf("%d", &r->registration_no);
        clean_input_buffer();

        printf("\nStudent Name: ");
        fgets(r->name, 99, stdin);
        strip_newline(r->name);

        printf("\nHostel No.: ");
        scanf("%d", &r->hostel_no);
        clean_input_buffer();

        printf("\nBlock Code: ");
        scanf(" %c", &r->block); 
        clean_input_buffer();

        printf("\nRoom No.: ");
        scanf("%d", &r->room_no);
        clean_input_buffer();

        printf("\nMobile No.: ");
        scanf("%ld", &r->mobile_no);
        clean_input_buffer();
    }
}

// Function to save the registration details to a file 
void get_register(Registration* r) {
    Registration temp_r;
    int last_r = 0;
    FILE *file;

    file = fopen("laundry_book.dat", "rb");
    if (file != NULL) {
        fseek(file, -sizeof(Registration), SEEK_END);
        fread(&temp_r, sizeof(Registration), 1, file);
        last_r = temp_r.l_no;
        fclose(file);
    }

    r->l_no = last_r + 1;

    file = fopen("laundry_book.dat", "ab");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    fwrite(r, sizeof(Registration), 1, file);
    fclose(file);

    printf("Congratulations!! You are successfully registered.\n");
    printf("Your laundry number is %d.\nPlease do not forget it.\n", r->l_no);
}

// Function to set and confirm the user's password
void set_pass(Registration* r) {
    int repin;
    printf("\nPlease set a strong password to protect your Laundry Account.\n");
    printf("Your laundry number will be your user name.\n");

    do {
        printf("Password: ");
        scanf("%d", &r->password);
        clean_input_buffer();

        printf("\nConfirm Password: ");
        scanf("%d", &repin);
        clean_input_buffer();

        if (r->password != repin) {
            printf("\nPasswords do not match. Please try again.\n");
        }
    } while (r->password != repin);

    printf("\nYour Password was saved successfully.\n");
}

// Function to check credentials match 
int check_credentials(const Registration* r, int num, int pin) {
    return (r->l_no == num && r->password == pin);
}

// Function to handle the login process 
int login_user() {
    int pin;
    int lnumber;
    Registration r;
    FILE *file;

    printf("Welcome Back\nPlease Enter the required credentials\n");
    printf("\nLaundry Number: ");
    scanf("%d", &lnumber);
    clean_input_buffer();

    printf("\nPassword: ");
    scanf("%d", &pin);
    clean_input_buffer();

    file = fopen("laundry_book.dat", "rb");
    if (file == NULL) {
        printf("Error: Cannot open registration file. No users registered yet.\n");
        return 0; 
    }

    while (fread(&r, sizeof(Registration), 1, file)) {
        if (check_credentials(&r, lnumber, pin)) {
            fclose(file);
            return 1; 
        }
    }

    fclose(file);
    return 0; 
}

// Utility function to clear the input buffer 
void clean_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Utility function to remove trailing newline from fgets
void strip_newline(char* str) {
    str[strcspn(str, "\r\n")] = 0;
}
