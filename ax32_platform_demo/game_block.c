#include "application.h"
#include "taskgame.h"


#if 1
#define RECT_W 40
#define RECT_H 20
#define MAP_W 8
#define MAP_H 9
#define BLOCK_NUM_HORIZONTAL 8
#define BLOCK_NUM_VERTICAL 9
#define BLOCK_NUM BLOCK_NUM_VERTICAL*BLOCK_NUM_HORIZONTAL

#define BLOCK_BREAKER_BOUNDARY_LEFT		0
#define BLOCK_BREAKER_BOUNDARY_RIGHT	TFT_WIDTH
#define BLOCK_BREAKER_BOUNDARY_TOP		RECT_H
#define BLOCK_BREAKER_BOUNDARY_BOTTOM	TFT_HEIGHT



#define ball_fire			0
#define ball_normal		384
#define block_blue			768
#define block_blue1		2008
#define block_green		3248
#define block_green1		4488
#define block_red			5728
#define block_red1			6968
#define block_yellow		8208
#define block_yellow1		9448
#define board			10688
#define digital_0		12048
#define digital_1		12336
#define digital_2		12624
#define digital_3		12912
#define digital_4		13200
#define digital_5		13488
#define digital_6		13776
#define digital_7		14064
#define digital_8		14352
#define digital_9				14640
#define effect_ball_fire		14928
#define effect_ball_multi		15492
#define effect_diamond			16056
#define effect_rectangle		17296



enum{
	BALL_NORMAL,
	BALL_FIRE,
	BLOCK_BLUE,
	BLOCK_BLUE1,
	BLOCK_GREEN,
	BLOCK_GREEN1,
	BLOCK_RED,
	BLOCK_RED1,
	BLOCK_YELLOW,
	BLOCK_YELLOW1,
	EFFECT_DIAMOND,
	EFFECT_RECTANGLE,
	BOARD,
	DIGITAL_0,
	DIGITAL_1,
	DIGITAL_2,
	DIGITAL_3,
	DIGITAL_4,
	DIGITAL_5,
	DIGITAL_6,
	DIGITAL_7,
	DIGITAL_8,
	DIGITAL_9,
	EFFECT_BALL_FIRE,
	EFFECT_BALL_MULTI
};

INT32U game_block_icon_item[] =
{
	ball_normal,
	ball_fire,      	
	block_blue,      	
	block_blue1,      		
	block_green,      			
	block_green1,     			
	block_red,      			
	block_red1,      		
	block_yellow,     		
	block_yellow1,
	effect_diamond,      		
	effect_rectangle,
	board,      		
	digital_0,      	
	digital_1,      		
	digital_2,     			
	digital_3,      			
	digital_4,      		
	digital_5,      			
	digital_6,      		
	digital_7,      			
	digital_8,      		
	digital_9,    	
	effect_ball_fire,     		
	effect_ball_multi  	
};   	

const DISPLAY_ICONSHOW block_icon_item[] = 
{
	{16, 16, TRANSPARENT_COLOR, 152, 211},
	{41, 20, TRANSPARENT_COLOR, 0, 0},
	{68, 13, TRANSPARENT_COLOR, 126, 227},
	{12, 16, TRANSPARENT_COLOR, 0, 0},
	{12, 31, TRANSPARENT_COLOR, 0, 0},	
};
enum{
	ICON_BALL,
	ICON_BLOCK,
	ICON_BOARD,
	ICON_DIGITAL,
	ICON_POWER_UP_EFFECT
};

enum{
	NONE,
    RIGHT,
	LONG_RIGHT,
    UP,
    LEFT,
    LONG_LEFT,
    DOWN,
    FORWARD
};
typedef struct{
	DISPLAY_ICONSHOW pos;
	u8 *src;
	INT8U type;
	INT16S v_dx;
	INT16S v_dy;
	INT8U hit_lock;
}HIT_STRUCT;
typedef struct{
	DISPLAY_ICONSHOW pos;
	u8 *src;
}BE_HIT_STRUCT;
typedef struct{
	INT32U i;
	INT32U j;
	INT8U posxy_be_hit_item;
	INT8S v_dx_change;
	INT8S v_dy_change;
	INT8U hit_result;
}HIT_INFO;

