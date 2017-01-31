#include <stdio.h>
#include <stdlib.h>

//Information in a node in a linked list
struct node
{
	int pid;
	int key;
	struct node *next;
	struct node *prev;
};

struct node *current = NULL;
struct node *rear = NULL;
struct node *front = NULL;

// Various functions of the queue
void enqueue(int id, int value);
struct node* dequeue();
void display();

//Main Function
void main()
{
	int choice;
	int processid;
	int keyvalue;
	struct node *delete;
	printf(" Please choose one of the following options");
	printf(" \n 1. Enqueue - Enter in the queue ");
	printf(" \n 2. Dequeue - Delete from the queue");
	printf(" \n 3. Display - Show the contents of the queue");
	printf(" \n 4. Exit - Exit the program");


	while(1)
	{
		printf("\n Enter one of the options from the menu");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("\n Enter the processid");
				scanf(" %d", &processid);
				printf("\n Enter the keyvalue");
				scanf(" %d", &keyvalue);
				enqueue(processid,keyvalue);
				break;
			case 2:
				delete = dequeue();
				if(delete == NULL)
				{
					printf("Sorry the queue is empty");
				}
				else
				{
				printf("The deleted element has a process id %d", delete -> pid);
				printf("The deleted element has a key value %d", delete -> key);
				}
				break;
			case 3:
				display();
				break;
			case 4:
				exit(0);
			default:
				printf("\n Please enter the correct choice from the menu!");
				break;
		}
		}
}


//Inserting in the end (FIFO Xinu requirements)
void enqueue(int id, int value)
{
	struct node *link = (struct node*) malloc (sizeof(struct node));
	link -> pid = id;
	link -> key = value;

	if(front == NULL)
	{
		front= rear = link;
	}
	else
	{
		rear -> next = link;
		link -> prev = rear;
	}

	rear = link;

}

//Deleting from the beginning (FIFO Xinu requirements)
struct node* dequeue()
{
	struct node *temp = front;

	//Only one node left in the queue
	if(front -> next == NULL)
	{
		rear = NULL;
		temp = NULL;
		
	}
	else
	{
		front -> next -> prev = NULL;
	}

	front = front -> next;

	//returning the deleted element
	return temp;
}
		


void display()
{
	if(front != NULL)
	{
	struct node *temp1 = front;
	printf(" \n");
	//Navigate till the end of the list
	while(temp1 != NULL)
	{
		printf(" Processid : %d , Keyvalue : %d \n", temp1 -> pid, temp1 -> key);
		temp1 = temp1 ->next;
	}
	}
	else
	{
		printf("\n Sorry the queue is empty");
	}
}


