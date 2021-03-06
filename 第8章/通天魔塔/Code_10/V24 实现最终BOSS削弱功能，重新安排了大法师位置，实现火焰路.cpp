#include<graphics.h>
#include<stdio.h>
#include<conio.h>	//头文件

#define WIDTH 32
#define HIGH 24	//块状化画布

struct attribute
{
	int Level;					//人物等级
	int Life;					//人物生命值
	int Attack;					//人物攻击力
	int Defence;				//人物防御力
	int GOLD;					//所持金币数
	int Exp;					//拥有经验值
	int GKnum;					//绿钥匙个数
	int RKnum;					//红钥匙个数
	int YKnum;					//黄钥匙个数
	int BKnum;					//蓝钥匙个数
};

struct monster
{
	int life;					//怪物生命值
	int att;					//怪物攻击力
	int def;					//怪物防御力
	int Gold;					//击败怪物可获金币数
	int exp;					//击败怪物可获经验值
};

struct Picture					//另外开辟一个结构体，用原结构体由于数组多达9999，在其中定义IMAGE会导致过多而崩溃
{
	IMAGE img_mon;				//战斗界面怪物大图
};

struct atlas
{
	int newcanvas[WIDTH][HIGH];	//新的画布数组
};

//函数外全局变量定义
IMAGE img_bk;						//背景图片
IMAGE img_land;						//道路图片
IMAGE img_door;						//门图片
IMAGE img_wall;						//墙图片
IMAGE img_monster,img_monster00;	//怪物图片
IMAGE img_player1,img_player2;		//玩家图片
IMAGE img_Elf,img_Elf00;			//精灵图片
IMAGE img_NPC1,img_NPC01;			//NPC图片
IMAGE img_tp1,img_tp01;				//传送水晶图片
IMAGE img_panel;					//人物面板图片
IMAGE img_excalibur1,img_excalibur2;//圣剑图片
IMAGE img_item1,img_item2;      //圣剑外的道具
IMAGE img_newbk;					//1层及以上的背景图片
IMAGE img_player;					//战斗界面人物大图

int i,j,z;
int canvas[WIDTH][HIGH];	//画布数组
struct atlas map[6];		//用于重定义怪物的各层画布数组 
int player_x,player_y;		//玩家位置
int step;					//移动步长
int dialogue;				//判断是否发生对话
int Stair;					//所在层数
int FLOOR;					//所在层数
int Gold;					//所持金币数
struct attribute p;			//人物属性面板参数
struct monster m[9999];		//怪物属性参数
struct Picture _m[10];		//对应怪物大图
int OwnExcalibur;			//判断是否拥有道具圣剑
int isdone;					//判断装备属性是否已经增加给人物
int isscan;					//判断是否正在查看道具属性
int isgiven,isgivenG;       //判断是否给过钥匙
int iscolor;				//判断鼠标是否移至可点击文字部分	
int isbattle;				//判断是否与怪物交战
int battleover;				//判断战斗是否结束
int bossappear;				//判断BOSS是否出现
int isCrystal;				//判断向上传送的水晶是否出现
int gameover;				//判断是否游戏结束
char lastinput;				//记录上一次键盘键入
TCHAR string[100];			//用于存放转换后的数字
int oldtime,newtime,time;
int counter1=0,counter2=0;
int timestart=0,move=-1,go=0;
int LIFE;
int start=0;
int isdone2=0,tips=0,test=0;
SYSTEMTIME ti;	

void transform(int number,TCHAR str[])			//将数字转化为字符串
{
	int i=0;
	int j=0;
	TCHAR temp;
	for(i=0;i<100;i++)
		str[i]='\0';
	if( number>0 )
	{
		while(number>0)
		{
			str[i] = number%10+'0';
			number /= 10;
			i++;
		}
		i--;
		while(j<i)
		{
			temp=str[j];
			str[j]=str[i];
			str[i]=temp;
			j++;
			i--;
		}
	}
	else if(number<=0)
	{
		str[0]='0';
	}
}

void delay(DWORD ms)//绝对延时
{
	static DWORD oldtime=GetTickCount();

	while(GetTickCount()-oldtime<ms)
		Sleep(1);

	oldtime=GetTickCount();
}

