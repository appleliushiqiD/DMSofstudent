#ifndef _MYHEAD_H_
#define _MYHEAD_H_

/**
 *头文件包含
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //字符串
#include <ctype.h>      //字符判断
#include <conio.h>      //密码设计
#include <Windows.h>    //system("cls") && Sleep() && PlaySound()
#include <mmsystem.h>   //playsound()
//#pragma comment(lib, "winmm.lib")//playsound()

/**
 *宏定义
*/
#define N 13
#define M 101


/**
 *结构体申明
*/
//房间信息
typedef struct room
{
    char flood[N];			//楼层
    char name[N];           //房间号
    int maxNumb;		    //最大入住人数
    int preNumb;			//当前入住人数
    struct room * next;		//节点链接
} Room;
//学生基础信息
typedef struct basic_info
{
    char numb[N];		    //学号
    char name[N];		    //姓名
    char flood[N];			//楼层
    char room[N];           //房间号
    char position[N];       //宿舍职位
} Basicf;
//学生详细信息
typedef struct detail_info
{
    char tel[N];            //电话
    char recordate[N];	    //入住时间
    char birthday[N];       //生日
    char province[N];       //省份或直辖市
    char Address[M];      //详细地址
} Detailf;
//学生信息
typedef struct student
{
    Basicf basic_info;	        //基础信息
    Detailf detail_info;        //详细信息
    struct student * next;		//节点链接
} Student;


/**
 *基础函数申明
*/
//功能菜单打印
void printMenu();
//密码检查
int checkPassword(const char password[], FILE * historyFile);
//数据导入
void importData(FILE * roomFile, FILE * stuFile, Room ** roomHead, Student ** stuHead);

/**
 *功能函数申明
 */

//1. 房间信息查看
void viewMainInfo(Room * roomHead);
//2. 学生信息查询、修改与删除
void searchStu(Room * roomHead, Student ** stuHead, FILE * historyFile);
//3. 学生信息录入
void addStudent(Room * roomHead, Student ** stuHead, FILE * historyFile);
//4. 学生信息排序
void sortStudent(Student * stuHead);
//5. 房间信息修改
void changeRoom(Room ** roomHead, Student * stuHead, FILE * historyFile);
//6. 管理员信息修改
void changeAdmin(char buildingName[], char adminName[], char adminPassword[], FILE * adminFile, FILE * historyFile);
//7. 更新文件（管理员与学生信息）
void resetFile(FILE * roomFile, FILE * stuFile, Room * roomHead, Student * stuHead);
//8. 初始化房间信息
void resetRoom(Room ** roomHead, Student ** stuHead);
//9. 操作记录查看
void printHistory(FILE * historyFile);

/**小函数*/
//查找房间
Room * searchRoom(char flood[], char roomName[], Room * roomHead);
//学号查重
Student * checkNumb(char stuNumb[], Student * stuHead);
//学生信息修改
void changeStu(Room * roomHead, Student * stuHead, Student * Sp, FILE * historyFile);
//学生人数统计
int studentCount(Student * stuHead);
//方法排序
void sortBySomeway(Student ** sortStart, int n, int (* someWay)(Student * a, Student * b));
//学号排序
int sortByNumb(Student * a, Student * b);
//姓名排序
int sortByName(Student * a, Student * b);
//房间排序
int sortByRoom(Student * a, Student * b);
//省份（直辖市）排序
int sortByProvince(Student * a, Student * b);



/**
 *基础函数定义
*/
//功能菜单打印
void printMenu()
{
    printf("\t 1. 房间信息查看\n");
    Sleep(50);
    printf("\t 2. 学生信息查询、修改与删除\n");
    Sleep(50);
    printf("\t 3. 学生信息录入\n");
    Sleep(50);
    printf("\t 4. 学生信息排序\n");
    Sleep(50);
    printf("\t 5. 房间信息修改\n");
    Sleep(50);
    printf("\t 6. 管理员信息修改\n");
    Sleep(50);
    printf("\t 7. 更新文件（管理员与学生信息）\n");
    Sleep(50);
    printf("\t 8. 初始化楼舍信息\n");
    Sleep(50);
    printf("\t 9. 操作记录查看\n");
    Sleep(50);
    printf("\t 0. 退出\n");
    Sleep(50);
    printf("\t******************************************************************\n\n");
}

//密码检查
int checkPassword(const char password[], FILE * historyFile)
{
    int i;
    char c;
    char tempPassword[N];
    printf("\t 请输入管理员密码：");
    fprintf(historyFile, "\t 请输入管理员密码：");
    for(i=0; i<N; i++)
    {
        c = getch();
        if(i == N-1)
        {
            tempPassword[N-1] = '\0';
            break;
        }
        else if(isalnum(c))
        {
            tempPassword[i] = c;
            putchar('*');
        }
        else if(c == '\n'||c == '\r')
        {
            if(i==0)
            {
                system("cls");
                return 0;
            }
            tempPassword[i] = '\0';
            printf("\n");
            break;
        }
        else if(c == '\b')
        {
            if(i>0)
            {
                i -= 2;
                printf("\b");
            }
        }
        else
        {
            i--;
        }
    }
    system("cls");
    if(strcmp(password,tempPassword) == 0)
    {
        fprintf(historyFile, "（密码不可见）\n\n");
        return 1;
    }
    else
    {
        fprintf(historyFile, "%s\n", tempPassword);
        return 0;
    }

}
//数据导入
void importData(FILE * roomFile, FILE * stuFile, Room ** roomHead, Student ** stuHead)
{
    Room * Rp = NULL;
    Student * Sp = NULL;
    //房间文件访问
    fgetc(roomFile);
    if(feof(roomFile))
    {
        *roomHead = NULL;
        return;
    }
    else
    {
        rewind(roomFile);
        *roomHead = (Room *)malloc(sizeof(Room));
        if(*roomHead == NULL)
        {
            printf("\t 内存不足！\n");
            exit(0);
        }
        //读取房间信息
        Rp = *roomHead;
        while(!feof(roomFile))
        {
            //读取信息
            fread(Rp, sizeof(Room), 1, roomFile);
            fgetc(roomFile);
            if(feof(roomFile))
            {
                Rp->next = NULL;
                break;
            }
            else//创建下一个房间节点
            {
                fseek(roomFile, -1, 1);
                Rp->next = (Room *)malloc(sizeof(Room));
                Rp = Rp->next;
                if(Rp == NULL)
                {
                    printf("\t 内存不足！\n");
                    exit(0);
                }
            }
        }
    }
    //学生文件访问
    fgetc(stuFile);
    if(feof(stuFile))
    {
        *stuHead = NULL;
    }
    else
    {
        rewind(stuFile);
        *stuHead = (Student *)malloc(sizeof(Student));
        if(*stuHead == NULL)
        {
            printf("\t 内存不足！\n");
            exit(0);
        }
        //读取学生信息
        Sp = *stuHead;
        while(!feof(stuFile))
        {
            fread(Sp, sizeof(Student), 1, stuFile);
            fgetc(stuFile);
            if(feof(stuFile))
            {
                Sp->next = NULL;
                break;
            }
            else//创建下一个学生节点
            {
                fseek(stuFile, -1, 1);
                Sp->next = (Student *)malloc(sizeof(Student));
                Sp = Sp->next;
                if(Sp == NULL)
                {
                    printf("\t 内存不足！\n");
                    exit(0);
                }
            }
        }
    }
    //关闭文件
    fclose(roomFile);
    fclose(stuFile);
}


