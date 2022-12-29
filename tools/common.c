//
// Created by Carl.Guo on 2022/12/30.
//
#include "common.h"

int leapYear(int y)
{
    if (y % 400 == 0 || y % 4 == 0 && y % 100 != 0)
        return 1;
    return 0;
}

/**
 * whatWeek
 * @return
 */
int whatWeekIsToday()
{
    int year, month, day;
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    year = p->tm_year + 1900;
    month = p->tm_mon + 1;
    day = p->tm_mday;

    int a, sum = 0;
    int b[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int j = 1; j < year; j++)
    {
        for (int i = 1; i <= 12; i++)
        { sum = sum + b[i]; }
        if (leapYear(j) == 1)
        { sum = sum + 1; }
    }
    for (int i = 0; i < month; i++)
    { sum = sum + b[i]; }
    if (month > 2 && leapYear(year) == 1)
    { sum = sum + 1; }
    a = (sum + day) % 7;
    return a;
}
