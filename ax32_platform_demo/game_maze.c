#include "application.h"
#include "taskgame.h"


void maze_DrawMap();
void show_maze_win();
void maze_show_all();
#define MAP_W 22
#define MAP_H 15

#define RECT_W 14//(TFT_HEIGHT/(MAP_W+2))
#define RECT_H 14//(TFT_HEIGHT/(MAP_H+2))


#define MAZE_MAP_EMPTY		4
#define MAZE_MAP_DESTINATION 2
#define MAZE_MAP_WALL		1
#define MAZE_MAP_BOX			6
#define MAZE_MAP_MAN			8
#define MAZE_MAP_BOX_SUCCESS	16
INT8U *maze_map[MAP_W];


extern void game_back_ground_ini_maze();

INT8U maze_row = 0, maze_column = 0;    // ÈËÎïÔÚµÚ¼¸ÐÐ£¬µÚ¼¸ÁÐ
INT8U maze_oldType = 0;    // ÈËÎïÕ¾×ÅµÄµØ·½Ô­À´ÊÇÊ²Ã´µØÃæ
INT8U maze_oldBoxType = 0; // Ïä×ÓÕ¾×ÅµÄµØ·½Ô­À´ÊÇÊ²Ã´µØÃæ 
INT8U maze_leve = 0;


void Move_maze(INT8S r, INT8S c)
{
	if (maze_map[maze_row + r][maze_column + c] != MAZE_MAP_WALL)  // Èç¹ûÇ°Ãæ²»ÊÇÇ½±ÚµÄ»°£¬¾Í¿ÉÒÔÒÆ¶¯
	{
		if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX ||
			maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX_SUCCESS)	// Èç¹ûÓöµ½Ïä×ÓµÄ´¦Àí°ì·¨
		{
			if (maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_WALL &&
				maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_BOX &&
				maze_map[maze_row + 2 * r][maze_column + 2 * c] != MAZE_MAP_BOX_SUCCESS)   // Ïä×ÓºóÃæ²»ÊÇÇ½±ÚºÍÏä×Ó
			{
				// ÒÆ¶¯Ïä×Ó
				if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX_SUCCESS)
				{
					maze_oldBoxType = MAZE_MAP_DESTINATION;
				}
				else if (maze_map[maze_row + r][maze_column + c] == MAZE_MAP_BOX)
				{
					maze_oldBoxType = MAZE_MAP_EMPTY;
				}
				maze_map[maze_row + r][maze_column + c] = maze_oldBoxType;
				if (maze_map[maze_row + 2 * r][maze_column + 2 * c] == MAZE_MAP_DESTINATION)
				{
					maze_map[maze_row + 2 * r][maze_column + 2 * c] = MAZE_MAP_BOX_SUCCESS;
				}
				else
				{
					maze_map[maze_row + 2 * r][maze_column + 2 * c] = MAZE_MAP_BOX;
				}
			}
			else {
				return; // Èç¹ûÏä×ÓºóÃæÊÇÇ½±Ú£¬ÄÇÃ´Í£Ö¹ÒÆ¶¯
			}
		}
		maze_map[maze_row][maze_column] = maze_oldType; // ½«ÈËÎï×ß¹ýµÄµØ·½ÉèÎªÔ­À´µÄµØÃæÀàÐÍ
		maze_oldType = maze_map[maze_row + r][maze_column + c]; // ´æ´¢ÈËÎï½ÓÏÂÀ´Òª×ßµÄµØ·½µÄµØÃæÀàÐÍ
		maze_map[maze_row + r][maze_column + c] = MAZE_MAP_MAN;   // ÈËÒÆ¶¯µÄÎ»ÖÃ·¢Éú±ä»¯
	}
}

INT8U resultCheck_maze()
{
	INT8U result = TRUE/*, i, j*/;
	if(MAZE_MAP_MAN!= maze_map[MAP_W-3][MAP_H-1])
		result = FALSE;

	if (result)
	{
		//game_result_tips(R_ID_STR_GAME_TIPS_NEXT_LEVEL, 500, true, true); //show pass a barrier
		  deg_Printf("0-0-0-0-0maze_leve= %d\n",maze_leve);
		//maze_leve=9;
		maze_leve++;
		if(maze_leve>9)
			maze_leve=0;

		//maze_leve=0;
				  deg_Printf("0-0-0-0-0maze_leve= %d\n",maze_leve);

}
	return result;
}

void maze_map_set(INT8U *data)
{

	INT8U i/*, j = 0*/;
	for (i = 0; i < MAP_W; i++)
	{
		maze_map[i] = (data + i * MAP_H );
	}
}
INT16U get_map_maze_size()
{
	return MAP_W * MAP_H;
}

void man_maze_init()
{
	maze_map[2][0] = MAZE_MAP_MAN;
}


void maze_map_init()
{
	INT8U* data;
//	INT8U* data2;
	data = DataRead_CMode(RES_GAME_MAZE_MAP,get_map_maze_size() *maze_leve, get_map_maze_size());
	maze_map_set(data);
	man_maze_init();
	maze_show_all();


}


void maze_show_one_rect(INT16U x, INT16U y,INT8U type)
{
//	INT16U pox,poy;

	DISPLAY_ICONSHOW icon;
	u32 color;

	if (type == MAZE_MAP_WALL)
		color = YUV_RED;
	else if (type == MAZE_MAP_EMPTY)
		color = YUV_BLACK;
	else if (type == MAZE_MAP_MAN)				
	{
		maze_row = x;
		maze_column = y;
		color = YUV_WHITE;
	}
	else
		color = YUV_BLACK;

	icon.icon_w = RECT_W;
	icon.icon_h = RECT_H;
	icon.transparent = TRANSPARENT_COLOR;
	icon.pos_x = x*RECT_W;
	icon.pos_y = y*RECT_H + 12;
	
	frame_icon_draw((u8 *)setting_frame_buff, NULL, &icon, color);
	
}

void maze_DrawMap() {
	INT8U x, y;
	for (x = 0; x < MAP_W; x++)
	{
		for (y = 0; y < MAP_H; y++)
		{
			maze_show_one_rect(x, y, maze_map[x][y]);
			
		}
	}

}

void maze_show_all()
{
	setting_frame_buff = get_game_show_buf(TRUE);
	maze_DrawMap();
	frame_buff_display();

}

void maze_keypress(INT8S r, INT8S c)
{

	Move_maze(r, c);
	maze_show_all();
	if(resultCheck_maze())
	{
		free_buf(maze_map[0]);
		maze_map_init();
	}
}

void maze_init()
{
	maze_leve = 0;
	game_bk_draw((u8 *)get_game_base_buf(), /*NULL*/0, 0x8080);
	maze_map_init();
}


void game_maze_entry(void)
{
	u8 msg, err;
	maze_init();
	//game_entry_animation();
	while(1){
		
		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_OK:
				maze_keypress(0, -1);
				break;
			case KEY_EVENT_PHOTO:
				maze_keypress(0, 1);
				break;
			case KEY_EVENT_RIGHT:
				maze_keypress(1, 0);
				break;
			case KEY_EVENT_LEFT:
	    		maze_keypress(-1, 0);
				break;
			case KEY_EVENT_POWEROFF:
				taskStart(TASK_POWER_OFF,0);
			case KEY_EVENT_POWER:
				free_buf(maze_map[0]);
				return;
		}
	}
}

