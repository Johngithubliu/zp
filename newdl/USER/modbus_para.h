//1.ͨѶ������
int ID_budrate;//���������ʹ�����
int Comm_Addr;//����ͨѶ��ַ������
//2.����������
int Initial_zero_range;//�����������㷶Χ
int Munal_zero_range;//�����ֶ����㷶Χ
int Trace_zero_range;//��������ٷ�Χ
//3.��ʱ������
int Delay_tare;//����ȥƤ��ʱ���
int Delay_Mid_feed;//�����м���ʱ
int Delay_Pre_process;//������ǰ��ʱ
int Delay_Next_loop;//������һѭ����ʱ
int Time_supvisor_Slow_feed_up_limt;//�������Ӽ�������ʱ��
int Time_supvisor_Slow_feed_low_limt;//�������Ӽ�������ʱ��
int Pulse_howmany_package;//�������ٴ��������
int Tare_howmany_package;//�������ٴ�ȥƤһ��
int Recive_x_signals;//�����յ�x���ź��趨
int no_recive_x_signals;//�����ղ���x���ź��趨��
int Even_howmany_package_Pre_process;//�������ٴ�ƽ������һ����ǰ��
int Delay_slow_feed;//����������ʱ
int Delay_initial_feed;//����������ʱ

//4.����������   
int Slow_feed_modify_uper_limt;//����������������
int Slow_feed_modify_down_limt;//����������������
int Pre_feed_modify_uper_limt;//������ǰ����������
int Pre_feed_modify_down_limt;//������ǰ����������
int Slow_feed;//����������
int Pre_feed;//������ǰ��
int Mid_feed;//�����м�ֵ
int Dlet_error;//�����ʲ�
int Target;//������ֵ
int Target_uper_limt;//������������
int Target_down_limt;//������������
int zero_range;//��������
int Slow_feed_modify;//��������������
int Initial_power_value;//�������ϵ�ֵ
int Initial_feed_value;//����������
int Random_setting_value;//��������趨ֵ

//5.����������
int ID_user_cal ;//�����û��궨������
int ID_slow_feed_modify ;//��������������ǹ�����
int ID_pre_feed_modify;//������ǰ������ǹ�����
int ID_run_mode;//��������ģʽ������



//6.����������
int Clear_merory_code;//�ڴ���������
int Target_pulse;//������������������
int ID_weight_unit;//����������λ�趨������
int Num_shift;//�������
//7.int ;//�궨������
int Cal_code;//�궨����
int Cal_point;//�궨С����
int Cal_ID_division;//�궨�ֶ���������
int Cal_full_Mig;//�궨������ֵ
float Cal_user_weight;//�궨�û���������
int Cal_zero;//�궨��λ
int Cal_ratio;//�궨����
float Cal_weight;	//�궨��������

//8.�˲������飺
int Range_Filter0;//�����˲���Χ0
int Range_Filter1;//�����˲���Χ1
int Range_Filter2;//�����˲���Χ2
int Range_Filter3;//�����˲���Χ3
int Range_Filter4;//�����˲���Χ4
int Strong_Filter0;//�����˲�ǿ��0
int Strong_Filter1;//�����˲�ǿ��1
int Strong_Filter2;//�����˲�ǿ��2
int Strong_Filter3;//�����˲�ǿ��3
int Strong_Filter4;//�����˲�ǿ��4
//����_��λ��������ƽ��ֵ
int Weighting_T,Weighting_package,Weighting_even;

int Experiment_write_para;

int Analog_mode_SEL;// ģ����ѡ��

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

