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


int main()
{
    return 0;
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

