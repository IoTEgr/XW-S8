#ifndef AX32XX_PMU_H
#define AX32XX_PMU_H

#define VDD_HW_IE           13
#define VDD_LW_IE           11
#define VDD_PROT_IE         9
#define BAT_HW_IE           7
#define BAT_LW_IE           5
#define BAT_PROT_IE         3
#define PMU_SMP_POWER       0
//ADC_channel
#define VBG                 0
#define VMID                1
#define VPAD1               2
#define VPAD2               3

/*******************************************************************************
* Function Name  : pmu_test
* Description	 : hal layer get rtc time
* Input 		 : u32 baudrate
* Output		 : None
* Return		 : 
*******************************************************************************/
void pmu_test(u32 baudrate);
/*******************************************************************************
* Function Name  : pmu_manual
* Description	 : hal layer get rtc time
* Input 		 : u32 baudrate,u32 ch
* Output		 : None
* Return		 : 
*******************************************************************************/
u32 pmu_manual(u32 baudrate,u32 ch);  
/*******************************************************************************
* Function Name  : checkbat_first
* Description    : debg BATTERY
* Input          : None
* Output         : None
* Return         : none
*******************************************************************************/
u16 checkbat_first(void);
#endif
