#include "PlanAlgorithm.h"

#define MAX_RISK 999999
#define MAX_PATH_TIME 9999
#define MAX_MONEY 32767

using std::set;
using std::stack;
using std::cout;
using std::endl;

QMap<QString, float> cityRisk {
    {"北京", 0.9},
    {"上海", 0.5},
    {"广州", 0.2},
    {"深圳", 0.9},
    {"成都", 0.5},
    {"杭州", 0.9},
    {"重庆", 0.2},
    {"武汉", 0.9},
    {"南京", 0.5},
    {"郑州", 0.5},
    {"西安", 0.2},
    {"青岛", 0.2}
};

QMap<QString, float> vehicleRisk {
    {"飞机", 9.0},
    {"火车", 5.0},
    {"汽车", 2.0}
};


PlanAlgorithm::PlanAlgorithm(int t, QString src,QString dst, QStringList cities, vector<int> hours, DayTime scheduledDepartT, DayTime expectedDestT)
{
    strategy = t;
    depart = src;
    dest = dst;
    passCities = cities;
    passHours = hours;
    departTime = scheduledDepartT;
    earliestDepartTime = scheduledDepartT;
    latestArrivalTime = expectedDestT;
    // sqlite数据库
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + QString("/TimeTable.db");
    db.setDatabaseName(dbPath);
    qDebug() << dbPath;
    if (!db.open())
        cout << "Failed to connect to database";
}

PlanAlgorithm::~PlanAlgorithm()
{
    // sqlite数据库
    db.close();
}


Plan PlanAlgorithm::getPlan()
{
    switch (strategy) {
        case 1:
        case 2:
        case 3:
            return singlePlan(strategy);
        case 4:
            return limitedSafestPlan();
        default: // case 5:
            return limitedCheapestPlan();
        }
}

Plan PlanAlgorithm::directPlan(int strategyType, QString src, QString dst, DayTime scheduledDepartT)
{
    switch (strategyType) {
        case 1:
            return directSafestPlan(src, dst, scheduledDepartT);
        case 2:
            return directFastestPlan(src, dst, scheduledDepartT);
        default: // case 3
            return directCheapestPlan(src, dst, scheduledDepartT);
    }
}

// 算法辅助函数
long findIndex(vector<QString> v, QString str)
{
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] == str)
            return i;
    return -1;
}

void updateOnePath(vector<int> &onePath, int radix)
{
    onePath[onePath.size() - 1]++;
    for (unsigned long i = onePath.size() - 1; i > 0; i--) {
        if (onePath[i] >= radix) {
            onePath[i] = 0;
            onePath[i - 1]++;
        }
    }
}

