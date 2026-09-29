#include<process_copy.h>

int main(int argc,char **argv)
{
	int pronum;
	int blocksize;
	if(argv[3]==0)
		pronum =3;
	else
		pronum =atoi(argv[3]);
	int pram =check_pram(argc,argv[1],pronum);//参数验证
	if(pram<0){
		if(pram==-1)
			printf("用户输入参数数量不符合要求\n");
		if(pram==-2)
			printf("用户输入的源文件不存在\n");
		if(pram==-3)
			printf("用户输入的进程数量不符合要求\n");
		exit(0);
	}else{
		printf("参数验证通过，用户输入参数合法\n");
	}
	blocksize=block_cur(argv[1],pronum);//切片
	process_create(argv[1],argv[2],pronum,blocksize);


	return 0;
}
