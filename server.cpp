# include "header.h"

// declaring global variables
sem_t doing_work;
FILE *output_file;
double last_time;
double first_time = 0;
double timeout_time;
bool timeout;
int process_count = 0;
bool exit_timeout = false;
bool exit_server = false;
map <string,int> summary;

void exit_function();
void *thread_func(void *args);
void *check_timeout(void *args);
void doWork(char* job, int client_sock, char* machine);

pthread_t thread;
pthread_t timeout_thread;

int main(int argc, char *argv[])
{
  // Creates a connection with a client and recieves the messages from client and processes the message.
  // Returns the number of process the server has done.
  int socket_desc, client_sock, c;
  struct sockaddr_in server, client;
  
  string job;
  int port_num;
  char* port;
  
  // get the number that the servers will read from
  port = argv[1];
  port_num = stoi(port);

  sem_init(&doing_work, 0, 1);
  
  timeout = true;
  
  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1)
    {
      printf("Could not create socket");
    }
	
  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port_num);
  
  //Bind
  if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0)
    {
      //print the error message
      perror("bind failed. Error");
      return 1;
    }

  //Listen
  if ( listen(socket_desc, 10000) < 0) {
    perror ("listen");
    exit(-1);
  }

  // create output file
  output_file = fopen("Server Output", "w");
  fprintf(output_file,"Using port %d\n",port_num);
  
  while (exit_server == false){  
    //Accept and incoming connection
    
    // check for server timeout after 30 seconds of no connection
    timeout_time = getTime();
    timeout = true;
    pthread_create(&timeout_thread, NULL, check_timeout, NULL);
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    timeout = false; //dont check for timeout
    if (client_sock < 0)
      {
	perror("accept failed");
	return 1;
      }

    // Create thread to do the processing work
    pthread_create(&thread, NULL, thread_func, &client_sock);
  }
  return 0;
}

void *check_timeout(void *args){
  double time_now;
  
  // checks if 30 seconds has passed
  // if 30 seconds has passed, exit the program
  while (timeout == true){
    // get the current time
    time_now = getTime();
    if (time_now - timeout_time >= 30 && time_now - last_time >= 30) { 
      exit_timeout = true;
      exit_function(); // exit the program
    }
  }
  return NULL;
}

void *thread_func(void *args){

  int *sock;
  int client_sock;
  int read_size;
  char machine[1000];
  char client_message[1000];
  char recieved_message[1000];
  
  sock = (int *)args;
  client_sock = *sock;

  //Receive a message from client
  while( (read_size = recv(client_sock, client_message, 1000, 0)) > 0 )
    {
      timeout = false;

      // reset the recieve message char array
      memset(recieved_message, 0, 1000);
      strcpy(recieved_message, client_message);

      sem_wait(&doing_work);
      doWork(recieved_message, client_sock, machine); //process the client message
      sem_post(&doing_work);
      memset(client_message, 0, 1000);     
    }

  // no more messages from client
  if(read_size == 0)
    {
      fflush(stdout);
    }
  else if(read_size == -1)
    {
      perror("recv failed");
    }

  timeout = true; // check if timeout has happened
  return NULL;
}

void doWork(char* job, int client_sock, char* machine){

  // get input from queue
  int index = -1;
  char server_reply[1000];
  int n;
  char n_value[1000];
  double time;

  // reset char array
  memset(machine, 0, 1000);
  memset(n_value, 0, 1000);

  process_count += 1;

  // get the index of the - char in the message
  for (int i = 0; job[i] != '\0';i++){
    if (job[i] == '-'){
      index = i;
    }
  }
	
  if (index == -1){
    perror("Message sent has error");
  } else {
    // get the n value from the message
    strncpy( n_value, job, index);
  }

  // get the machine name and pid from the client message
  strcpy( machine, job + index + 1);
  time = getTime();
    
  if (first_time == 0){
    // set the first time
    first_time = time;
  }
  
  fprintf(output_file,"%.2f: #%*d (T%*s) from %s\n",time, 3, process_count, 3, n_value, machine);
      
  n = atoi(n_value); // turn transaction number to integer
  Trans(n);
  
  last_time = getTime();     
  fprintf(output_file,"%.2f: #%*d (Done) from %s\n", last_time, 3, process_count, machine);

  sprintf(server_reply, "%d", process_count); // turn process count to char *
  write(client_sock, server_reply, strlen(server_reply));

  string str(machine); // turn machine char array into string
   // add to map
  if (summary.count(machine)) {
    // add one to the count
     summary[machine] += 1;
   } else {
     // insert the new machine to the summary map
     summary[machine] = 1;
   }
  return;
}

void exit_function(){

  int total_transactions = 0;
  double time_diff;
  double per_sec;

  exit_server = true;
  pthread_join(thread,NULL); // check that process are done

  fprintf(output_file, "SUMMARY\n");
  char machine[1000];
  // print off all the information in summary
  for (auto i : summary){
    strcpy(machine, i.first.c_str());
    fprintf(output_file, "%*d transactions from %s\n",4, i.second, machine);
    total_transactions += i.second; // add transactions numbers
  }

  // get the time difference
  time_diff = last_time - first_time;
  per_sec = total_transactions/time_diff;
  fprintf(output_file,"%.1f transactions/sec (%d,%.2f)", per_sec, total_transactions, time_diff);
  fclose(output_file);
  exit(0);
}
