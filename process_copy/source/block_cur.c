#include<process_copy.h>

int block_cur(const char *srcfile,int pronum)
{
	//计算文件大小
	int sfd;
	int filesize;
	sfd =open(srcfile,O_RDONLY);
	filesize=lseek(sfd,0,SEEK_END);
	if(filesize%pronum==0)
		return filesize/pronum;
	else
		return filesize/pronum+1;
}
