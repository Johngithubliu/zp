//1.通讯数据组
int ID_budrate;//参数波特率关联号
int Comm_Addr;//参数通讯地址关联号
//2.置零数据组
int Initial_zero_range;//参数开机置零范围
int Munal_zero_range;//参数手动置零范围
int Trace_zero_range;//参数零跟踪范围
//3.延时数据组
int Delay_tare;//参数去皮延时输出
int Delay_Mid_feed;//参数中加延时
int Delay_Pre_process;//参数提前延时
int Delay_Next_loop;//参数下一循环延时
int Time_supvisor_Slow_feed_up_limt;//参数慢加监视上限时间
int Time_supvisor_Slow_feed_low_limt;//参数慢加监视下限时间
int Pulse_howmany_package;//参数多少袋输出脉冲
int Tare_howmany_package;//参数多少袋去皮一次
int Recive_x_signals;//参数收到x次信号设定
int no_recive_x_signals;//参数收不到x次信号设定限
int Even_howmany_package_Pre_process;//参数多少袋平均修正一次提前量
int Delay_slow_feed;//参数慢加延时
int Delay_initial_feed;//参数初加延时

//4.控制数据组   
int Slow_feed_modify_uper_limt;//参数慢加修正上限
int Slow_feed_modify_down_limt;//参数慢加修正下限
int Pre_feed_modify_uper_limt;//参数提前量修正上限
int Pre_feed_modify_down_limt;//参数提前量修正下限
int Slow_feed;//参数慢加量
int Pre_feed;//参数提前量
int Mid_feed;//参数中加值
int Dlet_error;//参数允差
int Target;//参数定值
int Target_uper_limt;//参数定量上限
int Target_down_limt;//参数定量下限
int zero_range;//参数零区
int Slow_feed_modify;//参数慢加修正量
int Initial_power_value;//参数初上电值
int Initial_feed_value;//参数初加量
int Random_setting_value;//参数随机设定值

//5.修正数据组
int ID_user_cal ;//参数用户标定关联号
int ID_slow_feed_modify ;//参数慢加修正标记关联号
int ID_pre_feed_modify;//参数提前修正标记关联号
int ID_run_mode;//参数运行模式关联号



//6.杂项数据组
int Clear_merory_code;//内存清理密码
int Target_pulse;//参数定量的总脉冲数
int ID_weight_unit;//参数重量单位设定关联号
int Num_shift;//参数班次
//7.int ;//标定数据组
int Cal_code;//标定密码
int Cal_point;//标定小数点
int Cal_ID_division;//标定分度数关联号
int Cal_full_Mig;//标定满量程值
float Cal_user_weight;//标定用户砝码重量
int Cal_zero;//标定零位
int Cal_ratio;//标定标率
float Cal_weight;	//标定砝码重量

//8.滤波数据组：
int Range_Filter0;//参数滤波范围0
int Range_Filter1;//参数滤波范围1
int Range_Filter2;//参数滤波范围2
int Range_Filter3;//参数滤波范围3
int Range_Filter4;//参数滤波范围4
int Strong_Filter0;//参数滤波强度0
int Strong_Filter1;//参数滤波强度1
int Strong_Filter2;//参数滤波强度2
int Strong_Filter3;//参数滤波强度3
int Strong_Filter4;//参数滤波强度4
//称重_吨位，袋数，平均值
int Weighting_T,Weighting_package,Weighting_even;

int Experiment_write_para;

int Analog_mode_SEL;// 模拟量选择

union Modbus_packed_3    
{    
long Cal_zero; 
unsigned char zero[4];   
}Modbus_Cal_zero;

union Modbus_packed_4  
{    
long Cal_ratio; 
unsigned char ratio[4];   
}Modbus_Cal_ratio;

union Modbus_packed_5  
{    
long Cal_user_weight; 
unsigned char user_weight[4];   
}Modbus_Cal_user_weight;

union Modbus_packed_6  
{    
long Cal_weight; 
unsigned char add_weight[4];   
}Modbus_Cal_weight;

