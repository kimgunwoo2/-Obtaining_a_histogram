#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void history_init(int *);
void standardDeviation(float*,float,int ,int);

int main(int argc, char * argv[]){
    char name[10] = "data";
    int file,i,j;
    int k=0;
    int histogram[256] = {0,};
    int hisSum[256] = {0,};
    int his_num[0];
    int num_start,num_end;
    struct timeval start,end;
    int res;
    float *ttvalue;
    float sumvalue=0,tvalue;
  
    if(argc != 3){
        printf("usage:hw1 A B ((1<=(A and B)<=128) && (A < B) \n ");
        return -1;
    }
    num_start=atoi(argv[1]);
    num_end=atoi(argv[2]);
    if((1>num_start)||(num_end>128)||(num_start>num_end)){
     printf("usage:hw1 A B ((1<=(A and B)<=128) && (A < B) \n ");
        return -1;
    }
    ttvalue=(float *)malloc(sizeof(float)*(num_end-num_start));
    history_init(hisSum);
    for(i = num_start;i<=num_end;i++){
    tvalue=0;
        res=gettimeofday(&start,NULL);
    assert(res==0);
    sprintf(name,"data%d.bin",i);
        for(j = 0; j <256; j++){
            histogram[j] = 0;
        }
        file = open(name,O_RDONLY);
            while(read(file,his_num,1)>0){
                    histogram[his_num[0]]++;
             }
             close(file);
 
         file = open("histogram.bin",O_RDWR);
             read(file,hisSum,1024);
 
        
            for(j = 0; j <256; j++){
                 hisSum[j] = hisSum[j]+histogram[j];
             }
            lseek(file,(off_t)0,SEEK_SET);
 
            write(file,hisSum,1024);
      
            close(file);
        res=gettimeofday(&end,NULL);
        assert(res==0);
        tvalue=end.tv_sec-start.tv_sec+(end.tv_usec-start.tv_usec)/1000000.0;
	printf("%d file time is %f msc.\n",i,tvalue*1000);
        ttvalue[k++]=tvalue;
        sumvalue=sumvalue+tvalue;
            }
               standardDeviation(ttvalue,sumvalue,num_start,num_end);
}
void history_init(int *hisSum){
        int file;
         file = open("histogram.bin",O_RDWR);
            lseek(file,(off_t)0,SEEK_SET);
            write(file,hisSum,1024);
            close(file);
}

void standardDeviation(float *tvalue,float sumvalue,int start,int end){
    float averagevalue;
    float avrsumvalue=0;
    float x=1;
    averagevalue=sumvalue/(end-start+1);
       printf("total time is %f msec.\n",sumvalue*1000);
    printf("average time is %f msec.\n",averagevalue*1000);
    for(int i=0;i<(end-start+1); i++ ){
            tvalue[i]=(tvalue[i]-averagevalue)*1000;
            tvalue[i]=tvalue[i]*tvalue[i];
            avrsumvalue+=tvalue[i];
}      avrsumvalue=avrsumvalue/(end-start+1);
    while(1){
        x=(x+(avrsumvalue/x))/2;
    if(avrsumvalue-0.000001<x*x&&x*x<avrsumvalue+0.000001)
        break;
}
    printf("standard deviation time is %f msec.\n",x);
}
   

 