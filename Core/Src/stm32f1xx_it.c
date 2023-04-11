/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "characters.h"
#include "turnMethods.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
extern uint8_t rxBuffer[20];
extern int getInstruction;
extern UART_HandleTypeDef huart1;
extern unsigned char info[1024];
extern struct character A;
extern struct character B;
extern struct character Enemy;
extern round;

struct character *thisTurnPlayer = &A;
//struct character *thisTurnEnemy = &Enemy;
struct character *thisTurnEnemy = &Enemy;

unsigned char uRx_Data[1024] = {0};
unsigned char uLength = 0;

static int damageValue;
extern struct skill A_skill[4];
extern struct skill B_skill[4];
extern struct item item[4];
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void changeCharacter();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  HAL_UART_Receive_IT(&huart1, (uint8_t* )rxBuffer,1);
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1){
//		uRx_Data = {0};
//		uLength = 0;
		if(rxBuffer[0] == '\n'){
			HAL_UART_Transmit(&huart1, uRx_Data, uLength, 0xffff);

			if (getInstruction == -1){
				if (uLength == 2) {
					if (uRx_Data[0] == '0')
						getInstruction = 0;
					else if (uRx_Data[0] == '1')
						getInstruction = 1;
					else if (uRx_Data[0] == '2')
						getInstruction = 2;
					else if (uRx_Data[0] == '3')
						getInstruction = 3;
				}
			}else if (getInstruction == 0){
				if (uLength == 2) {
					struct skill *Sp;
					if (thisTurnPlayer == &A){
						if (uRx_Data[0] == '0'){
							Sp = &A_skill[0];
						}else if (uRx_Data[0] == '1'){
							Sp = &A_skill[1];
						}else if (uRx_Data[0] == '2'){
							Sp = &A_skill[2];
						}else if (uRx_Data[0] == '3'){
							Sp = &A_skill[3];
						}
					}

					if (thisTurnPlayer == &B){
						if (uRx_Data[0] == '0'){
							Sp = &B_skill[0];
						}else if (uRx_Data[0] == '1'){
							Sp = &B_skill[1];
						}else if (uRx_Data[0] == '2'){
							Sp = &B_skill[2];
						}else if (uRx_Data[0] == '3'){
							Sp = &B_skill[3];
						}
					}

					if (judgeMP(thisTurnPlayer, Sp)){
						switch (Sp->type) {
							case 0:
								//攻击�?�?
								damageValue = attackSkill(thisTurnPlayer, Sp);//将该值用通信传出
								beAttacked(thisTurnEnemy, damageValue);
								normailzeATKValue(thisTurnPlayer);
								break;
							case 1:
								//提升我方防御�?
								upDEF(thisTurnPlayer, Sp);
								break;
							case 2:
								//提升我方攻击�?
								upATK(thisTurnPlayer, Sp);
								break;
							case 3:
								//降低对方攻击�?
								downATK(thisTurnPlayer, thisTurnEnemy, Sp);
								break;
							case 4:
								//降低对方防御�?
								downDEF(thisTurnPlayer, thisTurnEnemy, Sp);
								break;
							case 5:
								//治疗�?�?
								healSkill(thisTurnPlayer, Sp);
								break;
							default:
								break;
						}
						printCharaterState();
						getInstruction = -1;
						round++;
					}else {
						sprintf(info,"MP less than needs");
						HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
					}
				}
			}else if (getInstruction == 1){
				//使用道具
				if (uLength == 2) {
					struct item *Ip;
					if (uRx_Data[0] == '0'){
						Ip = &item[0];
					}else if (uRx_Data[0] == '1'){
						Ip = &item[1];
					}else if (uRx_Data[0] == '2'){
						Ip = &item[2];
					}else if (uRx_Data[0] == '3'){
						Ip = &item[3];
					}

					if (judgeCount(Ip)){
						useItem(thisTurnPlayer, Ip);

						printCharaterState();
						getInstruction = -1;
						round++;
					}else {
						sprintf(info,"this item was ran out!");
						HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
					}
				}

			}else if (getInstruction == 2){
				//切换角色
//				if(thisTurnPlayer==&A){
//					thisTurnPlayer=&B;
//				}else{
//					thisTurnPlayer=&A;
//				}


			}else if (getInstruction == 3){
				//逃跑然后�?,通信相关

			}else if (getInstruction == 4){
				//我方被攻�?
				damageValue = 5;//通过通信获得
				beAttacked(thisTurnPlayer, damageValue);
				normailzeDEFValue(thisTurnPlayer);
				if (thisTurnPlayer->HP < 0){
					//HP小于0,自动换人
					getInstruction = 2;
				}else{
					getInstruction = -1;
				}
			}

			if (uLength == 3 && uRx_Data[0] == '-' && uRx_Data[1] == '1')
				getInstruction = -1;



			uLength = 0;
		}else{
			uRx_Data[uLength] = rxBuffer[0];
			uLength++;
		}
	}
}
void changeCharacter(){
	if ((*thisTurnPlayer).HP == A.HP) {
		if (B.HP > 0){
			thisTurnPlayer = &B;
			getInstruction = -1;
		}else if (A.HP < 0){
			//输输输
		}
	}else {
		if (A.HP > 0){
			thisTurnPlayer = &A;
			getInstruction = -1;
		}else if (B.HP < 0){
			//输输输
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	switch (getInstruction) {
		case -1:
			sprintf(info,"Pleas input instruction!\r\nInput 0 to use skill, input 1 to use item.\r\nInput 2 to change character, input 3 to escape.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			break;
		case 0:
			sprintf(info,"Please input the number of skills.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			if (thisTurnPlayer == &A)
				for (int var = 0; var < 4; var++) {
					sprintf(info,"value: %d	cost: %d		", A_skill[var].value, A_skill[var].cost);
					HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart1,(uint8_t*)A_skill[var].name, A_skill[var].nameLength, HAL_MAX_DELAY);
					sprintf(info,"\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
				}

			if (thisTurnPlayer == &B)
				for (int var = 0; var < 4; var++) {
					sprintf(info,"value: %d	cost: %d		", B_skill[var].value, B_skill[var].cost);
					HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
					HAL_UART_Transmit(&huart1,(uint8_t*)B_skill[var].name, B_skill[var].nameLength, HAL_MAX_DELAY);
					sprintf(info,"\r\n");
					HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
				}

			break;
		case 1:
			sprintf(info,"Please input the number of items.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			changeCharacter();
			for (int var = 0; var < 4; var++) {
				sprintf(info,"value: %d	count: %d		", item[var].value, item[var].count);
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart1,(uint8_t*)item[var].name, item[var].nameLength, HAL_MAX_DELAY);
				sprintf(info,"\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			}
			break;
		case 2:
			sprintf(info,"Now change character.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);

			if(thisTurnPlayer == &A){
				sprintf(info,"Player A\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
				sprintf(info,"HP: %d	MP: %d    ATK: %d    DEF: %d\r\n",A.HP,A.MP,A.ATK,A.DEF);
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			}else{
				sprintf(info,"Player B\r\n");
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
				sprintf(info,"HP: %d	MP: %d    ATK: %d    DEF: %d\r\n",B.HP,B.MP,B.ATK,B.DEF);
				HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			}

			break;
		case 3:
			sprintf(info,"Now escape and loss the game.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			break;
		case 5:
			sprintf(info,"Now be attacked.\r\n");
			HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
			break;
		default:
			break;
		}
}

void printCharaterState(){
	sprintf(info,"Player A\r\n");
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
	sprintf(info,"HP: %d	MP: %d    ATK: %d    DEF: %d\r\n",A.HP,A.MP,A.ATK,A.DEF);
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
	sprintf(info,"Player B\r\n");
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
	sprintf(info,"HP: %d	MP: %d    ATK: %d    DEF: %d\r\n",B.HP,B.MP,B.ATK,B.DEF);
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
	sprintf(info,"Enemy \r\n");
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
	sprintf(info,"HP: %d	MP: %d    ATK: %d    DEF: %d\r\n",Enemy.HP,Enemy.MP,Enemy.ATK,Enemy.DEF);
	HAL_UART_Transmit(&huart1,(uint8_t*)info, strlen(info), HAL_MAX_DELAY);
}

void blinkRed(){
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

void blinkGreen(){
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	HAL_Delay(1000);
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

/* USER CODE END 1 */
