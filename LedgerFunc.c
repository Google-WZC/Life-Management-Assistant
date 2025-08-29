#include "LedgerFunc.h"

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