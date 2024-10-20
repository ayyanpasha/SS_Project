


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "handler.h"
#include "customer.h"
#include "employee.h"
#include "manager.h"
#include "admin.h"
#include "utils.h"
#include "loan.h"



void customer(int sock){
   
        char* a = "\tHELLO CUSTOMER\n ENTER USERNAME AND PASSWORD TO LOGIN:-\n";
        int temp;
        send(sock, a, strlen(a), 0);
        char uname[1024],pwd[1024];
        read(sock, uname, 1024);
        read(sock,pwd,1024);
        int flag = authenticate_user(uname,pwd,1);
        send(sock,&flag,sizeof(int),0);
        read(sock,&temp,sizeof(temp));
        if(flag==1){
            a = "Login Succesfull\n";
            send(sock,a,strlen(a),0);
            read(sock,&temp,sizeof(temp));
            int opt =0;
            int nop =1;
            while(1){
            a = "1.View Account Balance\n2.Deposit Money\n3.Withdraw Money\n4.Transfer Funds\n5.Apply for a Loan\n6.Check Loan Status\n7.Change Password\n8.Adding Feedback\n9.View Transaction History\n10.Logout\n Enter Your Choice:-\n";
            send(sock,a,strlen(a),0);
            read(sock,&opt,sizeof(opt));
            
            int status;
            switch(opt){
                case 1: float balance = check_balance(uname);
                        send(sock,&balance,sizeof(float),0);
                        int nop;
                        read(sock,&nop,sizeof(float));
                        break;
                case 2: a = "Enter Amount To Deposit:-\n";
                        send(sock,a,strlen(a),0);
                        float amount;
                        read(sock,&amount,sizeof(amount));
                        status = deposit_balance(whoami,amount);
                        if(status==0){
                            a = "Deposit Successfull\n";
                        }
                        else{
                             a = "Deposit Not Successfull\n";
                        }
                        send(sock,a,strlen(a),0);
                        read(sock,&amount,sizeof(amount));
                        break;        
                case 3: a = "Enter Amount To Withdraw:-\n";
                        send(sock,a,strlen(a),0);
                        float amount_w;
                        read(sock,&amount_w,sizeof(amount_w));
                        status = deposit_balance(whoami,-1*amount_w);
                        if(status==0){
                            a = "Withdraw Successfull\n";
                        }
                        else{
                             a = "Withdraw Not Successfull\n";
                        }
                        send(sock,a,strlen(a),0);
                        read(sock,&amount_w,sizeof(amount_w));
                        break;
                case 4: a = "Enter recievers username:-\n";
                        send(sock,a,strlen(a),0);
                        char recv[20];
                        read(sock,recv,sizeof(recv));
                        a = "Enter amount:-\n";
                        send(sock,a,strlen(a),0);
                        float amount_s;
                        read(sock,&amount_s,sizeof(amount_s));
                        status = transfer_funds(whoami,recv,amount_s);
                        if(status==0){
                            a = "Transfer Successfull\n";
                        }
                        else{
                             a = "Transfer Not Successfull\n";
                        }
                        send(sock,a,strlen(a),0);
                        read(sock,&amount_s,sizeof(amount_s));
                        break;
                case 5: a = "Welocme to the loan Portal:\nEnter the following detail\n1.Loan Amount\n2.Loan Type\n3.Tenure\n";
                        send(sock,a,strlen(a),0);
                        float loan_amount;
                        char loan_type[20];
                        int tenure;
                        read(sock,&loan_amount,sizeof(float));
                        send(sock,&nop,sizeof(int),0);
                        read(sock,loan_type,sizeof(loan_type));
                        send(sock,&nop,sizeof(int),0);
                        read(sock,&tenure,sizeof(int));
                        int flag = apply_for_loan(whoami,loan_amount,loan_type,tenure);
                        if(flag==0){
                             a = "Loan Application Successfull\n";   
                        }
                        else{
                            a = "Loan Application Not Successfull\n";
                        }
                        send(sock,a,strlen(a)+1,0);
                        read(sock,&nop,sizeof(int));
                        break;
                case 7: a = "Enter new Password:\n";
                        send(sock,a,strlen(a),0);
                        char newpwd[20];
                        read(sock,newpwd,sizeof(newpwd));
                        int flag_o = change_password_user(whoami,newpwd,1);
                        if(flag_o==0){
                            a = "Password Change Successfull\n";
                                send(sock,a,strlen(a),0);    
                        }
                        else{
                                a = "Password Change Not Successfull:\n";
                                send(sock,a,strlen(a),0);
                        }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 8: a = "Enter Your Feedback\n";
                        send(sock,a,strlen(a),0);
                        char feedback[100];
                        read(sock,feedback,sizeof(feedback));
                        submit_feedback(whoami,feedback);
                        a = "Feedback Submitted\n";
                        send(sock,a,strlen(a),0);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 9: retrieve_transaction_history(sock,whoami);
                        read(sock,&temp,sizeof(temp));
                        break;
                case 10: a = "Logging Out\n";
                        send(sock,a,strlen(a),0);
                        break;
                case 6: char* loan_status = view_customer_loans(whoami);
                        send(sock,loan_status,strlen(loan_status)+1,0);
                        read(sock,&temp,sizeof(temp));
                        break;
                default: 
             }
             if(opt==10){
                break;
            }

            }
            
        }
        else{
            a = "Login Not Succesfull\n";
            send(sock,a,strlen(a),0);
        }
        return;
}

