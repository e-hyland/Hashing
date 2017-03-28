#define START_SIZE 1009
#define MAX_WORD 20

struct node{
   char *word;
   struct node *next;
};
typedef struct node Node;

struct hashtable{
   int size;
   Node **array;
};
typedef struct hashtable HashTable;

void insert_in_hash(HashTable *dict, char *str);
void readfile(FILE *fp, HashTable *dict);
float findaverage(FILE *scan, HashTable *dict);
float search(HashTable *dict, char *str);
void initialise(Node **array, int length);


