    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include "jrb.h"
    #include "dllist.h"
    

    #define INFINITIVE_VALUE  10000000

    typedef struct {
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
    int totalStation(Graph g,int a[]);
    void bufffile(Graph g);
    char *busroute(Graph g,int id1,int id2);// dang fix
    int main()
    {   
        Graph g=createGraph();
        int i,w, length, path[1000];
        char bus1[300],bus2[300];
        int stationID[1000];
        int id1,id2;
        bufffile(g);// đọc từ file và lưu vào graph 
        int output[10];
        JRB node;
        printf("Nhap vao 2 tram can tim:\n");
        do{                     // nhập vào trạm 1
        printf("Tram thu 1:");
        gets(bus1);
        id1=getID(g,bus1);
        if(id1==-1)
            printf("Khong co tram nay hay nhap lai!\n");
        }while(id1==-1);
       do{                          //nhập trạm 2
        printf("Tram thu 2:");
        gets(bus2);
        id2=getID(g,bus2);
        if(id2==-1)
            printf("Khong co tram nay hay nhap lai!\n");
       }while(id2==-1);
        
         
        w = shortestPath(g,id1,id2, path, &length);// tìm đường ngắn nhất giữa 2 trạm 
        if (w == INFINITIVE_VALUE) // nếu distan =vô cùng thì kc đường đi 
        {
            printf("No path from %s to %s\n", getVertex(g,id1), getVertex(g,id2));
        }else   
        {     
            printf("Path from %s to %s (with total distance %d)\n\n", getVertex(g,id1), getVertex(g,id2), w);
            for (i=0; i<length; i++)    // các trạm đi qua 
                { 

                printf("%s", getVertex(g, path[i]));
                /*if(i>0)
                    printf(" Qua tram: %s\n",busroute(g,path[i-1],path[i]));
                else if(i==0)
                    printf("\n");
                */
                }
        }
        
        dropGraph(g); // giải phóng 
    }

    Graph createGraph() //tạo đồ thị 
    {
    Graph g; 
    g.edges = make_jrb();  
    g.vertices = make_jrb();  
    g.count=0;
    return g;
    }
    int getID(Graph g,char a[]) // lấy id biết tên trạm 
    { JRB node;
    jrb_traverse(node,g.vertices)   // duyệt cây g.vertices(1 node trong cây gồm key là id và val là tên trạm )
    {
        if(strcmp(jval_s(node->val),a)==0)
        return jval_i(node->key);
    }
        
        return -1;
    }
    int makeID(Graph g,char a[]) //tạo id với mỗi tên trạm với g.count riêng biệt trong hàm buffile 
    {
        if(getID(g,a)==-1)
            return g.count;
        else
            return -1;          //nếu trạm đã tồn tại thì trả về -1 ko tạo id nữa 

    }

    void addVertex(Graph g, int id, char* name)// thêm đỉnh với key là id và val là trạm 
    {
        JRB node = jrb_find_int(g.vertices, id);
        if (node==NULL) // only add new vertex 
            jrb_insert_int(g.vertices, id, new_jval_s(strdup(name)));            
    }

    char *getVertex(Graph g, int id) // lấy ra đỉnh nếu biết id
    {
        JRB node = jrb_find_int(g.vertices, id);
        if (node==NULL) 
            return NULL;
        else                
            return jval_s(node->val);
    }     

    void addEdge(Graph graph, int v1, int v2,char *tuyen) //thêm cặp cạnh với 2 đầu mút là 2 trạm 
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
        
            Jval s=new_jval_s(tuyen);
            s.i=1;
            jrb_insert_int(tree, v2,s);
        }
    }

    int getEdgeValue(Graph graph, int v1, int v2)// gán mỗi cạnh có giá trị là 1
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
    void bufffile(Graph g)// hàm đọc file ,hàm này để tôi giải thích nhé :v
    {  FILE *f;  
    f = fopen("hanoi-bus.txt","r+");
        if(f == NULL){
            printf("Loi mo file.!!!");
            return ;
        }
        char tuyen[10];// lưu tuyến 
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
    int indegree (Graph graph, int v, int* output) // tìm các đỉnh trỏ vào v
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

    int outdegree (Graph graph, int v, int* output) //các đỉnh v trỏ tới 
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

    int shortestPath(Graph g, int s, int t, int* path, int*length)//dijstra 
    {
    int distance[1000], min, w, total;
    int previous[1000], tmp[100000];
    int visited[1000] = {};
    
    int n, output[100], i, u, v, start;
    Dllist ptr, queue, node;

    for (i=0; i<1000; i++)
        distance[i] = INFINITIVE_VALUE;
    distance[s] = 0;
    previous[s] = s;
        
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    visited[s] = 0;

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
        u = jval_i(node->val);
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
            if(visited[v]==0)   
                {
                    dll_append(queue, new_jval_i(v));
			         visited[v]=1;
                 }
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
    }

    void dropGraph(Graph graph)//giải phóng 
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
    int totalStation(Graph g,int a[])
    {
        int i=0;
        JRB node;
        jrb_traverse(node,g.vertices)
            {
                a[i]=jval_i(node->key);
                printf("%s\t%d\n",jval_s(node->val),jval_i(node->key));
                i++;

            }
            return i;
    }
     char *busroute(Graph g,int id1,int id2) // lay Tuyen di giua 2 tram
     {  char *a;
        JRB node,tmp,tree;
        jrb_traverse(tmp,g.edges)
        {
            if(id1==jval_i(tmp->key))
            {
                tree=(JRB)jval_v(tmp->val);
                jrb_traverse(node,tree)
                {
                    if(id2==jval_i(node->key))
                        return jval_s(node->key);
                }
            }
        }
        return NULL;
     }