#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "bin_tree.h"

void insert_values(node ** tree, char *idx, int *val, int nrVals)
{
    node *temp = NULL;
    
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        strcpy(temp->index,idx);
        temp->index[3] = '\0';
        temp->nrValues = nrVals;
        temp->values = val;
        *tree = temp;
        return;
    }
    int sCmp = strcmp(idx,(*tree)->index);
    if(sCmp < 0)
    {
        insert_values(&(*tree)->left, idx, val, nrVals);
    }
    else if(sCmp > 0)
    {
        insert_values(&(*tree)->right, idx, val, nrVals);
    } 
}

void insert(node ** tree, char *idx, int val)
{
    node *temp = NULL;
    
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        strcpy(temp->index,idx);
        temp->index[3] = '\0';
        temp->nrValues = 1;
        temp->values = (int*) malloc(sizeof(int));
        temp->values[0] = val;
        *tree = temp;
        return;
    }
    int sCmp = strcmp(idx,(*tree)->index);
    if(sCmp < 0)
    {
        insert(&(*tree)->left, idx, val);
    }
    else if(sCmp > 0)
    {
        insert(&(*tree)->right, idx, val);
    } 
    else if(sCmp == 0) {
        (*tree)->nrValues++;
        (*tree)->values = realloc((*tree)->values,(*tree)->nrValues * sizeof(int));
        (*tree)->values[(*tree)->nrValues - 1] = val;	
    }
}

void print_preorder(node * tree)
{
    if (tree)
    {
        printf("%s\n",tree->index);
        print_preorder(tree->left);
        print_preorder(tree->right);
    }

}

void load_index_from_file(node ** tree, const char *filename) {
   FILE *fp;
   char *arquivo;
   char *linha;
   char *index;
   char *nrVals;
   int nrValues;
   char *positions;
   char *curpos;
   int *values;
   int i;
   fp = fopen(filename , "rb");
   if(fp == NULL) 
   {
      perror("Erro abrindo arquivo");
      return;
   }
   
   off_t filesize = fsize(filename);

   arquivo = malloc((sizeof(char) * filesize) + 1) ;
   size_t lido = fread(arquivo, sizeof(char), (sizeof(char) * filesize) + 1, fp);
   arquivo[lido] = '\0';
   fclose(fp);
   linha = strsep(&arquivo, "\n");
   while (linha != NULL) {
      index = strsep(&linha, ",");
      if (index == NULL) break;
      nrVals = strsep(&linha, ",");
      if (nrVals == NULL) break;
      nrValues = atoi(nrVals);
      positions = strsep(&linha, ",");
      values = (int*) malloc(sizeof(int) * nrValues);
      for (i = 0;i<nrValues;i++) {
        curpos = strsep(&positions, " ");
        values[i] = atoi(curpos);
      }
//      printf("%s --> %i\n", index,nrValues);
      insert_values(tree, strdup(index), values, nrValues);
      linha = strsep(&arquivo, "\n");
   }
}

void save_index_to_file(node * tree, FILE *fp) {
    int i;
    if (tree)
    {
        fprintf(fp,"%s,%i,",tree->index,tree->nrValues);
        for (i=0; i<tree->nrValues; i++) {
           fprintf(fp, "%i",tree->values[i]);
           if (i< (tree->nrValues - 1)) {
               fprintf(fp, " ");
           }
        }
        fprintf(fp, "\n");
        save_index_to_file(tree->left, fp);
        save_index_to_file(tree->right, fp);
    }
}

void print_inorder(node * tree)
{
    int i;
    if (tree)
    {
        print_inorder(tree->left);
        printf("%s --> ",tree->index);
        for (i=0; i<tree->nrValues; i++) {
           printf("%i ",tree->values[i]);
        }
        printf("\n");
        print_inorder(tree->right);
    }
}

void print_postorder(node * tree)
{
    if (tree)
    {
        print_postorder(tree->left);
        print_postorder(tree->right);
        printf("%s\n",tree->index);
    }
}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* search(node ** tree, char *idx)
{
    if(!(*tree))
    {
        return NULL;
    }

    int sCmp = strcmp(idx,(*tree)->index);
    if(sCmp < 0)
    {
        search(&((*tree)->left), idx);
    }
    else if(sCmp > 0)
    {
        search(&((*tree)->right), idx);
    }
    else if(sCmp == 0)
    {
        return *tree;
    }
}

/*void main() {

   node *root;
   node *tmp;
   root = NULL;

   insert(&root, "AER",0);
   insert(&root, "ASF",1);
   insert(&root, "ASF",2);
   insert(&root, "CEK",3);
   insert(&root, "CEK",4);
   insert(&root, "DME",5);
   insert(&root, "DME",6);
   insert(&root, "DME",7);
   insert(&root, "DME",8);

   print_inorder(root);
}*/
