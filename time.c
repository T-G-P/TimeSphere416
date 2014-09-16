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

    int i = 0;
    unsigned long long a, b, c, sum;

    for(i = 0; i< 3; i++){
        __asm__ __volatile__("cpuid");
        __asm__ __volatile__("rdtsc" : "=r" (a): :"eax","edx");
        gettimeofday(&tv,NULL);
        __asm__ __volatile__("cpuid");
        __asm__ __volatile__("rdtsc" : "=r" (b): :"eax","edx");
        c = b - a;
        sum = sum + c;
    }

    printf("sum = %d\naverage cycles: %d\naverage time: %.2f seconds\n ", sum, (sum/15),((sum/3.0)/3590985000));
    //__asm__ __volatile__("rdtsc" : "=A" (e));
    //printf("%lld\n",e-s);
}

void main() {
    simple_time();
    //read_time();
    //mmap_time();
}
