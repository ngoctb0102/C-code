#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#define INF 11111110
typedef struct {
    JRB edges;
    JRB vertices;
    int count;
}Graph;

typedef struct str{
    char n[100];
}str;

Graph createGraph(){ //tao graph
    Graph g;
    g.edges = make_jrb(); //cay chua canh
    g.vertices = make_jrb(); //cay dinh
    g.count = 0;
    return g;
}
int getID(Graph g,char a[])
{ JRB node;
  jrb_traverse(node,g.vertices)
  {
    if(strcasecmp(jval_s(node->val),a)==0)
      return jval_i(node->key);
  }
     
     return -1;
}
int makeID(Graph g,char a[])
{
    if(getID(g,a)==-1)
        return g.count;
      else
        return -1;

}
void addVertex(Graph g,int stt, char* name){
    JRB node = jrb_find_int(g.vertices,stt);
    if(node==NULL) //chi them dinh moi
        jrb_insert_int(g.vertices,stt,new_jval_s(strdup(name))); 
}
void addEdge(Graph graph, int v1, int v2, char* name){
    JRB node, tree;
    if (!hasEdge(graph,v1,v2)){
        node = jrb_find_int(graph.edges, v1);//dua node den v1  
        if(node == NULL){
            tree = make_jrb();
            jrb_insert_int(graph.edges,v1,new_jval_v(tree));//neu chua co thi tao cay
        }
        else{
            tree = (JRB) jval_v(node->val);
        }
        
        jrb_insert_int(tree,v2, new_jval_s(strdup(name))); //dua vao cay voi khoa name2 - tuyen name
        jrb_insert_int(tree,v2, new_jval_i(1)); //dua vao cay voi khoa name2 - trong so
    }
}
int hasEdge(Graph graph, int v1, int v2){
    JRB node, tree;
    node = jrb_find_int(graph.edges,v1);//dua node den name1
    if(node==NULL)
        return 0;// ko ton tai return 0
    tree = (JRB) jval_v(node->val);//neu co name1 dua tree den cac con cua name 1 check
    if(jrb_find_int(tree,v2) == NULL){
        return 0;// khong thay return 0
    }
    else return 1;
}
int indegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total = 0;   
    jrb_traverse(node, graph.edges)
    {
       tree = (JRB) jval_v(node->val);
       if (jrb_find_int(tree, v))
       {
          output[total] = jval_i(node->key);
          total++;
       }                
    }
    return total;   
}
int outdegree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;  
}
void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}
char *getVertex(Graph g, int v)
{
     JRB node = jrb_find_int(g.vertices, v);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}
