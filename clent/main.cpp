
#include <iostream>
#include<stdio.h>
#include <winsock2.h>
#include<conio.h>
#include <windows.h>
#include <pthread.h>
#include<ctype.h>
#include <semaphore.h>
//#include <C:\\Users\\Ahmed Mostafa\\Desktop\\New folder (2)\\clent\\rlutil.h>
int isrecinving = 0;
using namespace std;
void *threadFunc(void *arg);
void *recive(void *arg);
SOCKET server;
struct Server

{
     char Serverip[20];
     char ServerName[20];
     SOCKET* sock;
};
int close = 1;
void sendFunct1(char ip[]);
void AddandCheck(char user[]);
struct users* checkuser(char ip[]);
void SetColor(int ForgC);
int getString(int pos,char* arr,char ch);
void PrintUsers();
struct Server getServer();
void gotoxy(short int x, short int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}
int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}
struct users
{
    char name[15];
    char ip[20];
    char online;
    struct users* Pnext;
}*Phead = NULL,*Ptail = NULL;
int AddNode(struct users* ptr)
{
    int returnval = 0;

    if (ptr)
    {
        ptr->Pnext = NULL;
        if (Phead == NULL)
        {
            Phead = Ptail = ptr;
        }
        else
        {
            Ptail->Pnext = ptr;
            Ptail = ptr;
        }
       returnval = 1;
    }
    return returnval;
}
struct users* getNext(struct users* p)
{
    struct users* ptr = NULL;
    if (p)
    {
      ptr = p->Pnext;
    }

    return ptr;
}
int main()

