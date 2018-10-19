#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void Fork(char *cmd){
  int pid = fork();
  if(pid == 0){
     
  }
  else{
    int status;
    wait(&status);
    printf("pid: %d status: %d \n", pid, WEXITSTATUS(status));
  }
}

void execute(char **command) {

}

void sigint_handler(int sig){
  char msg[] = "Signal handled.  Deal with it.\n";
  write(1, msg, sizeof(msg));
  exit(0);
}

void sigstp_handler(int sig){
  char msg[] = "\ncaught sigtstp\n";
  write(1,msg, sizeof(msg));
  char prompt[] = "CS361 > ";
  write(1, prompt, sizeof(prompt));
}


int main(){
        char line[500];
        char *argsarray[20];

        //signals
        signal(SIGINT, sigint_handler);
        signal(SIGTSTP, sigstp_handler);



        while(1) {
                printf("CS361 > ");

                //get input
                fgets(line, 500, stdin);

                char *word = strtok(line, " \n");
                int i = 0;

                while(word) {
                        argsarray[i] = word;
                        word = strtok(NULL, " \n");
                         i++;
                }

                argsarray[i+1] = '\0';

                if(strcmp(argsarray[0], "exit") == 0)
                        break;

                }

                execute(argsarray);


}
                                                                                      
