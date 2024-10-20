// loan.h
#ifndef LOAN_H
#define LOAN_H

typedef struct {
    int loan_id;            // Unique ID for the loan
    char customer_id[50];   // Customer who applied for the loan
    float loan_amount;      // Amount requested
    char loan_type[50];     // Type of loan (personal, home, etc.)
    int term;               // Loan term in months
    char status[20];        // Status: pending, approved, rejected
    char assigned_to[50];   // Assigned employee (for processing)
} Loan;

// Function declarations
int apply_for_loan(const char* customer_id, float loan_amount, const char* loan_type, int term);
int assign_loan_to_employee(int loan_id, const char* employee_id);
int approve_or_reject_loan(int loan_id, const char* employee_id, const char* decision);
char* view_customer_loans(const char* customer_id);
char* view_unassigned_loans();
char* view_loans_assigned_to_employee(const char* employee_id); 


#endif
