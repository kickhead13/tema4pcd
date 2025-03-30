/* Nume: Ana Alexadnru-Gabriel
 * IR3 Grupa 2
 * Descriere: 
 *  Rezolvare exercitiu 4 tema 4.
 * */

#include <sys/stat.h>  /*mkdir(..)*/
#include <unistd.h>    /*write(..)*/
#include <fcntl.h>     /*open(..)*/
#include <stdlib.h>    /*malloc(..)*/
#include <string.h>    /*strlen strcpy strcat*/


/*pentru tratare erori de creare*/
#define __CREATE_ERR(val)\
  if((val) == -1) {\
    write(2, "eroare creare\n", 14);\
    return -1;\
  }

/*obtinere cale globala a directorului curent si appenduire numele folderului nou*/
char *cwd_dir(char *name) {
  char cwdb[300];
  char * buff = (char*)malloc(sizeof(char) *300);;
  strcpy(buff, getcwd(buff, 300));
  strcat(buff, "/");
  strcat(buff, name);
  return buff;
}


/*crearea structurii dorite*/
int main(int _, char **__, char **___) {
  int val = mkdir(cwd_dir("d1"), S_IROTH | S_IXOTH);
  __CREATE_ERR(val);
  val = mkdir(cwd_dir("d1/d2"), S_IROTH | S_IWOTH);
  __CREATE_ERR(val);
  val = mkdir(cwd_dir("d1/d2/d3"), S_IRUSR | S_IXUSR | S_IXGRP | S_IWGRP | S_IWOTH);
  __CREATE_ERR(val);
  val = open("d1/d2/d3/f4", O_CREAT, S_IRWXU | S_IXGRP | S_IROTH);
  __CREATE_ERR(val);
  val = open("d1/f2", O_CREAT,  S_IROTH);
  __CREATE_ERR(val);
  val = open("d1/f3", O_CREAT,  S_IWOTH | S_IXOTH);
  __CREATE_ERR(val);
  val = open("d1/d2/f1",O_CREAT,  S_IRWXO);
  __CREATE_ERR(val);
  val = symlink(cwd_dir("d1/f4"), cwd_dir("d1/d2/f1")); /*nu functioneaza*/
  __CREATE_ERR(val);
  val = chmod(cwd_dir("d1/f4"), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  __CREATE_ERR(val);
  val = link(cwd_dir("d1/f5"), cwd_dir("d1/d2/f1")); /*nu functioneaza*/
  __CREATE_ERR(val);
  val = chmod(cwd_dir("d1/f5"), S_IRWXO);
  __CREATE_ERR(val);
  return 0;
}

/* Erorile de creare sunt tarate
 * Compilare: 
 *  $ gcc -o ./target/ex4 ./src/ex4.c
 *
 * */
