#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"


int
main(int argc, char *argv[]){
  int i;
  int MAX_ARG_LEN = 10;
  char new_argv[MAXARG][MAX_ARG_LEN];

  //see what the command was
  if (argc < 2){
    printf("xargs: requires command as arg");
    exit(1);
  }
  for (i = 1; i < argc; i++){
    strcpy(new_argv[i-1], argv[i]);
  }

  //read arguments from stdin
  char ch;
  int argv_idx = argc-1;
  char *p, buf[MAX_ARG_LEN];
  p = buf;

  while (read(0, &ch, 1)){

    //if we read a normal char - not space or newline
    if (ch != '\n' && ch != ' '){
      *p = ch;
      p++;
    }

    //if we read a space or finished the whole line
    if (ch ==  ' ' || ch ==  '\n'){
      *p = '\0';
      strcpy(*(new_argv+argv_idx), buf);
      argv_idx += 1;
      p = buf;
    }

    //if we read a newline
    if (ch ==  '\n'){
      // change char[][] to *char[]
      char * final [argv_idx + 1];
      for (int i = 0; i < argv_idx; ++i)
        final[i] = new_argv[i];
      final[argv_idx] = '\0';
      // fork -> exec
      if (fork() > 0){
        exec(final[0], final);
      }
      else{
	wait( (int*) 0);
	argv_idx = argc-1;
	p = buf;
      }
    }
  }
  exit(0);
}
