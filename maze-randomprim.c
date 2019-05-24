#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<stdlib.h>

typedef struct
{
    char dir;
    int path;
    int xdest;
    int ydest;
}mazeunit;

typedef struct{
    mazeunit list[1071];
    int count;
}nels;

void inis(mazeunit (*a)[51])
{
    for(int i = 0;i < 21;i++){
        for(int j = 0;j < 51; j++){
            (*a)[j].path = 1;//1 is wall,0 is path
            (*a)[j].xdest = j+1;
            (*a)[j].ydest = i+1;
            (*a)[j].dir = 'c';
        }
        a++;
    }
}

void show(mazeunit (*a)[51])
{
    for(int i = 0;i < 21;i++){
        for(int j = 0;j < 51;j++){
            if((*a)[j].path == 1)
                printf("%c",219);
            else
                printf("%c",178);
        }
        a++;
        printf("%d",i+1);
        printf("\n");
    }

}

int main(void)
{
    SetConsoleOutputCP(437);
    srand((int)time(NULL));


    mazeunit ma[21][51];
    nels newall;
    int x = 0 ,y = 0,n,w0 = 0,w1 =0;

    newall.count = 0;
    inis(ma);
    //the maze start
    x = 50;
    y = 20;

    ma[y][x].path = 0;
    //push the wall around the path to list
    //up wall
    if(y-1 >= 0){
        newall.list[newall.count] = ma[y-1][x];
        newall.list[newall.count].dir = 'u';
        newall.count++;
    }
    //down wall
    if(y+1 <= 20){
        newall.list[newall.count] = ma[y+1][x];
        newall.list[newall.count].dir = 'd';
        newall.count++;
    }
    //left wall
    if(x-1 >= 0){
        newall.list[newall.count] = ma[y][x-1];
        newall.list[newall.count].dir = 'l';
        newall.count++;
    }
    //right wall
    if(x+1 <= 50){
        newall.list[newall.count] = ma[y][x+1];
        newall.list[newall.count].dir = 'r';
        newall.count++;
    }
    
    while(1){
        if(newall.count == 1)
            n = 0;
        else
            n = rand()%(newall.count-1);
        //printf("%d\n",n);
        //printf("%d %d\n",newall.list[n].xdest,newall.list[n].ydest);
        w0 = newall.list[n].xdest - 1;
        w1 = newall.list[n].ydest - 1;
        switch(newall.list[n].dir)                  //choose the direction
        {
            case 'u':w1 --;break;
            case 'd':w1 ++;break;
            case 'l':w0 --;break;
            case 'r':w0 ++;break;
            default:printf("dest is the error\n");break;
        }
        if(w1 >= 0 && w1 <= 20 && w0 >= 0 && w0 <= 50){
            if(ma[w1][w0].path == 1)                        //the square is wall,let it be path and push the wall around it into list
            {
                ma[newall.list[n].ydest - 1][newall.list[n].xdest - 1].path = 0;
                ma[w1][w0].path = 0;

                for(int i = n;i < newall.count;i++){         //clear the wall in list
                    newall.list[i] = newall.list[i+1];
                }
                newall.count--;
                //printf("%d\n",newall.count);
                if(w1-1 >= 0){
                    if(ma[w1-1][w0].path == 1){
                        newall.list[newall.count] = ma[w1-1][w0];
                        newall.list[newall.count].dir = 'u';
                        newall.count++;
                    }
                }
                //printf("%d\n",newall.count);
                 if(w1+1 <= 20){
                    if(ma[w1+1][w0].path == 1){
                        newall.list[newall.count] = ma[w1+1][w0];
                        newall.list[newall.count].dir = 'd';
                        newall.count++;
                    }
                }
                //printf("%d\n",newall.count);
                if(w0-1 >= 0){
                    if(ma[w1][w0-1].path == 1){
                        newall.list[newall.count] = ma[w1][w0-1];
                        newall.list[newall.count].dir = 'l';
                        newall.count++;
                    }
                }
                //printf("%d\n",newall.count);
                if(w0+1 <= 50){
                    if(ma[w1][w0+1].path == 1){
                        newall.list[newall.count] = ma[w1][w0+1];
                        newall.list[newall.count].dir = 'r';
                        newall.count++;
                    }
                }
                //printf("%d\n",newall.count);
            }
            else if(ma[w1][w0].path == 0)                         //if the square has benn path,the wall will not in list
                {
                    for(int i = n;i < newall.count;i++){
                        newall.list[i] = newall.list[i+1];
                    }
                    newall.count--;
                }
        }
        if(newall.count == 0)
            break;
    }
    show(ma);
    return 0;
}
