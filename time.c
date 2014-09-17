#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

const char *a[3];

const char &test(){
    a[0] = "Faiq fuck off\n";
    a[1] = "Faiq fuck off dix\n";
    a[2] = "Faiq fuck off dix nigz\n";
    int i;
    //for(i=0; i<3; i++){
    //    printf(a[i]);
    //}

    return &a;

}

int simple_time(){

    unsigned long long s = 0;
    unsigned long long e = 0;
    //__asm__ __volatile__("rdtsc" : "=A" (start));
    ///__asm__ __volatile__("rdtsc" : "=A" (end));

    //printf("%lld\n",end-start);
    struct timeval tv;
    //__asm__ __volatile__("rdtsc" : "=A" (s));

    int i = 0;
    unsigned long long a, b, c, sum;

    //for(i = 0; i< 1; i++){
    __asm__ __volatile__("cpuid");
    //__asm__ __volatile__("rdtsc" : "=r" (a): :"eax","edx");
    __asm__ __volatile__("rdtsc" : "=r" (a): :);
    //__asm__ __volatile__("rdtsc" : "=A" (a));
    gettimeofday(&tv,NULL);
    __asm__ __volatile__("cpuid");
    //__asm__ __volatile__("rdtsc" : "=r" (b): :"eax","edx");
    __asm__ __volatile__("rdtsc" : "=r" (b): :);
    c = b - a;
    sum = sum + c;
    //}

    //printf("sum = %d\naverage cycles: %d\naverage time: %.2f seconds\n ", sum, (sum/1.0),((sum/10.0)/3590985000));
    //__asm__ __volatile__("rdtsc" : "=A" (e));
    //printf("%lld\n",e-s);
    //printf("start %llu\n", a);
    //printf("stop %llu\n", b);
    //printf("c is %llu\n ", c);
    printf("cycles: %llu\n",c);
    return c;
}

void main() {

    const char* b = test();
    int j;
    for(j=0; j<3; j++){
        printf(&b[j]);
    }
    int i;
    float sum;

    for(i = 0; i  < 10; i++){
        sum += simple_time();
    }
    printf("micro seconds: %f\n",((sum/10.0)/(.000001*3590985000.0)));
    //read_time();
    //mmap_time();
}
