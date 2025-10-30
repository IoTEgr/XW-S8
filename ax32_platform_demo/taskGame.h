#ifndef __TASKGAME_H_
#define __TASKGAME_H_


#define TFT_WIDTH		320
#define TFT_HEIGHT		240

#undef TRANSPARENT_COLOR
#define TRANSPARENT_COLOR 0x8c8080

#define RED_COLOR		0x4c5bff


#define	YUV_BLACK	0x8080
#define	YUV_RED 	0x6C5BFF
#define	YUV_RED1 	0x4C5BFF
#define YUV_WHITE	0xFF8080

enum{
GAME_KEY_NONE,
GAME_KEY_L,
GAME_KEY_U,
GAME_KEY_D,
GAME_KEY_R,
GAME_KEY_OK,
GAME_KEY_POWER,
};

typedef struct
{
	INT16U	icon_w;
	INT16U	icon_h;
	INT32U	transparent;
	INT16S	pos_x;
	INT16S	pos_y;
} DISPLAY_ICONSHOW;

typedef void (*msgGameFunc)(void);

extern msgGameFunc gameEntry;
extern bool endGamestatus;
extern u32 setting_frame_buff;
extern u16 sleeptime;

extern void game_snake_entry(void);
extern void game_sokoban_entry(void);
extern void game_tetris_entry(void);
extern void game_block_entry(void);
extern void game_maze_entry(void);

extern u16 rand_int(void);
extern u8 *DataRead_CMode(int id,INT32S offset,INT32S size);
extern u8 *get_bin_data(u32 id);
extern u32 get_game_base_buf(void);
extern u32 get_game_show_buf(bool show_bk);
extern bool game_tips(resID tips_msg);
extern void game_bk_draw(u8 *y_buf, u32 jpg, u32 color);
extern void game_over(bool osd_init);
extern u8 game_audio_play(u32 id,u8 wait,u8 finish);
extern u8 game_key_judge(void);
extern void block_free_buf(void);
extern void free_buf(void *p);
extern void frame_buff_display(void);
extern void frame_icon_draw(u8 *y_buf, u8 *icon_y_buf, DISPLAY_ICONSHOW *icon, u32 color);
extern void show_icon_rect(u16 x, u16 y,u8 type);
extern void fps_count_reset(void);


#endif
  