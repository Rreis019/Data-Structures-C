#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*
typedef struct treenode{
    int value;
    struct treenode ** childs;
    int childs_count;
    int childs_maxsize;
}treenode;

//alloc free after
treenode* treenode_createnode(int value)
{
    treenode* ret = malloc(sizeof(treenode));
    if(ret != NULL){
        ret->value = value;
        ret->childs = malloc(sizeof(treenode) * 1); 
        ret->childs_count = 0;
        ret->childs_maxsize = 1;
    }
    return ret;
}

treenode* treenode_insertnode(treenode* root,int value)
{
    if(root->childs_count == root->childs_maxsize){
        root->childs_maxsize *=2;
        root->childs = realloc(root->childs,root->childs_maxsize);
    }

    treenode* child =  treenode_createnode(value);
    root->childs[root->childs_count] = child;
    root->childs_count++; 
    return child;
}


void printspace(int num){
    for (int i = 0; i < num; i++){
        putchar(' ');
    }
}

void treenode_print_(treenode* child,int level)
{
    for (int i = 0; i < child->childs_count; i++)
    {
        printspace(level*3);
        printf("|- %d\n",child->childs[i]->value);
        treenode_print_(child->childs[i],level+1);
    }
    
}
void treenode_print(treenode * root)
{
    int level = 0;
    printf("|- %d\n",root->value);level++;
    for (int i = 0; i < root->childs_count; i++)
    {
        printspace(level*3);
        printf("|- %d\n",root->childs[i]->value);
        treenode_print_(root->childs[i],level+1);
    }
    printf("\n");
}

void treenode_freechild(treenode* child)
{
    for (int i = 0; i < child->childs_count; i++)
    {
        treenode_freechild(child->childs[i]);
        free(child->childs[i]);
    }
    free(child->childs);
}

void treenode_remove(treenode* parent,int childindex)
{
    treenode_freechild(parent->childs[childindex]);
    for (int i = childindex; i < parent->childs_count-1; i++){
        parent->childs[i] = parent->childs[i+1];
    }
    parent->childs_count--;
}


//if found value will return adress of the node else will return NULL
treenode* treenode_search(treenode* root,int value)
{
    if(root == NULL){return NULL;}
    if(root->value == value){return root;}

    for (int i = 0; i < root->childs_count; i++){
        
        if(root->childs[i]->value == value){return root->childs[i];}
        treenode* temp = treenode_search(root->childs[i],value);
        if(temp != NULL){return temp;}
    }

    return NULL;
}

*/


typedef struct treenode{
    char* data;
    struct treenode ** childs;
    int childs_count;
    int childs_maxsize;
}treenode;

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


#define ca(value) (typeof(value)){value} //cast as variable
#define treenode_createnode(value) _treenode_createnode(&value,sizeof(typeof(value)))
#define treenode_insertnode(root,value) _treenode_insertnode(root,&value,sizeof(typeof(value)))

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
    
    printf("---------------------------\n");

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
    
    return EXIT_SUCCESS;
}