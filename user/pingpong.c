#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    if(argc != 1) {
        printf("parameter wrong\n");
        exit(1);
    }
    int p[2];
    int p1[2];
    pipe(p);
    pipe(p1);
    int pid = fork();
    if(pid == 0) {
        //子进程
        //关闭p管道的写端
        char buf[10];
        close(p[1]);
        read(p[0], buf, 1);
        printf("%d: received ping\n", getpid());
        //关闭管道
        close(p[0]);
        
        //关闭p1管道的读端
        close(p1[0]);
        write(p1[1], "a", 1);
        close(p1[1]);

        exit(0);
    }
    else {
        //父进程
        //关闭p管道的读端
        close(p[0]);
        //从p管道写数据
        write(p[1], "a", 1);
        close(p[1]);

        char buf[10];
        close(p1[1]);
        read(p1[0], buf, 1);
        printf("%d: received pong\n", getpid());
        close(p1[0]);
        exit(0);
    }
}