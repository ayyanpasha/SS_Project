#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "employee.h"
#include "admin.h"
#include<string.h>
#include <sys/socket.h>
#include "utils.h"

int update_customer_internal(char* uname, char* newval, int opt) {
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
    sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f\n",
           cust.username, cust.account_number, cust.name, cust.phone_number, 
           &cust.balance, &cust.loan_status, &cust.loan_amount);

    // Unlock and close after reading
    unlock_file(fd);
    close(fd);
    printf("%d\n",opt);
    // Update the required field based on the option
    switch(opt) {
        case 1: strcpy(cust.account_number, newval); break;
        case 2: strcpy(cust.name, newval); break;
        case 3: strcpy(cust.phone_number, newval); break;
        default: 
            printf("Invalid option\n");
            return -1;
    }

    // Open the file for writing and truncate it (overwrite it)
    fd = open(filename, O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error opening file for writing");
        return -1;
    }

    // Lock the file for writing
    if (lock_file(fd, F_WRLCK) == -1) {
        close(fd);
        return -1;
    }
     int bytes_written = snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%.2f,%d,%.2f\n",
                                 cust.username, cust.account_number, cust.name, cust.phone_number,
                                 cust.balance, cust.loan_status, cust.loan_amount);

    // Write the updated content to the file
    if (write(fd, buffer, bytes_written) == -1) {
        perror("Error writing to the file");
        close(fd); // Always close the file descriptor
        return -1;
    }

    printf("Successfully wrote the updated data\n");

    // Unlock and close the file
    unlock_file(fd);
    close(fd);
    return 0;
}


int add_customer_internal(struct Customer cust) {
    char filename[100];
    snprintf(filename, sizeof(filename), "customer_data/%s.csv", cust.username);
    printf("%s\n",filename);
    int fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0666); // Open for appending
    if (fd == -1) {
        perror("Error opening employee_data.csv file");
        return -1;
    }

    // Lock the employee data file
    if (lock_file(fd, F_WRLCK) == -1) {
        close(fd);
        return -1;
    }

    // Check if the employee already exists
    // struct Customer temp;
    // char buffer[256];
    // while (fgets(buffer, sizeof(buffer), file)) {
    //     sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%f,%d,%f", temp.username, temp.account_number, temp.name, temp.phone_number, &temp.balance, &temp.loan_status, &temp.loan_amount);
    //     if (strcmp(temp.username, cust.username) == 0) {
    //         printf("Customer already exists!\n");
    //         // Unlock the file before exiting
    //         unlock_file(fd);
    //         fclose(file);
    //         return -1;  // Employee already exists
    //     }
    // }

    // Write new employee in CSV format
    char buffer[100];
    int bytes_written = snprintf(buffer, sizeof(buffer), "%s,%s,%s,%s,%.2f,%d,%.2f\n", 
                                  cust.username, cust.account_number, cust.name, 
                                  cust.phone_number, cust.balance, cust.loan_status, 
                                  cust.loan_amount);
     if (write(fd, buffer, bytes_written) == -1) {
        perror("Error writing to file");
        close(fd); // Don't forget to close the file descriptor
        exit(EXIT_FAILURE);
    }
    printf("Customer data saved successfully!\n");

    // Unlock the employee data file
    unlock_file(fd);
    close(fd);
    int pwd_fd = open("customer_credentials", O_RDWR | O_CREAT | O_APPEND, 0666);
        if (pwd_fd == -1) {
            perror("Error opening employee_passwords file");
            unlock_file(fd);
            close(fd);
            return -1;
        }

        // Lock the password file
        if (lock_file(pwd_fd, F_WRLCK) == -1) {
            close(pwd_fd);
            unlock_file(fd);
            close(fd);
            return -1;
        }

        // Example password entry (you can replace this with actual user input)
        char password_entry[40];
        snprintf(password_entry, sizeof(password_entry), "%s,%s\n", cust.username, "12345");
        // Write the new password entry
        if (write(pwd_fd, password_entry, strlen(password_entry)) == -1) {
            perror("Error writing to employee_passwords file");
            unlock_file(pwd_fd);
            unlock_file(fd);
            close(pwd_fd);
            close(fd);
            return -1;
        }

        // Unlock the password file
        unlock_file(pwd_fd);
        close(pwd_fd);

    return 0;  // Success
}

void add_cust(int sock){
  
    printf("I was called\n");
    struct Customer cust;
    char menu[] = "\t--Enter the detail of customer to add in following sequence:-\n1.Username:\n2.Account Number:\n3.Name of Customer:\n4.Contact Number:\n5.Balane:-\n";
    send(sock, menu, strlen(menu), 0);
    int num=0;
    read(sock,cust.username,sizeof(cust.username));
    send(sock,&num,sizeof(int),0); 
    read(sock,cust.account_number,sizeof(cust.account_number));
    send(sock,&num,sizeof(int),0); 
    read(sock,cust.name,sizeof(cust.name));
    send(sock,&num,sizeof(int),0); 
    read(sock,cust.phone_number,sizeof(cust.phone_number));
    send(sock,&num,sizeof(int),0); 
    read(sock,&cust.balance,sizeof(cust.balance));
    send(sock,&num,sizeof(int),0); 
    printf("%s\n",cust.username);
    printf("%s\n",cust.account_number);
    printf("%s\n",cust.name);
    printf("%s\n",cust.phone_number);
    printf("%f\n",cust.balance);
    cust.loan_amount=0.0;
    cust.loan_status=0;
    int flag = add_customer_internal(cust);
    if(flag==0){
        char* msg = "Customer added Successfully\n";
        send(sock, msg, strlen(msg), 0);
    }
    else{
        char* msg = "Customer Not Added\n";
        send(sock, msg, strlen(msg), 0);
    }
    return ;

}

void modify_customer_employee(int sock){
    printf("I was called\n");
    struct Customer cust;
    char uname[20];
    char* msg = "Enter username for which value is to be updated:-\n";
    send(sock,msg,strlen(msg),0);
    read(sock,uname,sizeof(uname));
    printf("updating value for %s\n",uname);
    char menu[] = "\t--choose the field of customer to update in following sequence:-\n1.Account number:\n2.Name\n3.Contact Number:\nEnter 1-3:-";
    send(sock, menu, strlen(menu), 0);
    int menu_opt;
    read(sock,&menu_opt,sizeof(int));
    int flag_curr;
    printf("%d\n",menu_opt);
    switch(menu_opt){
        case 1: msg = "Enter new account number:-";
                send(sock,msg,strlen(msg),0);
                read(sock,cust.account_number,sizeof(cust.account_number));
                flag_curr = update_customer_internal(uname,cust.account_number,1);
                if(flag_curr==0){
                    msg = "Update Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                else{
                    msg = "Update Not Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                break;
        case 2: msg = "Enter new name:-";
                send(sock,msg,strlen(msg),0);
                read(sock,cust.name,sizeof(cust.name));
                flag_curr = update_customer_internal(uname,cust.name,2);
                if(flag_curr==0){
                    msg = "Update Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                else{
                    msg = "Update Not Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                break;
        case 3: msg = "Enter new phone:-";
                send(sock,msg,strlen(msg),0);
                read(sock,cust.phone_number,sizeof(cust.phone_number));
                flag_curr = update_customer_internal(uname,cust.phone_number,3);
                if(flag_curr==0){
                    msg = "Update Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                else{
                    msg = "Update Not Successfull";
                    send(sock,msg,strlen(msg),0);
                }
                break;
    }
    
}