void DefineBackground()	//在数组中定义背景的值为0
{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			canvas[i][j]=0;
		}
	}
}
void DrawBackground()	//绘制背景
{	
	
	 if( FLOOR==0&&start==1 )
	{
		for(i=8;i<WIDTH;i++)
		{
			for(j=0;j<HIGH;j++)
			{
				if(canvas[i][j]==0)
				{
					loadimage(&img_bk, _T("E:\\魔塔project\\Resources\\Terrain\\T-SKY.jpg"));
					putimage(i*32, j*32, &img_bk);
				}
			}
		}
	}
	else if( FLOOR==1 )
	{
		loadimage(&img_newbk, _T("E:\\魔塔project\\Resources\\Map\\layer1.jpg"));
		putimage(256, 0, &img_newbk);
	}
	else if( FLOOR==2 )
	{
		loadimage(&img_newbk, _T("E:\\魔塔project\\Resources\\Map\\layer2.jpg"));
		putimage(256, 0, &img_newbk);
	}
	else if( FLOOR==3 )
	{
		loadimage(&img_newbk, _T("E:\\魔塔project\\Resources\\Map\\layer3.jpg"));
		putimage(256, 0, &img_newbk);
	}
}
void DefineRoad()	//在数组中定义道路的值为1
{
	if( FLOOR==0 )
	{
		for(j=8;j<12;j++)
		{	
			canvas[14][j]=1;
		}			
		for(j=9;j<12;j++)
		{
			canvas[15][j]=1;
		}
		for(j=11;j<13;j++)
		{
			canvas[16][j]=1;
		}
		for(j=HIGH-2;j<HIGH;j++)
		{
			canvas[16][j]=1;
		}
		for(j=11;j<14;j++)
		{
			canvas[17][j]=1;
		}
		for(j=HIGH-4;j<HIGH;j++)
		{
			canvas[17][j]=1;
		}
		for(j=11;j<15;j++)
		{
			canvas[18][j]=1;
			canvas[19][j]=1;
		}
		for(j=HIGH-7;j<HIGH;j++)
		{
			canvas[18][j]=1;
		}
		for(j=12;j<15;j++)
		{
			canvas[21][j]=1;
		}
		for(j=HIGH-8;j<HIGH;j++)
		{
			canvas[19][j]=1;
			canvas[21][j]=1;
		}
		for(i=21;i<24;i++)
		{
			for(j=8;j<10;j++)
			{
				canvas[i][j]=1;
			}
		}
		for(i=24;i<27;i++)
		{
			canvas[i][8]=1;
		}
		for(j=HIGH-6;j<HIGH;j++)
		{
			canvas[22][j]=1;
		}
		for(j=HIGH-4;j<HIGH;j++)
		{
			canvas[23][j]=1;
		}
		for(j=HIGH-3;j<HIGH;j++)
		{
			canvas[24][j]=1;
		}
		for(j=HIGH-1;j<HIGH;j++)
		{
			canvas[25][j]=1;
		}
		for(j=5;j<9;j++)
		{
			canvas[26][j]=1;
		}
		for(j=0;j<HIGH;j++)
		{
			canvas[20][j]=1;
		}	
	}
	else if( FLOOR==3 )	//特殊道路，定义值为9
	{
		for(i=17;i<=22;i++)
		{
			canvas[i][9]=9;
			canvas[i][14]=9;
		}
		for(j=10;j<=13;j++)
		{
			canvas[17][j]=9;
			canvas[22][j]=9;
		}
	}
}
void DrawRoad()	//绘制道路
{	
	if(start==1)
{
	if( FLOOR==0 )
	{
		for(i=8;i<WIDTH;i++)
		{
			for(j=0;j<HIGH;j++)
			{
				if(canvas[i][j]==1)
				{
					loadimage(&img_land, _T("E:\\魔塔project\\Resources\\Terrain\\T-LAND.jpg"));
					putimage(i*32, j*32, &img_land);
				}
			}
		}
	}
}

}
void DefineWall()	//在数组中定义墙的值为2
{
	if( FLOOR==0 )
	{
		for(j=8;j<13;j++)
		{	
			canvas[13][j]=2;
		}
		for(i=13;i<16;i++)
		{
			canvas[i][7]=2;
			canvas[i][12]=2;
		}
		canvas[15][8]=2;
		for(i=15;i<17;i++)
		{
			canvas[i][13]=2;
		}
		for(j=8;j<11;j++)
		{
			canvas[16][j]=2;
		}
		for(i=16;i<19;i++)
		{
			canvas[i][14]=2;
		}
		for(i=17;i<19;i++)
		{
			canvas[i][10]=2;
		}
		for(j=0;j<11;j++)
		{
			canvas[19][j]=2;
		}
		for(j=0;j<8;j++)
		{
			canvas[21][j]=2;
		}
		for(i=22;i<26;i++)
		{
			canvas[i][7]=2;
		}
		for(j=4;j<7;j++)
		{
			canvas[25][j]=2;
		}
		canvas[26][4]=2;
		for(j=4;j<10;j++)
		{
			canvas[27][j]=2;
		}
		for(i=24;i<27;i++)
		{
			canvas[i][9]=2;
		}
		for(i=21;i<25;i++)
		{
			canvas[i][10]=2;
		}
		canvas[19][15]=2;
		canvas[21][15]=2;
		for(j=12;j<15;j++)
		{
			canvas[22][j]=2;
		}
		canvas[21][12]=2;
		canvas[21][11]=2;
		canvas[21][15]=2;
		for(j=15;j<18;j++)
		{
			canvas[22][j]=2;
		}
		for(j=17;j<20;j++)
		{
			canvas[23][j]=2;
		}
		for(j=19;j<21;j++)
		{
			canvas[24][j]=2;
		}
		for(j=20;j<23;j++)
		{
			canvas[25][j]=2;
		}
		for(j=22;j<HIGH;j++)
		{
			canvas[26][j]=2;
		}
		for(j=15;j<17;j++)
		{
			canvas[18][j]=2;
		}
		for(j=16;j<20;j++)
		{
			canvas[17][j]=2;
		}
		for(j=19;j<22;j++)
		{
			canvas[16][j]=2;
		}
		for(j=HIGH-3;j<HIGH;j++)
		{
			canvas[15][j]=2;
		}
	}
	else if( FLOOR==1 )
	{
		for(i=8;i<WIDTH;i++)
		{
			canvas[i][0]=2;
			canvas[i][HIGH-1]=2;
		}
		for(j=0;j<HIGH;j++)
		{
			canvas[8][j]=2;
			canvas[WIDTH-1][j]=2;
		}
		canvas[9][4]=2;
		for(i=9;i<11;i++)
		{
			canvas[i][16]=2;
		}
		for(j=19;j<21;j++)
		{
			canvas[9][j]=2;
		}
		for(j=4;j<7;j++)
		{
			canvas[11][j]=2;
		}
		for(j=2;j<5;j++)
		{
			canvas[12][j]=2;
		}
		for(j=6;j<20;j++)
		{
			canvas[12][j]=2;
		}
		for(j=21;j<23;j++)
		{
			canvas[12][j]=2;
		}
		for(j=2;j<5;j++)
		{
			canvas[12][j]=2;
		}
		canvas[13][4]=2;
		canvas[13][6]=2;
		canvas[14][4]=2;
		canvas[14][6]=2;
		for(i=14;i<17;i++)
		{
			canvas[i][3]=2;
			canvas[i][7]=2;
		}
		for(i=13;i<20;i++)
		{
			canvas[i][11]=2;
		}
		for(i=13;i<18;i++)
		{
			canvas[i][16]=2;
		}
		canvas[16][1]=2;
		canvas[19][1]=2;
		for(i=16;i<20;i++)
		{
			canvas[i][4]=2;
		}
		for(i=19;i<22;i++)
		{
			canvas[i][3]=2;
		}
		for(i=16;i<24;i++)
		{
			canvas[i][6]=2;
		}
		canvas[21][5]=2;
		canvas[22][5]=2;
		for(i=23;i<26;i++)
		{
			canvas[i][3]=2;
		}
		canvas[23][4]=2;
		canvas[25][4]=2;
		canvas[25][1]=2;
		for(j=1;j<4;j++)
		{
			canvas[27][j]=2;
		}
		canvas[28][3]=2;
		canvas[30][3]=2;
		for(i=23;i<26;i++)
		{
			canvas[i][7]=2;
		}
		for(i=25;i<29;i++)
		{
			canvas[i][6]=2;
		}
		for(i=27;i<29;i++)
		{
			canvas[i][5]=2;
		}
		canvas[30][5]=2;
		canvas[30][6]=2;
		for(j=7;j<16;j++)
		{
			canvas[27][j]=2;
		}
		for(i=13;i<20;i++)
		{
			canvas[i][11]=2;
		}
		for(i=21;i<28;i++)
		{
			canvas[i][11]=2;
		}
		for(i=13;i<18;i++)
		{
			canvas[i][16]=2;
		}
		for(i=19;i<29;i++)
		{
			canvas[i][16]=2;
		}
		canvas[30][16]=2;
		for(j=17;j<23;j++)
		{
			canvas[16][j]=2;
		}
		for(i=17;i<19;i++)
		{
			canvas[i][19]=2;
		}
		canvas[20][18]=2;
		for(i=20;i<24;i++)
		{
			canvas[i][19]=2;
		}
		for(j=17;j<19;j++)
		{
			canvas[23][j]=2;
		}
		for(j=21;j<23;j++)
		{
			canvas[23][j]=2;
		}
	}
	else if( FLOOR==2 )
	{
		for(i=8;i<WIDTH;i++)
		{
			canvas[i][0]=2;
			canvas[i][HIGH-1]=2;
		}
		for(j=0;j<HIGH;j++)
		{
			canvas[8][j]=2;
			canvas[WIDTH-1][j]=2;
		}
		for(j=1;j<6;j++)
		{
			canvas[10][j]=2;
		}
		for(j=1;j<6;j++)
		{
			canvas[10][j]=2;
		}
		for(j=1;j<7;j++)
		{
			canvas[12][j]=2;
		}
		for(i=9;i<16;i++)
		{
			canvas[i][8]=2;
			canvas[i][12]=2;
			canvas[i][18]=2;
		}
		for(j=2;j<9;j++)
		{
			canvas[16][j]=2;
		}
		for(i=17;i<26;i++)
		{
			canvas[i][2]=2;
		}
		for(i=26;i<30;i++)
		{
			canvas[i][2]=2;
			canvas[i][6]=2;
			canvas[i][8]=2;
		}
		for(j=14;j<17;j++)
		{
			canvas[10][j]=2;
			canvas[12][j]=2;
		}
		for(j=20;j<23;j++)
		{
			canvas[10][j]=2;
			canvas[14][j]=2;
		}
		for(i=9;i<17;i++)
		{	canvas[i][10]=2;
		    canvas[i+3][14]=2;
		}
		for(j=3;j<6;j++)
		{	canvas[29][j]=2;
			canvas[25][j+1]=2;
			canvas[19][j+7]=2;
			canvas[17][j+7]=2;
			canvas[19][j+12]=2;
		}
		canvas[9][14]=2;
		canvas[18][12]=2;
		canvas[19][6]=2;
		for(i=13;i<15;i++)
		{
			canvas[i][3]=2;
			canvas[i][6]=2;
		}
		for(i=11;i<14;i++)
		{	
			canvas[i][20]=2;
		}
		for(j=18;j<22;j++)
		{
			canvas[16][j]=2;
		}
		for(j=4;j<9;j++)
		{
			canvas[18][j]=2;
			canvas[23][j]=2;
			canvas[18][j+13]=2;
			canvas[23][j+13]=2;
		}
		for(j=8;j<18;j++)
		{
			canvas[22][j]=2;
		}
		for(j=8;j<13;j++)
		{
			canvas[19][j]=2;
		}
		for(j=9;j<23;j++)
		{
			canvas[26][j]=2;
		}
		for(j=9;j<20;j++)
		{
			canvas[29][j]=2;
		}
		for(i=19;i<23;i++)
		{	
			canvas[i][4]=2;
			canvas[i][21]=2;
		}
	
	}
	else if(FLOOR==3)
	{
		for(i=10;i<=29;i++)
		{
			canvas[i][2]=2;
			canvas[i][21]=2;
		}
		for(i=12;i<=27;i++)
		{
			canvas[i][4]=2;
			canvas[i][19]=2;
		}
		for(i=14;i<=25;i++)
		{
			canvas[i][6]=2;
			canvas[i][17]=2;
		}
		for(i=16;i<=23;i++)
		{
			canvas[i][8]=2;
			canvas[i][15]=2;
		}
		for(j=2;j<=21;j++)
		{
			canvas[10][j]=2;
			canvas[29][j]=2;
		}
		for(j=4;j<=19;j++)
		{
			canvas[12][j]=2;
			canvas[27][j]=2;
		}
		for(j=6;j<=17;j++)
		{
			canvas[14][j]=2;
			canvas[25][j]=2;
		}
		for(j=8;j<=15;j++)
		{
			canvas[16][j]=2;
			canvas[23][j]=2;
		}
		canvas[18][10]=2;
		canvas[21][10]=2;
		canvas[18][13]=2;
		canvas[21][13]=2;
		canvas[12][20]=2;
		canvas[14][5]=2;
		canvas[24][15]=2;
		
	}
}
void DrawWall()	//绘制墙
{	if(start==1)
	{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if(canvas[i][j]==2)
			{
				if( FLOOR==0 )
				{
					loadimage(&img_wall, _T("E:\\魔塔project\\Resources\\Terrain\\T-BlockWALL.jpg"));
					putimage(i*32, j*32, &img_wall);
				}
			/*	else if( FLOOR==1 )
				{
					loadimage(&img_wall, _T("E:\\魔塔project\\Resources\\Terrain\\T-WoodenWALL.jpg"));
					putimage(i*32, j*32, &img_wall);
				}*/
			}
		}
	}
	}
}
void DefineDoor()	//在数组中定义门的值，区间为3-8
{
	if( FLOOR==0 )
	{
		canvas[20][15]=3;
	}
/*	if( FLOOR==1 )
	{
		canvas[12][20]=4;
		canvas[11][16]=4;
		canvas[10][4]=4;
		canvas[12][1]=4;
		canvas[16][2]=4;
		canvas[19][2]=4;
		canvas[25][2]=4;
		canvas[22][3]=4;
		canvas[29][16]=4;
		canvas[23][20]=4;
		canvas[19][19]=4;
		canvas[18][16]=4;
	}  */
}
void DrawDoor()	//绘制门
{
	if(start==1)
	{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if( canvas[i][j]==3 )
			{
				loadimage(&img_door, _T("E:\\魔塔project\\Resources\\Terrain\\T-IronDOOR.jpg"));
				putimage(i*32, j*32, &img_door);
			}
			/*if( canvas[i][j]==4 )
			{
				loadimage(&img_door, _T("E:\\魔塔project\\Resources\\Doors\\springdoor.jpg"));
				putimage(i*32, j*32, &img_door);
			}*/
		}
	}
	}
}
void Defineitems()	//在数组中定义物品的值，区间为10-99
{
	if( FLOOR==1 )
	{
		canvas[30][1]=10;
		canvas[14][10]=11;
		canvas[13][3]=12;
		canvas[17][21]=12;
		canvas[23][5]=12;
		canvas[19][2]=13;
		canvas[22][17]=13;
		canvas[30][22]=13;
		canvas[9][1]=20;
		canvas[26][10]=20;
		canvas[17][22]=20;

	}
	if( FLOOR==2 )
	{
		canvas[27][5]=21;
		canvas[20][19]=22;
		canvas[24][1]=12;
		canvas[21][19]=12;
		canvas[9][1]=12;
		canvas[13][15]=12;
		canvas[23][15]=12;
		canvas[11][1]=13;
		canvas[9][22]=13;
		canvas[27][9]=13;
		canvas[28][9]=13;
		canvas[13][4]=20;
		canvas[9][9]=20;
		canvas[19][20]=20;
		canvas[22][20]=20;
		canvas[12][22]=25;//蓝钥匙
	}
	if( FLOOR==3 )
	{
		canvas[15][7]=31;//惊雷剑
		canvas[14][18]=32;//神霄甲
		canvas[11][3]=30;//紫药水
		canvas[28][20]=30;
		canvas[28][3]=30;
		canvas[24][8]=30;
	}
}
void Drawitems()
{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if(canvas[i][j]==10)	//绘制短剑
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_shortsword.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\shortsword.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==11)	//绘制木盾
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_woodenshield.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\woodenshield.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==12)	//绘制攻击型宝石
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_reddiamond.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\reddiamond.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==13)	//绘制防御型宝石
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_bluediamond.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\bluediamond.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==20)	//绘制红药水
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_RedMedicine.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\RedMedicine.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==30)	//绘制紫药水
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_Item02-10_2_1.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\Item02-10_2_1.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==31)	//绘制惊雷剑
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_Item03-15_3_4.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\Item03-15_3_4.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==32)	//绘制神霄甲
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_Item03-15_2_4.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\Item03-15_2_4.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==22)	//绘制钢盾
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_steelshield.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\steelshield.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
			if(canvas[i][j]==25)	//绘制蓝钥匙
			{
				loadimage(&img_item1, _T("E:\\魔塔project\\Resources\\Item\\_BlueKey.jpg"));
				loadimage(&img_item2, _T("E:\\魔塔project\\Resources\\Item\\BlueKey.jpg"));
				putimage(i*32, j*32, &img_item1,NOTSRCERASE);
				putimage(i*32, j*32, &img_item2,SRCINVERT);
			}
		}
	}
}
void DefineNPC()	//在数组中定义NPC的值，区间为100-999
{
	if( FLOOR==0 )
	{
		canvas[26][5]=100;
		canvas[14][8]=101;
	}
	else if( FLOOR==1 )
	{
	  canvas[15][4]=102;
	  canvas[15][22]=103;
	}
	else if(FLOOR==2)
	{
		canvas[9][11]=104;
		
	}
}
void DrawNPC()	//绘制NPC
{
	if(start==1)
	{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if(canvas[i][j]==100)	//绘制精灵
			{
				loadimage(&img_Elf, _T("E:\\魔塔project\\Resources\\Character\\_Elf.jpg"));
				loadimage(&img_Elf00, _T("E:\\魔塔project\\Resources\\Character\\Elf.jpg"));
				putimage(i*32, j*32, &img_Elf,NOTSRCERASE);
				putimage(i*32, j*32, &img_Elf00,SRCINVERT);
			}
			if(canvas[i][j]==101)	//绘制智者
			{
				loadimage(&img_NPC01, _T("E:\\魔塔project\\Resources\\Character\\_NPC01.jpg"));
				loadimage(&img_NPC1, _T("E:\\魔塔project\\Resources\\Character\\NPC01.jpg"));
				putimage(i*32, j*32, &img_NPC01,NOTSRCERASE);
				putimage(i*32, j*32, &img_NPC1,SRCINVERT);
			}
			if(canvas[i][j]==102)	//绘制扫地大妈
			{
				loadimage(&img_NPC01, _T("E:\\魔塔project\\Resources\\Character\\_NPC02.jpg"));
				loadimage(&img_NPC1, _T("E:\\魔塔project\\Resources\\Character\\NPC02.jpg"));
				putimage(i*32, j*32, &img_NPC01,NOTSRCERASE);
				putimage(i*32, j*32, &img_NPC1,SRCINVERT);
			}
			if(canvas[i][j]==103)	//绘制看门大爷
			{
				loadimage(&img_NPC01, _T("E:\\魔塔project\\Resources\\Character\\_NPC03.jpg"));
				loadimage(&img_NPC1, _T("E:\\魔塔project\\Resources\\Character\\NPC03.jpg"));
				putimage(i*32, j*32, &img_NPC01,NOTSRCERASE);
				putimage(i*32, j*32, &img_NPC1,SRCINVERT);
			}
			if(canvas[i][j]==104)	//绘制冰雪女皇
			{
				loadimage(&img_NPC01, _T("E:\\魔塔project\\Resources\\Character\\_NPC04.jpg"));
				loadimage(&img_NPC1, _T("E:\\魔塔project\\Resources\\Character\\NPC04.jpg"));
				putimage(i*32, j*32, &img_NPC01,NOTSRCERASE);
				putimage(i*32, j*32, &img_NPC1,SRCINVERT);
			}
		}
	}
	}
}
void DefineMonster()	//在数组中定义怪物的值，区间为1000-9999
{
	if( FLOOR==0 )
	{
		canvas[20][16]=9999;
	}
	else if( FLOOR==1 )
	{
		canvas[10][19]=1000;
		canvas[11][18]=1000;
		canvas[10][1]=1000;
		canvas[9][6]=1000;
		canvas[10][6]=1000;
		canvas[11][7]=1000;
		canvas[9][12]=1000;
		canvas[11][12]=1000;
		canvas[13][2]=1000;
		canvas[18][2]=1000;
		canvas[20][2]=1000;
		canvas[24][2]=1000;
		canvas[29][2]=1000;
		canvas[29][7]=1000;
		canvas[29][15]=1000;
		canvas[28][17]=1000;
		canvas[24][19]=1000;
		canvas[29][21]=1000;
		canvas[30][21]=1000;
		canvas[29][22]=1000;
		canvas[25][22]=1000;
		canvas[18][22]=1000;
		canvas[18][21]=1000;
		canvas[18][20]=1000;
		canvas[20][17]=1000;
		for(i=16;i<24;i++)
		{
			canvas[i][13]=1000;
		}
		for(i=16;i<24;i++)
		{
			canvas[i][14]=1000;
		}
		for(i=15;i<25;i++)
		{
			canvas[i][10]=1000;
		}
		for(i=17;i<23;i++)
		{
			canvas[i][9]=1000;
		}
		for(i=18;i<22;i++)
		{
			canvas[i][8]=1000;
		}
		for(i=19;i<21;i++)
		{
			canvas[i][7]=1000;
		}
		canvas[13][7]=1000;
		canvas[26][7]=1000;
	}
	else if( FLOOR==2 )
	{
		canvas[9][4]=2001;
		canvas[9][5]=2001;
		canvas[9][16]=2001;
		canvas[9][19]=2001;
		canvas[10][9]=2001;
		canvas[11][2]=2001;
		canvas[11][4]=2001;
		canvas[11][14]=2001;
		canvas[12][7]=2001;
		canvas[13][1]=2001;
		canvas[13][5]=2001;
		canvas[13][11]=2001;
		canvas[13][16]=2001;
		canvas[14][2]=2001;
		canvas[14][4]=2001;
		canvas[14][19]=2001;
		canvas[14][15]=2001;
		canvas[15][4]=2001;
		canvas[15][11]=2001;
		canvas[15][21]=2001;
		canvas[17][13]=2001;
		canvas[19][13]=2001;
		canvas[22][1]=2001;
		canvas[23][1]=2001;
		canvas[20][8]=2001;
		canvas[21][8]=2001;
		canvas[20][16]=2001;
		canvas[21][16]=2001;
		canvas[20][18]=2001;
		canvas[21][18]=2001;
		canvas[20][20]=2001;
		canvas[21][20]=2001;
		canvas[19][19]=2001;
		canvas[22][19]=2001;
		canvas[23][16]=2001;
		canvas[24][12]=2001;
		canvas[24][10]=2001;
		canvas[25][14]=2001;
		canvas[27][10]=2001;
		canvas[28][10]=2001;
		for(i=26;i<=28;i++)
		{
			canvas[i][3]=2001;
			canvas[i][4]=2001;
		}
		canvas[26][5]=2001;canvas[28][5]=2001;
		canvas[27][11]=2001;canvas[27][16]=2001;
		canvas[28][14]=2001;canvas[28][18]=2001;
		canvas[30][4]=2001;canvas[30][13]=2001;
		canvas[29][22]=2001;
	    canvas[20][5]=9997;
	}
	else if( FLOOR==3 )
	{
		canvas[11][17]=3002;
		canvas[11][13]=3002;
		canvas[11][14]=3002;
		canvas[11][15]=3002;
		canvas[11][10]=3002;
		canvas[11][8]=3002;
		canvas[11][6]=3002;
		canvas[11][4]=3002;
		canvas[12][3]=3002;
		canvas[14][3]=3002;
		canvas[19][3]=3002;
		canvas[22][3]=3002;
		canvas[23][3]=3002;
		canvas[24][3]=3002;
		canvas[26][3]=3002;
		canvas[28][4]=3002;
		canvas[28][9]=3002;
		canvas[28][10]=3002;
		canvas[28][11]=3002;
		canvas[28][15]=3002;
		canvas[28][17]=3002;
		canvas[28][19]=3002;
		canvas[27][20]=3002;
		canvas[24][20]=3002;
		canvas[20][20]=3002;
		canvas[17][20]=3002;
		canvas[16][20]=3002;
		canvas[15][20]=3002;
		canvas[13][7]=3003;
		canvas[13][9]=3003;
		canvas[13][10]=3003;
		canvas[13][11]=3003;
		canvas[13][14]=3003;
		canvas[13][17]=3003;
		canvas[13][18]=3003;
		canvas[15][18]=3003;
		canvas[18][18]=3003;
		canvas[20][18]=3003;
		canvas[24][18]=3003;
		canvas[25][18]=3003;
		canvas[26][17]=3003;
		canvas[26][13]=3003;
		canvas[26][11]=3003;
		canvas[26][7]=3003;
		canvas[25][5]=3003;
		canvas[22][5]=3003;
		canvas[20][5]=3003;
		canvas[18][5]=3003;
		canvas[15][8]=3004;
		canvas[15][10]=3004;
		canvas[15][11]=3004;
		canvas[15][13]=3004;
		canvas[16][16]=3004;
		canvas[19][16]=3004;
		canvas[22][16]=3004;
		canvas[24][11]=3004;
		canvas[24][10]=3004;
		canvas[23][7]=3004;
		canvas[20][7]=3004;
		canvas[18][7]=3004;
		canvas[17][9]=4005;
		canvas[17][14]=4005;
		canvas[22][9]=4005;
		canvas[22][14]=4005;
		canvas[20][12]=9996;
	}
}
void ReDefineMonster()	//重新定义怪物，将被消灭的怪物赋值为1，以及出现而未击败的春之BOSS赋值为9998
{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if( map[FLOOR].newcanvas[i][j]==1 || map[FLOOR].newcanvas[i][j]==9998 )
			{
				canvas[i][j]=map[FLOOR].newcanvas[i][j];
			}
		}
	}
}
void DrawMonster()	//绘制怪物
{
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if(canvas[i][j]==1000)	//绘制春之妖精
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-Fairy.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-Fairy.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if( canvas[i][j]==9998 )	//绘制春之BOSS
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-SpringBoss.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-SpringBoss.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if( canvas[i][j]==9999 )	//绘制守门人
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-Porter.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-Porter.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if(canvas[i][j]==2001)	//绘制冰鬼
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-ice.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-ice.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if(canvas[i][j]==3002)	//绘制独眼南瓜
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster13-07_1_1.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster13-07_1_1.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if(canvas[i][j]==3003)	//绘制南瓜妖
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster13-07_2_1.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster13-07_2_1.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if(canvas[i][j]==3004)	//绘制南瓜王
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster13-07_3_1.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster13-07_3_1.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if( canvas[i][j]==9997 )	//绘制冬之BOSS
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-iceboss.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-iceboss.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if( canvas[i][j]==4005 )	//绘制大法师
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-magician.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-magician.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
			if( canvas[i][j]==9996 )	//绘制最终BOSS
			{
				loadimage(&img_monster, _T("E:\\魔塔project\\Resources\\Monster\\_Monster-FinalBOSS.jpg"));
				loadimage(&img_monster00, _T("E:\\魔塔project\\Resources\\Monster\\Monster-FinalBOSS.jpg"));
				putimage(i*32, j*32, &img_monster,NOTSRCERASE);
				putimage(i*32, j*32, &img_monster00,SRCINVERT);
			}
		}
	}
}
void DefineCrystal()	//在数组中定义传送水晶的值，区间为10000-10005
{
	if( FLOOR==0 )
	{
		canvas[20][0]=10001;
	}
	else if( FLOOR==1 )
	{
		canvas[9][22]=10000;
		canvas[12][5]=10002;
	}
	else if( FLOOR==2 )
	{
		canvas[9][1]=10001;
		canvas[11][21]=10003;
	}
	else if( FLOOR==3 )
	{
		canvas[11][20]=10002;
	}
}
void DrawCrystal()	//绘制传送水晶，作用类似于传统魔塔的楼梯
{
	if(start==1)
	{
	if( FLOOR==1 && bossappear==0 )
	{   
		isCrystal=0;
	}
	for(i=8;i<WIDTH;i++)
	{
 	for(j=0;j<HIGH;j++)
		{
			if( canvas[i][j]==10000 )	//绘制移动至初始层的传送水晶
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_T-Crystal.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\T-InitialCrystal.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
			if( canvas[i][j]==10001)	//绘制移动至春之层的传送水晶
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_T-Crystal.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\T-SpringCrystal.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
			if( canvas[i][j]==10002 && isCrystal==1)	//绘制移动至冬之层的传送水晶
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_T-Crystal.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\T-WinterCrystal.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
			if( canvas[i][j]==10003 )	//绘制移动至顶层的传送水晶
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_T-Crystal.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\T-InitialCrystal.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
		}			
	}
	}
}
void DefineTransfermatrix()//定义传送阵参数值为10006-10100
{
	if(FLOOR==2)
	{
		canvas[19][5]=10006;
		canvas[13][21]=10009;
		canvas[30][14]=10007;
		canvas[11][11]=10008;
	}
	if(FLOOR==3)
	{
		canvas[13][20]=10010;
		canvas[13][5]=10011;
		canvas[15][5]=10012;
		canvas[24][16]=10013;
		canvas[23][16]=0;
		canvas[24][14]=10014;
		canvas[19][11]=10015;
	}

}
void DrawTransfermatrix()//绘制传送阵
{
	
	for(i=8;i<WIDTH;i++)
	{
		for(j=0;j<HIGH;j++)
		{
			if( canvas[i][j]>=10006&&canvas[i][j]<=10009)	//绘制传送阵
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_Transfer matrix.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\Transfer matrix.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
			if( canvas[i][j]>=10010&&canvas[i][j]<=10015)	//绘制传送阵
			{
				loadimage(&img_tp1, _T("E:\\魔塔project\\Resources\\Terrain\\_Transfer matrix2.jpg"));
				loadimage(&img_tp01, _T("E:\\魔塔project\\Resources\\Terrain\\Transfer matrix2.jpg"));
				putimage(i*32, j*32, &img_tp1,NOTSRCERASE);
				putimage(i*32, j*32, &img_tp01,SRCINVERT);
			}
		}			
	}	
}
void DrawPlayer(char x)	//绘制玩家，w显示面朝前，s显示面朝后，a显示面朝左，d显示面朝右
{
	if(start==1)
	{
	if( x=='w' )
	{
		loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking10.jpg"));
		loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking10.jpg"));
		putimage(player_x*32, player_y*32, &img_player1,NOTSRCERASE);
		putimage(player_x*32, player_y*32, &img_player2,SRCINVERT);
	}
	else if( x=='s' )
	{
		loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior01.jpg"));
		loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior01.jpg"));
		putimage(player_x*32, player_y*32, &img_player1,NOTSRCERASE);
		putimage(player_x*32, player_y*32, &img_player2,SRCINVERT);
	}
	else if( x=='a' )
	{
		loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking04.jpg"));
		loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking04.jpg"));
		putimage(player_x*32, player_y*32, &img_player1,NOTSRCERASE);
		putimage(player_x*32, player_y*32, &img_player2,SRCINVERT);
	}
	else if( x=='d' )
	{
		loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking07.jpg"));
		loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking07.jpg"));
		putimage(player_x*32, player_y*32, &img_player1,NOTSRCERASE);
		putimage(player_x*32, player_y*32, &img_player2,SRCINVERT);
	}
	}
}

