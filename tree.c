#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tree.h

typedef struct treenode{
    char* data;
    struct treenode ** childs;
    int childs_count;
    int childs_maxsize;
}treenode;

typedef void (*treenode_func)(treenode*);
typedef bool (*treenode_cmp)(treenode*,void*);

treenode* _treenode_createnode(void* value,int datasize);
treenode* _treenode_insertnode(treenode* root,void* value,int datasize);
void treenode_print(treenode * root,treenode_func printfunc);
void treenode_remove(treenode* parent,int childindex);
treenode* treenode_search(treenode* root,void* value,treenode_cmp func);

#define ca(value) (typeof(value)){value} //cast as variable
#define treenode_createnode(value) _treenode_createnode(&value,sizeof(typeof(value)))
#define treenode_insertnode(root,value) _treenode_insertnode(root,&value,sizeof(typeof(value)))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tree.c
treenode* _treenode_createnode(void* value,int datasize)
{
    treenode* ret = malloc(sizeof(treenode));
    
    if(ret != NULL){
        //ret->value = value;
        ret->childs = malloc(sizeof(treenode) * 1); 
        ret->data = malloc(datasize);
        memcpy(ret->data,value,datasize);
        ret->childs_count = 0;
        ret->childs_maxsize = 1;
    }
    
    return ret;
}


void printspace(int num){
    for (int i = 0; i < num; i++){
        putchar(' ');
    }
}

treenode* _treenode_insertnode(treenode* root,void* value,int datasize)
{
    if(root->childs_count == root->childs_maxsize){
        root->childs_maxsize *=2;
        root->childs = realloc(root->childs,root->childs_maxsize);
    }

    treenode* child =  _treenode_createnode(value,datasize);
    root->childs[root->childs_count] = child;
    root->childs_count++; 
    return child;
}

typedef void (*treenode_func)(treenode*);

void treenode_freechild(treenode* child)
{
    for (int i = 0; i < child->childs_count; i++)
    {
        treenode_freechild(child->childs[i]);
        free(child->childs[i]);
    }
    free(child->childs);
}


void treenode_print_(treenode* child,int level,treenode_func printfunc)
{
    for (int i = 0; i < child->childs_count; i++)
    {
        printspace(level*3);
        fputs("|- ",stdout);
        printfunc(child->childs[i]);
        putchar('\n');
        treenode_print_(child->childs[i],level+1,printfunc);
    }
    
}



void treenode_print(treenode * root,treenode_func printfunc)
{
    if (root == NULL){puts("<empty>");return;}
    
    int level = 1;
    fputs("|- ",stdout);
    printfunc(root);putchar('\n');
    for (int i = 0; i < root->childs_count; i++)
    {
        printspace(level*3);
        fputs("|- ",stdout);
        printfunc(root->childs[i]);
        putchar('\n');
        treenode_print_(root->childs[i],level+1,printfunc);
    }
    printf("\n");
}

void treenode_remove(treenode* parent,int childindex)
{
    //if is outside of array throw error
    assert(childindex < parent->childs_count);
    treenode_freechild(parent->childs[childindex]);
    for (int i = childindex; i < parent->childs_count-1; i++){
        parent->childs[i] = parent->childs[i+1];
    }
    parent->childs_count--;
}

treenode* treenode_search(treenode* root,void* value,treenode_cmp func)
{
    if(root == NULL){return NULL;}
    if(func(root,value)){return root;}
    for (int i = 0; i < root->childs_count; i++){
        
        if(func(root->childs[i],value)){return root->childs[i];}
        treenode* temp = treenode_search(root->childs[i],value,func);
        if(temp != NULL){return temp;}
    }
    return NULL;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printNumber(treenode* a){printf("%d",*(int*)a->data);}

typedef struct Employer{
    char name[64];
    char role[32];
}Employer;

void printEmployer(treenode* a)
{
    Employer* e = (Employer*)(a->data);
    printf("%s (%s)",e->name,e->role);
}

bool employercmp(treenode* root,void* value){
    Employer* e = (Employer*)root->data;
    return strcmp(e->name,(char*)value) == 0;
}

//quicksort




int main()
{
    treenode* root = treenode_createnode(ca(10));
    treenode* child1 = treenode_insertnode(root,ca(52)); //will insert child node in root
    treenode* child2 = treenode_insertnode(root,ca(20));
    treenode* child3 = treenode_insertnode(root,ca(30));

    treenode_insertnode(child1,ca(5));
    treenode_insertnode(child1,ca(10));
    treenode_insertnode(child2,ca(30));

    treenode_remove(root,2);
    treenode_print(root,printNumber);
    
    puts("---------------------------");

    treenode* founder = treenode_createnode(((Employer){.name = "Maria", .role = "Founder"}));
    treenode*  director = treenode_insertnode(founder,((Employer){.name = "Joao", .role = "Sales Director"}));
    treenode*  director2 = treenode_insertnode(founder,((Employer){.name = "Manuel", .role = "Advertising Director"}));
    treenode*  director3 = treenode_insertnode(founder,((Employer){.name = "Rui", .role = "Communication Director"}));

    treenode_insertnode(director,((Employer){.name = "Antonio", .role = "Employer"}));
    treenode_insertnode(director2,((Employer){.name = "Rui", .role = "Employer"}));
    treenode_insertnode(director2,((Employer){.name = "Miguel", .role = "Employer"}));


    treenode_insertnode(director3,((Employer){.name = "Nuno", .role = "Employer"}));
    treenode_insertnode(director3,((Employer){.name = "Francisco", .role = "Employer"}));
    treenode_insertnode(director3,((Employer){.name = "Vasco", .role = "Employer"}));

    treenode_print(founder,printEmployer);
    
    puts("---------------------------");
    #define PERSON_NAME "Manuel"
    if(treenode_search(founder,PERSON_NAME,employercmp) != NULL){
        printf("%s found",PERSON_NAME);
    }else{
        printf("%s not found",PERSON_NAME);
    }

    return EXIT_SUCCESS;
}