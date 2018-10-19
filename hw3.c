
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void forkIt(char *cmd){
  int pid = fork();
  if(pid == 0){
     
  }
  else{
    int status;
    wait(&status);
    printf("pid: %d status: %d \n", pid, WEXITSTATUS(status));

  }
}

void doCommand(char **cmd){
  char *file = '\0'; //will hold our filenames
  char *file2 = '\0'; 
  char *io = '\0';   // will tell us the input direction
  char *io2 = '\0';  //if this is a ';' then there is a second command
  char *io3 = '\0';
  char *cmd2 = '\0'; 
  char *second[20];

  int pHold=0;
  int pHold2=0;
  int sHold = 0;
  
  //parse the input recieved 
  int i = 0;
  while(1){
    if(cmd[i]== '\0'){
      break;
    }
      if(strcmp(cmd[i],";")==0){
        io2 = cmd[i];
        cmd2 = cmd[i+1];
        second[0]=cmd[i+1];
        second[1] = '\0';
        if(cmd[i+2] != '\0'){
           if((strcmp(cmd[i],">") == 0)||( strcmp(cmd[i],"<")== 0)){
            io3 = cmd[i+2];
            file2 = cmd[i+3];
            cmd[i+2]= '\0';
            cmd[i+3] = '\0';
           }
           else{
            second[0] = cmd[i+1];
            second[1] = cmd[i+2];
            cmd[i+2] = '\0';
            second[2] = '\0';
            //printf("%s\n",second[0]);
           }
        }
        cmd[i] = '\0';
        cmd[i+1] = '\0';
        break;
      }
      if((strcmp(cmd[i],">") == 0)||( strcmp(cmd[i],"<")== 0)){
        io = cmd[i];    //gets the input/output direction
        file = cmd[i+1];//gets the file name
       // printf("%s %s \n",file, io); 
        if(cmd[i+2] != '\0'){
          if(strcmp(cmd[i+2],";")== 0){
            io2 = cmd[i+2];
            cmd2 = cmd[i+3];
            second[0] = cmd[i+3];
           
            //can't be sure the line will always involve another file.  
            if(cmd[i+4] != '\0'){
              io3 = cmd[i + 4];
              file2 = cmd[i + 5];
              cmd[i + 4] = '\0';
              cmd[i + 5] = '\0';

            }
            cmd[i+2]= '\0';
             cmd[i+3]= '\0';
          }
        }
        //clear these lines
        cmd[i] = '\0'; 
        cmd[i+1] = '\0';
        break;
      }
    
    i++; 
  }
  
  int pid = fork(); 
  if(pid == 0){
   if(io != '\0'){
    if(strcmp(io,">")==0){//redirects output to file name
     int fd = open(file, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH);//reads|writes, User can RWX, others can R 
      dup2(fd,1);
      close(fd);
    }
    if(strcmp(io,"<")==0){
     int fd1 = open(file, O_RDONLY);
     dup2(fd1,0);
     close(fd1);
    }
   
   }
   //execute
   // printf("%s",file);
   //pHold = getpid();
   exit(execv(cmd[0],cmd));
  }
  else{
    int status;
    wait(&status);
    //before reporting both processes must be completed
    //second command
   if(io2 != '\0'){
    int pid2 = fork();
    if(pid2 == 0){
     if(io3 != '\0'){
      if(strcmp(io3,">")==0){//redirects output to file name
       int fd2 = open(file2, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH);//same as above
       dup2(fd2,1);
       close(fd2);
      }
      if(strcmp(io3,"<")==0){
       int fd3 = open(file, O_RDONLY);
       dup2(fd3,0);
       close(fd3);
      }

     } 
     //pHold2 = getpid();
     exit(execv(second[0],second));
    } 
    else{
    int status1;
    wait(&status1);
    pHold2 = pid2;
    //printf("pid:%d status: %d \n", pid2, WEXITSTATUS(status1));
    sHold = status1;

   }
  }
    pHold = pid;
    printf("pid:%d status:%d \n", pHold, WEXITSTATUS(status)); 
    if(pHold2 != 0){
       printf("pid:%d status:%d \n", pHold2, WEXITSTATUS(sHold)); 

    }
  }
}

void sigint_handler(int sig){
  char msg[] = "\ncaught sigint\n";
  write(1,msg, sizeof(msg));
  char prompt[] = "CS361 >";
  write(1, prompt, sizeof(prompt));
}

void sigstp_handler(int sig){
  char msg[] = "\ncaught sigtstp\n";
  write(1,msg, sizeof(msg));
  char prompt[] = "CS361 >";
  write(1, prompt, sizeof(prompt));
  //Don't think it needs to do anything else? 
}

int main(){
  //Input storage
  char line[500]; //Going based off of Lab 4 
  char *argsarray[20];

  //signals based on lab 5 code
  signal(SIGINT, sigint_handler);
  signal(SIGTSTP, sigstp_handler);//prompt said sigstp but that didn't work

  while(1){ //infinite loop until we need to exit the program
   
    printf("CS361 > "); //NEEDS to print out exactly like this

    fgets(line, 500, stdin); //read input
    char *word = strtok(line, " \n");//reads line until newline character, Also the space matters
    int i = 0;
    while(word){
    //  printf("word: %s\n", word);
      
      argsarray[i]= word;
      word = strtok(NULL, " \n");//sets word to NULL to be copied over again
      i++;
    }
    argsarray[i+1] = '\0';

    if(strcmp(argsarray[0],"exit")== 0 || strcmp(argsarray[0],"Exit")==0){
      break;
    }
   
    doCommand(argsarray);
    
  }
	
  return 0;
}
