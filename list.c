//List.h
#include <stdio.h>
#include <stdbool.h>
typedef struct List
{
    char* data;
    int size;
    int maxsize;
}List;

typedef bool (*list_cmp)(void*,void*);
List* _list_create(int numElements,int sizeType);
void* _list_insert(List* this_,int sizeType,int index);
void* _list_pushback(List* this_,int sizeType);
void _list_remove(List* this_,int sizeType,int index);
void list_free(List* this_);
int _list_search(List* this_,int sizeType,void* value,list_cmp func);

#define list_create(numElements,type) _list_create(numElements,sizeof(type))
#define list_pushback(l,data) *(typeof(data)*)_list_pushback(l,sizeof(typeof(data))) = data
#define list_get(l,type,index) ((type*)l->data)[index]
#define list_remove(l,type,index) _list_remove(l,sizeof(type),index)
#define list_insert(l,data,index) *(typeof(data)*)_list_insert(l,sizeof(typeof(data)),index) = data
#define list_search(l,data,func) _list_search(l,sizeof(typeof(data)),&data,func)
#define list_sort(l,type,func)  qsort(&l->data[0],l->size,sizeof(type),func)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//List.c
#include <stdlib.h>
#include <string.h>
List* _list_create(int numElements,int sizeType){
    List *this_ = (List*)malloc(sizeof(List));
    this_->data = (char*)calloc(numElements,sizeType);
    this_->maxsize = numElements;
    this_->size = 0;
    return this_;
}

void* _list_insert(List* this_,int sizeType,int index)
{
    if(this_->size == this_->maxsize){
        this_->maxsize*=2;
        this_->data = (char*)realloc(this_->data,this_->maxsize*sizeType);
    }

    for(int i = this_->size;i > index;i--)
    {
          memcpy(
            (char*)(&this_->data[0]) + i*sizeType,
            (char*)(&this_->data[0]) + (i-1)*sizeType,
            sizeType
        );
    }
    this_->size++;
    return (void*)(&this_->data[0])+ (sizeType*index);
}

void* _list_pushback(List* this_,int sizeType)
{
    if(this_->size == this_->maxsize){
        this_->maxsize*=2;
        this_->data = (char*)realloc(this_->data,this_->maxsize*sizeType);
    }

    return (char*)(&this_->data[0])+ (sizeType*this_->size++);
}

void _list_remove(List* this_,int sizeType,int index)
{
    for(int i = index;i < this_->size;i++)
    {
          memcpy(
            (char*)(&this_->data[0]) + i*sizeType,
            (char*)(&this_->data[0]) + (i+1)*sizeType,
            sizeType
        );
    }
    this_->size--;
}

void list_free(List* this_)
{
    free(this_->data);
    free(this_);
}


int _list_search(List* this_,int sizeType,void* value,list_cmp func)
{

    for(int i = 0;i < this_->size;i++)
    {
        void* ptr = (&this_->data[0]) + i*sizeType;
        if(func(ptr,value)){return i;}
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//main.c
typedef struct Person
{
    char username[32];
    int idade;
}Person;

bool person_cmp(void* a,void* b){
    return strcmp(((Person*)a)->username,((Person*)b)->username) == 0 && ((Person*)a)->idade == ((Person*)b)->idade;
}

int person_sort(const void * a, const void * b) {
   return ((Person*)a)->idade - ((Person*)b)->idade;
}

int main()
{
    List* list = list_create(1,Person);
    list_pushback(list, ((Person){"Rodrigo",19}));
    list_pushback(list, ((Person){"Joao",15}));
    list_pushback(list, ((Person){"Tomas",21}));

    list_insert(list,((Person){"Batata",21}),1);

    for(int i = 0;i < list->size;i++){
        printf("Username[%d] : %s(%d)\n",i,list_get(list,Person,i).username,list_get(list,Person,i).idade);
    }
    
    printf("\nSorted by age: \n");
    list_sort(list,Person,person_sort);
    for(int i = 0;i < list->size;i++){
        printf("Username[%d] : %s(%d)\n",i,list_get(list,Person,i).username,list_get(list,Person,i).idade);
    }

    int index = list_search(list,((Person){"Joao",15}),person_cmp);
    printf("%s found at %d\n","Joao",index);


    ////////////////////////////////////////////////////////////////////////////////////////////////

    List* nums = list_create(5,int);
    for(int i = 0; i < 10;i++){list_pushback(nums,(i*4252)%32);}
    printf("\nNums:\n");
    for(int i = 0;i < nums->size;i++){
        printf("nums[%d] = %d\n",i,list_get(nums,int,i));
    }
}