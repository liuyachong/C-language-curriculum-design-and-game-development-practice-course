#include <graphics.h>
#include <conio.h>
#define width 400
#define high 650//定义画布大小

//定义全局变量

int diamond_x1, diamond_y1;////方块所在位置中心
int diamond_x0, diamond_y0;//灰色底布方块中心
int diamonds[width][high] = { 0 };///定义在整个画布的二维数组
int center_x1, center_y1;//出现方块的中心

int step_r, step, step1;//方块边长一半及边长

int i, j;//循环变量
int t;//消除判断变量

int isselect;//选择判断变量

int score;
char scoreString[10];//分数显示
TCHAR scoreText[10];   //用vs打开时取消这个注释

MOUSEMSG msg;//鼠标消息
IMAGE img_bk;	//定义游戏背景
IMAGE img_bg;	// 定义初始界面背景

void startup()
{
	center_x1 = 200;//出现随机方块的中心
	center_y1 = 500;

	diamond_x1 = center_x1;//随机方块的中心
	diamond_y1 = center_y1;

	diamond_x0 = 40;
	diamond_y0 = 90;

	step = 30;
	step_r = 15;

	isselect = 0;

	step1 = 32;

	t = 0;
	score = 0;

	diamonds[diamond_x1][diamond_y1] = rand() % 1 + 2;//先从一个方块开始写

	loadimage(&img_bk, _T(".\\background.jpg"));	// 读取图片到 img 对象中

	for (i = 0; i < 10; i++)//初始化灰色方块底布
	{
		for (j = 0; j < 10; j++)
		{
			diamonds[diamond_x0 + i*step1 + step_r][diamond_y0 + j*step1 + step_r] = 1;
		}
	}
}

void press()	//初始窗口
{
	initgraph(width, high);		// 初始化绘图窗口

	loadimage(&img_bg, _T(".\\begin.jpg"));	// 读取初始界面到 img 对象中
	putimage(0, 0, &img_bg);	// 在坐标 (0, 0) 位置显示初始界面
}

