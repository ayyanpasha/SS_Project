// utils.c
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include "handler.h"
#include "server.h"

char whoami[20];


int read_line(int fd, char *buffer) {
    char c;
    int i = 0;
    while (read(fd, &c, 1) == 1) {
        if (c == '\n' || c == '\0') {
            buffer[i] = '\0';
            return 1;
        }
        buffer[i++] = c;
    }
    return (i > 0);  // Return true if at least one character was read
}

int lock_file(int fd, int lock_type) {
    struct flock lock;
    lock.l_type = lock_type;  // F_WRLCK for write, F_RDLCK for read
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;  // Lock entire file
    lock.l_pid = getpid();

    // Block until lock is acquired
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error acquiring lock");
        return -1;
    }
    return 0;
}


void unlock_file(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;  // Unlock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error releasing lock");
    }
}


int authenticate_user(char *username, char *password,int choice) {
    for (int i = 0; i < 10; i++) {
        // Use strlen() to check if the username is empty
        printf("Hi\n");
        printf("%s\n",shared_usernames[i]);
        if (strlen(shared_usernames[i]) > 0 && strcmp(shared_usernames[i], username) == 0) {
            printf("User %s is already logged in.\n", username);
            return 0;  // User already logged in
        }
    }
    
    int fd;
    //printf("I am : %s\n",username);
    switch(choice){
        case 1: fd = open("customer_credentials", O_RDONLY);
                break;
        case 2: fd = open("employee_credentials", O_RDONLY);
                break;   
        case 3: fd = open("manager_credentials", O_RDONLY);
                break;
        case 4: fd = open("admin_credentials", O_RDONLY);
                break;
   }
    if (fd == -1) {
        perror("Error opening file");
        return 0;
    }
    
    char buffer[256];
    char stored_username[50], stored_password[50];
    while (read_line(fd, buffer)) {
        sscanf(buffer, "%[^,],%s", stored_username, stored_password);
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            printf("paswoed matched\n");
            strcpy(whoami,username);
            close(fd);
            for (int i = 0; i < 10; i++) {
                // Check for empty string using strlen()
                printf("HI\n");
                printf("%s\n",shared_usernames[i]);
                if (strlen(shared_usernames[i]) == 0) {
                    strcpy(shared_usernames[i], username);  // Add username to shared memory
                    printf("Written in shared memory: %s\n", username);
                    break;
                }
            }
            return 1;  // Credentials matched, login successful
        }
    }
    
    close(fd);
    

    return 0;  // Credentials not matched, login failed
}

int change_password_user(const char *username,const char*newpassword,int choice) {
    // Open the manager password file for reading
    char filename[50];
     printf("I am : %s\n",username);
     printf("new password is:-%s\n",newpassword);
    switch(choice){
        case 1: strcpy(filename,"customer_credentials");
                break;
        case 2: strcpy(filename,"employee_credentials");
                break;
        case 3: strcpy(filename,"manager_credentials");
                break;
        case 4: strcpy(filename,"admin_credentials");
                break;
    }
    FILE* file = fopen(filename, "r");
    int file_fd = fileno(file);
    if (file_fd == -1) {
        perror("Error opening manager password file");
        return -1;
    }
    printf("file open successfull\n");
    // Lock the manager password file
    if (lock_file(file_fd, F_RDLCK) == -1) {
        close(file_fd);
        return -1;
    }
    printf("file lock successfull\n");
    // Open a temporary file for writing
    FILE* temp = fopen("temp", "w");
    int temp_fd = fileno(temp);
    // if (temp_fd == -1) {
    //     perror("Error opening temporary manager password file");
    //     unlock_file(mgr_fd);
    //     close(mgr_fd);
    //     return -1;
    // }
    printf("file creation successfull\n");
    if (lock_file(temp_fd, F_WRLCK) == -1) {
        close(file_fd);
        return -1;
    }
    printf("file lock successfull\n");
    char line[100];
    int found = 0;

    // Read the manager password file line by line


    while (fgets(line, sizeof(line), file) != NULL) {
        // printf("%s\n",line);
        char myline[50];
        strcpy(myline,line);
        // Tokenize the line on comma
        char *token = strtok(line, ",");
        printf("%s\n",myline);

        if(strcmp(token,username)==0){
            found = 1;
             printf("%s\n",token);
            char newline[100];
            // sprintf(newline, "%s,%s\n", username, newpassword);
            // Use snprintf to ensure null-termination and prevent buffer overflow
            snprintf(newline, sizeof(newline), "%s,%s\n", username, newpassword);
            printf("line to be written is:%s\n",newline);
            if (fwrite(newline, sizeof(char), strlen(newline), temp) != strlen(newline)) {
            perror("Error writing to file\n");
            }
            fflush(temp);
        }
        else{
            printf("In else block\n");
            printf("line to write %s\n",line);
            int result = fwrite(myline, sizeof(char), strlen(myline), temp);
            fflush(temp);
            printf("%d\n",result);
            if(result!=strlen(myline)){
                perror("error\n");
                return -1;
            }
        }

    }
    

    // Clean up
    unlock_file(file_fd);
    close(file_fd);
    close(temp_fd);

    printf("lock unlock done\n");

    // Replace the original manager password file with the temporary file
   
        
        if (rename("temp",filename) == -1) {
            perror("Error replacing manager password file");
            return -1; // Indicate failure in renaming
        }
    printf("Done\n");
    // strcpy(whoami,username);
    return 0; // Success
}
