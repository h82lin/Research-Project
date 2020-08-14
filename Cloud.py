import socket

listensocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #Creates an instance of socket
Port = 8000 #Port to host server on
maxConnections = 999
#IP = socket.gethostname() #IP address of local machine
IP = '172.20.10.2'

listensocket.bind(('',Port))

#Starts server
listensocket.listen(maxConnections)
print("Server started at " + IP + " on port " + str(Port))

while True:
    (clientsocket, address) = listensocket.accept()
    print("New connection made!")
    message = clientsocket.recv(1024).decode() #Gets the incomming message
    if(message != ""):
        print(message)
    clientsocket.send("YAY".encode())

