//
// Created by Administrator on 2022/9/22.
//

/***
 * Linux下使用backtrace打印函数调用栈信息
 */
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 10

int myfunc1(int);
int myfunc2(int);
int myfunc3(int);

void printCallers()
{
    int layers = 0, i = 0;
    char ** symbols = NULL;

    void * frames[MAX_FRAMES];
    memset(frames, 0, sizeof(frames));
    layers = backtrace(frames, MAX_FRAMES);
    for (i=0; i<layers; i++) {
        printf("Layer %d: %p\n", i, frames[i]);
    }
    printf("------------------\n");

    symbols = backtrace_symbols(frames, layers);
    if (symbols) {
        for (i=0; i<layers; i++) {
            printf("SYMBOL layer %d: %s\n", i, symbols[i]);
        }
        free(symbols);
    }
    else {
        printf("Failed to parse function names\n");
    }
}


int myfunc1(int a)
{
    int b = a + 5;
    int result = myfunc2(b);
    return result;
}

int myfunc2(int b)
{
    int c = b * 2;
    int result = c + myfunc3(c);
    return result;
}

int myfunc3(int c)
{
    int d = c << 2;
    printCallers();
    d = d/0;
    return d;
}

int main()
{
    int result = 0;
    result = myfunc1(1);
    printf("result = %d\n", result);

    return 0;
}