void Entrance()	//初始化除人物以外的所有内容，类似于画布一层层涂上去的效果进行先定义后绘制，以免背景图片丢失
{
	
	DefineBackground();
	DrawBackground();
	if(start==1)
	{
		DefineRoad();
		DrawRoad();
		DefineWall();
		DrawWall();
		DefineDoor();
		DrawDoor();
		Defineitems();
		Drawitems();
		DefineNPC();
		DrawNPC();
		DefineMonster();
		ReDefineMonster();
		DrawMonster();
		DefineCrystal();
		DrawCrystal();
		DefineTransfermatrix();
		DrawTransfermatrix();
	}
}

void DrawWithoutPlayer()	//绘制除人物与背景以外其他图片
{
	if(start==1)
	{
	if(FLOOR!=0)
	{
		DrawBackground();
	}
	DrawRoad();
	DrawWall();
	DrawDoor();
	Drawitems();
	DrawNPC();
	DrawMonster();
	DrawCrystal();
	DrawTransfermatrix();
	}
}

void DrawPanel()	//绘制人物面板
{
	if(start==1)
	{
		loadimage(&img_panel, _T("E:\\魔塔project\\Resources\\Character\\panel.jpg"));
		putimage(0, 0, &img_panel);
		settextcolor(WHITE);
		settextstyle(64, 0, _T("黑体"));
		transform(FLOOR,string);
		outtextxy(112, 32, string);		//绘制层数
		if( p.Level<10 )
		{
			transform(p.Level,string);
			outtextxy(144, 128, string);//绘制等级值
		}
		else
		{
			transform(p.Level,string);
			outtextxy(112, 128, string);//绘制等级值
		}
		transform(p.GKnum,string);
		outtextxy(144, 384, string);	//绘制绿钥匙数
		transform(p.RKnum,string);
		outtextxy(144, 448, string);	//绘制红钥匙数
		transform(p.YKnum,string);
		outtextxy(144, 512, string);	//绘制黄钥匙数
		transform(p.BKnum,string);
		outtextxy(144, 576, string);	//绘制蓝钥匙数
		settextstyle(32, 0, _T("黑体"));
		transform(p.Life,string);
		outtextxy(128, 192, string);	//绘制生命值
		transform(p.Attack,string);
		outtextxy(128, 224, string);	//绘制攻击力值
		transform(p.Defence,string);
		outtextxy(128, 256, string);	//绘制防御力值
		transform(p.GOLD,string);
		outtextxy(128, 288, string);	//绘制金币数
		transform(p.Exp,string);
		outtextxy(128, 319, string);	//绘制经验值
		if(OwnExcalibur)
		{
			loadimage(&img_excalibur1, _T("E:\\魔塔project\\Resources\\Item\\_excalibur.jpg"));
			loadimage(&img_excalibur2, _T("E:\\魔塔project\\Resources\\Item\\excalibur.jpg"));
			putimage(32, 672, &img_excalibur1,NOTSRCERASE);
			putimage(32, 672, &img_excalibur2,SRCINVERT);
		}
	}
}