void showbk()	//显示背景模板
{
	setbkcolor(RGB(242, 255, 248));	// 设置背景色

	putimage(0, 0, &img_bk);	// 在坐标 (0, 0) 位置显示 IMAGE 对象

	for (i = 0; i < 400; i++)//绘制底布
	{
		for (j = 50; j < 500; j++)
		{
			if (diamonds[i][j] == 1)
			{
				setfillcolor(RGB(224, 224, 224));
				solidroundrect(i - step_r, j - step_r, i + step_r, j + step_r, 5, 5);
			}
		}
	}

}
void showdiamond()
{
	for (i = 0; i < 400; i++)//在整个画面内循环
	{
		for (j = 0; j < 650; j++)
		{
			if (diamonds[i][j] == 2)//当数组为2时显示该类型方块
			{
				setfillcolor(RGB(124, 136, 194));
				solidroundrect(i - step_r, j - step_r, i + step_r, j + step_r, 5, 5);
			}
		}
	}
	/*else if (diamonds[diamond_x1][diamond_y1] = 2)//横向两个方块
	{
	setfillcolor(RGB(255, 198, 62));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step_r, diamond_x1 - 1, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step_r, diamond_x1 + step + 1, diamond_y1 + step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 3)//竖向两个方块
	{
	setfillcolor(RGB(255, 198, 62));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step - 1, diamond_x1 + step_r, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + 1, diamond_x1 + step_r, diamond_y1 + step + 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 4)//横向三个方块
	{
	setfillcolor(RGB(237, 149, 75));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r, diamond_x1 + step_r, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r, diamond_x1 - step_r - 2, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r, diamond_x1 + step_r + step + 2, diamond_y1 + step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 5)//竖向三个方块
	{
	setfillcolor(RGB(237, 149, 75));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r, diamond_x1 + step_r, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r - step - 2, diamond_x1 + step_r, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + 2, diamond_x1 + step_r, diamond_y1 + step_r + step + 2, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 6)//四块正方形
	{
	setfillcolor(RGB(152, 220, 85));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step - 1, diamond_x1 - 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 - step - 1, diamond_y1 + 1, diamond_x1 - 1, diamond_y1 + step + 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step - 1, diamond_x1 + step + 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 + 1, diamond_x1 + step + 1, diamond_y1 + step + 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 7)//234三块
	{
	setfillcolor(RGB(89, 203, 134));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 + 1, diamond_x1 - 1, diamond_y1 + step + 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step - 1, diamond_x1 + step + 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 + 1, diamond_x1 + step + 1, diamond_y1 + step + 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 8)//124三块
	{
	setfillcolor(RGB(89, 203, 134));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step - 1, diamond_x1 - 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step - 1, diamond_x1 + step + 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 + 1, diamond_x1 + step + 1, diamond_y1 + step + 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 9)//134三块
	{
	setfillcolor(RGB(89, 203, 134));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step - 1, diamond_x1 - 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 - step - 1, diamond_y1 + 1, diamond_x1 - 1, diamond_y1 + step + 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 + 1, diamond_x1 + step + 1, diamond_y1 + step + 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 10)//123三块
	{
	setfillcolor(RGB(89, 203, 134));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step - 1, diamond_x1 - 1, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 - step - 1, diamond_y1 + 1, diamond_x1 - 1, diamond_y1 + step + 1, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step - 1, diamond_x1 + step + 1, diamond_y1 - 1, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 11)//横向四个方块
	{
	setfillcolor(RGB(255, 198, 62));
	solidroundrect(diamond_x1 - step - 1, diamond_y1 - step_r, diamond_x1 - 1, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + 1, diamond_y1 - step_r, diamond_x1 + step + 1, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - 2 * step - 3, diamond_y1 - step_r, diamond_x1 - step - 3, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + step + 3, diamond_y1 - step_r, diamond_x1 + 2 * step + 3, diamond_y1 + step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 12)//竖向四个方块
	{
	setfillcolor(RGB(255, 198, 62));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step - 1, diamond_x1 + step_r, diamond_y1 - 1, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + 1, diamond_x1 + step_r, diamond_y1 + step + 1, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 - 2 * step - 3, diamond_x1 + step_r, diamond_y1 - step - 3, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + step + 3, diamond_x1 + step_r, diamond_y1 + 2 * step + 3, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 13)//横向五个方块
	{
	setfillcolor(RGB(221, 101, 85));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r, diamond_x1 + step_r, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r, diamond_x1 - step_r - 2, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r, diamond_x1 + step_r + step + 2, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - 2 * step - step_r - 4, diamond_y1 - step_r, diamond_x1 - step - step_r - 4, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 + step + step_r + 4, diamond_y1 - step_r, diamond_x1 + 2 * step + step_r + 4, diamond_y1 + step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 14)//竖向五个方块
	{
	setfillcolor(RGB(221, 101, 85));
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r, diamond_x1 + step_r, diamond_y1 + step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r - step - 2, diamond_x1 + step_r, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + 2, diamond_x1 + step_r, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 - 2 * step - step_r - 4, diamond_x1 + step_r, diamond_y1 - step - step_r - 4, 5, 5);
	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + step + 4, diamond_x1 + step_r, diamond_y1 + 2 * step + step_r + 4, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 15)//九宫格方块
	{
	setfillcolor(RGB(77, 213, 175));

	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r, diamond_x1 + step_r, diamond_y1 + step_r, 5, 5);//中中
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r, diamond_x1 - step_r - 2, diamond_y1 + step_r, 5, 5);//中左
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r, diamond_x1 + step_r + step + 2, diamond_y1 + step_r, 5, 5);//中右

	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + 2, diamond_x1 + step_r, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 + step_r + 2, diamond_x1 - step_r - 2, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 + step_r + 2, diamond_x1 + step_r + step + 2, diamond_y1 + step_r + step + 2, 5, 5);

	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r - step - 2, diamond_x1 + step_r, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r - step - 2, diamond_x1 - step_r - 2, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r - step - 2, diamond_x1 + step_r + step + 2, diamond_y1 - 2 - step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 16)//12369五个方块
	{
	setfillcolor(RGB(92, 190, 229));

	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r, diamond_x1 + step_r + step + 2, diamond_y1 + step_r, 5, 5);

	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 + step_r + 2, diamond_x1 + step_r + step + 2, diamond_y1 + step_r + step + 2, 5, 5);

	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r - step - 2, diamond_x1 + step_r, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r - step - 2, diamond_x1 - step_r - 2, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r - step - 2, diamond_x1 + step_r + step + 2, diamond_y1 - 2 - step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 17)//36789五个方块
	{
	setfillcolor(RGB(92, 190, 229));

	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r, diamond_x1 + step_r + step + 2, diamond_y1 + step_r, 5, 5);

	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + 2, diamond_x1 + step_r, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 + step_r + 2, diamond_x1 - step_r - 2, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 + step_r + 2, diamond_x1 + step_r + step + 2, diamond_y1 + step_r + step + 2, 5, 5);

	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r - step - 2, diamond_x1 + step_r + step + 2, diamond_y1 - 2 - step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 18)//12347五个方块
	{
	setfillcolor(RGB(92, 190, 229));

	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r, diamond_x1 - step_r - 2, diamond_y1 + step_r, 5, 5);

	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 + step_r + 2, diamond_x1 - step_r - 2, diamond_y1 + step_r + step + 2, 5, 5);

	solidroundrect(diamond_x1 - step_r, diamond_y1 - step_r - step - 2, diamond_x1 + step_r, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r - step - 2, diamond_x1 - step_r - 2, diamond_y1 - 2 - step_r, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 - step_r - step - 2, diamond_x1 + step_r + step + 2, diamond_y1 - 2 - step_r, 5, 5);
	}
	else if (diamonds[diamond_x1][diamond_y1] = 19)//14789五个方块
	{
	setfillcolor(RGB(92, 190, 229));

	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r, diamond_x1 - step_r - 2, diamond_y1 + step_r, 5, 5);

	solidroundrect(diamond_x1 - step_r, diamond_y1 + step_r + 2, diamond_x1 + step_r, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 + step_r + 2, diamond_x1 - step_r - 2, diamond_y1 + step_r + step + 2, 5, 5);
	solidroundrect(diamond_x1 + step_r + 2, diamond_y1 + step_r + 2, diamond_x1 + step_r + step + 2, diamond_y1 + step_r + step + 2, 5, 5);

	solidroundrect(diamond_x1 - step_r - step - 2, diamond_y1 - step_r - step - 2, diamond_x1 - step_r - 2, diamond_y1 - 2 - step_r, 5, 5);
	}*/



	//vc分数显示
	/*settextcolor(RGB(0, 0, 0));
	settextstyle(30, 15, _T("Arial"));
	itoa(score, scoreString, 10);
	outtextxy(130, 40, _T(scoreString));*/


	//vs分数显示
	settextcolor(RGB(0, 0, 0));
	settextstyle(30, 15, _T("Arial"));
	_itoa_s(score, scoreString, 10);
	MultiByteToWideChar(CP_ACP, 0, scoreString, -1, scoreText, 10);

	outtextxy(130, 40, scoreText);
}

