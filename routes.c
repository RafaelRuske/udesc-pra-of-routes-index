#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>
#include "bin_tree.h"

#define REG_LEN 68

off_t fsize(const char *filename) {
    struct stat st; 

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1; 
}

int main()
{
   FILE *fp;
   char *linha;
   char *arquivo;
   char *arquivo_copy;
   char *token;
   int campos_tam[9];
   int i;
   int padLen, toklen;
   int nrLinha = 0;
   char pesquisa[5];
   node *root;
   root = NULL;


   for (i=0;i<9;i++) {
        campos_tam[i] = 0;
   }
 
   /* opening file for reading */

   fp = fopen("routes.out" , "rb");
   if(fp == NULL) 
   {
      perror("Erro abrindo arquivo");
      return(-1);
   }
   
   off_t filesize = fsize("routes.out");

   arquivo = malloc((sizeof(char) * filesize) + 1) ;
   arquivo_copy = malloc((sizeof(char) * filesize) + 1);
   size_t lido = fread(arquivo, sizeof(char), (sizeof(char) * filesize) + 1, fp);
   arquivo[lido] = '\0';
   fclose(fp);
   memcpy(arquivo_copy, arquivo, sizeof(char) * filesize);

   if (fsize("routes.idx") != -1) {
      //tem indice, carregar
      printf("Using index!\n");
      load_index_from_file(&root, "routes.idx");
   } else {
      //nao tem, gerar
      printf("Generating index!\n");
	   linha = strsep(&arquivo, "\n");
	   while (linha != NULL) {
	      i = 0;
	      
	      token = strsep(&linha, ",");
	      while (token != NULL) {
		  if (i == 2) {
		      insert(&root, token, nrLinha);
		  }
		  toklen = strlen(token);
		  if (token[toklen-1] == '\r') {
		      token[toklen-1] = '\0';
		      toklen -= 1;
		  }
		if (toklen > campos_tam[i]) {
		       campos_tam[i] = toklen; 
		      }
		token = strsep(&linha, ",");
		i++;
	      }
	      linha = strsep(&arquivo, "\n");
	      nrLinha++;
	   }
           FILE *indexout;
           indexout = fopen("routes.idx", "w");
           save_index_to_file(root, indexout);
           fclose(indexout) ;
    }
   free(arquivo);

   node *tmp;
   int cnt;
   char tmpReg[REG_LEN + 1];
   printf("Aeroporto: ");   
   while(fgets(pesquisa,5,stdin) != NULL) {
        pesquisa[3] = '\0';
        tmp = search(&root,pesquisa);
        if (tmp != NULL) {
                printf("Destinos: ");   
		for (cnt=0;cnt<tmp->nrValues;cnt++) {
		     //printf("cnt %i nrValues %i\n",cnt,tmp->nrValues);
		     memcpy(tmpReg,arquivo_copy+(tmp->values[cnt]*REG_LEN),sizeof(char)*REG_LEN);
		     tmpReg[REG_LEN-1] = '\0';
                     linha = tmpReg;
                     token = strsep(&linha, ","); token = strsep(&linha, ","); token = strsep(&linha, ","); token = strsep(&linha, ","); token = strsep(&linha, ",");
		     printf("%s,",token);
		}
                printf("\n");
        } else {         
               printf("Aeroporto Inexistente!\n");    
        } 
        printf("Aeroporto: ");   
   } 

   free(arquivo_copy);
   return(0);
}
