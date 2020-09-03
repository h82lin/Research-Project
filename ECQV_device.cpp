#include <stdlib.h>
#include <iostream>
#include<stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h> 
#include <vector>
#include"../Ed25519.h"

using namespace std;

int sendTTP(uint8_t *RaData, uint8_t *idData){
    int   sockfd, n;
    char  recvline[4096], sendline[4096];
    struct sockaddr_in  servaddr;
 
    //if( argc != 2){
    //    printf("usage: ./client <ipaddress>\n");
    //    return 0;
    //}
 
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        return 0;
    }
 
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(6666);
    if( inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n","127.0.0.1");
        return 0;
    }
 
    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }
 
     printf("send msg to server: \n");
     //fgets(sendline, 4096, stdin);
    if(send(sockfd, RaData, 32, 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }
    if(send(sockfd, idData, 4, 0) < 0){
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        return 0;
    }

    char receiveMessage[100] = {};
    recv(sockfd,receiveMessage,sizeof(receiveMessage),0);
    close(sockfd);
}

int main(){
    uint8_t id[5] = "0023";
    Ed25519::Point Ra;
    srand (time(NULL));
    limb_t ra = rand()%100000;
    Ed25519::multiplyBase(Ra, &ra);

    uint8_t encodedRa[32];
    Ed25519::encode(encodedRa, Ra);
    //encodedRa[32] = '\0';
    //for(int i=0; i<32; i++){
     //   cout << (int) encodedRa[i] << '\n';
    //}
    
    //for(int i=0; i<4; i++){
    //    cout << "x" << Ra.x[i] << '\n';
    //    cout << "y" << Ra.y[i] << '\n';
    //    cout << "z" << Ra.z[i] << '\n';
     //   cout << "t" << Ra.t[i] << '\n';
    //}
    Ed25519::Point Ra2;
    Ed25519::decode(Ra2, encodedRa);
    cout << '\n';
   // for(int i=0; i<4; i++){
   //     cout << "x" << Ra2.x[i] << '\n';
   //     cout << "y" << Ra2.y[i] << '\n';
   //     cout << "z" << Ra2.z[i] << '\n';
   //     cout << "t" << Ra2.t[i] << '\n';
   // }
    //for(int i=0; i<33; i++){
    //    cout << (int) encodedRa[i] << '\n';
   // }
    //cout <<data[0][0];
    cout << Ed25519::equal(Ra, Ra2);
    sendTTP(encodedRa, id);
	return 0;
}
/*

    uint8_t encodedRa[32];
    Ed25519::encode(encodedRa, Ra);
    vector<int> intEncoded;
    for(int i=0; i<32; i++){
        intEncoded.push_back((int) encodedRa[i]);
    }
    vector<vector<int>> data = {intEncoded, id};
    sendTTP(data);

*/