typedef struct{
	INT16S pos_x;
	INT16S pos_x1;
	INT16S pos_y;
	INT16S pos_y1;
}BOX_BOUNDARY;

#define GAME_BLOCK_BREAKER_START_DELAY	((1 << 5) + (1 << 4))
#define GAME_BLOCK_BREAKER_PASS_DELAY	(1 << 5)	//(1 << 5)
#define GAME_BLOCK_BREAKER_OVER_DELAY	(2 << 5)

#define BALL_MAX_NUM 27
#define POWER_UP_MAX_NUM 3
#define MAP_SIZE 72
#define INFO_STACK_SIZE		128

static u8 *block_icon = NULL;

HIT_STRUCT *ball = NULL;
INT32U ball_num;
BE_HIT_STRUCT board_a;
BE_HIT_STRUCT block_item;
BOX_BOUNDARY game_boundary;
INT8U *block_map = NULL;
INT16S board_move_cnt;
static INT32U score;
INT8U life_num;
//INT8U power_up_sum;
INT32U power_up_num;
HIT_STRUCT *power_up = NULL;
INT16U rand_level_prev = 5;
INT16U effect_cnt;
INT32U block_cnt;
INT32U block_break_pause = 0; // time used to stop motion of board and ball 
INT8U block_break_game_pause = 0; //game-pause
INT8U power_up_rand_list[MAP_SIZE];
INT16S board_move_cnt;
INT16U game_block_breaker_page_encode;


static INT8U first_entry_flag=1;
static INT8U ball_auto_flag=0;
static INT8U exit_judge_flag = 0;

static INT32U game_cnt = 0;
void  game_cnt_down(){
	if(game_cnt > 0)
		game_cnt--;
}

void game_cnt_reinit(INT32U init_cnt){
	game_cnt = init_cnt;
}

INT8U game_cnt_over(){
	return game_cnt ? 0 : 1;
}
void game_cnt_clear(){
	game_cnt = 0;
}


INT16U effect_rand_get(){
	INT16U rand;
	if(effect_cnt == 0){
		effect_cnt = block_cnt/POWER_UP_MAX_NUM;
		return 0;
	}
	else{
		rand = rand_int()%(block_cnt/POWER_UP_MAX_NUM);
		if(!rand)
			effect_cnt = 0;
		return rand;
	}
}



void power_up_rand_list_init(){
	int i, effect_max = POWER_UP_MAX_NUM;
	for(i = 0; i < 72; i++){
		if(block_map[i]&0x80){
			if(effect_max && (!effect_rand_get())){
				effect_max --;
				power_up_rand_list[i] = 1;
			}else{
				power_up_rand_list[i] = 0;
			}
		}
	}
}
void block_cnt_init(){
	INT32U i;
	block_cnt = 0;
	for(i = 0; i < 72; i++)
		if(block_map[i]&0x80)
			block_cnt++;
}
INT16U rand_get(){
	INT16U rand_level_new;
	rand_level_new = rand_int()%6;
	while(rand_level_new == rand_level_prev){
		rand_level_new = rand_int()%6;
	}
	rand_level_prev = rand_level_new;
	return rand_level_new;
}
INT32U level_back_index_get(){
	return (rand_level_prev % 4);
}


void block_map_init(INT16U level)
{
	do{
		block_map = DataRead_CMode(RES_GAME_BLOCK_MAP,MAP_SIZE * level, MAP_SIZE);
	}while(block_map == NULL);
}

void move_icon_universal_draw(DISPLAY_ICONSHOW icon, u8 *frame_buff, u8 *src, BOX_BOUNDARY boundary)
{
	icon.transparent = TRANSPARENT_COLOR;
	if(icon.pos_x < 0)
		icon.pos_x = 0;
	if(icon.pos_y < 0)
		icon.pos_y = 0;
	frame_icon_draw(frame_buff, src, &icon, /*NULL*/0);
}

u8 *game_frame_buff_show_get()
{
	return (u8 *)setting_frame_buff;
}

void block_icon_draw(DISPLAY_ICONSHOW icon, u8 *src)
{
	u8 *frame_buff = (u8 *)game_frame_buff_show_get();
	BOX_BOUNDARY game_boundary;
	game_boundary.pos_x = 0;
	game_boundary.pos_y = RECT_H;
	game_boundary.pos_x1 = TFT_WIDTH;
	game_boundary.pos_y1 = TFT_HEIGHT;
	move_icon_universal_draw(icon, frame_buff, src, game_boundary);
}


