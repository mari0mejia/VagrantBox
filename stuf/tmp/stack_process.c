#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
static void sigHandler_sigusr1(int sig)
{
  printf("Caught SIGUSR1\n");
  kill(getpid(), SIGUSR1);
}
static void sigHandler_sigusr2(int sig)
{
  printf("Caught SIGSR2\n");
  kill(getpid(), SIGUSR2);
}

static void sigHandler_sigint(int sig)
{
  printf("Caught SIGINT, Existing\n", getpid());
  kill(getpid(), SIGINT);
  exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{
  if (signal(SIGUSR1, sigHandler_sigusr1) == SIG_ERR)
    printf("Unable to create handler for SIGUSR1\n");
  if (signal(SIGUSR2, sigHandler_sigusr2) == SIG_ERR)
    printf("Unable to create handler for SIGUSR2\n");
  if (signal(SIGINT, sigHandler_sigint) == SIG_ERR)
    printf("Unable to create handler for SIGUSR1\n");
  kill(getppid(),SIGINT);
  while(1){
    /*some work*/
  }

  return 0;
}




