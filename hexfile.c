#include <stdio.h>
#include <stdlib.h>
void dump(char *file){
    FILE *p = fopen(file,"r");
    if(p == NULL){
        perror("error");
        exit(EXIT_FAILURE);
    }
    int i = 0,n = 0,a[16] = {};
    while(( a[n] = fgetc(p)) != EOF){
        if(i % 0x10 == 0)
            printf("%08x:",i);
        if(i%2 == 0)
            printf(" ");
        printf("%02x",a[n]);
        i++;
        n++;
        if(i % 0x10 == 0){
            printf("  ");
            for(int i = 0;i < 16;i++){
                if(a[i] >= 32 && a[i] <= 126)
                    printf("%c",a[i]);
                else 
                    printf(".");
            }
            puts("");
            n = 0;
        }
    }
    if((n-1)%2 == 1)
        printf(" ");
    for(int i = n;i < 16;i++){
        printf("  ");
        if(i%2 == 1)
            printf(" ");
    }
    printf(" ");
    for(int i = 0;i < n;i++){
        if(a[i] >= 32 && a[i] <= 126)
            printf("%c",a[i]);
        else 
            printf(".");
    }
    printf("\n");
}
int main(int argc,char *argv[]){
    switch(argc){
        case 1: puts("need a file!"); return 0;
        case 2: dump(argv[1]); return 0;
        default : puts("only one file can be dumped!"); return 0;
    }
}