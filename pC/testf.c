#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jrb.h"
#include"dllist.h"
#define INF 100000000
#define NIL -1
typedef struct {
   JRB edges;
   JRB vertices;
   int rank;
}Graph;
Graph createGraph(){
   Graph g;
   g.edges = make_jrb();
   g.vertices = make_jrb();

   return g;
}
void addVertex(Graph g,int id,char *URL){
   JRB node = jrb_find_int(g.vertices,id);
   if(node==NULL)
    //   jrb_insert_int(g.vertices, id, new_jval_i(id));
      jrb_insert_int(g.vertices,id, new_jval_s(strdup(URL)));
}
void addEdge(Graph graph, int v1, int v2){
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
      jrb_insert_int(tree, v2, new_jval_i(1));
    //   jrb_insert_int(tree, v2,new_jval_i(time));
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
int outdegree (Graph graph, int v, int* output){
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
int hasV(Graph graph, int id){
    JRB node, tree;
    node = jrb_find_int(graph.edges,id);//dua node den name1
    if(node==NULL)
        return 0;
    else return 1;
}
double shortestPath(Graph g, int s, int t, int *path, int *length) {
    double distance[1000], min, w, total;
    int previous[1000], tmp[1000];
    int n, output[100], i, u, v, start;
    Dllist ptr, queue, node;

    for (i = 0; i < 1000; i++) {
        previous[i] = -1;
        distance[i] = 100;
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
        min = 100;
        dll_traverse(ptr, queue) {
            i = jval_i(ptr->val);
            if (min > distance[i]) {
                min = distance[i];
                node = ptr;
                u = i;
            }
        }
        dll_delete_node(node);

        if (u == t) break; // stop at t
        int temp[100];
        n = outdegree(g, u, output);
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
    if (total != 100) {
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
char *getVertex(Graph g, int v)
{
     JRB node = jrb_find_int(g.vertices, v);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}
// void BFS(Graph graph, int start, int stop, void (*func)(int))
// {
//    int visited[1000] = {};
//    int n, output[100], i, u, v;
//    Dllist node, queue;
   
//    queue = new_dllist();
//    dll_append(queue, new_jval_i(start));

//    while ( !dll_empty(queue) )
//    {
//       node = dll_first(queue);
//       u = jval_i(node->val);
//       dll_delete_node(node);
//       if (!visited[u]) 
//       {
//           func(u);
//           visited[u] = 1;
//           if ( u == stop ) return;           
//           n = getAdjacentVertices(graph, u, output);
//           for (i=0; i<n; i++)
//           {
//               v = output[i];
//               if (!visited[v])
//                  dll_append(queue, new_jval_i(v));
//           }
//       }
//    }                            
// }
// int getAdjacentVertices (Graph graph, int v, int* output)
// {
//     JRB tree, node;
//     int total;
//     node = jrb_find_int(graph.vertices, v);
//     if (node==NULL)
//        return 0;
//     tree = (JRB) jval_v(node->val);
//     total = 0;   
//     jrb_traverse(node, tree)
//     {
//        output[total] = jval_i(node->key);
//        total++;                
//     }
//     return total;   
// }
// void printVertex(int v) // in 
// {
//      printf("%4d", v);
// }
int main(){
    int Luachon;
    int n1,n2;
    int ip[100];
    int c = 0;
    int out[100];
    Graph g = createGraph();
    do{
        printf("***********test menu**************\n");
        printf("**1. Doc file                   **\n");
        printf("**2. **\n");
        printf("**3. **\n");
        printf("**4. bt E                       **\n");
        printf("**5. **\n");
        printf("**6. Thoat                      **\n");
        printf("\nLua chon cua ban la : ");
        scanf("%d",&Luachon);
        if(Luachon == 1){
            FILE *f1,*f2;
            f1 = fopen("webpages.txt","r");
            
            char url[250];
            int temp;
            int id;
            int id1,id2,id3,id4;
            fscanf(f1,"%d",&n1);
            printf("%d\n",n1);
            while(!feof(f1)){
                // fgetc(f1);
                // fgets(url,250,f1);
                fscanf(f1,"%s %d",url,&id);
                printf("%s %d\n",url,id);
                ip[c] = id;
                c++;
                addVertex(g,id,url);
            }
            fclose(f1);
        //     f2 = fopen("pageConnections.txt","r+");
        //     fscanf(f2,"%d",&n2);
        //     fscanf(f2,"%d %d",&id1,&id2);
        //     addEdge(g,id1,id2);
        //     fscanf(f2,"%d,%d",&id1,&id2);
        //     addEdge(g,id1,id2);
        //     fscanf(f2,"%d %d %d",&id1,&id2,&id3);
        //     addEdge(g,id1,id2);
        //     addEdge(g,id1,id3);
        //     fscanf(f2,"%d %d %d %d",&id1,&id2,&id3,&id4);
        //     addEdge(g,id1,id2);
        //     addEdge(g,id1,id3);
        //     addEdge(g,id1,id4);
        //     fscanf(f2,"%d %d %d",&id1,&id2,&id3);
        //     addEdge(g,id1,id2);
        //     addEdge(g,id1,id3);
        //     fclose(f2);
        //     int max = indegree(g,ip[0],out);
        //     for(temp =0;temp<n1;temp++){
        //         if(max < indegree(g,ip[temp],out)){
        //             max = indegree(g,ip[temp],out);
        //         }
        //     }
        //     printf("co nhieu nhat : %d\n",max);
        //     for(temp=0;temp<n1;temp++){
        //         if(max == indegree(g,ip[temp],out)){
        //             printf("%s\n",getVertex(g,ip[temp]));
        //         }
        //     }
        //     int min = indegree(g,ip[0],out);
        //     for(temp =0;temp<n1;temp++){
        //         if(min > indegree(g,ip[temp],out)){
        //             min = indegree(g,ip[temp],out);
        //         }
        //     }
        //     printf("co it nhat : %d\n",min);
        //     for(temp=0;temp<n1;temp++){
        //         if(min == indegree(g,ip[temp],out)){
        //             printf("%s\n",getVertex(g,ip[temp]));
        //         }
        //     }
        }
        if(Luachon == 2){

        }
        if(Luachon == 3){}
        if(Luachon == 4){
            int temp;
            int c1 = 0;
            int c2 = 0;
            for(temp =0;temp<n1;temp++){
                if(indegree(g,ip[temp],out) != 0 && outdegree(g,ip[temp],out) == 0){
                    printf("%s\n",getVertex(g,ip[temp]));
                    c1++;
                }
            }
            if(c1 != 0){
                printf("la cac web chi den ma ko ra\n");
            }else {
                printf("khong co page chi den ma ko ra\n");
            }
            for(temp =0;temp<n1;temp++){
                if(indegree(g,ip[temp],out) == 0 && outdegree(g,ip[temp],out) != 0){
                    printf("%s\n",getVertex(g,ip[temp]));
                    c2++;
                }
            }
            if(c2 != 0){
                printf("la cac web chi ra ma ko den\n");
            }else {
                printf("khong co page chi ra ma ko den\n");
            }
        }
        if(Luachon == 5){
            int s,t,w;
            int path[100];
            int length;
            printf("nhap dinh di : ");
            scanf("%d",&s);
            printf("nhap canh dich : ");
            scanf("%d",&t);
            // BFS(g, s, t, printVertex);
            w = shortestPath(g, s, t, path, &length);
         if (w == INF)
         {
            printf("Route not found\n");
         }else
         {  int i;
            printf("total distance : %d\n",w);
            for (i=0; i<length; i++)
            printf(" => %d",path[i]);
         }
         printf("\n");
        }
        if(Luachon == 6){
            printf("Tam biet\n");
        }
        if(Luachon < 1|| Luachon >6){
            printf("nhap sai moi nhap lai\n");
        }
    }while(Luachon != 6);
    return 0;
}