#include<stdio.h>
#include <fcntl.h>
#include<stdlib.h>
#include <unistd.h>
#include<string.h>
#include "manager.h"
#include "utils.h"
#include "loan.h"



int apply_for_loan(const char* customer_id, float loan_amount, const char* loan_type, int term) {
    Loan new_loan;
    static int loan_counter = 1; // Load last loan ID to persist

    // Open the file to read the last loan ID if available
    FILE *file = fopen("loans.txt", "r");
    if (file != NULL) {
        char line[256];
        int last_id = 0;

        // Read through the file to find the last loan ID
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &last_id);
        }

        loan_counter = last_id + 1; // Set the loan_counter to the next ID
        fclose(file);
    }

    // Assign loan details
    new_loan.loan_id = loan_counter++;
    strcpy(new_loan.customer_id, customer_id);
    new_loan.loan_amount = loan_amount;
    strcpy(new_loan.loan_type, loan_type);
    new_loan.term = term;
    strcpy(new_loan.status, "pending");
    strcpy(new_loan.assigned_to, "unassigned");

    // Save the loan application in a file
    file = fopen("loans.txt", "a");
    if (file == NULL) {
        perror("Error opening loan file");
        return -1;
    }

    // Write loan details to the file
    fprintf(file, "%d,%s,%.2f,%s,%d,%s,%s\n", new_loan.loan_id, new_loan.customer_id, new_loan.loan_amount,
            new_loan.loan_type, new_loan.term, new_loan.status, new_loan.assigned_to);

    fclose(file);
    printf("Loan application submitted!\n");
    return 0;
}

