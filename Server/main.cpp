#include <winsock2.h>
#include <iphlpapi.h>
 #include <stdlib.h>
 #include<stdio.h>
#include<conio.h>


#include <pthread.h>


using namespace std;
struct Massage
{
  char Massage[1024];
  char client[10];
  struct Massage *Pnext;

}*MasHead,*MasTail;
struct Server

{
     char Serverip[20];
     char ServerName[20];
     SOCKET* sock;
};
struct AllSocket
{
    SOCKET* lising;
    SOCKET* broadcasting;
};
struct node{
    unsigned int socket;
    char ip[15];
    int online ;
    char name[10];
    pthread_t *thread;
    struct node* Pnext;
}*Phead = NULL,*Ptail = NULL;

struct pass
{
    pthread_t* thread;
    char ip[15];
    unsigned int socket;
};
int AddNode(struct node* ptr)
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
int AddMassage(struct Massage* ptr)
{
    int returnval = 0;

    if (ptr)
    {
        ptr->Pnext = NULL;
        if (MasHead == NULL)
        {
            MasHead = MasTail = ptr;
        }
        else
        {
            MasTail->Pnext = ptr;
            MasTail = ptr;
        }
       returnval = 1;
    }
    return returnval;
}
struct node* getNext(struct node* p)
{
    struct node* ptr = NULL;
    if (p)
    {
      ptr = p->Pnext;
    }

    return ptr;
}
struct Massage* getNextMassage(struct Massage* p)
{
    struct Massage *ptr = NULL;
    if (p)
    {
      ptr = p->Pnext;
    }

    return ptr;
}
void DeleteMassage(Massage* ptr)
{
    Massage *p,*next;
    p = MasHead;
    if(MasHead == NULL)
    {
        return;
    }
    else
    {
        if(p = ptr)
    {
       if(p->Pnext !=NULL)
       {
           MasHead = MasHead->Pnext;
       }
       else{
        MasHead = MasTail = NULL;
       }
        free(ptr);
    }
    else {
        while(p->Pnext!=NULL && p->Pnext!= ptr)
    {
        p = getNextMassage(p);
    }
    if(ptr->Pnext!=NULL)
    p->Pnext = ptr->Pnext;
    else MasTail = p;
    free(ptr);
    }
    }


}
char close = 0;
void ReleaseAssit()
{
    char buffer[2]  = {27};
    struct node* ptr = Phead;
   struct Massage* p = MasHead;
    while (ptr)
    {
        send(ptr->socket,buffer,sizeof(buffer),0);
        closesocket(ptr->socket);
       //  printf("cleint %s is closed",ptr->name);
        Sleep(10);

        ptr = getNext(ptr);
        free(Phead);
        Phead = ptr;

   }
   while(p)
   {
     p = getNextMassage(p);
     free(MasHead);
     MasHead = p;
   }
}
void *threadFunc(void *arg);
void *atuh(void *arg);void *Closer(void *arg);
struct IPv4  getMyIP();
struct Server getServer();
void* recvFunct(void *param);
int main()
{
    pthread_t tid;
    pthread_t closer;
    pthread_t broadcaster;
    SOCKET server;
    SOCKET broadcasting;
    struct AllSocket All;
    struct Server *se = (struct Server*)malloc(sizeof(struct Server));
     *se = getServer();
     All.broadcasting = &server;
     All.lising = &broadcasting;
    printf(" %s %s",se->Serverip,se->ServerName);
    se->sock = &broadcasting;

//pthread_create(&broadcaster, NULL, recvFunct, (void*)se);

pthread_create(&tid, NULL, atuh, (void*)&server);
pthread_create(&closer, NULL, Closer, (void*)&All);
pthread_join(tid, NULL);
pthread_join(closer,NULL);
 //   pthread_join(broadcaster,NULL);



return 0;
}
void* recvFunct(void *param)
{
   // SOCKET* sock;
    struct Server se = *((struct Server*)param);
    free(param);
    printf("this is broadcast recever\n");
     char buffer[1024] = {"this is server"};
     printf("%s \n",buffer);
     char broadcast = 'a';
    struct sockaddr_in Recv_addr;
    struct sockaddr_in client;
    struct sockaddr_in Sender_addr;
       Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(5000);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;
    int len = sizeof(struct sockaddr_in);
    Sender_addr.sin_family = AF_INET;
    Sender_addr.sin_port = htons(5000);
    client.sin_family = AF_INET;
    client.sin_port = htons(5001);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    *se.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(setsockopt(*se.sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast))<0)
        printf("no broadcast");
    Sender_addr.sin_addr.s_addr = inet_addr("172.16.255.255");
    if(bind(*se.sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) <0)
       printf("no binding");
        int state  = 1;
        int i;
    while(state && close != 27)
    {
        if(i = recvfrom(*se.sock, buffer, strlen(buffer)+1, 0, (sockaddr *)&Sender_addr, &len)>0)
          {
              printf("massage %s \n",buffer);

              client.sin_addr.s_addr = inet_addr(buffer);
        if(sendto(*se.sock,se.Serverip , strlen(se.Serverip)+1, 0, (sockaddr *)&client, sizeof(Sender_addr)) > 0)
        {
            perror("borhot send: ");

          // printf("broasd cast send %s\n",se.Serverip);
            //_getch();
            //closesocket(sock);
        }
          }
        if (i<0)
            state =0;

    }
    printf("exiting broadcast");
    return param;
}
void *Closer(void *arg)
{
    int state = 1;
    struct AllSocket* se = (struct AllSocket*) arg;
    SOCKET* socket = se->lising;
    SOCKET* broadcast = se->broadcasting;
    printf("\nthis is closer thread waiting\n");
    while(state)
    {
        Sleep(1000);

        char ch = getch();
            if (ch == 27)
            {

                close = 27;
                Sleep(1000);
                ReleaseAssit();
                closesocket(*socket);
                closesocket(*broadcast);

            state = 0;
          //  printf("if all goes good i will be closing now hit any key\n");
       // getch();

            }
                    }



    printf("exiting");
   // exit(1);
    return arg;
}