void block_src_init_and_draw(INT8U src_type, INT16S pos_x, INT16S pos_y){
	INT8U num = src_type + BLOCK_BLUE;
	block_item.src = &block_icon[game_block_icon_item[num]];
	block_item.pos = block_icon_item[1];
	block_item.pos.pos_x = pos_x;
	block_item.pos.pos_y = pos_y;
	block_icon_draw(block_item.pos, block_item.src);
}

void block_map_draw(){
	INT32U i, j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){			//9
			if(block_map[i + 8*j]&0xe0){
				block_src_init_and_draw((block_map[i + 8*j]&0x0f), i * RECT_W, (j + 1) * RECT_H);
			}
		}
	}
}

void game_boundary_init()
{
	game_boundary.pos_x = 0;
	game_boundary.pos_y = RECT_H;
	game_boundary.pos_x1 = TFT_WIDTH;
	game_boundary.pos_y1 = TFT_HEIGHT;

}

void game_block_level_init(INT32U index)
{
	game_bk_draw((u8 *)get_game_base_buf(), index + RES_GAME_BLOCK_BK0, /*NULL*/0);
}


void game_block_exit()
{
	deg_Printf("%s exit!!!-----> key exit\n", __func__);
	endGamestatus = 0;
	sleeptime = 60;	
	first_entry_flag=0;
	block_free_buf();
}
void life_icon_draw(){
	INT32U i;
	DISPLAY_ICONSHOW life = block_icon_item[0];
	u8 *frame_buff = (u8 *)game_frame_buff_show_get();
	BOX_BOUNDARY boundary;
	boundary.pos_x = 0;
	boundary.pos_y = 0;
	boundary.pos_x1 = TFT_WIDTH;
	boundary.pos_y1 = TFT_HEIGHT;
	
	for(i = 0; i < life_num; i++){
		life.pos_x = i * (life.icon_w + 4) + 4;
		life.pos_y = 2;
		move_icon_universal_draw(life, frame_buff, &block_icon[ball_normal], boundary);
	}
}
void right_score_draw(){
	INT32U i, offset, temp = score;
	DISPLAY_ICONSHOW score = block_icon_item[3];
	u8 *frame_buff = (u8 *)game_frame_buff_show_get();
	BOX_BOUNDARY boundary;
	boundary.pos_x = 0;
	boundary.pos_y = 0;
	boundary.pos_x1 = TFT_WIDTH;
	boundary.pos_y1 = TFT_HEIGHT;
	for(i = 0; i < 7; i++){
		score.pos_x = TFT_WIDTH - (i + 1) * (score.icon_w + 1);
		score.pos_y = 2;
		offset = temp%10;
		temp = temp/10;
		move_icon_universal_draw(score, 
			frame_buff, 
			(block_icon + game_block_icon_item[DIGITAL_0 + offset]), 
			boundary
			);
	}
}
void ball_icon_draw(){
	INT32U i;
	for(i = 0; i < ball_num; i++){
		block_icon_draw(ball[i].pos, ball[i].src);
	}	
}
void board_icon_draw(){
	block_icon_draw(board_a.pos, board_a.src);
}
void power_up_item_icon_draw(u8 *frame_buff){
	INT32U i;
	for(i = 0; i < power_up_num; i++){
		move_icon_universal_draw(power_up[i].pos, frame_buff, power_up[i].src, game_boundary);
	}
}

void DrawMap_block()
{
	u8 *buf_show = (u8 *)game_frame_buff_show_get();
	
	block_map_draw();
	ball_icon_draw();
	board_icon_draw();
	life_icon_draw();
	right_score_draw();
	power_up_item_icon_draw(buf_show);
}
void power_up_item_init(INT16S pos_x, INT16S pos_y, INT8U type){
	
	if(type&0x03){
		power_up[power_up_num].src = block_icon + effect_ball_fire;
	}else{
		power_up[power_up_num].src = block_icon + effect_ball_multi;
	}
	power_up[power_up_num].pos = block_icon_item[4];
	power_up[power_up_num].v_dx = 0;
	power_up[power_up_num].v_dy = 2;
	power_up[power_up_num].pos.pos_x = pos_x;
	power_up[power_up_num].pos.pos_y = pos_y;
	power_up[power_up_num].type = type;
	power_up_num++;

}
void power_up_item_destroy(INT32U i){
	power_up_num--;
	if(i != power_up_num){
		power_up[i] = power_up[power_up_num];
	}
}