void DrawCombatInterface(int num)	//绘制战斗界面
{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(400,250,880,520);
		settextcolor(WHITE);
		settextstyle(32, 0, _T("黑体"));
		TCHAR name1[] = _T("勇士");
		outtextxy(403, 253, name1);
		TCHAR LIFE1[] = _T("生命值");
		outtextxy(403, 290, LIFE1);
		transform(p.Life,string);
		outtextxy(550, 290, string);	//绘制战斗界面勇士生命值
		TCHAR ATTACK1[] = _T("攻击力");
		outtextxy(403, 327, ATTACK1);
		transform(p.Attack,string);
		outtextxy(550, 327, string);	//绘制战斗界面勇士攻击力值
		TCHAR DEFENCE1[] = _T("防御力");
		outtextxy(403, 364, DEFENCE1);
		transform(p.Defence,string);
		outtextxy(550, 364, string);	//绘制战斗界面勇士防御力值
		TCHAR name2[] = _T("怪物");
		outtextxy(813, 253, name2);
		TCHAR LIFE2[] = _T("生命值");
		outtextxy(781, 290, LIFE2);
		transform(m[num].life,string);
		outtextxy(700, 290, string);	//绘制战斗界面怪物生命值
		TCHAR ATTACK2[] = _T("攻击力");
		outtextxy(781, 327, ATTACK2);
		transform(m[num].att,string);
		outtextxy(700, 327, string);	//绘制战斗界面怪物攻击力值
		TCHAR DEFENCE2[] = _T("防御力");
		outtextxy(781, 364, DEFENCE2);
		transform(m[num].def,string);
		outtextxy(700, 364, string);	//绘制战斗界面怪物防御力值
		putimage(403,421, &img_player);
		settextcolor(RED);
		settextstyle(64, 0, _T("黑体"));
		TCHAR fight[] = _T("VS");
		outtextxy(608, 421, fight);
		putimage(813,421, &_m[num%10].img_mon);
		settextcolor(WHITE);
		settextstyle(16, 0, _T("黑体"));
		TCHAR quit[] = _T("撤退（Q）");
		outtextxy(800, 500, quit);
}

