v242
if no module,then no work immediately.

v241
X34 add check resure.

v240
fix bug COMM with BC26 at poweron.

v239
BC26 control ok.

v238
X34 used as water lever low alarm.
fix bug T44 ,char to int.



v237
only save to eeprom when X34 high.
add comm1 output information.
X34 no used,use X4 as save to eeprom and power fail signal.


v236
1 fix bug X3 active ,T44,T29,T30
2 49 error and 149 error only active if have middle_store
3 first X37/X38 not acitve  Y32
4 fix 24C26 read and write frequece, T_hight change from 400nS to 500+ns.
5 if counter0 no changed,then don't write 24c16


v233
20191113
fix bug
T52 used as middle_store enable


v232
for error check
add X50
add X51
add X52
add X53
add X54
add X46

add Y11

for middle store
add X48
add Y48
add X49
add Y49
add X58

for pakeg size big/middle/little
add X55
add X57

add parament 
T50 T51 T54 T55 

v231

v230 20190621
modify x33 up logic.

v230 20190620
add x33 up logic.

v229 20190610
clear y47 use clear_y 22 or x35 delay 4s.

v228 20190610
change logic of y47,set y47 at x35 up and clr at clr_y(22).
v227 20190609
y26 and y27 active condition,if X18 not ok ,then stop system and if resume ,continue check X18.
check X18 notice in screen.
v226 20190608
fix bug when  push pakge STEP modify for step 2 to step 2-9 ,the xstop  err.
v225 20190606
change y2 active logic,use X20 to disable y2

v224 20190604
fix bug ,the condition of y26 active.

v223 20190603
1 modify  y27 active contion,enable f_back_enable_x18 (equle enalbel the X35 up check) and add X18 notic.

v222 20190526
1 change logic of X20/X3/Y35 for multpackeg to same transtor
2 add step for PUSH_PACK

v221 20190519
fix X0 display bug and set_y(6)

v220
fix bug if xstop active,the y32 con't start .

v219
1 add condition X45 to y26 active
2 edit y27 condition ,delect f_back_18 ,add X18 direct at here.

v218
 删除恢复默认参数的功能
 
v217 
外部存储24lc16


V211


v210
修正X26为不报故障，提示等待

v188
1 去掉X2，X47
2 增加Y43，Y44
3 增加上电延时0.5S

v185
1 X43故障时，当X26到位，打开抱袋。
2 X15有时，X14有则报故障。
v180.3
完成6条


V180.2
完成2条

v180.1
完成第4条

v180
1 在171基础上做
2 修改X26故障不能停机的问题
3 在X27位置落袋后不要停在此位置
4 X8干扰容易死机的问题
5 故障提示和状态提示只提示第一个出现的
6 X42有时，最大拍袋次数由T15确定
7 “等待上袋完成"提示去掉




v171
增加了Y41，Y42 功能

v162
1 修改T47参数到T11参数delay_t[10],T47参数自V158版本起用做版本显示，和原T47时间参数冲突。
2 未使用参数T23，15
3 程序共使用了14.5K代码空间

v161
修正X18条件 
v160
Y27有增加X18有的条件

v159
修改消息os_send_signal(UP_BAG)发出位置，由整袋完成位置移至张袋完成位置。
去掉task_status[PRESS_CHECK]
v158
修改y26动作期间，发生停机时，如果X37或者X38有，不应立即停机，而应等待X38消失后延时T35再停机。
增加版本显示在T47位置

v157
恢复Y20第一次动作
v156
X18下降沿用T18延时代替

v155
X18上升沿用X43代替

v152
1 系统定时修改为10ms间隔
2 用X2上升沿取代原X18下降沿功能


v151
增加断线延时停机功能，T10，单位100ms

v150
修正T8期间X18来下降沿导致y40无法关闭的问题。

v149
y26有期间发生的所有停机，都在X26到位时停止。

v148
又增加X37下降沿X38无的故障
v147
1 去除38故障，X37下降沿如果X38为0，处理方法同X38下降沿
2 完善延迟停机xstop功能



v146
1 修改T8，T21不能为0，1的问题。
2 延迟停机功能xstop
  在X37或X38有时，不能立即停机，必须等到T35延时完成才能停止Y30，31，35，36，37，39
  
v145_1
y20动作次数改为一次。
v145
修正错误

