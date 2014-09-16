#include<stdio.h>
#include <sys/time.h>

void simple_time(){

    unsigned long long start = 0;
    unsigned long long end = 0;
    unsigned long long s = 0;
    unsigned long long e = 0;
    __asm__ __volatile__("rdtsc" : "=A" (start));
    __asm__ __volatile__("rdtsc" : "=A" (end));

    printf("%lld\n",end-start);
    struct timeval tv;
    __asm__ __volatile__("rdtsc" : "=A" (s));

    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);
    gettimeofday(&tv,NULL);

    __asm__ __volatile__("rdtsc" : "=A" (e));

    printf("%lld\n",e-s);
}

void main() {
    simple_time();
    //read_time();
    //mmap_time();
}