//1.ͨѶ������ ������̩ͨ�е�ַ(һ����ַ16λ)
#define comm_bud_rate 1508-1// 1008-1  //���������ʹ�����
#define comm_address comm_bud_rate+1 //����ͨѶ��ַ������
//2.����������
#define Initial_zero_range_address comm_address+1  //�����������㷶Χ
#define Munal_zero_range_address Initial_zero_range_address+1  //�����ֶ����㷶Χ
#define Trace_zero_range_address Munal_zero_range_address+1  //��������ٷ�Χ
//3.��ʱ������
#define  Delay_tare_address  Trace_zero_range_address+1//����ȥƤ��ʱ���
#define  Delay_Mid_feed_address  Delay_tare_address+1//�����м���ʱ
#define  Delay_Pre_process_address  Delay_Mid_feed_address+1//������ǰ��ʱ
#define  Delay_Next_loop_address  Delay_Pre_process_address+1//������һѭ����ʱ
#define  Time_supvisor_Slow_feed_up_limt_address  Delay_Next_loop_address+1//�������Ӽ�������ʱ��
#define  Time_supvisor_Slow_feed_low_limt_address  Time_supvisor_Slow_feed_up_limt_address+1//�������Ӽ�������ʱ��
#define  Pulse_howmany_package_address  Time_supvisor_Slow_feed_low_limt_address+1//�������ٴ��������
#define  Tare_howmany_package_address  Pulse_howmany_package_address+1//�������ٴ�ȥƤһ��
#define  Recive_x_signals_address  Tare_howmany_package_address+1//�����յ�x���ź��趨
#define  no_recive_x_signals_address  Recive_x_signals_address+1//�����ղ���x���ź��趨��
#define  Even_howmany_package_Pre_process_address  no_recive_x_signals_address+1//�������ٴ�ƽ������һ����ǰ��
#define  Delay_slow_feed_address  Even_howmany_package_Pre_process_address+1//����������ʱ
#define  Delay_initial_feed_address  Delay_slow_feed_address+1//����������ʱ

//4.����������   
#define   Slow_feed_modify_uper_limt_address  Delay_initial_feed_address +1//  ����������������
#define   Slow_feed_modify_down_limt_address   Slow_feed_modify_uper_limt_address +1//  ����������������
#define   Pre_feed_modify_uper_limt_address   Slow_feed_modify_down_limt_address +1//  ������ǰ����������
#define   Pre_feed_modify_down_limt_address   Pre_feed_modify_uper_limt_address +1//  ������ǰ����������
#define   Slow_feed_address   Pre_feed_modify_down_limt_address +1//  ����������
#define   Pre_feed_address   Slow_feed_address +1//  ������ǰ��
#define   Mid_feed_address   Pre_feed_address +1//  �����м�ֵ
#define   Dlet_error_address   Mid_feed_address +1//  �����ʲ�

 //*2017-10-28am*
#define   Target_uper_limt_address   Dlet_error_address +1  //  ������������
#define   Target_down_limt_address   Target_uper_limt_address +1//  ������������
#define   Target_address             Target_down_limt_address+1//  ������ֵ
#define   zero_range_address         Target_address +1//  ��������
 //*2017-10-28am*

#define   Slow_feed_modify_address   zero_range_address +1//  ��������������

#define   Initial_power_value_address   Slow_feed_modify_address +1//  �������ϵ�ֵ
#define   Initial_feed_value_address   Initial_power_value_address +1//  ����������
#define   Random_setting_value_address   Initial_feed_value_address +1//  ��������趨ֵ



//5.����������
#define   ID_user_cal_address   Random_setting_value_address +1//  �����û��궨������
#define   ID_slow_feed_modify_address   ID_user_cal_address +1//  ��������������ǹ�����
#define   ID_pre_feed_modify_address   ID_slow_feed_modify_address +1//  ������ǰ������ǹ�����
#define   ID_run_mode_address   ID_pre_feed_modify_address +1//  ��������ģʽ������

//7.�궨������
#define Cal_ID_division_address ID_run_mode_address+1//�궨�ֶ���������
#define Cal_user_weight_address Cal_ID_division_address+1//�궨�û���������
#define Cal_weight_address Cal_user_weight_address+2//�궨��������

//8.�˲������飺
#define Range_Filter0_address Cal_weight_address+2    //�����˲���Χ0
#define Range_Filter1_address Range_Filter0_address+1//�����˲���Χ1
#define Range_Filter2_address Range_Filter1_address+1//�����˲���Χ2
#define Range_Filter3_address Range_Filter2_address+1//�����˲���Χ3
#define Range_Filter4_address Range_Filter3_address+1//�����˲���Χ4

#define Strong_Filter0_address Range_Filter4_address+1//�����˲�ǿ��0
#define Strong_Filter1_address Strong_Filter0_address+1//�����˲�ǿ��1
#define Strong_Filter2_address Strong_Filter1_address+1//�����˲�ǿ��2
#define Strong_Filter3_address Strong_Filter2_address+1//�����˲�ǿ��3
#define Strong_Filter4_address Strong_Filter3_address+1//�����˲�ǿ��4

#define Cal_full_Mig_address  Strong_Filter4_address+1// 1561-1//������ֵ
#define Cal_pint_address  Cal_full_Mig_address+1// 1562//�궨С����
#define Analog_mode_SEL_address    Cal_pint_address+1//ģ�������ѡ��


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


//���������
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











