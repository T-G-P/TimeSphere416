#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

float getcpu_speed(){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    const char* delim = ":\t";
    char * token = NULL;

    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL){
        return 0;
    }

    //Tokenizing each line until the cpu speed is found. Once the cpu speed is found, it gets returned
    while ((read = getline(&line, &len, fp)) != -1) {
        token = strtok(line, delim);
        if(strcmp(line, "cpu MHz") == 0){
            token = strtok(NULL," \t:");
            return atof(token);
        }
        else{
            token = strtok(NULL,delim);
        }
    }

    if (line){
        free(line);
        return 0;
    }
}

inline unsigned long long rdtsc(){
    unsigned long long cycle;
    __asm__ __volatile__("cpuid");
    __asm__ __volatile__("rdtsc" : "=r" (cycle): :);
    return cycle;
}

unsigned long long call_gettimeofday(){
    struct timeval tv;
    unsigned long long start, end, cycles;
    start = rdtsc();
    gettimeofday(&tv,NULL);
    end = rdtsc();
    cycles = end - start;
    printf("cycles in gettimeofday(): %llu\n",cycles);
    return cycles;
}

unsigned long long call_getpid(){
    unsigned long long start, end, cycles;
    pid_t pid = getpid();
    start = rdtsc();
    end = rdtsc();
    cycles = end - start;
    printf("cycles in getpid(): %llu\n",cycles);
    return cycles;
}

unsigned long long call_getcwd(){
    char cwd[5024];
    unsigned long long start, end, cycles;
    start = rdtsc();
    getcwd(cwd,sizeof(cwd));
    end = rdtsc();
    cycles = end - start;
    printf("cycles in getcwd(): %llu\n",cycles);
    return cycles;
}

unsigned long long *call_mmap(){
    unsigned long long start,end,cycles,start2,end2,cycles2;
    unsigned long long *result = (unsigned long long*)malloc(sizeof(unsigned long long) * 2);
    struct stat sb;
    off_t len;
    char * mapped_region;
    char * memcpy_dest;
    int i,j,fd,size;

    fd = open ("simple_time.log", O_RDONLY);
    fstat(fd, &sb);

    start = rdtsc();
    mapped_region = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    memcpy_dest = (char *) malloc(sizeof(sb.st_size));
    end = rdtsc();
    cycles2 = end - start;
    printf("cycles in mmap(): %llu\n",cycles);
    result[0] = cycles;

    start2 = rdtsc();
    memcpy(memcpy_dest, mapped_region, sb.st_size);
    end2 = rdtsc();
    cycles2 = end2 - start2;
    printf("cycles in memcpy(): %llu\n",cycles);
    result[1] = cycles2;

    close (fd);
    return result;
}

void simple_time(){
    float cpu_speed = getcpu_speed()*1000000;
    int i;
    int j;
    int k;
    float sum1,sum2,sum3;

    for(i = 0; i  < 50; i++){
        sum1 += call_gettimeofday();
    }
    printf("micro seconds for gettimeofday(): %f\n",((sum1/50.0)/(.000001*cpu_speed)));

    for(j = 0; j  < 50; j++){
        sum2 += call_getpid();
    }
    printf("micro seconds for get_pid(): %f\n",((sum2/50.0)/(.000001*cpu_speed)));

    for(k = 0; k  < 50; k++){
        sum3 += call_getcwd();
    }
    printf("micro seconds for get_cwd(): %f\n",((sum3/50.0)/(.000001*cpu_speed)));
}

void mmap_time(){
    int i,j;
    float sum1,sum2;
    unsigned long long* cycles;
    float cpu_speed = getcpu_speed()*1000000;

    for(i = 0; i < 50; i++){
        cycles = call_mmap();
        printf(" mmap cycles: %llu",cycles[0]);
        sum1 += cycles[0];
    }
    printf("micro seconds for mmap(): %f\n",((sum1/50.0)/(.000001*cpu_speed)));

    for(j = 0; j < 50; j++){
        cycles = call_mmap();
        sum2 += cycles[1];
    }
    printf("micro seconds for memcpy(): %f\n",((sum2/50.0)/(.000001*cpu_speed)));
}

void main() {
    float cpu_speed = getcpu_speed()*1000000;
    printf("\nThe cpu speed in Mhz: %.3f\n\n",getcpu_speed());
    printf("The cpu speed in hz: %.3f\n\n",cpu_speed);

    simple_time();
    //read_time();
    mmap_time();
}
