#include <xinu.h>
#include <future.h>
syscall future_free(future_t* f){
		freemem(f,sizeof(future_t));
}
