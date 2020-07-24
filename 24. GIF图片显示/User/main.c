/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 固件库实验
  *********************************************************************
  * @attention
  *
  * 实验平台:野火 STM32全系列开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* 开发板硬件bsp头文件 */
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./key/bsp_key.h"
#include "./lcd/bsp_ili9806g_lcd.h"
//#include "./flash/bsp_spi_flash.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./sram/bsp_sram.h"	  
#include "./touch/gt5xx.h"
#include "ff.h"
#include "./USBAPP/usbh_bsp.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"
#include "ScreenShot.h"


/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
/* Touch任务句柄 */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI任务句柄 */
static TaskHandle_t GUI_Task_Handle = NULL;


/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
SemaphoreHandle_t ScreenShotSem_Handle = NULL;
SemaphoreHandle_t FatFs_Ready_Handle = NULL;

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
FATFS   fs;								/* FatFs文件系统对象 */
FIL     file;							/* file objects */
UINT    bw;            		/* File R/W count */
FRESULT result; 
FILINFO fno;
DIR dir;
#if 1
PARTITION VolToPart[]=
{
	{0,1},/* "0:" */
	{0,2},/* "1:" */
	{0,3}	/* "2:" */
};
int USB_states = 1;
#endif
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void* parameter);/* LED_Task任务实现 */
static void GUI_Task(void* parameter);/* GUI_Task任务实现 */
static void Touch_Task(void* parameter);
static void BSP_Init(void);/* 用于初始化板载相关资源 */
static void ESP_PDN_INIT(void);
/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  /* 开发板硬件初始化 */
  BSP_Init();
  
  printf("\r\n ********** emwin DEMO *********** \r\n");
  
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	taskENTER_CRITICAL();//进入临界区
	
	/* 创建ScreenShotSem信号量 */
	ScreenShotSem_Handle = xSemaphoreCreateBinary();
	FatFs_Ready_Handle = xSemaphoreCreateBinary();
	if(NULL != ScreenShotSem_Handle)
		printf("ScreenShotSem二值信号量创建成功！\r\n");
	if(NULL != FatFs_Ready_Handle)
		printf("FatFs_Ready_Handle二值信号量创建成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* 任务入口函数 */
											 (const char*    )"LED_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )4,         /* 任务的优先级 */
											 (TaskHandle_t   )&LED_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LED1_Task任务成功！\r\n");
  
  xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* 任务入口函数 */
											 (const char*      )"Touch_Task",/* 任务名称 */
											 (uint16_t         )256,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )3,       /* 任务的优先级 */
											 (TaskHandle_t     )&Touch_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Touch_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* 任务入口函数 */
											 (const char*      )"GUI_Task",/* 任务名称 */
											 (uint16_t         )4096,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )2,         /* 任务的优先级 */
											 (TaskHandle_t     )&GUI_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GUI_Task任务成功！\r\n");
	
	vTaskDelete(AppTaskCreate_Handle);//删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief LED任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void LED_Task(void* parameter)
{
	while(1)
	{
		LED3_TOGGLE;
		vTaskDelay(1000);
	}
}

/**
  * @brief 触摸检测任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void Touch_Task(void* parameter)
{
  /* 等待信号量 */
	xSemaphoreTake(ScreenShotSem_Handle,/* 二值信号量句柄 */
								 portMAX_DELAY);/* 阻塞等待 */  
	printf("请将存放有图片资源的U盘插入开发板中，并等待U盘就绪。\r\n");

	USBH_Init(&USB_OTG_Core,
		USB_OTG_FS_CORE_ID,
		&USB_Host,
		&USBH_MSC_cb,
		&USR_cb);
	while(1)
	{
		 /* USB状态检测处理函数，在检测设备插入时使用 */
		USBH_Process(&USB_OTG_Core, &USB_Host);
		
		GTP_TouchProcess();//触摸屏定时扫描
		vTaskDelay(50);
//		printf("%ld\r\n", GUI_ALLOC_GetNumUsedBytes());

	}
}

/**
  * @brief GUI任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void GUI_Task(void* parameter)
{
  
	/* 初始化STemWin */
  GUI_Init();
  /* 触摸屏初始化 */
  GTP_Init_Panel();
  /* 给出信号量 */
  xSemaphoreGive(ScreenShotSem_Handle);
  /* 开LCD背光灯 */
  ILI9806G_BackLed_Control ( ENABLE );
	while(1)
	{
		MainTask();
	}
}

/**
  * @brief 板级外设初始化
  * @note 所有板子上的初始化均可放在这个函数里面
  * @param 无
  * @retval 无
  */
static void BSP_Init(void)
{
#if 0
  /* SRAM初始化 */
  FSMC_SRAM_Init();
#endif
  /* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
  
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();
  
	/* 串口初始化	*/
	Debug_USART_Config();
	/* 禁用WIFI */
  ESP_PDN_INIT();
	

  /* 挂载文件系统，挂载时会对SD卡初始化 */
#if 0
  result = f_mount(&fs,"0:",1);
	if(result != FR_OK)
	{
		printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\n");
		while(1);
	}
#endif
}
/**
  * @brief AP6181_PDN_INIT
  * @note 禁止WIFI模块
  * @param 无
  * @retval 无
  */
static void ESP_PDN_INIT(void)
{
  /*定义一个GPIO_InitTypeDef类型的结构体*/
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOE, ENABLE); 							   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
  
  GPIO_ResetBits(GPIOE,GPIO_Pin_2);  //禁用WiFi模块
}

/********************************END OF FILE****************************/
