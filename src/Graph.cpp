#include "Graph.h"

using std::cout;
using std::endl;

Graph::Graph(unsigned long cityNum)
{
    vexCount = cityNum;
	
    // 动态申请内存
    distanceMat = new int*[vexCount];
    for(unsigned long i = 0; i < vexCount; i++)
        distanceMat[i] = new int[vexCount];

    timeMat = new DayTime*[vexCount];
    for(unsigned long i = 0; i < vexCount; i++)
        timeMat[i] = new DayTime[vexCount];

    // 均初始化为极大值MAX_INT
    for(unsigned long  i = 0; i < vexCount; i++)
        for(unsigned long j = 0;j < vexCount; j++)
        {
            distanceMat[i][j] = MAX_INT;
        }
}

Graph::~Graph()
{
    for(unsigned long i = 0; i < vexCount;i++)
        delete[] distanceMat[i];
    delete[] distanceMat;
}

// 返回城市的序号
long Graph::cityToNum(QString city)
{
    vector<QString>::iterator index;
    for (index = vexs.begin(); index != vexs.end(); index++ )
        if (*index == city)
            return (index - vexs.begin());
    return 0;
}

// 返回邻接矩阵中某点值
int Graph::getDistence(QString city1,QString city2)
{
    long i = cityToNum(city1);
    long j = cityToNum(city2);
    return distanceMat[i][j];
}

// 返回邻接矩阵中某点值
DayTime Graph::getTime(QString city1, QString city2)
{
    long i = cityToNum(city1);
    long j = cityToNum(city2);
    return timeMat[i][j];
}

// 返回序号i对应城市
QString Graph::numToCity(long i)
{
    return vexs[i];
}

void Graph::setDistence(QString city1,QString city2,int value)
{
    long i = cityToNum(city1);
    long j = cityToNum(city2);
    distanceMat[i][j] = value;
}

void Graph::setTime(QString city1, QString city2, DayTime value)
{
    long i = cityToNum(city1);
    long j = cityToNum(city2);
    timeMat[i][j] = value;
}

void Graph::setVexsList(vector<QString> list)
{
    vexs = list;
}

void Graph::printMatrix()
{
    for (unsigned long i = 0; i < vexCount; i++) {
        cout << std::setw(3) << i << ": ";
        for (unsigned long j = 0; j < vexCount; j++) {
            cout << std::setw(7) << distanceMat[i][j];
        }
        cout << endl;
    }
}

// 最少费用策略所用的最短路算法
void Graph::Dijkstra(QString Dep, QString Dest, Plan &result)
{
    /* 需要用到的数据 */
    unsigned long w, v;                 // 临时变量
    int min;                            // 通过迭代算出来的最小值
    Path path;                          // 临时存储一条路径
    long *tempPath = new long[vexCount];  // 临时存储
    bool *s = new bool[vexCount];         // 辅助数组
    int *d = new int[vexCount];           // 辅助数组
    unsigned long n = vexCount;           // 辅助变量
    long v0 = cityToNum(Dep);
    long vt = cityToNum(Dest);

    // Dijkstra Algorithm
    /* 初始化 */
    for (v = 0; v < n; v++) {
        s[v] = false;
        d[v] = distanceMat[v0][v];
        if (d[v] < MAX_INT) tempPath[v] = v0;
        else tempPath[v] = -1;
    }
    s[v0] = true;
    d[v0] = 0;

    /* 最短路算法运算部分 */
    for (unsigned long i = 1; i < n; i++) {
        min = MAX_INT;
        for (w = 0; w < n; w++)
            if (!s[w] && d[w] < min) {
                v = w;
                min = d[w];
            }
        s[v] = true;
        for (w = 0; w < n; w++)
            if (!s[w] && (d[v] + distanceMat[v][w] < d[w])) {
                d[w] = d[v] + distanceMat[v][w];
                tempPath[w] = v;
            }
    }

    /* 提取路径 */
    while (tempPath[vt] != -1) {
        long pre = tempPath[vt];
        path.begin = numToCity(pre);
        path.end = numToCity(vt);
        path.moneyCost = distanceMat[pre][vt];
        result.route.push_back(path);
        vt = pre;
    }
    vt = cityToNum(Dest);
    result.moneyCost = d[vt];
    std::reverse(result.route.begin(), result.route.end());

    /* 释放内存 */
    delete [] tempPath;
    delete [] s;
    delete [] d;
}
