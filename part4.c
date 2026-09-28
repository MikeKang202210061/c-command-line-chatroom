#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "project.h"

//get current time
void get_current_time(char *time_str)
{
    time_t current_time;
    struct tm *time_info;
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, 20, "%d/%m/%Y %H:%M:%S", time_info);
}

//compare dates
int compare_dates(char *date1, char *date2)
{
    struct tm tm1 = {0}, tm2 = {0};
    //read date into tm struct
    sscanf(date1, "%d/%d/%d", &tm1.tm_mday, &tm1.tm_mon, &tm1.tm_year);
    sscanf(date2, "%d/%d/%d", &tm2.tm_mday, &tm2.tm_mon, &tm2.tm_year);
    tm1.tm_mon -= 1;
    tm1.tm_year -= 1900;
    tm2.tm_mon -= 1;
    tm2.tm_year -= 1900;
    //translate into time that can be compared
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}
