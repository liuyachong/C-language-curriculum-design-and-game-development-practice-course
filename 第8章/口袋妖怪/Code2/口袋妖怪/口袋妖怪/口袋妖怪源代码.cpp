#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <graphics.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "hanshuku.h"

#define window_height 480
#define window_width  640               //窗口大小
#define pokemon_number 30              //精灵数量

IMAGE main_map;                              //地图图像对象
IMAGE character_vice;                         
IMAGE character;

int map_init_x,map_init_y;
int character_x,character_y;
int character_picture_i,character_picture_j;
int canvas[1000][10000]={0};   //定义画布坐标 使每个像素点坐标为0


char input;  //输入判断变量

struct pokemon  //精灵数据的结构体
{
	int x;
	int y;
	int attract;
	int bleed;
	int wufang;
	int special_attract;
	int special_fangyu;
	int level;
	char *name;
	char *speciese;
}PK[pokemon_number];

void startup_pokemon()    //精灵属性初始化
{
	 PK[1].x=100;
	 PK[1].y=200;
	 PK[1].attract=8;
	 PK[1].bleed=30;
	 PK[1].wufang=4;
	 PK[1].special_attract=9;
	 PK[1].special_fangyu=7;
	 PK[1].level = 5;
	 PK[1].name = "皮卡丘";
	 PK[1].speciese = "电";
}
void starup_show()                   //初始化地图
{
   initgraph(window_width,window_height);           //初始化绘图环境
   map_init_x=map_init_y=500;                         //地图初始时时截取从（500,500）开始的部分//
   character_x=320;                     // 人物的初始位置
   character_y=240;
   character_picture_i=0;
   character_picture_j=0;
  // loadimage(&main_map,_T("资源文件夹\\总地图.jpg"),640,1088);
    loadimage(&main_map,_T("资源文件夹\\大地图.jpg"),5000,5000);
   loadimage(&character_vice,_T("资源文件夹\\桌面行走图遮罩图.jpg",136,200));
   loadimage(&character,_T("资源文件夹\\桌面行走图.jpg",136,200));
   BeginBatchDraw();
}

void map_show()                                         //地图显示函数
{
   putimage(0,0,window_width,window_height,&main_map,map_init_x,map_init_y);               //随着取图坐标的更新而显示地图的移动
   putimage(character_x,character_y,45,50,&character_vice,character_picture_i*45,character_picture_j*50,NOTSRCERASE); //每个小人的像素是（45,50）
   putimage(character_x,character_y,45,50,&character,character_picture_i*45,character_picture_j*50,SRCINVERT);
   FlushBatchDraw();
}

void UpdateWithInput()                //随着输入而更新的函数
{
    if (kbhit())
	{                                                        //每次移动20个像素单位
    input=getch();
	if (character_picture_i==2)                    //实现了交替取人物图的不同部分
   {
		character_picture_i=-1;
    }
	if (input=='A' || input == 'a')                  //根据方向操作进行数据的更新
	{
		map_init_x-=20;
		character_picture_j=2;
	}
	if (input=='D' || input=='d')
	{
		map_init_x+=20;
		character_picture_j=1;
	}
	if (input=='S' || input=='s')
	{
		map_init_y+=20;
		character_picture_j=0;
	}
	if (input=='W'||input=='w')
	{
		map_init_y-=20;
		character_picture_j=3;
	}
    character_picture_i++;
	}
}
void UpdateWithoutInput()                 //给精灵赋予位置
{
		int i;
		for(i=1; i<pokemon_number; i++)                //给pokemon赋予初始坐标位置
		{
			PK[i].x=10000*i;
			PK[i].y=100000*i;
		}
}

void fight_animation()                      
{
	//这里实现了战斗前开场动画
	int i, j;
	i=320, j=0;       //从正上方开始画
	setlinecolor(BLACK);//画线颜色设置为黑色
	while(!(i==319&&j==0))  
	{
		line(320,240,i,j);
		FlushBatchDraw();
		if(i==640)
			j++;
		if(i==0)
			j--;
		if(j==600)
			i--;
		if(j==0)
			i++;
	}
   //到这里动画结束

}

