#ifndef PLAN_H
#define PLAN_H

#include "Path.h"
#include "DayTime.h"

#include <vector>

using std::vector;

typedef struct PLAN {
    // 方案时间、风险、费用
    float risk; // 旅行风险
    int moneyCost; // 费用
    DayTime beginTime; // 旅行开始时间
    DayTime timeCost; // 旅行时间消耗
    DayTime endTime; // 旅行结束时间
    // 出发地 -> 途经地 -> 目的地
    vector<Path> route;
} Plan;

#endif // PLAN_H
