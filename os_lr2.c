#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN 80

int main() {
 char buffer[MAX_WORDS][MAX_WORD_LEN];
 char* argv [MAX_WORDS];
 int i = 0; 
 int j = 0; 
 int in_word = 0; 
 int STDOUT_Forward = 0; 
 int STDIN_Forward = 0;
 int ch;
 printf("$"); 
 while ((ch = getchar()) != EOF) { 
   if (ch == ' ' || ch == '\n' || ch == '>' || ch == '<') {
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
 };
 if (ch == '>') {
  STDOUT_FORWARD = i;
 }
 if (ch == '<') {
  STDIN_FORWARD = i;
 }
 if (ch == '\n') {
    argv[i] = NULL;
    pid_t pid = fork();
    if (!pid) { // child branch
      if (STDOUT_FORWARD! = 0) {
       int fd = open(argv[STDOUT_FORWARD], O_WRONLY | O_CREAT | O_TRUNC, 0666);
       if (fd == -1) {
           perror("open");
           return EXIT_FAILURE;
       }
       // Forward STDOUT_FILENO to fd //
       if (-1 == dup2(fd, STDOUT_FILENO)) {
           perror("dup2");
           return EXIT_FAILURE;
       }
       argv[STDOUT_FORWARD] = NULL;
      }
      if (STDIN_FORWARD != 0) {
       int fd1 = open(argv[STDIN_FORWARD], O_RDONLY);
       if (fd1 == -1) {
           perror("open");
           return EXIT_FAILURE;
       }
       // Forward STDIN_FILENO to fd1 //
       if (-1 == dup2(fd1, STDIN_FILENO)) {
           perror("dup2");
           return EXIT_FAILURE;
       }
       argv[STDIN_FORWARD] = NULL;
      }       
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
    STDOUT_FORWARD = 0; 
    STDIN_FORWARD = 0;
   }
 }
  return EXIT_SUCCESS;
}
