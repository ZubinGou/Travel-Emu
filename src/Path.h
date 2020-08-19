#ifndef PATH_H
#define PATH_H

#include "DayTime.h"
#include <QString>


typedef struct PATH {
    // path的起始、结束城市
    QString begin;
    QString end;

    // 交通工具类型、班次
    QString vehicle;
    QString number;

    // 本条路径开始时间与结束时间、等待时间
    DayTime waitTime; // 停留、候车时间
    DayTime beginTime;
    DayTime endTime;

    // 本条路径时间与金钱的花费、风险
    DayTime timeCost;
    int moneyCost;
    float risk; // 风险 = 出发地停留风险 + 交通风险
} Path;

#endif // PATH_H
