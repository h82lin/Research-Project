#include <iostream>
#include <cstddef>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <stdio.h>
#include <errno.h>
#include <bits/stdc++.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include"../Ed25519.h"
#include"../SHA256.h"

using namespace std;

#define HASH_SIZE 32
#define MAXLINE 4096
#define MAXLINE 4096

void perfHash(Hash *hash, uint8_t *data, uint8_t *buff){
	int count;
    hash->reset();

    hash->update(data, 33);
    for (count = 0; count < 1000; ++count) {
        hash->finalize(buff, HASH_SIZE);
    }
}

string uintconverter(uint8_t *str){
    return string((char *)str);
}

string multiplyStrings(string str1, string str2) 
{ 
    if((str1.at(0) == '-' || str2.at(0) == '-') &&  
    (str1.at(0) != '-' || str2.at(0) != '-' )) 
    cout<<"-"; 
  
  
    if(str1.at(0) == '-' && str2.at(0)!='-') 
        { 
            str1 = str1.substr(1); 
        } 
        else if(str1.at(0) != '-' && str2.at(0) == '-') 
        { 
            str2 = str2.substr(1); 
        } 
        else if(str1.at(0) == '-' && str2.at(0) == '-') 
        { 
            str1 = str1.substr(1); 
            str2 = str2.substr(1); 
        }

    int len1 = str1.size(); 
    int len2 = str2.size(); 
    if (len1 == 0 || len2 == 0) 
    return "0"; 
  
    // will keep the result number in vector 
    // in reverse order 
    vector<int> result(len1 + len2, 0); 
  
    // Below two indexes are used to find positions 
    // in result.  
    int i_n1 = 0;  
    int i_n2 = 0;  
      
    // Go from right to left in num1 
    for (int i=len1-1; i>=0; i--) 
    { 
        int carry = 0; 
        int n1 = str1[i] - '0'; 
  
        // To shift position to left after every 
        // multiplication of a digit in num2 
        i_n2 = 0;  
          
        // Go from right to left in num2              
        for (int j=len2-1; j>=0; j--) 
        { 
            // Take current digit of second number 
            int n2 = str2[j] - '0'; 
  
            // Multiply with current digit of first number 
            // and add result to previously stored result 
            // at current position.  
            int sum = n1*n2 + result[i_n1 + i_n2] + carry; 
  
            // Carry for next iteration 
            carry = sum/10; 
  
            // Store result 
            result[i_n1 + i_n2] = sum % 10; 
  
            i_n2++; 
        } 
  
        // store carry in next cell 
        if (carry > 0) 
            result[i_n1 + i_n2] += carry; 
  
        // To shift position to left after every 
        // multiplication of a digit in num1. 
        i_n1++; 
    }
  
    // ignore '0's from the right 
    int i = result.size() - 1; 
    while (i>=0 && result[i] == 0) 
    i--; 
  
    // If all were '0's - means either both or 
    // one of num1 or num2 were '0' 
    if (i == -1) 
    return "0"; 
  
    // generate the result string 
    string s = ""; 
      
    while (i >= 0) 
        s += std::to_string(result[i--]); 
  
    return s; 
}

string addStrings(string str1, string str2) 
{ 
    // Before proceeding further, make sure length 
    // of str2 is larger. 
    if (str1.length() > str2.length()) 
        swap(str1, str2); 
  
    // Take an empty string for storing result 
    string str = ""; 
  
    // Calculate length of both string 
    int n1 = str1.length(), n2 = str2.length(); 
  
    // Reverse both of strings 
    reverse(str1.begin(), str1.end()); 
    reverse(str2.begin(), str2.end()); 
  
    int carry = 0; 
    for (int i=0; i<n1; i++) 
    { 
        // Do school mathematics, compute sum of 
        // current digits and carry 
        int sum = ((str1[i]-'0')+(str2[i]-'0')+carry); 
        str.push_back(sum%10 + '0'); 
  
        // Calculate carry for next step 
        carry = sum/10; 
    } 
  
    // Add remaining digits of larger number 
    for (int i=n1; i<n2; i++) 
    { 
        int sum = ((str2[i]-'0')+carry); 
        str.push_back(sum%10 + '0'); 
        carry = sum/10; 
    } 
  
    // Add remaining carry 
    if (carry) 
        str.push_back(carry+'0'); 
  
    // reverse resultant string 
    reverse(str.begin(), str.end()); 
  
    return str; 
} 

int main(){

    int  listenfd, connfd;
    struct sockaddr_in  servaddr;
    uint8_t  buff[4096];
    int  n, b;

    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    if( listen(listenfd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        return 0;
    }

    printf("======waiting for client‘s request======\n");
    if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
    }
    n = recv(connfd, buff, MAXLINE, 0);
    
    buff[n] = '\0';
    //for(int i=0; i<40; i++){
    //    cout << (int) buff[i];
    //    cout << '\n';
    //}
    //printf("recv msg from client: %s\n", buff);

	uint8_t id[5];
    uint8_t deviceRa[32];
    for(int i=0; i<32; i++){
        deviceRa[i] = buff[i];
    }
    for(int i=0; i<4; i++){
        id[i] = buff[32+i];
    }

	srand (time(NULL));
    limb_t rt = rand()%RAND_MAX;
    Ed25519::Point Rt;
    Ed25519::Point Ra;
    Ed25519::decode(Ra, deviceRa);
    for(int i=0; i<4; i++){
        cout << "x" << Ra.x[i] << '\n';
        cout << "y" << Ra.y[i] << '\n';
        cout << "z" << Ra.z[i] << '\n';
    }
    //cout << Ra.x[0];
    Ed25519::multiplyBase(Rt, &rt);

    Ed25519::Point cert;
    Ed25519::addition(Rt, Ra);
    cert = Rt;
    SHA256 sha256;

    uint8_t encoded[34];
    Ed25519::encode(encoded, cert);
    //encoded[32] = id;

    uint8_t buffer[HASH_SIZE];
    perfHash(&sha256, encoded, buffer);

    ostringstream os;

    for(int i=0; i<32; i++){
        os << (int)buffer[i];
    }
    string s(os.str());

    string k = to_string(rand()%RAND_MAX);
    string s_rt = to_string(rt);    

    string r = addStrings(multiplyStrings(s_rt, s), k);

    send(connfd,"YAY",sizeof("YAY"),0);
    
    close(connfd);
        
    
    close(listenfd);

}