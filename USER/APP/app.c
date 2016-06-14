#include <includes.h>


static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

#define  LED1_TASK_PRIO                        3
#define  LED1_TASK_STK_SIZE                    128
static	OS_TCB		LED1TaskTCB;
static  CPU_STK  LED1TaskStk[LED1_TASK_STK_SIZE];

#define  LED2_TASK_PRIO                        4
#define  LED2_TASK_STK_SIZE                    128
static	OS_TCB		LED2TaskTCB;
static  CPU_STK  LED2TaskStk[LED2_TASK_STK_SIZE];

static  void  AppTaskStart  (void *p_arg);

static  void  Led1Task  (void *p_arg);
static  void  Led2Task  (void *p_arg);
//static  void  Led3Task  (void *p_arg);

void LED1_Init(void);
void LED2_Init(void);

#define LED1(n) GPIO_WriteBit(GPIOB,GPIO_Pin_0,(BitAction)n)
#define LED2(n) GPIO_WriteBit(GPIOF,GPIO_Pin_7,(BitAction)n)

int  main (void)
{
    OS_ERR  err;

    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
		u8 val=0;

   (void)p_arg;
                                               /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();
	
	LED1_Init();
//	while(DEF_TRUE)
//	{
//		val=!val;
//		LED1(val);
//		OSTimeDlyHMSM(0, 0, 0, 500,
//									OS_OPT_TIME_HMSM_STRICT,
//									&err);
//	}

//		OSTimeDlyHMSM(0, 0, 0, 100,
//									OS_OPT_TIME_HMSM_STRICT,
//									&err);

//	LED1_Init();
//	LED2_Init();

		OSTaskCreate((OS_TCB     *)&LED1TaskTCB,                /* Create the LED1 task                                */
                 (CPU_CHAR   *)"LED1 Task",
                 (OS_TASK_PTR ) Led1Task,
                 (void       *) 0,
                 (OS_PRIO     ) LED1_TASK_PRIO,
                 (CPU_STK    *)&LED1TaskStk[0],
                 (CPU_STK_SIZE) LED1_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) LED1_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
								 
		OSTaskCreate((OS_TCB     *)&LED2TaskTCB,                /* Create the LED2 task                                */
                 (CPU_CHAR   *)"LED2 Task",
                 (OS_TASK_PTR ) Led2Task,
                 (void       *) 0,
                 (OS_PRIO     ) LED2_TASK_PRIO,
                 (CPU_STK    *)&LED2TaskStk[0],
                 (CPU_STK_SIZE) LED2_TASK_STK_SIZE / 10,
                 (CPU_STK_SIZE) LED2_TASK_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);


		
		OSTaskDel(&AppTaskStartTCB,&err);
}

static  void  Led1Task  (void *p_arg)
{
	OS_ERR err;
	
	u8 val=0;
	
	(void)p_arg;
	
	while(DEF_TRUE)
	{
		val=!val;
		LED1(val);
		OSTimeDlyHMSM(0, 0, 0, 500,
									OS_OPT_TIME_HMSM_STRICT,
									&err);
	}
}

static  void  Led2Task  (void *p_arg)
{
	OS_ERR err;
	
	u8 val=0;
	
	(void)p_arg;
	
	
	while(DEF_TRUE)
	{
		val=!val;
		LED2(val);
		OSTimeDlyHMSM(0, 0, 0, 500,
									OS_OPT_TIME_HMSM_STRICT,
									&err);
	}
}

//LED1--PB0,LED1--PF7,LED3--PF8
void LED1_Init(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);
		
		/*选择要控制的GPIOF引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;

		/*调用库函数，初始化GPIOF7*/
		GPIO_Init(GPIOF, &GPIO_InitStructure);			  

		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
		
		/* 关闭所有led灯	*/
		GPIO_SetBits(GPIOF, GPIO_Pin_7|GPIO_Pin_8);	 
}

void LED2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
}
