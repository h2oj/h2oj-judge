#include <iostream>
#include <queue>
#include <thread>
using namespace std;

#define MAX_N 105
#define MAX_M 5000005
struct edge_t {
    int v, w, nxt;
} edge[MAX_M];
int head[MAX_N], din[MAX_N], dout[MAX_N];
int cur = 1;
int c[MAX_N], u[MAX_N];

inline void add_edge(int u, int v, int w) {
    edge[cur].v = v;
    edge[cur].w = w;
    edge[cur].nxt = head[u];
    head[u] = cur++;
    ++din[v];
    ++dout[u];
}

int main() {
    int n, p;
    cin >> n >> p;
    for (int i = 1; i <= n; ++i) {
        cin >> c[i] >> u[i];
        if (c[i] == 0) c[i] = -u[i];
    }
    for (int i = 1; i <= p; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
    }
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
        if (din[i] == 0) q.push(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (c[u] <= 0) continue;
        for (int e = head[u]; e; e = edge[e].nxt) {
            const int v = edge[e].v;
            c[v] += edge[e].w * c[u];
            if (--din[v] == 0) q.push(v);
        }
    }
    bool f = true;
    for (int i = 1; i <= n; ++i) {
        if (c[i] > 0 && dout[i] == 0) {
            cout << i << " " << c[i] << endl;
            f = false;
        }
    }
    if (f) cout << "NULL" << endl;
    return 0;
}