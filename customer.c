#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include <unistd.h>
#include "customer.h"
#include "utils.h"
#include "admin.h"
#include "handler.h"
#include <time.h>
#include <sys/socket.h> 



#define MAX_TRANSACTIONS 100
#define BUFFER_SIZE 1024



void log_transaction(const char *username, const char *sender, const char *receiver, float amount, const char *type) {
    char filename[100];
    snprintf(filename, sizeof(filename), "customer_data/%s_transactions.csv", username);

    // Open the user-specific transaction history file
    FILE *log_file = fopen(filename, "a+");
    if (log_file == NULL) {
        perror("Error opening transaction history file");
        return;
    }
    
    time_t now = time(NULL);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Log the transaction details
    fprintf(log_file, "%s,%s,%s,%.2f,%s,%s\n", username,sender, receiver, amount, type, timestamp);
    fclose(log_file);
}

void retrieve_transaction_history(int sock,char*username) {
    char filename[100];
    //const char* username = current_cust;
    snprintf(filename, sizeof(filename), "customer_data/%s_transactions.csv", username);
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("Error opening transaction history file");
        char *msg = "Operation Not Successfull\n";
        send(sock,msg,strlen(msg),0);
        return ;
    }

    struct Transaction transactions[MAX_TRANSACTIONS];
    int count = 0;

    // Read the CSV file line by line
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Parse the line and populate the structure
        sscanf(buffer,  "%19[^,],%19[^,],%19[^,],%f,%9[^,],%19[^,]", 
               transactions[count].transaction_id, 
               transactions[count].sender,
               transactions[count].receiver,
               &transactions[count].amount,
               transactions[count].type,
               transactions[count].timestamp);

        printf("ID: %s, Sender: %s, Receiver: %s, Amount: %.2f, Type: %s, Timestamp: %s\n",
               transactions[count].transaction_id,
               transactions[count].sender,
               transactions[count].receiver,
               transactions[count].amount,
               transactions[count].type,
               transactions[count].timestamp);
        
        count++;
    }
    
    fclose(file);

    // Prepare and send the transaction history
    char send_buffer[BUFFER_SIZE];
    snprintf(send_buffer, sizeof(send_buffer), "Transaction History for %s:\n", username);
    
    for (int i = count-1; i >= 0; i--) {
        snprintf(send_buffer + strlen(send_buffer), sizeof(send_buffer) - strlen(send_buffer), 
                 "ID: %s, Sender: %s, Receiver: %s, Amount: %.2f, Type: %s, Timestamp: %s\n",
                 transactions[i].transaction_id,
                 transactions[i].sender,
                 transactions[i].receiver,
                 transactions[i].amount,
                 transactions[i].type,
                 transactions[i].timestamp);
    }

    // Send the transaction history to the client
    send(sock, send_buffer, strlen(send_buffer), 0);
}

float check_balance(char*uname){
    printf("check balance\n");
    char filename[100];
    snprintf(filename, sizeof(filename), "customer_data/%s.csv", uname);
    printf("%s\n", filename);

    int fd = open(filename, O_RDONLY); // Open for reading
    if (fd == -1) {
        perror("Error opening employee_data.csv file");
        return -1;
    }

    // Lock the employee data file
    if (lock_file(fd, F_RDLCK) == -1) {
        close(fd);
        return -1;
    }

    struct Customer cust;
    char buffer[256];
    int found = 0;

    // Read the file into buffer
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading the file");
        unlock_file(fd);
        close(fd);
        return -1;
    }
    buffer[bytes_read] = '\0';  // Null-terminate the buffer

    // Parse the file content into the Customer struct
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f\n",
           cust.username, cust.account_number, cust.name, cust.phone_number, 
           &cust.balance, &cust.loan_status, &cust.loan_amount);

    // Unlock and close after reading
    unlock_file(fd);
    close(fd);
    return cust.balance;
}

int deposit_balance(char* uname,float amount){
    // char* uname = current_cust;
    char filename[100];
    snprintf(filename, sizeof(filename), "customer_data/%s.csv", uname);
    printf("%s\n", filename);

    int fd = open(filename, O_RDWR); // Open for reading
    if (fd == -1) {
        perror("Error opening employee_data.csv file");
        return -1;
    }

    // Lock the employee data file
    if (lock_file(fd, F_WRLCK) == -1) {
        close(fd);
        return -1;
    }

    struct Customer cust;
    char buffer[256];
    int found = 0;

    // Read the file into buffer
    int bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Error reading the file");
        unlock_file(fd);
        close(fd);
        return -1;
    }
    buffer[bytes_read] = '\0';  // Null-terminate the buffer

    // Parse the file content into the Customer struct
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f\n",
           cust.username, cust.account_number, cust.name, cust.phone_number, 
           &cust.balance, &cust.loan_status, &cust.loan_amount);

    // Unlock and close after reading


    if(cust.balance+amount<0){
        unlock_file(fd);
    close(fd);
    return -1;
    }
    cust.balance = cust.balance+amount;


    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error seeking to the beginning of the file");
        unlock_file(fd);
        close(fd);
        return -1;
    }

   
    // Reformat the updated Customer data into the buffer
    int bytes_written = snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%.2f,%d,%.2f\n",
                                 cust.username, cust.account_number, cust.name, cust.phone_number,
                                 cust.balance, cust.loan_status, cust.loan_amount);

    // Write the updated content to the file
    if (write(fd, buffer, bytes_written) == -1) {
        perror("Error writing to the file");
        close(fd);
        return -1;
    }

    printf("Successfully wrote the updated data\n");

    if(amount>0){
    log_transaction(uname,"self","self",amount,"Deposit");
    }
    else{
    log_transaction(uname,"self","self",amount,"Withdraw");
    }
  
    unlock_file(fd);
    close(fd);
    return 0;

}

