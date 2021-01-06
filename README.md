# Client-Server-Program

The client connects to the server. The client sends transactions to the server, and the server listens for communications from the server.

* programs are in the master file

## Getting Started

In the master branch, there are the codes for the client.cpp program and the server.cpp program. Additionally, there are man pages for the client program 
and the server program.

### Installing

To clone, in terminal write:

```
git clone https://github.com/Anita-00/Client-Server-Program.git
```

## Connecting to Client

To connect to the client to a server, specify the ip address and the port number. Client reads T<n> and S<n> commands, where T<n> command is handled by tands.c file, and S<n> 
command causes the client to wait for "n" seconds. <n> is an integer.

Example:

```
./client 6000 127.0.0.1
```

## Running server

The sever listens for communications on the specified port number for clients. Server gets T<n> process to perform as a message from the client. Server watis for antoher message 
from clients after processing one message. Returns transaction number to client after message has been processed. Server times out after 30 seconds after last client disconnects if no other client connects within 30 seconds of last message.
  
To run program, write ./server [port number]
  
Example:
```
./server 6000
```

## Acknowledgements

* tands.c file was written by professor and TAA's at the University of Alberta
