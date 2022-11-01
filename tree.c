#include <stdio.h>
#include <stdlib.h>
int numfree = 0;
int nummalloc = 0;
#define free(ptr) numfree++; free(ptr);
#define malloc(size)  malloc(size);nummalloc++;
#define calloc(count,size) calloc(count,size);nummalloc++;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tree.h
#pragma once
#include <stdbool.h>

typedef struct TreeNode{
    struct TreeNode ** childs;
    int childs_count;
    int childs_maxsize;
    char* data;
}TreeNode;

typedef void (*treenode_func)(TreeNode*);
typedef bool (*treenode_cmp)(TreeNode*,void*);

TreeNode* _treenode_createnode(void* value,int datasize);
TreeNode* _treenode_insertdata(TreeNode* root,void* value,int datasize);
void treenode_insertnode(TreeNode* root,TreeNode* node);
void treenode_print(TreeNode * root,treenode_func printfunc);
void treenode_remove(TreeNode** parent,int childindex);
TreeNode* treenode_search(TreeNode* root,void* value,treenode_cmp func);
void treenode_free(TreeNode** root);

#define ca(value) (typeof(value)){value} //cast as variable
#define treenode_createnode(value) _treenode_createnode(&value,sizeof(typeof(value)))
#define treenode_insertdata(root,value) _treenode_insertdata(root,&value,sizeof(typeof(value)))
#define tree_data(node,type) (*(type*)(node->data))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//tree.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

TreeNode* _treenode_createnode(void* value,int datasize)
{
    TreeNode* ret = (TreeNode*)malloc(sizeof(TreeNode));
    
    if(ret != NULL){
        //ret->value = value;
        ret->childs = (TreeNode**)malloc(sizeof(TreeNode) * 1); 
        ret->data = (char*)malloc(datasize);
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

TreeNode* _treenode_insertdata(TreeNode* root,void* value,int datasize)
{
    if(root->childs_count == root->childs_maxsize){
        root->childs_maxsize *=2;
        root->childs = (TreeNode**)realloc(root->childs,sizeof(TreeNode*) * root->childs_maxsize);
    }

    TreeNode* child =  _treenode_createnode(value,datasize);
    root->childs[root->childs_count] = child;
    root->childs_count++; 
    return child;
}

void treenode_insertnode(TreeNode* root,TreeNode* node){
    if(root->childs_count == root->childs_maxsize){
        root->childs_maxsize *=2;
        root->childs = (TreeNode**)realloc(root->childs,sizeof(TreeNode*) *root->childs_maxsize);
    }

    root->childs[root->childs_count] = node;
    root->childs_count++; 
}


typedef void (*treenode_func)(TreeNode*);

void treenode_freechild(TreeNode** child)
{
    TreeNode* tree = *child;
    for (int i = 0; i < tree->childs_count; i++)
    {
        treenode_freechild(&tree->childs[i]);
    }
    free(tree->childs);
    free(tree->data);
    free(tree);
    //printf("free %p %p %p\n",tree,tree->data,tree->childs);
}

void treenode_print_(TreeNode* child,int level,treenode_func printfunc)
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

void treenode_print(TreeNode * root,treenode_func printfunc)
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

void treenode_remove(TreeNode** parent,int childindex)
{
    TreeNode* temp = *parent;
    //if is outside of array throw error
    assert(childindex < temp->childs_count);
    TreeNode* child = temp->childs[childindex];
    for (int i = childindex; i < temp->childs_count-1; i++){
        temp->childs[i] = temp->childs[i+1];
    }
    treenode_freechild(&child);
    temp->childs_count--;
}

TreeNode* treenode_search(TreeNode* root,void* value,treenode_cmp func)
{
    if(root == NULL){return NULL;}
    if(func(root,value)){return root;}
    for (int i = 0; i < root->childs_count; i++){
        
        if(func(root->childs[i],value)){return root->childs[i];}
        TreeNode* temp = treenode_search(root->childs[i],value,func);
        if(temp != NULL){return temp;}
    }
    return NULL;
}

void treenode_free(TreeNode** root)
{
    TreeNode* tree = *root;
    while (tree->childs_count != 0){
        treenode_remove(root,0);
    }
    
    free(tree->childs);
    free(tree->data);
    free(tree);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void printNumber(TreeNode* a){printf("%d",*(int*)a->data);}

typedef struct Employer{
    char name[64];
    char role[32];
}Employer;

void _printEmployer(TreeNode* a)
{
    Employer* e = (Employer*)(a->data);
    printf("%s (%s) %p (e)%p",e->name,e->role,a,e);
}

bool employercmp(TreeNode* root,void* value){
    Employer* e = (Employer*)root->data;
    return strcmp(e->name,(char*)value) == 0;
}


int main()
{
    TreeNode* root = treenode_createnode(ca(10));


    treenode_insertdata(root,ca(5));
    treenode_insertdata(root,ca(10));
    treenode_insertdata(root,ca(30));

    treenode_print(root,printNumber);

    TreeNode* first = treenode_createnode(ca(50));
    treenode_insertnode(first,root);
    root = first;
    puts("--------insert node--------");
    treenode_print(root,printNumber);
    treenode_free(&root);
    puts("---------------------------");
    TreeNode* founder = treenode_createnode(((Employer){.name = "Maria", .role = "Founder"}));
    TreeNode*  director = treenode_insertdata(founder,((Employer){.name = "Joao", .role = "Sales Director"}));
    TreeNode*  director2 = treenode_insertdata(founder,((Employer){.name = "Manuel", .role = "Advertising Director"}));
    TreeNode*  director3 = treenode_insertdata(founder,((Employer){.name = "Rui", .role = "Communication Director"}));

    treenode_insertdata(director,((Employer){.name = "Antonio", .role = "Employer"}));
    treenode_insertdata(director2,((Employer){.name = "Rui", .role = "Employer"}));
    treenode_insertdata(director2,((Employer){.name = "Miguel", .role = "Employer"}));


    treenode_insertdata(director3,((Employer){.name = "Nuno", .role = "Employer"}));
    treenode_insertdata(director3,((Employer){.name = "Francisco", .role = "Employer"}));
    treenode_insertdata(director3,((Employer){.name = "Vasco", .role = "Employer"}));

    treenode_print(founder,_printEmployer);
    
    puts("---------------------------");
    #define PERSON_NAME "Manuel"
    if(treenode_search(founder,PERSON_NAME,employercmp) != NULL){
        printf("%s found\n",PERSON_NAME);
    }else{
        printf("%s not found\n",PERSON_NAME);
    }

    treenode_free(&founder);
    printf("malloc:%d | free:%d\n",nummalloc,numfree);
    return EXIT_SUCCESS;
}