int transfer_funds(char* sender_uname,char* receiver_uname, float amount) {
    
    if(strcmp(sender_uname,receiver_uname)==0){
        return -1;
    }
    char sender_file[100], receiver_file[100];
    snprintf(sender_file, sizeof(sender_file), "customer_data/%s.csv", sender_uname);
    snprintf(receiver_file, sizeof(receiver_file), "customer_data/%s.csv", receiver_uname);
    printf("%s\n",receiver_uname);
    printf("%s\n",receiver_file);

    // Open sender and receiver files
    int sender_fd = open(sender_file, O_RDWR);
    if (sender_fd == -1) {
        perror("Error opening sender account file");
        return -1;
    }
    
    int receiver_fd = open(receiver_file, O_RDWR);
    if (receiver_fd == -1) {
        perror("Error opening receiver account file");
        close(sender_fd);
        return -1;
    }

    // Lock both accounts
    if (lock_file(sender_fd, F_WRLCK) == -1 || lock_file(receiver_fd, F_WRLCK) == -1) {
        close(sender_fd);
        close(receiver_fd);
        return -1;
    }

    struct Customer sender, receiver;
    char buffer[256];

    // Read sender's account
    read(sender_fd, buffer, sizeof(buffer) - 1);
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f",
           sender.username, sender.account_number, sender.name, sender.phone_number,
           &sender.balance, &sender.loan_status, &sender.loan_amount);

    // Read receiver's account
    read(receiver_fd, buffer, sizeof(buffer) - 1);
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f",
           receiver.username, receiver.account_number, receiver.name, receiver.phone_number,
           &receiver.balance, &receiver.loan_status, &receiver.loan_amount);

    // Check if sender has sufficient funds
    if (sender.balance < amount) {
        printf("Insufficient funds for transfer.\n");
        unlock_file(sender_fd);
        unlock_file(receiver_fd);
        close(sender_fd);
        close(receiver_fd);
        return -1;
    }

    // Update balances
    sender.balance -= amount;
    receiver.balance += amount;

    // Write updated balances back to the files
    lseek(sender_fd, 0, SEEK_SET);
    snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%.2f,%d,%.2f\n", sender.username,
             sender.account_number, sender.name, sender.phone_number,
             sender.balance, sender.loan_status, sender.loan_amount);
    write(sender_fd, buffer, strlen(buffer));

    lseek(receiver_fd, 0, SEEK_SET);
    snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%.2f,%d,%.2f\n", receiver.username,
             receiver.account_number, receiver.name, receiver.phone_number,
             receiver.balance, receiver.loan_status, receiver.loan_amount);
    write(receiver_fd, buffer, strlen(buffer));

    unlock_file(sender_fd);
    unlock_file(receiver_fd);
    
    close(sender_fd);
    close(receiver_fd);

    log_transaction(sender_uname,sender_uname,receiver_uname,amount,"Debit");
    log_transaction(receiver_uname,sender_uname,receiver_uname,amount,"Credit");

    printf("Transfer successful.\n");
    return 0;
}


int add_feedback(char*){
    printf("add feedback\n");
    return 0;
}

void submit_feedback(const char* username, const char* feedback) {
    FILE *file = fopen("feedback.txt", "a");
    if (file == NULL) {
        perror("Error opening feedback file");
        return;
    }
    printf("%s\n",feedback);
    // Ensure the strings are not breaking by spaces
    fprintf(file, "%s,\"%s\"\n", username, feedback);  // Wrapping feedback in quotes

    printf("Feedback submitted successfully!\n");

    fclose(file);
}

void get_feedbacks(char* result) {
    FILE *file = fopen("feedback.txt", "r");
    if (file == NULL) {
        perror("Error opening feedback file");
        strcpy(result, "No feedback available.\n");
        return;
    }

    char lines[100][256];  // To store up to 100 feedbacks
    int count = 0;

    // Read all feedback file
    while (fgets(lines[count], sizeof(lines[count]), file)) {
        lines[count][strcspn(lines[count], "\n")] = 0;  // Remove newline char
        count++;
    }

    fclose(file);

    // Determine how many feedbacks to return
    int start = count >= 5 ? count - 5 : 0;

    // Collect the last 5 feedbacks
    strcpy(result, "Last 5 Feedbacks:\n");
    for (int i = start; i < count; i++) {
        strcat(result, lines[i]);
        strcat(result, "\n");
    }

    if (count == 0) {
        strcpy(result, "No feedback available.\n");
    }
}


