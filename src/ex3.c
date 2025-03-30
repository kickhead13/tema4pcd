/* Nume: Ana Alexandru-Gabriel
 * IR3 Grupa 2
 * Descriere program:
 *  Acest program primeste ca atribut numele unui folder, iar apoi afiseaza informatii
 *  legate de fisierle continute de catre acel folder.
 * Utilizare:
 *  ./target/ex3 ./src/
 * */

#include <unistd.h>  /*write(..) read(..)*/
#include <dirent.h>  /*DIR struct dirent opendir readdir*/
#include <sys/stat.h>/*stat*/
#include <string.h>  /*strlen(..)*/ 
#include <stdio.h>   /*printf(..)*/
#include <pthread.h>
#include <stdlib.h>  /*malloc*/

/* struct utilizat pentru pasarea spre pthread-uri */
struct info_t {
  struct dirent* dir;
  char dir_name[256];
};

/* macro pentru cast-uri din pthread-uri (deoarece 
 * pthread se asteapta ca pointer-ul spre functie sa fie
 * de tipul void * (*)(void *) - ca atare cand pasam atributele
 * de tip struct info_t* acestea se vor descompune in void*, iar
 * astfel este necesar acest cast)
 * )*/
#define _CAST(ptr) ((struct info_t*)(ptr))

/* t_work (thread work functie pasata spre pthread-uri pentru afisarea
 * de informatii legate de un anumit fisier primit prin parametrul functiei
 *  void * info   --  informatii legate de runtime utilizate in rularea
 *                    pthread-ului
 *  return void*  --  NTR
 * */
void * t_work(void * info) {
  char build[256] = {0};
  struct stat f_stat;
  strcat(build, _CAST(info)->dir_name);
  strcat(build, _CAST(info)->dir->d_name);
  if(stat(build, &f_stat) == 0) {
    printf("---file:%s---\nbytes: %jd   inode: %jd   filetype: %d\nlast accessed @: %jd   last modified @: %jd   created @: %jd\n\n", build, f_stat.st_size, _CAST(info)->dir->d_ino, _CAST(info)->dir->d_type, f_stat.st_atime, f_stat.st_mtime, f_stat.st_ctime);
  }  
}

int main(int argc, char **argv, char **_) {
  DIR *dr;

  /*verificam ca utilizatorul a apelat corespunzator utilitara*/
  if(argc < 2) {
    write(2, "2nd arg must be directory name\n", strlen("2nd arg must be directory name\n"));
    return -1;
  }

  /*deschidem folder-ul primit din coommand-line*/
  if((dr = opendir(argv[1])) == NULL) {
    write(2, "dir error\n", 11);
    return -1;
  }
  struct dirent *dirents;
  pthread_t pt;

  /* mainloop-ul : parcurgem folder-ul si cream thread-uri pentru fiecare fisier*/
  while((dirents = readdir(dr)) != NULL) {
    struct info_t * info = (struct info_t*)malloc(sizeof(struct info_t));
    info->dir = dirents;
    strcpy(info->dir_name, argv[1]);
    pthread_create(&pt, NULL, &t_work, info);
    pthread_join(pt, NULL);
  }
  return 0;
}

/* Erori:
 *  au fos tratate in intregime
 * Compilare: 
 *  $ gcc -o ./target/ex3 ./src/ex3.c
 * */
