#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "extension.h"

int main(int argc, char **argv)
{
   FILE *fp, *scan;
   float average;
   int cnt=0;
   HashTable *dict = NULL;

   dict = (HashTable *)malloc(sizeof(HashTable));
   dict->size = START_SIZE;

   if (argc == 3){
      if ((fp = fopen(argv[1], "r")) == NULL){
         fprintf(stderr, "Error, please try again. Please enter valid filename.\n"); 
         return 1;
      } 
      cnt=hashfile(fp, dict); /* Reads in file line by line and inserts words into hash table */
      if ((scan = fopen(argv[2], "r")) == NULL){
         fprintf(stderr, "Error, please try again. Please enter valid filename.\n"); 
         return 1;
      }
      average=findaverage(scan, dict); /* Reads in 2nd file and looks up words in 1st file */
      printf("Number of lookups = %.3f\n", average);
      distribution(dict, cnt);
   }
   else {
      fprintf(stderr, "Error, please enter in correct format.\n");
   }
   free(dict);
   return 0;
}

int hashfile(FILE *fp, HashTable *dict)
{
   char str[MAX_WORD]; 
   float cnt=0, col=0;

   dict->array = (Word **)malloc(dict->size*sizeof(Word));
   initialise(dict->array, dict->size); /*Sets all string[0] = '\0' */

   while (fscanf(fp,"%s", str) != EOF){
      cnt++;
      str[strlen(str)+1]='\0'; /* Add NULL character to make it a string */
      col = col + insert_in_hash(dict, str); /* Finds empty cell in hash table */
   }  
   fclose(fp);
   printf("Collisions per word = %f\n", col/cnt);
   return cnt;
}

float findaverage(FILE *scan, HashTable *dict)
{
   char str[MAX_WORD];  
   float x=0.0, cnt=0.0, average=0.0;

   while (fscanf(scan,"%s", str) != EOF){
      cnt++; /* Counts total number of words to computer average */
      x = x + search(dict,str); /* Searches hash table for str and counts lookups */
   }
   fclose(scan);
   average = x/cnt;
   return average;
}

/* Counts the number of words in each 10% of the hash table */
int distribution(HashTable *dict, int cnt)
{
   int prcnt=0, i;
   float distr[PERCENT];

   for (i=0;i<PERCENT;i++){
      prcnt = countwords(dict);
      distr[i]=prcnt;
   }
   printarray(distr, cnt);   
   return 0;
}

int countwords(HashTable *dict)
{
   int n=0,i, cnt=0, m=0;
   static int j=1;

   n=(dict->size/PERCENT)*(j-1);
   m=(dict->size/PERCENT)*j;

   for (i=n;i<m;i++){
      if (dict->array[i]->string!=NULL){
         cnt++;
      }
   }
   j++;
   return cnt;
}

void printarray(float *distr, int cnt)
{
   int i;
   float x = 0.0;

   for (i=0;i<PERCENT;i++){
      x = (distr[i]/cnt)*(PERCENT*PERCENT);
      printf("%d-%d%% = %d, Percentage of total = %.2f\n", i*PERCENT, 
                                      (i+1)*PERCENT, (int)distr[i], x);
   }

}
