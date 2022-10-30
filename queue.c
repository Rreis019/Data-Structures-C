#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue
{
    char* data;
    int size;
    int maxsize;
}Queue;

Queue* _queue_create(int numElements,int sizeType)
{
    Queue *this_ = malloc(sizeof(Queue));
    this_->data = calloc(numElements,sizeType);
    this_->maxsize = numElements;
    this_->size = 0;
    return this_;
}

void _queue_enqueue(Queue* this_,int elementSize)
{
   for (int i = 0; i < this_->size; i++)
   {
        memcpy(
            (char*)(&this_->data[0]) + i*elementSize,
            (char*)(&this_->data[0]) + (i+1)*elementSize,
            elementSize
        );
   }
   this_->size--;
}
void* _queue(Queue* this_,int sizeType){  return (char*)(&this_->data[0])+ (sizeType*this_->size++); }
void queue_peek(){}
bool queue_isfull(Queue* this_){ return this_->size == this_->maxsize;}
bool queue_isempty(Queue* this_){ return this_->size == 0;}

void queue_free(Queue* this_)
{
    free(this_->data);
    free(this_);
}

#define queue_create(numElements,type) _queue_create(numElements,sizeof(type))
#define queue_queue(q,data) *(typeof(data)*)_queue(q,sizeof(typeof(data))) = data
#define queue_get(q,type,index) ((type*)q->data)[index]
#define queue_enqueue(q,type) _queue_enqueue(q,sizeof(type))


int main()
{
    //Queue -> First in first out
    Queue* nums = queue_create(10,int);
    queue_queue(nums,20);
    queue_queue(nums,32);
    queue_queue(nums,63);
    queue_queue(nums,12);
    queue_queue(nums,52);

    queue_enqueue(nums,int);

    for(int i = 0;i < nums->size;i++){
        printf("%d ",queue_get(nums,int,i));
    }

    queue_free(nums);
    return 0;
}
