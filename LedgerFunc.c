#include "LedgerFunc.h"

void init(struct database *lgPtr, FILE *fp)
{
    if (fp != NULL)
    {
        // 文件存在，读取数据
        lgPtr->fp = fp;
        fseek(fp, 0, SEEK_SET);                                                              // 将文件指针 fp 移动到文件的开头（偏移量为 0）
        while (fread(&lgPtr->entries[lgPtr->count], sizeof(struct data), 1, lgPtr->fp) == 1) // 匹配fwrite数据块
        {
            lgPtr->count++;
        }
    }
    else
    {
        // 文件不存在，创建新文件
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
        fp = fopen(DATABASE_FILE_NAME, "wb"); // w选项会覆盖与创建文件
        *lgPtr = (struct database){0};
        lgPtr->fp = fp;
    }
}


void add_entry(struct database *lgPtr)
{
    write_entry(lgPtr, lgPtr->count);
    // increment count
    lgPtr->count++;
}

void delete_last_entry(struct database *lgPtr)
{
    if (lgPtr->count > 0)
    {
        char ch = ' ';
        printf("确认删除最后一条记录吗？(y/n)：");
        while ((ch = getchar()) == EOF && (ch = getchar()) == '\n')
            ;
        if ((ch = getchar()) == 'y')
        {
            lgPtr->entries[lgPtr->count - 1] = (struct data){0};
            lgPtr->count--;
        }
    }
    else
    {
        printf("没有记录可删除。\n");
    }
}

void subtitute_existing_entry(struct database *lgPtr)
{
    printf("请输入要替换的条目编号（如：0001）：");
    char id[5];
    scanf("%4s", id);
    if (atoi(id) >= lgPtr->count)
    {
        printf("编号不存在，无法替换。\n");
    }
    else
    {
        write_entry(lgPtr, atoi(id));
    }
}

void print_database(struct database *lgPtr)
{
    for (int i = 0; i < lgPtr->count; i++)
    {
        printf("编号：%d\n日期：%s\n事由：%s\n内容：%s\n金额：%.2f\n单位：%s\n",
               atoi(lgPtr->entries[i].id)+1, // 显示从1开始的编号
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

void associate_database(struct database *lgPtr)
{
    printf("请输入要关联的账本文件名（如：.Ledger202509.dat）：");
    char filename[100];
    scanf("%s", filename);
    FILE *new_fp = fopen(filename, "rb");
    init(lgPtr, new_fp);
}