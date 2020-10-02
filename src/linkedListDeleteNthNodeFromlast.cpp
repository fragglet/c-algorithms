// C++ implementation of the approach 
#include<bits/stdc++.h> 
using namespace std; 

class LinkedList 
{ 
	public: 
	
	// Linked list Node 
	class Node 
	{ 
		public: 
		int data; 
		Node* next; 
		Node(int d) 
		{ 
			data = d; 
			next = NULL; 
		} 
	}; 
	
	// Head of list 
	Node* head; 

	// Function to delete the nth node from 
	// the end of the given linked list 
	Node* deleteNode(int key) 
	{ 

		// First pointer will point to 
		// the head of the linked list 
		Node *first = head; 

		// Second pointer will point to the 
		// Nth node from the beginning 
		Node *second = head; 
		for (int i = 0; i < key; i++) 
		{ 

			// If count of nodes in the given 
			// linked list is <= N 
			if (second->next == NULL) 
			{ 

				// If count = N i.e. 
				// delete the head node 
				if (i == key - 1) 
					head = head->next; 
				return head; 
			} 
			second = second->next; 
		} 

		// Increment both the pointers by one until 
		// second pointer reaches the end 
		while (second->next != NULL) 
		{ 
			first = first->next; 
			second = second->next; 
		} 

		// First must be pointing to the 
		// Nth node from the end by now 
		// So, delete the node first is pointing to 
		first->next = first->next->next; 
		return head; 
	} 

	// Function to insert a new Node 
	// at front of the list 
	Node* push(int new_data) 
	{ 
		Node* new_node = new Node(new_data); 
		new_node->next = head; 
		head = new_node; 
		return head; 
	} 

	// Function to print the linked list 
	void printList() 
	{ 
		Node* tnode = head; 
		while (tnode != NULL) 
		{ 
			cout << (tnode->data) << ( " "); 
			tnode = tnode->next; 
		} 
	} 
}; 

// Driver code 
int main() 
{ 
	LinkedList* llist = new LinkedList(); 

	llist->head = llist->push(7); 
	llist->head = llist->push(1); 
	llist->head = llist->push(3); 
	llist->head = llist->push(2); 

	cout << ("Created Linked list is:\n"); 
	llist->printList(); 

	int N = 1; 
	llist->head = llist->deleteNode(N); 

	cout << ("\nLinked List after Deletion is:\n"); 
	llist->printList(); 
} 

// This code is contributed by Arnab Kundu 
