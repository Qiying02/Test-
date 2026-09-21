#include <regex.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main()
{
	//准备表达式
	char * regstr="<a [^>]\\+\\?href=\"\\([^\"]\\+\\?\\)\"[^>]\\+\\?>\\([^<]\\+\\?\\)</a>";
	//准备正则
	regex_t reg;
	//正则转换
	regcomp(&reg,regstr,0);
	//关于数据源
	int fd;
	fd= open("url.txt",O_RDWR);
	int size;
	size= lseek(fd,0,SEEK_END);
	char * mmap_data=NULL;
	//内存映射
	mmap_data=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	close(fd);
	//验证是否映射成功
	//printf("%s\n",mmap_data);
	//遍历查找
	int regnum =3;
	regmatch_t match[regnum];
	char link[1024];
	char title[1024];
	char *cur_ptr = mmap_data;   //新增遍历指针，保留原始mmap_data不变

	while((regexec(&reg, cur_ptr, regnum, match,0)==0))
	 {
		//提取数据
		bzero(link,sizeof(link));
		bzero(title,sizeof(title));
		int len_link = match[1].rm_eo - match[1].rm_so;
		int len_title = match[2].rm_eo - match[2].rm_so;
		snprintf(link, sizeof(link), "%.*s", len_link, cur_ptr + match[1].rm_so);
		snprintf(title, sizeof(title), "%.*s", len_title, cur_ptr + match[2].rm_so);
		cur_ptr += match[0].rm_eo;      //移动副本指针，不要动mmap_data
		printf("匹配结果,title=%s link =%s\n",title,link);																						 }

	regfree(&reg);
	return 0;
}
