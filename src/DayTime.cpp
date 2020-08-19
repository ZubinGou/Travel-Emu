#include "DayTime.h"

void normalizeTime(unsigned short &day, unsigned short &hour, unsigned short &minute) {
    unsigned short hourCarry = 0;
    unsigned short dayCarry = 0;
    if(minute >= 60) {
        hourCarry = minute/60;
        minute %= 60;
    }
    hour += hourCarry;
    if(hour >= 24) {
        dayCarry = hour/24;
        hour %= 24;
    }
    day += dayCarry;
}

DayTime::DayTime() {
    minute = 0;
    day = 0;
    hour = 0;
}

DayTime::DayTime(unsigned short d, unsigned short h, unsigned short m) {
    minute = m;
    hour = h;
    day = d;
    normalizeTime(day, hour, minute);
}

DayTime::DayTime(QString timeString) {
    day = 0;
    hour = timeString.section(":", 0, 0).toUShort();
    minute = timeString.section(":", 1, 1).toUShort();
    normalizeTime(day, hour, minute);
}

DayTime::DayTime(const DayTime &obj) {
    this->minute = obj.minute;
    this->hour = obj.hour;
    this->day = obj.day;
    normalizeTime(this->day, this->hour, this->minute);
}

DayTime::~DayTime() {

}

DayTime DayTime::operator+(const DayTime &t) {
    DayTime newMyTime(this->day+t.day, this->hour+t.hour, this->minute+t.minute);
    return newMyTime;
}

DayTime DayTime::operator-(const DayTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    if(thisSumMinute < tSumMinute)
        std::cerr << "Illegal DayTime Operation" << std::endl;
    DayTime newMyTime(0, 0, thisSumMinute-tSumMinute);
    return newMyTime;
}

bool DayTime::operator==(const DayTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute == tSumMinute);
}

bool DayTime::operator<(const DayTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute < tSumMinute);
}

bool DayTime::operator>(const DayTime &t) {
    unsigned thisSumMinute = this->day * 1440 + this->hour * 60 + this->minute;
    unsigned tSumMinute = t.day * 1440 + t.hour * 60 + t.minute;
    return (thisSumMinute > tSumMinute);
}

void DayTime::fromString(QString timeString) {
    day = 0;
    hour = timeString.section(":", 0, 0).toUShort();
    minute = timeString.section(":", 1, 1).toUShort();
    normalizeTime(day, hour, minute);
}

void DayTime::print()
{
    qDebug()<<day << "天" << hour << "时" << minute << "分";
}

float DayTime::toHour()
{
    return (float)day * 24 + (float)hour + (float)minute / 60.0;
}

int DayTime::toMinute()
{
    return day * 24 * 60 + hour * 60 + minute;
}
