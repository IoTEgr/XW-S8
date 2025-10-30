#include "application.h"
#include "taskgame.h"

#define U 1
#define D 2
#define L 3 
#define R 4



// 7种方块的4旋转状态（4位为一行）
static INT16U gs_uTetrisTable[7][4] = { 
		{ 0x00F0U, 0x2222U, 0x00F0U, 0x2222U },  // I型
		{ 0x0072U, 0x0262U, 0x0270U, 0x0232U },  // T型
		{ 0x0223U, 0x0074U, 0x0622U, 0x0170U },  // L型
		{ 0x0226U, 0x0470U, 0x0322U, 0x0071U },  // J型
		{ 0x0063U, 0x0264U, 0x0063U, 0x0264U },  // Z型
		{ 0x006CU, 0x0462U, 0x006CU, 0x0462U },  // S型
		{ 0x0660U, 0x0660U, 0x0660U, 0x0660U }   // O型
};
//static INT16U gs_uTetrisTable_default[4] = {0};
//static INT16U save_flag = 0;
// =============================================================================
// 初始状态的游戏池
// 每个元素表示游戏池的一行，下标大的是游戏池底部
// 两端各置2个1，底部2全置为1，便于进行碰撞检测
// 这样一来游戏池的宽度为12列
// 如果想要传统的10列，只需多填两个1即可（0xE007），当然显示相关部分也要随之改动
// 当某个元素为0xFFFFU时，说明该行已被填满
// 顶部4行用于给方块，不显示出来
// 再除去底部2行，显示出来的游戏池高度为22行
static const INT16U gs_uInitialTetrisPool[22] = {0xC003,0xC003, 0xC003,
		0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003,
		0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003, 0xC003,
		0xC003, 0xFFFF, 0xFFFF };
		
#define COL_BEGIN 2
#define COL_END 14
#define ROW_BEGIN 4
#define ROW_END 20

#define RECT_W 14
#define RECT_H 14

// =============================================================================
typedef struct TetrisManager  // 这个结构体存储游戏相关数据
{
	uint16_t pool[22];  // 游戏池
	int8_t x;  // 当前方块x坐标，此处坐标为方块左上角坐标
	int8_t y;  // 当前方块y坐标
	int8_t type[3];  // 当前、下一个和下下一个方块类型
	int8_t orientation[3];  // 当前、下一个和下下一个方块旋转状态
	unsigned score;  // 得分
	unsigned erasedCount[4];  // 消行数
	unsigned erasedTotal;  // 消行总数
	unsigned tetrisCount[7];  // 各方块数
	unsigned tetrisTotal;  // 方块总数
	bool dead;  // 挂
} TetrisManager;

// =============================================================================
typedef struct TetrisControl  // 这个结构体存储控制相关数据
{
	bool pause;  // 暂停
	bool clockwise;  // 旋转方向：顺时针为true
	int8_t direction;  // 移动方向：0向左移动 1向右移动
	// 游戏池内每格的颜色
	// 由于此版本是彩色的，仅用游戏池数据无法存储颜色信息
	// 当然，如果只实现单色版的，就没必要用这个数组了
	uint16_t color[22][16];
} TetrisControl;


static TetrisManager tetrisManager;
static TetrisControl tetrisControl;

static u8 *tetris_icon = 0;