// 直达，最少风险
Plan PlanAlgorithm::directSafestPlan(QString src, QString dst, DayTime scheduledDepartT)
{
    qDebug() << "搜索直达，最少风险算法：" << (src + "->" + dst);
    // 读取sqlite数据库
    QSqlQuery query(db);
    query.exec("select * from timeTable");
    vector<QString> cityList;

    // 读取城市列表
    set<QString> citySet;
    QString str;
    while (query.next()) {
        str = query.value("begin").toString();
        citySet.insert(str);
    }
//    qDebug() << "citySet size" << citySet.size();
    for (set<QString>::iterator iter = citySet.begin(); iter != citySet.end(); iter++) {
        cityList.push_back(*iter);
//        qDebug() << "cityList.push_back:" <<  *iter;
    }

    // 数据结构
    stack<QString> s;                // 策略二算法运行时记录路径的栈
    QString top;                     // 栈顶元素
    long n;                         // 临时存储序号
    QString next;                    // 需要访问的下一个元素
    int count = 0;                  // 记录找到的所有路径的个数
    QString tempCity;                // 临时存储元素
    stack<QString> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
    vector<QString> tempRoute;    // 临时存储找到的路径
//    DayTime minscheduledDepartTime(10000, 10000, 10000); // 存储迭代过程中的最小到达时间
    float minPlanRisk = MAX_RISK;
    unsigned long cityNum = cityList.size();// 存储城市个数
    bool **visited = new bool*[cityNum];    // 访问数组，visited[i][j]，第一列表示i是否走过，其余表示i之后j是否走过。
    for (unsigned long i = 0; i < cityNum; i++) {
        visited[i] = new bool[cityNum + 1];
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;
    }

    // 提高算法的运算效率，将终点放到前面来，以便先行入栈
    long destIndex = findIndex(cityList, dst);
    QString temp = cityList[0];
    cityList[0] = dst;
    cityList[destIndex] = temp;

    Plan resPlan; // 初始化 resPlan
    // 算法运算部分
    s.push(src);                                 // 出发地先行入栈
    visited[findIndex(cityList, src)][0] = true; // 已访问出发地

    // 核心部分：搜索所有路径，并根据情况进行回溯
    while(!s.empty())
    {
        top = s.top();
        // 找到了一条从起点到终点的路径
        if (top == dst) {
            count++;
            // 临时存储一个结果
            Plan tempPlan;
            tempPlan.beginTime = scheduledDepartT;
            tempPlan.risk = 0;
            tempPlan.timeCost.day = 0;
            tempPlan.timeCost.hour = 0;
            tempPlan.timeCost.minute = 0;
            tempPlan.moneyCost = 0;
            tempPlan.route.clear();
            bool feasible = true; // 路径是否可行

            DayTime scheduledDepartTime = scheduledDepartT; // 到达新城市的时间后的计划出发时间
            DayTime currentTime;

            // 获取路径
            tempRoute.clear();
            tempS = s;
            while (!tempS.empty()) {
                tempCity = tempS.top();
                tempS.pop();
                tempRoute.push_back(tempCity);
            }
            std::reverse(tempRoute.begin(), tempRoute.end());

            // 路径按照城市分段，确定每两个城市之间的交通方式（Path）
            for (vector<QString>::iterator it = tempRoute.begin(); it != tempRoute.end() - 1; it++) {
                currentTime = scheduledDepartTime;
                currentTime.day = 0;
                Path minPath; // 最短路径
                Path tempPath; // 临时路径
                minPath.risk = MAX_RISK; // 临时路径最小风险

                // 遍历两城市之间的时刻表
                bool flag = false;  // 判断是否有找到，如果没有，天数++
                query.exec(QString("select * from TimeTable where begin='%1' and end='%2'").arg(*it).arg(*(it + 1)));
//                qDebug() << *it << " -> " << *(it + 1);

                // 比较选择两个城市之间所有班次中风险最低的
                while (query.next()) {
                    tempPath.beginTime.fromString(query.value("beginTime").toString());
                    if (tempPath.beginTime > currentTime) { // 可以赶上该列车次
                        flag = true;
                        tempPath.begin = *it;
                        tempPath.vehicle = query.value("vehicle").toString();
                        tempPath.waitTime = tempPath.beginTime - currentTime;
                        tempPath.timeCost.fromString(query.value("timeCost").toString());
                        tempPath.risk = tempPath.timeCost.toHour() * vehicleRisk[tempPath.vehicle] + tempPath.waitTime.toHour() * cityRisk[tempPath.begin]; // 风险 = 出发地停留风险 + 交通风险
                        if (tempPath.risk < minPath.risk) {
                            minPath = tempPath;
                            minPath.number = query.value("number").toString();
                            minPath.end = *(it + 1);
                            minPath.beginTime.day = scheduledDepartTime.day;
                            minPath.endTime = minPath.beginTime + minPath.timeCost;
                            minPath.moneyCost = query.value("moneyCost").toInt();
                        }
                    }
                }
                // 没有找到，天数++，再遍历一遍时刻表
                if (!flag) {
                    if (!feasible) {
                        tempPlan.risk = MAX_RISK;
                        break;
                    }
                    scheduledDepartTime.day += 1;
                    scheduledDepartTime.hour = 0;
                    scheduledDepartTime.minute = 0;
                    it--;
                    feasible = false;
                }
                // 找到了，向 tempPlan 中加入此条路径
                else {
                    scheduledDepartTime.hour = 0;
                    scheduledDepartTime.minute = 0;
                    scheduledDepartTime = minPath.endTime;
                    tempPlan.moneyCost += minPath.moneyCost;
                    tempPlan.timeCost = scheduledDepartTime - scheduledDepartT;
                    tempPlan.route.push_back(minPath);
                    tempPlan.risk += minPath.risk;
                }
            }
            // 更新最优值
            if (tempPlan.risk < minPlanRisk) {
                qDebug() << "更新最小风险方案" << tempPlan.risk;
                minPlanRisk = tempPlan.risk;
                resPlan = tempPlan;
            }
            // 回溯，搜索下一条路径
            s.pop();
            visited[findIndex(cityList, dst)][0] = false;
        }
        // 继续往下搜索
        else {
            for (unsigned int i = 0; i < cityList.size(); i++) {
                if (!visited[findIndex(cityList, top)][i + 1] && !visited[i][0]) {
                    n = i;
                    break;
                }
                else {
                    n = -1;
                }
            }
            // 搜索到了下一个结点
            if (n != -1 && s.size() < 4) {
                next = cityList[n];
                s.push(next);
                visited[findIndex(cityList, top)][n + 1] = true;
                visited[n][0] = true;
            }
            // 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）
            // 回溯，向另一个方向继续搜索下一条路径
            else {
                s.pop();
                for (unsigned long i = 0; i < cityNum + 1; i++)
                    visited[findIndex(cityList, top)][i] = false;
            }
        }
    }
    resPlan.endTime = scheduledDepartT + resPlan.timeCost;
    // 释放内存
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
    return resPlan;
}