void ball_init(INT16S v_dx, INT16S v_dy, INT8U type, DISPLAY_ICONSHOW icon){
	if(type&0x3f){
		ball[ball_num].src = block_icon + ball_fire;
	}else{
		ball[ball_num].src = block_icon + ball_normal;
	}
	ball[ball_num].pos = icon;
	ball[ball_num].v_dx = v_dx;
	ball[ball_num].v_dy = v_dy;
	ball[ball_num].type = type;
	ball[ball_num].hit_lock = 0;
	ball_num++;
}

void ball_item_destroy(INT32U i){
	ball_num--;
	if(i != ball_num){
		ball[i] = ball[ball_num];
	}
}
void copy_ball_action(HIT_STRUCT *hit, INT16S v_x_change){
	if(0 == hit->v_dy){
		return;
	}
	ball_init(hit->v_dx + v_x_change, hit->v_dy, hit->type, hit->pos);
}

void ball_and_board_init()
{
	ball_num = 0;
	ball_init(0, 0, 0x00, block_icon_item[0]);
	board_a.pos = block_icon_item[2];
	board_a.src = block_icon + board;
}

void board_and_ball_flash_init(){
	ball_and_board_init();
	board_move_cnt = 0;
}

void move_rule(HIT_STRUCT *hit)
{
	if((hit->v_dx == 0)&&(hit->v_dy == 0)){
		if(board_move_cnt > 0){
			hit->v_dx = 3;
			hit->v_dy = -3;
		}else if(board_move_cnt < 0){
			hit->v_dx = -3;
			hit->v_dy = -3;
		}
	}
	hit->pos.pos_x += hit->v_dx;
	hit->pos.pos_y += hit->v_dy;
	if(hit->pos.pos_x < BLOCK_BREAKER_BOUNDARY_LEFT)  
		hit->pos.pos_x = BLOCK_BREAKER_BOUNDARY_LEFT;
	else if(hit->pos.pos_x > BLOCK_BREAKER_BOUNDARY_RIGHT - hit->pos.icon_w)
		hit->pos.pos_x = BLOCK_BREAKER_BOUNDARY_RIGHT - hit->pos.icon_w;
	
	if(hit->pos.pos_y < BLOCK_BREAKER_BOUNDARY_TOP)
		hit->pos.pos_y = BLOCK_BREAKER_BOUNDARY_TOP;
	else if(hit->pos.pos_y > BLOCK_BREAKER_BOUNDARY_BOTTOM - hit->pos.icon_h)
		hit->pos.pos_y = BLOCK_BREAKER_BOUNDARY_BOTTOM - hit->pos.icon_h;
		
	
}

INT8U game_boundary_hit_check(HIT_STRUCT *hit)
{
	if(hit->pos.pos_y == game_boundary.pos_y1 - hit->pos.icon_h)
		return 0;
	else{
		if(hit->pos.pos_y == game_boundary.pos_y){
			hit->v_dy = - hit->v_dy;
			game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);
		}else if((hit->pos.pos_x == game_boundary.pos_x) || (hit->pos.pos_x == game_boundary.pos_x1 - hit->pos.icon_w)){
			hit->v_dx = - hit->v_dx;
			game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);
		}
		return 1;
	}
		
}