void employee(int sock){
        char* a = "\tHELLO EMPLOYEE\n ENTER USERNAME AND PASSWORD TO LOGIN:-\n";
        int temp;
        send(sock, a, strlen(a), 0);
        char uname[1024],pwd[1024];
        read(sock, uname, 1024);
        read(sock,pwd,1024);
        int flag = authenticate_user(uname,pwd,2);
        send(sock,&flag,sizeof(int),0);
        read(sock,&temp,sizeof(temp));
        if(flag==1){
            a = "Login Succesfull\n";
            send(sock,a,strlen(a),0);
            read(sock,&temp,sizeof(temp));
            int opt =0;
            while(1){
            a = "1.Add New Customer\n2.Modify Customer Detail\n3.Approve/Reject Loan\n4.View Assigned Loan Application\n5.View Customer Transaction\n6.Change Password\n7.Logout\n Enter Your Choice:-\n";
            send(sock,a,strlen(a),0);
            read(sock,&opt,sizeof(opt));
            
            int status;
            int nop;
            switch(opt){
                case 1: add_cust(sock);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 2: modify_customer_employee(sock);
                        read(sock,&nop,sizeof(nop));
                        break;        
                // case 3: process_loan(sock);
                //         a = "function call succesfull\n";
                //         send(sock,a,strlen(a),0);
                //         read(sock,&nop,sizeof(nop));
                //         break;
                case 3: //approve_loan(sock);
                        a = "Enter loan id and decision(approve/reject)\n";
                        send(sock,a,strlen(a),0);
                        int loan_id;
                        read(sock,&loan_id,sizeof(int));
                        char decision[10];
                        read(sock,decision,sizeof(decision));
                        flag = approve_or_reject_loan(loan_id,whoami,decision);
                        if(flag==0){
                                a = "Operation Successfull\n";
                        }
                        else{
                                a = "Operation Not Successfull\n";
                        }
                        send(sock,a,strlen(a),0);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 4: char *assigned_loan =view_loans_assigned_to_employee(whoami);
                        // a = "function call succesfull\n";
                        send(sock,assigned_loan,strlen(assigned_loan)+1,0);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 5: //view_transaction(sock);
                        a = "Enter username of customer:-\n";
                        send(sock,a,strlen(a)+1,0);
                        char cuname[20];
                        read(sock,cuname,sizeof(cuname));
                        retrieve_transaction_history(sock,cuname);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 6: a = "Enter new Password:\n";
                        send(sock,a,strlen(a),0);
                        char newpwd[20];
                        read(sock,newpwd,sizeof(newpwd));
                        int flag = change_password_user(whoami,newpwd,2);
                        if(flag==0){
                            a = "Password Change Successfull\n";
                                send(sock,a,strlen(a),0);    
                        }
                        else{
                                a = "Password Change Not Successfull:\n";
                                send(sock,a,strlen(a),0);
                        }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 7: //logout_emp(sock);
                        a = "Logging Out\n\n";
                        send(sock,a,strlen(a),0);
                        read(sock,&nop,sizeof(nop));
                        break;
             }
             if(opt==7){
                break;
            }

            }
            
        }
        else{
            a = "Login Not Succesfull\n";
            send(sock,a,strlen(a),0);
        }
        return;
}

