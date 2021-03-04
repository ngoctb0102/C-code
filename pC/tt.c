#include<stdio.h>
#include<stdlib.h>
#include"jrb.h"
#include"dllist.h"
#define INF 100000000
#define NIL -1
typedef struct {
   JRB edges;
   JRB vertices;
}Graph;
Graph createGraph(){
   Graph g;
   g.edges = make_jrb();
   g.vertices = make_jrb();
   return g;
}
void addVertex(Graph g,int id){
   JRB node = jrb_find_int(g.vertices,id);
   if(node==NULL)
      jrb_insert_int(g.vertices, id, new_jval_i(id));
}
void addEdge(Graph graph, int v1, int v2,int time){
   JRB node,tree;
   if(!hasEdge(graph,v1,v2)){
      node = jrb_find_int(graph.edges,v1);
      if(node==NULL){
         tree = make_jrb();
         jrb_insert_int(graph.edges, v1, new_jval_v(tree));
      }
      else{
         tree = (JRB) jval_v(node->val);
      }
      // jrb_insert_int(tree, v2, new_jval_i(1));
      jrb_insert_int(tree, v2,new_jval_i(time));
   }
   // if(!hasEdge(graph, v2, v1)){
   //    node = jrb_find_int(graph.edges,v2);
   //    if(node==NULL){
   //       tree = make_jrb();
   //       jrb_insert_int(graph.edges, v2, new_jval_v(tree));
   //    }
   //    else{
   //       tree = (JRB) jval_v(node->val);
   //    }
   //    jrb_insert_int(tree,v1,new_jval_i(time));
   // }
}
int hasEdge(Graph graph, int v1, int v2){
   JRB node, tree;
   node = jrb_find_int(graph.edges,v1);
   if (node==NULL) return 0;
   tree = (JRB) jval_v(node->val);
   if(jrb_find_int(tree,v2) == NULL) return 0;
   else return 1;
}
int indegree(Graph graph, int v, int* output){
   JRB node,tree;
   int total = 0;
   jrb_traverse(node,graph.edges){
      tree = (JRB) jval_v(node->val);
      if(jrb_find_int(tree,v)){
         output[total] = jval_i(node->key);
         total++;
      }
   }
   return total;
}
int outdegree (Graph graph, int v, int* output,int *visited){
   JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    { 
       if(!visited[jval_i(node->key)]){   
       output[total] = jval_i(node->key);
       total++;                }
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
// int shortestPath(Graph g, int s, int t, int* path, int*length)
// {
//    int distance[1000], min, w, total;
//    int previous[1000], tmp[1000];
//    int n, output[100], i, u, v, start;
//    int visited[100];
//    int temp[100];
//    int check[100];
//    for(i=0;i<100; i++){
//       check[i] = 0;
//    }
//    Dllist ptr, queue, node;

//    for (i=0; i<1000; i++)
//        distance[i] = INF;
//    distance[s] = 0;
//    previous[s] = s;
       
//    queue = new_dllist();
//    dll_append(queue, new_jval_i(s));

//    while ( !dll_empty(queue) )
//    {
//       // get u from the priority queue   
//       min = INF;   
//       dll_traverse(ptr, queue)
//       {
//           u = jval_i(ptr->val);              
//           if (min > distance[u])
//           {
//              min = distance[u];
//              node = ptr;
//           }                 
//       }
//       dll_delete_node(node);
      
//       //if (u == t) break; // stop at t
//       visited[s] = 1;
//       if(check[u] = 10){
//          visited[u] = 1;
//       }
//       n = outdegree(g, u, output,visited);
//       for (i=0; i<n; i++)
//       {
//           v = output[i];
//           check[v]++;
//           check[u]++;
//           w = getEdgeValue(g, u, v);
//           if ( distance[v] > distance[u] + w )
//           {    
//               distance[v] = distance[u] + w;
//               previous[v] = u;
//           }     
//           dll_append(queue, new_jval_i(v));
//       }
//    }
//    total = distance[t]; 
//    if (total != INF)
//    {
//        tmp[0] = t;
//        n = 1;              
//        while (t != s)
//        {
//              t = previous[t];
//              tmp[n++] = t;
//        }
//        for (i=n-1; i>=0; i--)
//            path[n-i-1] = tmp[i];
//        *length = n;                
//    }
//    for(i=0;i<100;i++){
//       visited[i] = 0;
//    }
//    return total;   
// }
// int extractMin(Dllist queue, int* distance){
//   Dllist node;
//   Dllist result;
//   int min = INF;
//   int index;
//   dll_traverse(node, queue){
//     index = jval_i(node->val);
//     if(distance[index] < min){
//       result = node;
//       min = distance[index];
//     }
//   }
//   int min_id = jval_i(result->val);
//   dll_delete_node(result);
//   return min_id;
// }

// void relax(int u, int v, Graph graph, int* distance, int* parent){
//   int w = getEdgeValue(graph, u, v);
//   if(distance[v] > distance[u] + w){
//     distance[v] = distance[u] + w;
//     parent[v] = u;
//   }
// }
// int getAdjacentVertices(Graph graph, int v1, int output[]){
//   JRB searchNode = jrb_find_int(graph.edges, v1);
//   JRB adjacentList = (JRB)jval_v(searchNode->val);
//   int total = 0;
//   JRB node;
//   jrb_traverse(node, adjacentList)
//     output[total++] = jval_i(node->key);
//   return total;
// }
// int shortestPath(Graph graph, int start, int end, int* path, int* length){
//   //create distance table
//   int distance[1000];
//   int visited[1000];
//   int parent[1000];
//   int i,j;
//   for(i = 0; i<1000; i++){
//     distance[i] = INF;
//     parent[i] = NIL;
//     visited[i] = 0;
//   }
//   distance[start] = 0;
//   parent[start] = start;
//   visited[start] = 1;
//   //create the queue
//   Dllist queue = new_dllist();
//   dll_append(queue, new_jval_i(start));
  
//   int vertex;
//   int output[10];
//   int totalAdjacent;
//   while(!dll_empty(queue)){
//     vertex = extractMin(queue, distance);
//     if(vertex == end) break;
//     totalAdjacent = getAdjacentVertices(graph, vertex, output);
//     for( j = 0; j<totalAdjacent; j++){
//       relax(vertex, output[j], graph, distance, parent);
//       if(visited[output[j]] == 0){
//         dll_append(queue, new_jval_i(output[j]));
//         visited[output[j]] = 1;
//       }
//     }
//   }
//   int total = distance[end];
//   int temp[1000];
//   int k;
//   if(total != INF){
//     temp[0] = end;
//     k = 1;
//     while(end != start){
//       end = parent[end];
//       temp[k++] = end;
//     }
//     for(i = k -1; i>=0; i--)
//       path[k-i-1] = temp[i];
//     *length = k;
//   }
//   return total;
// }
double shortestPath(Graph g, int s, int t, int *path, int *length) {
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000];
    int n, output[100], i, u, v, start;
    Dllist ptr, queue, node;

    for (i = 0; i < 1000; i++) {
        previous[i] = -1;
        distance[i] = INF;
    }
    distance[s] = 0;
    previous[s] = s;

    queue = new_dllist();
    JRB vertice;
    jrb_traverse(vertice, g.vertices) {
        dll_append(queue, vertice->key);
    }

    while (!dll_empty(queue)) {
        // get u from the priority queue
        min = INF;
        dll_traverse(ptr, queue) {
            i = jval_i(ptr->val);
            if (min > distance[i]) {
                min = distance[i];
                node = ptr;
                u = i;
            }
        }
        dll_delete_node(node);

        // if (u == t) break; // stop at t
        int temp[100];
        n = outdegree(g, u, output,temp);
        for (i = 0; i < n; i++) {
            v = output[i];
            w = getEdgeValue(g, u, v);
            // printf("u: %d, v: %d, w: %g\n", u, v, w);
            if (distance[v] > distance[u] + w) {
                // printf("%d yes\n", v);
                distance[v] = distance[u] + w;
                previous[v] = u;
            }
        }
    }
    total = distance[t];
    if (total != INF) {
        tmp[0] = t;
        n = 1;
        while (t != s) {
            t = previous[t];
            tmp[n++] = t;
        }
        for (i = n - 1; i >= 0; i--)
            path[n - i - 1] = tmp[i];
        *length = n;
    }
    return total;
}
int main(){
   Graph g = createGraph();
   int i,w,length,path[1000];
   int id1,id2,time;
   int dinh,canh;
   FILE *f;
   f = fopen("test.txt","r+");
   fscanf(f,"%d %d",&dinh,&canh);
   while(!feof(f)){
      fscanf(f,"%d %d %d",&id1,&id2,&time);
      addVertex(g,id1);
      addVertex(g,id2);
      addEdge(g,id1,id2,time);
      addEdge(g,id2,id1,time);
   }
   fclose(f);
   
   int Luachon;
    do{
        printf("************test menu*************\n");
        printf("**1. In ra ma tran ke           **\n");
        printf("**2. In ra danh sach ke         **\n");
        printf("**3. Dinh di bo va indegree max **\n");
        printf("**4. Tim duong ngan nhat        **\n");
        printf("**5.**\n");
        printf("**6. Thoat                      **\n");
        printf("\nLua chon cua ban la : ");
        scanf("%d",&Luachon);
        if(Luachon == 1){
           int matrix[dinh][dinh];
           int j,k;
           for(j = 0;j<dinh;j++){
              for(k=0;k<dinh;k++){
                 if(hasEdge(g,j+1,k+1) == 1){
                    matrix[j][k] = 1;
                  //   printf("%d , %d , %d\n",j,k,matrix[j][k]);
                 }else{
                    matrix[j][k] = 0;
                  //   printf("%d , %d , %d\n",j,k,matrix[j][k]);
                 }
              }
           }
           printf(" ");
           int h,l,p;
           for(h=0;h<dinh;h++){
              printf(" %d",h+1);
           }
           for(l = 0;l<dinh;l++){
              printf("\n%d",l+1);
              for(p=0;p<dinh;p++){
                 printf(" %d",matrix[l][p]);
              }
        }
        printf("\n");
        }
        if(Luachon == 2){
           int matrix[dinh][dinh];
           int j,k;
           for(j = 0;j<dinh;j++){
              for(k=0;k<dinh;k++){
                 if(hasEdge(g,j+1,k+1) == 1){
                    matrix[j][k] = 1;
                  //   printf("%d , %d , %d\n",j,k,matrix[j][k]);
                 }else{
                    matrix[j][k] = 0;
                  //   printf("%d , %d , %d\n",j,k,matrix[j][k]);
                 }
              }
           }
           int l,p;
           for(l = 0;l<dinh;l++){
              printf("\ncastel %d :",l+1);
              for(p=0;p<dinh;p++){
                 if(matrix[l][p] != 0){
                    printf(" %d",p+1);
                 }
              }
        }
           printf("\n");
        }
        if(Luachon == 3){
           int j,k;
           for(j = 0;j<dinh;j++){
              for(k=0;k<dinh;k++){
                 if(hasEdge(g,j+1,k+1) == 1 && getEdgeValue(g,j+1,k+1) >= 50){
                    printf("%d - %d\n",j+1,k+1);
                 }
              }
           }
           int max;
           int output[100];
           max = indegree(g,1,output);
           for(j=0;j<dinh;j++){
              if(max < indegree(g,j+1,output)){
                 max = indegree(g,j+1,output);
              }
           }
           printf("nhung dinh co indgree max = %d la :",max);
           for(j=0;j<dinh;j++){
              if(indegree(g,j+1,output) == max){
                 printf(" %d",j+1);
              }
           }
           printf("\n");
        }
        if(Luachon == 4){
           int s;
         int t;
         printf("nhap vao dinh xuat phat : ");
         scanf("%d",&s);
         printf("nhap vao dinh dich : ");
         scanf("%d",&t);
         w = shortestPath(g, s, t, path, &length);
         if (w == INF)
         {
            printf("Route not found\n");
         }else
         {
            printf("total distance : %d\n",w);
            for (i=0; i<length; i++)
            printf(" => %d",path[i]);
         }
         printf("\n");
        }
        if(Luachon == 5){
         
        }
        if(Luachon == 6){
            printf("Tam biet\n");
        }
    }while(Luachon != 6);
    dropGraph(g); 
}