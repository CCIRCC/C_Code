#include "57stepper_motor.h"

// 硬件配置宏
#define MOTOR_DIR_GPIO_PORT    GPIOB
#define MOTOR_DIR_PINS         GPIO_PIN_13

void _57_Motor_Init(void) {
    // 启动所有PWM通道（假设MX_TIM4_Init已调用）
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}

void _57_Motor_SetDirection(Motor_Direction dir) {
	  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	 _57_Motor_SetSpeed(MOTOR_CH1,60); 
    GPIO_PinState state = (dir == MOTOR_DIR_FORWARD) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(MOTOR_DIR_GPIO_PORT, MOTOR_DIR_PINS, state);
}

void _57_Motor_SetSpeed(Motor_Channel ch, uint8_t duty) {
    uint32_t channel;
    switch(ch) {
        case MOTOR_CH1: channel = TIM_CHANNEL_1; break;
        case MOTOR_CH2: channel = TIM_CHANNEL_2; break;
        case MOTOR_CH3: channel = TIM_CHANNEL_3; break;
        case MOTOR_CH4: channel = TIM_CHANNEL_4; break;
        default: return;
    }
    uint32_t pulse = (duty * (htim4.Instance->ARR + 1)) / 100;
    __HAL_TIM_SET_COMPARE(&htim4, channel, pulse);
		
		
}
// motor_control.c
void _57_Motor_Stop_Channel1(void) {
    /* 方法1：通过设置0%占空比停止 */
    _57_Motor_SetSpeed(MOTOR_CH1, 0);  // 复用已有函数
    
    /* 方法2：直接操作寄存器（更底层） */
     __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0); 
//    
//    /* 可选：关闭方向信号 */
//     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}
