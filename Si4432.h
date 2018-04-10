
 /*============================================================================
**
** FILE
**  Si4432.h
**
** DESCRIPTION
**  Header files for Si4432 usage, contains RF specific definition and type declaration
**
** CREATED
**  Silicon Laboratories Hungary Ltd
**
** COPYRIGHT
**  Copyright 2008 Silicon Laboratories, Inc.  
**	http://www.silabs.com
**20120613
** ============================================================================
*/

#ifndef __Si4432_H__
#define __Si4432_H__

#include "headerfile.h"	  //
/*/VCC
sbit si4432_SDN =P1^7;      //20120612改插座//sbit SDN =P1^0;0开启4432
sbit si4432_NIRQ =P1^6;     //sbit si4432_NIRQ =P1^1;中断信号0
sbit si4432_NSEL = P1^5; //sbit si4432_NSEL = P1^2; 读写模式控制/串行片选
sbit si4432_SCK  = P1^4;    //sbit si4432_SCK  = P1^3;时钟
sbit si4432_MOSI = P1^3;    //sbit MOSI = P1^4;写数据
sbit si4432_MISO = P1^2;    //sbit MISO = P1^5;读数据 
//GIPO0
//GND */


//void delay(unsigned char delaytime);
//void Delay5Ms1(void);
void SpiRfWrite8Bit(unsigned char byte);
unsigned char SpiRfRead8Bit(void);
void SpiWriteRegister(unsigned char add, unsigned char reg);
unsigned char SpiReadRegister(unsigned char add);
void RF_GPIO(void);
void RFSetRxMode(void);
void RFSetTxMode(void);
void RFSetIdleMode(void);
//void RFFIFOSendData(U8 length,  U16 *payload);//void RFFIFOSendData(U8 length, U8 *payload);
void RFWriteFIFO( U8 z,   U8 *(buffer));//void RFWriteFIFO( unsigned char z,  unsigned char data *(buffer));
void RFSendOutData(void);
void RFResetRxFIFO(void);
void RFResetTxFIFO(void);
unsigned char jieshou();
void RF_INIT(void);

#define TxGPIOSetting()   SpiWriteRegister(0x0E, 0x02);
#define RxGPIOSetting()   SpiWriteRegister(0x0E, 0x01);

#define	REG_READ					(0x00)
#define	REG_WRITE					(0x80)



