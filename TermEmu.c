/*
 * Steven Lim
 * CS 326 Assignment 2: Basic Terminal
 * This is code for a basic shell that can perform three commands:
 * cp(copy file), ls(list directory), and kill(terminate process)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/dir.h>
#include <signal.h>

/**
 * Struct containing available shell commands
 */
char *commands[] = {
  "cp", "ls", "kill", "end"
};
/********************Shell, User Activated Functions****************************/
//Function Declarations
int cp(char **args);
int ls(char **args);
int kills(char **args);
int end(char **ar);

/**
 * Struct containing available functions to suit commands
 * shellFunc is the array of function pointers
 */
int (*shellFunc[]) (char **) = {
  &cp,
  &ls,
  &kills,
  &end
};

/**
  * Copies a file from a source to an output destination
  * Returns false to continue shell operation
  */
int cp(char *args[]){
  char *source = args[1];
  char *destination = args[2];
  FILE *fileSrc, *fileDest;
  int charRead;

  //Check if args are empty
  if(args[1] == NULL || args[2] == NULL){
    printf("Please input a source and destination, ie. cp source destination\n");
    return false;
  }
  printf("Copying %s to %s..\n", args[1], args[2]);

 //Check if source can be read and dest can be written to
  fileSrc = fopen(source, "r");
  fileDest = fopen(destination, "w");
  if (fileSrc == NULL || fileDest == NULL){
    printf("An error has occurred, check your arguments\n");
    exit(EXIT_FAILURE);
  }
  //While a char is not at the end-of-file, typically -1
  while((charRead = fgetc(fileSrc)) != EOF){
    //Write into the destination file
    fputc(charRead, fileDest);
  };

  fclose(fileSrc);
  fclose(fileDest);
  printf("...Successful\n");
  //don't exit
  return false;
}

/**
  * Lists the working directory file structure
  * Returns false to continue shell operation
  */
int ls(char *args[]){
  printf("Listing contents of the directory...\n");

  char cwd[4096];
  getcwd(cwd, sizeof(cwd));
  DIR *directory;
  struct dirent *fileName;
 //!= ('.' || ".."|| " .. " || sizeof(cwd)))
  directory = opendir(cwd);
  while(fileName = readdir(directory)) {
     if((strcmp(fileName->d_name, ".") == 0) || (strcmp(fileName->d_name, "..") == 0)) {
       //Do nothing if . or .. is detected
     }
     else{
       printf("\t%s \n", fileName -> d_name);
     }
  }

  free(fileName);
  free(directory);

  return false;
}

/**
  * Kill Command forcibly terminates a running process
  * Returns false to continue shell operation
  * Try using shell command 'top' to check for PIDs
  */

int kills(char *args[]){
  printf("Executing kill command...\n");
  if(args[1] == NULL){
    printf("Please enter a valid Process ID");
    return false;
  }

  if(kill(atoi(args[1]), 0) == 0){
    //At least one signal was sent, meaning the process still exists
    kill(atoi(args[1]), SIGKILL);
  }
  else{
    printf("PID does not exist..?");
  }


  return false;
}

/**
  * Returns true to quit the program
  */
int end(char *args[]){
  return true;
}

/*
 * Reads user input from stdin and breaks it into tokenized commands
 * @Returns the tokenzied char array
 */
char **parseCommands(char delimiters[], char *inputBuffer){
  char **tokenCommands;
  char *token;
  ssize_t sizeOfBuffer = 0;
  //Note, when the line ptr input is NULL and Buffer size is set to 0,
  //getline() will handle allocatation and deallocation of the buffer
  getline(&inputBuffer, &sizeOfBuffer,stdin);
    //printf("token Commands buffer length: %ld \n", sizeof(inputBuffer));

  //Allocate tokenCommands
  tokenCommands = (char **)malloc(sizeof(inputBuffer) * sizeof(char*));
    //printf("token Commands buffer length: %ld \n", sizeof(tokenCommands));

  //Check for allocation error
  if(!tokenCommands || !inputBuffer){
    fprintf(stderr, "!Allocation error!\n");
    exit(0);
  }

  //Retrieve first token
  token = strtok(inputBuffer, delimiters);
  //Step through
  int i;
  for(i = 0; token != NULL; i++){
    tokenCommands[i] = token;
    token = strtok(NULL, delimiters);
  }
  //Null Terminate
  tokenCommands[i] = NULL;

  //Tests for if input was received
  //  printf("%s", tokenCommands[0]);
  //  printf("%s", tokenCommands[1]);
  //  printf("%s", tokenCommands[2]);
  return tokenCommands;
}

int executeCommands(char *args[]){
  int commandBuffer = sizeof(commands) / sizeof(char *);
  //User did not enter anything, just ignore and have them try again(not an error)
  if(args[0] == NULL){
    printf("!A command was not entered!\n");
    return false;
  }
  else{
    //A command argument was entered, parse it for the corresponding function
    for(int i = 0; i < commandBuffer; i++){
      if(strcmp(args[0], commands[i]) == 0){
        //printf("MATCH FOUND");

        if(strcmp(args[0], "kill") == 0){
          kills(args);
          return false;
        }

        return ((*shellFunc[i])(args));
      }
    }
    printf("Sorry, that command is unrecognized\n");
    return false;
  }
}

int main(int argc, char *argv[]) {
  char *inputBuffer = NULL;
  char delimiters[]= " \n\t";
  char cwd[4096];
  int isExiting = false;
  char **args;

  //Uses a char buffer with an arbitrary length to store the name of the current working directory
  if(getcwd(cwd, sizeof(cwd)) == NULL){
    //On failure, print error and exity
    printf("Error Status %d\n", errno);
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
  };

  //If commands were not passed beforehand, execute shell
  if(argc <= 1){
    printf("\nTerminal Successfully Started!\nTry Commands: cp, ls, kill, end\n");
    printf("Currently in: %s\n", cwd);

    do{
      printf("\n:> ");
      args = parseCommands(delimiters, inputBuffer);
      isExiting = executeCommands(args);

      //Deallocate arguments
      free(inputBuffer);
      free(args);
    }
    while(!isExiting);
  }
  else{
      //Execute command without the terminal
      isExiting = executeCommands(argv + 1);
  }


  return 0;
}
