#include<stdio.h>
int main(){
    int Luachon;
    do{
        printf("***********test menu*************\n");
        printf("**1. **\n");
        printf("**2. **\n");
        printf("**3. **\n");
        printf("**4. **\n");
        printf("**5. **\n");
        printf("**6. **\n");
        printf("\nLua chon cua ban la : ");
        scanf("%d",&Luachon);
        if(Luachon == 1){}
        if(Luachon == 2){}
        if(Luachon == 3){}
        if(Luachon == 4){}
        if(Luachon == 5){}
        if(Luachon == 6){
            printf("Tam biet\n");
        }
    }while(Luachon != 6);
    return 0;
}