void *atuh(void *arg)
{
    int port = 6555;
    int state = 0;
    FD_SET ReadSet;
     WSADATA WSAData;

    pthread_t* thread;
    SOCKET* server = (SOCKET*)arg;


    SOCKADDR_IN serverAddr, clientAddr;


    WSAStartup(MAKEWORD(2,0), &WSAData);
    *server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);


    char buffer[1024];
    printf("lisnting this is thread \n");
    while (close !=27)
    {


      FD_ZERO(&ReadSet);


    bind(*server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(*server, 0);
    int clientAddrSize = sizeof(clientAddr);
     unsigned int* ptr = (unsigned int*)(malloc(sizeof(int)));

    // FD_SET(server, &ReadSet);

    if(( *ptr= accept(*server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
    {
//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        SOCKADDR_IN client_info = {0};
        int addrsize = sizeof(client_info);
        getpeername(*ptr,(struct sockaddr*)&client_info,&addrsize);
        char *ip = inet_ntoa(client_info.sin_addr);
         printf("%d %s\n",state, ip);




         memset(buffer, 0, sizeof(buffer));

        thread = (pthread_t*)malloc(sizeof(pthread_t));
        struct pass* p = (struct pass*) malloc(sizeof(struct pass));
        p->thread = thread;
        p->socket = *ptr;
        strcpy(p->ip,ip);


        pthread_create(thread, 0, threadFunc, (void*)p);
       // pthread_join(thread, NULL);
        //closesocket(client);
        //FD_SET(*server, &ReadSet);
        printf("lisnting this is thread \n");

    }

    free(ptr);
//pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    Sleep(1000);

    }
    printf("lisnning is out\n");

return arg;
}
void SendClients ();
struct node* getClient(char name[]);
void sendMassage(char name[],unsigned int sock)
{
    struct Massage *ptr = MasHead,*p;
    while(ptr)
    {
        if(!strcmp(ptr->client,name))
        {
            send(sock,ptr->Massage,sizeof(ptr->Massage),0);
            p = ptr;
            ptr = getNextMassage(ptr);
            DeleteMassage(p);

        }
    }

}
void *threadFunc(void *arg)
{

    static int counte = 0;
    printf("\nlisnting this is thread %d\n",counte++);
    int state = 1;
    char mas[] = "this is server Whats your name";
    char hallo[15] = "hallo ";
    char name[10]= {0};
    char an[10] = {0};
    char buffer[1024] = {0};
    printf("Client connected!\n");
    struct node* chec;
    struct pass* p = (struct pass*)arg;

    unsigned int ptr ;
    ptr = p->socket;

    struct node* ptrr ;
    recv(ptr,buffer,sizeof(buffer),0);
    memset(buffer, 0, sizeof(buffer));
    send(ptr, mas, sizeof(mas), 0);
    recv(ptr, name, sizeof(name), 0);

     ptrr = (struct node*) malloc(sizeof(struct node));
     ptrr->socket = ptr;
     ptrr->Pnext = NULL;
     ptrr->online = 1;
     ptrr->thread = p->thread;
     strcpy(ptrr->ip,p->ip);
     chec = getClient(name);
     //printf("%s , %s",ptrr->ip,p->ip);

     free(arg);
     strcpy(ptrr->name,name);
     if(!chec)
      AddNode(ptrr);
     else {
            if (chec->online != 1)
            {
                chec->socket = ptrr->socket;
         chec->online = ptrr->online;
         strcpy(chec->ip,ptrr->ip);
         free(ptrr);
         sendMassage(name,chec->socket);
         ptrr  = chec;

            }
            else {
                    if (strlen(ptrr->name)<9)
               strcat(ptrr->name,"1");
            else
                ptrr->name[8] = '1';
            AddNode(ptrr);
            }

            }
            strcat(hallo,ptrr->name);
    send(ptr,hallo,sizeof(hallo),0);
      SendClients();


    while(state&&close != 27){
//pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
         // remove all white spaces and put -
        //replace scanf with getch
        //put a cheak for esc bottom to exit
       // memset(buffer, 0, sizeof(buffer));

        int r = recv(ptr, buffer, sizeof(buffer), 0);
        if (strlen(buffer) !=0)
        {



        //printf(" %d \t",*ptr);
        if(r>0){

       // printf( "Client says:%s %s\n",name,buffer);

        int s = 1;
        struct node* node = Phead;
        char m[1024];
        strcpy(m,name);
        strcat(m,":says ");

        if(buffer[0]=='.')
        {

            int i =1;
            while(buffer[i]!='.' &&i<10)
            {
               an[i-1] = buffer[i];
               i++;
            }
            i++;
            //printf("%s \n",an);
            strcat(m,buffer+i);
            node = getClient(an);
            if(node)
            {
              i = send(node->socket, m, sizeof(m), 0);
           if(i<0)
           {
               Massage* mm = (Massage*)malloc(sizeof(Massage));
               //mm->client = node->socket;
               strcpy(mm->client,node->name);
               strcpy(mm->Massage,m);
               mm->Pnext = NULL;
               AddMassage(mm);
           }
            }
            else {
                    strcpy(m,"package droped no user with this name\n");
                printf("%s \n",m);
                send(ptr, m, sizeof(m), 0);
            }

        }
        else{
                strcat(m,buffer);
           while (s)
        {
            int cas;
            if(node)
           {
              if(strcmp(name,node->name))
              {
                 cas =  send(node->socket, m, sizeof(m), 0);
                 if(cas<0)
                 {
                     Massage* mm = (Massage*)malloc(sizeof(Massage));
               //mm->client = node->socket;
               strcpy(mm->client,node->name);
               strcpy(mm->Massage,m);
               mm->Pnext = NULL;
               AddMassage(mm);
                 }

              }

           }
           else s = 0;
           node = getNext(node);

        }
        }

        }
        }
        else if (r >0)
        {

        }
        else {
                ptrr->online = 0;
                SendClients();
                memset(buffer, 0, sizeof(buffer));

               // strcpy(buffer," client %s is disconnect this thread is closing ");



                printf(" client %s is disconnect this thread is closing \n",name);
            state = 0;
        }

Sleep(1000);

}

printf("client %s is out\n ",name);
closesocket(ptr);
return arg;
}
struct node* getClient(char name[])
{
   struct node* ptr = Phead,*retrval = NULL;
   int state = 1;
   while(ptr&&state)
   {
       if(!strcmp(name,ptr->name))
        {
            state = 0;
            retrval = ptr;
        }
        ptr = getNext(ptr);
   }
   return retrval;
};
void SendClients ()
{
    int s =1,s2 = 1;
    char m[1024] ;
    struct node * node = Phead;
    struct node * ptr = Phead;
    memset(m, 0, sizeof(m));
    //m[0] = '/';
    //m[1] = 0;
    char online[2];
  while (s)
        {

            if(node)

           {
               send(node->socket, {"  "}, sizeof("  "), 0);
               while(s2)
                {

              if(ptr)
              {

                  if (node != ptr)
                  {
                  //m[0] = 1;
                  //m[1] = 0;
                  m[0] = ',';
                  m[1] = 0;
                  memset(online, 0, sizeof(online));
                  sprintf(online, "%d", ptr->online);
                  strcat(m,online);
                  strcat(m,",");
                  strcat(m,(ptr->ip));
                  strcat(m,",");
                  strcat(m,ptr->name);
                  strcat(m,",");



                  memset(online, 0, sizeof(online));

                  send(node->socket, m, sizeof(m), 0);
                  //printf("%s \n",m);
                  memset(m, 0, sizeof(m));

                  }
              }
              else
              {

                  s2 = 0;
              }
              ptr = getNext(ptr);

              //Sleep(100);

           }

           if(strlen(m)>1)
           {
              // printf(" \n");
           //send(node->socket, m, sizeof(m), 0);
           }

             memset(m, 0, sizeof(m));
            // printf("found %p \n",node);
             Sleep(100);
           }
           else s = 0;

           node = getNext(node);
           ptr = Phead;
           s2 = 1;

        }
}
//void *threadFunc(void *arg)
//{
//    WSADATA WSAData;
//    static int counte = 1;;
//
//    SOCKET server, client;
//
//    SOCKADDR_IN serverAddr, clientAddr;
//
//
//    WSAStartup(MAKEWORD(2,0), &WSAData);
//    server = socket(AF_INET, SOCK_STREAM, 0);
//
//    serverAddr.sin_addr.s_addr = INADDR_ANY;
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(5555);
//
//    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
//    listen(server, 0);
//
//    printf("lisnting this is thread %d\n",counte++);
//
//    char buffer[1024];
//    int clientAddrSize = sizeof(clientAddr);
//    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
//    {
//
//        int state = 0;
//        SOCKADDR_IN client_info = {0};
//        int addrsize = sizeof(client_info);
//        getpeername(client,(struct sockaddr*)&client_info,&addrsize);
//        char *ip = inet_ntoa(client_info.sin_addr);
//        printf("%s", ip);
//
//        while(state<10){
//        printf("Client connected!\n");
//         // remove all white spaces and put -
//        //replace scanf with getch
//        //put a cheak for esc bottom to exit
//        recv(client, buffer, sizeof(buffer), 0);
//        printf( "Client says: \n");
//        state++;
//        memset(buffer, 0, sizeof(buffer));
//    }
//
//
//
//        closesocket(client);
//        cout << "Client disconnected." << endl;
//    }
//}


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
