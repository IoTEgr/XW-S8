#ifndef  PROGRESS_BAR_H
    #define  PROGRESS_BAR_H


//#include "max.h"




typedef struct ProgressBar_OP_S
{
	s32_t (*open)(char *name);
	s32_t (*close)(u32_t handle);
	s32_t (*show)(u32_t handle);
    s32_t (*progress)(u32_t handle,s32_t progress);
	s32_t (*ioctrl)(u32_t handle,u32_t op,u32_t para);
	
}ProgressBar_OP_T;




enum
{
	PROGRESS_BAR_TYPE_NULL=0,
	PROGRESS_BAR_TYPE_STYLE1,

	PROGRESS_BAR_TYPE_MAX
};


enum//dialog op-cmd
{
	PROGRESSBAR_OP_NULL=0,
    PROGRESSBAR_OP_SHOW,
    PROGRESSBAR_OP_OPEN,
    PROGRESSBAR_OP_CLOSE,
    PROGRESSBAR_OP_CALLBACK,
    PROGRESSBAR_OP_PROGRESS,
//-----------for dialog ioctrl--------------
    PROGRESSBAR_OP_IOCTRL,
    PROGRESSBAR_OP_DEFAULT,
    PROGRESSBAR_OP_USERINFO,
	PROGRESSBAR_OP_MAX
};

/*******************************************************************************
* Function Name  : progressbarInit
* Description    : progress bar initial                
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarInit(void);
/*******************************************************************************
* Function Name  : progressbarunInit
* Description    : progress bar uninitial                
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarUninit(void);
/*******************************************************************************
* Function Name  : progressbarOpCtrl
* Description    : progress bar operation bar contrl         
* Input          :   u32_t handle : handle
                        u32_t op_cmd: op command
                        u32_t para    : parameter
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarOpCtrl(u32_t handle,u32_t op_cmd,u32_t para);
/*******************************************************************************
* Function Name  : progressbarActiveGet
* Description    : progress bar state get          
* Input          :  
* Output        : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t progressbarActiveGet(void);
/*******************************************************************************
* Function Name  : progressbarRegister
* Description	 : progress bar callback function			   
* Input 		 :	
* Output		: 
* Return		 : s32_t -1 : fail
								   0 : success
*******************************************************************************/
s32_t progressbarRegister(u32_t handle,s32_t (*func)(u32_t handle,s32_t value));










#endif



