#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
int find(char* path, char* filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return -1;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return -1;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0) {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_FILE && !strcmp(filename, de.name)) {
            printf("%s\n", buf);
            continue;
        }
        if(st.type == T_DIR && strcmp(".", de.name) != 0 && strcmp("..", de.name) != 0) {
            find(buf, filename);
        }
    }
    return 1;
}
int
main(int argc, char *argv[])
{
  if(argc != 3){
    printf("parameters error\n");
    exit(0);
  }
  if(find(argv[1], argv[2]) < 0) {
    printf("find error\n");
  }
  exit(0);
}
