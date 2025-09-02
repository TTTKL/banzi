#include<bits/stdc++.h>
using namespace std;
int n,m,s,t,tot=0;
int head[10010];

struct node{
    int t,l,next;
}edge[20020];


void addedge(int x,int y,int z)
{
    edge[++tot].l=z;
    edge[tot].t=y;
    edge[tot].next=head[x];
    head[x]=tot;
}

struct ty
{
    int x,dis;
    bool operator < (const ty &a) const 
    {
        return dis>a.dis;
    }
};

priority_queue<ty> q;
int dis[10010];
int vis[10010]={0};

int dij(int a,int b)
{
    memset(dis,0x3f,sizeof(dis));
    dis[a]=0;
    ty tmp;
    tmp.x=a;
    tmp.dis=0;
    q.push(tmp);
    while(!q.empty())
    {
        ty ind=q.top();
        q.pop();
        if(vis[ind.x])
        {
            continue;
        }
        vis[ind.x]=1;
        for(int i=head[ind.x];i!=-1;i=edge[i].next)
        {
            int y=edge[i].t;
            if(vis[y])continue;
            if(dis[y]>dis[ind.x]+edge[i].l)
            {
                dis[y]=dis[ind.x]+edge[i].l;
                ty ind2;
                ind2.x=y;
                ind2.dis=dis[y];
                q.push(ind2);
            }
            
        }
    }
    if(dis[b]>=0x3f3f3f3f)return -1;
    return dis[b];
}

int main()
{
    cin>>n>>m>>s>>t;
    memset(edge,-1,sizeof(edge));
    memset(head,-1,sizeof(head));
    while(m--)
    {
        int x,y,z;
        cin>>x>>y>>z;
        addedge(x,y,z);
        addedge(y,x,z);
    }
    int ans=dij(s,t);
    cout<<ans<<"\n";
    
    return 0;
}