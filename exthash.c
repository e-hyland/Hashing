#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "extension.h"

#define LIMIT 0.6
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define PRIME 31

Word **bigger_array(HashTable *dict, int newlength);
int calcsize(int x);
int is_prime(int v);
unsigned int hash_all(char *str);
int find_space(Word **array, int hash, char *str, HashTable *dict);
float probe_array(HashTable *dict, char *str, int hash);
Word *AllocateWord(Word *ptr);
unsigned long hashscnd(char *str);
unsigned long hashthird(char *str);

float insert_in_hash(HashTable *dict, char *str)
{
   static int cnt =0;
   int n=0, hash=0;
   float x=0;
   
   cnt++;

   if (cnt>=(dict->size*LIMIT)){ /* If array is 60% full */
      n=calcsize(dict->size); /* Doubles array size and finds closest prime */
      dict->array=bigger_array(dict, n); /* Hashes words into new array */
      dict->size=n;
   }

   hash=(hash_all(str))%(dict->size);
   x = find_space(dict->array, hash, str, dict); /* Finds empty cell in hash table and inserts word */
   return x;
}

int find_space(Word **array, int hash, char *str, HashTable *dict)
{
   int stride=0;
   int cnt=0;
   char *temp;
   int length = dict->size;

   temp = (char *)malloc(MAX_WORD*sizeof(char));
   strcpy(temp, str);

   if (array[hash]->string==NULL){
      array[hash]->string = temp; /* Insert word */
      return 0;
   }
   while (array[hash]->string!=NULL){
      cnt++;
      stride = MAX(((hash_all(str)/length)%length),1); /* Calculate linear probe */
      if (hash<stride){
         hash=length-(stride-hash); /* Ensure wrap around */
      }
      else {
         hash=hash-stride;
      }
   }
   array[hash]->string = temp; /* Insert word */
   return cnt;
}

float search(HashTable *dict, char *str)
{
   int hash=0;
   float x=0.0;

   hash=(hash_all(str))%dict->size;

   if ((dict->array[hash]->string==NULL) || /* If string[0] is NULL, word does not exist in hash table */
       (strcmp(dict->array[hash]->string, str)==0)){
      x++;
   }
   else {
      x= x + probe_array(dict, str, hash); /* Keeps count of number of look ups as we probe array */
   }

   return x;
}

/* Hash existing values into larger hash table, "newtable" */
Word **bigger_array(HashTable *dict, int newlength)
{
   int hash=0;
   int i;
   Word **newarray = NULL;

   newarray = (Word **)malloc(newlength*sizeof(Word));
   initialise(newarray, newlength);

   for (i=0;i<dict->size;i++){
      if (dict->array[i]->string!=NULL){ /* Only hashes strings if they have been filled */
         hash = hash_all(dict->array[i]->string)%dict->size; /* Calculate new hash number */
         find_space(newarray, hash, dict->array[i]->string, dict); /* Inserts word into appropriate cell */
         
      }
      free(dict->array[i]); 
   }
   free(dict->array);
   return newarray;
}

float probe_array(HashTable *dict, char *str, int hash)
{
   int stride=0;
   float x=0.0;
   int length = dict->size;

   stride=MAX(((hash_all(str)/length)%length),1); /* Calculate linear probe */

   while (dict->array[hash]->string!=NULL){ /* Loop until we find empty cell or correct word */
      if (strcmp(dict->array[hash]->string, str)==0){
         x++;
         return x;
      }
      if (hash<stride){
         hash=length-(stride-hash); /* Wrap around */
         x++;
      }
      else{
         hash = hash-stride;
         x++;
      }
   }
   return x;
}

void initialise(Word **array, int length)
{
   int i;

   for (i=0;i<length;i++){   
      array[i]=AllocateWord(array[i]);
   }

}

Word *AllocateWord(Word *ptr)
{
   ptr = (Word *)malloc(sizeof(Word));

   if (ptr==NULL){
      printf("Cannot allocate word\n");
      exit(1);
   }
   ptr->string=NULL;
   return ptr;
}

/* Hash function */
unsigned int hash_all(char *str)
{
   int i;
   int n;
   unsigned int hash=0;

   n = strlen(str);

   for (i=0;i<n;i++){
      hash = str[i]+(PRIME*hash);
   }

   return hash;
}

/* Hash function */
unsigned long hashthird(char *str)
{
   unsigned long hash = 0;
   int c;

   while ((c = *str++)){
      hash = c + (hash << 6) + (hash << 16) - hash;
   }
   return hash;

}

/* Hash function */
unsigned long hashscnd(char *str)
{
    unsigned long hash = 5381;
    int c=0;

    while ((c = *str++)){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
   }
    return hash;
}


int calcsize(int x)
{
   int size=x+x;
   int i;

   for (i=0;;i++){
      size++; /* Add one to size until we find prime number */
      if (is_prime(size)){
         return size;
      }
   }
   return 0;
}

int is_prime(int v)
{
   int d;

   for (d=2;d<=(v-1);d++){
      if (v%d==0){ /* If modulo is 0, 'v' is not prime */
         return 0;
      }
   }
   return 1; 
}