void updateWithInput()	//与用户输入有关的更新
{
	while (1)
	{
		msg = GetMouseMsg();
		if ((msg.y < diamond_y1 + step1 + step_r) && (msg.y > diamond_y1 - step1 - step_r) && (msg.x < diamond_x1 + step1 + step_r) && (msg.x > diamond_x1 - step1 - step_r))
		{
			if (msg.mkLButton)
			{
				isselect = 1;//被选择
			}
			if (isselect == 1)
			{
				if (diamonds[diamond_x1][diamond_y1] == 2)//如果选择的是2方块
				{
					if (msg.mkLButton)
					{
						diamonds[diamond_x1][diamond_y1] = 0;//原位置数组为0
						diamond_x1 = msg.x;
						diamond_y1 = msg.y;//方块位置和鼠标位置相同
						diamonds[diamond_x1][diamond_y1] = 2;//在新位置显示方块
					}
					else
					{
						for (i = 0; i < 10; i++)
							for (j = 0; j < 10; j++)
							{
								//判断方块是否在以10*10底板的某个方块中心为原点方块周长一半为半径的圆内
								if ((diamond_x1 - (diamond_x0 + i*step1 + step_r))*(diamond_x1 - (diamond_x0 + i*step1 + step_r)) + (diamond_y1 - (diamond_y0 + j*step1 + step_r))*(diamond_y1 - (diamond_y0 + j*step1 + step_r)) <= 15 * 15)
								{
									//判断该圆心数组是否为1
									if (diamonds[diamond_x0 + i*step1 + step_r][diamond_y0 + j*step1 + step_r] == 1)
									{
										diamonds[diamond_x0 + i*step1 + step_r][diamond_y0 + j*step1 + step_r] = 2;

										diamonds[diamond_x1][diamond_y1] = 0;//原位置为0
										diamond_x1 = center_x1;
										diamond_y1 = center_y1;//使方块中心回到出现方块的中心
										diamonds[diamond_x1][diamond_y1] = rand() % 1 + 2;//重新随机一个方块
										score = score + 1;//一个方块放上去分数加一
										isselect = 0;
									}
								}
							}
					}
					break;
				}
			}
		}
	}
}

