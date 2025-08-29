#include "UI.h"

char UI_choose(struct database *lgPtr)
{
    printf("欢迎使用记账本！目前有 %d 条记录，总开销为 %.2f 元\n", lgPtr->count, // 计算总开销
           ({
               float total = 0.0;
               for (int i = 0; i < lgPtr->count; i++)
               {
                   total += lgPtr->entries[i].money;
               }
               total;
           }));
    printf("请输入操作选项：\n");
    printf("1. 添加条目\n");
    printf("2. 查看所有条目\n");
    printf("3. 关联账本文件(未开发)\n");
    printf("4. 保存到账本文件\n");
    printf("5. 替换账本已存在的条目\n");
    printf("6. 删除账本末尾条目(不支持删除指定条目)\n");
    printf("7. 保存并退出\n");
    printf("0. 不保存退出\n");
    char c = ' ';
    scanf(" %c", &c);
    while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' &&  c != '7' && c != '0')
    {
        printf("无效选项，请重新输入：\n");
        scanf(" %c", &c);
    }
    switch (c)
    {
    case '1':
        printf("添加条目\n");
        add_entry(lgPtr);
        return '1';
    case '2':
        printf("查看所有条目(press Q to quit)\n");
        print_database(lgPtr);
        while (getchar() != 'q')
            ;
        return '2';
    case '3':
        printf("关联账本文件\n");
        associate_database(lgPtr);
        return '3';
    case '4':
        printf("保存到账本文件\n");
        restore_database(lgPtr);
        return '4';
    case '5':
        printf("替换账本已存在的条目\n");
        subtitute_existing_entry(lgPtr);
        return '5';
    case '6':
        printf("删除账本末尾条目(不支持删除指定条目)\n");
        delete_last_entry(lgPtr);
        return '6';
    case '7':
        //保存退出
        restore_database(lgPtr);
        fclose(lgPtr->fp);
        return '0'; // exit code
    case '0':
        //不保存退出
        fclose(lgPtr->fp);
        return '0'; // exit code
    default:
        fprintf(stderr, "未知错误，程序退出\n");
        fclose(lgPtr->fp);
        exit(EXIT_FAILURE); // C语言没有throw语句，可以使用return或exit
    }
}