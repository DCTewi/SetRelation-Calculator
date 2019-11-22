#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::cin;
using std::cout;

template <class T> T read()
{
    T x = 0; int w = 0, ch = getchar();
    while (!isdigit(ch)) w |= ch == '-', ch = getchar();
    while (isdigit(ch)) x = (x << 3) + (x << 1) + (ch ^ 48), ch = getchar();
    return w? -x: x;
}

class Graph
{
private:
    struct Edge
    {
        int u, v, next;
        Edge(int u = 0, int v = 0, int next = 0) : u(u), v(v), next(next) {}
    };

    vector<Edge> edges;
    vector<int> head;
    vector<vector<int>> mat;
    int size = 0, cnt = 0;

    void checkIdent()
    {
        if (cnt != size) return;
        for (int i = 1; i <= size; i++)
        {
            if (!mat[i][i]) return;
        }
        isIdent = true;
    }

    void checkReflex()
    {
        int ref = 1, aref = 1;
        for (int i = 1; i <= size; i++)
        {
            if (!mat[i][i])  ref = 0;
            if ( mat[i][i]) aref = 0;
        }
        isReflex = ref; isAntiReflex = aref;
    }

    void checkSym()
    {
        if (!isIdent)
        for (int i = 1; i < cnt; i++)
        {
            int &u = edges[i].u, &v = edges[i].v;
            if (!mat[v][u])
            {
                return;
            }
        }
        isSym = true;
    }

    void checkAntiSym()
    {
        if (!isIdent)
        for (int i = 1; i < cnt; i++)
        {
            int &u = edges[i].u, &v = edges[i].v;
            if (mat[v][u] && u != v)
            {
                return;
            }
        }
        isAntiSym = true;
    }

    void checkTrans()
    {
        for (int x = 1; x <= size; x++)
        {
            for (int i = head[x]; ~i; i = edges[i].next)
            {
                int &y = edges[i].v;
                for (int j = head[y]; ~j; j = edges[j].next)
                {
                    int &z = edges[j].v;
                    if (!mat[x][z])
                    {
                        return;
                    }
                }
            }
        }
        isTrans = true;
    }

public:
    bool isIdent = false;
    bool isReflex = false;
    bool isAntiReflex = false;
    bool isSym = false;
    bool isAntiSym = false;
    bool isTrans = false;
    bool isEquiv = false;
    bool isCompat = false;
    bool isPartOrd = false;

    bool isChecked = false;

    Graph(int n = 0) : size(n)
    {
        mat = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
        head = vector<int>(n + 1, -1);
        edges = vector<Edge>(1);
    }

    void addEdge(int u, int v)
    {
        mat[u][v] = 1;
        edges.emplace_back(Edge(u, v, head[u]));
        head[u] = edges.size() - 1;
        cnt++;
    }

    void checkAll()
    {
        checkIdent();
        checkReflex();
        checkSym();
        checkAntiSym();
        checkTrans();

        if (isReflex)
        {
            if (isTrans)
            {
                if (isSym)
                {
                    isEquiv = true;
                }
                if (isAntiSym)
                {
                    isPartOrd = true;
                }
            }
            if (isSym)
            {
                isCompat = true;
            }
        }

        isChecked = true;
    }
};

int main()
{
    // 输入集合大小
    puts("Please input the number of elements:");
    int n = read<int>();
    map<char, int> idof;
    map<int, char> index;
    // 输入集合元素
    puts("Please input every value of elements:");
    for (int i = 1; i <= n; i++)
    {
        char id; cin >> id;
        idof[id] = i;
        index[i] = id;
    }
    
    Graph graph(n);
    // 输入关系边数
    puts("Please input the number of edges:");
    int m = read<int>();
    // 输入每条边
    puts("Please input every pair(u, v) of edges:");
    for (int i = 1; i <= m; i++)
    {
        char u, v; cin >> u >> v;
        graph.addEdge(idof[u], idof[v]);
    }

    // 关系性质判定
    graph.checkAll();

    // 判定完成
    if (graph.isChecked)
    {
        cout 
        << (graph.isIdent ? "Identity\n" : "") // 恒等
        << (graph.isReflex? "Reflexive\n" : "") // 自反
        << (graph.isAntiReflex ? "Anti-Reflexive\n" : "") // 反自反
        << (graph.isSym ? "Symmetrical\n" : "") // 对称
        << (graph.isAntiSym ? "Anti-Symmetrical\n" : "") // 反对称
        << (graph.isTrans ? "Transitive\n" : "") // 传递
        << (graph.isEquiv ? "Equivalence\n" : "") // 等价
        << (graph.isCompat ? "Consistent\n" : "") // 相容
        << (graph.isPartOrd ? "Partially Ordered\n" : ""); // 偏序
    }

    return 0;
}
