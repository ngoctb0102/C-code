#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"

#define INFINITIVE_VALUE  10000000

typedef struct JRB{
   JRB edges;
   JRB vertices;
   int count;
} Graph;
Graph createGraph();
void addVertex(Graph graph, int id, char* name);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2,char *tuyen);
int getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int* output);
int outdegree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
int shortestPath(Graph graph, int s, int t, int* path, int*length);
int getID(Graph g,char a[]);
int makeID(Graph g,char a[]);
void bufffile(Graph g);
void printVertex(int v) // in 
{
     printf("%4d", v);
}
int main()
{   
    Graph g=createGraph();
    int i,w, length, path[100];
    char bus1[300],bus2[300];
    int id1,id2;
    bufffile(g);
    int output[10];
    JRB node;
  /*printf("Nhap vao 2 tram can tim:\n");
    do{
    printf("Tram thu 1:");
    gets(bus1);
    id1=getID(g,bus1);
    }while(id1==-1);
    do{
    printf("Tram thu 2:");
    gets(bus2);
    id2=getID(g,bus2);
    }while(id2==-1);*/
    
    int m=outdegree(g,2,output);
    for(i=0;i<m;i++)
    printf("%d\n",output[i] );    
   //w = shortestPath(g, 0,3, path, &length);
    //if (w == INFINITIVE_VALUE)
    //{
    //     printf("No path from %s to %s\n", getVertex(g,id1), getVertex(g,id2));
    //}else
    //{     
         printf("Path from %s to %s (with total distance %d)\n", getVertex(g,0), getVertex(g,3), w);
         //for (i=0; i<length; i++)
          //   { 
            DFS(g,0,3,printVertex);
          //    printf("%s->", getVertex(g, path[i]));
                  
          //  }printf("\n");

    //}
    
    dropGraph(g);
}

Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();
   g.count=0;
   return g;
}
int getID(Graph g,char a[])
{ JRB node;
  jrb_traverse(node,g.vertices)
  {
    if(strcmp(jval_s(node->val),a)==0)
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

void addVertex(Graph g, int id, char* name)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) // only add new vertex 
         jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
}

char *getVertex(Graph g, int id)
{
     JRB node = jrb_find_int(g.vertices, id);
     if (node==NULL) 
        return NULL;
     else                
        return jval_s(node->val);
}     

void addEdge(Graph graph, int v1, int v2,char *tuyen)
{
     JRB node, tree;
     if (getEdgeValue(graph, v1, v2)==INFINITIVE_VALUE)
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
       
        Jval s=new_jval_s(strdup(tuyen));
        s.i=1;
        jrb_insert_int(tree, v2,s);
     }
}

int getEdgeValue(Graph graph, int v1, int v2)
{   int count=0;
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return INFINITIVE_VALUE;
    tree = (JRB) jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node==NULL)
       return INFINITIVE_VALUE;
    return jval_i(node->val);
          
          
              
}
void bufffile(Graph g)
{  FILE *f;  
  f = fopen("hanoi-bus.txt","r+");
    if(f == NULL){
        printf("Loi mo file.!!!");
        return ;
    }
    char tuyen[10];
    char name[200];
    char name1[200];
    char c;
    int t,n;
    while(!feof(f)){
        t = 0;
        do{
            c = fgetc(f);
        }while (!isdigit(c));
        do{
            tuyen[t++] = c;
            c = fgetc(f);
        }while (c != ':');
       
        tuyen[t] = '\0';
        n = 0;
        fgetc(f);
        c = fgetc(f);
        do{
            name[n++] = c;
            c = fgetc(f);
        }while (c != '>' && c != '\n' && c != '}');
        name[n - 1] = '\0';
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
            addEdge(g,getID(g,name),getID(g,name1),tuyen);
            strcpy(name,name1);
            if(c == '}') break;
        }while(c != '\n');
        if(c == '}') break;
    }
    fclose(f);
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

/*int shortestPath(Graph g, int s, int t, int* path, int*length)
{
   int distance[1000], min, w, total;
   int previous[1000], tmp[1000];
   int n, output[100], i, u, v, start;
   Dllist ptr, queue, node;

   for (i=0; i<1000; i++)
       distance[i] = INFINITIVE_VALUE;
   distance[s] = 0;
   previous[s] = s;
       
   queue = new_dllist();
   dll_append(queue, new_jval_i(s));

   while ( !dll_empty(queue) )
   {
      // get u from the priority queue   
      min = INFINITIVE_VALUE;   
      dll_traverse(ptr, queue)
      {
          u = jval_i(ptr->val);              
          if (min > distance[u])
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
  
   if (total != INFINITIVE_VALUE)
   {
       tmp[0] = t;
       n = 1;              
       while (t != s)
       {
             t = previous[t];
             tmp[n++] = t;
       }
       for (i=n-1; i>=0; i--)
           path[n-i-1] = tmp[i];
       *length = n;                
   }
   return total;   
}*/

int getAdjacentVertices (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph, v);
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
void printVertex(int v) // in 
{
     printf("%4d", v);
}
void DFS(Graph graph, int start, int stop, void (*func)(int))
{
   int visited[1000] = {};
   int n, output[100], i, u, v;
   Dllist node, stack;
   
   stack = new_dllist();
   dll_append(stack, new_jval_i(start));

   while ( !dll_empty(stack) )
   {
      node = dll_last(stack);
      u = jval_i(node->val);
      dll_delete_node(node);
      if (!visited[u]) 
      {
          func(u);
          visited[u] = 1;
          if ( u == stop ) return;           
          n = getAdjacentVertices(graph, u, output);
          for (i=0; i<n; i++)
          {
              v = output[i];
              if (!visited[v])
                 dll_append(stack, new_jval_i(v));
          }
      }
   }                            
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