//1.通讯数据组 在昆仑通泰中地址(一个地址16位)
#define comm_bud_rate 1508-1// 1008-1  //参数波特率关联号
#define comm_address comm_bud_rate+1 //参数通讯地址关联号
//2.置零数据组
#define Initial_zero_range_address comm_address+1  //参数开机置零范围
#define Munal_zero_range_address Initial_zero_range_address+1  //参数手动置零范围
#define Trace_zero_range_address Munal_zero_range_address+1  //参数零跟踪范围
//3.延时数据组
#define  Delay_tare_address  Trace_zero_range_address+1//参数去皮延时输出
#define  Delay_Mid_feed_address  Delay_tare_address+1//参数中加延时
#define  Delay_Pre_process_address  Delay_Mid_feed_address+1//参数提前延时
#define  Delay_Next_loop_address  Delay_Pre_process_address+1//参数下一循环延时
#define  Time_supvisor_Slow_feed_up_limt_address  Delay_Next_loop_address+1//参数慢加监视上限时间
#define  Time_supvisor_Slow_feed_low_limt_address  Time_supvisor_Slow_feed_up_limt_address+1//参数慢加监视下限时间
#define  Pulse_howmany_package_address  Time_supvisor_Slow_feed_low_limt_address+1//参数多少袋输出脉冲
#define  Tare_howmany_package_address  Pulse_howmany_package_address+1//参数多少袋去皮一次
#define  Recive_x_signals_address  Tare_howmany_package_address+1//参数收到x次信号设定
#define  no_recive_x_signals_address  Recive_x_signals_address+1//参数收不到x次信号设定限
#define  Even_howmany_package_Pre_process_address  no_recive_x_signals_address+1//参数多少袋平均修正一次提前量
#define  Delay_slow_feed_address  Even_howmany_package_Pre_process_address+1//参数慢加延时
#define  Delay_initial_feed_address  Delay_slow_feed_address+1//参数初加延时

//4.控制数据组   
#define   Slow_feed_modify_uper_limt_address  Delay_initial_feed_address +1//  参数慢加修正上限
#define   Slow_feed_modify_down_limt_address   Slow_feed_modify_uper_limt_address +1//  参数慢加修正下限
#define   Pre_feed_modify_uper_limt_address   Slow_feed_modify_down_limt_address +1//  参数提前量修正上限
#define   Pre_feed_modify_down_limt_address   Pre_feed_modify_uper_limt_address +1//  参数提前量修正下限
#define   Slow_feed_address   Pre_feed_modify_down_limt_address +1//  参数慢加量
#define   Pre_feed_address   Slow_feed_address +1//  参数提前量
#define   Mid_feed_address   Pre_feed_address +1//  参数中加值
#define   Dlet_error_address   Mid_feed_address +1//  参数允差

 //*2017-10-28am*
#define   Target_uper_limt_address   Dlet_error_address +1  //  参数定量上限
#define   Target_down_limt_address   Target_uper_limt_address +1//  参数定量下限
#define   Target_address             Target_down_limt_address+1//  参数定值
#define   zero_range_address         Target_address +1//  参数零区
 //*2017-10-28am*

#define   Slow_feed_modify_address   zero_range_address +1//  参数慢加修正量

#define   Initial_power_value_address   Slow_feed_modify_address +1//  参数初上电值
#define   Initial_feed_value_address   Initial_power_value_address +1//  参数初加量
#define   Random_setting_value_address   Initial_feed_value_address +1//  参数随机设定值



//5.修正数据组
#define   ID_user_cal_address   Random_setting_value_address +1//  参数用户标定关联号
#define   ID_slow_feed_modify_address   ID_user_cal_address +1//  参数慢加修正标记关联号
#define   ID_pre_feed_modify_address   ID_slow_feed_modify_address +1//  参数提前修正标记关联号
#define   ID_run_mode_address   ID_pre_feed_modify_address +1//  参数运行模式关联号

