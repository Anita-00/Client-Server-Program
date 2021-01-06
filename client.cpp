# include "header.h"

char* sendMessage(char *message, int sock);
int main(int argc, char *argv[])
{
  // client program connects to server and sends T<n> to the server process and sleeps when recieves S<n>
  // messages.
  // Prints results in outputfile with machinename.pid
  
  // declaring variables
  char* port;
  char* ip_address;
  int port_num;
  bool is_exit = false;
  FILE *output_file;
  int len_command;
  string command;
  int n;
  int sleep_units;
  string substring;
  int sock;
  struct sockaddr_in server;
  char message[1000];
  string str_message;
  int pid;
  char *return_message;
  int trans = 0;

  // get the number that the servers will read from
  port = argv[1];
  port_num = stoi(port);

  // get the address of the machine that is hosting the server
  ip_address = argv[2];
  
  // create file once connected
  char hostname[HOST_NAME_MAX + 1 + 52];
  char original_hostname[HOST_NAME_MAX + 1 + 52];
  char dot[2] = ".";
  char dash[2] = "-";
  char buffer[50];
  char pid_buffer[50];
  char client_message[1000];
  double time;
  
  gethostname(hostname, HOST_NAME_MAX + 1); // get machine name
			 
  gethostname(original_hostname, HOST_NAME_MAX + 1); // get machine name
  pid = getpid(); // get process id number

  // do type conversions and add to hostname variable
  sprintf(pid_buffer, "%d",pid); // turn int to char*
  strcat(hostname,dot);
  strcat(hostname,pid_buffer);

  output_file = fopen(hostname, "w");

  // print to output file
  fprintf(output_file, "Using port %d\n", port_num);
  fprintf(output_file, "Using server address %s\n", ip_address);
  fprintf(output_file, "Host %s\n", hostname);
  
  //keep communicating with server
  while(is_exit == false)
    {
      //Create socket
      sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock == -1)
	{
	  printf("Could not create socket");
	}

      server.sin_addr.s_addr = inet_addr(ip_address);
      server.sin_family = AF_INET;
      server.sin_port = htons(port_num);

      //Connect to remote server
      if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
	  perror("connect failed. Error");
	  return 1;
	}

      // reset the char array
      memset(message, 0, 1000);
      memset(client_message, 0, 1000);

      // get message from the user or input
      cin >> message;
      str_message = string(message);
      
      if (cin.eof()){
	// check if end of file character entered	
	is_exit = true;
      }
      else {
	// get the n value
	len_command = str_message.length(); //get the length of the command
	substring = str_message.substr(1,len_command); //get substring without the letter
	n = stoi(substring); //convert string to integer

	// get char array with format n-machinename.pid
	sprintf(buffer, "%d",n);
        strcat(client_message, buffer);
	strcat(client_message, dash);
	strcat(client_message, original_hostname);
	strcat(client_message, dot);
	strcat(client_message, pid_buffer);

	if (message[0] == 'T'){
	  // send to the server

	  // get the current time
	  time = getTime();
	  fprintf(output_file, "%.2f: Send (T%*d)\n", time, 3, n);
	  
	  return_message = sendMessage(client_message,sock); // get the return message from the server 

	  // get the time
	  time = getTime();

	  // prints the message from server to the output file
	  fprintf(output_file, "%.2f: Recv (D%*s)\n", time, 3, return_message);
	 
	  trans += 1; // increase number of transactions made

	  // close connection
	  close(sock);
	  
	} else if (message[0] == 'S'){
	  sleep_units = n;
	  fprintf(output_file, "Sleep %d units\n",sleep_units);

	  // make client sleep for n seconds
	  Sleep(n);
	}
      }     
    }
  
  fprintf(output_file, "Sent %d transactions",trans);
  fclose(output_file);
  return 0;
}

char* sendMessage(char *message, int sock){
  char server_reply[1000];
  
  //Send some data
  if( send(sock, message, strlen(message), 0) < 0)
    {
      puts("Send failed");
      perror("Send");
    }
      
  //Receive a reply from the server
  memset(server_reply, 0, 1000);
  if( recv(sock, server_reply, 1000, 0) < 0)
    {
      puts("recv failed");
      perror("Recieve");
    }
  
  // set message to server_reply because need to return memory location allocated before function call
  message = server_reply; 
  return message;
}
