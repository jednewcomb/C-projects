// program to demonsrate interval timers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/signal.h>


int count = 0;   // used to count off timer interupts

// handler
void handler(int param)
{
  if (count%4 == 0)
    printf("\nhup");             // \n hup
  else
    printf(" %d", count%4 + 1);  //        2 3 4
  fflush(stdout);
  count++;
}


// start a timer with a repeat interval
int main()
{
  signal(SIGALRM, handler);   // install handler

  // now start repeating timer
  struct itimerval timer;
  timer.it_value.tv_sec = 2;   // start after 2 seconds
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 0;  // repeat every 3/8 of a second
  timer.it_interval.tv_usec = 375000;
  setitimer(ITIMER_REAL, &timer, NULL);

  while (count < 40)
    pause();                 // keep waiting  until 40 interupts

  // turn off timer
  timer.it_value.tv_sec = 0;
  timer.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &timer, NULL);

  printf("\ndone\n");
}