//7.标定数据组
#define Cal_ID_division_address ID_run_mode_address+1//标定分度数关联号
#define Cal_user_weight_address Cal_ID_division_address+1//标定用户砝码重量
#define Cal_weight_address Cal_user_weight_address+2//标定砝码重量

//8.滤波数据组：
#define Range_Filter0_address Cal_weight_address+2    //参数滤波范围0
#define Range_Filter1_address Range_Filter0_address+1//参数滤波范围1
#define Range_Filter2_address Range_Filter1_address+1//参数滤波范围2
#define Range_Filter3_address Range_Filter2_address+1//参数滤波范围3
#define Range_Filter4_address Range_Filter3_address+1//参数滤波范围4

#define Strong_Filter0_address Range_Filter4_address+1//参数滤波强度0
#define Strong_Filter1_address Strong_Filter0_address+1//参数滤波强度1
#define Strong_Filter2_address Strong_Filter1_address+1//参数滤波强度2
#define Strong_Filter3_address Strong_Filter2_address+1//参数滤波强度3
#define Strong_Filter4_address Strong_Filter3_address+1//参数滤波强度4

#define Cal_full_Mig_address  Strong_Filter4_address+1// 1561-1//满量程值
#define Cal_pint_address  Cal_full_Mig_address+1// 1562//标定小数点
#define Analog_mode_SEL_address    Cal_pint_address+1//模拟量输出选择


extern char  X_input_state[50];
extern char  Y_output_state[50];	   //bool
/***output rename**/
#define Tare                 Y_output_state[0]=1
#define Pass                 Y_output_state[1]=1
#define Over                 Y_output_state[2]=1
#define Mid_fed              Y_output_state[3]=1
#define Slow_fed             Y_output_state[4]=1
#define Pre_fed              Y_output_state[5]=1
#define Start                Y_output_state[6]=1
#define Stop                 Y_output_state[7]=1
#define N_package_output     Y_output_state[8]=1
#define Material_door_output  Y_output_state[9]=1
//#define Initial_power_output Y_output_state[10]=1
#define LOW_ERR              Y_output_state[10]=1
#define Random_output        Y_output_state[11]=1
#define Cal_output           Y_output_state[12]=1
#define Initial_output       Y_output_state[13]=1
#define Insert_board_output  Y_output_state[14]=1
#define Clamp_output         Y_output_state[15]=1


#define Un_Tare                 Y_output_state[0]=0
#define Un_Pass                 Y_output_state[1]=0
#define Un_Over                 Y_output_state[2]=0
#define Un_Mid_fed              Y_output_state[3]=0
#define Un_Slow_fed             Y_output_state[4]=0
#define Un_Pre_fed              Y_output_state[5]=0
#define Un_Start                Y_output_state[6]=0
#define Un_Stop                 Y_output_state[7]=0
#define Un_N_package_output     Y_output_state[8]=0
//#define Un_Zero_zone_output     Y_output_state[9]=0
#define Un_Material_door_output     Y_output_state[9]=0
//#define Un_Initial_power_output Y_output_state[10]=0
#define Un_LOW_ERR              Y_output_state[10]=0
#define Un_Random_output        Y_output_state[11]=0
#define Un_Cal_output           Y_output_state[12]=0
#define Un_Initial_output       Y_output_state[13]=0
#define Un_Insert_board_output  Y_output_state[14]=0
#define Un_Clamp_output         Y_output_state[15]=0


//定义输入口
#define Input_start            X_input_state[0]
#define Input_stop             X_input_state[1]
#define Input_zero             X_input_state[2]
#define Input_tare             X_input_state[3]
#define Input_setting_para     X_input_state[4]
#define Input_calibration      X_input_state[5]
#define Input_clear_number     X_input_state[6]
#define Input_count_enable     X_input_state[7]
#define Input_clamp_bag        X_input_state[8]
#define Input_comm_calibration X_input_state[9]

#define Input_material_door_open X_input_state[10]
#define Input_machine_Pasue      X_input_state[11]

#define Input_calibration_Confirm X_input_state[12]


#define Input_material_door     X_input_state[7]