int assign_loan_to_employee(int loan_id, const char* employee_id) {
    FILE *file = fopen("loans.txt", "r");
    FILE *temp = fopen("loans_temp.txt", "w");
    if (!file || !temp) {
        perror("Error opening loan file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Loan loan;
        sscanf(line, "%d,%[^,],%f,%[^,],%d,%[^,],%[^,\n]", &loan.loan_id, loan.customer_id, &loan.loan_amount,
               loan.loan_type, &loan.term, loan.status, loan.assigned_to);

        if (loan.loan_id == loan_id && strcmp(loan.status, "pending") == 0) {
            strcpy(loan.assigned_to, employee_id);  // Assign to employee
            printf("Loan ID %d assigned to employee %s.\n", loan_id, employee_id);
        }

        // Write updated loan back to the temp file
        fprintf(temp, "%d,%s,%.2f,%s,%d,%s,%s\n", loan.loan_id, loan.customer_id, loan.loan_amount,
                loan.loan_type, loan.term, loan.status, loan.assigned_to);
    }

    fclose(file);
    fclose(temp);

    // Replace the original file with the updated one
    rename("loans_temp.txt", "loans.txt");
    return 0;
}

int approve_or_reject_loan(int loan_id, const char* employee_id, const char* decision) {
    FILE *file = fopen("loans.txt", "r");
    FILE *temp = fopen("loans_temp.txt", "w");
    if (!file || !temp) {
        perror("Error opening loan file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Loan loan;
        sscanf(line, "%d,%[^,],%f,%[^,],%d,%[^,],%[^,\n]", &loan.loan_id, loan.customer_id, &loan.loan_amount,
               loan.loan_type, &loan.term, loan.status, loan.assigned_to);

        if (loan.loan_id == loan_id && strcmp(loan.assigned_to, employee_id) == 0) {
            strcpy(loan.status, decision);  // "approved" or "rejected"
            printf("Loan ID %d %s by employee %s.\n", loan_id, decision, employee_id);
        }

        // Write updated loan back to the temp file
        fprintf(temp, "%d,%s,%.2f,%s,%d,%s,%s\n", loan.loan_id, loan.customer_id, loan.loan_amount,
                loan.loan_type, loan.term, loan.status, loan.assigned_to);
    }

    fclose(file);
    fclose(temp);

    // Replace the original file with the updated one
    rename("loans_temp.txt", "loans.txt");
    return 0;
}

char* view_customer_loans(const char* customer_id) {
    FILE *file = fopen("loans.txt", "r");
    if (file == NULL) {
        perror("Error opening loan file");
        return NULL;
    }

    // Allocate a buffer to store the output string (adjust the size as needed)
    char *result = malloc(4096);  // Allocate a large enough buffer
    if (result == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Initialize the result string
    strcpy(result, "Loans for customer ID: ");
    strcat(result, customer_id);
    strcat(result, "\nLoan ID | Amount  | Type        | Term | Status    | Assigned To\n");
    strcat(result, "---------------------------------------------------------------\n");

    char line[256];
    int found = 0;

    // Read each line and append loans that match the customer's ID to the result string
    while (fgets(line, sizeof(line), file)) {
        Loan loan;
        sscanf(line, "%d,%[^,],%f,%[^,],%d,%[^,],%[^,\n]", &loan.loan_id, loan.customer_id, &loan.loan_amount,
               loan.loan_type, &loan.term, loan.status, loan.assigned_to);

        if (strcmp(loan.customer_id, customer_id) == 0) {
            char loan_info[256];
            sprintf(loan_info, "%7d | %.2f | %-10s | %4d | %-9s | %-10s\n", loan.loan_id, loan.loan_amount, loan.loan_type,
                    loan.term, loan.status, loan.assigned_to);
            strcat(result, loan_info);
            found = 1;
        }
    }

    if (!found) {
        strcat(result, "No loans found for customer ID: ");
        strcat(result, customer_id);
        strcat(result, "\n");
    }

    fclose(file);
    return result;
}

char* view_unassigned_loans() {
    FILE *file = fopen("loans.txt", "r");
    if (file == NULL) {
        perror("Error opening loan file");
        return NULL;
    }

    char line[256];
    int found = 0;

    // Allocate initial memory for the result string
    char* result = (char*)malloc(1024 * sizeof(char)); // Start with 1KB
    if (result == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }
    result[0] = '\0'; // Initialize the string as empty

    // Header
    strcat(result, "Unassigned Loans:\n");
    strcat(result, "Loan ID | Customer ID | Amount  | Type        | Term | Status\n");
    strcat(result, "------------------------------------------------------------\n");

    // Read each line and append loans that are unassigned to the result string
    while (fgets(line, sizeof(line), file)) {
        Loan loan;
        sscanf(line, "%d,%[^,],%f,%[^,],%d,%[^,],%[^,\n]", &loan.loan_id, loan.customer_id, &loan.loan_amount,
               loan.loan_type, &loan.term, loan.status, loan.assigned_to);

        if (strcmp(loan.assigned_to, "unassigned") == 0) {
            char buffer[256];
            sprintf(buffer, "%7d | %-11s | %.2f | %-10s | %4d | %-9s\n", loan.loan_id, loan.customer_id,
                    loan.loan_amount, loan.loan_type, loan.term, loan.status);
            strcat(result, buffer);
            found = 1;
        }
    }

    if (!found) {
        strcat(result, "No unassigned loans found.\n");
    }

    fclose(file);
    return result; // Return the accumulated result string
}

char* view_loans_assigned_to_employee(const char* employee_id) {
    FILE *file = fopen("loans.txt", "r");
    if (file == NULL) {
        perror("Error opening loan file");
        return NULL;
    }

    char line[256];
    int found = 0;

    // Allocate initial memory for the result string
    char* result = (char*)malloc(1024 * sizeof(char)); // Start with 1KB
    if (result == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return NULL;
    }
    result[0] = '\0'; // Initialize the string as empty

    // Header
    strcat(result, "Loans assigned to employee ID: ");
    strcat(result, employee_id);
    strcat(result, "\nLoan ID | Customer ID | Amount  | Type        | Term | Status\n");
    strcat(result, "------------------------------------------------------------\n");

    // Read each line and append loans that are assigned to the employee to the result string
    while (fgets(line, sizeof(line), file)) {
        Loan loan;
        sscanf(line, "%d,%[^,],%f,%[^,],%d,%[^,],%[^,\n]", &loan.loan_id, loan.customer_id, &loan.loan_amount,
               loan.loan_type, &loan.term, loan.status, loan.assigned_to);

        if (strcmp(loan.assigned_to, employee_id) == 0) {
            char buffer[256];
            sprintf(buffer, "%7d | %-11s | %.2f | %-10s | %4d | %-9s\n", loan.loan_id, loan.customer_id,
                    loan.loan_amount, loan.loan_type, loan.term, loan.status);
            strcat(result, buffer);
            found = 1;
        }
    }

    if (!found) {
        strcat(result, "No loans assigned to this employee.\n");
    }

    fclose(file);
    return result; // Return the accumulated result string
}