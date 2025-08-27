#include <stdio.h>
#include <math.h>

struct management
{
    float rate; // 月收益率
    float yearly_rate;
    double total_money; //(k)
};
struct management bank = {0};

struct buffer
{
    float monthly_rate;
    float yearly_rate;
    double monthly_money; //(k)
    int month;
    double total_money /*=monthly_money*month*/; //(k)
};
struct buffer phone = {0};

struct lifesal
{
    int age;
    double init_money;   //(k)
    double monthly_cost; //(k)
    double salary;       //(k)
    float rate;          // 理财月收益率
    float yearly_rate;   // 理财年收益率
};
struct lifesal person = {0};

void init(struct management *, struct buffer *, struct lifesal *);

int main()
{
    struct management *bankPtr = &bank;
    struct buffer *phonePtr = &phone;
    struct lifesal *personPtr = &person;

    init(bankPtr, phonePtr, personPtr);


    if (bankPtr->total_money < 0)
    {
        printf("初始资金不足支撑基金！\n");
        return -1;
    }

    while (personPtr->age < 55 /*退休年龄*/)
    {
        printf("我现在%2d岁\n", personPtr->age);
        for (int month = 1; month <= 12; month++)
        {
            bankPtr->total_money *= (1 + bankPtr->rate / 100);
            bankPtr->total_money = bankPtr->total_money + personPtr->salary + phonePtr->total_money * phonePtr->monthly_rate / 100 - personPtr->monthly_cost;
            printf("在%2d月最后一天成长理财有：%.5lf(k)\n", month, bankPtr->total_money);
        }
        personPtr->age++;
    }

    printf("\n总资产：%.6lf(w)\n", (bankPtr->total_money + phonePtr->monthly_money * phonePtr->month) / 10.);

    return 0;
}

void init(struct management *bankPtr, struct buffer *phonePtr, struct lifesal *personPtr){
    printf("请输入：年龄，初始资金(k)，每月实际开销(k)，工资(k)，           理想缓冲每月金额(k)，理想缓冲月数\n");
    scanf("%d %lf %lf %lf %lf %d", &personPtr->age, &personPtr->init_money, &personPtr->monthly_cost, &personPtr->salary,\
          &phonePtr->monthly_money, &phonePtr->month);
    printf("\"定期+\"收益率(%%)：年收益率选择y，月收益率选择m\n");
    char c='\0';
    scanf(" %c", &c);
    while (c != 'y' && c != 'm')
    {
        printf("输入错误！请重新输入(y/m)：");
        scanf(" %c", &c);
    }
    switch (c)
    {
    case 'y':
        printf("年收益率为(%%)：");
        scanf("%f", &phonePtr->yearly_rate);
        phonePtr->monthly_rate = pow(phonePtr->yearly_rate / 100 + 1, 1.0 / 12) - 1;
        phonePtr->monthly_rate *= 100;
        printf("月收益率为：%.2f%%\n", phonePtr->monthly_rate);
        break;
    case 'm':
        printf("月收益率为(%%)：");
        scanf("%f", &phonePtr->monthly_rate);
        phonePtr->yearly_rate = (pow(phonePtr->monthly_rate / 100 + 1, 12) - 1) * 100;
        printf("年收益率为：%.2f%%\n", phonePtr->yearly_rate);
        break;
    }
    printf("基金收益率(%%)：年收益率选择y，月收益率选择m\n");
    c='\0';
    scanf(" %c", &c);
    while (c != 'y' && c != 'm')
    {
        printf("输入错误！请重新输入(y/m)：");
        scanf(" %c", &c);
    }
    switch (c)
    {
    case 'y':
        printf("年收益率为(%%)：");
        scanf("%f", &personPtr->yearly_rate);
        personPtr->rate = pow(personPtr->yearly_rate / 100 + 1, 1.0 / 12) - 1;
        personPtr->rate *= 100;
        printf("月收益率为：%.2f%%\n", personPtr->rate);
        break;
    case 'm':
        printf("月收益率为(%%)：");
        scanf("%f", &personPtr->rate);
        personPtr->yearly_rate = (pow(personPtr->rate / 100 + 1, 12) - 1) * 100;
        printf("年收益率为：%.2f%%\n", personPtr->yearly_rate);
        break;
    }

    bankPtr->rate = personPtr->rate;
    phonePtr->total_money = phonePtr->monthly_money * (phonePtr->month - 1 /*留一个月的理想缓冲*/);
    bankPtr->total_money = personPtr->init_money - (phonePtr->total_money + phonePtr->monthly_money) /*6个月的生活费缓冲区*/;
}

