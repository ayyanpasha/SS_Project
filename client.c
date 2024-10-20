#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080



void customer(int sock){
    char buff[1024];
    int valread = read(sock, buff, 1024);
    printf("%s", buff);
    memset(buff, 0, sizeof(buff));
    char uname[1024],pwd[1024];
    scanf("%s",uname);
    scanf("%s",pwd);
    send(sock, uname, sizeof(uname), 0);
    send(sock, pwd, sizeof(pwd), 0);
    int flag;
    read(sock,&flag,sizeof(flag));
    memset(buff, 0, sizeof(buff));
    send(sock, &flag, sizeof(flag), 0);
    if(flag==1){
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
        send(sock, &flag, sizeof(flag), 0);
        int choice;
        while(1){
           valread = read(sock, buff, 1024);
            printf("%s", buff);
            memset(buff, 0, sizeof(buff));
            scanf("%d",&choice); 
            send(sock, &choice, sizeof(flag), 0);
            int nop;
            switch(choice){
                case 1: float result;
                        valread = read(sock, &result,sizeof(float));
                        printf("Your Current Balane is:-%.2f\n",result);
                        send(sock, &result,sizeof(float), 0);
                        break;
                case 2: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        float amount;
                        scanf("%f",&amount);
                        send(sock, &amount, sizeof(amount), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &amount, sizeof(amount), 0);
                        break;
                case 3: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        float amount_s;
                        scanf("%f",&amount_s);
                        send(sock, &amount_s, sizeof(amount_s), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &amount_s, sizeof(amount_s), 0);
                        break;
                case 4: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char recv[20];
                        scanf("%s",recv);
                        send(sock,recv,strlen(recv)+1,0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        float amount_w;
                        scanf("%f",&amount_w);
                        send(sock, &amount_w, sizeof(amount_w), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &amount, sizeof(amount), 0);
                        break;
                case 5: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        float loan_amount;
                        scanf("%f",&loan_amount);
                        send(sock,&loan_amount,sizeof(float), 0);
                        read(sock,&nop,sizeof(int));
                        char loan_type[20];
                        scanf("%s",loan_type);
                        send(sock,loan_type,strlen(loan_type)+1, 0);
                        read(sock,&nop,sizeof(int));
                        int tenure;
                        scanf("%d",&tenure);
                        send(sock,&tenure,sizeof(int), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock,&tenure,sizeof(int),0);
                        break;
                case 7: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char newpwd[20];
                        scanf("%s",newpwd);
                        send(sock,newpwd, strlen(newpwd)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &nop, sizeof(nop), 0);
                        break;
                case 8: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char feedback[100];
                        getchar();
                        fgets(feedback, sizeof(feedback), stdin);  // Read input including spaces
                        feedback[strcspn(feedback, "\n")] = 0;  
                        send(sock,feedback, strlen(feedback)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 9: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 6: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 10:valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        //send(sock, &choice, sizeof(flag), 0);
                        break;
                default: printf("Invalid Input\n");

            }
            if(choice==10){
                break;
            }
        }
    }
    else{
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
    }
}

void employee(int sock){
    char buff[1024];
    int valread = read(sock, buff, 1024);
    printf("%s", buff);
    memset(buff, 0, sizeof(buff));
    char uname[1024],pwd[1024];
    scanf("%s",uname);
    scanf("%s",pwd);
    send(sock, uname, sizeof(uname), 0);
    send(sock, pwd, sizeof(pwd), 0);
    int flag;
    read(sock,&flag,sizeof(flag));
    memset(buff, 0, sizeof(buff));
    send(sock, &flag, sizeof(flag), 0);
    if(flag==1){
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
        send(sock, &flag, sizeof(flag), 0);
        int choice;
        while(1){
           valread = read(sock, buff, 1024);
            printf("%s", buff);
            memset(buff, 0, sizeof(buff));
            scanf("%d",&choice); 
            send(sock, &choice, sizeof(flag), 0);
            switch(choice){
                case 1: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char username[20],name[100],account_number[20],phone[15];
                        float balance;
                        int nop;
                        scanf("%s",username);
                        scanf("%s",account_number);
                        scanf("%s",name);
                        scanf("%s",phone);
                        scanf("%f",&balance);
                        send(sock,username, strlen(username)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,account_number, strlen(account_number)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,name, strlen(name)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,phone, strlen(phone)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,&balance, sizeof(float), 0);
                        read(sock, &nop, sizeof(int));
                        printf("sent");
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 2: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char uname[20];
                        scanf("%s",uname);
                        send(sock, uname, strlen(uname)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        int opt;
                        scanf("%d",&opt);
                        send(sock, &opt, sizeof(opt), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char updated_value[100];
                        scanf("%s",updated_value);
                        send(sock, updated_value, strlen(updated_value)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &opt, sizeof(opt), 0);
                        break;
                // case 3: valread = read(sock, buff, 1024);
                //         printf("%s", buff);
                //         memset(buff, 0, sizeof(buff));
                //         send(sock, &choice, sizeof(flag), 0);
                //         break;
                case 3: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        int loan_id;
                        scanf("%d",&loan_id);
                        send(sock,&loan_id,sizeof(int),0);
                        char decision[10];
                        scanf("%s",decision);
                        send(sock,decision,strlen(decision)+1,0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 4: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 5: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char cuname[20];
                        scanf("%s",cuname);
                        send(sock, cuname, strlen(cuname)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 6: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char newpwd[20];
                        scanf("%s",newpwd);
                        send(sock,newpwd, strlen(newpwd)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &nop, sizeof(nop), 0);
                        break;
                case 7: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                default: printf("Invalid Choice\n");

            }
            if(choice==7){
                break;
            }
        }
    }
    else{
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
    }
}

void manager(int sock){
    char buff[1024];
    int valread = read(sock, buff, 1024);
    printf("%s", buff);
    memset(buff, 0, sizeof(buff));
    char uname[1024],pwd[1024];
    scanf("%s",uname);
    scanf("%s",pwd);
    send(sock, uname, sizeof(uname), 0);
    send(sock, pwd, sizeof(pwd), 0);
    int flag;
    read(sock,&flag,sizeof(flag));
    memset(buff, 0, sizeof(buff));
    send(sock, &flag, sizeof(flag), 0);
    if(flag==1){
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
        send(sock, &flag, sizeof(flag), 0);
        int choice;
        int nop;
        while(1){
           valread = read(sock, buff, 1024);
            printf("%s", buff);
            memset(buff, 0, sizeof(buff));
            scanf("%d",&choice); 
            send(sock, &choice, sizeof(flag), 0);
            switch(choice){
                // case 1: valread = read(sock, buff, 1024);
                //         printf("%s", buff);
                //         memset(buff, 0, sizeof(buff));
                //         send(sock, &choice, sizeof(flag), 0);
                //         break;
                case 1: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        int loan_id;
                        scanf("%d",&loan_id);
                        send(sock, &loan_id, sizeof(int), 0);
                        char emp_id[20];
                        scanf("%s",emp_id);
                        send(sock,emp_id,strlen(emp_id)+1,0);
                         valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 2: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 3: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char newpwd[20];
                        scanf("%s",newpwd);
                        send(sock,newpwd, strlen(newpwd)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &nop, sizeof(nop), 0);
                        break;
                case 4: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;

            }
            if(choice==4){
                break;
            }
        }
    }
    else{
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
    }
}

void admin(int sock){
    char buff[1024];
    int valread = read(sock, buff, 1024);
    printf("%s", buff);
    memset(buff, 0, sizeof(buff));
    char uname[1024],pwd[1024];
    scanf("%s",uname);
    scanf("%s",pwd);
    send(sock, uname, sizeof(uname), 0);
    send(sock, pwd, sizeof(pwd), 0);
    int flag;
    read(sock,&flag,sizeof(flag));
    memset(buff, 0, sizeof(buff));
    send(sock, &flag, sizeof(flag), 0);
    if(flag==1){
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
        send(sock, &flag, sizeof(flag), 0);
        int choice;
        while(1){
            char uname[20];
             int opt;
             char updated_value[100];
           valread = read(sock, buff, 1024);
            printf("%s", buff);
            memset(buff, 0, sizeof(buff));
            scanf("%d",&choice); 
            send(sock, &choice, sizeof(flag), 0);
            switch(choice){
                case 1: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char username[20],name[100],role[20],salary[10],phone[15];
                        int nop;
                        scanf("%s",username);
                        scanf("%s",name);
                        scanf("%s",role);
                        scanf("%s",phone);
                        scanf("%s",salary);
                        send(sock,username, strlen(username)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,name, strlen(name)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,role, strlen(role)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,phone, strlen(phone)+1, 0);
                        read(sock, &nop, sizeof(int));
                        send(sock,salary, strlen(salary)+1, 0);
                        read(sock, &nop, sizeof(int));
                        //printf("sent");
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;
                case 3: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        //char uname[20];
                        scanf("%s",uname);
                        send(sock, uname, strlen(uname)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                       
                        scanf("%d",&opt);
                        send(sock, &opt, sizeof(opt), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        // char updated_value[100];
                        scanf("%s",updated_value);
                        send(sock, updated_value, strlen(updated_value)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &opt, sizeof(opt), 0);
                        break;
                case 2: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char uname[20];
                        scanf("%s",uname);
                        send(sock, uname, strlen(uname)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        // int opt;
                        scanf("%d",&opt);
                        send(sock, &opt, sizeof(opt), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        // char updated_value[100];
                        scanf("%s",updated_value);
                        send(sock, updated_value, strlen(updated_value)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &opt, sizeof(opt), 0);
                        break;
                case 4: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        // int opt;
                        scanf("%d",&opt);
                        send(sock, &opt, sizeof(opt), 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        //char uname[20];
                        scanf("%s",uname);
                        send(sock, uname, strlen(uname)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &opt, sizeof(opt), 0);
                        break;
                case 5: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        char newpwd[20];
                        scanf("%s",newpwd);
                        send(sock,newpwd, strlen(newpwd)+1, 0);
                        valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &opt, sizeof(opt), 0);
                        break;
                case 6: valread = read(sock, buff, 1024);
                        printf("%s", buff);
                        memset(buff, 0, sizeof(buff));
                        send(sock, &choice, sizeof(flag), 0);
                        break;

            }
            if(choice==6){
                break;
            }
        }
    }
    else{
        valread = read(sock, buff, 1024);
        printf("%s", buff);
        memset(buff, 0, sizeof(buff)); 
    }
}


void logout(int sock){
    char buff[1024] = {0};
    int valread = read(sock, buff, 1024);
    printf("%s", buff);
    memset(buff, 0, sizeof(buff));
}

// void otherwise(int sock){
//     char buff[1024] = {0};
//     int valread = read(sock, buff, 1024);
//     printf("%s", buff);
//     memset(buff, 0, sizeof(buff));
// }

int main() {
    int  valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    int choice;
    int sock;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Setup server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Read menu from server
   
    while(1){
    
    valread = read(sock, buffer, 1024);
    printf("%s", buffer);
    memset(buffer, 0, sizeof(buffer));


    // Enter choice and numbers
    scanf("%d", &choice);
    send(sock, &choice, sizeof(int), 0);

    switch(choice){
        case 1: customer(sock);
                send(sock, &choice, sizeof(int), 0);
                break;
        case 2: employee(sock);
                send(sock, &choice, sizeof(int), 0);
                break;
        case 3: manager(sock);
                send(sock, &choice, sizeof(int), 0);
                break;
        case 4: admin(sock);
                send(sock, &choice, sizeof(int), 0);
                break;
        case 5: logout(sock);
                send(sock, &choice, sizeof(int), 0);
                break;
        default: printf("Invalid Chpice\n");
    }

    if(choice==5){
        break;
    }

    }

    close(sock);

    return 0;
}
