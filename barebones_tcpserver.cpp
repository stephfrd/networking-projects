//libraries
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
//anything that says .h is a C library/includes a lot of C stuff 
//link i got to doing this: https://www.youtube.com/watch?v=cNdlrbZSkyQ
//works only on linux, but i'm sure there are a lot of windows socket programming videos too
//this is a barebones code but a usual socket code will use OOP, but since I can't master it very well as of right now, I will not use it
using namespace std;
int main(){
    //create a socket
        //AF_INET is the ipv4 protocol; there's also AF_INET6 But that is ipv6, obviously
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening== -1){
        cerr<<"Can't create socket!"; //i'm basically creating an error that the compiler can give me if a socket cannot be created
        return -1;
    }
    
    //bind socket to IP/port; we will bind it to any IP address for the sake of this code
        //sockaddr_in=internet version protocol 
    sockaddr_in hint;
    hint.sin_family= AF_INET; //binding it to the IPV4
        //the port is on 54000 but because we are on linux, and i'm compiling it on amd (but it works the name on intel too) we can't use that lol
        // instead, we use a "conversion" called htons (host short to network short); it does literally what the name says it does
        //there is also ntohs (network to host short) which does the reverse operation of htons
    hint.sin_port= htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); //pton is an internet command (hence inet), and it's a pointer (because ofc it, is it's cpp)
    //the ip numbers are strings that are converted by pton into numbers, hence, it's a pointer to a string to a number
    //this function has as parameters: the format of the address (INET or INET6), the character array of the IP address and the buffer(pointer)
    //the character array (it's "0.0.0.0") to get any address, since it's a default route
    if (bind(listening, (sockaddr*)&hint, sizeof(hint))== -1){ //func checks if hint can bind to the port
        cerr<< "Can't bind to IP/port";
        return -2;
    }
    //the first parameter of the bind function is the file descriptor, which in unix/unix like system is the socket number
    if(listen(listening, SOMAXCON)==-1){ //SOMAXCON=the max number of connection
        cerr<<"Can't listen!";
        return -3;
    } //marks the socket as being able to listen 
    //accept a call
    sockaddr_in client;
    socklen_t clientSize=sizeof(client);
    char host[NI_MAXHOST], svc[NI_MAXSERVICE]; //buffers
    //accepting an incoming connection
    int clientSocket=accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket=-1){
        cerr<<"Problem with client connecting";
        return -4;
    }

    //close the listening sockey
    close(listening);
    //cleaning up the char stuff 
    memset(host, 0, NI_MAXHOST); //fills the whole char array with zeros
    memset(svc, 0, NI_MAXSERVICE);

    int result= getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERVICE, 0) //gets the name of the computer
    if (result){
        cout<<host<<"connected on"<<svc<<endl; //a notification that 
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST); //the opposite of pton; a numeric array to a string
        cout<<host<<"connected on"<<nthos(client.sin_port)<<endl;
    }
    //the 0 in the function is a flag

    //while receiving display message, echo message
    char buf[4096];
    while(true){
        //clear buffer
        memset(buf, 0. 4096);
        //wait for message
        //clientsocket stores the socket number
        int bytesRecv=recv(clientSocket, buf, 4096, 0);
        if(bytesRecv == -1){
            cerr<<"There was a connection issue"<<endl;
            break;
        }
        if(bytesRecv=0){
            cout<<"The client disconnected"<<endl;
            break;
        }

        //display message
        cout<<"Received:"<<string(buf, 0, bytesRecv)<<endl;

        //resend message
        send(clientSocket, buf, bytesRecv+1, 0);
    }
    //close socket
    close(clientSocket);
    return 0;
}