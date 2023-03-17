# Мовшин Максим | БПИ213 | ИДЗ-1 | Вариант 24
Условие задачи:
Разработать программу, которая ищет в ASCII-строке заданную подстроку и возвращает список индексов первого символа для всех вхождений подстроки в строке. Подстрока вводится как параметр.
команда для запуска: `gcc main.c -o res && ./res <имя входного файла> <имя выходного файла>`

например:
```
gcc main.c -o res && ./res main.c output.txt && cat output.txt
```
output.txt:
```
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int size = 4096;
int main(int argc, char* argv[]) {
    int fd;
    ssize_t read_bytes;
    char buffer[size];
    if((fd = open(argv[1], O_RDONLY)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    read_bytes = read(fd, buffer, size);

    if (read_bytes == -1) {
        printf("Can\'t read this file\n");
    }
    if (close(fd) < 0) {
        printf("Can\'t close this file\n");
    }
    if((fd = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }
    write(fd, buffer, read_bytes);
    if (close(fd) < 0) {
        printf("Can\'t close this file\n");
    }
    return 0;
}
```
