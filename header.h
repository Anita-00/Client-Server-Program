#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>   
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <iostream>
#include <queue>
#include <string>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <map>

void Trans( int n );
void Sleep( int n );
double getTime();

using std::istringstream;
using namespace std;