// 直达，最少时间
// 采用深度优先搜索算法结合回溯算法，遍历所有有效路径（长度 < 4），从中选出用时最小的路径
Plan PlanAlgorithm::directFastestPlan(QString src, QString dst, DayTime scheduledDepartT)
{
//    qDebug() << "搜索直达，最少时间算法：" << src + "->" + dst;
    // 读取sqlite数据库
    QSqlQuery query(db);
    query.exec("select * from timeTable");

    // 读取城市列表
    vector<QString> cityList;
    set<QString> citySet;
    QString str;
    while (query.next()) {
        str = query.value("begin").toString();
        citySet.insert(str);
    }
    for (set<QString>::iterator iter = citySet.begin(); iter != citySet.end(); iter++) {
        cityList.push_back(*iter);
    }

    // 数据结构
    stack<QString> s;                // 策略二算法运行时记录路径的栈
    QString top;                     // 栈顶元素
    long n;                         // 临时存储序号
    QString next;                    // 需要访问的下一个元素
    int count = 0;                  // 记录找到的所有路径的个数
    QString tempCity;                // 临时存储元素
    stack<QString> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
    vector<QString> tempRoute;    // 临时存储找到的路径
    vector<QString> bestPath;        // 存储找到的最佳路径
    DayTime minscheduledDepartTime(10000, 10000, 10000); // 存储迭代过程中的最小到达时间
    unsigned long cityNum = cityList.size();// 存储城市个数
    bool **visited = new bool*[cityNum]; // 访问数组，visited[i][j]，第一列表示i是否走过，其余表示i之后j是否走过。
    for (unsigned long i = 0; i < cityNum; i++) {
        visited[i] = new bool[cityNum + 1];
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;
    }

    // 提高算法的运算效率，将终点放到前面来，以便先行入栈
    long destIndex = findIndex(cityList, dst);
    QString temp = cityList[0];
    cityList[0] = dst;
    cityList[destIndex] = temp;

    // 初始化 resPlan
    Plan resPlan;
    // 算法运算部分
    s.push(src);                                 // 出发地先行入栈
    visited[findIndex(cityList, src)][0] = true; // 已访问出发地
    // 搜索所有路径，并根据进行情况回溯
    while(!s.empty())
    {
        top = s.top();
        // 找到了一条从起点到终点的路径
        if (top == dst)
        {
            count++;
            // 临时存储一个结果
            Plan tempPlan;
            tempPlan.risk = 0;
            tempPlan.timeCost.day = 0;
            tempPlan.timeCost.hour = 0;
            tempPlan.timeCost.minute = 0;
            tempPlan.moneyCost = 0;
            tempPlan.route.clear();
            bool feasible = true;

            // 记录到达当前结点时的最佳时间
            DayTime scheduledDepartTime = scheduledDepartT;
            DayTime currentTime;

            // 产生调试信息：搜索出的一条路径
            // 并产生一个临时的 tempPlan
            tempRoute.clear();
            tempS = s;
            //std::cout << count << ": ";
            while (!tempS.empty()) {
                tempCity = tempS.top();
                tempS.pop();
                tempRoute.push_back(tempCity);
            }
            std::reverse(tempRoute.begin(), tempRoute.end());
            for (vector<QString>::iterator it = tempRoute.begin(); it != tempRoute.end() - 1; it++) {
                currentTime = scheduledDepartTime;
                currentTime.day = 0;
                Path minPath;                               // 临时存储一条路径
                Path tempPath;
                minPath.endTime.day = MAX_PATH_TIME;

                // 遍历两城市之间的时刻表
                bool flag = false;  // 判断是否有找到，如果没有，天数++
                query.exec(QString("select * from TimeTable where begin='%1' and end='%2'").arg(*it).arg(*(it + 1)));
                while (query.next()) {
                    tempPath.beginTime.fromString(query.value("beginTime").toString());
                    if (tempPath.beginTime > currentTime) {
                        flag = true;
                        tempPath.timeCost.fromString(query.value("timeCost").toString());
                        tempPath.beginTime.day = scheduledDepartTime.day;
                        tempPath.endTime = tempPath.beginTime + tempPath.timeCost;
                        if (tempPath.endTime < minPath.endTime) {
                            minPath.endTime = tempPath.endTime;
                            minPath.number = query.value("number").toString();
                            minPath.beginTime = tempPath.beginTime;
                            minPath.waitTime = minPath.beginTime - scheduledDepartTime;
                            minPath.timeCost = tempPath.timeCost;
                            minPath.endTime = minPath.beginTime + minPath.timeCost;
                            minPath.moneyCost = query.value("moneyCost").toInt();
                            minPath.vehicle = query.value("vehicle").toString();
                            minPath.risk = minPath.timeCost.toHour() * vehicleRisk[minPath.vehicle] + minPath.waitTime.toHour() * cityRisk[minPath.begin]; // 风险 = 出发地停留风险 + 交通风险
                        }
                    }
                }
                // 没有找到，天数++，再遍历一遍时刻表
                if (!flag) {
                    if (!feasible) { // 第二次搜索无果，代表该路径不可行
                        scheduledDepartTime.day = MAX_PATH_TIME;
                        break;
                    }
                    scheduledDepartTime.day += 1;
                    scheduledDepartTime.hour = 0;
                    scheduledDepartTime.minute = 0;
                    it--;
                    feasible = false;
                }
                // 找到了，向 tempPlan 中加入此条路径
                else {
                    scheduledDepartTime = minPath.endTime;
                    minPath.begin = *it;
                    minPath.end = *(it + 1);
                    tempPlan.moneyCost += minPath.moneyCost;
                    tempPlan.timeCost = scheduledDepartTime - scheduledDepartT;
                    tempPlan.risk += minPath.risk;
                    tempPlan.route.push_back(minPath);
                }
            }
            // 更新最优值
            if (scheduledDepartTime < minscheduledDepartTime) {
                minscheduledDepartTime = scheduledDepartTime;
                bestPath = tempRoute;
                resPlan = tempPlan;
            }
            // 回溯，搜索吓一条路径
            s.pop();
            visited[findIndex(cityList, dst)][0] = false;
        }
        // 继续往下搜索
        else {
            for (unsigned int i = 0; i < cityList.size(); i++) {
                if (!visited[findIndex(cityList, top)][i + 1] && !visited[i][0]) {
                    n = i;
                    break;
                }
                else {
                    n = -1;
                }
            }
            // 搜索到了下一个结点
            if (n != -1 && s.size() < 4) {
               next = cityList[n];
               s.push(next);
               visited[findIndex(cityList, top)][n + 1] = true;
               visited[n][0] = true;
            }
            // 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）
            // 回溯，向另一个方向继续搜索下一条路径
            else {
               s.pop();
               for (unsigned long i = 0; i < cityNum + 1; i++)
                 visited[findIndex(cityList, top)][i] = false;
            }
        }
    }
    // 释放内存
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
    resPlan.endTime = minscheduledDepartTime;
    return resPlan;
}