/**
 *功能函数定义
 */
//1. 房间信息查看
void viewMainInfo(Room * roomHead)
{
    int i;
    char flood[N];
    Room * Rp;
    if(roomHead == NULL)
    {
        printf("\t 当前无房间数据，请建立后查看！\n");
        return;
    }
    Rp = roomHead;
    i = -1;
    strcpy(flood, Rp->flood);
    printf("\n\t %s  楼：\n", flood);
    Sleep(50);
    while(Rp!=NULL)
    {
        if(strcmp(flood, Rp->flood)!=0)
        {
            i = 0;
            strcpy(flood, Rp->flood);
            printf("\n\n\t %s  楼：\n", flood);
            Sleep(50);
            printf("\n\t\t ");
        }
        else
        {
            i++;
            if(i%4 == 0)
            {
                printf("\n\t\t ");
                Sleep(50);
            }
            else
            {
                printf("\t");
            }
        }
        printf("%s(%d/%d)", Rp->name, Rp->preNumb, Rp->maxNumb);
        Rp = Rp->next;
    }
    printf("\n");
}
//2. 学生信息查询、修改与删除
void searchStu(Room * roomHead, Student ** stuHead, FILE * historyFile)
{
    int findFlag;
    char c;
    char Fchoice, Schoice, Tchoice, Mchoice;
    char searchStr[N];
    Room * Rp = NULL;
    Student * Sp =NULL;
    Student * Spr = NULL;

    if(*stuHead==NULL)
    {
        printf("\t 当前无学生信息\n");
        fprintf(historyFile, "\t 当前无学生信息\n");
        return ;
    }
    do
    {
        Sleep(1000);
        system("cls");
        printf("\t******************************************************************\n\n");
        Sleep(50);
        printf("\t 选择功能：（1. 查询  0. 退出）\n\t ");
        Sleep(50);
        do
        {
            Fchoice = getchar();
            while((c=getchar())!=EOF&&c!='\n');
            if(Fchoice!='1'&&Fchoice!='0')
            {
                system("cls");
                printf("\t******************************************************************\n\n");
                Sleep(50);
                printf("\t 请正确输入\n");
                Sleep(50);
                printf("\t 选择功能：（1. 查询  0. 退出）\n\t ");
                Sleep(50);
            }
        }while(Fchoice!='1'&&Fchoice!='0');
        fprintf(historyFile, "\t 选择功能：（1. 查询  0. 退出）\n\t %c\n", Fchoice);
        if(Fchoice=='1')
        {
            do
            {
                printf("\t 选择查询方式：（1. 学号  2. 姓名  3. 电话  4. 房间号）\n\t ");
                Sleep(50);
                Schoice = getchar();
                while((c=getchar())!=EOF&&c!='\n');
                if(Schoice<'1'||Schoice>'4')
                {
                    printf("\t 请正确输入\n\t ");
                }
            }while(Schoice<'1'||Schoice>'4');
            fprintf(historyFile, "\t 选择查询方式：（1. 学号  2. 姓名  3. 电话  4. 房间号）\n\t %c\n", Schoice);
            printf("\t 请输入相关信息：");
            Sleep(50);
            fgets(searchStr, N, stdin);
            if(searchStr[0]=='\n')
            {
                printf("\t 错误输入\n");
                continue;
            }
            else if(searchStr[strlen(searchStr)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                searchStr[strlen(searchStr)-1] = '\0';
            }
            fprintf(historyFile, "\t 请输入相关信息：%s\n", searchStr);
            if(Schoice=='2'||Schoice=='4')//选择修改
            {
                do
                {
                    printf("\t 选择信息打印方式：（1. 简单  2. 详细）\n\t ");
                    Sleep(50);
                    Tchoice = getchar();
                    while((c=getchar())!=EOF&&c!='\n');
                    if(Tchoice!='1'&&Tchoice!='2')
                    {
                        printf("\t 输错误入\n");
                    }
                }while(Tchoice!='1'&&Tchoice!='2');
                fprintf(historyFile, "\t 选择信息打印方式：（1. 简单  2. 详细）\n\t %c\n", Tchoice);
                if(Tchoice=='1')
                {
                    printf("\t %-14s%-14s%-14s%-14s%-14s\n\n",
                            "姓名", "学号", "楼层", "房间号", "职位");
                    Sleep(50);
                    fprintf(historyFile, "\t %-14s%-14s%-14s%-14s%-14s\n\n",
                            "姓名", "学号", "楼层", "房间号", "职位");
                }
                else
                {
                    printf("\t %-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n\n",
                            "姓名", "学号", "楼层", "房间号", "职位",
                            "电话", "入住时间", "生日", "省份", "详细地址");
                    Sleep(50);
                    fprintf(historyFile, "\t %-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n\n",
                            "姓名", "学号", "楼层", "房间号", "职位",
                            "电话", "入住时间", "生日", "省份", "详细地址");
                }
                Sp = *stuHead;
                Spr = Sp;
                findFlag = 0;
                while(Sp!=NULL)
                {
                    if(Schoice=='2')
                    {
                        if(strcmp(Sp->basic_info.name, searchStr)==0)
                        {
                            findFlag = 1;
                        }
                        else
                        {
                            findFlag = 0;

                        }
                    }
                    else if(Schoice=='4')
                    {
                        if(strcmp(Sp->basic_info.room, searchStr)==0)
                        {
                            findFlag = 1;
                        }
                        else
                        {
                            findFlag = 0;
                        }
                    }
                    //查询结果处理
                    if(findFlag==1)
                    {
                        if(Tchoice=='1')
                        {
                            printf("\t %-14s%-14s%-14s%-14s%-14s\n",
                                    Sp->basic_info.name,
                                    Sp->basic_info.numb,
                                    Sp->basic_info.flood,
                                    Sp->basic_info.room,
                                    Sp->basic_info.position);
                            Sleep(50);
                            fprintf(historyFile, "\t %-14s%-14s%-14s%-14s%-14s\n",
                                    Sp->basic_info.name,
                                    Sp->basic_info.numb,
                                    Sp->basic_info.flood,
                                    Sp->basic_info.room,
                                    Sp->basic_info.position);
                        }
                        else if(Tchoice=='2')
                        {
                            printf("\t %-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n",
                                    Sp->basic_info.name,
                                    Sp->basic_info.numb,
                                    Sp->basic_info.flood,
                                    Sp->basic_info.room,
                                    Sp->basic_info.position,
                                    Sp->detail_info.tel,
                                    Sp->detail_info.recordate,
                                    Sp->detail_info.birthday,
                                    Sp->detail_info.province,
                                    Sp->detail_info.Address);
                            Sleep(50);
                            fprintf(historyFile, "\t %-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%-12s%s\n\n",
                                    Sp->basic_info.name,
                                    Sp->basic_info.numb,
                                    Sp->basic_info.flood,
                                    Sp->basic_info.room,
                                    Sp->basic_info.position,
                                    Sp->detail_info.tel,
                                    Sp->detail_info.recordate,
                                    Sp->detail_info.birthday,
                                    Sp->detail_info.province,
                                    Sp->detail_info.Address);
                        }
                        do
                        {
                            printf("\t 功能选择：（1. 修改  2. 删除  3. 查找下一个  0. 退出）\n\t ");
                            Sleep(50);
                            Mchoice = getchar();
                            if(Mchoice<'0'||Mchoice>'3')
                            {
                                printf("\t 错误输入， 请重新输入\n");
                            }
                        }while(Mchoice<'0'||Mchoice>'3');
                        fprintf(historyFile, "\t 功能选择：（1. 修改  2. 删除  3. 查找下一个  0. 退出）\n\t %c\n", Mchoice);
                        if(Mchoice=='0')
                        {
                            return ;
                        }
                        else if(Mchoice=='3')
                        {
                            Spr = Sp;
                            Sp = Sp->next;
                            continue;
                        }
                        else if(Mchoice=='1')
                        {
                            changeStu(roomHead, *stuHead, Sp, historyFile);
                            break;
                        }
                        else if(Mchoice=='2')
                        {
                            //更新房间信息
                            Rp = searchRoom(Sp->basic_info.flood, Sp->basic_info.room, roomHead);
                            Rp->preNumb -= 1;
                            //更新链表
                            if(Sp==(*stuHead))
                            {
                                Spr = *stuHead;
                                *stuHead = (*stuHead)->next;
                                free(Spr);
                            }
                            else
                            {
                                Spr->next = Sp->next;
                                free(Sp);
                            }
                            printf("\t 删除成功\n\n");
                            fprintf(historyFile, "\t 删除成功\n\n");
                            break;
                        }
                    }
                    else
                    {
                        Spr = Sp;
                        Sp = Sp->next;
                    }

                }//遍历循环
                if(Sp==NULL)
                {
                    printf("\t 未找到\n\n");
                    fprintf(historyFile, "\t 未找到\n\n");
                    getchar();
                    continue;
                }
            }//房间查找
            else if(Schoice=='1'||Schoice=='3')//唯一修改
            {
                Sp = *stuHead;
                findFlag = 0;
                while(Sp!=NULL)
                {
                    if(Schoice=='1')//学号
                    {
                        if(strcmp(Sp->basic_info.numb, searchStr)==0)
                        {
                            findFlag = 1;
                            break;
                        }
                        else
                        {
                            findFlag = 0;
                            Spr = Sp;
                            Sp = Sp->next;
                        }
                    }
                    else if(Schoice=='3')//电话
                    {
                        if(strcmp(Sp->detail_info.tel, searchStr)==0)
                        {
                            findFlag = 1;
                            break;
                        }
                        else
                        {
                            findFlag = 0;
                            Spr = Sp;
                            Sp = Sp->next;
                        }
                    }
                }
                if(findFlag==1)
                {
                    printf("\t 找到符合条件的学生\n");
                    Sleep(50);
                    fprintf(historyFile, "\t 找到了符合条件的学生\n");
                    printf("\t 基础信息：\n\t\t 姓名：%s\n\t\t 学号：%s\n\t\t 楼层：%s\n\t\t 房间号：%s\n\t\t 职位：%s\n",
                            Sp->basic_info.name, Sp->basic_info.numb, Sp->basic_info.flood,
                            Sp->basic_info.room, Sp->basic_info.position);
                    Sleep(100);
                    printf("\t 详细信息：\n\t\t 电话：%s\n\t\t 生日：%s\n\t\t 入住时间：%s\n\t\t 省份（直辖市）：%s\n\t\t 详细地址：%s\n\n",
                            Sp->detail_info.tel, Sp->detail_info.birthday, Sp->detail_info.recordate,
                            Sp->detail_info.province, Sp->detail_info.Address);
                    Sleep(100);
                    do
                    {
                        printf("\t 选择功能：（1. 修改  2. 删除  0. 退出）\n\t ");
                        Sleep(50);
                        Mchoice = getchar();
                        while((c=getchar())!=EOF&&c!='\n');
                        if(Mchoice<'0'||Mchoice>'2')
                        {
                            printf("\t 错误输入，请重新选择\n");
                        }
                    }while(Mchoice<'0'||Mchoice>'2');
                    fprintf(historyFile, "\t 选择功能：（1. 修改  2. 删除  0. 退出）\n\t %c", Mchoice);
                    if(Mchoice=='0')
                    {
                        return;
                    }
                    else if(Mchoice=='1')
                    {
                        changeStu(roomHead, *stuHead, Sp, historyFile);
                        continue;
                    }
                    else if(Mchoice=='2')
                    {
                        //更新房间信息
                        Rp = searchRoom(Sp->basic_info.flood, Sp->basic_info.room, roomHead);
                        Rp->preNumb -= 1;
                        //更新链表
                        if(Sp==(*stuHead))
                        {
                            Spr = *stuHead;
                            *stuHead = (*stuHead)->next;
                            free(Spr);
                        }
                        else
                        {
                            Spr->next = Sp->next;
                            free(Sp);
                        }
                        printf("\t 删除成功\n\n");
                        fprintf(historyFile, "\t 删除成功\n\n");
                        break;
                    }
                }
                else
                {
                    printf("\t 未找到\n\n");
                    fprintf(historyFile, "\t 未找到\n\n");
                    continue;
                }
            }
        }
        else
        {
            break;
        }
    }while(Fchoice=='1');
}
//3. 学生信息录入
void addStudent(Room * roomHead, Student ** stuHead, FILE * historyFile)
{
    char c;
    char Fchoice, Schoice;
    char flood[N];
    char roomName[N];
    Room * Rp = NULL;
    Student * Sp = NULL;
    Student * Spr = NULL;
    Student tempMemb;

    do
    {
        /**功能选择*/
        do
        {
            printf("\t 功能选择：（1. 录入  0. 退出）\n\t ");
            Sleep(50);
            Fchoice = getchar();
            while((c=getchar())!=EOF&&c!='\n');
            if(Fchoice!='1'&&Fchoice!='0')
            {
                system("cls");
                printf("\t******************************************************************\n\n");
                Sleep(50);
                printf("\t 请正确输入\n");
                Sleep(50);
            }
        }while(Fchoice!='1'&&Fchoice!='0');
        fprintf(historyFile, "\t 功能选择：（1. 录入  0. 退出）\n\t %c\n", Fchoice);
        if(Fchoice=='1')
        {
            RoomChange:
            printf("\t 请输入目标房间楼层：");
            Sleep(50);
            fgets(flood, N, stdin);
            if(flood[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(flood[strlen(flood)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                flood[strlen(flood)-1] = '\0';
            }
            fprintf(historyFile, "\t 请输入目标房间楼层：%s\n", flood);
            printf("\t 请输入目标房间号：");
            Sleep(50);
            fgets(roomName, N, stdin);
            if(roomName[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(roomName[strlen(roomName)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                roomName[strlen(roomName)-1] = '\0';
            }
            fprintf(historyFile, "\t 请输入目标房间号：%s\n", roomName);
            Rp = searchRoom(flood, roomName, roomHead);
            if(Rp==NULL)
            {
                printf(" \t 未找到匹配房间，请检查后重试\n");
                Sleep(50);
                fprintf(historyFile, " \t 未找到匹配房间，请检查后重试\n");
                continue;
            }
            if(Rp->preNumb == Rp->maxNumb)
            {
                printf("\t 当前房间已满，可选择新的房间\n");
                Sleep(50);
                fprintf(historyFile, "\t 当前房间已满，可选择新的房间\n");
                continue;
            }
            else
            {
                do//同房间输入
                {
                    printf("\t 输入学生信息：\n");
                    Sleep(50);
                    fprintf(historyFile, "\t 输入学生信息：\n");
                    do
                    {
                        printf("\t 学号：");
                        Sleep(50);
                        fgets(tempMemb.basic_info.numb, N, stdin);
                        if(tempMemb.basic_info.numb[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.basic_info.numb[strlen(tempMemb.basic_info.numb)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.basic_info.numb[strlen(tempMemb.basic_info.numb)-1] = '\0';
                        }
                        //学号查重
                        Sp = checkNumb(tempMemb.basic_info.numb, *stuHead);
                        if(Sp!=NULL)
                        {
                            printf("\t 该学号学生已存在\n");
                            Sleep(50);
                            printf("\t 基础信息：\n\t\t 姓名：%s\n\t\t 学号：%s\n\t\t 楼层：%s\n\t\t 房间号：%s\n\t\t 职位：%s\n",
                                    Sp->basic_info.name, Sp->basic_info.numb, Sp->basic_info.flood,
                                    Sp->basic_info.room, Sp->basic_info.position);
                            Sleep(100);
                            printf("\t 详细信息：\n\t\t 电话：%s\n\t\t 生日：%s\n\t\t 入住时间：%s\n\t\t 省份（直辖市）：%s\n\t\t 详细地址：%s\n\n",
                                    Sp->detail_info.tel, Sp->detail_info.birthday, Sp->detail_info.recordate,
                                    Sp->detail_info.province, Sp->detail_info.Address);
                            Sleep(100);
                            break;
                        }
                        printf("\t 姓名：");
                        Sleep(50);
                        fgets(tempMemb.basic_info.name, N, stdin);
                        if(tempMemb.basic_info.name[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.basic_info.name[strlen(tempMemb.basic_info.name)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.basic_info.name[strlen(tempMemb.basic_info.name)-1] = '\0';
                        }
                        printf("\t 宿舍职位：");
                        Sleep(50);
                        fgets(tempMemb.basic_info.position , N, stdin);
                        if(tempMemb.basic_info.position[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.basic_info.position[strlen(tempMemb.basic_info.position)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.basic_info.position[strlen(tempMemb.basic_info.position)-1] = '\0';
                        }
                        printf("\t 电话：");
                        Sleep(50);
                        fgets(tempMemb.detail_info.tel, N, stdin);
                        if(tempMemb.detail_info.tel[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.detail_info.tel[strlen(tempMemb.detail_info.tel)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.detail_info.tel[strlen(tempMemb.detail_info.tel)-1] = '\0';
                        }
                        printf("\t 入住时间：");
                        Sleep(50);
                        fgets(tempMemb.detail_info.recordate, N, stdin);
                        if(tempMemb.detail_info.recordate[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.detail_info.recordate[strlen(tempMemb.detail_info.recordate)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.detail_info.recordate[strlen(tempMemb.detail_info.recordate)-1] = '\0';
                        }
                        printf("\t 生日：");
                        Sleep(50);
                        fgets(tempMemb.detail_info.birthday, N, stdin);
                        if(tempMemb.detail_info.birthday[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.detail_info.birthday[strlen(tempMemb.detail_info.birthday)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.detail_info.birthday[strlen(tempMemb.detail_info.birthday)-1] = '\0';
                        }
                        printf("\t 省份（直辖市）：");
                        Sleep(50);
                        fgets(tempMemb.detail_info.province, N, stdin);
                        if(tempMemb.detail_info.province[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.detail_info.province[strlen(tempMemb.detail_info.province)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.detail_info.province[strlen(tempMemb.detail_info.province)-1] = '\0';
                        }
                        printf("\t 详细地址：");
                        Sleep(50);
                        fgets(tempMemb.detail_info.Address, M, stdin);
                        if(tempMemb.detail_info.Address[0] == '\n')
                        {
                            system("cls");
                            printf("\t******************************************************************\n\n");
                            Sleep(50);
                            printf("\t 输入错误，重新输入：\n");
                            Sleep(50);
                            printf("\t 输入学生信息：\n");
                            Sleep(50);
                            continue;
                        }
                        else if(tempMemb.detail_info.Address[strlen(tempMemb.detail_info.Address)-1]!='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            tempMemb.detail_info.Address[strlen(tempMemb.detail_info.Address)-1] = '\0';
                        }
                        Spr = malloc(sizeof(Student));
                        if(Spr==NULL)
                        {
                            printf("\t 内存不足\n");
                            exit(0);
                        }
                        else
                        {
                            strcpy(tempMemb.basic_info.flood, flood);
                            strcpy(tempMemb.basic_info.room, roomName);
                            tempMemb.next = NULL;
                            *Spr = tempMemb;
                            Spr->next = *stuHead;
                            *stuHead = Spr;
                            Rp->preNumb += 1;
                            printf("\t 登记成功\n\n");
                            Sleep(50);
                            fprintf(historyFile, "\t 登记了学号为：%s的学生\n\n", Spr->basic_info.numb);
                        }
                    }while(Sp!=NULL);
                    printf("\t 房间转换选择：（1. 当前房间继续 2. 选择信新房间 0. 退出）\n\t ");
                    Sleep(50);
                    fprintf(historyFile, "\t 房间转换选择：（1. 当前房间继续 2. 选择信新房间 0. 退出）\n");
                    Schoice = getchar();
                    while((c=getchar())!=EOF&&c!='\n');
                    fprintf(historyFile, "\t %c\n", Schoice);
                    if(Schoice<'0'||Schoice>'2')
                    {
                        printf("\t 错误输入\n");
                        Sleep(50);
                        break;
                    }
                }while(Schoice=='1');
                if(Schoice=='2')
                {
                    goto RoomChange;
                }
                else if(Schoice=='0')
                {
                    return ;
                }
            }
        }
    }while(Fchoice=='1');
}
//4. 学生信息排序
void sortStudent(Student * stuHead)
{
    int i;
    int totalStu;
    char Fchoice, Schoice;
    char c;
    Student ** sortStart = NULL;
    Student * Sp = NULL;


    if(stuHead == NULL)
    {
        printf("\t 当前无学生信息\n");
        Sleep(50);
        return ;
    }
    Sp = stuHead;
    totalStu = studentCount(stuHead);
    if(totalStu==1)
    {
        printf("\t 当前仅有一位学生\n");
        Sleep(50);
        printf("\t %-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n\n",
               "姓名", "学号", "楼层", "房间号", "职位",
               "电话", "入住时间", "生日", "省份", "详细地址");
        Sleep(50);
        printf("\t %-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n",
                Sp->basic_info.name,
                Sp->basic_info.numb,
                Sp->basic_info.flood,
                Sp->basic_info.room,
                Sp->basic_info.position,
                Sp->detail_info.tel,
                Sp->detail_info.recordate,
                Sp->detail_info.birthday,
                Sp->detail_info.province,
                Sp->detail_info.Address);
        Sleep(50);
        return;
    }
    sortStart = (Student **)calloc(totalStu, sizeof(Student *));
    for(i=0;i<totalStu&&Sp!=NULL;i++)
    {
        *(sortStart + i) = Sp;
        Sp = Sp->next;
    }
    if(i!=totalStu)
    {
        printf("\t 系统错误\n");
        Sleep(50);
        return ;
    }
    do
    {
        printf("\t 选择排序条件：（1. 学号 2. 姓名 3. 房间号 4. 省份）\n\t ");
        Sleep(50);
        Fchoice = getchar();
        while((c=getchar())!=EOF&&c!='\n');
        if(Fchoice<'1'||Fchoice>'4')
        {
            printf("\t 错误输入，重新选择\n");
            Sleep(50);
        }
    }while(Fchoice<'1'||Fchoice>'4');
    do
    {
        printf("\t 选择信息显示方式：（1. 简单 2. 详细）\n\t ");
        Sleep(50);
        Schoice = getchar();
        while((c=getchar())!=EOF&&c!='\n');
        if(Schoice!='1'&&Schoice!='2')
        {
            printf("\t 错误输入，重新选择\n");
            Sleep(50);
        }
    }while(Schoice!='1'&&Schoice!='2');
    if(Fchoice=='1')
    {
        sortBySomeway(sortStart, totalStu, sortByNumb);
    }
    else if(Fchoice=='2')
    {
        sortBySomeway(sortStart, totalStu, sortByName);
    }
    else if(Fchoice=='3')
    {
        sortBySomeway(sortStart, totalStu, sortByRoom);
    }
    else if(Fchoice=='4')
    {
        sortBySomeway(sortStart, totalStu, sortByProvince);
    }
    //打印信息
    system("cls");
    printf("\t******************************************************************\n\n");
    if(Schoice=='1')
    {
        printf("\t %-6s%-14s%-14s%-14s%-14s%-14s\n\n",
               "序号", "姓名", "学号", "楼层", "房间号", "职位");
        Sleep(50);
    }
    else if(Schoice=='2')
    {
        printf("\t %-6s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n\n",
               "序号", "姓名", "学号", "楼层", "房间号", "职位",
               "电话", "入住时间", "生日", "省份", "详细地址");
        Sleep(50);
    }
    for(i=0;i<totalStu;i++)
    {
        if(Schoice=='1')
        {
            printf("\t %-6d%-14s%-14s%-14s%-14s%-14s\n",
                    i+1,
                    (*(sortStart+i))->basic_info.name,
                    (*(sortStart+i))->basic_info.numb,
                    (*(sortStart+i))->basic_info.flood,
                    (*(sortStart+i))->basic_info.room,
                    (*(sortStart+i))->basic_info.position);
        }
        else if(Schoice=='2')
        {
            printf("\t %-6d%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%-14s%s\n",
                    i+1,
                    (*(sortStart+i))->basic_info.name,
                    (*(sortStart+i))->basic_info.numb,
                    (*(sortStart+i))->basic_info.flood,
                    (*(sortStart+i))->basic_info.room,
                    (*(sortStart+i))->basic_info.position,
                    (*(sortStart+i))->detail_info.tel,
                    (*(sortStart+i))->detail_info.recordate,
                    (*(sortStart+i))->detail_info.birthday,
                    (*(sortStart+i))->detail_info.province,
                    (*(sortStart+i))->detail_info.Address);
        }
    }
    free(sortStart);
}
//5. 房间信息修改
void changeRoom(Room ** roomHead, Student * stuHead, FILE * historyFile)
{
    char c;
    char Fchoice;
    char flood[N];
    char roomName[N];
    Room * Rp = NULL;
    Room * Rpr = NULL;
    Room * Rpm = NULL;
    Student * Sp = NULL;
    do
    {
        do
        {
            printf("\t 选择功能：（1. 修改  0. 返回主菜单）\n\t ");
            Fchoice = getchar();
            while((c=getchar())!=EOF&&c!='\n');
            if(Fchoice!='0'&&Fchoice!='1')
            {
                system("cls");
                printf("\t******************************************************************\n\n");
                printf("\t 请正确输入\n");
            }
        }while(Fchoice!='0'&&Fchoice!='1');
        fprintf(historyFile, "\t 选择功能：（1. 修改  0. 返回主菜单）\n\t %c\n", Fchoice);
        if(Fchoice=='0')
        {
            return ;
        }
        else if(Fchoice=='1')//修改
        {
            printf("\t 请输入目标房间楼层：");
            fprintf(historyFile, "\t 请输入目标房间楼层：");
            fgets(flood, N, stdin);
            if(flood[0] == '\n')
            {
                printf("\t 输入错误\n");
                continue;
            }
            else
            {
                if(flood[strlen(flood)-1]!='\n')
                {
                    while((c=getchar())!=EOF&&c!='\n');
                }
                else
                {
                    flood[strlen(flood)-1] = '\0';
                }
                fprintf(historyFile, "%s\n", flood);
                Rp = * roomHead;
                while(Rp!=NULL)
                {
                    if(strcmp(flood, Rp->flood)==0)
                    {
                        break;
                    }
                    else
                    {
                        Rp = Rp->next;
                    }
                }
                if(Rp==NULL)
                {
                    printf("\t 未找到\n");
                    fprintf(historyFile, "\t 未找到\n");
                    continue;
                }
                else
                {
                    Rpr = Rp;
                    printf("\t 请输入房间号：");
                    fprintf(historyFile, "\t 请输入房间号：");
                    fgets(roomName, N, stdin);
                    if(flood[0] == '\n')
                    {
                        printf("\t 输入错误\n");
                        continue;
                    }
                    else//查找对应楼层所有房间
                    {
                        if(roomName[strlen(roomName)-1] !='\n')
                        {
                            while((c=getchar())!=EOF&&c!='\n');
                        }
                        else
                        {
                            roomName[strlen(roomName)-1] = '\0';
                        }
                        fprintf(historyFile, "%s\n", roomName);
                        while(strcmp(Rpr->flood, flood)==0)
                        {
                            if(Rpr==NULL)
                            {
                                break;
                            }
                            else if(strcmp(Rpr->name, roomName)==0)
                            {
                                break;
                            }
                            else
                            {
                                Rpr = Rpr->next;
                                if(Rpr==NULL)
                                {
                                    break;
                                }
                            }
                        }
                        if(Rpr==NULL||strcmp(Rpr->flood, flood)!=0)
                        {
                            printf("\t 未找到\n");
                            fprintf(historyFile, "\t 未找到\n");
                            continue;
                        }
                        else//找到对应房间
                        {
                            char tempFchoice;
                            printf("\n\t 房间信息：\n");
                            fprintf(historyFile, "\n\t 房间信息：\n");
                            printf("\t\t 楼层：%s\n\t\t 房间号：%s\n\t\t 最大入住人数：%d\n\t\t 当前入住人数：%d\n\n",
                                    Rpr->flood, Rpr->name, Rpr->maxNumb, Rpr->preNumb);
                            fprintf(historyFile,
                                    "\t\t 楼层：%s\n\t\t 房间号：%s\n\t\t 最大入住人数：%d\n\t\t 当前入住人数：%d\n\n",
                                    Rpr->flood, Rpr->name, Rpr->maxNumb, Rpr->preNumb);
                            printf("\t 选择你要修改的内容：（1. 房间号  2. 最大入住人数）\n\t ");
                            fprintf(historyFile, "\t 选择你要修改的内容：（1. 房间号  2. 最大入住人数）\n\t ");
                            do
                            {
                                tempFchoice = getchar();
                                while((c=getchar())!=EOF&&c!='\n');
                                if(tempFchoice!='1'&&tempFchoice!='2')
                                {
                                    printf("\t 错误输入，重新输入：");
                                }
                            }while(tempFchoice!='1'&&tempFchoice!='2');
                            fprintf(historyFile, "%c\n", tempFchoice);
                            if(tempFchoice=='1')
                            {
                                char tempName[N];
                                printf("\t 请输入新的房间号：");
                                fprintf(historyFile, "\t 请输入新的房间号：");
                                fgets(tempName, N, stdin);
                                if(tempName[0] == '\n')
                                {
                                    printf("\t 错误输入\n");
                                    fprintf(historyFile, "\t 错误输入\n");
                                    continue;
                                }
                                else if(tempName[strlen(tempName)-1]!='\n')
                                {
                                    while((c=getchar())!=EOF&&c!='\n');
                                }
                                else
                                {
                                    tempName[strlen(tempName)-1] = '\0';
                                }
                                fprintf(historyFile, "%s\n", tempName);
                                //房间名重复检查
                                Rpm = Rp;
                                while(strcmp(Rp->flood, Rpm->flood)==0&&Rpm !=NULL)
                                {
                                    if(Rpm==Rpr)
                                    {
                                        Rpm = Rpm->next;
                                        continue;
                                    }
                                    else if(strcmp(tempName, Rpm->name)==0)
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        Rpm = Rpm->next;
                                    }
                                }
                                //修改判断
                                if(Rpm==NULL||strcmp(Rp->flood, Rpm->flood)!=0)
                                {
                                    //同步修改学生信息
                                    if(Rpr->preNumb!=0)
                                    {
                                        Sp = stuHead;
                                        while(Sp!=NULL)
                                        {
                                            if(strcmp(Sp->basic_info.flood, Rpr->flood)==0)
                                            {
                                                if(strcmp(Sp->basic_info.room, Rpr->name)==0)
                                                {
                                                    strcpy(Sp->basic_info.room, tempName);
                                                }
                                                else
                                                {
                                                    Sp = Sp->next;
                                                }
                                            }
                                            else
                                            {
                                                Sp = Sp->next;
                                            }
                                        }
                                        printf("\t 学生信息同步修改成功\n");
                                        fprintf(historyFile, "\t 学生信息同步修改成功\n");
                                    }
                                    strcpy(Rpr->name, tempName);
                                    printf("\t 房间号修改成功\n");
                                    fprintf(historyFile, "\t 房间号修改成功\n");
                                }
                                else if(strcmp(tempName, Rpm->name)==0)
                                {
                                    printf("\t 该名称同楼层已存在\n");
                                    fprintf(historyFile, "\t 该名称在同楼层已存在\n");
                                }
                            }
                            else if(tempFchoice=='2')
                            {
                                char tempMax;
                                do
                                {
                                    printf("\t 请输入房间新的最大入住人数：");
                                    tempMax = getchar();
                                    fprintf(historyFile, "\t 请输入房间新的最大入住人数：%c\n", tempMax);
                                    while((c=getchar())!=EOF&&c!='\n');
                                    if(tempMax<'0'||tempMax>'8')
                                    {
                                        printf("\t 请合理输入\n");
                                        fprintf(historyFile, "\t 请合理输入\n");
                                    }
                                    else if((tempMax-'0')<Rpr->preNumb)
                                    {
                                        printf("\t 当前房间已有 %c 人，请删除人员后修改\n", Rpr->preNumb);
                                        fprintf(historyFile, "\t 当前房间已有 %c 人，请删除人员后修改\n", Rpr->preNumb);
                                        break;
                                    }
                                    else
                                    {
                                        Rpr->maxNumb = tempMax - '0';
                                        printf("\t 修改成功\n");
                                        fprintf(historyFile, "\t 修改成功\n");
                                        break;
                                    }
                                }while(tempMax<'0'||tempMax>'8');
                            }
                        }
                    }
                }
            }
        }
        printf("\n\t 任意键继续");
        getchar();
        system("cls");
        printf("\t******************************************************************\n\n");
    }while(Fchoice=='1');
}
//6. 楼舍、管理员信息修改
void changeAdmin(char buildingName[], char adminName[], char adminPassword[], FILE * adminFile, FILE * historyFile)
{
    char c;
    int i;
    //设置宿舍楼名
    printf("\t 请输入楼舍名称：");
    Sleep(50);
    fprintf(historyFile, "\t 请输入楼舍名称：");
    fgets(buildingName,N,stdin);
    //未输入
    if(buildingName[0] == '\0'||buildingName[0] == '\n')
    {
        printf("\t 未输入，初始化为“未命名”\n");
        Sleep(50);
        strcpy(buildingName,"未命名");
    }
    else if(buildingName[strlen(buildingName)-1] != '\n')
    {
        while((c=getchar())!=EOF&&c!='\n');
    }
    else
    {
        buildingName[strlen(buildingName)-1] = '\0';
    }
    fprintf(historyFile, "%s\n", buildingName);
    //设置管理员名称
    printf("\t 请设置管理员姓名（admin）：");
    Sleep(50);
    fprintf(historyFile, "\t 请设置管理员姓名（admin）：");
    fgets(adminName,N,stdin);
    //未输入
    if(adminName[0] == '\0'||adminName[0] == '\n')
    {
        printf("\t 未输入，初始化为“admin”\n");
        Sleep(50);
        strcpy(adminName,"admin");
    }
    else if(adminName[strlen(adminName)-1] != '\n')
    {
        while((c=getchar())!=EOF&&c!='\n');
    }
    else
    {
        adminName[strlen(adminName)-1] = '\0';
    }
    fprintf(historyFile, "%s\n", adminName);
    //设置管理员密码
    printf("\t 请设置管理员初始密码（123456）：");
    Sleep(50);
    fprintf(historyFile, "\t 请设置管理员初始密码（123456）：（密码不可见）\n");
    for(i=0; i<N; i++)
    {
        c = getch();
        if(i == N-1)
        {
            adminPassword[i] = '\0';
            break;
        }
        else if(isalnum(c))
        {
            adminPassword[i] = c;
            putchar('*');
        }
        else if(c == '\n'||c == '\r')
        {
            //未输入
            if(i==0)
            {
                printf("\t 未输入，初始化为“123456”\n");
                Sleep(50);
                strcpy(adminPassword,"123456");
            }
            else
            {
                adminPassword[i] = '\0';
            }
            printf("\n");
            break;
        }
        else if(c == '\b')
        {
            if(i>0)
            {
                i -= 2;
                printf("\b");
            }
        }
        else
        {
            i--;
        }
    }
    //更新文件
    fprintf(adminFile, "%s\n", buildingName);
    fprintf(adminFile, "%s\n", adminName);
    fprintf(adminFile, "%s\n", adminPassword);
    fclose(adminFile);
}
//7. 更新文件（管理员与学生信息）
void resetFile(FILE * roomFile, FILE * stuFile, Room * roomHead, Student * stuHead)
{
    Room * Rp;
    Student * Sp;
    Rp = roomHead;
    //写入文件
    while(Rp!=NULL)
    {
        fwrite(Rp, sizeof(Room), 1, roomFile);
        Rp = Rp->next;
    }
    fclose(roomFile);
    Sp = stuHead;
    //写入文件
    while(Sp!=NULL)
    {
        fwrite(Sp, sizeof(Student), 1, stuFile);
        Sp = Sp->next;
    }
    fclose(stuFile);
}
//8. 初始化房间信息
void resetRoom(Room ** roomHead, Student ** stuHead)
{
    int i,j;
    int flood, room, numb;
    char c;
    Room * Rp, *Rpr;
    Student * Sp, *Spr;

    //清空链表
    Rp = *roomHead;
    while(Rp!=NULL)
    {
        Rpr = Rp;
        Rp = Rp->next;
        free(Rpr);
    }
    Sp = (*stuHead);
    while(Sp!=NULL)
    {
        Spr = Sp;
        Sp = Sp->next;
        free(Spr);

    }
    //输入检查
    printf("\t 请输入楼层数（1-20）、房间数（1-50）与房间人数极限（1-8）：（例：7 30 6）\n\t ");
    do
    {
        if(scanf("%d %d %d", &flood, &room, &numb)!=3)
        {
            while((c=getchar())!=EOF&&c!='\n');
            printf("\t 输入错误！重新输入：");
            continue;
        }
        else if((flood<=0||flood>20)||(room<=0||room>50)||(numb<=0||numb>8))
        {
            while((c=getchar())!=EOF&&c!='\n');
            printf("\t 请合理输入数值：");
            continue;
        }
        getchar();
    }
    while((flood<=0||flood>20)||(room<=0||room>50)||(numb<=0||numb>8));
    Rpr = NULL;
    //初始化房间信息
    for(i=1; i<=flood; i++)
    {
        for(j=1; j<=room; j++)
        {
            Rp = (Room *)malloc(sizeof(Room));
            if(Rp == NULL)
            {
                printf("\t 内存不足\n");
                exit(0);
            }
            //楼层
            Rp->flood[0] = '0' + (i/10);
            Rp->flood[1] = '0' + (i%10);
            Rp->flood[2] = '\0';
            //房间号
            Rp->name[0] = Rp->flood[0];
            Rp->name[1] = Rp->flood[1];
            Rp->name[2] = '0';
            Rp->name[3] = '0' + (j/10);
            Rp->name[4] = '0' + (j%10);
            Rp->name[5] = '\0';
            //其他
            Rp->maxNumb = numb;
            Rp->preNumb = 0;
            Rp->next = NULL;
            if(i==1&&j==1)
            {
                *roomHead = Rp;
            }
            else
            {
                Rpr->next = Rp;
            }
            Rpr = Rp;
        }
    }
}
//9. 操作记录查看
void printHistory(FILE * historyFile)
{
    char c;
    fseek(historyFile, 0, 0);       //移动光标
    while(!feof(historyFile))
    {
        c = fgetc(historyFile);
        if(c == '\n'||c == '\r')
        {
            Sleep(50);
        }
        putchar(c);
    }
}
//查找房间
Room * searchRoom(char flood[], char roomName[], Room * roomHead)
{
    Room * Rp = NULL;
    Rp = roomHead;
    while(Rp != NULL)
    {
        if(strcmp(Rp->flood, flood)==0)
        {
            if(strcmp(Rp->name, roomName)==0)
            {
                return Rp;
            }
            else
            {
                Rp = Rp->next;
            }
        }
        else
        {
            Rp = Rp->next;
        }
    }
    if(Rp==NULL)
    {
        return NULL;
    }
    return NULL;
}
//学号查重
Student * checkNumb(char stuNumb[], Student * stuHead)
{
    Student * Sp = NULL;
    Sp = stuHead;
    while(Sp!=NULL)
    {
        if(strcmp(Sp->basic_info.numb, stuNumb)==0)
        {
            return Sp;
        }
        else
        {
            Sp = Sp->next;
        }
    }
    if(Sp==NULL)
    {
        return NULL;
    }
    return NULL;
}
//学生信息修改
void changeStu(Room * roomHead, Student * stuHead, Student * Sp, FILE * historyFile)
{
    char c;
    char Fchoice;
    char flood[N];
    char roomName[N];
    Room * Rp = NULL;

    do
    {
        printf("\t 选择修改的内容：（1. 房间  2. 电话  3. 住址）\n\t ");
        Sleep(50);
        Fchoice = getchar();
        while((c=getchar())!=EOF&&c!='\n');
        if(Fchoice<'1'||Fchoice>'3')
        {
            printf("\t 错误输入，请重新输入\n");
            Sleep(50);
        }
    }while(Fchoice<'1'||Fchoice>'3');
    fprintf(historyFile, "\t 选择修改的内容：（1. 房间  2. 电话  3. 住址）\n\t %c\n", Fchoice);
    if(Fchoice=='1')
    {
        do
        {
            printf("\t 请输入目标房间楼层：");
            Sleep(50);
            fgets(flood, N, stdin);
            if(flood[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(flood[strlen(flood)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                flood[strlen(flood)-1] = '\0';
            }
            printf("\t 请输入目标房间号：");
            Sleep(50);
            fgets(roomName, N, stdin);
            if(roomName[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(roomName[strlen(roomName)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                roomName[strlen(roomName)-1] = '\0';
            }
            Rp = searchRoom(flood, roomName, roomHead);
            if(Rp==NULL)
            {
                printf("\t 未找到对应房间, 请重新选择\n");
                Sleep(50);
                continue;
            }
        }while(Rp==NULL);
        fprintf(historyFile, "\t 请输入目标房间楼层：%s\n", flood);
        fprintf(historyFile, "\t 请输入目标房间号：%s\n", roomName);
        Rp->preNumb += 1;
        Rp = searchRoom(Sp->basic_info.flood, Sp->basic_info.room, roomHead);
        Rp->preNumb -= 1;
        strcpy(Sp->basic_info.flood, flood);
        strcpy(Sp->basic_info.room, roomName);
        printf("\t 修改成功\n\n");
        Sleep(50);
        fprintf(historyFile, "\t 修改成功\n\n");
    }
    else if(Fchoice=='2')
    {
        do
        {
            printf("\t 新的电话号码：");
            Sleep(50);
            fgets(Sp->detail_info.tel, N, stdin);
            if(Sp->detail_info.tel[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(Sp->detail_info.tel[strlen(Sp->detail_info.tel)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                Sp->detail_info.tel[strlen(Sp->detail_info.tel)-1] = '\0';
            }
        }while(Sp->detail_info.tel[0]=='\n');
        fprintf(historyFile, "\t 新的电话号码：%s\n", Sp->detail_info.tel);
        printf("\t 修改成功\n\n");
        Sleep(50);
        fprintf(historyFile, "\t 修改成功\n\n");
    }
    else if(Fchoice=='3')
    {
        do
        {
            printf("\t 新的省份（直辖市）：");
            Sleep(50);
            fgets(Sp->detail_info.province, N, stdin);
            if(Sp->detail_info.province[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(Sp->detail_info.province[strlen(Sp->detail_info.province)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                Sp->detail_info.province[strlen(Sp->detail_info.province)-1] = '\0';
            }
        }while(Sp->detail_info.province[0]=='\n');
        do
        {
            printf("\t 新的详细地址：");
            Sleep(50);
            fgets(Sp->detail_info.Address, N, stdin);
            if(Sp->detail_info.Address[0]=='\n')
            {
                printf("\t 错误输入\n");
                Sleep(50);
                continue;
            }
            else if(Sp->detail_info.Address[strlen(Sp->detail_info.Address)-1]!='\n')
            {
                while((c=getch())!=EOF&&c!='\n');
            }
            else
            {
                Sp->detail_info.Address[strlen(Sp->detail_info.Address)-1] = '\0';
            }
        }while(Sp->detail_info.Address[0]=='\n');
        fprintf(historyFile, "\t 新的省份（直辖市）：%s\n", Sp->detail_info.province);
        fprintf(historyFile, "\t 新的详细地址：%s\n", Sp->detail_info.Address);
        printf("\t 修改成功\n\n");
        Sleep(50);
        fprintf(historyFile, "\t 修改成功\n\n");
    }
}
//学生人数统计
int studentCount(Student * stuHead)
{
    int totalStu = 0;
    Student * Sp = NULL;
    Sp = stuHead;
    while(Sp!=NULL)
    {
        totalStu++;
        Sp = Sp->next;
    }
    return totalStu;
}
//方法排序
void sortBySomeway(Student ** sortStart, int n, int (* someWay)(Student * a, Student * b))
{
    int i, j;
    Student * changeSam = NULL;
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-1-i;j++)
        {
            if(someWay(*(sortStart+j), *(sortStart+j+1))>0)
            {
                changeSam = *(sortStart+j);
                *(sortStart+j) = *(sortStart+j+1);
                *(sortStart+j+1) = changeSam;
            }
        }
    }
}
//学号排序
int sortByNumb(Student * a, Student * b)
{
    return strcmp(a->basic_info.numb, b->basic_info.numb);
}
//姓名排序
int sortByName(Student * a, Student * b)
{
    return strcmp(a->basic_info.name, b->basic_info.name);
}
//房间排序
int sortByRoom(Student * a, Student * b)
{
    return strcmp(a->basic_info.room, b->basic_info.room);
}
//省份（直辖市）排序
int sortByProvince(Student * a, Student * b)
{
    return strcmp(a->detail_info.province, b->detail_info.province);
}


#endif // _myhead_h_
