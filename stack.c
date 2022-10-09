#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct stack{
    char* data;
    int size;
    int maxsize;
}stack;

stack* _stack_create(int numElements,int sizeType)
{
    stack* this_ = (stack*)malloc(sizeof(stack)); 
    this_->maxsize = numElements <= 0 ? 1 : numElements;
    this_->size = 0;
    this_->data = (char*)calloc(this_->maxsize,sizeType);
    return this_;
}

void* _stack_push(stack* this_,int sizeType)
{
    if(this_->size == this_->maxsize){
        this_->maxsize*=2;
        this_->data = (char*)realloc(this_->data,this_->maxsize*sizeType);
    }

    return (char*)(&this_->data[0])+ (sizeType*this_->size++);
}

void* _stack_pop(stack* this_,int sizeType)
{
    return (char*)(&this_->data[0])+ (sizeType*--this_->size);
}

void stack_free(stack* this_)
{
    free(this_->data);
    free(this_);
}

bool stack_empty(stack* this_){return this_->size == 0;}

#define stack_create(numberOfElements,Type) _stack_create(numberOfElements,sizeof(Type))
#define stack_push(s,data) *(typeof(data)*)_stack_push(s,sizeof(typeof(data))) = data
#define stack_pop(s,TYPE) ((TYPE*)_stack_pop(s,sizeof(TYPE)))
#define sg stack_get
#define stack_get(s,type,index) ((type*)s->data)[index]
#define stack_peek(s,type) ((type*)s->data)[(s->size)-1]

typedef struct Person
{
    char username[32];
    int idade;
}Person;

int main()
{
    //Stack -> First in last out
    stack* persons = stack_create(1,Person);

    stack_push(persons,((Person){"Rodrigo",14}));
    stack_push(persons,((Person){"Joao",17}));
    stack_push(persons,((Person){"Manuel",21}));

    printf("popped: %s\n",stack_pop(persons,Person)->username);
    printf("peek: %s\n\n",stack_peek(persons,Person));
    
    for(int i = 0;i < persons->size;i++){
        printf("p.name=%s,p.age= %d\n",sg(persons,Person,i).username,sg(persons,Person,i).idade);
    }

    stack_free(persons);
    return 0;
}