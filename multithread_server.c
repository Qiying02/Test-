#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>
#include<sys/wait.h>
#include<time.h>
void * recv_thread(void *arg)
{
	int client_sockfd =*(int*)arg;
	free(arg);
	//响应系统时间业务
	char buffer[4096];
	char * dfl_msg="please try again\n";
	time_t tp;
	char time_buf[1024];
	int len;
	bzero(buffer,sizeof(buffer));
	while((len=recv(client_sockfd,buffer,sizeof(buffer),0))>0){
				//时间处理及响应
				buffer[strlen(buffer)-1]='\0';
				if(strcmp(buffer,"time")==0){
					bzero(time_buf,sizeof(time_buf));
					tp=time(NULL);
					//`time_r(时间戳指针, 你自己的字符数组)`，把时间戳直接转成人类可读的时间字符串，线程安全，网络服务器日志打印常用。
					ctime_r(&tp,time_buf);
					send(client_sockfd,time_buf,strlen(time_buf),MSG_NOSIGNAL);
				}else
					send(client_sockfd,dfl_msg,strlen(dfl_msg),MSG_NOSIGNAL);
				bzero(buffer,sizeof(buffer));
	}

	if(len ==-1) {
		perror("recv call failed\n");
		close(client_sockfd);
		return NULL;
	}
	else if(len==0){
			printf("client exit ,close clientsockfd %d\n",client_sockfd);
			close(client_sockfd);
			return NULL;//变成僵尸
	}
}
int main()
{
	//网络初始化
	int server_sockfd;
	int client_sockfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t addrlen;
	if((server_sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
		perror("sock call failed\n");
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(8080);
	server_addr.sin_addr.s_addr=INADDR_ANY;//127.0.0.1本机地址
	if((bind(server_sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)))==-1)
		perror("bind call failed\n");
	listen(server_sockfd,128);
	pid_t pid;
	printf("process server Runing,parent %d Accepting..\n",getpid());
	while(1){
		addrlen =sizeof(client_addr);
		if((client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_addr,&addrlen))==-1){
		perror("accept call failed\n");
		exit(0);
		}
		char response[4096];
		char cip[16];
		bzero(cip,16);
		bzero(response,sizeof(response));
		inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,cip,16);
		sprintf(response,"hi %s,connect 127.0.0.1 success.\n",cip);
		send(client_sockfd,response,strlen(response),MSG_NOSIGNAL);
		printf("sever accept success ,client ip %s client port %d\n",cip,ntohs(client_addr.sin_port));
		//修改线程属性，改变其中的退出状态
		pthread_attr_t attr;
		pthread_attr_init(&attr);//默认属性
		//直接设置属性中线程退出状态
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		pthread_t tid;
		int pthread_err;
		int *pclient =malloc(sizeof(int));
		if(pclient==NULL)
		{
			perror("malloc failed");
			close(client_sockfd);
			pthread_attr_destroy(&attr);
			continue;
		}
		*pclient =client_sockfd;
		if((pthread_err=pthread_create(&tid,&attr,recv_thread,pclient))!=0)
		{
			printf("create thread error:%s\n",strerror(pthread_err));
			free(pclient);
			close(client_sockfd);
			pthread_attr_destroy(&attr);
			continue;
		}
		pthread_attr_destroy(&attr);	

	}
	printf("server exit\n");
	close(server_sockfd);
	exit(0);

	return 0;
}

