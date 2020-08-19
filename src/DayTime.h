#ifndef DayTime_H
#define DayTime_H

#include <QDebug>
#include <QString>

#include <iostream>

class DayTime {
public:
    // Day + Time数据结构
    unsigned short day;
    unsigned short hour;
    unsigned short minute;

    // 构造函数
    DayTime();
    DayTime(unsigned short d, unsigned short h, unsigned short m);
    DayTime(QString timeString);
    DayTime(const DayTime &obj);

    // 析构函数
    ~DayTime();

    // 重载运算符、比较符
    DayTime operator+(const DayTime &t);
    DayTime operator-(const DayTime &t);
    bool operator==(const DayTime &t);
    bool operator<(const DayTime &t);
    bool operator>(const DayTime &t);

    // 格式化字符串中的时间
    void fromString(QString timeString);
    void print();
    float toHour(); // 转化为浮点数表示的小时
    int toMinute(); // 转化为整数表示的分钟
};

#endif // DayTime_H