void updateWithoutInput()	//与用户输入无关的更新
{
	//从竖排判断消除
	for (i = 0; i < 10; i++)
	{
		t = 0;
		for (j = 0; j < 10; j++)
		{

			if (getpixel(diamond_x0 + i*step1 + 10, diamond_y0 + j*step1 + 10) != RGB(224, 224, 224))
				t++;
		}
		if (t == 10)
		{
			for (j = 0; j < 10; j++)
			{
				setfillcolor(RGB(224, 224, 224));
				solidroundrect(diamond_x0 + i*step1, diamond_y0 + j*step1, diamond_x0 + i*step1 + step, diamond_y0 + j*step1 + step, 5, 5);
				diamonds[diamond_x0 + i*step1 + step_r][diamond_y0 + j*step1 + step_r] = 1;
			}
			score += 10;
		}
		t = 0;
	}
	//从横排判断消除
	for (j = 0; j < 10; j++)
	{
		t = 0;
		for (i = 0; i < 10; i++)
		{
			if (getpixel(diamond_x0 + i*step1 + 10, diamond_y0 + j*step1 + 10) != RGB(224, 224, 224))
				t++;
		}
		if (t == 10)
		{
			for (i = 0; i < 10; i++)
			{
				setfillcolor(RGB(224, 224, 224));
				solidroundrect(diamond_x0 + i*step1, diamond_y0 + j*step1, diamond_x0 + i*step1 + step, diamond_y0 + j*step1 + step, 5, 5);
				diamonds[diamond_x0 + i*step1 + step_r][diamond_y0 + j*step1 + step_r] = 1;
			}
			score += 10;
		}
		t = 0;
	}
}

void main()
{
	startup();
	press();	//初始界面	

	while (1)
	{
		msg = GetMouseMsg();//获取鼠标信息
		if (msg.mkLButton&&msg.y < 317 && msg.y >233 && msg.x < 296 && msg.x > 99)
		{
			break;
		}
	}

	BeginBatchDraw();//开始批量绘制

	while (1)
	{
		showbk();
		showdiamond();
		FlushBatchDraw();
		updateWithInput();
		updateWithoutInput();
	}
	EndBatchDraw();//结束批量绘制
	_getch();
	closegraph();
}
