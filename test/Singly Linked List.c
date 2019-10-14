#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
struct node *front=NULL;
struct node *rear=NULL;
struct node
{
	int key;
	struct node *addr;
};
void ins_at_beg(int k)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(front==NULL)
	{
		temp->key=k;
		temp->addr=NULL;
		front=rear=temp;
	}
	else
	{
		temp->key=k;
		temp->addr=front;
		front=temp;
		
	}
}
void ins_at_end(int k)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(front==NULL)
	{
		temp->key=k;
		temp->addr=NULL;
		front=rear=temp;
	}
	else
	{
	rear->addr=temp;
	temp->key=k;
	temp->addr=NULL;
	rear=temp;
	}
}
void ins_at_pos(int k,int pos)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(pos==1)
	{
		ins_at_beg(k);
	}
	else
	{
		struct node *a;
		a=front;
		int i=1;
		while(i<pos-1)
		{
			a=a->addr;
			i++;
		}
		temp->addr=a->addr;
		a->addr=temp;
		temp->key=k;
	}
}
void del_at_beg()
{
	if(front==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(front==rear)
	{
		front=rear=NULL;
		free(front);
	}
	else
	{
	 struct node *a;
	 a=front->addr;
	 front->addr=NULL;
	 free(front);
	 front=a;
	}
}
void del_at_end()
{
	if(front==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(front==rear)
	{
		front=rear=NULL;
		free(front);
	}
	else
	{
		struct node *a;
		a=front;
		while(a->addr!=rear)
		{
			a=a->addr;
		}
		a->addr=NULL;
		free(rear);
		rear=a;
	}
}
void del_at_pos(int pos)
{
	if(front==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(front==rear)
	{
		front=rear=NULL;
		free(front);
	}
	else
	{
		int i=1;
		struct node *a,*temp;
		a=front;
		while(i<pos-1)
		{
			a=a->addr;
			i++;
		}
		temp=a->addr;
		a->addr=a->addr->addr;
		temp->addr=NULL;
		free(temp);
	}
}
void print()
{
	if(front==NULL)
	{
		printf("No nodes\n");
	}
	else
	{
		struct node *a;
		a=front;
		printf("The elements are:\n");
		while(a!=NULL)
		{
			printf("%d\n",a->key);
			a=a->addr;
		}
		
	}
}

void main()
{
	int a,b,c,pos,k,pos1,k1;
	while(1)
	{
		printf("Select the option:\n");
		printf("1.Insert\n2.Delete\n3.Print\n4.Exit\n");
		scanf("%d",&a);
		switch(a)
		{
		   case 1: printf("Select the option:\n");
			        printf("1.1 Insert at beginning\n1.2 Insert at end\n1.3 Insert at position\n1.4 Exit\n");
			        scanf("%d",&b);
			        switch(b)
			        {
			        	case 1: printf("Enter the element to be inserted:");
			        	        scanf("%d",&k);
			        	        ins_at_beg(k);
			        	        break;
			        	case 2: printf("Enter the element to be inserted:");
			        	        scanf("%d",&k);
			        	        ins_at_end(k);
			        	        break;
			        	case 3: printf("Enter the element and position:");
			        	        scanf("%d%d",&k,&pos);
			        	        ins_at_pos(k,pos);
			        	        break;
			        	case 4: break;
			        	default: printf("Error!!");
			        	
			    	}
		            break;	
	       case 2: printf("Select the option:\n");
	     	       printf("2.1 Delete at beginning\n2.2 Delete at end\n2.3 Delete at position\n2.4 Exit\n");
			       scanf("%d",&c);
			       switch(c)
			        {
			        	case 1:	del_at_beg();
			        	        break;
			        	case 2: del_at_end();
			        	        break;
			        	case 3: printf("Enter the position:");
			        	        scanf("%d",&pos1);
			        	        del_at_pos(pos1);
			        	        break;
			        	case 4: break;
			        	default: printf("Error!!");
			        }
		            break;
		   case 3: print();
		           break;
		   case 4: exit(0);
		   default : printf("Error!!");
		}
		
     } 
     
	}