{
//char ip[] = {"192.168.1.2"};
char ip[] = {"172.16.5.193"};
//sendFunct1(ip);
SetColor(6);
pthread_t tid;

pthread_create(&tid, NULL, threadFunc, (void*)ip);
pthread_join(tid, NULL);
return 0;


}
void sendFunct1(char ip[])
{
    SOCKET sock;
     char buffer[1024] = {"this is client"};
     struct Server s = getServer();
     strcpy(buffer,s.Serverip);
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
    int len = sizeof(struct sockaddr_in);
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))<0)
        printf("no broadcast");

    Sender_addr.sin_addr.s_addr = inet_addr("172.16.255.255");
  // if(bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) <0)
   // printf("no binding");
     int state = 1;
     int i;
     memset(ip, 0, sizeof(ip));
    while(state)
    {
        //getTime();
        if(i = sendto(sock, buffer, strlen(buffer)+1, 0, (sockaddr *)&Sender_addr, sizeof(Sender_addr))>0)
        {
           // perror("borhot send: ");
          // printf("broasd cast send \n");
          //printf("massage %s \n",ip);
           memset(ip, 0, sizeof(ip));
           Sleep(1000);
           while(i = recvfrom(sock, ip, strlen(ip), 0, (sockaddr *)&Recv_addr, &len)<0)
               {


                   Sleep(100);
               }
                 printf("massage %s \n",ip);
                   if(strcmp(ip,"this is client"))
                   {

                    closesocket(sock);
                   state = 0;
                   }
            //_getch();

        }
        //printf("%d",i);


        Sleep(500);
    }
    printf("massage %s \n",ip);
}
void Connect(SOCKET* client,SOCKADDR_IN addr)
{
    int Connect = 1;
    closesocket(*client);
    SOCKET so = socket(AF_INET, SOCK_STREAM, 0);
    *client = so;
    int con;
    printf("tring to connect\n");
    int x = wherex();
    int y = wherey();
   while(Connect && close)
    {
         con = connect(*client, (SOCKADDR *)&addr, sizeof(addr));

        Sleep(100);
      if (con==0)
      {
          //
          Connect = 0;
      }
      else
      {
         //printf("Could not connect will try again later\n");
         printf(".");

         Sleep(500);

      }
      if(x+15<wherex())
      {
        gotoxy(x,y);
        printf("                                  ");
        gotoxy(x,y);
      }

    }

printf("\nconnected t \n");
}
void *threadFunc(void *arg)
{
    WSADATA WSAData;
    char* ip = (char*)arg;
    SOCKADDR_IN addr;
    pthread_t tid;
    //int* c = (int*)malloc(sizeof(int));
printf("this is sending and connecting thread");

    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr(ip); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6555);

     Connect(&server,addr);
     printf("\n press enter to send \n use .username. to send to one user \n press enter to continue \n ");
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
        {//Connect(&server,addr);
        send(server, buffer, sizeof(buffer), 0);
        }
        if(strlen(buffer)>0)
            {
                printf("%s \t",buffer);
    printf("sent \n");
            }
           // else printf("\t");

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
printf("this is recivinig thread \n");
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
        if(buffer[0] == ',')
          AddandCheck(buffer);
        else if(strlen(buffer)>0)
        printf( "%s\n",buffer);

        //sem_post(&t_2_sem);

        }
        else {

        }        isrecinving = 1;
        //Sleep(1000);


}
return arg;
}
void AddandCheck(char user[])
{
    char* word,*split;
    struct users* ptr,*check;
    int state = 1;
    int pos = 0,p,i;
    //user[0] = 0;
    //printf("%s\n",user);
    //split=strtok(user,",");

    ptr = (struct users*)malloc(sizeof(struct users));
    if(ptr)
    {
       ptr->online = user[1];
  for(i =3,pos =0;i<14;i++,pos++)
   {
      ptr->ip[pos] = user[i];
      //printf(" %c , %d ",ptr->ip[pos],pos);
   }
   //printf("\n");
   ptr->ip[pos] = 0;
      for(i =16,pos=0;user[i]!=0;i++,pos++)
   {
      ptr->name[pos] = user[i];
      //printf("%c",ptr->name[pos]);
   }
   //printf("\n");
   ptr->name[pos-1] = 0;
   //printf(",, %s ,, %s ,, %c\n",ptr->name,ptr->ip,ptr->online);
   check = checkuser(ptr->name);
   if(!check)
   {
       AddNode(ptr);
       //printf("added");
   }

   else{
    check->online = ptr->online;
    //printf("not added");
    free(ptr);
   }
    }






PrintUsers();
}
struct users* checkuser(char ip[])
{
    struct users* ptr = Phead,*retval = NULL;
    int states = 1;
    while(ptr && states)
    {
       //printf(",%s, ,%s,",ip,ptr->ip);
        if(!strcmp(ptr->name,ip))
        {
            states = 0;
            retval = ptr;
        }
        ptr = getNext(ptr);
    }
    return retval;
};

void PrintUsers()
{
    short int  x, y,mov = 2;


    x = wherex();
    y = wherey();
    struct users* ptr = Phead;
    //ptr = getNext(ptr);
    //if(ptr)
        //printf("data");
        while(!isrecinving){
    Sleep(100);
}
isrecinving = 0;
    while(ptr)
    {
        gotoxy(50, mov);
        if(ptr->online=='0')
        SetColor(4);
        else if (ptr->online =='1')
        SetColor(10);
    printf("%s ip %s",ptr->name,ptr->ip);
    mov+=1;
    ptr = getNext(ptr);
    }

    gotoxy(x,y);
    SetColor(6);
    isrecinving = 1;


}
void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
struct Server getServer()
{
     WORD wVersionRequested;
 WSADATA wsaData;
 char name[255];
 PHOSTENT hostinfo;
 wVersionRequested = MAKEWORD( 1, 1 );
 char *ip;
 struct Server server;

 if ( WSAStartup( wVersionRequested, &wsaData ) == 0 )
  if( gethostname ( name, sizeof(name)) == 0)
  {
   printf("Host name: %s\n", name);

   if((hostinfo = gethostbyname(name)) != NULL)
   {

     ip = inet_ntoa(*(
          struct in_addr *)hostinfo->h_addr_list[2]);
          strcpy(server.Serverip,ip);
          strcpy(server.ServerName,name);


    }
   }
return server;
};

