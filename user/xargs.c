#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    char* args[MAXARG];
    //需要注意的是，参数个数从xargs开始往后算
    for(int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }
    char buf[512];
    int idx_buf = 0;
    //读取前面的参数到buf中
    args[argc] = 0;
    while(1) {
        //读取每一行
        idx_buf = 0;
        while (1)
        {
            int len = read(0, &buf[idx_buf], 1);
            if(len == 0 || buf[idx_buf] == '\n') {
                break;
            }
            idx_buf++;
        }
        if(idx_buf == 0) break;
        buf[idx_buf] = 0;
        args[argc - 1] = buf;
        int pid = fork();
        if(pid == 0) {
            exec(args[0], args);
            exit(0);
        }
        else {
            wait(0);
        }
    }
    exit(0);
}
// int main(int argc, char *argv[])
// {
//   char buf[512];
//   char* full_argv[MAXARG];
//   int i;
//   int len;
//   if(argc < 2){
//     fprintf(2, "usage: xargs your_command\n");
//     exit(1);
//   }
//   // we need an extra arg and a terminating zero
//   // and we don't need the first argument xargs
//   // so in total, we need one extra space than the origin argc
//   if (argc + 1 > MAXARG) {
//       fprintf(2, "too many args\n");
//       exit(1);
//   }
//   // copy the original args
//   // skip the first argument xargs
//   printf("%d\n", argc);
//   for (i = 1; i < argc; i++) {
//       full_argv[i-1] = argv[i];
//       printf("%s\n", argv[i]);
//   }
//   // full_argv[argc-1] is the extra arg to be filled
//   // full_argv[argc] is the terminating zero
//   full_argv[argc] = 0;
//   while (1) {
//       i = 0;
//       // read a line
//       while (1) {
//         len = read(0,&buf[i],1);
//         if (len == 0 || buf[i] == '\n') break;
//         i++;
//       }
//       if (i == 0) break;
//       // terminating 0
//       buf[i] = 0;
//       full_argv[argc-1] = buf;
//       if (fork() == 0) {
//         // fork a child process to do the job
//         exec(full_argv[0],full_argv);
//         exit(0);
//       } else {
//         // wait for the child process to complete
//         wait(0);
//       }
//   }
//   exit(0);
// }