// 最少费用，直达
Plan PlanAlgorithm::directCheapestPlan(QString src, QString dst, DayTime scheduledDepartT)
{
    qDebug() << "PlanAlgorithm::directCheapestPlan";
    // 读取sqlite数据库
    QSqlQuery query(db);
    query.exec("select * from timeTable");

    // 读取城市列表
    vector<QString> cityList;
    set<QString> citySet;
    QString str;
    while (query.next()) {
        str = query.value("begin").toString();
        citySet.insert(str);
    }
    set<QString>::iterator iter;
    for (iter = citySet.begin(); iter != citySet.end(); iter++) {
        cityList.push_back(*iter);
    }

    /* 建立算法所需要的 Graph 类 */
    Graph G(cityList.size());
    G.setVexsList(cityList);

    // 建立费用邻接矩阵
    vector<QString>::iterator i, j;
    int value;
    QString select;
    for (i = cityList.begin(); i != cityList.end(); i++)
        for (j = cityList.begin(); j != cityList.end(); j++) {
            if (*i != *j) {
                QString s1 = *i;
                QString s2 = *j;
                select = QString("select * from timeTable where begin='%1' and end='%2' order by moneyCost asc limit 1").arg(s1).arg(s2);
                query.exec(select);
                if (query.next()) {
                    value = query.value("moneyCost").toInt();
                    G.setDistence(*i, *j, value);
                    QString MyTimeCost = query.value("timeCost").toString();
                    DayTime t1;
                    t1.day = 0;
                    t1.hour = MyTimeCost.section(":", 0, 0).toUShort();
                    t1.minute = MyTimeCost.section(":", 1, 1).toUShort();
                    G.setTime(*i, *j, t1);
                }
                else {
                    G.setDistence(*i, *j, MAX_MONEY);
                }
            }
        }
    qDebug() << "printMatrix:";
//    G.printMatrix();
    Plan resPlan;
    resPlan.risk = 0;
    G.Dijkstra(src, dst, resPlan);    // 调用算法

    vector<Path>::iterator it;
    DayTime timeUsed;
    DayTime currentTime;
    DayTime scheduledDepartTime = scheduledDepartT; // 上一次到达时间
    // 根据Dijkstra结果重新遍历得到完整数据
    for (it = resPlan.route.begin(); it != resPlan.route.end(); it++) {
        QString start = (*it).begin;
        QString end = (*it).end;
//        qDebug() << "get Dij res: " << start << "->" << end << "cost: " << (*it).moneyCost;

        timeUsed = G.getTime((*it).begin, (*it).end);
        // 相同价格取乘坐时间最短的
        select = QString("select * from timeTable where begin='%1' and end='%2' and moneyCost=%3 order by timeCost asc").arg(start).arg(end).arg((*it).moneyCost);
        query.exec(select);
        query.first();

        it->beginTime.fromString(query.value("beginTime").toString());
        it->number = query.value("number").toString();
        it->vehicle = query.value("vehicle").toString();
        it->timeCost.fromString(query.value("timeCost").toString());

        qDebug() << "scheduledDepartTime";
        scheduledDepartTime.print();

        it->beginTime.day = scheduledDepartTime.day;
        // 当天无法乘车
        if (scheduledDepartTime > it->beginTime) {
             it->beginTime.day++;
        }
        qDebug() << "beginTime:";
        it->beginTime.print();

        it->waitTime = it->beginTime - scheduledDepartTime;
        qDebug() << "waitTime:";
        it->waitTime.print();

        qDebug() << "timeCost:";
        it->timeCost.print();

        it->endTime = it->beginTime + it->timeCost;
        qDebug() << "endTime:";
        it->endTime.print();
        it->risk = it->timeCost.toHour() * vehicleRisk[it->vehicle] + it->waitTime.toHour() * cityRisk[it->begin];
        resPlan.risk += it->risk;
        resPlan.timeCost = resPlan.timeCost + it->timeCost;
        resPlan.endTime = it->endTime;
        scheduledDepartTime = it->endTime;
    }
    return resPlan;
}

