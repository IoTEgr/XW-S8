#include "application.h"
#include "taskgame.h"



/******************************ui***************************/
//20x20


#define icon_box	  			0 
#define icon_destination	  	1350	
#define icon_end	  			2700
#define icon_floor				4050
#define icon_man	  			5400
#define icon_wall	   			6750


static u8 *txz_icon = 0;



void DrawMap();
void show_win();
void TXZ_show_all();
#define MAP_W 8
#define MAP_H 8

#define RECT_W 30//(TFT_HEIGHT/(MAP_W+2))
#define RECT_H 30//(TFT_HEIGHT/(MAP_H+2))

/*
	 ¿ÕµØ
	 Ä¿µÄµØ   ¡î
	 Ç½±Ú     ¡ö
	 Ïä×Ó     ¨
	 Íæ¼Ò     ¡â
	 Ïä×Óµ½´ïÄ¿µÄµØ  ¡ï
*/
#define MAP_EMPTY		4
#define MAP_DESTINATION 2
#define MAP_WALL		1
#define MAP_BOX			6
#define MAP_MAN			8
#define MAP_BOX_SUCCESS	16

static INT8U *map[MAP_W];

static INT8U row = 0, column = 0;    // ÈËÎïÔÚµÚ¼¸ÐÐ£¬µÚ¼¸ÁÐ
static INT8U oldType = MAP_EMPTY;    // ÈËÎïÕ¾×ÅµÄµØ·½Ô­À´ÊÇÊ²Ã´µØÃæ
static INT8U oldBoxType = 0; // Ïä×ÓÕ¾×ÅµÄµØ·½Ô­À´ÊÇÊ²Ã´µØÃæ 
static INT8U leve = 0;


void Move(INT8S r, INT8S c)
{
	if (map[row + r][column + c] != MAP_WALL)  // Èç¹ûÇ°Ãæ²»ÊÇÇ½±ÚµÄ»°£¬¾Í¿ÉÒÔÒÆ¶¯
	{
		if (map[row + r][column + c] == MAP_BOX ||
			map[row + r][column + c] == MAP_BOX_SUCCESS)	// Èç¹ûÓöµ½Ïä×ÓµÄ´¦Àí°ì·¨
		{
			if (map[row + 2 * r][column + 2 * c] != MAP_WALL &&
				map[row + 2 * r][column + 2 * c] != MAP_BOX &&
				map[row + 2 * r][column + 2 * c] != MAP_BOX_SUCCESS)   // Ïä×ÓºóÃæ²»ÊÇÇ½±ÚºÍÏä×Ó
			{
				// ÒÆ¶¯Ïä×Ó
				if (map[row + r][column + c] == MAP_BOX_SUCCESS)
				{
					oldBoxType = MAP_DESTINATION;
				}
				else if (map[row + r][column + c] == MAP_BOX)
				{
					oldBoxType = MAP_EMPTY;
				}
				map[row + r][column + c] = oldBoxType;
				if (map[row + 2 * r][column + 2 * c] == MAP_DESTINATION)
				{
					map[row + 2 * r][column + 2 * c] = MAP_BOX_SUCCESS;
				}
				else
				{
					map[row + 2 * r][column + 2 * c] = MAP_BOX;
				}
			}
			else {
				return; // Èç¹ûÏä×ÓºóÃæÊÇÇ½±Ú£¬ÄÇÃ´Í£Ö¹ÒÆ¶¯
			}
		}
		map[row][column] = oldType; // ½«ÈËÎï×ß¹ýµÄµØ·½ÉèÎªÔ­À´µÄµØÃæÀàÐÍ
		oldType = map[row + r][column + c]; // ´æ´¢ÈËÎï½ÓÏÂÀ´Òª×ßµÄµØ·½µÄµØÃæÀàÐÍ
		map[row + r][column + c] = MAP_MAN;   // ÈËÒÆ¶¯µÄÎ»ÖÃ·¢Éú±ä»¯
	}
}

INT8U resultCheck()
{
	INT8U result = TRUE, i, j;
	for (i = 0; i < MAP_W; i++)
	{
		for (j = 0; j < MAP_H; j++)
		{
			if (map[i][j] == MAP_DESTINATION || map[i][j] == MAP_BOX)
			{
				result = FALSE;
				goto end;
			}
		}

	}
end:
	if (result)
	{
		//show_win(GAME_WIN);//ÏÔÊ¾½øÈëÏÂÒ»¹Ø
		if(leve++>11)
			leve=0;
	}
	return result;
}