char *getEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return NULL;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return NULL;
    else
       return jval_s(node->val);
}
int getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INF;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INF;
    else
       return jval_i(node->val);    
}
int shortestPath(Graph g, int s, int t, int* path, int*length)
{
   int distance[1000], min, w, total;//kc - min -... - tong
   int previous[1000], tmp[1000];// trc - 
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INF;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist(); //tao ds
   dll_append(queue, new_jval_i(s));//dua start vao ds

   while ( !dll_empty(queue) )// chay den khi hang cho uu tien trong
   {  
      min = INF;   //dua min = INF
      dll_traverse(ptr, queue) //duyet hang doi ptr va que
      {
          u = jval_i(ptr->val);    //u = gia tri ptr          
          if (min > distance[u])// neu min > distance
          {
             min = distance[u];
             node = ptr;
          }                 
      }
      dll_delete_node(node);
      
      if (u == t) break; // stop at t
      
      n = outdegree(g, u, output);
      for (i=0; i<n; i++)
      {
          v = output[i];
          w = getEdgeValue(g, u, v);
          if ( distance[v] > distance[u] + w )
          {    
              distance[v] = distance[u] + w;
              previous[v] = u;
          }     
          dll_append(queue, new_jval_i(v));
      }
   }
   total = distance[t]; 
   if (total != INF)
   {
       tmp[0] = t;
       n = 1;              
       while (t != s)
       {
             t = previous[t];
             tmp[n++] = t;
       }
       for (i = n-1; i>=0; i--)
           path[n-i-1] = tmp[i];
       *length = n;                
   }
   return total; 
}
void bufffile(Graph g)
{  FILE *f;  
  f = fopen("hanoi-bus.txt","r+");
    if(f == NULL){
        printf("Loi mo file.!!!");
        return ;
    }
    char tuyen[100];
    char name[200];
    char name1[200];
    char c;
    int t,n;
    while(!feof(f)){
        // t = 0;
        // do{
        //     c = fgetc(f);
        // }while (!isdigit(c));
        // do{
        //     tuyen[t++] = c;
        //     c = fgetc(f);
        // }while (c != ':');
        fscanf(f, "%[^:]", tuyen);
        fscanf(f, "%*[:]");
       
        //tuyen[t] = '\0';
        //printf("%s ",tuyen);
        n = 0;
        fgetc(f);
        c = fgetc(f);
        do{
            name[n++] = c;
            c = fgetc(f);
        }while (c != '>' && c != '\n' && c != '}');
        name[n - 1] = '\0';
        //printf("%s ",name);
        if(makeID(g,name)!=-1)
        { 
          addVertex(g,makeID(g,name),name);
          g.count++;
        }
        do{
            n = 0;
            fgetc(f);
            c = fgetc(f);
            do{
                name1[n++] = c;
                c = fgetc(f);
            }while (c != '>' && c != '\n' && c != '}');
            if(c != '}' && c != '\n') name1[n - 1] = 0;
            else name1[n] = 0;
            
            
            if(makeID(g,name1)!=-1)
              { 
                addVertex(g,makeID(g,name1),name1);
                g.count++;
              }
            //   printf("%s - %s\n",name,name1);
            addEdge(g,getID(g,name),getID(g,name1),tuyen);
            
            strcpy(name,name1);
            if(c == '}') break;
        }while(c != '\n');
        if(c == '}') break;
    }
    fclose(f);
}
/*void DFS(Graph graph, int start, int stop, void (*func)(int)){
    int visited[10000] = {};
    int n, output[10000], i, u, v;
    Dllist node,stack;
    stack = new_dllist();
    dll_append(stack, new_jval_i(start));
    while(!dll_empty(stack)){
        node = dll_last(stack);
        u = jval_i(node->val);
        dll_delete_node(node);
        if(!visited[u]){
            func(u);
            visited[u] = 1;
            if(u == stop) return;
            n = outdegree(graph, u, output);
            for(i=0;i<n;i++){
                v = output[i];
                if(!visited[v]) dll_append(stack,new_jval_i(v));
            }
        }
    }
}*/
void print(Graph g,int v,int k){
    /*if(getEdge(g,v,k) != NULL){
        printf("tuyen %s : %s to %s\n",getEdge(g,v,k),getVertex(g,v),getVertex(g,k));
    }
    else{*/
        printf("%s\n",getVertex(g,k));
    // }
}
int trace(int s,int t,int back[],Graph g,int visit[],int j,int *c){
    
    if(s == t){
        printf("%s\n",getVertex(g, t));
       
        j++;
    }
    else{
        if(back[t] == -1 && visit[t] == 1){
            printf("");
        }
        else{
            visit[t] = 1;
            j += trace(s,back[t],back,g,visit,j,c);
            if(getEdge(g,back[t],t) == NULL){
                printf("%s\n",getVertex(g, t));
                j++;
            }
            else{
                printf("tuyen %s di den %s\n",getEdge(g,back[t],t),getVertex(g, t));
                if(getEdge(g,back[back[t]],back[t]) != NULL && getEdge(g,back[t],t) != NULL){
                    
                if(strcmp(getEdge(g,back[back[t]],back[t]),getEdge(g,back[t],t)) != 0){
                    
                    (*c)++;
                }}
                j++;
            }
            
        }
    }
    return j;
}
int change(int s,int t,int back[],Graph g,int visit[],int j){}
void BFS(Graph graph, int start, int stop, void (*func)(Graph,int,int))
{
   int visited[100000] = {};
   int n, output[100], i, u, v,x;
   int back[100000];
   int visit[100000];
   Dllist node, queue;
   //x = -1;
   queue = new_dllist();
   dll_append(queue, new_jval_i(start));
   for(x = 0;x<100000;x++){
       back[x] = -1;
       visit[x] = 0;
   }

   while ( !dll_empty(queue) )
   {
      node = dll_first(queue);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          
          //func(graph,x,u);
          visited[u] = 1;
          if ( u == stop ) {
              int j =0;
              int c =0;
              j = trace(start,stop,back,graph,visit,j,&c);
              printf("total distance : %d\n",j);
              printf("Chuyen tuyen %d lan\n",c);
              return ;
          }           
          n = outdegree(graph, u, output);
          int count = 0;
          for (i=0; i<n; i++)
          {
              v = output[i];
              
              if (!visited[v]){
                if(getEdge(graph,back[v],back[back[v]]) == NULL || getEdge(graph,v,back[v]) == NULL){
                    back[v] = u;
                }
                else{
                    if(strcmp(getEdge(graph,back[v],back[back[v]]),getEdge(graph,v,back[v])) == 0){
                      back[v] = u;
                      count++;
                  }
                    else if(count == 0){
                        back[v] = u;
                    }
                }
                 dll_append(queue, new_jval_i(v));
                 }
          }
          //x = u;
      }
      
   }   
                            
}


int main(){
    Graph g=createGraph();
    int i,w, length, path[1000];
    char bus1[300],bus2[300],t[100];
    int id1,id2;
    bufffile(g);
    int output[10];
    JRB node;
    printf("Nhap vao 2 tram can tim:\n");
    do{
    printf("Tram thu 1:");
    gets(bus1);
    id1=getID(g,bus1);
    if(id1 == -1){
        printf("\n khong tim thay tuyen \n");
    }
    }while(id1==-1);
    do{
    printf("Tram thu 2:");
    gets(bus2);
    id2=getID(g,bus2);
    if(id2 == -1){
        printf("\n khong tim thay tuyen \n");
    }
    }while(id2==-1);
    printf("%d %d\n",id1,id2);
    
    //int m=outdegree(g,id2,output);
    //for(i=0;i<m;i++)
    //printf("%d\n",output[i] );    
    /*w = shortestPath(g, id1,id2, path, &length);
    if (w == INF)
    {
         printf("No path from %s to %s\n", getVertex(g,id1), getVertex(g,id2));
    }else
    {     
         printf("Path from %s to %s (with total distance %d)\n", getVertex(g,id1), getVertex(g,id2), w);
         printf("len chuyen %s :",getEdge(g, path[0],path[1]));
         strcpy(t,getEdge(g, path[0],path[1]));
         for (i=0; i<length; i++)
            { 
                printf("%s ", getVertex(g, path[i]));
                if(getEdge(g, path[i],path[i+1])!=NULL){
                if(strcmp(t,getEdge(g, path[i],path[i+1])) != 0){
                    strcpy(t,getEdge(g, path[i],path[i+1]));
                    printf("chuyen tuyen %s ... ",t);
                }
                }
                  
           }printf("\n");

    }*/
    //DFS(g,id1,id2,print);
    BFS(g,id1,id2,print);
    dropGraph(g);
    
    return 0;
}