void fight_show()
{
		int x = 30;  
		int y = 365;//技能选择图标坐标
		
		int jumpy = 150;    //己方精灵坐标
		int jump = 5;        //精灵循环跳动变量
		char input;  //输入判断变量

		while(1)
		{
			//background 部分
				IMAGE img1;
				loadimage(&img1,_T("资源文件夹\\战斗背景1.jpg"));
				putimage(0, 0, &img1);
				//我方血条框
				setlinestyle(PS_SOLID, 3);                      //边框宽度
				setlinecolor(BLACK);                            //边框颜色
				setfillcolor(RGB(255, 245, 220));               //填充颜色
				fillroundrect(25, 50, 220, 130, 10, 10);         //边框大小
																 //敌方血条框
				setlinestyle(PS_SOLID, 3);                      //边框宽度
				setlinecolor(BLACK);                            //边框颜色
				setfillcolor(RGB(255, 245, 220));               //填充颜色
				fillroundrect(330, 220, 580, 310, 10, 10);         //边框大小

				//最下方技能框
				setfillcolor(BLACK);
				solidrectangle(0, 335, 640, 480);                //黑色背景   

				setfillcolor(LIGHTGRAY);
				solidroundrect(5, 340, 340, 475, 10, 10);
				setfillcolor(WHITE);
				solidroundrect(15, 350, 330, 465, 10, 10);          //左边技能框

				setfillcolor(LIGHTGRAY);
				solidroundrect(345, 340, 635, 475, 10, 10);
				setfillcolor(WHITE);
				solidroundrect(355, 350, 625, 465, 10, 10);        //右边技能框

				if (jump == 5)
				{
					jumpy = 150;                                //己方精灵循环上下跳动
				}
					
				if (jump == 10)
				{
					jumpy = 155;
					jump = 0;
				}
				jump++;

				//皮卡丘
				pokemon_fight_show(PK[1].name,jumpy);
				/*IMAGE im1, im2;
				loadimage(&im1, _T("资源文件夹\\皮卡丘.jpg"));
				loadimage(&im2, _T("资源文件夹\\皮卡丘2.jpg"));
				putimage(65, jumpy, &im2, NOTSRCERASE);
				putimage(65, jumpy, &im1, SRCINVERT);*/
				
				//技能选择图标
				setfillcolor(BLACK);
				POINT pts[] = { { x, y },{ x, y + 20 },{ x + 15, y + 10 } };
				solidpolygon(pts, 3);
  
				

				//control控制部分
				if (kbhit()) //操作判断
				{
					input = getch();
					if ( (input == 'd' || input == 'D')&&x < 190)
						x += 160;
					if ((input == 'a' || input == 'A')&&x > 30)
						x -= 160;
					if ((input == 's' || input == 'S')&&y<365+60)
						y += 60;
					if ((input == 'w' || input == 'W')&&y > 365)
						y -= 60;
					if (input == 'z' || input == 'Z')
					{
						if (x==30&&y==365)
						{
							lighting();
						}
						if (x == 190 && y == 365)
						{
							scream() ;
						}
					}
				}
				FlushBatchDraw();//结束批量绘制
			}	
		}


void fight()
{
	int i;
	for(i=0; i<pokemon_number; i++)    //遍历精灵坐标数组，如果人物与某个精灵距离小于某长度时进入战斗
		if( abs(map_init_x+window_width/2-PK[i].x)<=15 &&  abs(map_init_y+window_height/2-PK[i].y)<=15)
		{
				fight_animation();  //战斗前动画
				fight_show();      //战斗显示函数
		}
}



void main()
{
	startup_pokemon();//对精灵属性进行初始化
	starup_show();  //先对地图进行初始化
   while(1)
   {
    map_show();         //地图显示函数
    UpdateWithInput();       
	UpdateWithoutInput();
	fight();                         //战斗函数
   }
   EndBatchDraw();
   getch();
}