void universal_hit_action(HIT_INFO *hit_info, HIT_STRUCT *hit){
	hit->v_dx = hit_info->v_dx_change * hit->v_dx;
	hit->v_dy = hit_info->v_dy_change * hit->v_dy;
}
void universal_hit_check(HIT_STRUCT *hit, BE_HIT_STRUCT *be_hit, HIT_INFO *hit_info){
	INT8U  x_flag, y_flag, dx_flag = 3, dy_flag = 3;
	INT16S x_len=0, y_len=0;
	INT16U code;
	(*hit_info).v_dx_change = 1;
	(*hit_info).v_dy_change = 1;

	if((hit->pos.pos_x < (*be_hit).pos.pos_x) && (hit->pos.pos_x + hit->pos.icon_w > (*be_hit).pos.pos_x)){
		x_flag = 1;
		x_len = hit->pos.pos_x + hit->pos.icon_w - (*be_hit).pos.pos_x;
	}else if((hit->pos.pos_x < ((*be_hit).pos.pos_x + (*be_hit).pos.icon_w))&&(hit->pos.pos_x + hit->pos.icon_w > ((*be_hit).pos.pos_x + (*be_hit).pos.icon_w))){
		x_flag = 2;
		x_len = ((*be_hit).pos.pos_x + (*be_hit).pos.icon_w) - hit->pos.pos_x;
	}else if((hit->pos.pos_x > (*be_hit).pos.pos_x)&&(hit->pos.pos_x + hit->pos.icon_w < ((*be_hit).pos.pos_x + (*be_hit).pos.icon_w))){
		x_flag = 3;
	}else{
		x_flag = 0;
	}
	if((hit->pos.pos_y < (*be_hit).pos.pos_y)&&(hit->pos.pos_y + hit->pos.icon_h > (*be_hit).pos.pos_y)){
		y_flag = 1;
		y_len = hit->pos.pos_y + hit->pos.icon_h - (*be_hit).pos.pos_y;
	}else if((hit->pos.pos_y < ((*be_hit).pos.pos_y + (*be_hit).pos.icon_h))&&(hit->pos.pos_y + hit->pos.icon_h > ((*be_hit).pos.pos_y + (*be_hit).pos.icon_h))){
		y_flag = 2;
		y_len = ((*be_hit).pos.pos_y + (*be_hit).pos.icon_h) - hit->pos.pos_y;
	}else if((hit->pos.pos_y > (*be_hit).pos.pos_y)&&(hit->pos.pos_y + hit->pos.icon_w < ((*be_hit).pos.pos_y + (*be_hit).pos.icon_h))){
		y_flag = 3;
	}else{
		y_flag = 0;
	}
	
	if((x_flag != 0)&&(y_flag != 0)){
		if(hit->v_dx > 0){
			dx_flag = 2;
		}else if(hit->v_dx < 0){
			dx_flag = 1;
		}else{
			dx_flag = 0;
		}
		if(hit->v_dy > 0){
			dy_flag = 2;
		}else if(hit->v_dy < 0){
			dy_flag = 1;
		}else{
			dy_flag = 0;
		}
	}
	
	if((dx_flag == 3) && (dy_flag == 3)){
		(*hit_info).posxy_be_hit_item = 0;
		(*hit_info).hit_result = 0;
	}else{
		(*hit_info).posxy_be_hit_item = ((INT8U)((((*hit_info).i + 1) << 4)|((*hit_info).j +1)));
		(*hit_info).hit_result = 1;
		if((x_flag == 3) || (dx_flag == 0)){
			(*hit_info).v_dy_change = -1;
		}else if((y_flag == 3) || (dy_flag == 0)){
			(*hit_info).v_dx_change = -1;
		}else{
			code = (x_flag << 12) | (y_flag << 8) | (dx_flag << 4) | (dy_flag);
			switch(code){
				case 0x2221:
				case 0x2122:
				case 0x1211:
				case 0x1112:
					(*hit_info).v_dy_change = -1;
					break;
				case 0x1121:
				case 0x1222:
				case 0x2111:
				case 0x2212:
					(*hit_info).v_dx_change = -1;
					break;
				case 0x1122:
				case 0x2112:
				case 0x2211:
				case 0x1221:
					if(y_len > x_len + 1){
						(*hit_info).v_dx_change = -1;
					}else if(y_len + 1 < x_len){
						(*hit_info).v_dy_change = -1;
					}else{
						(*hit_info).v_dx_change = -1;
						(*hit_info).v_dy_change = -1;
					}
					break;
				default:
					break;
			}
		}
	}
}

