#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct HashItem
{
    char* value;
    char* key;
    HashItem* next;
}HashItem;

typedef struct HashTable
{
    HashItem* data;
    int size;
    int maxsize;
}HashTable;

unsigned int hashtable_hash(HashTable* this_,char* key,int sizeType) {
    int hash_value = 0x425;
    for(int i = 0; i< sizeType;i++){
        hash_value ^= key[i];
        hash_value += hash_value;
    }
    return abs(hash_value) % this_->maxsize;
}
HashTable* _hashtable_create(int numElements,int sizeType)
{
    HashTable *this_ = (HashTable*)malloc(sizeof(HashTable));
    this_->maxsize = numElements < 11 ? numElements = 10 : numElements;
    this_->data = (char*)calloc(this_->maxsize,sizeType);
    this_->size = 0;
    return this_;
}

void* _hash_put(HashTable* this_,char* value,char* key,int sizeValue,int sizeKey)
{
    int index = hashtable_hash(this_,key,sizeKey);
    return (void*)(&this_->data[0])+ (sizeValue*index);
}


bool _hash_search()
{
    
}

//key must be char*
#define hashtable_create(numElements,type) _hashtable_create(numElements,sizeof(type))
#define hashtable_put(h,v,k) memcpy( _hash_put(h,(char*)&v,(char*)&k,sizeof(typeof(v)),sizeof(typeof(k))) ,&v, sizeof(typeof(v)))
#define hash_get(l,type,index) ((type*)l->data)[index]
#define hash_search(h,key) hashtable_hash(h,(char*)&(typeof(key)){key},sizeof(typeof(key)))


int main()
{
    /* Not made yet
    HashTable* map = hashtable_create(10,int);
    hashtable_put(map,(int){40},(int){40});

    for(int i = 0; i < map->maxsize;i++)
    {
        printf("%d\n",hash_get(map,int,i));
    }

    printf("Found 40 at: %d",hash_search(map,421));


    */

    return 0;
}