void add_plan(Plan &main, Plan add)
{
    main.timeCost = main.timeCost + (add.endTime - main.endTime);
    main.endTime = add.endTime;
    main.moneyCost += add.moneyCost;
    main.risk += add.risk;
    for (auto path : add.route)
        main.route.push_back(path);
}

Plan PlanAlgorithm::singlePlan(int strategyType)
{
    qDebug() << "PlanAlgorithm::singlePlan: " << strategyType;
    int cityCount = passCities.length();
    Plan resPlan;
    if(cityCount == 0) {
        resPlan = directPlan(strategyType, depart, dest, earliestDepartTime);
    }
    else {
        resPlan = directPlan(strategyType, depart, passCities.at(0), earliestDepartTime);
        DayTime newDepartTime(0,passHours[0],0);
        newDepartTime = newDepartTime + resPlan.endTime;
        qDebug() << "newDeTime:" << newDepartTime.day << ". " << newDepartTime.hour << ":" << newDepartTime.minute;
        int i = 0;
        if(cityCount > 1) {
            for(; i < cityCount-1; i++) {
                Plan middleplan = directPlan(strategyType, passCities.at(i), passCities.at(i+1), newDepartTime);
                add_plan(resPlan, middleplan);
                DayTime passTime(0,passHours[i+1],0);
                newDepartTime = resPlan.endTime + passTime;
                qDebug() << "newDeTime:";
                newDepartTime.print();
            }
        }
        Plan lastplan = directPlan(strategyType, passCities.at(i), dest, newDepartTime);
        add_plan(resPlan, lastplan);
    }
    return resPlan;
}

