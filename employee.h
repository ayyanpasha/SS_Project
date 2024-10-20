#ifndef EMPLOYEE_H  // Include guard to prevent multiple inclusions
#define EMPLOYEE_H

// Function prototypes
void my_fun_emp(int);  // Example function declaration
int login_employee(char*uname,char*psd);
void add_cust(int sock);     
void process_loan(int sock);
void approve_loan(int sock);
void assigned_loan(int sock);
void view_transaction(int sock);
void change_pwd(int sock);
void logout_emp(int sock);
void modify_customer_employee(int sock);
extern char whoami[20];

// struct Customer {
//     char username[20];
//     char account_number[20];
//     char name[100];
//     char phone_number[15];
//     double balance;
//     int loan_status;
//     double loan_amount;
// };


#endif  // End of include guard
