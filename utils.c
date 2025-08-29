#include "utils.h"

void fullfill_entry(struct database *lgPtr, int id)
{
    // fullfill id
    snprintf(lgPtr->entries[id].id, 5, "%04d", id); // 格式化为 4 位数字，前面补零
    // fullfill date
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(lgPtr->entries[id].date, 9, "%Y%m%d", tm); // 格式化为 "YYYYMMDD"
    // fullfill thing,money,unit with str
    int matched = sscanf(lgPtr->entries[id].str, "%[^0-9]%f%[^0-9]",
                         lgPtr->entries[id].thing,
                         &lgPtr->entries[id].money,
                         lgPtr->entries[id].unit);
    if (matched != 3)
    {
        printf("输入格式错误，无法解析事由、金额或单位。\n");
        lgPtr->entries[id].thing[0] = '\0';
        lgPtr->entries[id].money = 0.0;
        lgPtr->entries[id].unit[0] = '\0';
    }
}

void write_entry(struct database *lgPtr, int id)
{
    printf("请输入条目：（如：买蔬菜花费20元）");
    scanf("%s", lgPtr->entries[id].str);
    fullfill_entry(lgPtr, id);
}
