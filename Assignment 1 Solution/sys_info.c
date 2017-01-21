#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int length = 0;
	int i,j;
	int readingfromchild;
	int pipedestination[2];
	pid_t processid;
	char buffer[BUFSIZ];
	if(argc < 2)
	{
		printf("Please provide atleast one parameter to run execl() \n");
	}


	if(pipe(pipedestination) < 0 )
	{
		printf("Pipe Error \n");
	}
	else if((processid = fork()) < 0)
	{
		printf("Fork Error \n");
	}
	else if(processid > 0)
	{
		/* This is a parent process */
		printf("This is a parent process %d \n",processid);
		close(pipedestination[0]);
			for(i = 1;i < argc; i++)
			{
				write(pipedestination[1], argv[i],strlen(argv[1]));
			}
		wait(0);
	}
	else
	{
		/*This is a child process */
		printf("This a child process %d \n",processid);
		close(pipedestination[1]);
		for(j = 0; j < argc; j++)
		{
			length += (strlen(argv[j]));
		}

		readingfromchild = read(pipedestination[0],buffer,length);

		if(strcmp(argv[1],"echo") == 0)
				{
		execl("/usr/bin/echo","echo","hello world", (char*)0);
				}
	}
     char dest[50];
	 strcpy(dest, "/usr/bin/");
	 strcat(dest, argv[1]);
	 execl(dest,argv[1], 0 );
   //  execl("/usr/bin/date","date",0);
	exit(0);
}

