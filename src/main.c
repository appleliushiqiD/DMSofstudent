#include "myhead.h"

int main()
{

    /**
     *变量声明
     */
    char choice;                            //功能选择
    char adminName[N] = "admin";            //管理员名称
    char adminPassword[N] = "123456";       //管理员密码
    char buildingName[N] = "未命名";        //楼舍名称
    FILE * adminFile = NULL;                //管理员信息文件访问
    FILE * roomFile = NULL;                 //房间文件访问
    FILE * stuFile = NULL;                  //学生文件访问
    FILE * historyFile = NULL;              //操作记录文件访问
    Student * stuHead = NULL;               //学生链表头
    Room * roomHead = NULL;                 //房间链表头

    //文件打开
    adminFile = fopen("管理员信息.txt", "r");
    historyFile = fopen("操作记录.txt", "w+");
    if(historyFile==NULL)
    {
        printf("\t 文件创建失败！\n");
        //文件关闭
        fclose(adminFile);
        exit(0);
    }
    //检查是否有使用记录
    if(adminFile == NULL)
    {
        printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        fprintf(historyFile, "\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        Sleep(50);
        adminFile = fopen("管理员信息.txt", "w");
        changeAdmin(buildingName, adminName, adminPassword, adminFile, historyFile);
        system("cls");
        printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        printf("\t 是否建立简单宿舍信息？（1. 是\t0. 否）\n");
        fprintf(historyFile, "\t 是否建立简单宿舍信息？（1. 是\t0. 否）\n");
        printf("\t ");
        fprintf(historyFile, "\t ");
        do
        {
            char c;
            choice = getchar();
            while((c=getchar())!=EOF&&c!='\n');
            if(choice!='0'&&choice!='1')
            {
                printf("\t 输入错误！请重新输入（1 / 0）\n\t ");
                fprintf(historyFile, "%c\n", choice);
                fprintf(historyFile, "\t 输入错误！请重新输入（1 / 0）\n\t ");
            }
        }while(choice!='0'&&choice!='1');
        fprintf(historyFile, "%c\n", choice);
        if(choice == '1')
        {
            resetRoom(&roomHead, &stuHead);
        }
        system("cls");
    }
    else
    {
        //读取信息
        fgets(buildingName, N, adminFile);
        buildingName[strlen(buildingName)-1] = '\0';
        fgets(adminName, N, adminFile);
        adminName[strlen(adminName)-1] = '\0';
        fgets(adminPassword, N, adminFile);
        adminPassword[strlen(adminPassword)-1] = '\0';
        fclose(adminFile);
        //系统登陆
        printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        fprintf(historyFile, "\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        printf("\t 管理员密码验证\n");
        fprintf(historyFile, "\t 管理员密码验证\n");
        while(!(checkPassword(adminPassword, historyFile)))
        {
            system("cls");
            printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
            printf("\t 密码错误！请重试 QAQ \n");
            fprintf(historyFile, "\t 密码错误！请重试 QAQ \n");
            Sleep(50);
        }
        //导入数据
        roomFile = fopen("房间信息.txt", "r");
        stuFile = fopen("学生信息.txt", "r");
        if(roomFile!=NULL && stuFile!=NULL)
        {
            importData(roomFile, stuFile, &roomHead, &stuHead);
        }
    }

    //背景音乐彩蛋
    /**

    if(strcmp(adminPassword, "20000713")==0)
    {
        PlaySound("bgm.wav", NULL, SND_LOOP|SND_ASYNC);
    }
    */
    /**
     *功能循环
     */
    do
    {
        printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        fprintf(historyFile, "\n\n");
        printMenu();
        Sleep(50);
        printf("\t 请选择：");
        do
        {
            char c;
            choice = getchar();
            while((c=getchar())!=EOF&&c!='\n');
            if(choice<'0'||choice>'9')
            {
                system("cls");
                printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
                printMenu();
                printf("\t 请正确输入\n\t ");
            }
        }while(choice<'0'||choice>'9');
        if(choice == '0')
        {
            printf("\t 欢迎下次使用！\tHave a good day!\n");
            break;
        }
        if(choice != '9')
        {
            fprintf(historyFile, "\t 选择了功能：%c\n\n", choice);
        }
        system("cls");
        printf("\t******楼舍：%-20s管理员：%-20s******\n\n", buildingName, adminName);
        switch(choice)
        {
            case '1'://房间信息查看
                viewMainInfo(roomHead);
                fprintf(historyFile, "\t 查看了房间信息（内容略）\n");
                break;
            case '2'://学生信息查询、修改与删除
                searchStu(roomHead, &stuHead, historyFile);
                break;
            case '3'://学生信息录入
                addStudent(roomHead, &stuHead, historyFile);
                break;
            case '4'://学生信息排序
                sortStudent(stuHead);
                fprintf(historyFile, "\t 进行了学生信息排序（内容略）\n");
                break;
            case '5'://房间信息修改
                changeRoom(&roomHead, stuHead, historyFile);
                break;
            case '6'://管理员信息修改
                fprintf(historyFile, "\t 尝试管理员信息修改\n");
                while(!(checkPassword(adminPassword, historyFile)))
                {
                    printf("\t 密码错误！请重试 QAQ \n");
                    fprintf(historyFile, "\t 密码错误！请重试 QAQ \n");
                    Sleep(50);
                }
                adminFile = fopen("管理员信息.txt", "w");
                changeAdmin(buildingName, adminName, adminPassword, adminFile, historyFile);
                /**彩蛋音乐
                if(strcmp(adminPassword, "20000713")==0)
                {
                    PlaySound("bgm.wav", NULL, SND_LOOP|SND_ASYNC);
                }
                else
                {
                    PlaySound(NULL, NULL, SND_LOOP|SND_ASYNC);
                }
                */
                printf("\t 修改成功\n");
                break;
            case '7'://更新文件（学生信息）
                roomFile = fopen("房间信息.txt", "w");
                stuFile = fopen("学生信息.txt", "w");
                resetFile(roomFile, stuFile, roomHead, stuHead);
                printf("\t 更新成功\n");
                fprintf(historyFile, "\t 更新了信息文件\n");
                break;
            case '8'://初始化楼舍信息
                resetRoom(&roomHead, &stuHead);
                printf("\t 初始化成功\n");
                fprintf(historyFile, "\t 初始化了房间信息\n");
                break;
            case '9'://操作记录查看
                printf("\n\n\t 历史：\n");
                printHistory(historyFile);
                fprintf(historyFile, "\t 选择了功能：9\n");
                fprintf(historyFile, "\t 查看历史……（内容省略）\n\n");
                break;
        }
        printf("\n\t 任意键返回菜单");
        getchar();
        system("cls");
    }while(choice != 0);

    //文件关闭
    fclose(adminFile);
    fclose(roomFile);
    fclose(stuFile);
    fclose(historyFile);
    return 0;
}
