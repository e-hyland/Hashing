#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dhash.h"

int main(int argc, char **argv)
{
   FILE *fp, *scan;
   float average;
   HashTable *dict = NULL;

   dict = (HashTable *)malloc(sizeof(HashTable));
   dict->size = START_SIZE;

   if (argc == 3){
      if ((fp = fopen(argv[1], "r")) == NULL){
         fprintf(stderr, "Error, please try again. Please enter valid filename.\n"); 
         return 1;
      } 
      hashfile(fp, dict); /* Reads in file line by line and inserts words into hash table */
      if ((scan = fopen(argv[2], "r")) == NULL){
         fprintf(stderr, "Error, please try again. Please enter valid filename.\n"); 
         return 1;
      }
      average=findaverage(scan, dict); /* Reads in 2nd file and looks up words in 1st file */
      printf("Number of lookups = %.3f\n", average);
   }
   else {
      fprintf(stderr, "Error, please enter in correct format.\n");
   }
   free(dict);
   return 0;
}

void hashfile(FILE *fp, HashTable *dict)
{
   char str[MAX_WORD]; 

   dict->array = (Word **)malloc(dict->size*sizeof(Word));
   initialise(dict->array, dict->size); /*Sets all string[0] = '\0' */

   while (fscanf(fp,"%s", str) != EOF){
      str[strlen(str)+1]='\0'; /* Add NULL character to make it a string */
      insert_in_hash(dict, str); /* Finds empty cell in hash table */
   }  
   fclose(fp);
}

float findaverage(FILE *scan, HashTable *dict)
{
   char str[MAX_WORD];  
   float x=0.0;
   float average=0.0;
   int cnt=0;

   while (fscanf(scan,"%s", str) != EOF){
      cnt++; /* Counts total number of words to computer average */
      x = x + search(dict,str); /* Searches hash table for str and counts lookups */
   }
   fclose(scan);
   average = x/cnt;
   return average;
}

