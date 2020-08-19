#ifndef PASSENGER_H
#define PASSENGER_H

#include "DayTime.h"
#include "PlanAlgorithm.h"

#include <string>
#include <QStringList>
#include <QTableWidget>

using namespace std;

class Passenger {
public:
    /* 由交互见面传来的某游客的信息 */
    QString id;                         // 乘客id

    QString begin;                      // 出发地
    QString end;                        // 目的地
    int strategy;                      // 旅客选择的策略1. 2. 3. 4. 5.
    DayTime earliestDepartTime;          // 期望出发时间
    DayTime latestArrivalTime;            // 期望到达时间
    QStringList passCity;
    vector<int> passHours;

    Plan plan;
    PlanAlgorithm * planAlgorithm;
    bool planReady; // 旅行方案就绪
    QString log;

    // 旅行过程数据结构
    bool started;
    bool onPath;
    int pathIndex; // 当前路径，0表示第一条
    Path curPath; // 当前路径
    QString status; // 当前状态描述
    QColor color;
    QDateTime expBeginTime; // 期望出发时间
    QDateTime expEndTime; // 期望到达时间
    QDateTime beginTime; // 实际出发时间
    QDateTime endTime; // 实际到达时间
    int minuteCost; // 所需总时间，分钟表示，用于进度条
    int curMinuteCost; // 已经使用分钟数
    int curMoneyCost;
    QVector<QPointF> passedPoints;  //用于存储当前模拟路径已通过的点

    /* 构造函数 */
    Passenger();
    void setPlan(QString start, QString dest, QStringList passCity, vector<int> passHours, DayTime startTime, DayTime endTime, int strategy);
    void getPlan(); // 获取方案
};

#endif // PASSENGER_H
