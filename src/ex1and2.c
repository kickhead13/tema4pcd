/* Nume: Ana Alexandru-Gabriel
 * IR3 Grupa 2
 * Descriere: 
 *  Acest porgram creaza doua thread-uri joinable, unul citeste din fila in, iar celalat scrie in fila out.
 * Utilizare:
 *  ./target/ex1and2 
 * */

#include <stdlib.h>    /*malloc(..)*/
#include <unistd.h>    /*read(..)*/
#include <fcntl.h>     /*open(..)*/
#include <string.h>    /*strlen(..)*/
#include <pthread.h>   /*phtread...*/

/* marime chunk de citire/scriere (cati bytes se scriu/citesc odata) */
#define __CHUNK_SIZE 10

/* struct pentru pasare spre thread-uri la crearea acestora*/
typedef struct {
   char * path;
   char ** buff;
   ssize_t size;
}if_t;
#define of_t if_t; 

/* crearea pointer spre o variabila de tip if_t de pe heap
 *  char *const restrict path   --   numele fisierului
 *  char *const buff            --   buffer din care citim/scriem
 *  ssize_t size                --   lungime buffer de citit/scris
 *  return *if_t                --   pointer-ul creat
 * */
if_t * new_if_t(char *const restrict path, char *const buff, ssize_t size) {
  if_t * ret = (if_t *)malloc(sizeof(if_t));
  ret->size = size;
  ret->path = (char*)malloc(sizeof(char)*(strlen(path)+1));
  strcpy(ret->path, path);
  ret->buff = (char**)malloc(sizeof(char*));
  if(buff) *(ret->buff) = buff;
  else *(ret->buff) = NULL;
  return ret;
}


/* myRead citeste dintr-o fila specificat prin nume un anume numar de bytes
 * intr-un buffer
 *  char *path     -- cale spre fila
 *  char **buffer  -- buffer in care scriem
 *  ssize_t size   -- numar bytes de citit
 *  return ssize_t -- numar de bytes cititi (<0 eroare)
 * */
ssize_t myRead(char * path, char ** buffer, ssize_t size){
  int fd = open(path, O_RDONLY);
  ssize_t bytes_read = 0;
  ssize_t total_br = 0;
  if(fd == -1) return -1;
  *buffer = (char*)malloc(sizeof(char) * (size+1));
  if(*buffer == 0) return -2;
  while(size > 0) {
    bytes_read = read(fd, (*buffer) + total_br, (size < __CHUNK_SIZE ? size : __CHUNK_SIZE));
    if(bytes_read == -1) {close(fd); return -1;}
    total_br += bytes_read;
    size -= bytes_read;
  }
  close(fd);
  return total_br;
}

/* myWrite scrie intr-o fila specificata un numar de bytes dintr-un buffer
 *  char *path                    -- cale spre fila
 *  char *const restrict buffer   -- buffer din care scriem
 *  ssize_t size                  -- numar de bytes de scris
 *  return ssize_t                -- numar de bytes scrisi (<0 eroare)
 * */
ssize_t myWrite(char * path, char *const restrict buffer, ssize_t size){
  int fd = open(path, O_RDWR | O_CREAT, S_IRWXU);
  ssize_t bytes_written = 0;
  ssize_t total_bw = 0;
  if(fd == -1) return -1;
  while(size > 0) {
    bytes_written = write(fd, buffer + total_bw, (size < __CHUNK_SIZE ? size : __CHUNK_SIZE));
    if(bytes_written == -1) {close(fd); return -1;}
    total_bw += bytes_written;
    size -= bytes_written;
  }
  close(fd);
  return total_bw;
}

/*cast pentru pasare pointer spre pthread (explicat in ex3.c)*/
#define _CAST(ptr) ((if_t*)(ptr))

/* functii de citit/scris pentru pasat spre pthread-uri*/
void * i_thread(void *file) {
  ssize_t ret = myRead(_CAST(file)->path, _CAST(file)->buff, _CAST(file)->size);
  if(ret < 0) write(2, "eroare read\n", 12);
}
void * o_thread(void *file) {
  ssize_t ret = myWrite(_CAST(file)->path, *(_CAST(file)->buff), _CAST(file)->size);
  if(ret < 0) write(2, "eroare write\n", 13);
}


int main(int _, char **__, char**___) {
  char **buff = (char**)malloc(sizeof(char*));

  if_t * in = new_if_t("in", NULL, 10);
  if_t * out = new_if_t("out", "test", 4);

  pthread_t p1, p2;
  
  /* creare pthread-uri */
  pthread_create(&p1, NULL, &i_thread, in);
  pthread_create(&p2, NULL, &o_thread, out);
  pthread_join(p1,NULL);
  pthread_join(p2, NULL);

  /* eliberare memorie */
  free(in);
  free(out);
  if(*buff) free(*buff);
  if(buff) free(buff);
  return 0;
}