void map_init(INT8U *data)
{

	INT8U i/*, j = 0*/;
	for (i = 0; i < MAP_W; i++)
	{
		map[i] = (data + i * MAP_H );
	}
}
INT32S get_map_size()
{
	return MAP_W * MAP_H;
}

void man_init(INT8U *data)
{
	oldType = MAP_EMPTY;
	
	if(leve == 1){
	map[data[1]][data[0]+1] = MAP_MAN;
	}else
	map[data[1]][data[0]] = MAP_MAN;
}


u8 *DataRead_CMode(int id,INT32S offset,INT32S size)
{
	u8 *data_adr =  get_bin_data(id);
	u8 *offset_adr = (u8 *)hal_sysMemMalloc(size,32);

	memcpy(offset_adr, data_adr + offset, size);

	free_buf(data_adr);
	return offset_adr;
}


void get_game_map()
{
	INT8U* data;
	INT8U* data2;
	
	data = DataRead_CMode(RES_GAME_SOKOBAN_MAP,(get_map_size() + MAP_W)*leve + MAP_W, get_map_size());
	if(leve == 1){
		INT8U i;
		for(i = 63; i < 255; i--){
			
			if(i % 8==0)
				data[i] = MAP_EMPTY;
			else
				data[i] = data[i - 1];
		}
	}
	map_init(data);
	data2 = DataRead_CMode(RES_GAME_SOKOBAN_MAP,(get_map_size() + MAP_W)*leve, 2);
	man_init(data2);
	free_buf(data2);
	TXZ_show_all();
}




void TXZ_show_icon_rect(INT16U x, INT16U y,INT16U type)
{
	u8 * p;
	const u8 *p_icon;
	DISPLAY_ICONSHOW icon;
	
	if(type == MAP_WALL)
		p_icon = txz_icon+icon_wall;
	else if(type == MAP_EMPTY)
		p_icon = txz_icon+icon_floor;
	else if(type == MAP_BOX)
		p_icon = txz_icon+icon_box;
	else if(type == MAP_MAN)
	{
		p_icon = txz_icon+icon_man;
		row = x;
		column = y;
	}
	else if(type == MAP_DESTINATION)
		p_icon = txz_icon+icon_destination;
	else if(type == MAP_BOX_SUCCESS)
		p_icon = txz_icon+icon_end;
	else
		p_icon = txz_icon+icon_floor;
	
	
	p = (u8 *)setting_frame_buff;
	
	icon.icon_w = RECT_W;
	icon.icon_h = RECT_H;
	icon.transparent = TRANSPARENT_COLOR;
	icon.pos_x = x*RECT_W + 32;
	icon.pos_y = y*RECT_H;
	
	frame_icon_draw(p, (u8*)p_icon, &icon, /*NULL*/0);
	
}

void TXZ_show_one_rect(INT16U x, INT16U y,INT8U type)
{
//	INT16U * p;
	if(RECT_W == 30)
	{
		TXZ_show_icon_rect(x,y,type);
		return;
	}
}

void DrawMap() {
	INT8U x, y;
	for (x = 0; x < MAP_W; x++)
	{
		for (y = 0; y < MAP_H; y++)
		{
			TXZ_show_one_rect(x, y, map[x][y]);
			
		}
	}

}

void TXZ_show_all()
{
	
	setting_frame_buff = get_game_show_buf(TRUE);
	DrawMap();
	frame_buff_display();
}

void sokoban_init()
{
	txz_icon = get_bin_data(RES_GAME_SOKOBAN_ICON);
	leve = 0;
	game_bk_draw((u8 *)get_game_base_buf(), /*NULL*/0, 0x8080);
	get_game_map();
}


void game_sokoban_entry(void)
{
	u8 msg, err;
	sokoban_init();
	while(1){
		
		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_OK:
				Move(0, -1);
				TXZ_show_all();
				if(resultCheck())
				{
					free_buf(map[0]);
					get_game_map();
				}
				break;
			case KEY_EVENT_PHOTO:
				Move(0, 1);
				TXZ_show_all();
				if(resultCheck())
				{
					free_buf(map[0]);
					get_game_map();
				}
				break;
			case KEY_EVENT_RIGHT:
				Move(1, 0);
				TXZ_show_all();
				if(resultCheck())
				{
					free_buf(map[0]);
					get_game_map();
				}
				break;
			case KEY_EVENT_LEFT:
	    		Move(-1, 0);
				TXZ_show_all();
				if(resultCheck())
				{
					free_buf(map[0]);
					get_game_map();
				}
				break;
			case KEY_EVENT_POWER:
				free_buf(txz_icon);
				free_buf(map[0]);
				return;
			case SYS_EVENT_GAME_TICK:
				
				break;
		}
	}
}


