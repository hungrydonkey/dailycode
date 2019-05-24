#include<stdio.h>
#include<malloc.h>

#define infi  1000
//the edge of graph
typedef struct {
	int qid;
	int zod;
	int quan;
}row;

// define the list to store nodes
typedef struct yao{
    row data;
    struct yao * listnode;
    struct yao *front; // Pointing to the nearby previous summits in nx1
}list;

//edge in the  adjacency list
typedef struct Node{
	int dest;
	int cost;
	struct Node *next;
}edge;

//define adjacency list
typedef struct {
	int data;
	int source;
	edge *link;
}adj;

//define graph
typedef struct {
	adj a[10];
	int numvertex;
	int numedge;
}Graph;

//initialization of graph
void initgraph(Graph *G){
	G ->numvertex = 0;
	G ->numedge = 0;
	for(int i = 0;i < 10;i++){
		G ->a[i].source = i;
		G ->a[i].link = NULL;
	}
}

//insert data of nodes
void invert(Graph * G,int i,int n){
	if(i >= 0 && i < 6 ){
		G ->a[i].data = n;
		G ->numvertex ++;
	}
	else printf("Graph >= 5");
}

//insert edge of nodes
void inedge(Graph * G,int v1,int v2,int n){
	edge *p;
	if(v1 < 0 || v1 >= G ->numvertex || v2 < 0 || v2 >= G -> numvertex){
		printf("cross the edge!");
		return	;
	}
	p = (edge*)malloc(sizeof(edge)) ;
	p ->dest = v2;
	p ->cost = n;
	p ->next = G ->a[v1].link;
	G ->a[v1].link = p;
	G ->numedge ++;
}

//build the graph
void build(Graph *G,int v[],int n,row d[],int e){
	initgraph(G);
	for(int i = 0 ; i < n;i++)
		invert(G,i,v[i]);
	for(int i = 0;i < e;i++)
		inedge(G,d[i].qid,d[i].zod,d[i].quan);
}

// export the path of shortest node
void minpath(list *p){
    if(p ->data.quan == 0){
        printf(" %d ",p ->data.zod);
        return;
    }
     if(p ->front != NULL){
        minpath(p ->front);
        printf(" %d ",p ->data.zod);
   }
   else printf("no path");
}

// Output length of the shortest path to the source point
void put(list *a){
    if(a == NULL)
        return;
    put(a ->listnode);
    if(a ->data.quan != infi)
    printf("<%d,%d> = %d      ",a ->data.qid-1,a ->data.zod-1,a ->data.quan );
    else printf("<%d,%d> =  infinite    ",a ->data.qid,a ->data.zod);
    minpath(a);
    printf("\n\n");
}

//free the space of nx1
void freenode(list *a){
    if(a == NULL)
        return;
    freenode(a ->listnode);
    free(a);
}

