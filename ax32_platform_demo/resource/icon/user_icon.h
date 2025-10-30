/****************************************************************************
       ***             ***                      MAXLIB-GRAPHC                  
      ** **           ** **                                                    
     **   **         **   **            THE MAXLIB FOR IMAGE SHOW PROCESS      
    **     **       **     **                                                  
   **       **     **       **              MAX ROURCE ICON MANAGEMENT         
  **         **   **         **                                                
 **           ** **           **              (C) COPYRIGHT 2016 MAX           
**             ***             **                                              
                                                                               
* File Name   : user_icon.c                                                    
* Author      : Mark.Douglas                                                   
* Version     : V0100                                                          
* Date        : 05/25/2016                                                     
* Description : This file for maxlib resource icon managemant                  
*                                                                              
* History     :                                                                
* 2016-05-25  :                                                                
*      <1>.This is created by mark,set version as v0103.                       
*      <2>.Add basic functions.                                                
******************************************************************************/

#ifndef USER_ICON_H 
   #define USER_ICON_H



#define  TRANSPARENT_COLOR    0

enum
{
   R_ID_ICON_MENUAUDIO=R_ID_TYPE_ICON, 
   R_ID_ICON_MENUBATTERY,
   R_ID_ICON_MENUCLOCK,
   R_ID_ICON_MENUDELALL,
   R_ID_ICON_MENUDELONE,
   R_ID_ICON_MENUEV,
   R_ID_ICON_MENUFORMAT,
   R_ID_ICON_MENUGSENSOR,
   R_ID_ICON_MENUHZ,
   R_ID_ICON_MENUIMAGEQ,
   R_ID_ICON_MENULANGUAGE,
   R_ID_ICON_MENULIGHTNORMAL,
   R_ID_ICON_MENULOCK,
   R_ID_ICON_MENULOOPRECORD,
   R_ID_ICON_MENUMONITOR,
   R_ID_ICON_MENUMOON,
   R_ID_ICON_MENUMOREPHOTO,
   R_ID_ICON_MENUMOTION,
   R_ID_ICON_MENUMUSIC,
   R_ID_ICON_MENUMUSICPAUSE,
   R_ID_ICON_MENUMUSICPLAY,
   R_ID_ICON_MENUPOWEROFF,
   R_ID_ICON_MENUPRINTDENSITY,
   R_ID_ICON_MENURESET,
   R_ID_ICON_MENURESOLUTION,
   R_ID_ICON_MENUSCRENNOFF,
   R_ID_ICON_MENUSTRAMP,
   R_ID_ICON_MENUTV,
   R_ID_ICON_MENUUNLOCK,
   R_ID_ICON_MENUVERSION,
   R_ID_ICON_MENUVOLUME,
   R_ID_ICON_MTBACKWARD2,
   R_ID_ICON_MTBACKWARD4,
   R_ID_ICON_MTBATTERY0,
   R_ID_ICON_MTBATTERY1,
   R_ID_ICON_MTBATTERY2,
   R_ID_ICON_MTBATTERY3,
   R_ID_ICON_MTBATTERY4,
   R_ID_ICON_MTBATTERY5,
   R_ID_ICON_MTBATTERY_LOW2,
   R_ID_ICON_MTFORWARD2,
   R_ID_ICON_MTFORWARD4,
   R_ID_ICON_MTGAME,
   R_ID_ICON_MTIROFF,
   R_ID_ICON_MTIRON,
   R_ID_ICON_MTLOCK,
   R_ID_ICON_MTLOOP1,
   R_ID_ICON_MTLOOP10,
   R_ID_ICON_MTLOOP5,
   R_ID_ICON_MTMENU,
   R_ID_ICON_MTMICOFF,
   R_ID_ICON_MTMICON,
   R_ID_ICON_MTMORE,
   R_ID_ICON_MTMOTION,
   R_ID_ICON_MTOFF,
   R_ID_ICON_MTON,
   R_ID_ICON_MTON1,
   R_ID_ICON_MTON2,
   R_ID_ICON_MTPARKOFF,
   R_ID_ICON_MTPARKON,
   R_ID_ICON_MTPAUSE,
   R_ID_ICON_MTPHOTO,
   R_ID_ICON_MTPHOTO3,
   R_ID_ICON_MTPHOTO5,
   R_ID_ICON_MTPHOTOFOCUSRED,
   R_ID_ICON_MTPHOTOFOCUSYELLOW,
   R_ID_ICON_MTPHOTOSKETCH,
   R_ID_ICON_MTPHOTOSKETCHCOLOR,
   R_ID_ICON_MTPHOTOTIME10,
   R_ID_ICON_MTPHOTOTIME3,
   R_ID_ICON_MTPHOTOTIME5,
   R_ID_ICON_MTPHOTOTIME5P3,
   R_ID_ICON_MTPLAY,
   R_ID_ICON_MTPRINTER,
   R_ID_ICON_MTPRTDLY0,
   R_ID_ICON_MTPRTDLY1,
   R_ID_ICON_MTPRTDLY2,
   R_ID_ICON_MTPRTDLY3,
   R_ID_ICON_MTRECORD,
   R_ID_ICON_MTSDCNORMAL,
   R_ID_ICON_MTSDCNULL,
   R_ID_ICON_NUM1,
   R_ID_ICON_NUM10,
   R_ID_ICON_NUM2,
   R_ID_ICON_NUM3,
   R_ID_ICON_NUM4,
   R_ID_ICON_NUM5,
   R_ID_ICON_NUM6,
   R_ID_ICON_NUM7,
   R_ID_ICON_NUM8,
   R_ID_ICON_NUM9,
   R_ID_ICON__MUSIC_1,
   R_ID_ICON__MUSIC_2,
   R_ID_ICON__MUSIC_3,

  R_ICON_MAX
};
extern R_ICON_T User_Icon_Table[];




#endif