// =============================================================================
// 插入方块
void insertTetris(TetrisManager *manager) {
	// 当前方块
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	// 当前方块每4位取出，位或到游戏池相应位置，即完成插入方块
	manager->pool[manager->y + 0] |= (((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] |= (((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] |= (((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] |= (((tetris >> 0xC) & 0x000F) << manager->x);
}
bool checkCollision_L(uint16_t tetris)
{
	uint16_t dest = 0;
	
	dest |= ((tetris >> 0x0) & 0x0008);
	dest |= ((tetris >> 0x4) & 0x0008);
	dest |= ((tetris >> 0x8) & 0x0008);
	dest |= ((tetris >> 0xC) & 0x0008);
	
	return dest;
}
bool checkCollision_R(uint16_t tetris)
{
	uint16_t dest = 0;
	
	dest |= ((tetris >> 0x0) & 0x0001);
	dest |= ((tetris >> 0x4) & 0x0001);
	dest |= ((tetris >> 0x8) & 0x0001);
	dest |= ((tetris >> 0xC) & 0x0001);
	
	return dest;
}

// =============================================================================
// 碰撞检测
bool checkCollision(const TetrisManager *manager) {
	// 当前方块
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];
	uint16_t dest = 0;
	
	
	// 获取当前方块在游戏池中的区域：
	// 游戏池坐标x y处小方格信息，按低到高存放在16位无符号数中
	dest |= (((manager->pool[manager->y + 0] >> manager->x) << 0x0) & 0x000F);
	dest |= (((manager->pool[manager->y + 1] >> manager->x) << 0x4) & 0x00F0);
	dest |= (((manager->pool[manager->y + 2] >> manager->x) << 0x8) & 0x0F00);
	dest |= (((manager->pool[manager->y + 3] >> manager->x) << 0xC) & 0xF000);

	// 若当前方块与目标区域存在重叠（碰撞），则位与的结果不为0
	return ((dest & tetris) != 0);
}
void show_one_diamonds(int8_t x,int8_t y,const uint16_t color)
{
	u8 * p;
	DISPLAY_ICONSHOW icon;
	
	p = (u8 *)setting_frame_buff;
	
	icon.icon_w = RECT_W;
	icon.icon_h = RECT_H;
	icon.transparent = TRANSPARENT_COLOR;
	icon.pos_x = x*RECT_W + 8;
	icon.pos_y = y*RECT_H + 8;
	
	frame_icon_draw(p, tetris_icon, &icon, /*NULL*/0);
}


extern INT16U score[3];
// =============================================================================
// 显示得分信息
void printScore(const TetrisManager *manager) 
{
//	int8_t i;
	INT16U pos_x_shift = 236;
	INT32U buf_show = setting_frame_buff;
	
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/10000,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/1000%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/100%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score/10%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
	pos_x_shift += (R_drawAsciiChar((unsigned short *)buf_show, 48 + manager->score%10,pos_x_shift,65, 320, 0xffff, 0, R_FONT_NUM5) & 0xff);
}

// =============================================================================
// 显示下一个
void printNextTetris(const TetrisManager *manager) {
	int8_t i,x,y;
	uint16_t tetris;

	// 下一个，用相应颜色显示
	tetris = gs_uTetrisTable[manager->type[1]][manager->orientation[1]];

	for (i = 0; i < 16; ++i) {
		y = (i >> 2) + 11;  // 待设置的行
		if (y > ROW_END)  // 超过底部限制
		{
			break;
		}
		x = (i & 3) + 16;  // 待设置的列
		if ((tetris >> i) & 1)  // 检测的到小方格属于当前方块区域
		{
			show_one_diamonds(x,y,0xF7A1);
		}
	}
}

// =============================================================================
// 给一个方块
void giveTetris(TetrisManager *manager) {
	uint16_t tetris;

	manager->type[0] = manager->type[1];  // 下一个方块置为当前
	manager->orientation[0] = manager->orientation[1];

	manager->type[1] = rand_int() % 7;  // 随机生成下一个方块
	manager->orientation[1] = rand_int() & 3;

	tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]]; // 当前方块
	// 设置当前方块y坐标，保证刚给出时只显示方块最下面一行
	// 这种实现使得玩家可以以很快的速度将方块落在不显示出来的顶部4行内
	if (tetris & 0xF000) {
		manager->y = 0;
	} else {
		manager->y = (tetris & 0xFF00) ? 1 : 2;
	}
	manager->x = 6;  // 设置当前方块x坐标
	
	if (checkCollision(manager))  // 检测到碰撞
	{
		manager->dead = 1;  // 标记游戏结束
	}
	else  // 未检测到碰撞
	{
		insertTetris(manager);  // 将当前方块加入游戏池
	}
}


// =============================================================================
// 移除方块
void removeTetris(TetrisManager *manager) {
	// 当前方块
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	// 当前方块每4位取出，按位取反后位与到游戏池相应位置，即完成移除方块
	manager->pool[manager->y + 0] &=
			~(((tetris >> 0x0) & 0x000F) << manager->x);
	manager->pool[manager->y + 1] &=
			~(((tetris >> 0x4) & 0x000F) << manager->x);
	manager->pool[manager->y + 2] &=
			~(((tetris >> 0x8) & 0x000F) << manager->x);
	manager->pool[manager->y + 3] &=
			~(((tetris >> 0xC) & 0x000F) << manager->x);
}

// =============================================================================
// 显示当前方块
void printCurrentTetris(const TetrisManager *manager,const TetrisControl *control) {
	int8_t x, y;
//	int8_t i = 0, j= 0;

	// 显示当前方块是在移动后调用的，为擦去移动前的方块，需要扩展显示区域
	// 由于不可能向上移动，故不需要向下扩展
	y = (manager->y > ROW_BEGIN) ? (manager->y - 1) : ROW_BEGIN;  // 向上扩展一格
	for (; y < ROW_END && y < manager->y + 4; ++y) {
		x = (manager->x > COL_BEGIN) ? (manager->x - 1) : COL_BEGIN;  // 向左扩展一格
		for (; x < COL_END && x < manager->x + 5; ++x)  // 向右扩展一格
		{
			if ((manager->pool[y] >> x) & 1)  // 游戏池该方格有方块
			{
				// 用相应颜色，显示一个实心方块
				show_one_diamonds( x-2, y-ROW_BEGIN, control->color[y][x]);
			} 
			else
			{
				//show_one_diamonds( x, y-ROW_BEGIN, 0x8c71);
			}
		}
	}
}
// =============================================================================
// 设置颜色
void setPoolColor(const TetrisManager *manager, TetrisControl *control) {
	// 由于显示游戏池时，先要在游戏池里判断某一方格有方块才显示相应方格的颜色
	// 这里只作设置即可，没必要清除
	// 当移动方块或给一个方块时调用

	int8_t i, x, y;

	// 当前方块
	uint16_t tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]];

	for (i = 0; i < 16; ++i) {
		y = (i >> 2) + manager->y;  // 待设置的行
		if (y > ROW_END)  // 超过底部限制
		{
			break;
		}
		x = (i & 3) + manager->x;  // 待设置的列
		if ((tetris >> i) & 1)  // 检测的到小方格属于当前方块区域
		{
			control->color[y][x] = 0xF7A1;  // 设置颜色
		}
	}
}

// =============================================================================
// 旋转方块
void rotateTetris(TetrisManager *manager, TetrisControl *control) {
	int8_t ori = manager->orientation[0];  // 记录原旋转状态

	removeTetris(manager);  // 移走当前方块

	// 顺/逆时针旋转
	manager->orientation[0] =
			(control->clockwise) ? ((ori + 1) & 3) : ((ori + 3) & 3);

	if (checkCollision(manager))  // 检测到碰撞
	{
		manager->orientation[0] = ori;  // 恢复为原旋转状态
		insertTetris(manager);  // 放入当前方块。由于状态没改变，不需要设置颜色
	} else {
		insertTetris(manager);  // 放入当前方块
		setPoolColor(manager, control);  // 设置颜色
		printCurrentTetris(manager, control);  // 显示当前方块
	}
}



// 消行检测
bool checkErasing(TetrisManager *manager, TetrisControl *control) {
	static const unsigned scores[5] = { 0, 50, 150, 300, 400};  // 消行得分
	int8_t count = 0;
	int8_t k = 0, y = manager->y + 3;  // 从下往上检测

	do {
		if (y < ROW_END && manager->pool[y] == 0xFFFFU)  // 有效区域内且一行已填满
				{
			++count;
			// 消除一行方块
			memmove(manager->pool + 1, manager->pool, sizeof(uint16_t) * y);
			// 颜色数组的元素随之移动
			memmove(control->color[1], control->color[0],
					sizeof(int8_t[16]) * y);
		} else {
			--y;
			++k;
		}
	} while (y >= manager->y && k < 4);

	manager->erasedTotal += count;  // 消行总数
	manager->score += scores[count];  // 得分
	//game_gradr += scores[count]; 

	if (count > 0) {
		++manager->erasedCount[count - 1];  // 消行
	}

	giveTetris(manager);  // 给下一个方块
	setPoolColor(manager, control);  // 设置颜色

	return (count > 0);
}


// =============================================================================
// 显示游戏池
void printTetrisPool(const TetrisManager *manager, const TetrisControl *control) {
	int8_t x, y;

	for (y = ROW_BEGIN; y < ROW_END; ++y)  // 不显示顶部4行和底部2行
	{
		// 定点到游戏池中的方格
		for (x = COL_BEGIN; x < COL_END; ++x)  // 不显示左右边界
		{
			if ((manager->pool[y] >> x) & 1)  // 游戏池该方格有方块
			{
				// 用相应颜色，显示一个实心方块
				show_one_diamonds( x-2, y-ROW_BEGIN, control->color[y][x]);
			} else  // 没有方块，显示空白
			{
				//show_one_diamonds( x, y-ROW_BEGIN, 0x8c71);
			}
		}
	}
	printCurrentTetris(manager, control);  // 显示当前方块
	printNextTetris(manager);  // 显示下一个方块
	printScore(manager);  // 显示得分信息
}


// =============================================================================
// 向下移动方块
void moveDownTetris(TetrisManager *manager, TetrisControl *control) 
{
	int8_t y = manager->y;  // 记录原行位置

	removeTetris(manager);  // 移走当前方块
	++manager->y;  // 向下移动

	if (checkCollision(manager))  // 检测到碰撞
	{
		manager->y = y;  // 恢复为原行位置
		insertTetris(manager);  // 放入当前方块。由于位置没改变，不需要设置颜色
		if (checkErasing(manager, control))  // 检测到消行
		{
			printTetrisPool(manager, control);  // 显示游戏池
		}
		//ap_setting_memcpy(buf_show,buf_base,getDispDevBufSize());
	}
	else 
	{
		insertTetris(manager);  // 放入当前方块
		setPoolColor(manager, control);  // 设置颜色
		//printTetrisPool(manager, control);  // 显示游戏池
		printCurrentTetris(manager, control);  // 显示当前方块
	}
}
// =============================================================================
// 水平移动方块
void horzMoveTetris(TetrisManager *manager, TetrisControl *control) {
	int x = manager->x;  // 记录原列位置
	
	removeTetris(manager);  // 移走当前方块
	control->direction == 0 ? (--manager->x) : (++manager->x);  // 左/右移动

	if (checkCollision(manager))  // 检测到碰撞
	{
		manager->x = x;  // 恢复为原列位置
		insertTetris(manager);  // 放入当前方块。由于位置没改变，不需要设置颜色
	} else {
		insertTetris(manager);  // 放入当前方块
		setPoolColor(manager, control);  // 设置颜色
		printCurrentTetris(manager, control);  // 显示当前方块
	}
}

// =============================================================================
// 键按下
void keydownControl(TetrisManager *manager, TetrisControl *control, int key) 
{
//	INT16U tetris;
//	tetris = gs_uTetrisTable[manager->type[0]][manager->orientation[0]]; // 当前方块
	setting_frame_buff = get_game_show_buf(TRUE);

	switch (key) {
		case U:  // 上
			control->clockwise = 1;  // 顺时针旋转
			rotateTetris(manager, control);  // 旋转方块
			break;
		
		case L:  // 左
			control->direction = 1;  // 向左移动
			horzMoveTetris(manager, control);  // 水平移动方块
			break;
		
		case R:  // 右
			control->direction = 0;  // 向右移动
			horzMoveTetris(manager, control);  // 水平移动方块
			break;
		
		case D:  // 下
			moveDownTetris(manager, control);  // 向下移动方块
			break;
		
		default:
			break;
	}
	
	printTetrisPool(manager, control);//显示游戏池
	frame_buff_display();
}

//结束游戏判断
INT8U endgame(TetrisManager *manager)
{
	if(manager->dead == 1)
		return 0;
	return 1;
}
// 重新开始游戏
void restartGame(TetrisManager *manager, TetrisControl *control) 
{
	//INT8U i;
	memset(manager, 0, sizeof(TetrisManager));  // 全部置0
	//gp_memset((INT8S *) manager, 0, sizeof(TetrisManager));
	// 初始化游戏池
	memcpy(manager->pool, gs_uInitialTetrisPool, sizeof(uint16_t[22]));
	
	manager->type[1] = rand_int() % 7;  // 下一个
	manager->orientation[1] = rand_int() & 3;

	memset(control, 0, sizeof(TetrisControl));  // 全部置0

	giveTetris(manager);  // 给下一个方块
	setPoolColor(manager, control);  // 设置颜色
}
// 初始化游戏
void game_tetris_init(TetrisManager *manager, TetrisControl *control)
{ 
	tetris_icon = get_bin_data(RES_GAME_TETRIS_ICON);
	game_bk_draw((u8 *)get_game_base_buf(), RES_GAME_TETRIS_BK, /*NULL*/0);
	get_game_show_buf(TRUE);
	frame_buff_display();
	restartGame(manager, control); 
	sleeptime = 34;
	endGamestatus = 0;
}



void game_tetris_exit()
{
	endGamestatus = 1;
	free_buf(tetris_icon);
}


void game_tetris_entry(void)
{
	u8 msg, err;
	game_tetris_init(&tetrisManager, &tetrisControl);
	while(1){
		
		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_OK:
				keydownControl(&tetrisManager, &tetrisControl,U);
				break;
			case KEY_EVENT_PHOTO:
				keydownControl(&tetrisManager, &tetrisControl,D);
				break;
			case KEY_EVENT_RIGHT:
				keydownControl(&tetrisManager, &tetrisControl,L);
				break;
			case KEY_EVENT_LEFT:
				keydownControl(&tetrisManager, &tetrisControl,R);
				break;
			case KEY_EVENT_POWER:
				game_tetris_exit();
				return;
			case SYS_EVENT_GAME_TICK:
				if(endgame(&tetrisManager))
					keydownControl(&tetrisManager, &tetrisControl,D);
				else
					XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_POWER,0));
				break;
		}
	}
}