void board_hit_check(HIT_STRUCT *hit, BE_HIT_STRUCT *be_hit){
	HIT_INFO hit_info;
	universal_hit_check(hit, be_hit, &hit_info);
	if(hit_info.hit_result){
		if(!hit->hit_lock){
			universal_hit_action(&hit_info, hit);
			hit->pos.pos_y = be_hit->pos.pos_y -hit->pos.icon_h + 3;
			if((hit->v_dx != 0) && (hit->v_dy != 0)){
				game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);
			}
			if(board_move_cnt < 0){
				hit->v_dx -= 1;
			}else if(board_move_cnt > 0){
				hit->v_dx += 1;
			}
		}
	}else{
		hit->hit_lock = 0;
	}
	//DBG_PRINT("%s finish!!!-----> result\n", __func__);
}

INT8U block_hit_check(HIT_STRUCT *hit){
	INT32U i, j;
	HIT_INFO hit_info;
	hit_info.posxy_be_hit_item = 0;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){					//9
			if(block_map[i + 8*j]&0x80){
				block_item.pos.pos_x = i * RECT_W;
				block_item.pos.pos_y = (j + 1) * RECT_H;
				hit_info.i = i;
				hit_info.j = j;
				universal_hit_check(hit, &block_item, &hit_info);
				if(hit_info.posxy_be_hit_item){
					//DBG_PRINT("posxy_be_hit_item in %s is %x", __func__, hit_info.posxy_be_hit_item);
					if(hit->type & 0x3f){
						hit->type = (hit->type &0xc0) | ((hit->type & 0x3f) - 1);
						if(!(hit->type & 0x3f)){
							hit->src = block_icon + ball_normal;
						}
					}else{
						universal_hit_action(&hit_info, hit);					
					}
					game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);	//EFFECT_CORRECT);
					return hit_info.posxy_be_hit_item;
				}	
			}
		}
	}
	return hit_info.posxy_be_hit_item;
}


void clear_block_effect(INT8U posxy){
	INT32U 	i = (posxy >> 4) - 1,
			j = (posxy&0x0f) - 1,
			x, y;
	INT8U diamond = EFFECT_DIAMOND  - BLOCK_BLUE;
	INT8U rectangle = EFFECT_RECTANGLE - BLOCK_BLUE;
	INT8U effect_num =  diamond + rand_int()%2;
	INT16U rand = rand_int()%2;
	for(x = 0; x < 8; x++){
		for(y = 0; y < 9; y++){
			if(((block_map[x + 8*y] & 0x0f) == diamond) || ((block_map[x + 8*y] & 0x0f) == rectangle)){
				if(block_map[x + 8*y] & 0x60){
					block_map[x + 8*y] = block_map[x + 8*y] - 0x20;
				}else{
					block_map[x + 8*y] = 0x00;
					if(power_up_rand_list[x + 8*y]){
						if(rand){
							power_up_item_init(x * RECT_W, y * RECT_H, 0x30);
						}else{
							power_up_item_init(x * RECT_W, y * RECT_H, 0x03);
						}
					}	
				}
			}
		}
	}
	if(posxy){
		block_map[i + 8*j] = (block_map[i + 8*j] & 0) | ( 0x40 | effect_num);
		score += 10;
	}

	
}



void board_move(HIT_STRUCT *hit, BE_HIT_STRUCT *be_hit){
	if((hit->v_dx == 0)&&(hit->v_dy == 0))
		return;
	if(board_move_cnt < 0){
		if((*be_hit).pos.pos_x < game_boundary.pos_x + 3){
			(*be_hit).pos.pos_x = game_boundary.pos_x;
		}else{
			(*be_hit).pos.pos_x -= 8;
		}
		board_move_cnt ++;
	}else if(board_move_cnt > 0){
		if((*be_hit).pos.pos_x > game_boundary.pos_x1 - (*be_hit).pos.icon_w - 4){
			(*be_hit).pos.pos_x = game_boundary.pos_x1 - (*be_hit).pos.icon_w;
		}else
			(*be_hit).pos.pos_x += 8;
		board_move_cnt --;
	}
}

