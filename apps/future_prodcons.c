#include <xinu.h>
#include <future.h>
  uint32 future_prod (future_t * fut, int n)
{
  printf("Produced %d\n",n);
  int status = future_set (fut,n);
  if (status < 1)
    {
      printf ("future_set failed\n");
      return SYSERR;
    }
  return OK;
}

uint32 future_cons (future_t * fut)
{
  int i, status;
  status = (int) future_get (fut, &i);
  if (status < 1)
    {
      printf ("future_get failed\n");
      return -1;
    }
 // printf ("it produced %d\n", i);
  printf("Consumed %d\n", i); 
  return OK;
}
