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
void priority_enqueue(int ids,int values);

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
	printf(" \n 3. Priority Enqueue - Enter into the queue with the key vales as priority");
	printf(" \n 4. Display - Show the contents of the queue");
	printf(" \n 5. Exit - Exit the program");


	while(1)
	{
		printf("\n Enter one of the options from the menu: ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				printf("\n Enter the processid: ");
				scanf(" %d", &processid);
				printf("\n Enter the keyvalue: ");
				scanf(" %d", &keyvalue);
				enqueue(processid,keyvalue);
				break;
			case 2:
				delete = dequeue();
				if(delete == NULL)
				{
					printf("Sorry! The queue is empty!");
				}
				else
				{
				printf("The deleted element has a process id:  %d", delete -> pid);
				printf("\n The deleted element has a key value:  %d", delete -> key);
				}
				break;
			case 3:
				printf("\n Enter the processid: ");
				scanf(" %d", &processid);
				printf(" \n Enter the priority key value: ");
				scanf(" %d", &keyvalue);
				priority_enqueue(processid,keyvalue);
				break;
			case 4:
				display();
				break;
			case 5:
				exit(0);
			default:
				printf("\n Please enter the correct choice from the menu!");
				break;
		}
		}
}

//Priority enqueue 
void priority_enqueue(int ids,int values)
{
    struct node *find  = front;
	struct node *link1 = (struct node*) malloc (sizeof(struct node));
	link1 -> pid = ids;
	link1 -> key = values;
	// No node in the queue
	if(front == NULL)
	{
		front = rear = link1;
	}
	else
	{ 
		//Most priority
		if(link1 -> key > find -> key)
		{
			front -> prev = link1;
			link1 -> next = front;
			front = link1;
		}

		//Looking for the position to place the node according to priority
		else
		{
			while(link1 -> key < find -> key)
			{
				if(find -> next == NULL)
				break;
				find = find -> next;
	}
			if(find -> next == NULL && link1 -> key < find -> key)
			{
				rear -> next = link1;
				link1 -> prev = rear;
				rear = link1;
			}

			else
			{
				find -> next -> prev = link1;
				link1 -> prev = find -> prev;
				link1 -> next = find;
				find -> prev = link1;
			}
		}
	}
}


//Inserting in the end (FIFO Xinu requirements)
void enqueue(int id, int value)
{
	struct node *link = (struct node*) malloc (sizeof(struct node));
	link -> pid = id;
	link -> key = value;
//Nothing is present in the queue
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
	struct node *temp;

	//No  node left in the queue
	if(front == NULL)
	{
		temp = NULL;
		return temp;
		
	}
	//Only one node left in the queue
	else if(front -> next == NULL)
	{
		struct node* tempo = front;
		temp = tempo;
		front = NULL;
		rear  = NULL;
		return temp;
	}
	else //Making sure all the pointers are taken care of
	{
		struct node* tempo1 = front;
		temp = tempo1;
		front = front -> next;
		front -> prev = NULL;

		return temp;
	}


	//returning temp returns the deleted element
}
		


void display()
{
	if(front != NULL)
	{
	struct node *temp1 = front;
	printf(" \n");
	//Navigate till the end of the list till the next node exists
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


