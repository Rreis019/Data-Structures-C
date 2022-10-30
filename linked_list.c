#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



int numfree = 0;
int nummalloc = 0;
#define free(ptr) free(ptr);numfree++;
#define malloc(size)  malloc(size);nummalloc++;
#define calloc(count,size) calloc(count,size);nummalloc++;
   

//LinkedList.h
typedef struct LinkedNode
{
    char* data;
    struct LinkedNode* next;
}LinkedNode;

typedef void (*linkedlist_func)(LinkedNode*);
typedef bool (*linkedlist_sortcmp)(void*,void*);

LinkedNode* _linkedlist_createNode(void* value,int sizeValue);
void _linkedlist_insert(LinkedNode** head,void* value,int sizeValue);
void linkedlist_remove(LinkedNode** head,int index);
void linked_list_loop(LinkedNode* head,linkedlist_func func);
void linked_list_free(LinkedNode** head);
void linkedlist_sort(LinkedNode** top,linkedlist_sortcmp sortcmp);

#define ca(value) (typeof(value)){value} //cast as variable
#define linkedlist_createnode(value) _linkedlist_createNode(&value,sizeof(typeof(value)))
#define linkedlist_insert(l,value) _linkedlist_insert(&l,&value,sizeof(typeof(value)))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LinkedList.c

LinkedNode* _linkedlist_createNode(void* value,int sizeValue)
{ 
    LinkedNode *n = malloc(sizeof(LinkedNode));
    n->data = malloc(sizeValue);
    n->next = NULL;
    memcpy(n->data,value,sizeValue);
    return n;
}

void _linkedlist_insert(LinkedNode** head,void* value,int sizeValue) 
{
    LinkedNode* n = _linkedlist_createNode(value,sizeValue); 
    n->next = *head; 
    *head = n; 
} 

void linkedlist_remove(LinkedNode** head,int index)
{
    LinkedNode* temp = *head;
    if(index == 0){
        *head = temp->next;
        free(temp->data);
        free(temp);
        return;
    }
    LinkedNode* prev = NULL;
    for(int i = 0; i < index;i++)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    free(temp->data);
    free(temp);
}


void linkedlist_split(LinkedNode *top, LinkedNode **front, LinkedNode **back) {
	LinkedNode* fast = top->next;
	LinkedNode* slow = top;

    /* fast pointer advances two nodes, slow pointer advances one node */
	while (fast != NULL) {
		fast = fast -> next;		/* "fast" moves on first time */
		if (fast != NULL) { 
			slow = slow->next;	/* "slow" moves on first time */
			fast = fast->next;	/* "fast" moves on second time */
        }
    }
	
    /* "slow" is before the middle in the list, so split it in two at that point */
	*front = top;
	*back = slow -> next;
	slow -> next = NULL;			/* end of the input list */
}

LinkedNode* linkedlist_merge(LinkedNode *top1, LinkedNode *top2,linkedlist_sortcmp sortcmp_)
{
    if (top1 == NULL) 
		return top2; 
    else{
		if (top2 == NULL) 
			return top1;   
    }

    LinkedNode* pnt = NULL;
    /* pick either top1 or top2, and merge them */ // *(float*)top1->data <= *(float*)top2->data
    if (sortcmp_(top1->data,top2->data)) { 
		pnt = top1;
		pnt->next = linkedlist_merge(top1->next, top2,sortcmp_); 
    } 
    else {
        pnt = top2; 
        pnt->next = linkedlist_merge(top1, top2->next,sortcmp_); 
    }
    return pnt; 
} 

void linkedlist_sort(LinkedNode** top,linkedlist_sortcmp sortcmp_)
{
    LinkedNode *tmp = *top, *a, *b;

    if (tmp != NULL && tmp -> next != NULL) {
		linkedlist_split(tmp, &a, &b);				/* (divide) split head into "a" and "b" sublists */
	  
		/* (conquer) sort the sublists */
		linkedlist_sort(&a,sortcmp_); 
		linkedlist_sort(&b,sortcmp_); 
	  
		*top = linkedlist_merge(a, b,sortcmp_);				/* (combine) merge the two sorted lists together */
	}

}

void linkedlist_loop(LinkedNode* head,linkedlist_func func)
{
    LinkedNode* n = head; 
    do{
        func(n);
        n = n->next;
    } while (n != NULL);
}


void linkedlist_free(LinkedNode* head)
{
    LinkedNode* tmp;
    while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp->data);
       free(tmp);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LinkedList vs List
//LinkedList is faster than list to insert/delete  since we dont copy values.
//LinkedList use only memory necessary
//But linked list(o(n)) is slow to access data by index 

typedef struct Person
{
    char name[32];
    int age;
}Person;


void linkedListFloat(LinkedNode* m){printf("%f - ",*(float*)(m->data));}
void linkedListPerson(LinkedNode* m)
{
    printf("%s:%d - ",(*(Person*)m->data).name,(*(Person*)m->data).age);
}

bool linkedfloatSortcmp(void* a,void* b){
    return *(float*)a <= *(float*)b;
}

bool linkedPersonSortcmp(void* a,void* b){
    return (*(Person*)a).age <= (*(Person*)b).age;
}



int main()
{
    LinkedNode* listNum = linkedlist_createnode(ca(10.5f));
    linkedlist_insert(listNum,ca(420.0f));
    linkedlist_insert(listNum,ca(12.0f));
    linkedlist_insert(listNum,ca(63.0f));
    linkedlist_remove(&listNum,0);

    printf("Unordered:\n");
    linkedlist_loop(listNum,linkedListFloat);printf("NULL");
    linkedlist_sort(&listNum,linkedfloatSortcmp);
    printf("\nSorted:\n");
    linkedlist_loop(listNum,linkedListFloat);printf("NULL\n\n");

    linkedlist_free(listNum);
    

    LinkedNode* persons = linkedlist_createnode( ((Person){"Joao",15}));
    linkedlist_insert(persons,((Person){"Joao",19}));
    linkedlist_insert(persons,((Person){"Manuel",19}));

    printf("Unordered:\n");
    linkedlist_loop(persons,linkedListPerson);printf("NULL");
    linkedlist_sort(&persons,linkedPersonSortcmp);
    linkedlist_remove(&persons,0);
    printf("\nSorted by age:\n");
    linkedlist_loop(persons,linkedListPerson);printf("NULL\n\n");


    linkedlist_free(persons);
    printf("malloc:%d | free:%d\n",nummalloc,numfree);
    return 0;
}

