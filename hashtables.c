#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define KEY_SIZE 128
typedef struct HashItem
{
    void* object; 
    char key[KEY_SIZE]; 
    struct HashItem* next;
}HashItem;

void hashitem_free(HashItem* item)
{
    free(item->object);
    free(item);
}

typedef struct HashTable
{
    HashItem** data;
    int size;
    int maxsize;
}HashTable;

int hashtable_index(HashTable* this_,char* key) {
    int hash_value = 0x425;
    for(int i = 0; i< KEY_SIZE;i++){
        hash_value ^= key[i];
        hash_value += hash_value;
    }
    hash_value = abs(hash_value) % this_->maxsize;
    return hash_value;
}

HashTable* hashtable_create(int numElements)
{
    HashTable *this_ = (HashTable*)malloc(sizeof(HashTable));
    this_->maxsize = numElements < 11 ? numElements = 10 : numElements;
    this_->data = (void*)calloc(this_->maxsize,sizeof(HashItem));
    this_->size = 0;
    return this_;
}

void* hastable_search(HashTable* this_,char* key)
{
    if(key == NULL){return NULL;}

    //search for the key
    HashItem* current = this_->data[hashtable_index(this_,key)];
    while(current != NULL && strcmp(current->key,key) != 0){current = current->next;}
    if(current == NULL){return NULL;}
    return current->object;
}


bool hashtable_insert(HashTable* this_,void* object,char* key)
{
    if(object == NULL || key == NULL){return false;}

    //if the key is already in the hashtable dont insert it
    assert(hastable_search(this_,key) == NULL && "The key is already in the hashtable"); // false   


    int index = hashtable_index(this_,key);

    //create a new
    HashItem* h = malloc(sizeof(HashItem));
    h->object = object;
    strcpy(h->key,key);

    //insert entry to linked list
    h->next = this_->data[index];
    this_->data[index] = h;
    

    this_->size++;
}


void hashtable_destroy(HashTable* this_)
{
    if(this_ == NULL){return;}
    for(int i = 0; i < this_->maxsize; i++){
        HashItem* current = this_->data[i];
        while(current != NULL){
            HashItem* next = current->next;
            hashitem_free(current);
            current = next;
        }
    }
    free(this_->data);
    free(this_);
}



int main()
{
    HashTable* map = hashtable_create(2);

    int * n = malloc(sizeof(int));
    *n = 10;

    hashtable_insert(map,n,"key");
    hashtable_insert(map,n,"fwafwa");
    hashtable_insert(map,n,"gwagwag");

    printf("%p\n",hastable_search(map,"gwagwag"));

    //printf("hash %d\n",hash_search(map,"monday"));

    hashtable_destroy(map);

    printf("destroyed\n");
    return 0;
}