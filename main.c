// 02-parent-child.c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


// corner case:
// text: [aaaa.aaa] (5000 symbols)
// substr: a
// output.txt = [0    1    2    3    ... 4998 4999] //  5000 * (4 + 1) = 25 000 symbols
const int buf_size = 25100;

int main(int argc, char* argv[]) {
    int    fd[2], result;
    int fd_arr[2];
    size_t size;
    //char* tt[] = {"", "text.txt", "ab", "output.txt"};
    //argv = tt;

    if(pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if(pipe(fd_arr) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();
    if(result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) { /* Parent process */
        if(close(fd[0]) < 0){
            printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
        }
        int my_fd = open(argv[1], O_RDONLY, 0666);

        char   my_str_buf[buf_size];
        int my_size = read(my_fd, my_str_buf, sizeof(my_str_buf) - 1);
        if (my_size > 5000) {
            printf("only files smaller than 5000");
            exit(-1);
        }
        close(my_fd);
        my_str_buf[my_size] = 0;

        size = write(fd[1], my_str_buf, my_size + 1);
        if(size != my_size + 1){
            printf("Can\'t write all string to pipe\n");
            exit(-1);
        }
        if(close(fd[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }
    } else { /* Child process */
        char   str_buf[buf_size];
        if(close(fd[1]) < 0){
            printf("child: Can\'t close writing side of pipe\n"); exit(-1);
        }
        size = read(fd[0], str_buf, buf_size);
        if(close(fd[0]) < 0){
            printf("child: Can\'t close reading side of pipe\n"); exit(-1);
        }

        if(size < 0){
            printf("Can\'t read string from pipe\n");
            exit(-1);
        }

        /// logic
        char last_str_buf[buf_size];
        char* last_pointer = last_str_buf;
        char* pointer = str_buf;

        while (pointer - str_buf != size) {
            char *ss = strstr(pointer, argv[2]);
            if (ss) {
                last_pointer[0] = last_pointer[1] = last_pointer[2] = last_pointer[3] = last_pointer[4] = ' ';
                sprintf(last_pointer, "%ld", ss - str_buf);
                last_pointer[strlen(last_pointer)] = ' ';
                last_pointer += 5;
                pointer = ss + strlen(argv[2]);
            } else {
                break;
            }
        }
        *last_pointer = 0;
        ///
        int new_result = fork();

        if (new_result > 0) {
            /// write data for second time
            if(close(fd_arr[0]) < 0){
                printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
            }
            size = write(fd_arr[1], last_str_buf,  strlen(last_str_buf) + 1);
            if(size != strlen(last_str_buf) + 1){
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            }
            if(close(fd_arr[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }
            ///
        } else if (new_result == 0) {
            char output[buf_size];

            if(close(fd_arr[1]) < 0){
                printf("child: Can\'t close writing side of pipe\n"); exit(-1);
            }
            size = read(fd_arr[0], output, buf_size);

            int output_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);

            write(output_fd, output, strlen(output));
        } else {
            printf("something wrong, i can feel it\n");
        }
    }
    return 0;
}