void manager(int sock){
        char* a = "\tHELLO MANAGER\n ENTER USERNAME AND PASSWORD TO LOGIN:-\n";
        int temp;
        send(sock, a, strlen(a), 0);
        char uname[1024],pwd[1024];
        read(sock, uname, 1024);
        read(sock,pwd,1024);
        int flag = authenticate_user(uname,pwd,3);
        send(sock,&flag,sizeof(int),0);
        read(sock,&temp,sizeof(temp));
        int nop;
        if(flag==1){
            a = "Login Succesfull\n";
            send(sock,a,strlen(a),0);
            read(sock,&temp,sizeof(temp));
            int opt =0;
            while(1){
            a = "1.Assign Loan Application Processes to Employees\n2.Review Customer Feedback\n3.Change Password\n4.Logout\n Enter Your Choice:-\n";
            send(sock,a,strlen(a),0);
            read(sock,&opt,sizeof(opt));
            
            int status;
            switch(opt){
                case 1: //loan_assignment(sock);
                        char* unassigned_loans = view_unassigned_loans();
                        send(sock,unassigned_loans,strlen(unassigned_loans)+1,0);
                        read(sock,&opt,sizeof(opt));
                        a = "Enter loan id and epmloyye id to assign";
                        send(sock,a,strlen(a),0);
                        int loan_id;
                        read(sock,&loan_id,sizeof(int));
                        char emp_id[20];
                        read(sock,emp_id,sizeof(emp_id));
                        flag = assign_loan_to_employee(loan_id,emp_id);
                        if(flag==0){
                                a = "Assignment Successfull";
                        }
                        else{
                                a = "Assignment Not Successfull";
                        }
                        send(sock,a,strlen(a),0);
                        read(sock,&opt,sizeof(opt));
                        break;        
                case 2: //review_feedback(sock);
                        char getfeedback[1024];
                        get_feedbacks(getfeedback);
                        // a = "function call succesfull\n";
                        send(sock,getfeedback,strlen(getfeedback)+1,0);
                        read(sock,&nop,sizeof(nop));
                        break;
                case 3: a = "Enter new Password:\n";
                        send(sock,a,strlen(a),0);
                        char newpwd[20];
                        read(sock,newpwd,sizeof(newpwd));
                        flag = change_password_user(whoami,newpwd,3);
                        if(flag==0){
                            a = "Password Change Successfull\n";
                                send(sock,a,strlen(a),0);    
                        }
                        else{
                                a = "Password Change Not Successfull:\n";
                                send(sock,a,strlen(a),0);
                        }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 4: //logout_manager(sock);
                        a = "Logging Out\n";
                        send(sock,a,strlen(a),0);
                        read(sock,&opt,sizeof(opt));
                        break;
                
             }
             if(opt==4){
                break;
            }

            }
            
        }
        else{
            a = "Login Not Succesfull\n";
            send(sock,a,strlen(a),0);
        }
        return;
    }

