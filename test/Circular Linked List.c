#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
struct node *head=NULL;
struct node
{
	int key;
	struct node *addr;
};
void ins_at_beg(int k)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(head==NULL)
	{
		temp->key=k;
		temp->addr=temp;
		head=temp;
	}
	else
	{
		struct node *a;
		a=head;
		while(a->addr!=head)
		{
			a=a->addr;
		}
		a->addr=temp;
		temp->key=k;
		temp->addr=head;
		head=temp;
		
	}
}
void ins_at_end(int k)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(head==NULL)
	{
		temp->key=k;
		temp->addr=head;
		head=temp;
	}
	else
	{
	struct node *a;
	a=head;
	while(a->addr!=head)
	{
		a=a->addr;
	}
	a->addr=temp;
	temp->key=k;
	temp->addr=head;
	}
}
void ins_at_pos(int k,int pos)
{
	struct node *temp;
	temp=(struct node *)malloc(sizeof(struct node));
	if(head==NULL)
	{
		ins_at_beg(k);
	}
    else if(head->addr==head)
    {
    	if(pos==1)
    	{
    		ins_at_beg(k);
		}
		else if(pos==2)
		{
			ins_at_end(k);
		}
	}
	else
	{
		struct node *a;
		a=head;
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
	if(head==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(head->addr==head)
	{
		head=NULL;
		free(head);
	}
	else
	{
	 struct node *a;
	a=head;
	while(a->addr!=head)
	{
		a=a->addr;
	}
	a->addr=head->addr;
	free(head);
	head=a->addr;
	}
}
void del_at_end()
{
	if(head==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(head->addr==head)
	{
		head=NULL;
		free(head);
	}
	else
	{
		struct node *a,*temp;
		a=head;
		while(a->addr->addr!=head)
		{
			a=a->addr;
		}
		temp=a->addr;
		a->addr=head;
		free(temp);
		temp=NULL;
	}
}
void del_at_pos(int pos)
{
	if(head==NULL)
	{
		printf("No nodes available!\n");
	}
	else if(head->addr==head)
	{
		if(pos==1)
		{
			del_at_beg();
		}
		else if(pos==2)
		{
			del_at_end();
		}
	}
	else
	{
		int i=1;
		struct node *a,*temp;
		a=head;
		while((a->addr!=head)&&(i<pos-1))
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
	if(head==NULL)
	{
		printf("No nodes\n");
	}
	else
	{
		struct node *a;
		a=head;
		printf("The elements are:\n");
		while(a->addr!=head)
		{
			printf("%d\n",a->key);
			a=a->addr;
		}
		printf("%d\n",a->key);
		
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


