#include <stdio.h>  //perror()���������Ϣ
#include <stdlib.h> //exit()
#include <sys/types.h>
#include <sys/socket.h>     //socket(),bind(),listen()
#include <string.h> //bzero()
#include <arpa/inet.h> //htons(),inet_pton()
#include <unistd.h> //read(),write(),close()
#include <ctype.h> //toupper()


// man 7 ip �鿴��ַ
int main(){
    // ����socket������˵��ļ����������ںͼ���
    // int socket(int domain, int type, int protocol);
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd < 0){
		perror("socket error!");
        return -1;
    }

    // struct sockaddr_in {
    //            sa_family_t    sin_family; /* address family: AF_INET */
    //            in_port_t      sin_port;   /* port in network byte order */
    //            struct in_addr sin_addr;   /* internet address */
    //        };
    //  /* Internet address. */
    //        struct in_addr {
    //            uint32_t       s_addr;     /* address in network byte order */
    //        };
    struct sockaddr_in serv;
    // void bzero(void *s, size_t n);   //���ڴ���ǰn���ֽ�����
    bzero(&serv,sizeof(serv));
    //�ṹ��ʹ��֮ǰ��Ҫ��ʼ��
    //ȷ����ʹ�õ�����ڴ��Ѿ���ʼ���ˣ�����ṹ������ڴ����������ֵ���������õ��²���Ԥ֪�ĺ�������صĿ��ܵ��³��������
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    serv.sin_addr.s_addr = htonl(INADDR_ANY); //��ʾʹ�ñ����������IP
    // �󶨣���֮ǰ��Ҫ�ȶ���ṹ��
    // int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
    //���д������д�����ʱ��Ҫ�õ�ַ
    int ret = bind(lfd,(struct sockaddr *)&serv,sizeof(serv));
    if(ret < 0) {
        perror("bind error!");
        return -1;
    }

    // int listen(int sockfd, int backlog);
    // ���������ܷ���ֵ
    listen(lfd,128);

    // sleep(30);//�۲�listen�Ժ���������ǲ����Ѿ�����
    //��������accept֮ǰ�Ѿ�������
    // ����accept��������˵�½�һ�����ӣ����Ǵ������Ӷ�����ȡ��һ����������


    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    // �ڶ����ǿͻ���IP�Ķ˿ڣ������ľͲ�д��
    // ����һ���������󣬷���ֵ��һ���ļ�������������ͨѶ
    struct sockaddr_in client;
    socklen_t len = sizeof(client);
    // int cfd = accept(lfd,NULL,NULL); 
    int cfd = accept(lfd,(struct sockaddr*)&client,&len); //len��һ�������������
    //��ȡ�ͻ��˵�IP,���ã����ú�����������֪��˭��������
    //  const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
    //��ȡclient�˵�IP�Ͷ˿�
    char sIP[16];
    memset(sIP,0x00,sizeof(sIP));
    printf("client-->IP:[%s],PORT:[%d]\n",inet_ntop(AF_INET,&client.sin_addr.s_addr,sIP,sizeof(sIP)),ntohs(client.sin_port));
    printf("lfd==[%d],cfd==[%d]\n",lfd,cfd);

    int i = 0;
    int n = 0;
    char buf[1024];
    while(1) {
        // void *memset(void *s,int ch,size_t n);  //����Ҳ����ʹ�����
        // memset(&my_addr, 0, sizeof(struct sockaddr_un));
        // ������
        memset(buf,0x00,sizeof(buf));
        //  ssize_t read(int fd, void *buf, size_t count); 
        n = read(cfd,buf,sizeof(buf));
        if(n <= 0) {
            //0�ǶԷ��ر�����
            //-1Ϊ��ȡʧ��
            printf("read error or client close,n==[%d]\n",n);
            break;
        }
        printf("n==[%d],buf==[%s]",n,buf);

        for(i = 0;i < n;i++) {
            // int toupper(int c);
            buf[i] = toupper(buf[i]);//ת���ɴ�д
        }
        //  ������
        //  ssize_t write(int fd, const void *buf, size_t count);
        write(cfd,buf,n);//���յ���λ��ת���ɴ�д���ͼ�λ
    }
    // int close(int fd);
    // �رռ����ļ���������ͨ���ļ�������
    close(lfd);
    close(cfd);

    return 0;
}


// ʹ��nc���Թ��߽��в���
// nc 127.0.0.1 8888
// nc 127.1 8888
// ֱ�ӷ��ͱ��磬nihao,hello,��ת���ɴ�д
// �ʼ�����ڹر�nc�Ժ󣬷�������һֱ�������ݣ������ظ���ʾ