void admin(int sock){
        char* a = "\tHELLO ADMIN\n ENTER USERNAME AND PASSWORD TO LOGIN:-\n";
        int temp;
        send(sock, a, strlen(a), 0);
        char uname[1024],pwd[1024];
        read(sock, uname, 1024);
        read(sock,pwd,1024);
        int flag = authenticate_user(uname,pwd,4);
        send(sock,&flag,sizeof(int),0);
        read(sock,&temp,sizeof(temp));
        if(flag==1){
            a = "Login Succesfull\n";
            send(sock,a,strlen(a),0);
            read(sock,&temp,sizeof(temp));
            int opt =0;
            struct Employee emp;
            char menu[200];
            char msg[200];
            char uname[20];
            while(1){
            a = "1.Add New Employee\n2.Modify Customer Detail\n3.Modify Employee Detail\n4.Manage Roles\n5.Change Password\n6.Logout\n Enter Your Choice:-\n";
            send(sock,a,strlen(a),0);
            read(sock,&opt,sizeof(opt));
            
            int status;
            int nop;
            switch(opt){
                case 1: //add_employee(sock);
                        printf("I was called\n");
                        //struct Employee emp;
                        strcpy(menu,"\t--Enter the detail of employee to add in following sequence:-\n1.Username:\n2.Name of Employee:\n3.Role(Manager/Not a Manager):\n4.Contact Number:\n5.Salary:-\n");
                        send(sock, menu, strlen(menu), 0);
                        int num=0;
                        read(sock,emp.username,sizeof(emp.username));
                        send(sock,&num,sizeof(int),0); 
                        read(sock,emp.name,sizeof(emp.name));
                        send(sock,&num,sizeof(int),0); 
                        read(sock,emp.role,sizeof(emp.role));
                        send(sock,&num,sizeof(int),0); 
                        read(sock,emp.phone,sizeof(emp.phone));
                        send(sock,&num,sizeof(int),0); 
                        read(sock,emp.salary,sizeof(emp.salary));
                        send(sock,&num,sizeof(int),0); 
                        printf("%s\n",emp.salary);
                        printf("%s\n",emp.username);
                        printf("%s\n",emp.name);
                        printf("%s\n",emp.role);
                        printf("%s\n",emp.phone);

                        int flag = add_employee(emp);
                        if(flag==0){
                                strcpy(msg,"Employee added Successfully\n");
                                send(sock, msg, strlen(msg), 0);
                        }
                        else{
                               strcpy(msg,"Employee Not Added\n");
                                send(sock, msg, strlen(msg), 0);
                        }
                        read(sock,&opt,sizeof(opt));
                        break;
                case 2: modify_customer_employee(sock);
                        read(sock,&nop,sizeof(nop));
                        break;        
                case 3: //update_employee(sock);
                        printf("I was called\n");
                        //struct Employee emp;
                        //char uname[20];
                        strcpy(msg,"Enter username for which value is to be updated:-\n");
                        send(sock,msg,strlen(msg),0);
                        read(sock,uname,sizeof(uname));
                        printf("updating value for %s\n",uname);
                        strcpy(menu,"\t--choose the field of employee to update in following sequence:-\n1.Name of Employee:\n2.Contact Number:-\n3.Salary\nEnter 1-3:-");
                        send(sock, menu, strlen(menu), 0);
                        int menu_opt;
                        read(sock,&menu_opt,sizeof(int));
                        int flag_curr;
                        switch(menu_opt){
                                case 1: strcpy(msg,"Enter new name:-");
                                        send(sock,msg,strlen(msg),0);
                                        read(sock,emp.name,sizeof(emp.name));
                                        flag_curr = update_employee_internal(uname,emp.name,2);
                                        if(flag_curr==0){
                                        strcpy(msg,"Update Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                                        else{
                                        strcpy(msg,"Update Not Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                                        break;
                                case 2: strcpy(msg,"Enter new phone:-");
                                        send(sock,msg,strlen(msg),0);
                                        read(sock,emp.phone,sizeof(emp.phone));
                                        flag_curr = update_employee_internal(uname,emp.phone,4);
                                        if(flag_curr==0){
                                        strcpy(msg,"Update Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                                        else{
                                        strcpy(msg,"Update Not Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                                        break;
                                case 3: strcpy(msg,"Enter new salary:-");
                                        send(sock,msg,strlen(msg),0);
                                        read(sock,emp.salary,sizeof(emp.salary));
                                        flag_curr = update_employee_internal(uname,emp.salary,5);
                                        if(flag_curr==0){
                                        strcpy(msg,"Update Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                                        else{
                                        strcpy(msg,"Update Not Successfull");
                                        send(sock,msg,strlen(msg),0);
                                        }
                         }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 4: //manage_role(sock);
                        //printf("%s\n",curr_admin);
                        strcpy(msg,"\tChoose one of the option:-\n1.Promote to Manager\n2.Demote from Manager\nEnter Your Choice:-");
                        send(sock, msg, strlen(msg), 0);
                        int choice;
                        read(sock,&choice,sizeof(int));
                        //char uname[20];
                        printf("%d\n",choice);
                        strcpy(msg,"Enter the username:-\n");
                        send(sock, msg, strlen(msg), 0);
                        read(sock,uname,sizeof(uname));
                        //int flag;
                        if(choice==1){
                                flag = add_manager_role(uname);
                        }
                        else{
                                flag = remove_manager_role(uname);
                        }
                        
                        if(flag==0){
                                strcpy(msg,"Update Successfull\n");
                                send(sock, msg, strlen(msg), 0);
                        }
                        else{
                                strcpy(msg,"Update Not Successfull\n");
                                send(sock, msg, strlen(msg), 0);
                        }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 5: //change_password_admin(sock);
                        a = "Enter new Password:\n";
                        send(sock,a,strlen(a),0);
                        char newpwd[20];
                        read(sock,newpwd,sizeof(newpwd));
                        flag = change_password_user(whoami,newpwd,4);
                        if(flag==0){
                            a = "Password Change Successfull\n";
                                send(sock,a,strlen(a),0);    
                        }
                        else{
                                a = "Password Change Not Successfull:\n";
                                send(sock,a,strlen(a),0);
                        }
                        read(sock,&nop,sizeof(nop));
                        break;
                case 6:  a = "Logging Out\n";
                        send(sock,a,strlen(a),0);
                        read(sock,&opt,sizeof(opt));
                        break;
             }
             if(opt==6){
                break;
            }

            }
            
        }
        else{
            a = "Login Not Succesfull\n";
            send(sock,a,strlen(a),0);
        }
        return;
    }

void logout(int sock){
        char msg[] = "bye bye :)\n";
        send(sock, msg, strlen(msg), 0);
        return;
}

void otherwise(int sock){
        char msg[] = "Invalid Choice\n";
        send(sock, msg, strlen(msg), 0);
        return;
}

void *handle_client(void *socket_desc) {
    int sock = *(int *)socket_desc;
    int choice=0;
    char menu[] = "\t\t--LOGIN--\n1. Customer\n2. Employye\n3. Manager\n4. Admin\n5.Exit\n Enter Your Choice: \n";
    char buffer[1024] = {0};
    int temp;
   
    while(1){
    
    // Send menu to client
    send(sock, menu, strlen(menu), 0);

    // Read client choice
    read(sock, &choice, sizeof(int));

    switch(choice){
        case 1: customer(sock);
                read(sock, &temp, sizeof(int));
                break;
        case 2: employee(sock);
                read(sock, &temp, sizeof(int));
                break;
        case 3: manager(sock);
                read(sock, &temp, sizeof(int));
                break;
        case 4: admin(sock);
                read(sock, &temp, sizeof(int));
                break;
        case 5: logout(sock);
                read(sock, &temp, sizeof(int));
                break;
        
                
    }
        if(choice==5){
            break;
        }
    }


    close(sock);
    free(socket_desc);

    return 0;
}