void startup()				//数据初始化
{
	dialogue=0;				//对话为0时不发生，>0时即对话进行中
	FLOOR=0;				//初始位于魔塔外景，视作在第0层
	p.Level=1;				//初始人物等级为1	
	p.Life=1000;			//初始生命值为1000	
	p.Attack=10;			//初始攻击力为10	
	p.Defence=10;			//初始防御力为10
	p.GOLD=0;				//初始持有金币数为0
	//p.GOLD=100;	            //测试数据，金币持有100
	p.Exp=0;				//初始拥有经验值为0
	p.GKnum=0;					
	p.RKnum=0;					
	p.YKnum=0;					
	p.BKnum=0;				//初始拥有各类钥匙数为0	
	OwnExcalibur=1;			//初始拥有圣剑
	isdone=0;				//0为未执行将装备属性赋给人物，1为已经执行
	isscan=0;				//0为未处于查看道具属性状态中，1为正处于查看状态
	iscolor=0;				//0为鼠标未移至可点击文字部分，不作变色处理，1为已移至，执行变色提示
	isbattle=0;				//0为未处于战斗中，1为与怪物战斗，打开战斗界面
	battleover=0;			//0为战斗未结束，1为战斗结束
	bossappear=0;			//0为BOSS未出现，1为已经出现
	isCrystal=1;			//0为不显示，1为显示
	gameover=0;				//0为游戏未结束，1为游戏结束
	isgiven=0;				//0为未给予任何道具，1为给予了黄钥匙，2为给予了精灵吊坠
    isgivenG=0;
	player_x=20;
	player_y=HIGH-1;		//玩家初始位置
	//player_x=20;
	//player_y=5;  //测试2层玩家位置
	//player_x=11;
	//player_y=19;  //测试3层玩家位置
	step=8;					//步长，为1/4的格子大小
	m[1000].life=10;		//怪物1生命值为10
	m[1000].att=21;			//怪物1攻击力为21
	m[1000].def=7;			//怪物1防御力为7
	m[1000].exp=1;			//击败怪物1获得经验值为1
	m[1000].Gold=1;			//击败怪物1获得金币为1
	m[2001].life=70;		//怪物2生命值为70
	m[2001].att=63;			//怪物2攻击力为60
	m[2001].def=44;			//怪物2防御力为40
	m[2001].exp=5;			//击败怪物2获得经验值为5
	m[2001].Gold=5;			//击败怪物2获得金币为5
	m[3002].life=90;		//怪物3生命值为90
	m[3002].att=77;			//怪物3攻击力为77
	m[3002].def=54;			//怪物3防御力为54
	m[3002].exp=2;			//击败怪物3获得经验值为2
	m[3002].Gold=2;			//击败怪物3获得金币为2
	m[3003].life=120;		//怪物4生命值为120
	m[3003].att=83;			//怪物4攻击力为83
	m[3003].def=61;			//怪物4防御力为61
	m[3003].exp=4;			//击败怪物4获得经验值为4
	m[3003].Gold=3;			//击败怪物4获得金币为3
	m[3004].life=150;		//怪物5生命值为150
	m[3004].att=99;			//怪物5攻击力为99
	m[3004].def=67;			//怪物5防御力为67
	m[3004].exp=6;			//击败怪物5获得经验值为6
	m[3004].Gold=5;			//击败怪物5获得金币为5
	m[4005].life=250;		//怪物6生命值为250
	m[4005].att=120;		//怪物6攻击力为120
	m[4005].def=75;			//怪物6防御力为75
	m[4005].exp=10;			//击败怪物6获得经验值为10
	m[4005].Gold=8;			//击败怪物6获得金币为8
	m[9998].life=300;		//BOSS1生命值为300
	m[9998].att=100;		//BOSS1攻击力为100
	m[9998].def=30;			//BOSS1防御力为30
	m[9998].exp=50;			//击败BOSS1获得经验值为50
	m[9998].Gold=30;		//击败BOSS1获得金币为30
	m[9997].life=500;		//BOSS2生命值为500
	m[9997].att=200;		//BOSS2攻击力为200
	m[9997].def=100;		//BOSS2防御力为100
	m[9997].exp=100;		//击败BOSS2获得经验值为100
	m[9997].Gold=50;		//击败BOSS2获得金币为50
	m[9996].life=10000;		//最终BOSS生命值为10000
	m[9996].att=5000;		//最终BOSS攻击力为5000
	m[9996].def=5000;		//最终BOSS防御力为5000
	m[9996].exp=1000;		//击败最终BOSS获得经验值为1000
	m[9996].Gold=500;		//击败最终BOSS获得金币为500

	for(i=0;i<6;i++)
	{
		for(j=8;j<WIDTH;j++)
		{
			for(z=0;z<HIGH;z++)
			{
				map[i].newcanvas[j][z]=0;	//将新画布全部初始化赋值为0
			}
		}
	}

	initgraph(1024,768);	//定义画布大小
	BeginBatchDraw();	
	loadimage(&img_player, _T("E:\\魔塔project\\Resources\\Character\\WarriorB.jpg"));
	loadimage(&_m[0].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-FairyB.jpg"));
	loadimage(&_m[1].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-iceB.jpg"));
	loadimage(&_m[8].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-SpringBossB.jpg"));
	loadimage(&_m[7].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-icebossB.jpg"));
	loadimage(&_m[2].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-PumpkinSingleEyeB.jpg"));
	loadimage(&_m[3].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-PumpkinB.jpg"));
	loadimage(&_m[4].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-PumpkinKingB.jpg"));
	loadimage(&_m[5].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-magicianB.jpg"));
	loadimage(&_m[6].img_mon, _T("E:\\魔塔project\\Resources\\Monster\\Monster-FinalBOSSB.jpg"));
	if(start==0)
	{
	loadimage(&img_bk, _T("E:\\魔塔project\\Resources\\Map\\start.jpg"));
	putimage(0,0, &img_bk);
	}
	else
	{
		Entrance();
		DrawPlayer('w');
		DrawPanel();
	}
	
}


void show()						//显示画面
{

	if( isscan==0 )			//在未查看道具属性时将人物面板刷屏
	{
		DrawPanel();
	}
	if(start==1&&isdone2==0&&FLOOR==0)
	{
		Entrance();
		DrawPlayer('w');
		DrawPanel();
		isdone2=1;
	}
	FlushBatchDraw();
	Sleep(10);
}

void updateWithoutInput()	//与用户输入无关的更新
{
	if(FLOOR!=0&&OwnExcalibur==1)
	{
       test=1;
	}
	if(start==0&&tips==0)
	{
		loadimage(&img_bk, _T("E:\\魔塔project\\Resources\\Map\\start.jpg"));
		putimage(0,0, &img_bk);
	}
	else if(start==0&&tips==1)
	{
		loadimage(&img_bk, _T("E:\\魔塔project\\Resources\\Map\\tips.jpg"));
		putimage(0,0, &img_bk);
	}

	//判断执行对话剧情
	if( dialogue==1 && iscolor==0 )	//与守门人的对话
	{
		DrawPanel();	//防止发生对话时人物面板道具介绍仍显示，下同
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(500,480,750,580);
		settextcolor(WHITE);
		settextstyle(20, 0, _T("黑体"));
		TCHAR name[] = _T("守门人");
		outtextxy(503, 483, name);
		settextstyle(16, 0, _T("宋体"));
		TCHAR d1[] = _T("欲入此门者，必先失其重要之物，");
		outtextxy(505,508,d1);
		TCHAR d2[] = _T("然汝亦要前行？");
		outtextxy(505,529,d2);
		TCHAR yes[] = _T("是");
		outtextxy(593,550,yes);
		TCHAR no[] = _T("否");
		outtextxy(647,550,no);
	}
	if( dialogue==2 && iscolor==0 )	//与精灵的对话
	{
		if( isgiven==0 )
		{
			DrawPanel();
			fillrectangle(750,100,1000,200);
			settextstyle(20, 0, _T("黑体"));
			settextcolor(WHITE);
			TCHAR name[] = _T("精灵");
			outtextxy(753, 103, name);
			settextstyle(16, 0, _T("宋体"));
			TCHAR d1[] = _T("我的玲珑钥匙四散于这座塔内，如");
			outtextxy(755,128,d1);
			TCHAR d2[] = _T("如果你能找齐钥匙，我可以为你提");
			outtextxy(755,149,d2);
			TCHAR d3[] = _T("供一些帮助。");
			outtextxy(755,170,d3);
			TCHAR con[] = _T("-点此继续-");
			outtextxy(917,181,con);
		}
		else if( isgiven==1 && (p.YKnum!=1 || p.BKnum!=1 || p.GKnum!=1 || p.RKnum!=1) )
		{
			DrawPanel();
			fillrectangle(750,100,1000,200);
			settextstyle(20, 0, _T("黑体"));
			settextcolor(WHITE);
			TCHAR name[] = _T("精灵");
			outtextxy(753, 103, name);
			settextstyle(16, 0, _T("宋体"));
			TCHAR d1[] = _T("你找齐我的钥匙了吗？求求你，这");
			outtextxy(755,128,d1);
			TCHAR d2[] = _T("真的对我很重要，不然我就回不了");
			outtextxy(755,149,d2);
			TCHAR d3[] = _T("家了，快帮我找找吧！");
			outtextxy(755,170,d3);
			TCHAR con[] = _T("-点此继续-");
			outtextxy(917,181,con);
		}
		else if( isgiven==1 && p.YKnum==1 && p.BKnum==1 && p.GKnum==1 && p.RKnum==1 )
		{
			DrawPanel();
			fillrectangle(750,100,1000,200);
			settextstyle(20, 0, _T("黑体"));
			settextcolor(WHITE);
			TCHAR name[] = _T("精灵");
			outtextxy(753, 103, name);
			settextstyle(16, 0, _T("宋体"));
			TCHAR d1[] = _T("谢谢你为我找齐了钥匙！这个吊坠");
			outtextxy(755,128,d1);
			TCHAR d2[] = _T("就送给你吧！");
			outtextxy(755,149,d2);
			TCHAR con[] = _T("-点此继续-");
			outtextxy(917,181,con);
		}
		else if( isgiven==2 )
		{
			DrawPanel();
			fillrectangle(750,100,1000,200);
			settextstyle(20, 0, _T("黑体"));
			settextcolor(WHITE);
			TCHAR name[] = _T("精灵");
			outtextxy(753, 103, name);
			settextstyle(16, 0, _T("宋体"));
			TCHAR d1[] = _T("太感谢你了！愿神灵庇佑你！");
			outtextxy(755,128,d1);
			TCHAR con[] = _T("-点此继续-");
			outtextxy(917,181,con);
		}
	}
	if( dialogue==3 && iscolor==0 )	//与智者的对话
	{
		DrawPanel();
		fillrectangle(400,200,650,300);
		settextstyle(20, 0, _T("黑体"));
		settextcolor(WHITE);
		TCHAR name[] = _T("智者");
		outtextxy(403, 203, name);
		settextstyle(16, 0, _T("宋体"));
		TCHAR d1[] = _T("据说最终魔王身边有四位大法师展");
		outtextxy(405,228,d1);
		TCHAR d2[] = _T("开结界守护，不先击败大法师破除");
		outtextxy(405,249,d2);
		TCHAR d3[] = _T("结界，魔王无法击败。");
		outtextxy(405,270,d3);
		TCHAR con[] = _T("-点此继续-");
		outtextxy(567,281,con);
	}
	if( dialogue==4 && iscolor==0 )	//与看门大爷的对话
	{
		DrawPanel();
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(300,600,600,700);
		settextstyle(16, 0, _T("黑体"));
		settextcolor(WHITE);
		TCHAR name[] = _T("看门大爷");
		outtextxy(303, 603, name);
		settextstyle(12, 0, _T("宋体"));
		TCHAR d1[] = _T("  冒险者，欢迎来到春之层，这层的特性是生长。");
		outtextxy(305,624,d1);
		TCHAR d2[] = _T("魔王正在沉睡，要打完所有的小兵才能唤醒魔王。");
		outtextxy(305,644,d2);
		TCHAR d3[] = _T("魔王的属性取决于你的清兵速度，抓紧时间吧！");
		outtextxy(305,664,d3);
		TCHAR con[] = _T("-点此继续-");
		outtextxy(527,681,con);
	}
	if( dialogue==6 && iscolor==0 )	//与冰雪女王的对话
	{
		DrawPanel();
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(300,300,600,400);
		settextstyle(16, 0, _T("黑体"));
		settextcolor(WHITE);
		TCHAR name[] = _T("冰雪女王");
		outtextxy(303, 303, name);
		settextstyle(12, 0, _T("宋体"));
		TCHAR d1[] = _T("  冒险者，欢迎来到冬之层，这层的特性是衰败。");
		outtextxy(305,324,d1);
		TCHAR d2[] = _T("你的属性将会随你的走动步数的增长而下跌。发挥");
		outtextxy(305,344,d2);
		TCHAR d3[] = _T("你的聪明才智去冒险吧！");
		outtextxy(305,364,d3);
		TCHAR con[] = _T("-点此继续-");
		outtextxy(527,381,con);
	}
	if( dialogue==5 && iscolor==0 )	//与扫地大妈的对话
	{
		DrawPanel();
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(512,128,790,275);
		settextstyle(16, 0, _T("黑体"));
		settextcolor(WHITE);
		TCHAR name[] = _T("扫地大妈");
		outtextxy(515,131, name);
		settextstyle(12, 0, _T("宋体"));
		TCHAR d1[] = _T("  我这里可以用金币兑换属性，孩子要试试吗？");
		outtextxy(529,148,d1);
		TCHAR d2[] = _T("     套餐一 +50生命 10金     BUY");
		outtextxy(515,165,d2);
		TCHAR d3[] = _T("     套餐二 +10攻击 10金     BUY");
		outtextxy(515,195,d3);
		TCHAR d4[] = _T("     套餐三 +10防御 10金     BUY");
		outtextxy(515,225,d4);
		TCHAR con[] = _T("-点此继续-");
		outtextxy(720,255,con);
	}
	if(OwnExcalibur && isdone==0)	//持有圣剑时附加属性
	{
		p.Attack += 1000;
		isdone=1;
		DrawPanel();
	}
	if(OwnExcalibur==0 && isdone)	//失去圣剑时属性恢复
	{
		p.Attack -= 1000;
		isdone=0;
		p.RKnum=1;
		DrawPanel();
	}
	if( player_x==20 && player_y==0 && FLOOR==0 )	//在0层时升至1层的转换
	{
		player_x=9;
		player_y=21;
		FLOOR=1;
		Entrance();
		DrawPlayer('d');
		FlushBatchDraw();
	}
	else if( player_x==9 && player_y==22 && FLOOR==1 )	//在1层时降至0层的转换
	{
		player_x=20;
		player_y=1;
		timestart=0;
		FLOOR=0;
		DefineBackground();
		DrawBackground();
		DefineRoad();
		DrawRoad();
		DefineWall();
		DrawWall();
		DefineNPC();
		DrawNPC();
		DefineCrystal();
		DrawCrystal();
		DrawPlayer('s');
		FlushBatchDraw();
	}
	else if( player_x==12 && player_y==5 && FLOOR==1 )	//在1层时升至2层的转换
	{
		player_x=9;
		player_y=2;
		FLOOR=2;
		Entrance();
		DrawPlayer('s');
		FlushBatchDraw();
		//move=1;
	}
	else if( player_x==9 && player_y==1 && FLOOR==2 )	//在2层时降至1层的转换
	{
		player_x=13;
		player_y=5;
		FLOOR=1;
		DefineBackground();
		DrawBackground();
		DefineRoad();
		DrawRoad();
		DefineWall();
		DrawWall();
		DefineNPC();
		DrawNPC();
		DefineCrystal();
		DrawCrystal();
		DrawPlayer('d');
		FlushBatchDraw();
	}
	else if( player_x==11 && player_y==21 && FLOOR==2 )	//在2层时升至顶层的转换
	{
		player_x=11;
		player_y=19;
		FLOOR=3;
		DefineBackground();
		DrawBackground();
		DefineRoad();
		DrawRoad();
		DefineWall();
		DrawWall();
		Defineitems();
		Drawitems();
		DefineNPC();
		DrawNPC();
		DefineMonster();
		ReDefineMonster();
		DrawMonster();
		DefineCrystal();
		DrawCrystal();
		DefineTransfermatrix();
		DrawTransfermatrix();
		DrawPlayer('w');
		FlushBatchDraw();
	}
	else if( player_x==11 && player_y==20 && FLOOR==3 )	//在顶层时降至2层的转换
	{
		player_x=12;
		player_y=21;
		FLOOR=2;
		DefineBackground();
		DrawBackground();
		DefineRoad();
		DrawRoad();
		Defineitems();
		Drawitems();
		DefineWall();
		DrawWall();
		DefineNPC();
		DrawNPC();
		DefineMonster();
		ReDefineMonster();
		DrawMonster();
		DefineCrystal();
		DrawCrystal();
		DefineTransfermatrix();
		DrawTransfermatrix();
		DrawPlayer('d');
		FlushBatchDraw();
	}
	if(FLOOR==2)//实现冬之层衰败特性
	{
		if(move==1)
			go=1;
		if(counter2%10==0&&go==1)
		{	
			if(p.Life>200)
			p.Life=p.Life-(p.Life*counter2)/1000;
			
			if(p.Attack>10)
			p.Attack=p.Attack-(p.Attack*counter2)/500;
			
			if(p.Defence>10)
			p.Defence=p.Defence-(p.Defence*counter2)/500;

			if(p.Life<=200) {p.Life=200;counter2=0;}
			if(p.Attack<=10){p.Attack=10;counter2=0;}
			if(p.Defence<=10){p.Defence=10;counter2=0;}
			DrawPanel();
			go=0;
			move=-move;
		}
	    }
	if(FLOOR==1)//实现春之层“生长”特性
	{
		int num=canvas[player_x][player_y];	//存储怪物对应值
		m[num].life=m[num].life+m[num].life*(time/60)/3;
		m[num].att=m[num].att+m[num].att*(time/60)/20;
		m[num].def=m[num].def+m[num].def*(time/60)/10;
		LIFE=m[num].life;
	}
	
	if ( isbattle )	//进入战斗流程
	{
		int num=canvas[player_x][player_y];	//存储怪物对应值
		LIFE=m[num].life;	//存储怪物生命值
		DrawCombatInterface(num);
		FlushBatchDraw();
		delay(1000);
		while( m[num].life>0 && p.Life>0 )	//只要怪物和玩家生命值为正，战斗继续
		{
			char input;
			if( _kbhit() )
			{
				input=_getch();
				if( input=='q' )
				{
					battleover=1;
					m[num].life=LIFE;	//重置怪物生命值，防止下一次战斗相同怪物的生命值为0
					break;
				}
			}
			if( m[num].att-p.Defence>0 )
				p.Life-=(m[num].att-p.Defence);
			if( m[num].def<p.Attack )
				m[num].life-=(p.Attack-m[num].def);
			DrawCombatInterface(num);
			FlushBatchDraw();
			delay(1000);
		}
		if( battleover==0 )
		{
			p.Exp+=m[num].exp;	//获得击败怪物应得经验值
			p.GOLD+=m[num].Gold;	//获得击败怪物应得金币数
			canvas[player_x][player_y]=1;
			map[FLOOR].newcanvas[player_x][player_y]=1;	//记录下怪物被击败
			m[num].life=LIFE;	//重置怪物生命值，防止下一次战斗相同怪物的生命值为0
			battleover=1;	//战斗结束标志
		}	
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}

	if(m[9998].life<=0)
	{
		if(isgivenG==0)
		{
			setfillcolor(BLACK);
			setlinecolor(RGB(255,20,180));
			setlinestyle(PS_SOLID ,3);
			fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
			settextcolor(LIGHTRED);
			settextstyle(20, 0, _T("隶书"));
			TCHAR d1[] = _T("获得绿钥匙！");
			outtextxy(player_x*32-70,player_y*32-10,d1);
			FlushBatchDraw();
			delay(500);
		p.GKnum+=1;
		DrawPanel();
		isgivenG=1;
		}
	}
	if(canvas[player_x][player_y]==10)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得短剑！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Attack+=10;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==11)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得木盾！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Defence+=10;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==12)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得红宝石！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Attack+=2;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==13)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得蓝宝石！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(1000);
		p.Defence+=2;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==20)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得红药水！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Life+=50;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==30)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得紫药水！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Life+=100;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==21)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得精钢剑！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Attack+=20;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==22)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得钢盾！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Defence+=20;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==31)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得惊雷剑！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Attack+=50;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==32)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得神霄甲！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.Defence+=50;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
	if(canvas[player_x][player_y]==25)
	{
		setfillcolor(BLACK);
		setlinecolor(RGB(255,20,180));
		setlinestyle(PS_SOLID ,3);
		fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
		settextcolor(LIGHTRED);
		settextstyle(20, 0, _T("隶书"));
		TCHAR d1[] = _T("获得蓝钥匙！");
		outtextxy(player_x*32-70,player_y*32-10,d1);
		FlushBatchDraw();
		delay(500);
		p.BKnum+=1;
		canvas[player_x][player_y]=0;
		DrawPanel();
		DrawWithoutPlayer();
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
   if(canvas[player_x][player_y]==10006)//传送阵转移1-1
	{
		player_x=13;
		player_y=21;
		//DrawTransfermatrix();
		DrawPanel();
		DefineTransfermatrix();
		DrawWithoutPlayer();
		canvas[13][21]=0;
		DrawPlayer(lastinput);	//重新绘制所有界面
		FlushBatchDraw();
	}
   if( canvas[player_x][player_y]==10009)//传送阵转移1-2
   {
	   player_x=19;
	   player_y=5;
	    //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[19][5]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10007)//传送阵转移2-1
   {
	   player_x=11;
	   player_y=11;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[11][11]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10008)//传送阵转移2-2
   {
	   player_x=30;
	   player_y=14;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[30][14]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10010)//传送阵转移3-1
   {
	   player_x=13;
	   player_y=5;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[13][5]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10011)//传送阵转移3-2
   {
	   player_x=13;
	   player_y=20;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[13][20]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10012)//传送阵转移3-3
   {
	   player_x=24;
	   player_y=16;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[24][16]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10013)//传送阵转移3-4
   {
	   player_x=15;
	   player_y=5;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[15][5]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10014)//传送阵转移3-5
   {
	   player_x=19;
	   player_y=11;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[19][11]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }
   if( canvas[player_x][player_y]==10015)//传送阵转移3-6
   {
	   player_x=24;
	   player_y=14;
	   //DrawTransfermatrix();
	   DrawPanel();
	   DefineTransfermatrix();
	   DrawWithoutPlayer();
	   canvas[24][14]=0;
	   DrawPlayer(lastinput);	//重新绘制所有界面
	   FlushBatchDraw();
   }

	if( p.Exp>=p.Level*10 )	//经验值达到当前等级10的倍数时升级
	{
		p.Exp-=p.Level*10;	//扣去升级所需经验值
		p.Level++;	//等级提升
		p.Life+=50;	//升级时生命值提高50
		p.Attack++;	//升级时攻击力提高1
		p.Defence++;	//升级时防御力提高1
		DrawPanel();
		FlushBatchDraw();
	}

	if( FLOOR==1 )
	{
		int boss=1;	//判断执行BOSS是否出现
		for(i=0;i<WIDTH;i++)
		{
			for(j=0;j<HIGH;j++)
			{
				if( canvas[i][j]==1000 )
				{
					boss=0;	//当本层其他怪物未被清光时，BOSS不出现
				}
			}
		}
		if( boss && bossappear==0 )	//重新绘制怪物，令BOSS出现
		{
			canvas[13][5]=9998;
			map[FLOOR].newcanvas[13][5]=9998;//记录下BOSS出现
			DrawMonster();
			bossappear=1;
			isCrystal=1;
			DrawCrystal();
			FlushBatchDraw();
		}
	}

	if( FLOOR==3 )
	{
		int boss=5;	//判断执行BOSS是否削弱，5为完全削弱，1为最强强化状态
		for(i=0;i<WIDTH;i++)
		{
			for(j=0;j<HIGH;j++)
			{
				if( canvas[i][j]==4005 )
				{
					boss--;	//当本层大法师存在时，BOSS仍旧被强化
				}
			}
		}
		if( boss )	//重新定义怪物属性，削弱BOSS
		{
			m[9996].life=10000/boss;	//最终BOSS生命值按击败大法师数削弱
			m[9996].att=5000/boss;		//最终BOSS攻击力按击败大法师数削弱
			m[9996].def=5000/boss;		//最终BOSS防御力按击败大法师数削弱
		}
		if( boss==5 )	//完全削弱
		{
			m[9996].life=1000;			//最终BOSS生命值完全削弱后为1000
			m[9996].att=250;			//最终BOSS攻击力完全削弱后为250
			m[9996].def=250;			//最终BOSS防御力完全削弱后为250
		}
	}

	if( p.Life<=0 )	//玩家生命值非正，游戏结束
	{
		gameover=1;
	}

	if( gameover )	//游戏结束显示
	{
		cleardevice();	//清空画面
		settextcolor(RED);
		settextstyle(64, 0, _T("黑体"));
		RECT r = {0, 0, 1024, 768};
		drawtext(_T("Game Over!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	//绘制游戏结束信息
		FlushBatchDraw();
		EndBatchDraw();
		_getch();	//按任意键继续
		closegraph();	//关闭画布
		exit(0);	//退出游戏
	}
}

void updateWithInput()		//与用户输入有关的更新 
{
	char input;
	MOUSEMSG m; 
	while(_kbhit())
	{
		DefineTransfermatrix();
		input=_getch();
		if(start==0)
		{
			if(input=='1'&&tips==0)
				start=1;
			else if(input=='3')
				tips=1;
		}
		if(tips==1)
		{
			if(input=='z')
				tips=0;
		}
		if(OwnExcalibur==0)
		{
			if(input=='n')
				OwnExcalibur=1;
		}
		if(FLOOR==1)
		{
			if(timestart==0)
			{
				GetLocalTime(&ti);
				oldtime=ti.wSecond +ti.wMinute*60+ti.wHour*3600;
				timestart=1;
			}
			
			if(timestart==1)
			{
				GetLocalTime(&ti);// 获取当前时间
				newtime= ti.wSecond +ti.wMinute*60+ti.wHour*3600;  // 结算秒数
			    time=newtime-oldtime;
				
			}
		}
		else if(FLOOR==2)
		{
			counter2+=1;
			move=-move;
		}
		lastinput=input;
		if( dialogue==0 && isbattle==0 )		//在无对话、无战斗情况下进行行走、对话判断
		{
			if( input=='w' )
			{
				DrawWithoutPlayer();	//每次行走前用除玩家外的其他图案刷屏
				DrawPlayer('w');	//无论是否执行行走动作，都要进行转向动作，绘制转向后人物，之后同理
				if( canvas[player_x][player_y-1]==9999 )	//遭遇守门人
				{
					dialogue=1;
				}
				else if( canvas[player_x][player_y-1]==100 )	//与精灵对话
				{
					dialogue=2;
				}
				else if( canvas[player_x][player_y-1]==101 )	//与智者对话
				{
					dialogue=3;
				}
				else if( canvas[player_x][player_y-1]==102 )	//与宿管大妈对话
				{
					dialogue=5;
				}
			/*	else if( canvas[player_x][player_y-1]==4 )
				{
				}*/
				else if( canvas[player_x][player_y-1]!=2 && player_y>0 )		//向上走时前方不为墙，且未越过画布上边界，执行行走动作，绘制行走动画，之后行走判断类似同理
				{
					DrawWithoutPlayer();	//每次移动用除玩家以外的图案刷屏，下同
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking11.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking11.jpg"));
					putimage(player_x*32, player_y*32-step, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32-step, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();		//每次移动完后绘制，防止移动动画不可见，下同
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking10.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking10.jpg"));
					putimage(player_x*32, player_y*32-step*2, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32-step*2, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking12.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking12.jpg"));
					putimage(player_x*32, player_y*32-step*3, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32-step*3, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					player_y--;		//每次移动为一格，仅最后一步移动改变玩家位置，之后同
					DrawPlayer('w');
					delay(1);
					FlushBatchDraw();
					if( canvas[player_x][player_y]==9 || canvas[player_x][player_y]==4005 )	//玩家移动至火焰路所在格时，生命值受损，下同
					{
						p.Life-=50;
						DrawPanel();
					}
					if( canvas[player_x][player_y]>=1000 && canvas[player_x][player_y]<=9998 )	//玩家移动至怪物所在格时，发生战斗，下同
					{
						isbattle=1;
					}
				}
			}
			else if( input=='s' )
			{
				DrawWithoutPlayer();
				DrawPlayer('s');
				if( canvas[player_x][player_y+1]==103)	//与看门大爷对话
				{
					dialogue=4;
				}
			/*	if( canvas[player_x][player_y+1]==4 )
				{
				}*/
				else if( canvas[player_x][player_y+1]!=2 && player_y<HIGH-1 )
				{
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking02.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking02.jpg"));
					putimage(player_x*32, player_y*32+step, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32+step, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior01.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior01.jpg"));
					putimage(player_x*32, player_y*32+step*2, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32+step*2, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking03.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking03.jpg"));
					putimage(player_x*32, player_y*32+step*3, &img_player1,NOTSRCERASE);
					putimage(player_x*32, player_y*32+step*3, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					player_y++;
					DrawPlayer('s');
					delay(1);
					FlushBatchDraw();
					if( canvas[player_x][player_y]==9 || canvas[player_x][player_y]==4005 )	//玩家移动至火焰路所在格时，生命值受损，下同
					{
						p.Life-=50;
						DrawPanel();
					}
					if( canvas[player_x][player_y]>=1000 && canvas[player_x][player_y]<=9998 )
					{
						isbattle=1;
					}
				}
			}
			else if( input=='a' )
			{
				DrawWithoutPlayer();
				DrawPlayer('a');
				if( canvas[player_x-1][player_y]==9999 )	//如果从左面接触守门人，不发生剧情
				{

				}
				else if( canvas[player_x-1][player_y]==104)	//与冰雪女王对话
				{
					dialogue=6;
				}
				
			/*	else if( canvas[player_x-1][player_y]==4 )
				{
				} */
				else if( canvas[player_x-1][player_y]!=2  && player_x>0 )
				{
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking05.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking05.jpg"));
					putimage(player_x*32-step, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32-step, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking04.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking04.jpg"));
					putimage(player_x*32-step*2, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32-step*2, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking06.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking06.jpg"));
					putimage(player_x*32-step*3, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32-step*3, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					player_x--;
					DrawPlayer('a');
					delay(1);
					FlushBatchDraw();					
					if( canvas[player_x][player_y]==9 || canvas[player_x][player_y]==4005 )	//玩家移动至火焰路所在格时，生命值受损，下同
					{
						p.Life-=50;
						DrawPanel();
					}
					if( canvas[player_x][player_y]>=1000 && canvas[player_x][player_y]<=9998 )
					{
						isbattle=1;
					}
				}
			}
			else if( input=='d' )
			{
				DrawWithoutPlayer();
				DrawPlayer('d');
				if( canvas[player_x+1][player_y]==9999 )	//如果从右面接触守门人，不发生剧情
				{

				}
			/*	else if( canvas[player_x+1][player_y]==4 )
				{
				}*/
				else if( canvas[player_x+1][player_y]==103)	//与看门大爷对话
				{
					dialogue=4;
				}
				else if( canvas[player_x+1][player_y]!=2 && player_x<WIDTH-1 )
				{
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking08.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking08.jpg"));
					putimage(player_x*32+step, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32+step, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking07.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking07.jpg"));
					putimage(player_x*32+step*2, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32+step*2, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					loadimage(&img_player1, _T("E:\\魔塔project\\Resources\\Character\\_Warrior-Walking09.jpg"));
					loadimage(&img_player2, _T("E:\\魔塔project\\Resources\\Character\\Warrior-Walking09.jpg"));
					putimage(player_x*32+step*3, player_y*32, &img_player1,NOTSRCERASE);
					putimage(player_x*32+step*3, player_y*32, &img_player2,SRCINVERT);
					delay(1);
					FlushBatchDraw();
					DrawWithoutPlayer();
					player_x++;
					DrawPlayer('d');
					delay(1);
					FlushBatchDraw();					
					if( canvas[player_x][player_y]==9 || canvas[player_x][player_y]==4005 )	//玩家移动至火焰路所在格时，生命值受损，下同
					{
						p.Life-=50;
						DrawPanel();
					}
					if( canvas[player_x][player_y]>=1000 && canvas[player_x][player_y]<=9998 )
					{
						isbattle=1;
					}
				}		
			}
			else if( input=='x' && OwnExcalibur )	//拥有圣剑时按X一键清兵，方便测试
			{
				for(i=0;i<WIDTH;i++)
				{
					for(j=0;j<HIGH;j++)
					{
						if( canvas[i][j]>=1000 && canvas[i][j]<=9999 )
						{
							canvas[i][j]=1;			
						}
					}
				}
				DrawWithoutPlayer();
				FlushBatchDraw();
			}
		}
	}
	if( battleover && isbattle )	//防止在战斗中按其他键导致战斗结束后自动移动
	{
		isbattle=0;	
		battleover=0;	//结束战斗
	}
	while(MouseHit())
	{
		m = GetMouseMsg();
		if( dialogue==0 && isbattle==0 )	//在无对话、无战斗情况下能够查看道具属性
		{
			if( m.x>=32 && m.y>=672 && m.y<=704 && m.x<=64 && OwnExcalibur && isscan==0 )
			{
				DrawPanel();
				setfillcolor(BLACK);
				setlinecolor(RGB(255,20,180));
				setlinestyle(PS_SOLID ,3);
				fillrectangle(m.x,m.y,m.x+180,m.y+70);
				settextstyle(20, 0, _T("黑体"));
				TCHAR name[] = _T("圣剑");
				outtextxy(m.x+3,m.y+3,name);
				settextstyle(16, 0, _T("宋体"));
				TCHAR i1[] = _T("最强之剑。");
				outtextxy(m.x+5,m.y+28,i1);
				TCHAR i2[] = _T("持有者攻击力+1000。");
				outtextxy(m.x+5,m.y+49,i2);
				isscan=1;	//变更为正在查看道具属性状态
			}
			else
			{
				isscan=0;	//鼠标移开后变为非查看道具属性状态
			}
		}
		else if( dialogue==1 )	//控制与守门人对话继续游戏
		{
			if( m.x>=593 && m.y>=550 && m.y<=566 && m.x<=609 )	//鼠标移动至“是”字，字体变色以提示
			{			
				settextcolor(RED);
				settextstyle(16, 0, _T("宋体"));
				TCHAR yes[] = _T("是");
				outtextxy(593,550,yes);
				iscolor=1;	//着色提示玩家点击，下同
				if( m.uMsg==WM_LBUTTONDOWN )	//点击对话框“是”的部分，守门人和门消失，且失去圣剑
				{
					canvas[20][16]=1;
					canvas[20][15]=1;
					DefineBackground();
					DrawBackground();
					DefineRoad();
					DrawRoad();
					DefineWall();
					DrawWall();
					DefineNPC();
					DrawNPC();
					DrawPlayer('w');
					DefineCrystal();
					DrawCrystal();
					setfillcolor(BLACK);
					setlinecolor(RGB(255,20,180));
					setlinestyle(PS_SOLID ,3);
					fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
					settextcolor(GREEN);
					settextstyle(20, 0, _T("隶书"));
					TCHAR d1[] = _T("获得红钥匙！");
					outtextxy(player_x*32-70,player_y*32-10,d1);
					FlushBatchDraw();
					delay(500);
					DefineBackground();
					DrawBackground();
					DefineRoad();
					DrawRoad();
					DefineWall();
					DrawWall();
					DefineNPC();
					DrawNPC();
					DrawPlayer('w');
					DefineCrystal();
					DrawCrystal();
					dialogue=0;
					OwnExcalibur=0;
				}
			}
			else if( m.x>=647 && m.y>=550 && m.y<=566 && m.x<=663 )	//鼠标移动至“否”字，字体变色以提示
			{			
				settextcolor(RED);
				settextstyle(16, 0, _T("宋体"));
				TCHAR no[] = _T("否");
				outtextxy(647,550,no);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )	//点击对话框“否”的部分，恢复原状
				{
					Entrance();
					DrawPlayer('w');
					dialogue=0;
				}
			}
			else
			{
				iscolor=0;	//鼠标移开后恢复原色，下同
			}
		}
		else if( dialogue==2 )	//控制与精灵对话继续游戏
		{
			
			if( m.x>=917 && m.y>=181 && m.y<=197 && m.x<=997 )	//鼠标移动到“继续部分”，字体变色来提示，下同
			{			
				settextcolor(RED);
				settextstyle(16, 0, _T("宋体"));
				TCHAR con[] = _T("-点此继续-");
				outtextxy(917,181,con);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )	//点击对话框“继续”部分，游戏继续，下同
				{
					if(isgiven==0)
					{
						DefineBackground();
						DrawBackground();
						DefineRoad();
						DrawRoad();
						DefineWall();
						DrawWall();
						DefineNPC();
						DrawNPC();
						DrawPlayer('w');
						DefineCrystal();
						DrawCrystal();
						setfillcolor(BLACK);
						setlinecolor(RGB(255,20,180));
						setlinestyle(PS_SOLID ,3);
						fillrectangle(player_x*32-100,player_y*32-25,player_x*32+100,player_y*32+25);
						settextcolor(GREEN);
						settextstyle(20, 0, _T("隶书"));
						TCHAR d1[] = _T("获得黄钥匙！");
						outtextxy(player_x*32-70,player_y*32-10,d1);
						FlushBatchDraw();
						delay(500);
						p.YKnum=1;
						isgiven=1;
					}
				    else if(p.YKnum==1&&p.BKnum==1&&p.GKnum==1&&p.RKnum==1)
					{
						p.YKnum=0;
						p.BKnum=0;
						p.GKnum=0;
						p.RKnum=0;
						DrawPanel();
						DrawBackground();
						DefineRoad();
						DrawRoad();
						DefineWall();
						DrawWall();
						DefineNPC();
						DrawNPC();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer('w');
						setfillcolor(BLACK);
						setlinecolor(RGB(255,20,180));
						setlinestyle(PS_SOLID ,3);
						fillrectangle(750,200,1000,250);
						settextcolor(GREEN);
						settextstyle(20, 0, _T("隶书"));
						TCHAR d1[] = _T("获得精灵吊坠！");
						outtextxy(800,215,d1);
						FlushBatchDraw();
						delay(2000);
					    p.Life+=500;
						p.Attack+=100;
						p.Defence+=100;
						DrawPanel();
						dialogue=0;
						isgiven=2;
					}
					DrawBackground();
					DefineRoad();
					DrawRoad();
					DefineWall();
					DrawWall();
					DefineNPC();
					DrawNPC();
					DefineCrystal();
					DrawCrystal();
					DrawPlayer('w');
					DrawPanel();
					dialogue=0;
				}
			}
			else
			{
				iscolor=0;
			}
		}
		else if( dialogue==3 )	//控制与智者对话继续游戏
		{
			if( m.x>=567 && m.y>=281 && m.y<=297 && m.x<=647 )
			{			
				settextcolor(RED);
				settextstyle(16, 0, _T("宋体"));
				TCHAR con[] = _T("-点此继续-");
				outtextxy(567,281,con);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					DrawBackground();
					DefineRoad();
					DrawRoad();
					DefineWall();
					DrawWall();
					DefineNPC();
					DrawNPC();
					DefineCrystal();
					DrawCrystal();
					DrawPlayer('w');
					dialogue=0;
				}
			}
			else 
			{
				iscolor=0;
			}
		}
		else if( dialogue==4)	//控制与看门大爷对话继续游戏
			{
				if( m.x>=527 && m.y>=681 && m.y<=687 && m.x<=607 )
				{			
					settextcolor(RED);
					settextstyle(12, 0, _T("宋体"));
					TCHAR con[] = _T("-点此继续-");
					outtextxy(527,681,con);
					iscolor=1;
					if( m.uMsg==WM_LBUTTONDOWN )
					{
						DrawBackground();
						DefineWall();
						DrawWall();
						DefineNPC();
						DrawNPC();
						DefineCrystal();
						DrawCrystal();
						DrawMonster();
						Drawitems();
						DrawPlayer(lastinput);
						dialogue=0;
					}
				}
				else 
			{
				iscolor=0;
			}
		}

		else if( dialogue==5)	//控制与扫地大妈对话
		{
			if( m.x>=720 && m.y>=255 && m.y<=267 && m.x<=750 )
			{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR con[] = _T("-点此继续-");
				outtextxy(720,255,con);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					DrawBackground();
					DefineWall();
					DrawWall();
					DefineNPC();
					DrawNPC();
					DefineCrystal();
					DrawCrystal();
					DrawMonster();
					Drawitems();
					DrawPlayer(lastinput);
					dialogue=0;
				}
			}
			else if( m.x>=690 && m.y>=165 && m.y<=177 && m.x<=710 )
			{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR d2[] = _T("BUY");
				outtextxy(689,165,d2);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					if(p.GOLD>=10)
					{
						p.Life+=50;
						p.GOLD-=10;
						DrawBackground();
						DefineWall();
						DrawWall();
						Drawitems();
						DefineNPC();
						DrawNPC();
						DrawMonster();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer(lastinput);
						dialogue=0;
					}
				}
			}
			else if( m.x>=690 && m.y>=165 && m.y<=177 && m.x<=710 )
			{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR d2[] = _T("BUY");
				outtextxy(689,165,d2);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					if(p.GOLD>=10)
					{
						p.Life+=50;
						p.GOLD-=10;
						DrawBackground();
						DefineWall();
						DrawWall();
						Drawitems();
						DefineNPC();
						DrawNPC();
						DrawMonster();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer(lastinput);
						dialogue=0;
					}
				}
			}
			}
			else if( m.x>=690 && m.y>=195 && m.y<=207 && m.x<=710 )
			{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR d2[] = _T("BUY");
				outtextxy(689,195,d2);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					if(p.GOLD>=10)
					{
						p.Attack+=10;
						p.GOLD-=10;
						DrawBackground();
						DefineWall();
						DrawWall();
						Drawitems();
						DefineNPC();
						DrawNPC();
						DrawMonster();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer(lastinput);
						dialogue=0;
					}
				}
			}
			else if( m.x>=690 && m.y>=225 && m.y<=237 && m.x<=710 )
			{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR d2[] = _T("BUY");
				outtextxy(689,225,d2);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					if(p.GOLD>=10)
					{
						p.Defence+=10;
						p.GOLD-=10;
						DrawBackground();
						DefineWall();
						DrawWall();
						Drawitems();
						DefineNPC();
						DrawNPC();
						DrawMonster();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer(lastinput);
						dialogue=0;
					}
				}
				else 
				{
					iscolor=0;
				}
			}

			else if(dialogue==6)
			{
				if( m.x>=527 && m.y>=381 && m.y<=393 && m.x<=547)
				{			
				settextcolor(RED);
				settextstyle(12, 0, _T("宋体"));
				TCHAR con[] = _T("-点此继续-");
				outtextxy(527,381,con);
				iscolor=1;
				if( m.uMsg==WM_LBUTTONDOWN )
				{
					
						DrawBackground();
						DefineWall();
						DrawWall();
						Drawitems();
						DefineNPC();
						DrawNPC();
						DrawMonster();
						DefineCrystal();
						DrawCrystal();
						DrawPlayer(lastinput);
						dialogue=0;
				}
				}
				else 
				{
					iscolor=0;
				}
			}

		}
	}

void main()
{

	startup();
	while(1)				//游戏循环执行 
	{
		show();
		updateWithoutInput();
		updateWithInput();	
	}
	EndBatchDraw();
	_getch();
	closegraph();
}
