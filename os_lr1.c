#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
int i,j,ch, in_word;
char buffer[16][80];
char* argv [16];
 i=0; j=0; in_word=0;
  printf ("$"); 
    while ((ch=getchar()) != EOF) { 
      
    if (ch == ' ' || ch == '\n') {
     if (in_word==1) { in_word=0; buffer[i][j]='\0'; argv[i]=&buffer[i]; ++i; j=0; }}
    else { buffer[i][j]=ch; ++j; in_word=1; };
      
    if (ch=='\n') {
       argv[i]=NULL;
       pid_t pid = fork();
       if (!pid) { // child branch
        int rv = execvp(argv[0], argv);
        if (rv == -1) {
         perror("execvp");
         return EXIT_FAILURE;
        }
       }
       // parent branch
       pid = wait(NULL);
       if (pid == -1) {
         perror("wait");
         return EXIT_FAILURE;
       }
     printf ("$");
     i=0; j=0; in_word=0;
    }
}
  return EXIT_SUCCESS;
}