Plan PlanAlgorithm::limitedSafestPlan()
{
    Plan resPlan;
    if(latestArrivalTime == earliestDepartTime)
    {
        QMessageBox::information(NULL, "提示", "期望到达时间不合理，请重新输入", QMessageBox::Ok, QMessageBox::NoButton);
        resPlan.moneyCost = -1;
        return resPlan;
    }

    /* 配置接口 */
    QSqlQuery query(db);
    query.exec("select * from timeTable");

    /* 读取数据库，读入所有要用的城市 */
    vector<QString> cityList;
    set<QString> citySet;
    QString str;
    while (query.next()) {
        str = query.value("begin").toString();
        citySet.insert(str);
    }
    set<QString>::iterator iter;
    for (iter = citySet.begin(); iter != citySet.end(); iter++) {
        cityList.push_back(*iter);
    }
    // ==================================================
    // （1）计算最少费用的路线，看是否符合时间限制。
    QString tempString;
    resPlan = singlePlan(3); // 3.最少费用
    if(resPlan.timeCost + earliestDepartTime < latestArrivalTime)
    {
        return resPlan;
    }
    // ==================================================
    // （2）计算最少时间的路线，看是否符合时间限制。
    resPlan = singlePlan(2); // 2.最少时间
    if(latestArrivalTime < resPlan.endTime) {
         //cout << "!!!!" << endl;
         resPlan.moneyCost = -1;
         QMessageBox::information(NULL, "提示", "计划到达时间过早，不存在这么快的方案", QMessageBox::Ok,QMessageBox::NoButton);
        return resPlan;
    }
    // ==================================================
    // （3）遍历a和b所有路线，每条路线都按最短时间计算。
    // 选出所有路线中满足用户时间预期的路线，再从中选出花费最少的路线输出
    stack<QString> s;                // 策略二算法运行时记录路径的栈
    QString top;                     // 栈顶元素
    long n;                         // 临时存储序号
    QString next;                    // 需要访问的下一个元素
    QString tempCity;                // 临时存储元素
    stack<QString> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
    vector<QString> tempRoute;    // 临时存储找到的路径
    unsigned long cityNum = cityList.size();// 存储城市个数
    bool **visited = new bool*[cityNum]; // 访问数组，visited[i][j]，第一列表示i是否走过，其余表示i之后j是否走过。
    /* 为访问辅助矩阵分配内存 */
    for(unsigned long i = 0; i < cityNum; i++)
        visited[i] = new bool[cityNum + 1];
    /* 初始化访问辅助矩阵 */
    for (unsigned long i = 0; i < cityNum; i++)
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;

    resPlan.risk = MAX_INT;
    while (!s.empty()) {
        s.pop();
    }
    s.push(depart);
    visited[findIndex(cityList, depart)][0] = true;
    int m = 0;
    bool allPassCitiesFound = false;
    int c = 0;
    // 深搜，回溯
    while(!s.empty())
    {
        top = s.top();
        if (m < passCities.length() && top == passCities.at(m)) {
            m++;
        }
        if (m == passCities.length())
            allPassCitiesFound = true;
        else
            allPassCitiesFound = false;
        // 搜索到一条完整路径
        if (top == dest)
        {
            c++;
            if (allPassCitiesFound) {
                DayTime scheduledDepartTime = earliestDepartTime;
                DayTime currentTime = scheduledDepartTime;
                currentTime.day = 0;
                currentTime.hour = 0;
                currentTime.minute = 0;

                /* 产生调试信息：搜索出的一条路径 */
                /* 并产生一个临时的 tempPlan */
                tempRoute.clear();
                tempS = s;
                while (!tempS.empty()) {
                    tempCity = tempS.top();
                    tempS.pop();
                    tempRoute.push_back(tempCity);
                }
                std::reverse(tempRoute.begin(), tempRoute.end());
                // tempRoute拆分成几个Path，每个Path的所有交通方式为1行
                vector<vector<Path>> allPathOfRoute;
                unsigned long maxLen = 0;
                for (unsigned long it = 0; it < tempRoute.size() - 1; it++) {
                    Path minPath;
                    vector<Path> minPathVector;
                    minPath.begin = tempRoute[it];
                    minPath.end = tempRoute[it + 1];
                    query.exec(QString("select * from TimeTable where begin='%1' and end='%2'").arg(tempRoute[it]).arg(tempRoute[it + 1]));
                    while (query.next()) {
                        minPath.vehicle = query.value("vehicle").toString();
                        minPath.number = query.value("number").toString();
                        minPath.beginTime = DayTime(query.value("beginTime").toString());
                        minPath.timeCost = DayTime(query.value("timeCost").toString());
                        minPath.endTime = minPath.beginTime + minPath.timeCost;
                        minPath.moneyCost = query.value("moneyCost").toInt();
                        //allPathOfRoute[it].push_back(minPath);
                        minPathVector.push_back(minPath);
                    }
                    if (minPathVector.size() > maxLen)
                        maxLen = minPathVector.size();
                    allPathOfRoute.push_back(minPathVector);
                }

                int radix = maxLen;
                vector<int> onePath(allPathOfRoute.size(), 0);
                // 遍历所有路径搜索
                while (onePath[0] != radix) {
                    int idx = 0;
                    Plan tempPlan;
                    tempPlan.route.clear();
                    tempPlan.timeCost.day = 0;
                    tempPlan.timeCost.hour = 0;
                    tempPlan.timeCost.minute = 0;
                    tempPlan.moneyCost = 0;
                    tempPlan.risk = 0;
                    scheduledDepartTime = earliestDepartTime;
                    currentTime = scheduledDepartTime;
                    for (unsigned long index = 0; index < onePath.size(); index++) {
                        if ((int)allPathOfRoute[index].size() - 1 < onePath[index]) {
                            tempPlan.risk = MAX_INT;
                            break;
                        }
                        else {
                            int p = onePath[index];
                            // 包含需要经过的城市，则停留一段时间
                            if (passCities.contains(allPathOfRoute[index][p].begin)) {
                                idx++;
                                DayTime passTime(0, passHours[idx], 0);
                                currentTime = currentTime + passTime;
                            }
                            // 当天无车次，等到第二天
                            if (currentTime > allPathOfRoute[index][p].beginTime)
                                scheduledDepartTime.day++;
                            scheduledDepartTime.minute = 0;
                            scheduledDepartTime.hour = 0;
                            scheduledDepartTime = scheduledDepartTime + allPathOfRoute[index][p].endTime;                // 超过计划时间，抛弃该路径
                            if (scheduledDepartTime > latestArrivalTime) {
                                tempPlan.risk = MAX_INT;
                                break;
                            }
                            Path tempPath = allPathOfRoute[index][p];
                            tempPath.endTime.day = scheduledDepartTime.day;
                            tempPath.beginTime = tempPath.endTime - tempPath.timeCost;
                            tempPath.waitTime = tempPath.beginTime - currentTime;
                            tempPath.risk = tempPath.timeCost.toHour() * vehicleRisk[tempPath.vehicle] + tempPath.waitTime.toHour() * cityRisk[tempPath.begin]; // 风险 = 出发地停留风险 + 交通风险
                            tempPlan.route.push_back(tempPath);
                            tempPlan.moneyCost += allPathOfRoute[index][p].moneyCost;
                            tempPlan.risk += tempPath.risk;
                            currentTime = scheduledDepartTime;
                            currentTime.day = 0;
                        }
                        tempPlan.timeCost = scheduledDepartTime - earliestDepartTime;
                    }
                    // 时间范围内，且费用更少 -> 更新
                    if (tempPlan.risk < resPlan.risk && !(scheduledDepartTime > latestArrivalTime)) {
                        resPlan = tempPlan;
                        for (auto path : tempPlan.route)
                            qDebug() << path.begin + "->" + path.end + " ";
                        qDebug() << resPlan.timeCost.day << QString("天") << resPlan.timeCost.hour << "时" << resPlan.timeCost.minute << "分   ";
                        qDebug() << resPlan.moneyCost << "元";
                        qDebug() << "End Time:" << scheduledDepartTime.day << " " << scheduledDepartTime.hour << " " << scheduledDepartTime.minute << endl;
                        qDebug() << "Risk:" << resPlan.risk;
                    }
                    updateOnePath(onePath, radix);
                }
            }
            // 回溯，继续搜索
            s.pop();
            visited[findIndex(cityList, dest)][0] = false;
        }
        // 非完整路径，继续搜索
        else
        {
            for (unsigned int i = 0; i < cityList.size(); i++) {
                if (!visited[findIndex(cityList, top)][i + 1] && !visited[i][0]) {
                    n = i;
                    break;
                }
                else
                    n = -1;
            }
            /* 搜索到了下一个结点 */
            if (n != -1 && (int)s.size() <= m + 2) {
               next = cityList[n];
               s.push(next);
               visited[findIndex(cityList, top)][n + 1] = true;
               visited[n][0] = true;
            }
            /* 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）*/
            /* 回溯，向另一个方向继续搜索下一条路径 */
            else {
               s.pop();
               if (m > 0 && top == passCities.at(m - 1))
                   m--;
               for (unsigned long i = 0; i < cityNum + 1; i++)
                 visited[findIndex(cityList, top)][i] = false;
            }
        }
    }
    arrivalTime = earliestDepartTime + resPlan.timeCost;          // 生成到达时
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
    return resPlan;
}