void ball_move(){
	INT32U i/*, j*/;
	INT8U clear_flag;
//	HIT_STRUCT *temp;
	for(i = 0; i < ball_num;){
		
		move_rule(ball + i);
		board_hit_check(ball + i, &board_a);
		clear_flag = block_hit_check(ball + i);
		clear_block_effect(clear_flag);
		if(!game_boundary_hit_check(ball + i)){
			ball_item_destroy(i);
		}else{
			i++;
		}
	}
}
void ball_fire_power_up(){
	INT32U i;
	
	for(i = 0; i < ball_num; i++){
		ball[i].type = 0x06;
		ball[i].src = block_icon + ball_fire;
	}
}
void ball_multi_power_up(){
	INT32U i, prev_num = ball_num;
	
	for(i = 0; i < prev_num; i++){
		copy_ball_action(ball + i, 1);
		copy_ball_action(ball + i, -1);
	}
}

INT8U power_up_item_move(HIT_STRUCT *hit, INT32U i){
	INT8U result = 0;
	hit->pos.pos_x += hit->v_dx;
	hit->pos.pos_y += hit->v_dy;
	
	if(hit->pos.pos_y > TFT_HEIGHT - board_a.pos.icon_h - hit->pos.icon_h){
		if(hit->pos.pos_x <  board_a.pos.pos_x +  board_a.pos.icon_w){
			if(hit->pos.pos_x + hit->pos.icon_w > board_a.pos.pos_x)
				result = 1;
		}
	}
	if(result){
		if(hit->type == 0x03){
			ball_fire_power_up();
		}else{
			ball_multi_power_up();
		}
		game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);//EFFECT_CORRECT);
		power_up_item_destroy(i);
		return 0;
	}
	 if(hit->pos.pos_y > game_boundary.pos_y1){
		power_up_item_destroy(i);
		return 0;
	}
	return 1;
}

void power_up_move(){
	INT32U i;
	for(i = 0; i < power_up_num;)
		if(power_up_item_move(power_up + i, i))
			i++;
}

INT8U move(){
	if(ball_num){
		board_move(ball, &board_a);
	}
	if(power_up_num){
		power_up_move();
	}
	if(ball_num && (!block_break_pause)){
		ball_move();
	}else if(block_break_pause){
		block_break_pause--;
	}
	if(ball_num){
		return 1;
	}else{
		return 0;
	}
}
INT8U pass_check(){
	INT32U i, j, flag = 1;
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)			//9
			if(block_map[i + 8*j] != 0x00)
				flag = 0;
	return flag;
}

void game_block_init()
{
	first_entry_flag=0;
	game_block_breaker_page_encode = 0x0000;
	sleeptime = 1000;
	endGamestatus = 0;	//4
}

void block_free_buf(){

	free_buf(ball);

	free_buf(power_up);

	free_buf(block_map);

	free_buf(block_icon);
}

void flash_reinit(){
//	INT32U i;
	block_free_buf();

	ball = (HIT_STRUCT *)hal_sysMemMalloc(sizeof(HIT_STRUCT) * BALL_MAX_NUM, 32);
	power_up = (HIT_STRUCT *)hal_sysMemMalloc(sizeof(HIT_STRUCT) * POWER_UP_MAX_NUM, 32);
	block_break_pause = 0;
	block_icon = get_bin_data(RES_GAME_BLOCK_ICON);
	
	power_up_num = 0;
	//power_up_sum = POWER_UP_MAX_NUM;
	game_boundary_init();
	block_map_init(rand_get());
	block_cnt_init();
	power_up_rand_list_init();
	ball_and_board_init();
	game_block_level_init(level_back_index_get());
	frame_buff_display();

}
void flash_init()
{
	life_num = 3;
	score = 0;
	flash_reinit();
}


void block_breaker_going_key_response(){
	
	INT8U key = (game_block_breaker_page_encode & 0x000f);
	
	if(!block_break_pause){		
		switch(key){
			case LEFT:
				board_move_cnt = -10;
				break;
			case RIGHT:
				board_move_cnt = 10;
				break;
			case LONG_LEFT:
				board_move_cnt = -4;
				break;
			case LONG_RIGHT:
				board_move_cnt = 4;
				break;
			default:
				break;
		}
	}
	game_block_breaker_page_encode &= 0xfff0;
}

