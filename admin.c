#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "admin.h"
#include "utils.h"
#include <sys/socket.h>
#include "employee.h"
#include "handler.h"

int add_manager_role(const char *username) {
    FILE *emp = fopen("employee_credentials", "r");
    if (emp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    int emp_fd = fileno(emp);
    if (lock_file(emp_fd, F_RDLCK) == -1) {
        close(emp_fd);
        return -1;
    }

    FILE *mgr = fopen("manager_credentials", "r");
    if (mgr == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    int mgr_fd = fileno(mgr);
    if (lock_file(mgr_fd, F_RDLCK) == -1) {
        close(mgr_fd);
        unlock_file(emp_fd);
        close(emp_fd);
        return -1;
    }

    char line[100];
    while (fgets(line, sizeof(line), mgr) != NULL) {
        char *token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            unlock_file(mgr_fd);
            unlock_file(emp_fd);
            fclose(emp);
            fclose(mgr);
            return -1;
        }
    }
    fclose(mgr);
    mgr = fopen("manager_credentials", "a");
    if (mgr == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    mgr_fd = fileno(mgr);
    if (lock_file(mgr_fd, F_WRLCK) == -1) {
        close(mgr_fd);
        unlock_file(emp_fd);
        close(emp_fd);
        return -1;
    }
    
    int found = 0;
    while (fgets(line, sizeof(line), emp) != NULL) {
        char myline[20];
        strcpy(myline, line);
        char *token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            int result = fwrite(myline, sizeof(char), strlen(myline), mgr);
            if (result != strlen(myline)) {
                perror("error\n");
                return -1;
            }
            unlock_file(mgr_fd);
            unlock_file(emp_fd);
            fclose(emp);
            fclose(mgr);
            return 0;
        }
    }

    return -1;
}

int remove_manager_role(const char *username) {
    FILE *mgr = fopen("manager_credentials", "r");
    int mgr_fd = fileno(mgr);
    if (lock_file(mgr_fd, F_RDLCK) == -1) {
        close(mgr_fd);
        return -1;
    }
    FILE *temp = fopen("temp", "w");
    int temp_fd = fileno(temp);
    if (lock_file(temp_fd, F_WRLCK) == -1) {
        close(mgr_fd);
        return -1;
    }
    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), mgr) != NULL) {
        char myline[20];
        strcpy(myline, line);
        char *token = strtok(line, ",");
        if (strcmp(token, username) == 0) {
            found = 1;
        } else {
            int result = fwrite(myline, sizeof(char), strlen(myline), temp);
            fflush(temp);
            if (result != strlen(myline)) {
                perror("error\n");
                return -1;
            }
        }
    }

    unlock_file(mgr_fd);
    close(mgr_fd);
    close(temp_fd);

    if (found == 1) {
        if (rename("temp", "manager_credentials") == -1) {
            perror("Error replacing manager password file");
            return -1;
        }
    } else {
        printf("Username '%s' not found in manager password file.\n", username);
        return -1;
    }

    return 0;
}

int add_employee(struct Employee emp) {
    FILE *file = fopen("employee_data.csv", "a+");
    if (file == NULL) {
        perror("Error opening employee_data.csv file");
        return -1;
    }

    int fd = fileno(file);
    if (lock_file(fd, F_WRLCK) == -1) {
        fclose(file);
        return -1;
    }

    struct Employee temp;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^\n]", temp.username, temp.name, temp.role, temp.phone, temp.salary);
        if (strcmp(temp.username, emp.username) == 0) {
            printf("Employee already exists!\n");
            unlock_file(fd);
            fclose(file);
            return -1;
        }
    }

    fprintf(file, "%s,%s,%s,%s,%s\n", emp.username, emp.name, emp.role, emp.phone, emp.salary);
    printf("Employee data saved successfully!\n");

    unlock_file(fd);
    fclose(file);

    int pwd_fd = open("employee_credentials", O_RDWR | O_CREAT | O_APPEND, 0666);
    if (pwd_fd == -1) {
        perror("Error opening employee_passwords file");
        unlock_file(fd);
        close(fd);
        return -1;
    }

    if (lock_file(pwd_fd, F_WRLCK) == -1) {
        close(pwd_fd);
        unlock_file(fd);
        close(fd);
        return -1;
    }

    char password_entry[40];
    snprintf(password_entry, sizeof(password_entry), "%s,%s\n", emp.username, "12345");
    if (write(pwd_fd, password_entry, strlen(password_entry)) == -1) {
        perror("Error writing to employee_passwords file");
        unlock_file(pwd_fd);
        unlock_file(fd);
        close(pwd_fd);
        close(fd);
        return -1;
    }

    unlock_file(pwd_fd);
    close(pwd_fd);

    return 0;
}

int update_employee_internal(char* uname, char* newval, int opt) {
    FILE *file = fopen("employee_data.csv", "r+");
    if (file == NULL) {
        perror("Error opening employee_data.csv file");
        return -1;
    }

    int fd = fileno(file);
    if (lock_file(fd, F_WRLCK) == -1) {
        fclose(file);
        return -1;
    }

    FILE *tempFile = fopen("temp.csv", "w");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        unlock_file(fd);
        fclose(file);
        return -1;
    }

    struct Employee temp;
    char buffer[256];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%[^\n]", temp.username, temp.name, temp.role, temp.phone, temp.salary);
        
        if (strcmp(temp.username, uname) == 0) {
            found = 1;
            switch (opt) {
                case 1: strncpy(temp.username, newval, sizeof(temp.username) - 1); break;
                case 2: strncpy(temp.name, newval, sizeof(temp.name) - 1); break;
                case 3: strncpy(temp.role, newval, sizeof(temp.role) - 1); break;
                case 4: strncpy(temp.phone, newval, sizeof(temp.phone) - 1); break;
                case 5: strncpy(temp.salary, newval, sizeof(temp.salary) - 1); break;
                default: printf("Invalid option\n"); break;
            }
        }
        fprintf(tempFile, "%s,%s,%s,%s,%s\n", temp.username, temp.name, temp.role, temp.phone, temp.salary);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("employee_data.csv");
        rename("temp.csv", "employee_data.csv");
    } else {
        printf("Employee not found!\n");
        remove("temp.csv");
        unlock_file(fd);
        return -1;
    }

    unlock_file(fd);
    return 0;
}
