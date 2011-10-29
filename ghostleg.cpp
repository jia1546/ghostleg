#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define MAXSIZE 10
#define BOTTOM 280
struct node
{
	int x;	//line number of the cross-switch
	int y;	//line number of the cross-switch
	int z;	//distance from the top
	struct node *next;
};
typedef node node;

struct timeval start_time, end_time;

/*
** search whether there exists such a cross-switch
** exist then return 1, otherwise return 0
*/
int search(node *head, int x, int y, int z)
{
	node *tmp = head;
	while(tmp -> next != NULL)
	{
		tmp = tmp -> next;
		if(tmp -> x == x && tmp -> y == y && tmp -> z == z)
			return 1;
	}
	return 0;
}

void display(node *head)
{
	node *tmp = head;
	while(tmp -> next != NULL)
	{
		tmp = tmp -> next;
		printf("%d %d %d\n", tmp -> x, tmp -> y, tmp -> z);
	}
}

int main()
{
	gettimeofday(&start_time, NULL);
	int n, p, i;
	int para1, para2, para3;
	char cmd[MAXSIZE+1];
	node *head, *tmp, *pre;
	FILE *fp = NULL;
	if((fp = fopen("ghostleg.txt", "rt")) == NULL)
		printf("error opening the file ghost.txt!\n");
	fscanf(fp, "%d%d", &n, &p);
	head = (node*)malloc(sizeof(node));
	head -> x = -1;
	head -> y = -1;
	head -> z = -1;
	head -> next = NULL;
	
	for(i=0; i<p; i++)
	{
		node *newNode;
		newNode = (node*)malloc(sizeof(node));
		fscanf(fp, "%d%d%d", &(newNode -> x), &(newNode -> y), &(newNode -> z)); // needs sorting here
		newNode -> next = NULL;
		
		if(head -> next == NULL)
			head -> next = newNode;
		else
		{
			pre = head;
			tmp = head -> next;
			while(tmp != NULL && tmp -> z < newNode -> z)
			{
				pre = tmp;
				tmp = tmp -> next;	
			}
			if(tmp == NULL)
				pre -> next = newNode;
			else
			{
				pre -> next = newNode;
				newNode -> next = tmp;
			}
		}
	}
	
	display(head);
	
	/*
	**get commands from the user
	**each brach handle a specified command and output the results
	*/
	while(scanf("%s%d%d%d", cmd, &para1, &para2, &para3) != EOF)
	{
		if(strcmp(cmd, "findresult") == 0)
		{
			int position = para1;
			tmp = head -> next;
			while(tmp != NULL)
			{
				if(tmp -> x == position)
					position = tmp -> y;
				else if(tmp -> y == position)
					position = tmp -> x;
				tmp = tmp -> next;
			}
			printf("%d\n", position);
		}
		else if(strcmp(cmd, "path") == 0)
		{
			int position = para1;
			printf("<");
			tmp = head -> next;
			while(tmp != NULL)
			{
				if(tmp -> x == position)
				{
					position = tmp -> y;
					printf("(%d,%d) ", tmp -> x, tmp -> z);
				}
				else if(tmp -> y == position)
				{
					position = tmp -> x;
					printf("(%d,%d) ", tmp -> y, tmp -> z);
				}
				tmp = tmp -> next;
			}
			printf("(%d,%d)>\n", position, BOTTOM);
		}
		else if(strcmp(cmd, "insert") == 0)
		{
			int result = search(head, para1, para2, para3);
			if(result == 1)
				printf("insert failed\n");
			else
			{
				node *newNode = (node*)malloc(sizeof(node));
				newNode -> x = para1;
				newNode -> y = para2;
				newNode -> z = para3;
				newNode -> next = NULL;
				
				if(head -> next == NULL)
					head -> next = newNode;
				else
				{
					pre = head;
					tmp = head -> next;
					while(tmp != NULL && tmp -> z < newNode -> z)
					{
						pre = tmp;
						tmp = tmp -> next;
					}
					if(tmp == NULL)
						pre -> next = newNode;
					else
					{
						pre -> next = newNode;
						newNode -> next = tmp;
					}
				}
				
				printf("after inserting...\n");
				display(head);
			}
		}
		else if(strcmp(cmd, "delete") == 0)
		{
			int result = search(head, para1, para2, para3);
			if(result == 0)
				printf("remove failed\n");
			else
			{
				pre = head;
				tmp = head -> next;
				while(!(tmp -> x == para1 && tmp -> y == para2 && tmp -> z == para3))
				{
					pre = tmp;
					tmp = tmp -> next;
				}
				pre -> next = tmp -> next;
				free(tmp);
				
				printf("after deleting...\n");
				display(head);
			}			
		}
		else if(strcmp(cmd, "count") == 0)
		{
			int counter = 0;
			tmp = head -> next;
			while(tmp != NULL)
			{
				if(tmp -> x == para1 || tmp -> y == para1)
					counter++;
				tmp = tmp -> next;
			}	
			printf("%d\n", counter);
		}
	}
	fclose(fp);
	free(head);
	free(pre);
	free(tmp);
	gettimeofday(&end_time, NULL);
  	long usec = (end_time.tv_sec - start_time.tv_sec)*1000000
       	 + (end_time.tv_usec - start_time.tv_usec);
    printf("time spent in this program = %ld usec\n", usec);
	return 0;
}
