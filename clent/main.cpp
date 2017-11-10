
#include <iostream>
#include<stdio.h>
#include <winsock2.h>
#include<conio.h>
#include <pthread.h>
#include<ctype.h>
#include <semaphore.h>

int isrecinving = 0;
using namespace std;
void *threadFunc(void *arg);
void *recive(void *arg);
SOCKET server;
int close = 1;
void sendFunct1(void *param);
int main()

{

sendFunct1(NULL);

    pthread_t tid;

pthread_create(&tid, NULL, threadFunc, NULL);
pthread_join(tid, NULL);
return 0;


}
void sendFunct1(void *param)
{
    SOCKET sock;
     char buffer[1024] = {"this is client"};
     printf("%s \n",buffer);
     char broadcast = 'a';
    struct sockaddr_in Recv_addr;
    struct sockaddr_in Sender_addr;
       Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(5000);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;
    Sender_addr.sin_family = AF_INET;
    Sender_addr.sin_port = htons(5000);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))<0)
        printf("no broadcast");
    Sender_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    if(bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) <0)
        printf("no binding");

    while(1)
    {
        //getTime();
        if(sendto(sock, buffer, strlen(buffer)+1, 0, (sockaddr *)&Sender_addr, sizeof(Sender_addr)) < 0)
        {
           // perror("borhot send: ");
           printf("broasd cast send");
            //_getch();
            closesocket(sock);
        }
        Sleep(500);
    }
}
void Connect(SOCKET* client,SOCKADDR_IN addr)
{
    int Connect = 1;
    closesocket(*client);
    SOCKET so = socket(AF_INET, SOCK_STREAM, 0);
    *client = so;
    int con;

   while(Connect && close)
    {
         con = connect(*client, (SOCKADDR *)&addr, sizeof(addr));
        printf("tring to connect\n");
        Sleep(100);
      if (con==0)
      {
          printf("connected t \n");
          Connect = 0;
      }
      else
      {
         printf("Could not connect will try again later\n");
         Sleep(3000);

      }

    }


}
void *threadFunc(void *arg)
{
    WSADATA WSAData;

    SOCKADDR_IN addr;
    pthread_t tid;
    //int* c = (int*)malloc(sizeof(int));


    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("192.168.1.2"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

     Connect(&server,addr);
     int state = 12;
   pthread_create(&tid, NULL, recive, (void*)&state );
//pthread_mutex_unlock(&mutex);

    char buffer[1024]= {0};

    //sem_wait(&t_1_sem);
    while (state && close)
    {
    isrecinving = 1;
memset(buffer, 0, sizeof(buffer));

        int control = 0,i=0;

       while (state>11 && close){
         char ch = getche();


        if (ch ==0)
           {
             ch = getch();
             control = 1;
           }
        else if (!isprint(ch))
        {
            control = 1;
        }
        else
          {

              //printf("%d",i);
              buffer[i] = ch;
              i++;
              if(i == 1)
              isrecinving = 0;




            //sem_wait(&t_2_sem);

          }
        if (control)
        switch(ch)
        {
            case 27: // esc
				state = 0;
				break;
			case 13://enter
                state = 10;
                buffer[i] = 0;

                isrecinving = 1;
                //sem_post(&t_2_sem);
				break;
        }

        }

        //sem_post(&t_2_sem);
    if (state >0){

    int sucss = send(server, buffer, sizeof(buffer), 0);
    if (sucss<0)
        {Connect(&server,addr);
        send(server, buffer, sizeof(buffer), 0);
        }
    printf("%s \t",buffer);
    printf("sent \n");
    state = 13;

    }
    }



    closesocket(server);
    WSACleanup();
   // cout << "Socket closed." << endl << endl;
    return arg;

}

void *recive(void *arg)
{

   // int* endt = (int*)arg;
    SOCKET ptr = server;
    //free(arg);
    int state = 1;
 //   int frist = 1;
    char buffer[1024];
printf("\nthis is listing %d",ptr);
while(state && close){




         // remove all white spaces and put -
        //replace scanf with getch
        //put a cheak for esc bottom to exit
        memset(buffer, 0, sizeof(buffer));


        int r = recv(ptr, buffer, sizeof(buffer), 0);
        if (buffer[0] == 27)
           {
              printf("server is closed closing the system");
             // *endt = 0;
             close = 0;
             closesocket(server);
             exit(1);
           }

        //printf(" %d \t",ptr);
        if(r>0){
while(!isrecinving){
    Sleep(100);
}
        //sem_wait(&t_2_sem);

        printf( "%s\n",buffer);

        //sem_post(&t_2_sem);

        }
        else {

        }
        isrecinving = 1;
        //Sleep(1000);


}
return arg;
}
