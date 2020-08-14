import socket
import concurrent.futures
import time

def sendDevice(msg):
    s= socket.socket()
    hostname = '172.20.10.14'
    port = 8000
    s.connect((hostname,port))
    s.send(msg.encode())
    
def sendCloud(msg):
    s= socket.socket()
    hostname = '172.20.10.2'
    port = 8000
    s.connect((hostname,port))
    s.sendall(msg.encode())
    message = s.recv(1024).decode() 
    return message
    
listensocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
Port = 8000
maxConnections = 999

listensocket.bind(('',Port))
  
listensocket.listen(maxConnections)
    
while True:
    (clientsocket, address) = listensocket.accept()
    message = clientsocket.recv(1024).decode() 
    returned = sendCloud(message)
    sendDevice(returned)