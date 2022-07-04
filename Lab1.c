#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#define SIZE 1024

//Global and static variables

static int count=0;
char cwd[256];
static pid_t cpid;
int status=0;
char *buff;
static char* args[SIZE]={};
static char child_stack[1048576];

//Function to read in user input, line by line

char *read_line(){

int i =0;
char *args=malloc(sizeof(char)*1000);
char *line=malloc(sizeof(char)*1000);

while(1){
i++;
fgets(line, 1000, stdin);
strcpy(&args[i],line);
if(strchr(line, '\n') != NULL){
    return args;
  }
  
}
free(line);
free(args);
}

//Function that takes in user input and places it into an array

void parsing(char* line){
  int i=0;
  char* lines = strtok (&line[1],"\n");
  lines[strlen(lines)]='\0';
  while(lines!=NULL){
  i++;
  args[i]=lines;
   lines = strtok(NULL,"\n"); 
  }

  free(lines);
}

//Function to change the root of the file system

void changeRoot(){
char newroot[50];
getcwd(cwd,sizeof(cwd));
printf("The directory is %s\n",cwd);

if(strcmp(cwd,"/")!=0 && count==0){
chdir("/tmp");
if(chroot("/tmp") !=0){
perror("chroot /tmp");
}
printf("Enter new root ");
fgets(newroot,50,stdin);
mkdir(newroot,0700);
chdir(newroot);

}

else if(strcmp(cwd,"/")!=0){
getcwd(cwd,sizeof(cwd));
chdir("..");
rmdir(cwd);
printf("Enter new root ");
fgets(newroot,50,stdin);
mkdir(newroot,0700);
chdir(newroot);

}

getcwd(cwd,sizeof(cwd));
printf("The new directory is %s",cwd);
count++;
}

//Function to change hostname 

void changeHostname(){
struct utsname utsname;
uname(&utsname);
printf("Old hostname %s\n", utsname.nodename);
char newname[50];
printf("Enter the new hostname\n");
fgets(newname,50,stdin);
sethostname(newname,50);
uname(&utsname);
printf("New hostname %s", utsname.nodename);

}


//Function that takes in users input and matches it to built in commands and executable 

void commandExec(){

char *const argv[]={args,NULL};
  int i=1;

  
  while(args[i]!=NULL){
  if(strcmp(args[i],"quit")==0){
  exit(0);
    }
  else if(strcmp(args[i],"new")==0){
            changeHostname();
    }
    
    else if(strcmp(args[i],"pid")==0){
      printf("PID: %d\n",cpid);
    }
    else if(strcmp(args[i],"root")==0){
     changeRoot();
     }
    else if(strcmp(args[i],"net")==0){
    system("ip link");

     }
     
char line[50]; 
strcpy(line,args[i]);

 cpid=fork();
wait(NULL);
 if(cpid==0){

 exit(execvp(line,argv));

     }

    i++;
  }
}

//Child Function that displays avalible commands and takes in the users input to perform them

int child_fn(void* arg){
printf("Welcome to shell\n");
printf("--------------------------\n");
printf("Command list\n");
printf("quit "); printf(" Quit program\n");
printf("new"); printf(" Change host name\n");
printf("pid "); printf(" Get Process ID of current process\n");
printf("root "); printf("Change current root\n");
printf("net "); printf("Display network\n");
printf("--------------------------\n");
while(1){

char* line=malloc(sizeof(char)*1000);
printf("sh-4.1# ");
line=read_line();
parsing(&line[0]);
commandExec();
}
}

//Main function that houses the clone command used to create new namespace and network
int main(int arg, char* argv[]) {


 cpid = clone(child_fn, child_stack+1048576, CLONE_NEWUTS |CLONE_NEWNET | CLONE_NEWNS | SIGCHLD, argv+1);


  waitpid(cpid, NULL, 0);
  
}

