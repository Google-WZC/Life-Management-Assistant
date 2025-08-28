/**
 * @file Ledger.c
 * @platform Ubuntu24
 * @date 2025-09-28
 * @brief 记账本终端界面程序，具备增加新条目到数据库，计算总开销的功能
 * @todo 1 增加删除功能 2 regex entry 3 图形界面
 * @version 0.1
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h> // for system("clear")
#include <string.h> // for strlen

#define MAX_ENTRIES 1000
#define DATABASE_FILE_NAME ".Ledger202509.dat"

struct data
{
    char id[5];     // 0001/0
    char date[9];   // 20250828/0
    char str[100];  // 买蔬菜花费20元
    char thing[70]; // 买蔬菜花费
    float money;    // 20
    char unit[10];  // 元
};

struct database
{
    struct data entries[MAX_ENTRIES];
    int count; // when exist data id==0000, count==1
    FILE *fp;  // connect to file
};
struct database ledger = {0};

void fullfill_entry(struct database *);
void write_entry(struct database *);
void print_database(struct database *);
void init(struct database *, FILE *);
void restore_database(struct database *);
char UI_choose(struct database *);

int main()
{
    struct database *lgPtr = &ledger;

    init(lgPtr, fopen(DATABASE_FILE_NAME, "rb"));

    char run = ' '; // 32
    while (run != '0')
    {
        system("clear"); // 终端清屏
        run = UI_choose(lgPtr);
    }
}

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
    printf("0. 不保存退出\n");
    char c = ' ';
    scanf(" %c", &c);
    while (c != '1' && c != '2' && c != '3' && c != '4' && c != '0')
    {
        printf("无效选项，请重新输入：\n");
        scanf(" %c", &c);
    }
    switch (c)
    {
    case '1':
        printf("添加条目\n");
        write_entry(lgPtr);
        fullfill_entry(lgPtr);
        return '1';
    case '2':
        printf("查看所有条目(press Q to quit)\n");
        print_database(lgPtr);
        while (getchar() != 'q')
            ;
        return '2';
    case '3':
        printf("关联账本文件\n");

        return '3';
    case '4':
        printf("保存到账本文件\n");
        restore_database(lgPtr);
        return '4';
    case '0':
        printf("不保存退出\n");
        fclose(lgPtr->fp);
        return '0'; // exit code
    default:
        fprintf(stderr, "未知错误，程序退出\n");
        fclose(lgPtr->fp);
        exit(EXIT_FAILURE); // C语言没有throw语句，可以使用return或exit
    }
}

void init(struct database *lgPtr, FILE *fp)
{
    if (fp != NULL)
    {
        // 文件存在，读取数据
        lgPtr->fp = fp;
        fseek(fp, 0, SEEK_SET);                                                              // 将文件指针 fp 移动到文件的开头（偏移量为 0）
        while (fread(&lgPtr->entries[lgPtr->count], sizeof(struct data), 1, lgPtr->fp) == 1) // 匹配fread数据块
        {
            lgPtr->count++;
        }
    }
    else
    {
        printf("%s 文件不存在，是否创建该文件 y/n？\n", DATABASE_FILE_NAME);
        char ch = ' ';
        while ((ch = getchar()) != 'y')
        {
            if (ch == 'n')
            {
                fprintf(stderr, "没有指定数据文件\n");
                exit(EXIT_FAILURE);
            }
        }
        fp = fopen(DATABASE_FILE_NAME, "wb"); // w选项会覆盖并创建文件
        lgPtr->fp = fp;
    }
}

void fullfill_entry(struct database *lgPtr)
{
    // fullfill id
    snprintf(lgPtr->entries[lgPtr->count].id, 5, "%04d", lgPtr->count); // 格式化为 4 位数字，前面补零
    // fullfill date
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(lgPtr->entries[lgPtr->count].date, 9, "%Y%m%d", tm); // 格式化为 "YYYYMMDD"
    // fullfill thing,money,unit with str
    int matched = sscanf(lgPtr->entries[lgPtr->count].str, "%[^0-9]%f%[^0-9]",
                         lgPtr->entries[lgPtr->count].thing,
                         &lgPtr->entries[lgPtr->count].money,
                         lgPtr->entries[lgPtr->count].unit);
    if (matched != 3)
    {
        printf("输入格式错误，无法解析事由、金额或单位。\n");
        lgPtr->entries[lgPtr->count].thing[0] = '\0';
        lgPtr->entries[lgPtr->count].money = 0.0;
        lgPtr->entries[lgPtr->count].unit[0] = '\0';
    }
    // increment count
    lgPtr->count++;
}

void write_entry(struct database *lgPtr)
{
    printf("请输入条目：（如：买蔬菜花费20元）");
    scanf("%s", lgPtr->entries[lgPtr->count].str);
}

void print_database(struct database *lgPtr)
{
    for (int i = 0; i < lgPtr->count; i++)
    {
        printf("编号：%s\n日期：%s\n事由：%s\n内容：%s\n金额：%.2f\n单位：%s\n",
               lgPtr->entries[i].id,
               lgPtr->entries[i].date,
               lgPtr->entries[i].thing,
               lgPtr->entries[i].str,
               lgPtr->entries[i].money,
               lgPtr->entries[i].unit);
    }
}

void restore_database(struct database *lgPtr)
{
    if (lgPtr->fp != NULL)
    {
        // 先关闭再以写模式打开，覆盖写入
        fclose(lgPtr->fp);
        lgPtr->fp = fopen(DATABASE_FILE_NAME, "wb"); 
        for (int i = 0; i < lgPtr->count; i++)
        {
            fwrite(&lgPtr->entries[i], sizeof(struct data), 1, lgPtr->fp);
        }
    }
    else
    {
        printf("文件未打开，无法保存数据。\n");
    }
}