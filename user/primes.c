#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int primes[281];

void dfs(int cnt)
{
    //  埃氏筛法，进程中的第一个素为质数，用其倍数筛掉其他数
    int x = primes[0];
    printf("prime %d\n", x);

    if (cnt == 1)
        return;

    // 创建管道
    int p[2];
    pipe(p);

    int pid = fork();

    if (pid == 0)
    {   // 子进程中
        int num = 0; // 父进程筛除后剩余数的个数
        close(p[1]);
        while (read(p[0], &primes[num++], sizeof(int)))
            ;

        close(p[0]);
        dfs(num - 1); // 最后read返回0，num还要加一次，所以这里减1
        exit(0); // 记得退出
    }
    else
    {   // 父进程中
        close(p[0]);
        for (int i = 1; i < cnt; i++)
        {
            if (primes[i] % x != 0)
                write(p[1], primes + i, sizeof(int));
        }
        close(p[1]); // 记得及时关闭fd，否则会占用资源
        wait(0); // 父进程必须要等待子进程的递归都结束了，再返回该层递归
    }
}

int main(void)
{
    for (int i = 0; i <= 280; i++)
        primes[i] = i + 2;

    dfs(280); // 2~35

    exit(0);
}
