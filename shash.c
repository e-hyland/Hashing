#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shash.h"

#define LIMIT 5
#define PRIME 31
#define START 2

Node **bigger_array(HashTable *dict, int length);
int calcsize(int x);
int is_prime(int v);
unsigned int hash_all(char *str);
int find_space(int hash, Node **array, char *str);
float search_lnklist(HashTable *dict, char *str, int hash);
Node *AllocateListNode(char *str);
Node *AllocateNode(Node *ptr);
void freenodes(HashTable *dict, int x);
int insert(int hash, Node **array, Node *str);

void insert_in_hash(HashTable *dict, char *str)
{
   int n=0, hash=0;
   int listlen=0;

   hash=(hash_all(str))%(dict->size);
   listlen = find_space(hash, dict->array, str); /* Inserts word into table and measures 
                                                    linked list length if one exists */

   if (listlen>=LIMIT){ /* If we have a linked list of >= 5 words long where str was inserted, resize array */
         n=calcsize(dict->size); /* Find prime number closest to double of current size */
         dict->array=bigger_array(dict, n); /* Re-hash words into larger array - do not check linked list 
                                               length when building new array as this would make the program 
                                               very slow and the array unreasonably large */
         dict->size=n;
   }
}

int find_space(int hash, Node **array, char *str)
{
   Node *ptr=NULL;
   Node *current = array[hash];
   char *temp;
   int cnt=0;
 
   temp = (char *)malloc(MAX_WORD*sizeof(char));
   strcpy(temp, str);

   if (current->word == NULL){
      current->word = temp; /* Insert word into table */
      return 1;
   }
   else {
      ptr = AllocateListNode(temp); /* Allocate node outside of array */
      while (current->next!=NULL){
         cnt++;
         current=current->next;
      }
      current->next = ptr; /* Insert word at end of list */   
   }
   return cnt;
}

/* Hash existing values into larger hash table, "newtable" */
Node **bigger_array(HashTable *dict, int length)
{
   int hash=0, i;
   Node **newtable = NULL;
   Node *ptr;

   newtable = (Node **)malloc(length*(sizeof(Node)));
   initialise(newtable, length);

   for (i=0;i<dict->size;i++){
      if (dict->array[i]->word!=NULL){ /*Don't re-hash empty cells */
         hash = hash_all(dict->array[i]->word)%length;
         find_space(hash, newtable, dict->array[i]->word); /* Insert word */
         ptr = dict->array[i]->next; 
         while (ptr!=NULL){ /* Re-hash words in linked list if necessary */
            find_space(hash, newtable, ptr->word);
            ptr=ptr->next;
         }
         freenodes(dict, i);
      }        
   }
   free(dict->array);
   return newtable;
}

void initialise(Node **array, int length)
{
   int i;
   Node *ptr=NULL;

   for (i=0;i<length;i++){  
     array[i] = AllocateNode(ptr);
   }

}

float search(HashTable *dict, char *str)
{
   int hash=0;
   float x=0.0;

   hash=hash_all(str)%(dict->size); 

   if ((dict->array[hash]->word==NULL) || /* If string[0] is NULL, word does not exist in hash table */
       (strcmp(dict->array[hash]->word, str)==0)){
      x++;
   }
   else {
      x= x + search_lnklist(dict, str, hash);
   }
   return x;
}

float search_lnklist(HashTable *dict, char *str, int hash)
{
   float x=1.0;
   Node *current=dict->array[hash]->next;

   while (current!=NULL){ /* Looks through linked list */
      if (strcmp(current->word, str)==0){
         x++;
         return x;
      }         
      current = current->next;
      x++;
   }
   
   return x;
}

/* Free node in array and linked list nodes if they exist */
void freenodes(HashTable *dict, int x)
{
   Node *current = dict->array[x];
   Node *prev = current;

   current = current->next;
   while (current!= NULL){
      free(prev); 
      prev=prev->next;
      current = current->next; 
   }
   free(prev);

}

/* Hash function */
unsigned int hash_all(char *str)
{
   int i;
   int n;
   unsigned int hash=0;

   n = strlen(str);

   for (i=0;i<n;i++){
      hash = str[i]+PRIME*hash;
   }

   return hash;
}

/* Allocating node if we are forming linked list with str */
Node *AllocateListNode(char *str)
{
   Node *p;

   p = (Node *)malloc(sizeof(Node));

   if (p==NULL){
      printf("Cannot allocate word\n");
      exit(1);
   }
   p->word=str;
   p->next=NULL;
   return p;
}

/* Initialising array to NULL */
Node *AllocateNode(Node *ptr)
{
   ptr = (Node *)malloc(sizeof(Node));

   if (ptr==NULL){
      printf("Cannot allocate word\n");
      exit(1);
   }

   ptr->next=NULL;
   ptr->word=NULL;
   return ptr;
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

   for (d=START;d<=(v-1);d++){
      if (v%d==0){ /* If modulo is 0, 'v' is not prime */
         return 0;
      }
   }
   return 1; 
}
