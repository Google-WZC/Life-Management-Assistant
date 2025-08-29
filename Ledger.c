/**
 * @file Ledger.c
 * @platform Ubuntu24
 * @date 2025-09-28
 * @brief 记账本终端界面程序，具备增加新条目到数据库，计算总开销的功能
 * @todo 1 增加删除功能 2 regex entry 3 图形界面
 * @version 0.1
 */

#include "Ledger.h"

int main()
{
    struct database ledger = {0};
    struct database *lgPtr = &ledger;

    init(lgPtr, fopen(DATABASE_FILE_NAME, "rb"));

    char run = ' '; // 32
    while (run != '0')
    {
        system("clear"); // 终端清屏
        run = UI_choose(lgPtr);
    }
}