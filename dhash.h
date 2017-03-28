#define START_SIZE 1009
#define MAX_WORD 20

struct word{
   char *string;
};
typedef struct word Word;

struct hashtable{
   int size;
   Word **array;
};
typedef struct hashtable HashTable;

void hashfile(FILE *fp, HashTable *dict);
void insert_in_hash(HashTable *dict, char *str);
float search(HashTable *dict, char *str);
float findaverage(FILE *scan, HashTable *dict);
void initialise(Word **array, int length);

