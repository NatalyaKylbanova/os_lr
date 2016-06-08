#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN 80

int main() {
 char buffer[MAX_WORDS][MAX_WORD_LEN];
 char* argv[MAX_WORDS];
 int i = 0; 
 int j = 0; 
 int in_word = 0;
 int ch;
 printf("$"); 
 while ((ch = getchar()) != EOF) { 
   if (ch == ' ' || ch == '\n') {
     if (in_word == 1) { 
       in_word = 0;
       buffer[i][j] = '\0'; 
       argv[i] = buffer[i]; 
       ++i; 
       j = 0; 
     }
   } else { 
       buffer[i][j] = ch; 
       ++j; 
       in_word = 1;
   }
   if (ch == '\n') {
     argv[i] = NULL;
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
     printf("$");
     i = 0; 
     j = 0; 
     in_word = 0;
   }
 }
 return EXIT_SUCCESS;
}

