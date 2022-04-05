#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>     //FD_SET(),FD_LISTEN(),FD_ZERO(),FD_CLR()
#include <ctype.h>

int main(){

    int lfd = socket(AF_INET,SOCK_STREAM,0);

    int opt = 1;
    setsockopt(lfd,SOL_SOCKET,SOL_REUSEADDR,&opt,sizeof(int));

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8888);
    inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr.s_addr);
    bind(lfd,(struct sockaddr*)&serv,sizeof(serv));

    listen(lfd,128);

    int cfd
    fd_set readfds;         //�����ļ�������������
    fd_set tmpfds;          //Ϊ�˷�ֹreadfds���ı䣬�����������ʱ����������һ��
    FD_ZERO(&readfds);      //����ļ�������������
    FD_SET(lfd,&readfds);   //��lfd���뵽readfds������;
    maxfd = lfd;
    int i,n;
    char buf[1024];
    while(1) {
        // int select(int nfds, fd_set *readfds, fd_set *writefds,
        //           fd_set *exceptfds, struct timeval *timeout); 
        //ί���ں˼�ؿɶ�,��д,�쳣�¼�   
        tmpfds = readfds;   //��ֹreadfds�������Ժ�ǰ��Ͳ�������
        nready = select(maxfd+1,&tmpfds,NULL,NULL,NULL)
        //���ھͼ�ؿɶ����ļ�������
        //�Կ�д���쳣�¼��������ģ���ʱ�¼�NULL����ʾ��������
        if(nready < 0) {    //�����˴���
            if(errno == EINTR)  //����Ǳ��ź��жϵģ��ǾͿ��Լ���
                continue;
            break;
        }

        //����пͻ�������������
        if(FD_ISSET(lfd,&tmpfds)){
            cfd = accept(lfd,NULL,NULL);//��ô�ͽ����µĿͻ�����������
            FD_SET(cfd,&readfds);//��cfd���뵽readfds������

            if(maxfd < cfd)//�޸��ں˼�ص��ļ��������ķ�Χ
                maxfd = cfd;
            
            if(--nready == 0)//�������ֻ��һ���ͻ��˵����ӷ������ˣ��Ͳ��ü�����������
                continue;
        }

        //�пͻ������ݷ���
        for(i = lfd+1;i <= maxfd;i++){//��ûһ�����Ӷ�����һ��
            if(FD_ISSET(i,&tmpfds)){
                n = read(i,buf,sizeof(buf));//read����
                if(n <= 0){
                    close(i);
                    FD_CLR(i,&readfds);//���ļ�������i���ں���ȥ��

                }
                write(i,buf,n);//writeӦ�����ݸ��ͻ���
            }
        }

    }

    close(lfd);

    return 0;
}










