typedef enum _RF_REG_MAP					
{
  DeviceType 							   	= 0x00, //设备类型码
  DeviceVersion								= 0x01, //硬件版本码
  DeviceStatus 								= 0x02, //器件状态
  InterruptStatus1 							= 0x03, //中断状态1
  InterruptStatus2 							= 0x04, //中断状态2
  InterruptEnable1 							= 0x05, //中断使能1        
  InterruptEnable2 							= 0x06, //中断使能2         
  OperatingFunctionControl1 				= 0x07, //工作模式&功能控制1
  OperatingFunctionControl2 				= 0x08, //工作模式&功能控制2
  CrystalOscillatorLoadCapacitance 		= 0x09, //30MHZ晶体负载电容
  MicrocontrollerOutputClock 				= 0x0A, //微控制器输出时钟
  GPIO0Configuration 						= 0x0B, //GPIO配置0
  GPIO1Configuration 						= 0x0C, //GPIO配置1         
  GPIO2Configuration					   	= 0x0D, //GPIO配置2
  IOPortConfiguration						= 0x0E, //IO端口配置
  ADCConfiguration							= 0x0F, //ADC配置
  ADCSensorAmplifierOffset					= 0x10, //ADC传感放大器偏差
  ADCValue								   	= 0x11, //ADC值
  TemperatureSensorControl					= 0x12, //温度传感器校正
  TemperatureValueOffset					= 0x13, //温度传感器偏差
  WakeUpTimerPeriod1 						= 0x14, //唤醒定时器时期1          
  WakeUpTimerPeriod2 						= 0x15, //唤醒定时器时期2         
  WakeUpTimerPeriod3 						= 0x16, //唤醒定时器时期3         
  WakeUpTimerValue1							= 0x17, //唤醒定时器值1
  WakeUpTimerValue2							= 0x18, //唤醒定时器值2
  LowDutyCycleModeDuration 				= 0x19, //低任务周期模式持续时间       
  LowBatteryDetectorThreshold  			= 0x1A, //电池欠压检测门限
  BatteryVoltageLevel 						= 0x1B, //电池电压级别                          
  IFFilterBandwidth  						= 0x1C, //IF滤波器带宽                           
  AFCLoopGearshiftOverride					= 0x1D, //使能AFC频率校正//AFC循环变速超速
  AFCTimingControl 							= 0x1E, //AFC定时控制                              
  ClockRecoveryGearshiftOverride 		= 0x1F, //时钟恢复变速超速              
  ClockRecoveryOversamplingRatio 		= 0x20, //时钟恢复过抽样率              
  ClockRecoveryOffset2 						= 0x21, //时钟恢复偏差2                       
  ClockRecoveryOffset1 						= 0x22, //时钟恢复偏差1                       
  ClockRecoveryOffset0 						= 0x23, //时钟恢复偏差0                     
  ClockRecoveryTimingLoopGain1 			= 0x24, //时钟恢复定时循环增益1             
  ClockRecoveryTimingLoopGain0 			= 0x25, //时钟恢复定时循环增益0             
  ReceivedSignalStrengthIndicator 		= 0x26, //接收信号强度指示器          
  RSSIThresholdForClearChannelIndicato	= 0x27, //无干扰信道门限   
  AntennaDiversityRegister1				= 0x28, //天线分集1
  AntennaDiversityRegister2				= 0x29, //天线分集2


  DataAccessControl 					   	= 0x30, //数据存取控制                         
  EZmacStatus 							   	= 0x31, //EzMAC状态                                  
  HeaderControl1 						   	= 0x32, //帧头控制1                               
  HeaderControl2 							   = 0x33, //帧头控制2                              
  PreambleLength 					   		= 0x34, //前导码长度                               
  PreambleDetectionControl 				= 0x35, //前导码侦测控制1                    
  SyncWord3 							   	= 0x36, //同步字3                                   
  SyncWord2 							   	= 0x37, //同步字2                                  
  SyncWord1 							   	= 0x38, //同步字1                              
  SyncWord0 							   	= 0x39, //同步字0                                
  TransmitHeader3						   	= 0x3A, //发射帧头3                       
  TransmitHeader2 							= 0x3B, //发射帧头2                           
  TransmitHeader1 							= 0x3C, //发射帧头1                              
  TransmitHeader0 							= 0x3D, //发射帧头0                             
  TransmitPacketLength 						= 0x3E, //发射数据包长度                         
  CheckHeader3 								= 0x3F, //检查帧头3                                
  CheckHeader2 								= 0x40, //检查帧头2                              
  CheckHeader1 								= 0x41, //检查帧头1                             
  CheckHeader0 								= 0x42, //检查帧头0                            
  HeaderEnable3 						   	= 0x43, //帧头使能3                               
  HeaderEnable2 						   	= 0x44, //帧头使能2                                
  HeaderEnable1 						   	= 0x45, //帧头使能1                                
  HeaderEnable0 						   	= 0x46, //帧头使能0                              
  ReceivedHeader3 							= 0x47, //接收到的帧头3                          
  ReceivedHeader2 							= 0x48, //接收到的帧头2                         
  ReceivedHeader1 							= 0x49, //接收到的帧头1                           
  ReceivedHeader0 							= 0x4A, //接收到的帧头0                             
  ReceivedPacketLength						= 0x4B, //接收到的数据包长度
  AnalogTestBus 						   	= 0x50, //模拟测试总线选择                             
  DigitalTestBus 						   	= 0x51, //数字测试总线                          
  TXRampControl 						   	= 0x52, //发射斜率控制                             
  PLLTuneTime 							   	= 0x53, //PLL调谐时间/54h保留1                            
  CalibrationControl 						= 0x55, //校正控制                     
  ModemTest 							   	= 0x56, //调制测试                               
  ChargepumpTest 						   	= 0x57, //充电泵测试                    
  ChargepumpCurrentTrimming_Override 	= 0x58, //充电泵电流微调/超调         
  DividerCurrentTrimming				 	= 0x59, //电流分压器微调
  VCOCurrentTrimming 						= 0x5A, //VCO电流微调节                           
  VCOCalibration_Override 					= 0x5B, //VCO校正                    
  SynthesizerTest 							= 0x5C, //合成器测试                              
  BlockEnableOverride1 						= 0x5D, //模块使能超驰1                        
  BlockEnableOverride2 						= 0x5E, //模块使能超驰2                      
  BlockEnableOverride3 						= 0x5F, //模块使能超驰3                       
  ChannelFilterCoefficientAddress 		= 0x60, //频道滤波器系数地址             
  ChannelFilterCoefficientValue 			= 0x61, //频道滤波器系数值            
  CrystalOscillator_ControlTest 			= 0x62, //晶体振荡器/上电复位控制	              
  RCOscillatorCoarseCalibration_Override 	= 0x63,//RC振荡器粗略校正    
  RCOscillatorFineCalibration_Override 	= 0x64,//RC振荡器精细校正      
  LDOControlOverride 						= 0x65, //LDO控制超驰/66h保留2                          
  DeltasigmaADCTuning1			 			= 0x67, //ADC调谐1
  DeltasigmaADCTuning2			 			= 0x68, //ADC调谐2
  AGCOverride1					 		   	= 0x69, //AGC过载1
  AGCOverride2 								= 0x6A, //AGC过载2
  GFSKFIRFilterCoefficientAddress 		= 0x6B, //滤波器系数地址            
  GFSKFIRFilterCoefficientValue 			= 0x6C, //滤波器系数值             
  TXPower 								   	= 0x6D, //发射功率                                   
  TXDataRate1 							   	= 0x6E, //发射数据波特率1                            
  TXDataRate0 							   	= 0x6F, //发射数据波特率0                              
  ModulationModeControl1 					= 0x70, //调制模式控制1                   
  ModulationModeControl2 					= 0x71, //调制模式控制2                   
  FrequencyDeviation 						= 0x72, //频率偏差                            
  FrequencyOffset 							= 0x73, //频率偏移                            
  FrequencyChannelControl					= 0x74, //频率频道控制
  FrequencyBandSelect 						= 0x75, //频段选择                        
  NominalCarrierFrequency1	 				= 0x76, //标称载波频率                    
  NominalCarrierFrequency0 				= 0x77, //标称载波频率                  
  FrequencyHoppingChannelSelect 			= 0x79, //跳频频率选择               
  FrequencyHoppingStepSize 				= 0x7A, //跳频步码                    
  TXFIFOControl1 						   	= 0x7C, //TX FIFO控制1                       
  TXFIFOControl2 						   	= 0x7D, //TX FIFO控制2  
  RXFIFOControl 						   	= 0x7E, //RX FIFO控制                               
  FIFOAccess							   	= 0x7F  //FIFO存取
} RF_REG_MAP;

             

#endif
