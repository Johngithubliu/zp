v80
if auto=1 then no work untile auth arived
else working normaly

v77
add remote control

v76
1 X2 =1 ,modbus address=0x0b,else =0x0a;
2 only save parament to eeprom when X8 up.
3 add total clear
4 add ver display
5 comm1 


v75 
change T10 unit from 100mS to S.
v74 
add t[31] choice v71 or v73 ,y26 and y27 on.

v73
if find locked each other,then ,if delay_t[30]==1,this mechinie clear y3 and clare y20.
add y31 output.
change the condition of y26 y27

v72
y26,y27动作条件修改为和X1相关

v71
T14 溢出处理

v70
删除恢复默认参数的功能
v69
外部存储器24lc16.

v64
停秤后再启动时，参照首次上电时对X10的处理方式。

v4.5
增加等待信号提示：

等待X1/101
等待X3/103
等待X4/104
等待X5/105
等待X14/114
等待X16/116
等待X20消失/120
等待X23/123
等待X31/131
等待X0消失/190
等待X1消失/191
等待Y3/203
等待Y19消失/219

系统提示：
X12/12
X13/13
X14/14
X17/17
停秤/90

v4.4
错误提示只提示第一个。


v3.3
y19有增加条件X5，X4

v3.2
修正X21/X22有时不停机的bug
v3·1
修正bug
v30
修改Y21/Y24/Y25运行逻辑
v29
增加Y29/Y30互锁条件

v28
增加X29，X30，Y29，Y30，T25，T26及其逻辑

v27
合并自动手动V25/V26两个版本
增加T15切换自动手动模式
修正x21/x24/x25动作

v25
增加Y26，Y27动作



20150821v23
y19有增加一个条件X28有。
20150814v21
修改夹袋问题，在X9一直有效情况下，上电允许直接夹袋
20150812v20
修改吨称输送为手动
20150812v19
1 增加双称互锁功能
2 增加吨称输送带功能（全自动）
2015.4.18v18
1 去掉线程DISABLE_JD(禁止夹袋)，功能放入线程STEP_12中
2 增加线程TIME_100MS,处理Y9延时T4.
3 Y16动作条件中，去掉X12

2015.4.17v17
1 所有clear_y(8)之前增加 os_delete_task(Y8_ON),防止Y8启动。

2015.4.2v16
1 去掉线程MODBUS，功能移入sys_con
2 增加单独线程Y16_ON,检查X12及X13，任一个有，则Y16有
3 增加单独线程check_x14
2015.4.2v15
1 首次上电Y14检查和T17延时同时进行
2 X13检查输出Y16，检查点放在7步。

2015.4.2 V14
1 增加首次上电X14无时闪烁Y14.
2 当X13有，则Y16有

2015.4.2 v13
首次上电必须收到X14有，才继续工作。

2015.3.31 v12
T7,T17,R15都改为秒单位
if((X23)&&(X16)&&(!X0)&&(!get_y(19)))set_y(10);


2015.3.30 v11

1 初次上电延时T7（10S）Y16有，延时T17（1.2S），Y16无
2 Y10有增加X23 有的条件
3 X12有，Y16有
4 X14有，延时T15，Y16无


2015.2.17 调试
v10