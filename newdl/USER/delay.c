
/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/

#include "delay.h"

void Delay(__IO u32 nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
} 
