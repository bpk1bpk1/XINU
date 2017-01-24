#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <process_ring.h>
void ring_process (int);
volatile int count = 0;
int numberofrounds;
shellcmd xsh_process_ring (int32 args, char *argv[])
{
  int i;
  numberofrounds = 5;
  if (args == 2)
    {
      numberofrounds = atoi(argv[1]);
    }
  if (args == 2 && strncmp (argv[1], "--help", 7) == 0)
    {
      printf ("Usage: %s <rounds>\n\n ", argv[0]);
      return 0;
    }
  printmessage[0] = count = numberofprocesses * numberofrounds;
  for (i = 0; i < numberofprocesses; i++)
    {
      resume (create (ring_process, 1024, 20, "i", 1, i));
    }
  return 0;
}
