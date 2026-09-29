#include<process_copy.h>
int check_pram(int argc,const char* srcfile,int pronum)
{
//实现代码
	if(argc<3)
		return -1;
	if((access(srcfile,F_OK)==-1))
		return -2;
	if(pronum<=3||pronum>100)
		return -3;
	return 0;
}