Plan PlanAlgorithm::limitedCheapestPlan()
{
    Plan resPlan;
    if(latestArrivalTime == earliestDepartTime)
    {
        QMessageBox::information(NULL, "提示", "期望到达时间不合理，请重新输入", QMessageBox::Ok, QMessageBox::NoButton);
        resPlan.moneyCost = -1;
        return resPlan;
    }

    /* 配置接口 */
    QSqlQuery query(db);
    query.exec("select * from TimeTable");

    /* 读取数据库，读入所有要用的城市 */
    vector<QString> cityList;
    set<QString> citySet;
    QString str;
    while (query.next()) {
        str = query.value("begin").toString();
        citySet.insert(str);
    }
    set<QString>::iterator iter;
    for (iter = citySet.begin(); iter != citySet.end(); iter++) {
        cityList.push_back(*iter);
    }
    // ==================================================
    // （1）计算最少费用的路线，看是否符合时间限制。
    QString tempString;
    resPlan = singlePlan(3); // 3.最少费用
    if(resPlan.timeCost + earliestDepartTime < latestArrivalTime)
    {
        return resPlan;
    }
    // ==================================================
    // （2）计算最少时间的路线，看是否符合时间限制。
    resPlan = singlePlan(2); // 2.最少时间
    if(latestArrivalTime < resPlan.endTime) {
         //cout << "!!!!" << endl;
         resPlan.moneyCost = -1;
         QMessageBox::information(NULL, "提示", "计划到达时间过早，不存在这么快的方案", QMessageBox::Ok,QMessageBox::NoButton);
        return resPlan;
    }
    // ==================================================
    // （3）遍历a和b所有路线，每条路线都按最短时间计算。
    // 选出所有路线中满足用户时间预期的路线，再从中选出花费最少的路线输出
    stack<QString> s;                // 策略二算法运行时记录路径的栈
    QString top;                     // 栈顶元素
    long n;                         // 临时存储序号
    QString next;                    // 需要访问的下一个元素
    QString tempCity;                // 临时存储元素
    stack<QString> tempS;            // 临时存储栈，便于提取栈中元素且不破坏原栈内容
    vector<QString> tempRoute;    // 临时存储找到的路径
    unsigned long cityNum = cityList.size();// 存储城市个数
    bool **visited = new bool*[cityNum]; // 访问数组，visited[i][j]，第一列表示i是否走过，其余表示i之后j是否走过。
    /* 为访问辅助矩阵分配内存 */
    for(unsigned long i = 0; i < cityNum; i++)
        visited[i] = new bool[cityNum + 1];
    /* 初始化访问辅助矩阵 */
    for (unsigned long i = 0; i < cityNum; i++)
        for (unsigned long j = 0; j < cityNum + 1; j++)
            visited[i][j] = false;
    resPlan.moneyCost = MAX_INT;
    while (!s.empty()) {
        s.pop();
    }

    s.push(depart);
    visited[findIndex(cityList, depart)][0] = true;
    int m = 0;
    bool allPassCitiesFound = false;
    int c = 0;
    // 深搜，回溯
    while(!s.empty())
    {
        top = s.top();
        if (m < passCities.length() && top == passCities.at(m)) {
            m++;
        }
        if (m == passCities.length())
            allPassCitiesFound = true;
        else
            allPassCitiesFound = false;
        // 搜索到一条完整路径
        if (top == dest)
        {
            c++;
            if (allPassCitiesFound) {
                DayTime scheduledDepartTime = earliestDepartTime;
                DayTime currentTime = scheduledDepartTime;
                currentTime.day = 0;
                currentTime.hour = 0;
                currentTime.minute = 0;

                /* 产生调试信息：搜索出的一条路径 */
                /* 并产生一个临时的 tempPlan */
                tempRoute.clear();
                tempS = s;
                while (!tempS.empty()) {
                    tempCity = tempS.top();
                    tempS.pop();
                    tempRoute.push_back(tempCity);
                }
                std::reverse(tempRoute.begin(), tempRoute.end());
                // tempRoute拆分成几个Path，每个Path的所有交通方式为1行
                vector<vector<Path>> allPathOfRoute;
                unsigned long maxLen = 0;
                for (unsigned long it = 0; it < tempRoute.size() - 1; it++) {
                    Path minPath;
                    vector<Path> minPathVector;
                    minPath.begin = tempRoute[it];
                    minPath.end = tempRoute[it + 1];
                    query.exec(QString("select * from TimeTable where begin='%1' and end='%2'").arg(tempRoute[it]).arg(tempRoute[it + 1]));
                    while (query.next()) {
                        minPath.vehicle = query.value("vehicle").toString();
                        minPath.number = query.value("number").toString();
                        minPath.beginTime = DayTime(query.value("beginTime").toString());
                        minPath.timeCost = DayTime(query.value("timeCost").toString());
                        minPath.endTime = minPath.beginTime + minPath.timeCost;
                        minPath.moneyCost = query.value("moneyCost").toInt();
                        //allPathOfRoute[it].push_back(minPath);
                        minPathVector.push_back(minPath);
                    }
                    if (minPathVector.size() > maxLen)
                        maxLen = minPathVector.size();
                    allPathOfRoute.push_back(minPathVector);
                }

                int radix = maxLen;
                vector<int> onePath(allPathOfRoute.size(), 0);
                // 遍历所有路径搜索
                while (onePath[0] != radix) {
                    int idx = 0;
                    Plan tempPlan;
                    tempPlan.route.clear();
                    tempPlan.timeCost.day = 0;
                    tempPlan.timeCost.hour = 0;
                    tempPlan.timeCost.minute = 0;
                    tempPlan.moneyCost = 0;
                    tempPlan.risk = 0;
                    scheduledDepartTime = earliestDepartTime;
                    currentTime = scheduledDepartTime;
                    for (unsigned long index = 0; index < onePath.size(); index++) {
                        if ((int)allPathOfRoute[index].size() - 1 < onePath[index]) {
                            tempPlan.moneyCost = MAX_INT;
                            break;
                        }
                        else {
                            int p = onePath[index];
                            // 包含需要经过的城市，则停留一段时间
                            if (passCities.contains(allPathOfRoute[index][p].begin)) {
                                idx++;
                                DayTime passTime(0, passHours[idx], 0);
                                currentTime = currentTime + passTime;
                            }
                            // 当天无车次，等到第二天
                            if (currentTime > allPathOfRoute[index][p].beginTime)
                                scheduledDepartTime.day++;
                            scheduledDepartTime.minute = 0;
                            scheduledDepartTime.hour = 0;
                            scheduledDepartTime = scheduledDepartTime + allPathOfRoute[index][p].endTime;                // 超过计划时间，抛弃该路径
                            if (scheduledDepartTime > latestArrivalTime) {
                                tempPlan.moneyCost = MAX_INT;
                                break;
                            }
                            Path tempPath = allPathOfRoute[index][p];
                            tempPath.endTime.day = scheduledDepartTime.day;
                            tempPath.beginTime = tempPath.endTime - tempPath.timeCost;
                            tempPath.waitTime = tempPath.beginTime - currentTime;
                            tempPath.risk = tempPath.timeCost.toHour() * vehicleRisk[tempPath.vehicle] + tempPath.waitTime.toHour() * cityRisk[tempPath.begin]; // 风险 = 出发地停留风险 + 交通风险
                            tempPlan.route.push_back(tempPath);
                            tempPlan.risk += tempPath.risk;
                            tempPlan.moneyCost += allPathOfRoute[index][p].moneyCost;
                            currentTime = scheduledDepartTime;
                            currentTime.day = 0;
                        }
                        tempPlan.timeCost = scheduledDepartTime - earliestDepartTime;
                    }
                    // 时间范围内，且费用更少 -> 更新
                    if (tempPlan.moneyCost < resPlan.moneyCost && !(scheduledDepartTime > latestArrivalTime)) {
                        resPlan = tempPlan;
                        for (auto path : tempPlan.route)
                            qDebug() << path.begin + "->" + path.end + " ";
                        qDebug() << resPlan.timeCost.day << QString("天") << resPlan.timeCost.hour << "时" << resPlan.timeCost.minute << "分   ";
                        qDebug() << resPlan.moneyCost << "元";
                        qDebug() << "End Time:" << scheduledDepartTime.day << " " << scheduledDepartTime.hour << " " << scheduledDepartTime.minute << endl;
                    }
                    updateOnePath(onePath, radix);
                }
            }
            // 回溯，继续搜索
            s.pop();
            visited[findIndex(cityList, dest)][0] = false;
        }
        // 非完整路径，继续搜索
        else
        {
            for (unsigned int i = 0; i < cityList.size(); i++) {
                if (!visited[findIndex(cityList, top)][i + 1] && !visited[i][0]) {
                    n = i;
                    break;
                }
                else
                    n = -1;
            }
            /* 搜索到了下一个结点 */
            if (n != -1 && (int)s.size() <= m + 2) {
               next = cityList[n];
               s.push(next);
               visited[findIndex(cityList, top)][n + 1] = true;
               visited[n][0] = true;
            }
            /* 如果没有找到下一个结点（此方向遍历结束）或路径过长（不可能时最优解了）*/
            /* 回溯，向另一个方向继续搜索下一条路径 */
            else {
               s.pop();
               if (m > 0 && top == passCities.at(m - 1))
                   m--;
               for (unsigned long i = 0; i < cityNum + 1; i++)
                 visited[findIndex(cityList, top)][i] = false;
            }
        }
    }
    arrivalTime = earliestDepartTime + resPlan.timeCost;          // 生成到达时
    for (unsigned long i=0; i < cityNum;i++)
        delete[] visited[i];
    delete[] visited;
    return resPlan;
}

