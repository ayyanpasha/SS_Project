#ifndef ADMIN_H  // Include guard to prevent multiple inclusions
#define ADMIN_H

// Function prototypes

struct Employee {
    char username[20];          
    char name[100];            
    char role[20];             
    char phone[15];
    char salary[10];      
};

struct Customer {
    char username[20];
    char account_number[20];
    char name[100];
    char phone_number[15];
    float balance;
    int loan_status;
    float loan_amount;
};


struct Transaction {
    char transaction_id[20];
    char sender[20];
    char receiver[20];
    float amount;
    char type[10]; // "deposit", "withdraw", "transfer"
    char timestamp[20]; // Format: YYYY-MM-DD HH:MM:SS
};



char* my_fun_adm();  // Example function declaration

int add_manager_role(const char* username);
int remove_manager_role(const char* username);
int add_employee(struct Employee emp);   
int update_employee_internal(char* uname, char* newval, int opt);
// void manage_role(int);
// void logout_admin(int);  
// void change_password_admin(int); 
extern char whoami[20];

#endif  // End of include guard

