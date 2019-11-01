//binary search tree using array
#include<stdio.h>
#include<stdlib.h>

void insert(int *, int);
void preorder(int*,int);
void postorder(int*,int);
void inorder(int*,int);
int minimum(int*);
int maximum(int*);
void main()
{
  int t[100];//place for tree elements 
  int ch,num,k,i;
  for(i=0;i<100;i++)
    t[i]=-1;
 while(1)
 {
      printf("\n1.Insert");
      printf("\n2. Minimum");
      printf("\n3.largest");
      printf("\n4.Preorder");
      printf("\n5. Inorder");
      printf("\n6.Postorder");
      printf("\n4.Exit");
      scanf("%d",&ch);
       switch(ch)
       {
         case 1 :printf("Enter the element");
                 scanf("%d",&num);
                 insert(t,num);
                 break;
         case 2 : k=minimum(t);
                printf("The smallest elemenet = %d",k);
                break;
       case 3 : k=maximum(t);
                printf("The largest elemenet = %d",k);
                break;
        case 4:printf("The preorder traversal\n");
               preorder(t,1)
               break;
        case 5:printf("The Inorder traversal\n");
               inorder(t,1)
               break;
        case 6:printf("The postorder traversal\n");
               postrder(t,1)
               break;
        
         case 4:exit(0);
       }
    }
  }    
  

 void insert(int x)
 {
   int i;
   i=1;

   while(t[i]!=-1)
   {
     if(x<t[i])
      i=2*i;
     else
      i=2*i+1;
   }
   t[i]=x;
 }

  int minimum(int *t)  
  {
    int i=1;
    while(t[2*i]!=-1)
      i=2*i;
    return(t[i]);
  }




  




    




   







  




 void preorder(int *t, int i)
  {
    if(t[i]!=-1)
    {
       printf("%d ",t[i]);
       preorder(t,2*i);
       preorder(t,2*i+1);
     }
  }

  void inorder(int *t, int i)
  {
    if(t[i]!=-1)
    {
       inorder(t,2*i);
       printf("%d ",t[i]);
       inorder(t,2*i+1);
     }
  }

 void postorder(int *t, int i)
  {
    if(t[i]!=-1)
    {
       postorder(t,2*i);
       postorder(t,2*i+1);
       printf("%d ",t[i]);
     }
  }














/*  int minimum(int *t)
   {
    int i=0;
    while(t[2*i+1]!=-1)
      i=2*i+1;
    return(t[i]);
   }
      
    int maximum(int *t)
   {
    int i=0;
    while(t[2*i+2]!=-1)
      i=2*i+2;
    return(t[i]);
   }



    void insert(int *t, int x)
    {
     int i;
     i=0;

     while(t[i]!=-1)
     {
        if(x<t[i])
         i=2*i+1;//move left
        else
         i=2*i+2;//move right
     }
    t[i]=x;
  }
  
  
  
























/*void insert(int *t, int key)
    {
      int i=0;

      while(t[i]!=0)
      {
        if(key<t[i])
          i=2*i+1;//move left
       else
          i=2*i+2;//move right
      }
    t[i]=key;
   }
           
   

  int min(int *t)
  {
    int i=0;
    while(t[i*2+1]!=0)
      i=2*i+1;
    return t[i];
  }

  int max(int *t)
  {
    int i=0;
    while(t[i*2+2]!=0)
      i=2*i+2;
    return t[i];
  } */