//algorithm of dijkstra
void dijk(int x ,Graph *o){
	int a = 0,b = 0,i = 0,p;
	list nx,nx1,*freenx[10],*st,*r;
	list *z;
	edge *l;
    nx.listnode = NULL;
    nx1.listnode = NULL;

    //initialize D(x)
	for(int i = 0;i < 6;i++ ){
        z = (list*)malloc(sizeof(list)) ;
		z ->data.qid = x;
		z  ->data.zod = o ->a[i].data ;
		z ->data.quan = infi;
		z ->listnode = NULL;
		z ->front      =NULL;
		//nx.listnode = z;
		if(x == o ->a[i].data ){
            p = i ;
            z ->data.quan = 0;
        }

        z ->listnode = nx.listnode;
        nx.listnode = z;
	}
	st = nx.listnode;

	//start
	while(1){
        st = &nx;
        r = st ->listnode;

        //find min cost of current  nodes
        while(st ->listnode != NULL){
            if(r ->data.quan > st ->listnode ->data.quan)
                r = st ->listnode;
            st = st ->listnode;
        }

        //add r to nx1
        z = (list*)malloc(sizeof(list)) ;
        z ->data.qid  = r ->data.qid;
        z ->data.zod  = r ->data.zod;
        z ->data.quan = r ->data.quan;
        z ->front     = r ->front;
        z ->listnode  = NULL;
        z ->listnode  = nx1.listnode;
        nx1.listnode  = z;

        //put(nx1.listnode);
        freenx[i] = r;
        i++;

        //delete the min node in nx;
        st = &nx;
        while(st ->listnode != NULL){
            if(st ->listnode ->data.zod == r ->data.zod)
                st ->listnode = r ->listnode;
            if(st ->listnode != NULL)
                st = st ->listnode;
        }

        //put(nx.listnode);



        // N = N' break
        if(nx.listnode == NULL )
            break;

        //update D(x)
        //find the shortest distant node
        for(int i = 0;i < 6 ;i++)
            if(o ->a[i].data == r ->data.zod)
                    p = i;

        //locate the neighbor  of x
        //printf("%d ",p);
        l = o ->a[p].link;

        //st = nx.listnode;
        while(l != NULL){
            st = nx.listnode;
          //printf("%d ",l ->dest);
            while(st != NULL){
                    if(o ->a[l ->dest].data == st ->data.zod){
                        //printf("%d ",st ->data.zod);
                        if(st ->data.quan > r ->data.quan + l ->cost){
                            st ->data.quan = r ->data.quan + l ->cost;
                           //st = st ->listnode;
                            st ->front = r;
                            //printf("%d",st ->front ->data.zod);
                        }
                    }
                    st = st ->listnode;
            }
            l = l ->next;
        }
	}
     put(nx1.listnode);
     for(int j = 0;j < i ;j++){
        //printf(" %d ",freenx[j] ->data.zod);
        free(freenx[j]);
     }
     freenode(nx1.listnode);
}

//algorithm of floyed
void floyed(void){
    row fedge[6][6] ;
    int a[6][6] = {0,20,4,34,1000,1000,20,0,1000,8,1000,25,4,1000,0,1000,14,1000,34,8,1000,0,12,10,1000 ,1000, 14, 12, 0, 30, 1000, 25, 1000, 10, 30, 0};
    for(int i = 0; i < 6;i++){
        for(int j = 0;j < 6;j++){
            fedge[i][j].qid = i + 1;
            fedge[i][j].zod = j + 1;
            fedge[i][j].quan = a[i][j];
        }
	}
	//update path for n
    for(int k = 0;k < 6;k++){
            for(int i = 0; i < 6;i++){
                for(int j = 0;j < 6;j++){
                    if(fedge[i][j].quan > fedge[i][k].quan + fedge[k][j].quan){
                        fedge[i][j].quan = fedge[i][k].quan + fedge[k][j].quan ;//get the shortest path
                    }
                }
            }
    }

    for(int i = 0; i < 6;i++){
        for(int j = 0;j < 6;j++){
            if(fedge[i][j].quan == infi)
                printf("<%d,%d> = infinite ",fedge[i][j].qid,fedge[i][j].zod);
            else
            printf("<%d,%d> = %d     ",fedge[i][j].qid,fedge[i][j].zod,fedge[i][j].quan);
        }
        printf("\n\n");
	}
}

int main(void){
	int a[6] = {1,2,3,4,5,6};
	int b;
	Graph g;
	row bian[12] = {{0,1,1},{0,2,20},{1,0,1},{1,2,1},{2,0,20},{2,1,1},{1,4,1},{4,1,1},{2,3,1},{3,2,1},{4,5,1},{5,4,1}};

	build(&g,a,6,bian,12);
	printf("print a node of graph (1-6):\n");
	scanf("%d",&b);
	printf("the result of dijkstra:\n");
	dijk(b,&g);
    printf("the result of floyed:\n");
    floyed();

	return 0;
}
