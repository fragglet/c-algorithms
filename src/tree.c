//PROGRAM TO IMPLEMENT A BINARY TREE USING LINKED REPRESENTATION
#include<stdio.h>
#include<stdlib.h>
struct tnode
{
  int data;
  struct tnode *left;
  struct tnode *right;
};
struct tnode* create();
void preorder(struct tnode *);
void postorder(struct tnode*);
void inorder(struct tnode*);
int height(struct tnode *);
int leafcount(struct tnode*);
void main()
{
  struct tnode *root;
  int ch,num,k,n;
  root=NULL;
 
  root=create();
  printf("\nTraversal in preorder\n");
  preorder(root);
  printf("\nInorder Traversal..\n");
  inorder(root);
  printf("\nPost order traversal..\n");
  postorder(root);

  printf("\nThe height of the tree..\n");
  printf("\nHeight= %d ",height(root));

  printf("\nThe no of leaf nodes..");
  printf("\nLeaf nodes = %d",leafcount(root)); 
}

  int height(struct tnode* t)
  {
    int l,r;

    if(t==NULL)//height of empty tree is -1
     return -1;
   
    if((t->left==NULL)&&(t->right==NULL))//height of one node tree is 0
     return 0;
    
    l=height(t->left);
    r=height(t->right);

    if(l>r)
      return(l+1);
    return(r+1);
  }

 int leafcount(struct tnode* t)
 {
   int l,r;

   if(t==NULL)
    return 0;
   if((t->left==NULL)&&(t->right==NULL))
    return 1;
   l=leafcount(t->left);//leaf nodes in left subtree
   r=leafcount(t->right);//leaf nodes in right subtree
   return(l+r);
  }









  


  struct tnode *create()
  {

   int x;
    struct tnode *p;

   printf("Enter the element...");
   scanf("%d",&x);

   if(x==-1)
     return NULL;

   p=malloc(sizeof(struct tnode));
   p->data=x;

   printf("\ncreating the left subtree of %d\n",x);
   p->left=create();


   printf("\nCreating the right subtree of %d\n",x);
   p->right=create();

   return p;
 }


  void preorder(struct tnode* t)
  {
    if(t!=NULL)
    {
      printf("%d ",t->data);
      preorder(t->left);
      preorder(t->right);
    }
 }

 void inorder(struct tnode* t)
 {
    if(t!=NULL)
    {
      inorder(t->left);
      printf("%d ",t->data);
      inorder(t->right);
    }
  }


 void postorder(struct tnode* t)
  {
    if(t!=NULL)
    {
      postorder(t->left);
      postorder(t->right);
      printf("%d ",t->data);
    }
 }

 
 




   















   
























  
















  







  



















