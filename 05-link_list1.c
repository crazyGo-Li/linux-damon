#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct node
{
    int i_data;
    struct node *next;
}LNode, *LinkList;

LinkList Create_LinkList();
LinkList Create_LinkList2();
LinkList Create_LinkList3();

int Length_LinkList1(LinkList);
int Length_LinkList2(LinkList);

LNode *Get_LinkList(LinkList , int );
LNode *Locate_LinkList(LinkList , int );

int Insert_LinkList(LinkList , int , int );
int Delete_LinkList(LinkList, int);

void Destroy_LinkList(LinkList);
void Scan_Link_list(LinkList );

int main()
{
    LinkList L = Create_LinkList();
    Insert_LinkList(L, 1, 1);
    Insert_LinkList(L, 2, 2);
    Insert_LinkList(L, 3, 3);
    Scan_Link_list(L);
    Destroy_LinkList(L);

    return 0;
}

void Destroy_LinkList(LinkList L)
{
    LinkList p = NULL;
    while(L != NULL)
    {
        p = L;
        L = L->next;
        free(p);
    }
}

void Scan_Link_list(LinkList L)
{
    LinkList p =L;
    int i=1;

    while(p != NULL)
    {
        printf("is: %d data: %d\n", i, p->i_data);
        p = p->next;
    }
}

int Delete_LinkList(LinkList L, int i)
{
    LinkList p, s;
    p = Get_LinkList(L, i-1);
    if(p == NULL || p->next == NULL)
    {
        printf("args i error\n");
        return -1;
    }
    else
    {
        s = p->next;
        p->next = s->next;
        free(s);
        return 0;
    }
}

int Insert_LinkList(LinkList L, int i, int x)
{
    LNode *p, *s;
    p = Get_LinkList(L, i-1);
    if(p == NULL)
    {
        printf("args i error\n");
        return -1;
    }
    else
    {
        s = (LinkList)malloc(sizeof(LNode));
        s->i_data = x;
        s->next = p->next;
        p->next = s;
        return 0;
    }
}

LNode *Locate_LinkList(LinkList L, int x)
{
    LNode *p = L;
    while(p != NULL && p->i_data != x)
    {
        p = p->next;
    }

    return p;
}

LNode *Get_LinkList(LinkList L, int i)
{
    LNode *p = L;
    int j = 0;
    while(p->next != NULL && j < i)
    {
        p = p->next;
        j++;
    }
    if(j == i)
    {
        return p;
    }
    else
    {
        return NULL;
    }
}

int Length_LinkList2(LinkList L)
{
    LNode *p = L;
    int j = 0;
    if( p == NULL )
    {
        return j;
    }
    j = 1;
    while(p->next)
    {
        p = p->next;
        j++;
    }
    return j;

}

int Length_LinkList1(LinkList L)
{
    LNode *p = L;
    int j = 0;
    while(p->next)
    {
        p = p->next;
        j++;
    }        
    return j;
}

LinkList Create_LinkList3()
{
    LinkList L;
    LNode *R;
    int x;
    L = (LinkList)malloc(sizeof(LNode));
    L->next = NULL;
    R = L;
    scanf("%d", &x);
    while(x != 99)
    {
        R->next = (LNode *)malloc(sizeof(LNode));
        R->next->i_data = x;
        R=R->next;
        scanf("%d", &x);
    }
    R->next = NULL;
    return L;
}

LinkList Create_LinkList2()
{
    LinkList L = NULL;
    LNode *s, *R=NULL;
    int x;
    scanf("%d", &x);
    while(x != 99)
    {
        s = (LNode *)malloc(sizeof(LNode));
        s->i_data = x;
        if(L == NULL)
        {
            L = s;
        }
        else
        {
            R->next = s;
        }
        R = s;
        scanf("%d", &x);
    }
    if (R != NULL)
    {
        R->next = NULL;
    }
    return L;
}

LinkList Create_LinkList()
{
    LinkList L=NULL;
    LNode *s;
    int x;
    scanf("%d", &x);
    while(x!=99)
    {
        s = (LNode *)malloc(sizeof(LNode));
        s->i_data = x;
        s->next = L;
        L = s;
        scanf("%d", &x);
    }
    return L;
}

