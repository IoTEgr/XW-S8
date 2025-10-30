#ifndef __MENU_H
#define __MENU_H



#define ARRAY_LEN(a)  (sizeof(a)/sizeof(a[0]))
typedef void (*menuItemProc)(winHandle handle,uint32 parameNum,uint32* parame);
typedef struct
{
	uint32 image;
	uint32 str;
}menuOption;
typedef struct
{
	uint32 configId;
	uint32 image;
	uint32 str;
	uint32 optionSum;
	menuOption* pOption;
}menuItem;
typedef struct
{
	uint32 unselectImage;
	uint32 selectImage;
	uint32 str;
	uint32 itemSum;
	menuItem* pItem;
}menuPage;
typedef struct
{
	uint32 curPage;
	uint32 pageSum;
	menuPage* pPage;
}menu;

#define MENU_OPTION_START(name)  menuOption  menuOption##name[]={
#define MENU_OPTION_IMAGE_STR(image,str) {image,str},
#define MENU_OPTION_IMAGE(image)         {image,(uint32)0},
#define MENU_OPTION_STR(str)             {(uint32)0,(uint32)str},
#define MENU_OPTION_END()           };

#define MENU_ITME_START(name)    menuItem  menuItem##name[]={
#define MENU_ITEM_OPTIONS(name,id,image,str)  {id,image,str,ARRAY_LEN(menuOption##name),menuOption##name},
#define MENU_ITEM_NO_ID(name, image,str)      {(uint32)0,image,str,ARRAY_LEN(menuOption##name),menuOption##name},
#define MENU_ITEM_PROC(itemProc,image,str)    {(uint32)0,image,str,(uint32)0,(menuOption*)itemProc},
#define MENU_ITME_END()             };

#define MENU_PAGE_START(name)       menuPage  menuPage##name[]={
#define MENU_PAGE_ITEMS(name,unselectImage,selectImage,str)  {unselectImage,selectImage,str,ARRAY_LEN(menuItem##name),menuItem##name},
#define MENU_PAGE_END()             };

#define MENU_DEFINE(name)   menu  menu##name={(uint32)0,ARRAY_LEN(menuPage##name),menuPage##name};
#define MENU(name)          (menu##name)



#endif
