#include <conio.h>
#include <graphics.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")
float man_x;	 //定义人物的横坐标
float man_y;	 //定义人物的纵坐标
float man_vy_begin;	 //定义人物的垂直起跳初速度
float man_vx;	 //定义人物的水平速度
float man_vy;	 //定义人物的实时垂直速度
float a_vy;	 //定义垂直速度方向的重力加速度
int sky_if;	 //这个变量用于记载人物状态 空中状态为1 非空中状态为0 只有当非空中状态时才可以按‘w’起跳 空中状态人物受重力
float bullet_x[4],bullet_y[4];	 //定义地图自动发射的子弹的坐标
float bullet_vx[4],bullet_vy[4];	//定义地图自动发射的子弹的速度
float bullet_v1,bullet_v0;	 //定义两种速度情况
int shoot_time;	 //这个变量用于实现让子弹固定时间段发射一次
int shoot_if;	 //这个变量用来决定子弹多久发射一次
int live=1;
int footnum=0;
IMAGE man_shadow;
IMAGE img_bk;	 //定义 IMAGE 对象 背景
IMAGE img_man;	 //定义 IMAGE 对象 人物
IMAGE zhezhao;	//定义  IMAGE 对象 人物遮罩
IMAGE right;
IMAGE right_zhezhao;
IMAGE left;
IMAGE left_zhezhao;
IMAGE img_bullet1;	 //定义 IMAGE 对象 地图自动发射子弹
IMAGE img_bullet;	 //定义 IMAGE 对象 子弹遮罩
IMAGE img_start;	//定义 IMAGE 对象  开始界面
IMAGE img_pause;    //定义 IMAGE 对象  暂停界面
int gameStatus = 0; // 游戏状态，0为初始菜单界面，1为正常游戏，2为结束游戏状态，3为游戏暂停
void readRecordFile()  //读取游戏数据文件存档
{
	FILE *fp;
	fp = fopen(".\\gameRecord.dat","r");
	fscanf(fp,"%f %f %f %f",&man_x,&man_y,&bullet_x,&bullet_y);
	fclose(fp);
}
void writeRecordFile()  //存储游戏数据文件存档
{
	FILE *fp;
	fp = fopen(".\\gameRecord.dat","w");
	fprintf(fp,"%f %f %f %f",man_x,man_y,bullet_x,bullet_y);
	fclose(fp);
}
void startMenu() // 初始菜单界面
{	
	putimage(0, 0, &img_start);//显示背景
	char input;
	if(kbhit())  // 判断是否有输入
	{
		input = getch();  // 根据用户的不同输入来移动，不必输入回车
		if (input == '1') 
			gameStatus = 1;
		else if (input == '2')
		{
			readRecordFile();
			gameStatus = 1;
		}
		else if (input == '3')
		{
			gameStatus = 2;
			exit(0);
		}
	}
}
void pauseMenu() // 游戏暂停后菜单界面，一般按ESC键后启动该界面
{
	putimage(0, 0, &img_pause);	// 显示背景	
	char input;
	if(kbhit())  // 判断是否有输入
	{
		input = getch();  // 根据用户的不同输入来移动，不必输入回车
		if (input == '1') 
			gameStatus = 1;
		else if (input == '2')
		{
			writeRecordFile();
			gameStatus = 1;
		}
		else if (input == '3')
		{
			gameStatus = 2;
			exit(0);
		}
	}
}
void begining()	 //游戏初始化模块
{
	mciSendString("open .\\bgm.mp3 alias bkmusic", NULL, 0, NULL);//打开背景音乐
	mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放
	man_x=100;	
	man_y=633;
	man_vx=2;
	man_vy_begin=-6.5;
	man_vy=0;
	shoot_time=0;
	shoot_if=180;
	bullet_v1=5;
	bullet_v0=0;
	int i;
	for(i=0;i<4;i++)
	{
		bullet_x[i]=bullet_y[i]=50;
		bullet_vx[i]=bullet_v1;
		bullet_vy[i]=bullet_v0;
	}
	a_vy=0.24;
	sky_if=0;

	loadimage(&img_bk, ".\\蓝色药水第八关.jpg",696,696);						// 读取背景图片到 img_bk 对象中
	loadimage(&img_man,".\\我的药水.jpg",37,37);			//读取人物图片到img_man 对象中
	loadimage(&zhezhao,".\\遮罩层.jpg",37,37);			//读取人物图片到img_man 对象中
	loadimage(&img_bullet1,".\\子弹.jpg",30,30);					//读取子弹图片到 img_bullet 对象中
	loadimage(&img_bullet,".\\子弹遮罩层.jpg",30,30);			
	loadimage(&img_start,".\\start.jpg",696,696);   //读取菜单背景到img_start 对象中
	loadimage(&img_pause,".\\pause.jpg",696,696);
	loadimage(&right_zhezhao,".\\我的2遮罩层.jpg",37,37);
	loadimage(&right,".\\我的2.jpg",37,37);
	loadimage(&left_zhezhao,".\\我的3遮罩层.jpg",37,37);
	loadimage(&left,".\\我的3.jpg",37,37);
	
	while (gameStatus==0)
		startMenu();//初始菜单界面
}
int map_y_down(float x1,float y1)						//定义地图函数 地图函数判断人物是否踩到地面 踩到返回非0 否则返回0
{
	int judge;
	if(y1>=635)
		judge=635;
	else if(y1>=575&&y1<=582&&x1>=393&&x1<=533)//1
		judge=575;
	else if(y1>535&&y1<=542&&x1>=538&&x1<=629)//2
		judge=535;
	else if(y1>=474&&y1<=481&&x1>=413&x1<=508)//3
		judge=474;	
	else if(y1>=474&&y1<=481&&x1>250&&x1<346)//4
		judge=474;
	else if(y1>=474&&y1<=481&&x1>88&&x1<=185)//5
		judge=474;
	else if(y1>=535&&y1<=542&&x1<=185)//6
		judge=535;
	else if(y1>=413&&y1<=420&&x1<60)//7
		judge=413;
	else if(y1>=352&&y1<=359&&x1>86&&x1<=185)//8
		judge=352;
	else if(y1>=290&&y1<=297&&x1<62)//9
		judge=290;
	else if(y1>=230&&y1<=237&&x1>86&&x1<=185)//10
		judge=230;
	else if(y1>=169&&y1<=176&&x1<62)//11
		judge=169;
	else if(y1>=330&&y1<=338&&x1>=213&&x1<=260)//穿屏豆1
		judge=330;
	else if(y1>=252&&y1<=259&&x1>=311&&x1<=547)//12
		judge=252;
	else if(y1>=252&&y1<=259&&x1>=550&&x1<=635)//13
		judge=252;
	else if(y1>=330&&y1<=337&&x1>=620)//14
		judge=330;
	else if(y1>=370&&y1<=377&&x1>=524&&x1<=629)//15
		judge=370;
	else if(y1>=330&&y1<=338&&x1>=505&&x1<=547)//穿屏豆2
		judge=330;
	else if(y1>=330&&y1<=338&&x1>=470&&x1<=490)//穿屏豆3
		judge=330;
	else if(y1>=88&&y1<=95&&x1<=635)//16
		judge=88;
	else if(y1>=555&&y1<=597&&x1>=642)//2
		judge=555;
	else 
		judge=0;
	return judge;

}
int map_y_up(float x2,float y2)						//定义地图函数 判断人物的头顶是否有墙 有则返回非0 否则返回0
{
	int judge;
	if(y2<=23)
		judge=23;
	else if(y2<=631&&y2>=624&&x2>=393&&x2<=527)//1
		judge=631;
	else if(y2<=590&&y2>=583&&x2>=538&&x2<=629)//2
		judge=590;
	else if(y2<=530&&y2>=523&&x2>=413&x2<=508)//3
		judge=530;	
	else if(y2<=530&&y2>=523&&x2>=253&&x2<=343)//4
		judge=530;
	else if(y2<=530&&y2>=523&&x2>=88&&x2<=182)//5
		judge=530;
	else if(y2<=592&&y2>=584&&x2<=185)//6
		judge=592;
	else if(y2<=468&&y2>=457&&x2<60)//7
		judge=468;
	else if(y2<=346&&y2>=339&&x2<60)//7
		judge=346;
	else if(y2<=226&&y2>=219&&x2<60)//7
		judge=226;
	else if(y2>=136&&y2<=143&&x2<=635)//16
		judge=143;
	else if(y2>=401&&y2<=408&&x2>90&&x2<=185)//8
		judge=408;
	else if(y2>=503&&y2<=510&&x2<=16)//8
		judge=510;
	else if(y2<=284&&y2>=277&&x2<=126&&x2>86)//2
		judge=284;
	else if(y2>=521&&y2<=528&&x2>=642)//2
		judge=528;
	else if(y2>=299&&y2<=306&&x2>=360&&x2<=635)//zhongjian
		judge=306;
	else 
		judge=0;
	return judge;
}
int map_x_left(float x3,float y3)						//定义地图函数 判断人物的左边是否有墙 有则返回1 否则返回0
{
	int judge;
	if(x3==18&&y3>=555)
		judge=1;
	else if(x3==18&&y3<=510)
		judge=1;
	else if(x3==534&&y3>=576)
		judge=1;
	else if(y3>=535&&y3<=590&&x3==630)//2
		judge=1;
	else if(y3>=475&&y3<=590&&x3==186)//2
		judge=1;
	else if(y3>=475&&y3<=590&&x3==186)//2
		judge=1;
	else if(y3>=225&&y3<=408&&x3==186)//2
		judge=1;
	else if(y3<=470&&y3>=405&&x3==60)//7
		judge=468;
	else if(y3>=475&&y3<=528&&x3==510)//3
		judge=1;
	else if(y3>=290&&y3<=340&&x3==62)//9
		judge=1;
	else if(y3>=169&&y3<=220&&x3==62)//11
		judge=1;
	else if(y3>=474&&y3<=530&&x3==346)//4
		judge=1;
	else if(y3>=252&&y3<=300&&x3==636)//13
		judge=1;
	else if(y3>=88&&y3<=136&&x3==636)//16
		judge=1;
	else
		judge=0;
	return judge;
}
int map_x_right(float x4,float y4)					//定义地图函数 判断人物的右边是否有墙 有则返回1 否则返回0
{
	int judge;
	if(x4==640&&y4>=556)
		judge=1;
	else if(x4==640&&y4<=530)
		judge=1;
	else if(x4==474&&y4>=576)
		judge=1;
	else if(y4>=575&&y4<=631&&x4==392)//1
		judge=1;
	else if(y4>=535&&y4<=590&&x4==536)//2
		judge=1;
	else if(y4>=475&&y4<=590&&x4==126)//2
		judge=1;
	else if(y4>=235&&y4<=408&&x4==126)//2
		judge=1;
	else if(y4>=480&&y4<=531&&x4==88)//2
		judge=1;
	else if(y4>=475&&y4<=528&&x4==412)//3
		judge=1;
	else if(y4>=352&&y4<=400&&x4==86)//8
		judge=1;
	else if(y4>=230&&y4<=290&&x4==86)//10
		judge=1;
	else if(y4>=474&&y4<=530&&x4==250)//4
		judge=1;
	else
		judge=0;
	return judge;
}
void show()	 //游戏显示模块
{
	while(gameStatus==3)
		pauseMenu();//游戏暂停界面

	putimage(0, 0, &img_bk);	 // 在坐标 (0, 0) 位置显示 IMAGE 对象
	if(GetAsyncKeyState('D')&0x8000)
	{
		putimage(man_x,man_y,&right_zhezhao,NOTSRCERASE);
		putimage(man_x, man_y,&right,SRCINVERT);	
	}
	else if(GetAsyncKeyState('A')&0x8000)
	{
		putimage(man_x,man_y,&left_zhezhao,NOTSRCERASE);
		putimage(man_x, man_y,&left,SRCINVERT);	
	}
	else
	{	
		putimage(man_x,man_y,&zhezhao,NOTSRCERASE);
		putimage(man_x, man_y, &img_man,SRCINVERT);	 //显示出人物
	}
	putimage(bullet_x[0],bullet_y[0],&img_bullet,NOTSRCERASE);//显示子弹
	putimage(bullet_x[0],bullet_y[0],&img_bullet1,SRCINVERT);//显示子弹
	
	if(shoot_time>=shoot_if)
	{
		putimage(bullet_x[1],bullet_y[1],&img_bullet,NOTSRCERASE);//显示子弹
		putimage(bullet_x[1],bullet_y[1],&img_bullet1,SRCINVERT);//显示子弹
	
	}
	if(shoot_time>=2*shoot_if)
	{
		putimage(bullet_x[2],bullet_y[2],&img_bullet,NOTSRCERASE);//显示子弹
		putimage(bullet_x[2],bullet_y[2],&img_bullet1,SRCINVERT);//显示子弹
	}
	if(shoot_time>=3*shoot_if)
	{
		putimage(bullet_x[3],bullet_y[3],&img_bullet,NOTSRCERASE);//显示子弹
		putimage(bullet_x[3],bullet_y[3],&img_bullet1,SRCINVERT);//显示子弹
	}
}
void without_input()	 //与用户输入无关的更新
{
	//穿屏函数
	if(man_x>=260&&man_x<=330&&man_y>=330&&man_y<=338)
		man_y=man_y-185;
	if(man_x>=460&&man_x<=505&&man_y>=330&&man_y<=337)
		man_y=man_y-310;
	if(man_x>=270&&man_x<=305&&man_y>=88&&man_y<=95)
		man_y=man_y+340;
	if(man_x==18&&man_y>=510&&man_y<=555)
		man_x=man_x+620;
	if(man_x==640&&man_y>=510&&man_y<=555)
		man_x=man_x-620;man_y=man_y+man_vy;	 //人物的垂直速度man_y初始化是0，所以虽然有这段表达式，但人物并不会起跳
	bullet_x[0]=bullet_x[0]+bullet_vx[0];
	bullet_y[0]=bullet_y[0]+bullet_vy[0];
	if(shoot_time>=shoot_if)
	{
		bullet_x[1]=bullet_x[1]+bullet_vx[1];
		bullet_y[1]=bullet_y[1]+bullet_vy[1];
	}
	if(shoot_time>=2*shoot_if)
	{
		bullet_x[2]=bullet_x[2]+bullet_vx[2];
		bullet_y[2]=bullet_y[2]+bullet_vy[2];
	}
	if(shoot_time>=3*shoot_if)
	{
		bullet_x[3]=bullet_x[3]+bullet_vx[3];
		bullet_y[3]=bullet_y[3]+bullet_vy[3];
	}
	shoot_time++;
	int i=0;
	for(i=0;i<4;i++)
	{
		if(bullet_x[i]==630&&bullet_y[i]==50)
		{
			bullet_vy[i]=bullet_v1;
			bullet_vx[i]=bullet_v0;
		}
		if(bullet_x[i]==630&&bullet_y[i]==625)
		{
			bullet_vy[i]=bullet_v0;
			bullet_vx[i]=-bullet_v1;
		}
		if(bullet_x[i]==545&&bullet_y[i]==625)
		{
			bullet_vy[i]=-bullet_v1;
			bullet_vx[i]=bullet_v0;
		}
		if(bullet_x[i]==545&&bullet_y[i]==180)
		{
			bullet_vy[i]=bullet_v0;
			bullet_vx[i]=-bullet_v1;
		}
		if(bullet_x[i]==80&&bullet_y[i]==180)
		{
			bullet_vy[i]=bullet_v1;
			bullet_vx[i]=bullet_v0;
		}
		if(bullet_x[i]==80&&bullet_y[i]==525)
		{
			bullet_vy[i]=bullet_v0;
			bullet_vx[i]=-bullet_v1;
		}	
		if(bullet_x[i]==0&&bullet_y[i]==525)
		{
			bullet_x[i]=695;
		}
		if(bullet_x[i]==385&&bullet_y[i]==525)
		{
			bullet_vy[i]=-bullet_v1;
			bullet_vx[i]=bullet_v0;
		}
		if(bullet_x[i]==385&&bullet_y[i]==325)
		{
			bullet_vy[i]=bullet_v0;
			bullet_vx[i]=-bullet_v1;
		}
		if(bullet_x[i]==210&&bullet_y[i]==325)
		{
			bullet_vy[i]=bullet_v1;
			bullet_vx[i]=bullet_v0;
		}
		if(bullet_y[i]==650)
		{
			bullet_vx[i]=bullet_v1;
			bullet_vy[i]=bullet_v0;
			bullet_x[i]=50;
			bullet_y[i]=50;
		}
	}
	for(i=0;i<4;i++)
	{
		if( ( (bullet_x[i]+15)- (man_x+18) )*( (bullet_x[i]+15)- (man_x+18) )+ ((bullet_y[i]+15)- (man_y+18) )* ((bullet_y[i]+15)- (man_y+18))<=33*33)    
		live=0;
	}
	if(!map_y_down(man_x,man_y))	 //调用地图函数，判断人物是否处于空中状态
	{
		sky_if=1;	 //如果人物处于空中状态，则sky_if变为1；
	}
	if(sky_if==1)	 //当人物空中状态为1的时候 启动在空中应该有的自动更新 
	{
		if(man_vy<=-man_vy_begin)	 //限制最大速度为起跳初速度
		{
			man_vy=man_vy+a_vy;	 //在空中的时候速度因重力加速度而改变;	
		}
		if(map_y_up(man_x,man_y))	 //调用地图函数 地图函数判断人物头顶是否有墙 有则返回1 否则返回0
		{
			man_vy=0;	 //人物头顶撞到墙 垂直速度变为0	
			man_y=map_y_up(man_x,man_y)+1;
		}
		if(map_y_down(man_x,man_y))	 //调用地图函数 地图函数判断人物是否踩到地面 踩到返回非0 否则返回0
		{
			man_vy=0;
			sky_if=0;	 //人物踩到地面 变为非空中状态 0
			man_y=map_y_down(man_x,man_y);
		}
	}
	if(man_x<=100&&man_y<=88)
		live=2;
}
void user_input()	 //与用户输入有关的更新
{
	if(live)
	{
		if(GetAsyncKeyState('A')&0x8000)  
	{
		if(!map_x_left(man_x,man_y))	 //调用地图函数 判断人物的左边是否有墙 有则返回1 否则返回0（前面有一个‘!’符号 没有墙返回0 ‘!0’则为真） 
		{
			man_x-=2;  // 位置左移
		}
	}
	if(GetAsyncKeyState('D')&0x8000)
	{
		if(!map_x_right(man_x,man_y))	 //调用地图函数 判断人物右边是否有墙 有则返回1 否则返回0 (前面加了一个‘!’ 没有墙返回0 ‘!0’为真)
		{
			man_x+=2;  // 位置右移
		}	
	}
	if(sky_if==0)	 //当人物踩到地面为非空中状态时 ‘w’键才能有效输入
	{
		if(GetAsyncKeyState('W')&0x8000)
		{
			man_vy=man_vy_begin;	 //‘w’键有效输入之后 人物立即拥有一个竖直方向上的初速度
		}
	}	
	}
	char input;
	if(kbhit())  // 判断是否有输入
	{
		input = getch();  // 根据用户的不同输入来移动，不必输入回车
		if (input == ' ') // ESC键的ACSII码为27
		{
			gameStatus = 3;
		}
	}
}
void main()	 //主函数
{
	initgraph(696,696);
	begining();
	BeginBatchDraw();
	while(1)
	{
		mciSendString("open .\\近距离作战.mp3 alias bkmusic", NULL, 0, NULL);	 //背景音乐
		mciSendString("play bkmusic repeat", NULL, 0, NULL);	 // 循环播放
		show();
		FlushBatchDraw();
		without_input();
		Sleep(15);
		user_input();
		if(live==0||live==2)
			break;
	}
	EndBatchDraw();
	if(live==0)
	{
		IMAGE lose;
		loadimage(&lose, ".\\lose.jpg",696,696);	
		while(1)
			putimage(0, 0, &lose);
	}
	else if(live==2)
	{
		IMAGE win;
		loadimage(&win, ".\\win.jpg",696,696);	
		while(1)
			putimage(0, 0, &win);
	}

	getch();
	closegraph();
}