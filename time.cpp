#include "header.h"

struct timeval t1;

double getTime(){
  double start_sec;
  double start_micro;
  double return_time;
  // get the time of the start of the process
  gettimeofday(&t1,NULL);
  start_sec = t1.tv_sec;
  start_micro = t1.tv_usec;
  start_micro = start_micro/1000000.0;
  return_time = start_sec + start_micro;
  //cout << "RETURN TIME: " << return_time <<endl;
  return return_time;
}

