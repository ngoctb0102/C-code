#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *f;
    f = fopen("test.txt","r+");
    int dinh,canh;
    int id1,id2,time;
    fscanf(f,"%d %d",&dinh,&canh);
    printf("%d %d\n",dinh,canh);
    while(!feof(f)){
        fscanf(f,"%d %d %d",&id1,&id2,&time);
        printf("%d %d %d\n",id1,id2,time);
    }
    fclose(f);
    return 0;
}