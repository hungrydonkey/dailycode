void dis(int n,int a[][n]){
    for(int i = 0;i < n;++i){
        for(int j = 0;j < n;++j)
            a[i][j] == 1 ? printf("+ ") : printf("* ");
        puts("");
    }
}

int panduan(int n,int a[][n],int row,int clo){
    for(int i = clo,k = clo;row >= 0 ;--row,--i,++k){
        if(a[row][clo] == 1) return 0;
        if(i >= 0)
            if(a[row][i] == 1) return 0;
        if(k < n)
         if(a[row][k] == 1)  return 0;
    }
    return 1;
}

void findindex(int n,int a[][n],int row){
    if(row == n){
        dis(n,a);
        puts("");
        return ;
    }
    for(int i = 0;i < n;++i)
        if(panduan(n,a,row,i)){
            a[row][i] = 1;
            findindex(n,a,row+1);
            a[row][i] = 0;
        }
}

int main(){
    int n;
    puts("print n:");
    scanf("%d",&n);
    int a[n][n];
    for(int i = 0;i < n;++i)
        for(int j = 0;j < n;++j)
            a[i][j] = 0;
    findindex(n,a,0);
}
