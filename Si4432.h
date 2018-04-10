
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
sbit si4432_SDN =P1^7;      //20120612�Ĳ���//sbit SDN =P1^0;0����4432
sbit si4432_NIRQ =P1^6;     //sbit si4432_NIRQ =P1^1;�ж��ź�0
sbit si4432_NSEL = P1^5; //sbit si4432_NSEL = P1^2; ��дģʽ����/����Ƭѡ
sbit si4432_SCK  = P1^4;    //sbit si4432_SCK  = P1^3;ʱ��
sbit si4432_MOSI = P1^3;    //sbit MOSI = P1^4;д����
sbit si4432_MISO = P1^2;    //sbit MISO = P1^5;������ 
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
  DeviceType 							   	= 0x00, //�豸������
  DeviceVersion								= 0x01, //Ӳ���汾��
  DeviceStatus 								= 0x02, //����״̬
  InterruptStatus1 							= 0x03, //�ж�״̬1
  InterruptStatus2 							= 0x04, //�ж�״̬2
  InterruptEnable1 							= 0x05, //�ж�ʹ��1        
  InterruptEnable2 							= 0x06, //�ж�ʹ��2         
  OperatingFunctionControl1 				= 0x07, //����ģʽ&���ܿ���1
  OperatingFunctionControl2 				= 0x08, //����ģʽ&���ܿ���2
  CrystalOscillatorLoadCapacitance 		= 0x09, //30MHZ���帺�ص���
  MicrocontrollerOutputClock 				= 0x0A, //΢���������ʱ��
  GPIO0Configuration 						= 0x0B, //GPIO����0
  GPIO1Configuration 						= 0x0C, //GPIO����1         
  GPIO2Configuration					   	= 0x0D, //GPIO����2
  IOPortConfiguration						= 0x0E, //IO�˿�����
  ADCConfiguration							= 0x0F, //ADC����
  ADCSensorAmplifierOffset					= 0x10, //ADC���зŴ���ƫ��
  ADCValue								   	= 0x11, //ADCֵ
  TemperatureSensorControl					= 0x12, //�¶ȴ�����У��
  TemperatureValueOffset					= 0x13, //�¶ȴ�����ƫ��
  WakeUpTimerPeriod1 						= 0x14, //���Ѷ�ʱ��ʱ��1          
  WakeUpTimerPeriod2 						= 0x15, //���Ѷ�ʱ��ʱ��2         
  WakeUpTimerPeriod3 						= 0x16, //���Ѷ�ʱ��ʱ��3         
  WakeUpTimerValue1							= 0x17, //���Ѷ�ʱ��ֵ1
  WakeUpTimerValue2							= 0x18, //���Ѷ�ʱ��ֵ2
  LowDutyCycleModeDuration 				= 0x19, //����������ģʽ����ʱ��       
  LowBatteryDetectorThreshold  			= 0x1A, //���Ƿѹ�������
  BatteryVoltageLevel 						= 0x1B, //��ص�ѹ����                          
  IFFilterBandwidth  						= 0x1C, //IF�˲�������                           
  AFCLoopGearshiftOverride					= 0x1D, //ʹ��AFCƵ��У��//AFCѭ�����ٳ���
  AFCTimingControl 							= 0x1E, //AFC��ʱ����                              
  ClockRecoveryGearshiftOverride 		= 0x1F, //ʱ�ӻָ����ٳ���              
  ClockRecoveryOversamplingRatio 		= 0x20, //ʱ�ӻָ���������              
  ClockRecoveryOffset2 						= 0x21, //ʱ�ӻָ�ƫ��2                       
  ClockRecoveryOffset1 						= 0x22, //ʱ�ӻָ�ƫ��1                       
  ClockRecoveryOffset0 						= 0x23, //ʱ�ӻָ�ƫ��0                     
  ClockRecoveryTimingLoopGain1 			= 0x24, //ʱ�ӻָ���ʱѭ������1             
  ClockRecoveryTimingLoopGain0 			= 0x25, //ʱ�ӻָ���ʱѭ������0             
  ReceivedSignalStrengthIndicator 		= 0x26, //�����ź�ǿ��ָʾ��          
  RSSIThresholdForClearChannelIndicato	= 0x27, //�޸����ŵ�����   
  AntennaDiversityRegister1				= 0x28, //���߷ּ�1
  AntennaDiversityRegister2				= 0x29, //���߷ּ�2


  DataAccessControl 					   	= 0x30, //���ݴ�ȡ����                         
  EZmacStatus 							   	= 0x31, //EzMAC״̬                                  
  HeaderControl1 						   	= 0x32, //֡ͷ����1                               
  HeaderControl2 							   = 0x33, //֡ͷ����2                              
  PreambleLength 					   		= 0x34, //ǰ���볤��                               
  PreambleDetectionControl 				= 0x35, //ǰ����������1                    
  SyncWord3 							   	= 0x36, //ͬ����3                                   
  SyncWord2 							   	= 0x37, //ͬ����2                                  
  SyncWord1 							   	= 0x38, //ͬ����1                              
  SyncWord0 							   	= 0x39, //ͬ����0                                
  TransmitHeader3						   	= 0x3A, //����֡ͷ3                       
  TransmitHeader2 							= 0x3B, //����֡ͷ2                           
  TransmitHeader1 							= 0x3C, //����֡ͷ1                              
  TransmitHeader0 							= 0x3D, //����֡ͷ0                             
  TransmitPacketLength 						= 0x3E, //�������ݰ�����                         
  CheckHeader3 								= 0x3F, //���֡ͷ3                                
  CheckHeader2 								= 0x40, //���֡ͷ2                              
  CheckHeader1 								= 0x41, //���֡ͷ1                             
  CheckHeader0 								= 0x42, //���֡ͷ0                            
  HeaderEnable3 						   	= 0x43, //֡ͷʹ��3                               
  HeaderEnable2 						   	= 0x44, //֡ͷʹ��2                                
  HeaderEnable1 						   	= 0x45, //֡ͷʹ��1                                
  HeaderEnable0 						   	= 0x46, //֡ͷʹ��0                              
  ReceivedHeader3 							= 0x47, //���յ���֡ͷ3                          
  ReceivedHeader2 							= 0x48, //���յ���֡ͷ2                         
  ReceivedHeader1 							= 0x49, //���յ���֡ͷ1                           
  ReceivedHeader0 							= 0x4A, //���յ���֡ͷ0                             
  ReceivedPacketLength						= 0x4B, //���յ������ݰ�����
  AnalogTestBus 						   	= 0x50, //ģ���������ѡ��                             
  DigitalTestBus 						   	= 0x51, //���ֲ�������                          
  TXRampControl 						   	= 0x52, //����б�ʿ���                             
  PLLTuneTime 							   	= 0x53, //PLL��гʱ��/54h����1                            
  CalibrationControl 						= 0x55, //У������                     
  ModemTest 							   	= 0x56, //���Ʋ���                               
  ChargepumpTest 						   	= 0x57, //���ò���                    
  ChargepumpCurrentTrimming_Override 	= 0x58, //���õ���΢��/����         
  DividerCurrentTrimming				 	= 0x59, //������ѹ��΢��
  VCOCurrentTrimming 						= 0x5A, //VCO����΢����                           
  VCOCalibration_Override 					= 0x5B, //VCOУ��                    
  SynthesizerTest 							= 0x5C, //�ϳ�������                              
  BlockEnableOverride1 						= 0x5D, //ģ��ʹ�ܳ���1                        
  BlockEnableOverride2 						= 0x5E, //ģ��ʹ�ܳ���2                      
  BlockEnableOverride3 						= 0x5F, //ģ��ʹ�ܳ���3                       
  ChannelFilterCoefficientAddress 		= 0x60, //Ƶ���˲���ϵ����ַ             
  ChannelFilterCoefficientValue 			= 0x61, //Ƶ���˲���ϵ��ֵ            
  CrystalOscillator_ControlTest 			= 0x62, //��������/�ϵ縴λ����	              
  RCOscillatorCoarseCalibration_Override 	= 0x63,//RC��������У��    
  RCOscillatorFineCalibration_Override 	= 0x64,//RC������ϸУ��      
  LDOControlOverride 						= 0x65, //LDO���Ƴ���/66h����2                          
  DeltasigmaADCTuning1			 			= 0x67, //ADC��г1
  DeltasigmaADCTuning2			 			= 0x68, //ADC��г2
  AGCOverride1					 		   	= 0x69, //AGC����1
  AGCOverride2 								= 0x6A, //AGC����2
  GFSKFIRFilterCoefficientAddress 		= 0x6B, //�˲���ϵ����ַ            
  GFSKFIRFilterCoefficientValue 			= 0x6C, //�˲���ϵ��ֵ             
  TXPower 								   	= 0x6D, //���书��                                   
  TXDataRate1 							   	= 0x6E, //�������ݲ�����1                            
  TXDataRate0 							   	= 0x6F, //�������ݲ�����0                              
  ModulationModeControl1 					= 0x70, //����ģʽ����1                   
  ModulationModeControl2 					= 0x71, //����ģʽ����2                   
  FrequencyDeviation 						= 0x72, //Ƶ��ƫ��                            
  FrequencyOffset 							= 0x73, //Ƶ��ƫ��                            
  FrequencyChannelControl					= 0x74, //Ƶ��Ƶ������
  FrequencyBandSelect 						= 0x75, //Ƶ��ѡ��                        
  NominalCarrierFrequency1	 				= 0x76, //����ز�Ƶ��                    
  NominalCarrierFrequency0 				= 0x77, //����ز�Ƶ��                  
  FrequencyHoppingChannelSelect 			= 0x79, //��ƵƵ��ѡ��               
  FrequencyHoppingStepSize 				= 0x7A, //��Ƶ����                    
  TXFIFOControl1 						   	= 0x7C, //TX FIFO����1                       
  TXFIFOControl2 						   	= 0x7D, //TX FIFO����2  
  RXFIFOControl 						   	= 0x7E, //RX FIFO����                               
  FIFOAccess							   	= 0x7F  //FIFO��ȡ
} RF_REG_MAP;

             

#endif
