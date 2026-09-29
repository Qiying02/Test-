#include<process_copy.h>

void process_wait(void)
{
	pid_t zpid;
	while((zpid=waitpid(-1,NULL,WNOHANG))!=-1)
	{
		if(zpid>0)
		{
			printf("parent %d wait zombie sucess,zpid %d\n",getpid(),zpid);
		}
		sleep(1);
	}
	
}
