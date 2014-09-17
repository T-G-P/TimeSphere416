#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>

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
    char cwd[1024];
    unsigned long long start, end, cycles;
    start = rdtsc();
    getcwd(cwd,sizeof(cwd));
    end = rdtsc();
    cycles = end - start;
    printf("cycles in getcwd(): %llu\n",cycles);
    return cycles;
}

//figure out how to get cpu speed
void simple_time(){
    int i;
    int j;
    int k;
    float sum1,sum2,sum3;
    //float cpu_speed = getcpu_speed()*1000000;
    float cpu_speed = getcpu_speed()*1000000;
    printf("The cpu speed in hz: %.3f\n",cpu_speed);

    for(i = 0; i  < 10; i++){
        sum1 += call_gettimeofday();
    }
    printf("micro seconds for gettimeofday(): %f\n",((sum1/10.0)/(.000001*cpu_speed)));

    for(j = 0; j  < 10; j++){
        sum2 += call_getpid();
    }
    printf("micro seconds for get_pid(): %f\n",((sum2/10.0)/(.000001*cpu_speed)));

    for(k = 0; k  < 10; k++){
        sum3 += call_getcwd();
    }
    printf("micro seconds for get_cwd(): %f\n",((sum3/10.0)/(.000001*cpu_speed)));
}

void main() {

    simple_time();
    //read_time();
    //mmap_time();
}
