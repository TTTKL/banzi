#include<bits/stdc++.h>
using namespace std;
int n,m,s,t,tot=0;
int head[10010];

struct node{
    int t,l,next;
}edge[20020];

void addedge(int x,int y,int z)
{
    edge[++ tot].l = z;
    edge[tot].t = y;
    edge[tot].next = head[x];
    head[x] = tot; 
}

int dis[10010];
int vis[10010]={0};

queue<int> q;

int SPFA(int a,int b)
{
    memset(dis,0x3f,sizeof(dis));
    dis[a] = 0;
    vis[a] = 1;
    q.push(s);
    while(!q.empty())
    {
        int x = q.front();
        q.pop();
        vis[x] = 0;
        for(int i = head[x];i != -1;i = edge[i].next)
        {
            int y = edge[i].t;
            if(dis[y] > dis[x] + edge[i].l)
            {
                dis[y] = dis[x] + edge[i].l;
                if(!vis[y])
                {
                    q.push(y);
                    vis[y] = 1;
                } 
            }
        }
    }
    if(dis[b] >= 0x3f3f3f3f)
    {
        return -1;
    }
    return dis[b];
}

int main()
{
    cin >> n >> m >> s >> t;
    memset(head,-1,sizeof(head));
    memset(edge,-1,sizeof(edge));
    while(m --)
    {
        int x,y,z;
        cin >> x >> y >> z;
        addedge(x,y,z);
        addedge(y,x,z);
    }
    int ans = SPFA(s,t);
    cout << ans << "\n";
    return 0;
}
