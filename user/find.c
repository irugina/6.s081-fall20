#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* extract(char *path){
  /*
  input: long path (e.g. a/b/c)
  return: last entry on path (e.g. c)
  edgecase: if input is filename -> return filename
  */
  
  char *p;
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void find(char *path, char *pattern){
  /* recursive implementation of find
   * input: start path and pattern
   * prints out path to matches
   */
  
  int fd;
  struct dirent de;
  struct stat st;
  char *filename;

  //first: determine if path points to a file or a directory
  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // if file - check for matches to pattern
  switch(st.type){
  case T_FILE:
    filename = extract(path);
    if (strcmp(filename, pattern) == 0){
      printf(path);
      printf("\n");
    }
    break;
  
  // if directory - recurse in each file
  case T_DIR:
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name,"..") == 0)
        continue;

      //create newpath s.t. newpath = path + "/" + de
      char newpath[512], *p;
      strcpy(newpath, path);
      p = newpath+strlen(newpath);
      *p++ = '/';
      memmove(p, de.name, DIRSIZ);
      find(newpath, pattern);
  }
  }
  close(fd);

}

int
main(int argc, char *argv[])
{
  char *path;
  char *pattern;

  if(argc < 2){
    printf("find: need pattern arg");
    exit(1);
  }
  if (argc == 2){
    path = ".";
    pattern = argv[1];
  }
  if (argc > 2){
    path = argv[1];
    pattern = argv[2];
  }

  find(path, pattern);
  exit(0);
}