20150826v144
1 时间参数未使用27，23，18，15，11，10
2 拍袋剩余T27次时，推包机开始返回,如果T27大于选定的拍袋次数，则不超过拍袋次数。
如果此时重新改写任意参数，则T27可能会改写为选定的拍袋次数。



v143_1修改推包机上电等待X27时间为原来2倍
v143修改推包机上电等待X27时间为原来2倍
20150825v142
修改y22和y25时间参数
20150824v141
修改v139.2条件，在X18下降沿去掉了条件y26.


20150824v140
修改y25有和y22无之先后关系
y25延时为T37或T33
y22延时为T22

20150824v139
1 删除V137新增之逻辑
2 X18控制Y40，Y5，用到时间t21，t8
3 y25输出之前增加延时t22

20150819v138
修改v137.2中，X4为X2.
20150819v137
1 删除v135逻辑
2 增加X18，X4，X37与Y5，Y40之间的逻辑


20150817v136
增加y26输出之前延时T24


20150815v135
修改有X18且Y26有时，Y5输出T8时间的功能
修改有X18且Y26有时，Y40输出T21时间的功能

20150815v134
增加有X18且Y27有时，Y5输出T8时间的功能
增加有X18且Y27有时，Y15输出T21时间的功能
20150812v133
1 修正v132问题
2 增加X22故障时不夹袋功能

20150812v132
	用X47切换130和130_1之区别
	如果X47有，则不延时，等最后一次拍袋。
	否则，等延时不等拍袋。

20150808v131_1
1 完整的信息提示，报警功能
2 修正重复上袋的问题
3 修正上袋假死机的问题
4 修正X8对张袋功能的影响

2015.8.1
增加等待信号提示X1xx

2015.4.3 v130
原为X34有就OK，改为
X34有并且X33有就OK
2015.4.3
//else if(!X34)改为
else if((X34)||(X33))	//20150403
原为X34有就OK，改为
X34无并且X33无就OK

20150808
v131_1
1 修正上袋死等问题
2 修正X22故障处理问题
3 增加所有问题提示包括故障处理和状态提示
4 增加新功能18-22条
5 修改部分参数为25S范围

20150312-1
去掉20150311功能
20150312
整袋机送袋0.6S
2015.3.11
1 拍袋前停机恢复
2 拍袋快完成时才允许推包机回位（剩余拍袋次数小于等于1）

2015.3.6
去掉拍袋前停机检查

2015.2.14
1 修改X23X24不到位死机问题
2 X16到位后延时 T43 ，如果发现有X22故障，不上空袋
3 完善诊断输出

2015.2.12
repare get_y() bug
replace with get_outy() and get_y()

2015.2.11
1 处理X22故障时，上空袋问题
  发现X22故障，不发出f_open信号，则不上袋。
2 等待X33信号之后，加延时T42再加气吸袋张口

2015.1.21
1 修改撑袋失败后不重撑袋的问题（增加f_open变量互斥）
2 修改可能死机的BUG
3 修改可能导致上袋和推包机打架的可能（待测）

2015.1.20
1 修改程序结构，增加线程OPEN_BAG，撑袋线程
2 增加撑袋等待信号X33

2014.7.8
1 增加X44检测，下料门动作到位信号
2 增加x46检测，料仓料位检测信号

20140208
修改原X42的功能，根据X42是否有决定是否掉袋

2014.1.29
增加上袋后延时夹袋参数T38
增加根据X42决定T9是否再增加附加的延时T41

v.12.4
1 增加细料功能


20140122
v1.2.3
1 增加上电不抱袋功能，f_y25
2 增加拍袋前延迟参数T6
3 增加拍袋后延迟T9
4 增加掉袋和抱袋、拨袋强制输出

v1.2.2

1 增加参数自动保存功能
2 修改调试模式部分端口不能调试的问题

v1.2.1
增加485支持

v1.2
1 修正剪线错误
2 增加完整modbus

v1.1
 
add modbus ok
19200,8,n,2
in[]4 word
out[]4 word
delay_t[]32 word

v1.0.1
1 修改上袋不成功不再上袋的问题
2 增加推包机两端停止功能
3 缝包机和皮带机在推包状态2不允许停止
4 Y22（掉袋）消失时间比Y25（抱袋）信号晚0.1S

V1.0
1 修改拍袋次数T26为4，时间T25为500mS
2 修改拨袋收起时间T14为400mS
