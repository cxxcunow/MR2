#include "combinations.h"

//bool optoelec_switch[2];

/**
*组合动作 放在这里写了
*有：		JUMP		开始动作		结束动作		越障		过程逻辑
*/

void StepOver(void)
{
    StepOver_one_leg(0);  //跨0
    vTaskDelay(150);

    StepOver_one_leg(1);	//跨1
    vTaskDelay(150);

    state=REALSE;				//释放
    vTaskDelay(150);

    x = 18*sin(40*PI/180);		//前两个腿保持一致小小的站起来 后一点
    y = 18*cos(40*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(0);
    SetCoupledPosition(1);
    vTaskDelay(100);

    _leg_active[0]=NO;		//关闭前两个腿
    _leg_active[1]=NO;
    state=TEST4;				//前面低下来走
    vTaskDelay(300);
    _leg_active[0]=YES;		//打开前两个腿
    _leg_active[1]=YES;
    vTaskDelay(3000);

    state=TEST6;				//小碎步
    vTaskDelay(1500);

    state=REALSE; 			//释放

    StepOver_one_leg(2);	//跨2
    vTaskDelay(400);

    StepOver_one_leg(3);	//跨3
    vTaskDelay(400);

    state=REALSE;		//释放
    vTaskDelay(500);

    x = 19*sin(28*PI/180);		//后两个腿 小站 向前走
    y = 19*cos(28*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(2);
    SetCoupledPosition(3);
    vTaskDelay(100);

    _leg_active[2]=NO;		//关闭后两个腿
    _leg_active[3]=NO;
    state=TEST5; //前面正常 后面低下来走
    vTaskDelay(1000);

    _leg_active[2]=YES;		//打开后两个腿
    _leg_active[3]=YES;
    vTaskDelay(2000);

    state=REALSE;		//释放
    vTaskDelay(250);

    state=TROT;		    //小跑跑下沙丘

    vTaskDelay(250);

}


void StepOver_one_leg(int LegId)
{

    //------四个腿子全部抬到最高-----//

    if(LegId==0||LegId==2)
    {
        x = 0;
        y = 28;
        CartesianToTheta(1.0);
        CommandAllLegs_v();
        vTaskDelay(600);
    }
    else if(LegId==1||LegId==3)
    {
        vTaskDelay(200);
    }

    //--------对角腿缩最短--------//
    int rev_legid, rev_legid1;
    if(LegId==0) {
        rev_legid=3 ;
        y = 25;
    }
    else if(LegId==1) {
        x = 0;
        y = 28;
        CartesianToTheta(1.0);
        SetCoupledPosition(3);
        vTaskDelay(300);

        rev_legid=2 ;
        y = 25 ;
    }
    else if(LegId==2) {
        rev_legid=1 ;
        y = 25;
    }
    else if(LegId==3) {
        rev_legid=0 ;
        y = 16;
    }
    x = 0;
    CartesianToTheta(1.0);
    SetCoupledPosition(rev_legid);
    vTaskDelay(300);

    //----只有leg3的时候运行----前三个腿进入平衡状态---//

    if(LegId==3)
    {
        x = -21*sin(15*PI/180);
        y = 21*cos(15*PI/180);
        CartesianToTheta(1.0);
        SetCoupledPosition(1);

        x = -18*sin(17*PI/180);
        y = 18*cos(17*PI/180);
        CartesianToTheta(1.0);
        SetCoupledPosition(0);

        x = 22*sin(10*PI/180);
        y = 22*cos(40*PI/180)-4;		//高度保持
        CartesianToTheta(1.0);
        SetCoupledPosition(2);

        vTaskDelay(800);

    }

    //--------腿最短向后--------//
    if(LegId==3)
    {
        x = -10*sin(80*PI/180);
        y = 10*cos(80*PI/180);
        CartesianToTheta(1.0);
        SetCoupledPosition(LegId);
        vTaskDelay(600);
    }
    else
    {

//        x = -10*sin(60*PI/180);
//        y = 10*cos(60*PI/180);
//        CartesianToTheta(1.0);
//        SetCoupledPosition(LegId);
//        vTaskDelay(600);

    }

    //-------腿最短向前----------//
    x = 10*sin(60*PI/180);
    y = 10*cos(60*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------腿最长状态向前--------//
    x = 26*sin(80*PI/180);
    y = 26*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------腿最长状态向下触地--------//
    if(LegId==0||LegId==1)
    {
        x = 29*sin(34*PI/180);
        y = 29*cos(34*PI/180);
    }
    else if(LegId==2)
    {
        x = 22*sin(40*PI/180);
        y = 22*cos(40*PI/180);
    }
    else if(LegId==3)
    {
        x = 20*sin(50*PI/180);
        y = 20*cos(50*PI/180);
    }
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

}



void CrossTheLine(void)
{

    state=REALSE;

    CrossTheLine_one_leg(0); //跨0
    vTaskDelay(150);

    CrossTheLine_one_leg(1);	//跨1
    vTaskDelay(150);

    now_time=times;
    state=TEST1;  //------------向前走让后腿接触绳子
    vTaskDelay(2200);
    state=REALSE;

    CrossTheLine_one_leg(2);		//跨2
    vTaskDelay(150);

    CrossTheLine_one_leg(3);		//跨3
    vTaskDelay(150);

    //**********中间行走**********//
    now_time=times;
    state=TEST1;

    OpenMvInspect(openmv_Red);  //检测到红色之后开始迈第二根绳子

    IndicateLED_On;

    state=STOP;
    state=REALSE;

    //**********END**********//

    CrossTheLine_one_leg(0);
    vTaskDelay(200);

    IndicateLED_Off;

    CrossTheLine_one_leg(1);
    vTaskDelay(200);

    now_time=times;
    state=TEST1;  //跨绳子步态
    vTaskDelay(2000);
    state=REALSE;

    CrossTheLine_one_leg(2);
    vTaskDelay(200);

    CrossTheLine_one_leg(3);
    vTaskDelay(200);
}

void CrossTheLine_one_leg(int LegId)
{

    //------四个腿子全部抬到最高-----//
    x = 0;
    y = 28;
    CartesianToTheta(1.0);
    CommandAllLegs_v();
    vTaskDelay(600);

    //--------对角腿缩最短--------//
    int rev_legid;
    if(LegId==0) rev_legid=3;
    else if(LegId==1) rev_legid=2;
    else if(LegId==2) rev_legid=1;
    else if(LegId==3) rev_legid=0;
    x = 0;
    y = 24.5;
    CartesianToTheta(1.0);
    SetCoupledPosition(rev_legid);
    vTaskDelay(150);

    //当前腿先向下蹬一下保持平衡//
    x = 0;
    y = 29;
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(800);

    //--------腿最长到后面--------//
    x = -24*sin(80*PI/180);
    y = 24*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------腿缩最短在后面--------//
    x = -10*sin(80*PI/180);
    y = 10*cos(80*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------腿最短状态向前--------//
    x = 10*sin(75*PI/180);
    y = 10*cos(75*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(400);

    //--------腿伸长向前--------//
    x = 24*sin(70*PI/180);
    y = 24*cos(70*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(300);

    //--------腿最长向下触地--------//
    x = 29*sin(1*PI/180); //1°
    y = 29*cos(1*PI/180);
    CartesianToTheta(1.0);
    SetCoupledPosition(LegId);
    vTaskDelay(300);

    //------四个腿恢复初始位置-----//
    x = 0;
    y = 28;
    CartesianToTheta(1.0);
    CommandAllLegs_v();
    vTaskDelay(200);

}

void OpenMvInspect(int color)
{

    while(1)
    {
        vTaskDelay(100);
        if(openmvinfo.ActVal[2]==color)
        {
            vTaskDelay(100);
            if(openmvinfo.ActVal[2]==color)
            {
                vTaskDelay(100);
                if(openmvinfo.ActVal[2]==color)
                {
                    vTaskDelay(100);
                    if(openmvinfo.ActVal[2]==color)
                    {
                        vTaskDelay(100);
                        if(openmvinfo.ActVal[2]==color)
                            break;
                    }
                }

            }
        }
    }

}

float OpenMv_Line_Dec(void)
{
	
//	float dev_calc_ang;
//	
//	dev_calc_ang = pid_calc(&pid_openmv_dev,openmvinfo.ActVal[0],); 
//	
//	
//	return
}
void Climbing_Test(void)
{
	
	        //-------------------    爬坡测试程序  ------------------------//
        while(keyInf1!=0||keyInf2!=0) //第一段完成-------等待光电开关--进入爬坡程序-----
            vTaskDelay(500);

        LinearCorrection=Deny;
				LinearCorrection=climbing_correction;

        yaw_set=imuinfo.ActVal[0];//设定当前角度为前进方向
        now_time=times;
        climbing_offset_flag=YES;
        _climbing_offset_angle=15;
        state= CLIMBING;

				vTaskDelay(5000);  //延时3000

        OpenMvInspect(openmv_Red);  //等待检测到红色 色块 摄像头已经检测到了爬到了坡顶

        IndicateLED_On;

        vTaskDelay(3500);  //延时3000

        IndicateLED_Off;

        state= STOP;

        Servo3_OPEN;  //锁位舵机打开

        Servo1_PEAK; //舵机一高位
        vTaskDelay(200);

        Servo2_PEAK_POS;//舵机二高位

        while(1)		//停
            vTaskDelay(500);

}

float testtheta,testtheta1,testtheta2;

float legs_point_offset = 0,front_leg_angle = 100,back_leg_angle =102,back_open_angle_offset = 0;

//float legs_point_offset = 3,front_leg_angle = 101,back_leg_angle =113,back_open_angle_offset = 0;


void StartPosToMiddlePos (void)
{
    state = REALSE;

    //----------------------------先小小的张开腿--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=25.0;
    testtheta2=25.0;
    temp_pid.ref_agle[0]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta2+0)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta2+0)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=testtheta2*ReductionAndAngleRatio;

    //后腿儿朝向向后
//    temp_pid.ref_agle[4]-=(testtheta2+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=(testtheta2+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta2*ReductionAndAngleRatio;
//

    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(800);

//----------------------------躺着dao起来--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=front_leg_angle;
    testtheta2=back_leg_angle;
    temp_pid.ref_agle[0]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;

//后腿儿朝向向后
//    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]-=(testtheta2-0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=(testtheta2-0)*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(800);

//----------------------------打开腿--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 25.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=60.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;

//后腿儿朝向向后
//    temp_pid.ref_agle[4]-=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕


    vTaskDelay(1000);

    // printf(" moto_chassis = %d   moto_chassis[1]  =%d    ref_agle %f\n",moto_chassis[0].total_angle,moto_chassis[7].total_angle,ref_agle[0]);
//----------------------------设置当前点为零点--------------------------
    memset(&moto_chassis,0,sizeof(moto_measure_t)*8);

    for(int i=0; i<8; i++)
        temp_pid.ref_agle[i]=ref_agle[i]=0;

    vTaskDelay(100);
    //printf(" moto_chassis = %d   moto_chassis[1]  =%d  ref_agle %f\n",moto_chassis[0].total_angle,moto_chassis[7].total_angle,ref_agle[0]);

    state = REALSE;
    printf("Start Done.");


}

void MiddlePosToEndPos (void)
{

    x=0;
    y = 17.3205081;

    CartesianToTheta(1.0);
    CommandAllLegs_v();

    state = REALSE;
//----------------------------腿并拢--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 24.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta=-85.0;
    temp_pid.ref_agle[0]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]-=testtheta*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]-=(testtheta+back_open_angle_offset)*ReductionAndAngleRatio;

//后腿儿朝向向后
//    temp_pid.ref_agle[4]-=(testtheta+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]-=(testtheta+0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]+=testtheta*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=testtheta*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕


    vTaskDelay(600);
//----------------------------下去--------------------------
    for (int i = 0; i < 8; i++) {
        pid_reset_kpkd(&pid_pos[i], 22.0, 0);
        pid_reset_kpkd(&pid_spd[i], 15.5f, 0);
    }
    testtheta1=-front_leg_angle;
    testtheta2=-back_leg_angle;

    temp_pid.ref_agle[0]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[1]+=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[2]-=testtheta1*ReductionAndAngleRatio;
    temp_pid.ref_agle[3]+=testtheta1*ReductionAndAngleRatio;

    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
    temp_pid.ref_agle[6]-=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;
    temp_pid.ref_agle[7]+=(testtheta2-legs_point_offset)*ReductionAndAngleRatio;

//后腿儿朝向向后
//    temp_pid.ref_agle[4]-=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[5]+=testtheta2*ReductionAndAngleRatio;
//    temp_pid.ref_agle[6]-=(testtheta2-0)*ReductionAndAngleRatio;
//    temp_pid.ref_agle[7]+=(testtheta2-0)*ReductionAndAngleRatio;
    IsMotoReadyOrNot= IsReady;		//数据填充完毕

    vTaskDelay(400);

//----------------------------设置0位--------------------------
    memset(&moto_chassis,0,sizeof(moto_measure_t)*8);
    for(int i=0; i<8; i++)
        temp_pid.ref_agle[i]=ref_agle[i]=0;

    vTaskDelay(100);

    state = REALSE;

    printf("END Done.");

}




void TrajectoryJump(float t, float launchTime, float stanceHeight, float downAMP) {
    //Need to check if n works
    float n = t/launchTime;
    x = 0;
    y = downAMP*n + stanceHeight;
    //y = downAMP*sin(PI/4 + PI/4*n) + stanceHeight;
}

float start_time_ = 0.0f;

void StartJump(float start_time_s) {
    start_time_ = start_time_s;
    state = JUMP;
}

void ExecuteJump() {
    // min radius = 0.8
    // max radius = 0.25

    const float prep_time = 0.8f; // 准备时间 [s]
    const float launch_time = 0.2f ; // 收缩腿前的持续时间 [s]
    const float fall_time = 0.8f; //收缩腿后持续时间恢复正常行为 [s]

    const float stance_height = 22.0f; // 跳跃之前腿的高度 [cm]
    const float jump_extension = 28.9f; // 最大伸腿长度 [cm]
    const float fall_extension = 20.0f; // 降落时的期望腿长 [cm]


    float t = times/1000.0f - start_time_/1000.0f; // 跳跃开始后的时间

    if (t < prep_time) {
        x = 0;
        y = stance_height;
        CartesianToTheta(1.0);
        // 使用刚度小，阻尼大的增益
        LegGain gains = {15.5, 0.00, 25.0, 0.00};
        CommandAllLegs(gains);

    } else if (t >= prep_time && t < prep_time + launch_time) {

        x = -jump_extension*sin(20*PI/180);
        y = jump_extension*cos(20*PI/180);
//        x = 0;
//        y = jump_extension;
        CartesianToTheta(1.0);

        // 使用高刚度和低阻尼执行跳转
        LegGain gains = {25.5, 0.00, 49.0, 0.00};
        CommandAllLegs(gains);

    } else if (t >= prep_time + launch_time && t < prep_time + launch_time + fall_time) {
        x = 0;
        y = fall_extension;
        CartesianToTheta(1.0);

        //使用低刚度和大量的阻尼来处理下降
        LegGain gains = {5.5, 0.00, 21.0, 0.00};
        CommandAllLegs(gains);

    } else {
        state = STOP;
        printf("Jump Complete.");
    }

}