static void key_active(INT8U key){
	game_block_breaker_page_encode = (game_block_breaker_page_encode & 0xfff0) | key;
}
void game_block_start(){
	if(game_cnt_over()){
		flash_init();
		if(first_entry_flag==0)
		{
		DrawMap_block();	//@add
		
		first_entry_flag=2;
		}
		game_block_breaker_page_encode = 0x1000;
	}else{
		game_cnt_down();
	}
	if((game_block_breaker_page_encode & 0x000f) == FORWARD){
		flash_init();
		game_cnt_clear();
		game_block_breaker_page_encode = 0x1000;
	}
	game_block_breaker_page_encode &= 0xfff0;
}


void block_breaker_going(){
	if(ball_auto_flag==1)
	{
		key_active(LONG_RIGHT);//LONG_RIGHT
		ball_auto_flag=0;
	}
	block_breaker_going_key_response();
	if(move()){
		
		DrawMap_block();
		if(pass_check()){			
			game_cnt_reinit(GAME_BLOCK_BREAKER_PASS_DELAY);
			game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);	//EFFECT_SUCCESS
			game_block_breaker_page_encode |= 0x0100;
		}
	}else if(life_num){
		life_num--;
		board_and_ball_flash_init();
		
		DrawMap_block();
		block_break_pause = (1 << 3);
	}else{
		endGamestatus = 1;
		exit_judge_flag = 1;
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_POWER,0));
		block_free_buf();
		
		game_block_breaker_page_encode = (game_block_breaker_page_encode & 0xf0ff) | 0x0200;
	}	
}
void block_breaker_level_pass(){
	if(game_cnt_over()){
		flash_reinit();
		game_block_breaker_page_encode = 0x1000;
	}else{
		game_cnt_down();
	}
	game_block_breaker_page_encode &= 0xfff0;
}

void block_breaker_game_over_show(){
	if((game_block_breaker_page_encode & 0x000f) == FORWARD){
		game_block_breaker_page_encode = 0x0000;
	}
	if(game_cnt_over()){
		game_block_breaker_page_encode = 0x0000;
	}else{
		game_cnt_down();
	}
	game_block_breaker_page_encode &= 0xfff0;
}
void block_breaker_game_pause(){
	if((game_block_breaker_page_encode & 0x000f) == FORWARD){
		game_block_level_init(level_back_index_get());
		game_audio_play(RES_GAME_BLOCK_KNOCK,0,0);						//EFFECT_CORRECT);
		block_break_pause = 0;
		game_block_breaker_page_encode = 0x1000;
	}
}
static void flash_draw(){	
	setting_frame_buff = get_game_show_buf(TRUE);
	switch(game_block_breaker_page_encode&0xff00){
		case 0x0000:
			game_block_start();
			break;
		case 0x1000:
			block_breaker_going();
			break;
		case 0x1100:
			block_breaker_level_pass();
			break;
		case 0x1200:
			block_breaker_game_over_show();
			break;
		case 0x1300:
			block_breaker_game_pause();
			break;
		default:
			break;
	}
	if(first_entry_flag==2)
	{
		first_entry_flag=1;
		
	}
	frame_buff_display();
}


void game_block_key_active()
{
	u8 get_key = game_key_judge();
	if(get_key == GAME_KEY_R)
		key_active(LONG_RIGHT);
	else if(get_key == GAME_KEY_L)
		key_active(LONG_LEFT);
}

void game_block_entry(void)
{
	u8 msg, err;
	game_block_init();
	game_bk_draw((u8 *)get_game_base_buf(), /*NULL*/0, 0x8080);
	flash_draw();
	flash_draw();
	if(!game_tips(R_ID_STR_GAME_TIPS_START)){
		game_block_exit();
		return;
	}
	while(1){
		msg = (u8)(u32)XMsgQPend(SysCtrl.sysQ,&err); 
		switch (msg){
			case KEY_EVENT_OK:
				//endGamestatus = !endGamestatus;
				break;
			case KEY_EVENT_POWER:
				if(exit_judge_flag){
					exit_judge_flag = 0;
					if(!game_tips(R_ID_STR_GAME_TIPS_PLAY_AGAIN)){
						game_block_exit();
						return;
					}
				}
				else{
					if(game_tips(R_ID_STR_GAME_TIPS_EXIT)){
						game_block_exit();
						return;
					}
				}
				break;
			case SYS_EVENT_GAME_TICK:
				game_block_key_active();
				flash_draw();
				break;
		}
	}
}
#endif



