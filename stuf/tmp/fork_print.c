#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void P(char *s)
{
  fprintf(stderr, "%s \n",s);
}
int main()
{
  int status;
  P("B");
  if(fork())
  {
    P("L");
    if(fork()==0){
      P("G");
    }else{
      P("S");
      printf("wait\n");
      wait(&status);
    }
  }
  else
  {
    P("X");
    if(fork()){
      P("W");
      printf("2nd wait\n");
      wait(&status);
      P("Z");
    }else{
      P("T");
      if(fork()){
        P("H");
      }else{
        P("M");
      }
      exit(0);
    }
  }
  P("R");
}



