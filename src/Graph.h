#ifndef GRAPH_H
#define GRAPH_H

#include "Plan.h"
#include "DayTime.h"

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#define MAX_INT 32767        // Dijkstra Algorithm 中设定的无限大的值

using std::vector;
//using std::string;

// 图类的定义
class Graph 
{
private:
    /* 策略一 */
    /* 一个有向图 */
    unsigned long vexCount;       // 顶点数
    vector<QString> vexs;        // 顶点表
    int **distanceMat;         // 距离邻接矩阵
    DayTime **timeMat;         // 出发时间邻接矩阵

    /* 需要用到的 getter */
    QString numToCity(long i);      // 返回序号对应的城市
    long cityToNum(QString city);// 返回城市对应的序号

public:
    Graph(unsigned long cityNum);   // 构造函数,参数是城市的数量
    ~Graph();                       // 析构函数

    /* 需要用到的 getter 函数 */
    int getDistence(QString city1,QString city2);                // 返回邻接矩阵某个点的值
    DayTime getTime(QString city1, QString sity2);   // 返回 MyTime 矩阵某个点的值

    /* 需要用到的 setter 函数 */
    void setDistence(QString city1,QString city2,int value);     // 为邻接矩阵某个点赋值
    void setTime(QString city1, QString city2, DayTime value);   // 为 MyTime 矩阵某个点赋值
    void setVexsList(vector<QString> list);                  // 传入城市表

    /* Dijkstra 算法求最短路径，将结果存在 result（目前来看是暂时）中 */
    void Dijkstra(QString Dep, QString Dest, Plan &result);

    /* 打印邻接矩阵，用于产生调试信息 */
    void printMatrix();
};

#endif //GRAPH_H
