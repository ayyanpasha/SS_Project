#ifndef MANAGER_H  // Include guard to prevent multiple inclusions
#define MANAGER_H

// Function prototypes
char* my_fun_man();  // Example function declaration

int login_manager(char*,char*);
void manage_account(int );
void loan_assignment(int );       
void review_feedback(int );
void change_password_manager(int );
void logout_manager(int );
extern char whoami[20];

#endif  // End of include guard
