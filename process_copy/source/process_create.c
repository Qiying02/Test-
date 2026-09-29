#include<process_copy.h>

int process_create(const char * srcfile,const char *destfile,int pronum, int blocksize)
{
	pid_t pid;
	int i;
	for(i=0;i<pronum;i++)
	{
		pid=fork();
		if(pid==0) break;
	}
	if(pid>0){
	//父进程循环回收
	process_wait();
	}else if(pid==0){
		printf("child %d alive\n",getpid());
		int offset;
		offset= i*blocksize;
		char str_offset[100];
		char str_blocksize[100];
		sprintf(str_offset,"%d",offset);//整型转为字符串
		sprintf(str_blocksize,"%d",blocksize);
		execl("/home/colin/20250915/Process/process_copy/MOD/copy","copy",srcfile,destfile,str_offset,str_blocksize,NULL);
	}else{
		perror("fork call failed");
		return -1;
	}
}
