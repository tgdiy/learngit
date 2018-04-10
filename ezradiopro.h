#ifndef EZRADIOPRO_H
#define EZRADIOPRO_H
/***********************************************************************
#ifndef COMPILER_DEFS_H
#define COMPILER_DEFS_H
************************************************************************/
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
// Keil C51
// http://www.keil.com
# define SEG_GENERIC
# define SEG_FAR   xdata
# define SEG_DATA  data
# define SEG_NEAR  data
# define SEG_IDATA idata
# define SEG_XDATA xdata
# define SEG_PDATA pdata
# define SEG_CODE  code
# define SEG_BDATA bdata

# define SBIT(name, addr, bit)  sbit  name = addr^bit
# define SFR(name, addr)        sfr   name = addr
# define SFR16(name, addr)      sfr16 name = addr
# define SFR16E(name, fulladdr) /* not supported */
# define SFR32(name, fulladdr)  /* not supported */
# define SFR32E(name, fulladdr) /* not supported */

# define INTERRUPT(name, vector) void name (void) interrupt vector
# define INTERRUPT_USING(name, vector, regnum) void name (void) interrupt vector using regnum
# define INTERRUPT_PROTO(name, vector) void name (void)
# define INTERRUPT_PROTO_USING(name, vector, regnum) void name (void)

# define FUNCTION_USING(name, return_value, parameter, regnum) return_value name (parameter) using regnum
# define FUNCTION_PROTO_USING(name, return_value, parameter, regnum) return_value name (parameter)
// Note: Parameter must be either 'void' or include a variable type and name. (Ex: char temp_variable)

# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
# define VARIABLE_SEGMENT_POINTER(name, vartype, targsegment) vartype targsegment * name
# define SEGMENT_VARIABLE_SEGMENT_POINTER(name, vartype, targsegment, locsegment) vartype targsegment * locsegment name
# define SEGMENT_POINTER(name, vartype, locsegment) vartype * locsegment name
# define LOCATED_VARIABLE_NO_INIT(name, vartype, locsegment, addr) vartype locsegment name _at_ addr

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0

//typedef unsigned char U8;
//typedef unsigned int U16;
//typedef unsigned long U32;

//typedef signed char S8;
//typedef signed int S16;
//typedef signed long S32;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

// NOP () macro support
extern void _nop_ (void);
#define NOP() _nop_()
/****************************************************************
#ifndef HARDWARE_DEFS_H
#define HARDWARE_DEFS_H
*****************************************************************/
#include "AT89X52.H"  //#include "reg24le1.h"
#define  SI1010_DAUGHTER_CARD

//-----------------------------------------------------------------------------
// Use daughter card def to select SDN
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Enable RF switch on high-power Si1000 and Si1010
//-----------------------------------------------------------------------------
#ifdef SI1010_DAUGHTER_CARD
#define ENABLE_RF_SWITCH
#endif

// OSC load cap value for XTAL on Si1000 Daughtercard
//-----------------------------------------------------------------------------
#define EZRADIOPRO_OSC_CAP_VALUE 0x7f      //12pbm
/*******************************************************************************
#ifndef  PP_PHY_H
#define  PP_PHY_H
*******************************************************************************/
//=============================================================================
//-----------------------------------------------------------------------------
// Expected RADIO_VERSION code for radio VERSION register
// Production version should be 0x06
//-----------------------------------------------------------------------------
#define MIN_RADIO_VERSION   0x05
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define SYSCLK_HZ                (24500000L)
#define MILLISECONDS(t)          (((SYSCLK_HZ/1000)*(t))/48)
//-----------------------------------------------------------------------------
// ET0 inverted and used as timeout flag
//-----------------------------------------------------------------------------
#define TIMEOUT_T0   (!ET0)
//-----------------------------------------------------------------------------
// phy status values
//-----------------------------------------------------------------------------
#define SI4432_STATUS U8
enum
{
   SI4432_STATUS_SUCCESS = 0x00,
   SI4432_STATUS_ERROR_SPI,
   SI4432_STATUS_ERROR_UNSUPPORTED_RADIO,
   SI4432_STATUS_ERROR_NO_IRQ,
   SI4432_STATUS_ERROR_RADIO_XTAL,
   SI4432_STATUS_ERROR_RADIO_SHUTDOWN,
   SI4432_STATUS_TRANSMIT_ERROR,
   SI4432_STATUS_ERROR_NO_PACKET
};
//-----------------------------------------------------------------------------
// Public variables (API)
//-----------------------------------------------------------------------------

//=============================================================================
// end PP_PHY_H
//=============================================================================
/****************************************************************************
#ifndef  PHY_DEFS_H
#define  PHY_DEFS_H
*******************************************************************************/
//-----------------------------------------------------------------------------
// Frequency and Data rate settings
//-----------------------------------------------------------------------------
#define TRX_FREQUENCY         (433000000L)
#define TRX_CHANNEL_SPACING   (0)
#define TRX_DEVIATION         (45000L)	     // 偏频
#define TRX_DATA_RATE         (9600L)	     //速率
#define RX_BANDWIDTH          (99600L)		 //IF滤波器带宽
//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define BUFFER_MSPACE         SEG_XDATA
//=============================================================================
// End
//=============================================================================
/*******************************************************************************
#ifndef  PP_PHY_CONST_H
#define  PP_PHY_CONST_H
*******************************************************************************/
//================================================================================================
//------------------------------------------------------------------------------------------------
// defines
//------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define DEFAULT_MODULATION_MODE_CONTROL_1    0x0C  // Manchester disabled
#define DEFAULT_MODULATION_MODE_CONTROL_2    0x23  // FIFO mode GFSK
#define DEFAULT_AFC_LOOP_GEARSHIFT_OVERRIDE  0x40  // AFC Enabled setting
#define DEFAULT_TX_POWER                     0x1F  // +20 dB, LNA switch enabled
//-----------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
// defines
//------------------------------------------------------------------------------------------------
#define DIVR(a,b)    ((a+b/2)/b)
#define BYTE0(a)     (a&0x000000FFL)
#define BYTE1(a)     ((a&0x0000FF00L)>>8)
#define BYTE2(a)     ((a&0x00FF0000L)>>16)
#define BYTE3(a)     ((a&0xFF000000L)>>24)
//------------------------------------------------------------------------------------------------
// Frequency Macros
//------------------------------------------------------------------------------------------------
#define CALC_HIGH_FREQ_BAND(f)               ((f-480000000L)/20000000L)
#define CALC_LOW_FREQ_BAND(f)                ((f-240000000L)/10000000L)
#define CALC_FREQUENCY_BAND_SELECT(f)        ((f>480000000)?(CALC_HIGH_FREQ_BAND(f)|0x60):(CALC_LOW_FREQ_BAND(f)|0x40))
#define CALC_CARRIER_FREQUENCY_HIGH(f)       ((DIVR((f-480000000L-20000000L*CALC_HIGH_FREQ_BAND(f)),625L))<<1)
#define CALC_CARRIER_FREQUENCY_LOW(f)        ((DIVR((f-240000000L-10000000L*CALC_LOW_FREQ_BAND(f)),625L))<<2)
#define CALC_NOMINAL_CARRIER_FREQEUNCY(f)    ((f>480000000)?CALC_CARRIER_FREQUENCY_HIGH(f):CALC_CARRIER_FREQUENCY_LOW(f))
#define CALC_NOMINAL_CARRIER_FREQUENCY_0(f)  BYTE0(CALC_NOMINAL_CARRIER_FREQEUNCY(f))
#define CALC_NOMINAL_CARRIER_FREQUENCY_1(f)  BYTE1(CALC_NOMINAL_CARRIER_FREQEUNCY(f))
//------------------------------------------------------------------------------------------------
// TRX_CHANNEL_SPACING Macros
//------------------------------------------------------------------------------------------------
#define CALC_FREQUENCY_HOPPING_STEP_SIZE(f)  (DIVR(f,10000L))
//------------------------------------------------------------------------------------------------
// TX_DEVIATION Macros
//------------------------------------------------------------------------------------------------
#define CALC_TX_DEVIATION(f)  (DIVR(f,625L))
#define TX_DEVIATION_B8(f)      ((DIVR(f,625L)>255)?0x40:0x00)
//------------------------------------------------------------------------------------------------
// TX_DATA_RATE Macros
//------------------------------------------------------------------------------------------------
#define TX_DATA_RATE_LOW(r)         ((r<30000L)?0x20:0x00)
#define CALC_TX_DATA_RATE_LOW(r)    (DIVR((r<<16),31250L))
#define CALC_TX_DATA_RATE_HIGH(r)   (DIVR((r<<15),500000L))
#define CALC_TX_DATA_RATE(r)        ((r<30000L)?CALC_TX_DATA_RATE_LOW(r):CALC_TX_DATA_RATE_HIGH(r))
#define CALC_TX_DATA_RATE_0(r)      BYTE0(CALC_TX_DATA_RATE(r))
#define CALC_TX_DATA_RATE_1(r)      BYTE1(CALC_TX_DATA_RATE(r))
//------------------------------------------------------------------------------------------------
// RX Modem Macros
//------------------------------------------------------------------------------------------------
#define CALC_RXOSR(dwn3_bypass,ndec_exp,rb)   (DIVR(((500000L*8*(dwn3_bypass*2+1))>>ndec_exp),(rb)))

#define CALC_NCOFF(dwn3_bypass,ndec_exp,rb)  (DIVR(((DIVR(((rb)<<15),15625))<<ndec_exp),(dwn3_bypass*2+1)))

#define CALC_CRGAIN(rb,dwn3_bypass,ndec_exp,rxdev) (DIVR(DIVR((rb<<15),(DIVR(((500000L*8*(dwn3_bypass*2+1))>>ndec_exp),(rb)))),rxdev)+2)

//------------------------------------------------------------------------------------------------
// RX Modem Register Macros
//------------------------------------------------------------------------------------------------
#define CALC_IF_FILTER_BANDWIDTH(dwn3_bypass,ndec_exp,filset) (((dwn3_bypass)<<7)+((ndec_exp)<<4)+(filset))
#define CALC_CLOCK_RECOVERY_OVERSAMPLING_RATIO(dwn3_bypass,ndec_exp,rb)  BYTE0(CALC_RXOSR(dwn3_bypass,ndec_exp,rb))
#define CALC_CLOCK_RECOVERY_OFFSET_2(dwn3_bypass,ndec_exp,rb)    BYTE2(CALC_NCOFF(dwn3_bypass,ndec_exp,rb))
#define CALC_CLOCK_RECOVERY_OFFSET_1(dwn3_bypass,ndec_exp,rb)    BYTE1(CALC_NCOFF(dwn3_bypass,ndec_exp,rb))
#define CALC_CLOCK_RECOVERY_OFFSET_0(dwn3_bypass,ndec_exp,rb)    BYTE0(CALC_NCOFF(dwn3_bypass,ndec_exp,rb))
#define CALC_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1(rb,dwn3_bypass,ndec_exp,rxdev)    BYTE1( CALC_CRGAIN(rb,dwn3_bypass,ndec_exp,rxdev))
#define CALC_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0(rb,dwn3_bypass,ndec_exp,rxdev)    BYTE0( CALC_CRGAIN(rb,dwn3_bypass,ndec_exp,rxdev))
#define CALC_AFC_LIMITER(afcrange, f)   ((f>480000000)?(DIVR((afcrange/2),625)):(DIVR(afcrange,625)))
//------------------------------------------------------------------------------------------------
// defines
//------------------------------------------------------------------------------------------------
#ifdef TRANSMITTER_ONLY
#define NUMBER_OF_REGISTERS                       15
#endif
#ifdef  RECEIVER_ONLY
#define NUMBER_OF_REGISTERS                       20
#endif
#ifndef NUMBER_OF_REGISTERS						   //收发一起的寄存器
#define NUMBER_OF_REGISTERS                       24
#endif
//------------------------------------------------------------------------------------------------
// Public code constants
//------------------------------------------------------------------------------------------------

//=================================================================================================
/***************************************************************************************
#ifndef SI1010_DEFS_H
#define SI1010_DEFS_H
****************************************************************************************/
// EZRadioPRO Register Defines
//
// The register names are defined exactly as listed in the Function/
// Description field of the Si1000 data sheet. Upper Case is used for
// constants. Spaces and miscellaneous characters are replaced with
// underscores. EZRADIOPRO_ prefix added for Si1000.
//
//=============================================================================
#define  EZRADIOPRO_DEVICE_TYPE                                0x00
#define  EZRADIOPRO_DEVICE_VERSION                             0x01
#define  EZRADIOPRO_DEVICE_STATUS                              0x02
#define  EZRADIOPRO_INTERRUPT_STATUS_1                         0x03
#define  EZRADIOPRO_INTERRUPT_STATUS_2                         0x04
#define  EZRADIOPRO_INTERRUPT_ENABLE_1                         0x05
#define  EZRADIOPRO_INTERRUPT_ENABLE_2                         0x06
#define  EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1           0x07
#define  EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_2           0x08
#define  EZRADIOPRO_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE        0x09
#define  EZRADIOPRO_MICROCONTROLLER_OUTPUT_CLOCK               0x0A
#define  EZRADIOPRO_GPIO0_CONFIGURATION                        0x0B
#define  EZRADIOPRO_GPIO1_CONFIGURATION                        0x0C
#define  EZRADIOPRO_GPIO2_CONFIGURATION                        0x0D
#define  EZRADIOPRO_IO_PORT_CONFIGURATION                      0x0E
#define  EZRADIOPRO_ADC_CONFIGURATION                          0x0F
#define  EZRADIOPRO_ADC_SENSOR_AMPLIFIER_OFFSET                0x10
#define  EZRADIOPRO_ADC_VALUE                                  0x11
#define  EZRADIOPRO_TEMPERATURE_SENSOR_CONTROL                 0x12
#define  EZRADIOPRO_TEMPERATURE_VALUE_OFFSET                   0x13
#define  EZRADIOPRO_WAKE_UP_TIMER_PERIOD_1                     0x14
#define  EZRADIOPRO_WAKE_UP_TIMER_PERIOD_2                     0x15
#define  EZRADIOPRO_WAKE_UP_TIMER_PERIOD_3                     0x16
#define  EZRADIOPRO_WAKE_UP_TIMER_VALUE_1                      0x17
#define  EZRADIOPRO_WAKE_UP_TIMER_VALUE_2                      0x18
#define  EZRADIOPRO_LOW_DUTY_CYCLE_MODE_DURATION               0x19
#define  EZRADIOPRO_LOW_BATTERY_DETECTOR_THRESHOLD             0x1A
#define  EZRADIOPRO_BATTERY_VOLTAGE_LEVEL                      0x1B
#define  EZRADIOPRO_IF_FILTER_BANDWIDTH                        0x1C
#define  EZRADIOPRO_AFC_LOOP_GEARSHIFT_OVERRIDE                0x1D
#define  EZRADIOPRO_AFC_TIMING_CONTROL                         0x1E
#define  EZRADIOPRO_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE          0x1F
#define  EZRADIOPRO_CLOCK_RECOVERY_OVERSAMPLING_RATIO          0x20
#define  EZRADIOPRO_CLOCK_RECOVERY_OFFSET_2                    0x21
#define  EZRADIOPRO_CLOCK_RECOVERY_OFFSET_1                    0x22
#define  EZRADIOPRO_CLOCK_RECOVERY_OFFSET_0                    0x23
#define  EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1          0x24
#define  EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0          0x25
#define  EZRADIOPRO_RECEIVED_SIGNAL_STRENGTH_INDICATOR         0x26
#define  EZRADIOPRO_RSSI_THRESHOLD                             0x27
#define  EZRADIOPRO_ANTENNA_DIVERSITY_REGISTER_1               0x28
#define  EZRADIOPRO_ANTENNA_DIVERSITY_REGISTER_2               0x29
#define  EZRADIOPRO_DATA_ACCESS_CONTROL                        0x30
#define  EZRADIOPRO_EZMAC_STATUS                               0x31
#define  EZRADIOPRO_HEADER_CONTROL_1                           0x32
#define  EZRADIOPRO_HEADER_CONTROL_2                           0x33
#define  EZRADIOPRO_PREAMBLE_LENGTH                            0x34
#define  EZRADIOPRO_PREAMBLE_DETECTION_CONTROL                 0x35
#define  EZRADIOPRO_SYNC_WORD_3                                0x36
#define  EZRADIOPRO_SYNC_WORD_2                                0x37
#define  EZRADIOPRO_SYNC_WORD_1                                0x38
#define  EZRADIOPRO_SYNC_WORD_0                                0x39
#define  EZRADIOPRO_TRANSMIT_HEADER_3                          0x3A
#define  EZRADIOPRO_TRANSMIT_HEADER_2                          0x3B
#define  EZRADIOPRO_TRANSMIT_HEADER_1                          0x3C
#define  EZRADIOPRO_TRANSMIT_HEADER_0                          0x3D
#define  EZRADIOPRO_TRANSMIT_PACKET_LENGTH                     0x3E
#define  EZRADIOPRO_CHECK_HEADER_3                             0x3F
#define  EZRADIOPRO_CHECK_HEADER_2                             0x40
#define  EZRADIOPRO_CHECK_HEADER_1                             0x41
#define  EZRADIOPRO_CHECK_HEADER_0                             0x42
#define  EZRADIOPRO_HEADER_ENABLE_3                            0x43
#define  EZRADIOPRO_HEADER_ENABLE_2                            0x44
#define  EZRADIOPRO_HEADER_ENABLE_1                            0x45
#define  EZRADIOPRO_HEADER_ENABLE_0                            0x46
#define  EZRADIOPRO_RECEIVED_HEADER_3                          0x47
#define  EZRADIOPRO_RECEIVED_HEADER_2                          0x48
#define  EZRADIOPRO_RECEIVED_HEADER_1                          0x49
#define  EZRADIOPRO_RECEIVED_HEADER_0                          0x4A
#define  EZRADIOPRO_RECEIVED_PACKET_LENGTH                     0x4B
#define  EZRADIOPRO_ANALOG_TEST_BUS                            0x50
#define  EZRADIOPRO_DIGITAL_TEST_BUS                           0x51
#define  EZRADIOPRO_TX_RAMP_CONTROL                            0x52
#define  EZRADIOPRO_PLL_TUNE_TIME                              0x53
#define  EZRADIOPRO_CALIBRATION_CONTROL                        0x55
#define  EZRADIOPRO_MODEM_TEST                                 0x56
#define  EZRADIOPRO_CHARGEPUMP_TEST                            0x57
#define  EZRADIOPRO_CHARGEPUMP_CURRENT_TRIMMING_OVERRIDE       0x58
#define  EZRADIOPRO_DIVIDER_CURRENT_TRIMMING                   0x59
#define  EZRADIOPRO_VCO_CURRENT_TRIMMING                       0x5A
#define  EZRADIOPRO_VCO_CALIBRATION_OVERRIDE                   0x5B
#define  EZRADIOPRO_SYNTHESIZER_TEST                           0x5C
#define  EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_1                    0x5D
#define  EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_2                    0x5E
#define  EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_3                    0x5F
#define  EZRADIOPRO_CHANNEL_FILTER_COEFFICIENT_ADDRESS         0x60
#define  EZRADIOPRO_CHANNEL_FILTER_COEFFICIENT_VALUE           0x61
#define  EZRADIOPRO_CRYSTAL_OSCILLATOR_CONTROL_TEST            0x62
#define  EZRADIOPRO_RC_OSCILLATOR_COARSE_CALIBRATION_OVERRIDE  0x63
#define  EZRADIOPRO_RC_OSCILLATOR_FINE_CALIBRATION_OVERRIDE    0x64
#define  EZRADIOPRO_LDO_CONTROL_OVERRIDE                       0x65
#define  EZRADIOPRO_LDO_LEVEL_SETTING                          0x66
#define  EZRADIOPRO_DELTASIGMA_ADC_TUNING_1                    0x67
#define  EZRADIOPRO_DELTASIGMA_ADC_TUNING_2                    0x68
#define  EZRADIOPRO_AGC_OVERRIDE_1                             0x69
#define  EZRADIOPRO_AGC_OVERRIDE_2                             0x6A
#define  EZRADIOPRO_GFSK_FIR_FILTER_COEFFICIENT_ADDRESS        0x6B
#define  EZRADIOPRO_GFSK_FIR_FILTER_COEFFICIENT_VALUE          0x6C
#define  EZRADIOPRO_TX_POWER                                   0x6D
#define  EZRADIOPRO_TX_DATA_RATE_1                             0x6E
#define  EZRADIOPRO_TX_DATA_RATE_0                             0x6F
#define  EZRADIOPRO_MODULATION_MODE_CONTROL_1                  0x70
#define  EZRADIOPRO_MODULATION_MODE_CONTROL_2                  0x71
#define  EZRADIOPRO_FREQUENCY_DEVIATION                        0x72
#define  EZRADIOPRO_FREQUENCY_OFFSET_1                         0x73
#define  EZRADIOPRO_FREQUENCY_OFFSET_2                         0x74
#define  EZRADIOPRO_FREQUENCY_BAND_SELECT                      0x75
#define  EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1                0x76
#define  EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0                0x77
#define  EZRADIOPRO_FREQUENCY_HOPPING_CHANNEL_SELECT           0x79
#define  EZRADIOPRO_FREQUENCY_HOPPING_STEP_SIZE                0x7A
#define  EZRADIOPRO_TX_FIFO_CONTROL_1                          0x7C
#define  EZRADIOPRO_TX_FIFO_CONTROL_2                          0x7D
#define  EZRADIOPRO_RX_FIFO_CONTROL                            0x7E
#define  EZRADIOPRO_FIFO_ACCESS                                0x7F
//------------------------------------------------------------------------------------------------
// new registers for B1 radio
#define  EZRADIOPRO_AFC_LIMITER                                0x2A
#define  EZRADIOPRO_AFC_CORRECTION                             0x2B
#define  EZRADIOPRO_OOK_COUNTER_VALUE_1                        0x2C
#define  EZRADIOPRO_OOK_COUNTER_VALUE_2                        0X2D
#define  EZRADIOPRO_SLICER_PEAK_HOLD                           0X2E
#define  EZRADIOPRO_ADC8_CONTROL                               0x4F
#define  EZRADIOPRO_INVALID_PREAMBLE_THRESHOLD_AND_PA_MISC     0x54
#define  EZRADIOPRO_MISCELLANEOUS_SETTINGS                     0x78
#define  EZRADIOPRO_TURN_AROUND_AND_15_4_LENGTH_COMPLIANCE     0x7B
//=============================================================================
//
// Register Bit Masks
//
//=============================================================================
// EZRADIOPRO_DEVICE_TYPE                                      0x00
#define  EZRADIOPRO_DT_MASK            0x1F
// EZRADIOPRO_DEVICE_VERSION                                   0x01
#define  EZRADIOPRO_VC_MASK            0x1F
// EZRADIOPRO_DEVICE_STATUS                                    0x02
#define  EZRADIOPRO_CPS_MASK           0x03
#define  EZRADIOPRO_LOCKDET            0x04
#define  EZRADIOPRO_FREQERR            0x08
#define  EZRADIOPRO_HEADERR            0x10
#define  EZRADIOPRO_RXFFEM             0x20
#define  EZRADIOPRO_FFUNFL             0x40
#define  EZRADIOPRO_FFOVFL             0x80

// EZRADIOPRO_INTERRUPT_STATUS_1                               0x03
#define  EZRADIOPRO_ICRCERROR          0x01
#define  EZRADIOPRO_IPKVALID           0x02
#define  EZRADIOPRO_IPKSENT            0x04
#define  EZRADIOPRO_IEXT               0x08
#define  EZRADIOPRO_IRXFFAFULL         0x10
#define  EZRADIOPRO_ITXFFAEM           0x20
#define  EZRADIOPRO_ITXFFAFULL         0x40
#define  EZRADIOPRO_IFFERR             0x80

// EZRADIOPRO_INTERRUPT_STATUS_2                               0x04
#define  EZRADIOPRO_IPOR               0x01
#define  EZRADIOPRO_ICHIPRDY           0x02
#define  EZRADIOPRO_ILBD               0x04
#define  EZRADIOPRO_IWUT               0x08
#define  EZRADIOPRO_IRSSI              0x10
#define  EZRADIOPRO_IPREAINVAL         0x20
#define  EZRADIOPRO_IPREAVAL           0x40
#define  EZRADIOPRO_ISWDET             0x80

// EZRADIOPRO_INTERRUPT_ENABLE_1                               0x05
#define  EZRADIOPRO_ENCRCERROR         0x01
#define  EZRADIOPRO_ENPKVALID          0x02
#define  EZRADIOPRO_ENPKSENT           0x04
#define  EZRADIOPRO_ENEXT              0x08
#define  EZRADIOPRO_ENRXFFAFULL        0x10
#define  EZRADIOPRO_ENTXFFAEM          0x20
#define  EZRADIOPRO_ENTXFFAFULL        0x40
#define  EZRADIOPRO_ENFFERR            0x80

// EZRADIOPRO_INTERRUPT_ENABLE_2                               0x06
#define  EZRADIOPRO_ENPOR              0x01
#define  EZRADIOPRO_ENCHIPRDY          0x02
#define  EZRADIOPRO_ENLBDI             0x04 // added I to make unique
#define  EZRADIOPRO_ENWUT              0x08
#define  EZRADIOPRO_ENRSSI             0x10
#define  EZRADIOPRO_ENPREAINVAL        0x20
#define  EZRADIOPRO_ENPREAVAL          0x40
#define  EZRADIOPRO_ENSWDET            0x80

// EZRADIOPRO_OPERATING_FUNCTION_CONTROL_1                     0x07
#define  EZRADIOPRO_XTON               0x01
#define  EZRADIOPRO_PLLON              0x02
#define  EZRADIOPRO_RXON               0x04
#define  EZRADIOPRO_TXON               0x08
#define  EZRADIOPRO_X32KSEL            0x10
#define  EZRADIOPRO_ENWT               0x20
#define  EZRADIOPRO_ENLBD              0x40
#define  EZRADIOPRO_SWRES              0x80

// EZRADIOPRO_OPERATING_FUNCTION_CONTROL_2                     0x08
#define  EZRADIOPRO_FFCLRTX            0x01
#define  EZRADIOPRO_FFCLRRX            0x02
#define  EZRADIOPRO_ENLDM              0x04
#define  EZRADIOPRO_AUTOTX             0x08
#define  EZRADIOPRO_RXMPK              0x10
#define  EZRADIOPRO_ANTDIV_MASK        0xE0

// EZRADIOPRO_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE              0x09
#define  EZRADIOPRO_XLC_MASK           0x7F
#define  EZRADIOPRO_XTALSHFT           0x80

// EZRADIOPRO_MICROCONTROLLER_OUTPUT_CLOCK                     0x0A
#define  EZRADIOPRO_MCLK_MASK          0x07
#define  EZRADIOPRO_ENLFC              0x08
#define  EZRADIOPRO_CLKT_MASK          0x30

// EZRADIOPRO_GPIO0_CONFIGURATION                              0x0B
#define  EZRADIOPRO_GPIO0_MASK         0x1F
#define  EZRADIOPRO_PUP0               0x20
#define  EZRADIOPRO_GPIO0DRV_MASK      0xC0

// EZRADIOPRO_GPIO1_CONFIGURATION                              0x0C
#define  EZRADIOPRO_GPIO1_MASK         0x1F
#define  EZRADIOPRO_PUP1               0x20
#define  EZRADIOPRO_GPIO1DRV_MASK      0xC0

// EZRADIOPRO_GPIO2_CONFIGURATION                              0x0D
#define  EZRADIOPRO_GPIO2_MASK         0x1F
#define  EZRADIOPRO_PUP2               0x20
#define  EZRADIOPRO_GPIO2DRV_MASK      0xC0

// EZRADIOPRO_IO_PORT_CONFIGURATION                            0x0E
#define  EZRADIOPRO_DIO_MASK           0x07
#define  EZRADIOPRO_ITSDO              0x08
#define  EZRADIOPRO_EXTITST_MASK       0x70

// EZRADIOPRO_ADC_CONFIGURATION                                0x0F
#define  EZRADIOPRO_ADCGAIN_MASK        0x03
#define  EZRADIOPRO_ADCREF_MASK         0x0C
#define  EZRADIOPRO_ADCSEL_MASK         0x70
#define  EZRADIOPRO_ADCSTART            0x80  //W
#define  EZRADIOPRO_ADCDONE             0x80  //R

// EZRADIOPRO_ADC_SENSOR_AMPLIFIER_OFFSET                      0x10
#define  EZRADIOPRO_ADCOFFS_MASK       0x0F

// EZRADIOPRO_ADC_VALUE                                        0x11
// no bits or mask

// EZRADIOPRO_TEMPERATURE_SENSOR_CONTROL                       0x12
#define  EZRADIOPRO_TSTRIM_MASK         0x0F
#define  EZRADIOPRO_ENTSTRIM            0x10
#define  EZRADIOPRO_ENTSOFFS            0x20
#define  EZRADIOPRO_TSRANGE_MASK        0xC0

// EZRADIOPRO_TEMPERATURE_VALUE_OFFSET                         0x13
// no bits or mask

// EZRADIOPRO_WAKE_UP_TIMER_PERIOD_1                           0x14
#define  EZRADIOPRO_WTD_MASK           0x03
#define  EZRADIOPRO_WTR_MASK           0x3C

// EZRADIOPRO_WAKE_UP_TIMER_PERIOD_2                           0x15
// no bits or mask

// EZRADIOPRO_WAKE_UP_TIMER_PERIOD_3                           0x16
#define  EZRADIOPRO_WTM_MASK           0x80

// EZRADIOPRO_WAKE_UP_TIMER_VALUE_1                            0x17
// no bits or mask

// EZRADIOPRO_WAKE_UP_TIMER_VALUE_2                            0x18
// no bits or mask

// EZRADIOPRO_LOW_DUTY_CYCLE_MODE_DURATION                     0x19
// no bits or mask

// EZRADIOPRO_LOW_BATTERY_DETECTOR_THRESHOLD                   0x1A
#define  EZRADIOPRO_LBDT_MASK          0x1F

// EZRADIOPRO_BATTERY_VOLTAGE_LEVEL                            0x1B
#define  EZRADIOPRO_VBAT_MASK          0x1F

// EZRADIOPRO_I_F_FILTER_BANDWIDTH                             0x1C
#define  EZRADIOPRO_FILSET_MASK        0x0F
#define  EZRADIOPRO_NDEC_MASK          0x70
#define  EZRADIOPRO_DWN3_BYPASS        0x80

// EZRADIOPRO_A_F_C_LOOP_GEARSHIFT_OVERRIDE                    0x1D
#define  EZRADIOPRO_AFCGEARH_MASK      0x3F
#define  EZRADIOPRO_ENAFC              0x40
#define  EZRADIOPRO_AFCBD              0x80

// EZRADIOPRO_A_F_C_TIMING_CONTROL                             0x1E
#define  EZRADIOPRO_LGWAIT_MASK        0x07
#define  EZRADIOPRO_SHWAIT_MASK        0x38

// EZRADIOPRO_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE                0x1F
#define  EZRADIOPRO_CRSLOW_MASK        0x07
#define  EZRADIOPRO_CRFAST_MASK        0x38
#define  EZRADIOPRO_RXREADY            0x40

// EZRADIOPRO_CLOCK_RECOVERY_OVERSAMPLING_RATIO                0x20
// no bits or mask

// EZRADIOPRO_CLOCK_RECOVERY_OFFSET_2 0x21
#define  EZRADIOPRO_NCOFF_MASK         0x0F
#define  EZRADIOPRO_STALLCTRL          0x10
#define  EZRADIOPRO_RXOSR_MASK         0xE0

// EZRADIOPRO_CLOCK_RECOVERY_OFFSET_1                          0x22
// no bits or mask

// EZRADIOPRO_CLOCK_RECOVERY_OFFSET_0                          0x23
// no bits or mask

// EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_1                0x24
#define  EZRADIOPRO_CRGAIN_MASK        0x07

// EZRADIOPRO_CLOCK_RECOVERY_TIMING_LOOP_GAIN_0                0x25
// no bits or mask

// EZRADIOPRO_RECEIVED_SIGNAL_STRENGTH_INDICATOR               0x26
// no bits or mask

// EZRADIOPRO_RSSI_THRESHOLD                                   0x27
// no bits or mask

// EZRADIOPRO_ANTENNA_DIVERSITY_REGISTER_1                     0x28
// no bits or mask

// EZRADIOPRO_ANTENNA_DIVERSITY_REGISTER_2                     0x29
// no bits or mask

// EZRADIOPRO_DATA_ACCESS_CONTROL                              0x30
#define  EZRADIOPRO_CRC_MASK           0x03
#define  EZRADIOPRO_CRC_16             0x01
#define  EZRADIOPRO_ENCRC              0x04
#define  EZRADIOPRO_ENPACTX            0x08
#define  EZRADIOPRO_CRCDONLY           0x20
#define  EZRADIOPRO_LSBFRST            0x40
#define  EZRADIOPRO_ENPACRX            0x80

// EZRADIOPRO_EZ_MAC_STATUS                                    0x31
#define  EZRADIOPRO_PKSENT             0x01
#define  EZRADIOPRO_PKTX               0x02
#define  EZRADIOPRO_CRCERROR           0x04
#define  EZRADIOPRO_PKVALID            0x08
#define  EZRADIOPRO_PKRX               0x10
#define  EZRADIOPRO_PKSRCH             0x20
#define  EZRADIOPRO_RXCRC1             0x40

// EZRADIOPRO_HEADER_CONTROL_1                                 0x32
#define  EZRADIOPRO_HDCH_MASK          0x0F
#define  EZRADIOPRO_BCEN_MASK          0xF0
#define  EZRADIOPRO_BCEN               0xF0
#define  EZRADIOPRO_DISABLE_HFILTERS   0x00

// EZRADIOPRO_HEADER_CONTROL_2                                 0x33
#define  EZRADIOPRO_PREALEN_MASK       0x01
#define  EZRADIOPRO_SYNCLEN_MASK       0x06
#define  EZRADIOPRO_FIXPKLEN           0x08
#define  EZRADIOPRO_HDLEN_MASK         0x70
#define  EZRADIOPRO_SYNCLEN_2BYTE      0x02
#define  EZRADIOPRO_SYNCLEN_3BYTE      0x04
#define  EZRADIOPRO_HDLEN_2BYTE        0x20
#define  EZRADIOPRO_HDLEN_3BYTE        0x30
#define  EZRADIOPRO_HDLEN_4BYTE        0x40

// EZRADIOPRO_PREAMBLE_LENGTH                                  0x34
// no bits or mask

// EZRADIOPRO_SYNC_WORD_3                                      0x36
// no bits or mask

// EZRADIOPRO_SYNC_WORD_2                                      0x37
// no bits or mask

// EZRADIOPRO_SYNC_WORD_1                                      0x38
// no bits or mask

// EZRADIOPRO_SYNC_WORD_0                                      0x39
// no bits or mask

// EZRADIOPRO_TRANSMIT_HEADER_3                                0x3A
// no bits or mask

// EZRADIOPRO_TRANSMIT_HEADER_2                                0x3B
// no bits or mask

// EZRADIOPRO_TRANSMIT_HEADER_1                                0x3C
// no bits or mask

// EZRADIOPRO_TRANSMIT_HEADER_0                                0x3D
// no bits or mask

// EZRADIOPRO_TRANSMIT_PACKET_LENGTH                           0x3E
// no bits or mask

// EZRADIOPRO_CHECK_HEADER_3                                   0x3F
// no bits or mask

// EZRADIOPRO_CHECK_HEADER_2                                   0x40
// no bits or mask

// EZRADIOPRO_CHECK_HEADER_1                                   0x41
// no bits or mask

// EZRADIOPRO_CHECK_HEADER_0                                   0x42
// no bits or mask

// EZRADIOPRO_HEADER_ENABLE_3                                  0x43
// no bits or mask

// EZRADIOPRO_HEADER_ENABLE_2                                  0x44
// no bits or mask

// EZRADIOPRO_HEADER_ENABLE_1                                  0x45
// no bits or mask

// EZRADIOPRO_HEADER_ENABLE_0                                  0x46
// no bits or mask

// EZRADIOPRO_RECEIVED_HEADER_3                                0x47
// no bits or mask

// EZRADIOPRO_RECEIVED_HEADER_2                                0x48
// no bits or mask

// EZRADIOPRO_RECEIVED_HEADER_1                                0x49
// no bits or mask

// EZRADIOPRO_RECEIVED_HEADER_0                                0x4A
// no bits or mask

// EZRADIOPRO_RECEIVED_PACKET_LENGTH                           0x4B
// no bits or mask

// EZRADIOPRO_ANALOG_TEST_BUS                                  0x50
#define  EZRADIOPRO_ATB_MASK           0x1F

// EZRADIOPRO_DIGITAL_TEST_BUS                                 0x51
#define  EZRADIOPRO_DTB_MASK           0x2F
#define  EZRADIOPRO_ENSCTEST           0x40

// EZRADIOPRO_TX_RAMP_CONTROL                                  0x52
#define  EZRADIOPRO_TXRAMP_MASK        0x03
#define  EZRADIOPRO_LDORAMP_MASK       0x0C
#define  EZRADIOPRO_TXMOD_MASK         0x70

// EZRADIOPRO_PLL_TUNE_TIME                                    0x53
#define  EZRADIOPRO_PLLT0_MASK         0x07
#define  EZRADIOPRO_PLLTS_MASK         0xF8

// EZRADIOPRO_CALIBRATION_CONTROL                              0x55
#define  EZRADIOPRO_SKIPVCO            0x01
#define  EZRADIOPRO_VCOCAL             0x02
#define  EZRADIOPRO_VCOCALDP           0x04
#define  EZRADIOPRO_RCCAL              0x08
#define  EZRADIOPRO_ENRCFCAL           0x10
#define  EZRADIOPRO_ADCCALDONE         0x20
#define  EZRADIOPRO_XTALSTARTHALF      0x40

// EZRADIOPRO_MODEM_TEST                                       0x56
#define  EZRADIOPRO_IQSWITCH           0x01
#define  EZRADIOPRO_REFCLKINV          0x02
#define  EZRADIOPRO_REFCLKSEL          0x04
#define  EZRADIOPRO_AFCPOL             0x10
#define  EZRADIOPRO_DTTYPE             0x20
#define  EZRADIOPRO_SLICFBYP           0x40
#define  EZRADIOPRO_BCRFBYP            0x80

// EZRADIOPRO_CHARGEPUMP_TEST                                  0x57
#define  EZRADIOPRO_CDCCUR_MASK        0x07
#define  EZRADIOPRO_CDCONLY            0x08
#define  EZRADIOPRO_CPFORCEDN          0x10
#define  EZRADIOPRO_CPFORCEUP          0x20
#define  EZRADIOPRO_FBDIV_RST          0x40
#define  EZRADIOPRO_PFDRST             0x80

// EZRADIOPRO_CHARGEPUMP_CURRENT_TRIMMING_OVERRIDE             0x58
#define  EZRADIOPRO_CPCORR_MASK        0x1F
#define  EZRADIOPRO_CPCORROV           0x20
#define  EZRADIOPRO_CPCURR_MASK        0xC0

// EZRADIOPRO_DIVIDER_CURRENT_TRIMMING                         0x59
#define  EZRADIOPRO_D1P5TRIM_MASK      0x03
#define  EZRADIOPRO_D2TRIM_MASK        0x0C
#define  EZRADIOPRO_D3TRIM_MASK        0x30
#define  EZRADIOPRO_FBDIVHC            0x40
#define  EZRADIOPRO_TXCORBOOSTEN       0x80

// EZRADIOPRO_VCO_CURRENT_TRIMMING                             0x5A
#define  EZRADIOPRO_VCOCUR_MASK        0x03
#define  EZRADIOPRO_VCOCORR_MASK       0x3C
#define  EZRADIOPRO_VCOCORROV          0x40
#define  EZRADIOPRO_TXCURBOOSTEN       0x80

// EZRADIOPRO_VCO_CALIBRATION_OVERRIDE                         0x5B
#define  EZRADIOPRO_VCOCAL_MASK        0x7F
#define  EZRADIOPRO_VCOCALOV           0x80  //W
#define  EZRADIOPRO_VCDONE             0x80  //R

// EZRADIOPRO_SYNTHESIZER_TEST                                 0x5C
#define  EZRADIOPRO_DSRST               0x01
#define  EZRADIOPRO_DSRSTMOD            0x02
#define  EZRADIOPRO_DSORDER_MASK        0x0C
#define  EZRADIOPRO_DSMOD               0x10
#define  EZRADIOPRO_ENOLOOP             0x20
#define  EZRADIOPRO_VCOTYPE             0x40
#define  EZRADIOPRO_DSMDT               0x80

// EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_1                          0x5D
#define  EZRADIOPRO_ENMX2              0x01
#define  EZRADIOPRO_ENBF12             0x02
#define  EZRADIOPRO_ENDV32             0x04
#define  EZRADIOPRO_ENBF5              0x08
#define  EZRADIOPRO_ENPA               0x10
#define  EZRADIOPRO_ENPGA              0x20
#define  EZRADIOPRO_ENLNA              0x40
#define  EZRADIOPRO_ENMIX              0x80

// EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_2                          0x5E
#define  EZRADIOPRO_PLLRESET           0x01
#define  EZRADIOPRO_ENBF2              0x02
#define  EZRADIOPRO_ENBF11             0x04
#define  EZRADIOPRO_ENBF3              0x08
#define  EZRADIOPRO_ENBF4              0x10
#define  EZRADIOPRO_ENMX3              0x20
#define  EZRADIOPRO_ENLDET             0x40
#define  EZRADIOPRO_ENDS               0x80

// EZRADIOPRO_BLOCK_ENABLE_OVERRIDE_3                          0x5F
#define  EZRADIOPRO_ENBG               0x01
#define  EZRADIOPRO_ENCP               0x02
#define  EZRADIOPRO_ENVCO              0x04
#define  EZRADIOPRO_DVBSHUNT           0x08
#define  EZRADIOPRO_ENDV1P5            0x10
#define  EZRADIOPRO_ENDV2              0x20
#define  EZRADIOPRO_ENDV31             0x40
#define  EZRADIOPRO_ENFRDV             0x80

// EZRADIOPRO_CHANNEL_FILTER_COEFFICIENT_ADDRESS               0x60
#define  EZRADIOPRO_CHFILADD_MASK      0x0F

// EZRADIOPRO_CHANNEL_FILTER_COEFFICIENT_VALUE                 0x61
#define  EZRADIOPRO_CHFILVAL_MASK      0x3F

// EZRADIOPRO_CRYSTAL_OSCILLATOR_CONTROL_TEST                  0x62
#define  EZRADIOPRO_ENBUF              0x01
#define  EZRADIOPRO_BUFOVR             0x02
#define  EZRADIOPRO_ENAMP2X            0x04
#define  EZRADIOPRO_ENBIAS2X           0x08
#define  EZRADIOPRO_CLKHYST            0x10
#define  EZRADIOPRO_PWST_MASK          0xE0

// EZRADIOPRO_RC_OSCILLATOR_COARSE_CALIBRATION_OVERRIDE        0x63
#define  EZRADIOPRO_RCC_MASK           0x7F
#define  EZRADIOPRO_RCCOV              0x80

// EZRADIOPRO_RC_OSCILLATOR_FINE_CALIBRATION_OVERRIDE          0x64
#define  EZRADIOPRO_RCF_MASK           0x7F
#define  EZRADIOPRO_RCFOV              0x80

// EZRADIOPRO_LDO_CONTROL_OVERRIDE   0x65
#define  EZRADIOPRO_ENDIGPWDN          0x01
#define  EZRADIOPRO_ENDIGLDO           0x02
#define  EZRADIOPRO_ENPLLLDO           0x04
#define  EZRADIOPRO_ENRFLDO            0x08
#define  EZRADIOPRO_ENIFLDO            0x10
#define  EZRADIOPRO_ENVCOLDO           0x20
#define  EZRADIOPRO_ENBIAS             0x40
#define  EZRADIOPRO_ENSPOR             0x80

// EZRADIOPRO_LDO_LEVEL_SETTING   0x66
#define  EZRADIOPRO_DIGLVL_MASK        0x07
#define  EZRADIOPRO_ENRC32             0x10
#define  EZRADIOPRO_ENTS               0x20
#define  EZRADIOPRO_ENXTAL             0x40
#define  EZRADIOPRO_ENOVR              0x80

// EZRADIOPRO_DELTASIGMA_ADC_TUNING_1                          0x67
#define  EZRADIOPRO_ADCTUNE_MASK       0x0F
#define  EZRADIOPRO_ADCTUNEOVR         0x10
#define  EZRADIOPRO_ENADC              0x20
#define  EZRADIOPRO_ENREFDAC           0x40
#define  EZRADIOPRO_ADCRST             0x80

// EZRADIOPRO_DELTASIGMA_ADC_TUNING_2                          0x68
#define  EZRADIOPRO_DSADCREF_MASK      0x07 // added DS to make unique
#define  EZRADIOPRO_ADCOLOOP           0x08
#define  EZRADIOPRO_ENVCM              0x10

// EZRADIOPRO_AGC_OVERRIDE_1                                   0x69
#define  EZRADIOPRO_PGA_MASK           0x0F
#define  EZRADIOPRO_LNAGAIN            0x10
#define  EZRADIOPRO_AGCEN              0x20

// EZRADIOPRO_AGC_OVERRIDE_2   0x6A
#define  EZRADIOPRO_PGATH_MASK         0x03
#define  EZRADIOPRO_LNACOMP_MASK       0x3F
#define  EZRADIOPRO_AGCSLOW            0x40
#define  EZRADIOPRO_AGCOVPM            0x80

// EZRADIOPRO_GFSK_FIR_FILTER_COEFFICIENT_ADDRESS              0x6B
#define  EZRADIOPRO_FIRADD_MASK        0x07

// EZRADIOPRO_GFSK_FIR_FILTER_COEFFICIENT_VALUE                0x6C
#define  EZRADIOPRO_FIRVAL_MASK        0x3F

// EZRADIOPRO_TX_POWER   0x6D
#define  EZRADIOPRO_TXPOW_MASK         0x07

// EZRADIOPRO_TX_DATA_RATE_1                                   0x6E
// no bits or mask

// EZRADIOPRO_TX_DATA_RATE_0                                   0x6F
// no bits or mask

// EZRADIOPRO_MODULATION_MODE_CONTROL_1                        0x70
#define  EZRADIOPRO_ENWHITE            0x01
#define  EZRADIOPRO_ENMANCH            0x02
#define  EZRADIOPRO_ENMANINV           0x04
#define  EZRADIOPRO_MANPPOL            0x08
#define  EZRADIOPRO_ENPHPWDN           0x10
#define  EZRADIOPRO_TXDTRTSCALE        0x20

// EZRADIOPRO_MODULATION_MODE_CONTROL_2                        0x71
#define  EZRADIOPRO_MODTYP_MASK        0x03
#define  EZRADIOPRO_FD_MASK            0x04
#define  EZRADIOPRO_ENINV              0x08
#define  EZRADIOPRO_DTMOD_MASK         0x30
#define  EZRADIOPRO_TRCLK_MASK         0xC0
#define  EZRADIOPRO_MODTYP_GFSK        0x03
#define  EZRADIOPRO_FIFO_MODE          0x20
#define  EZRADIOPRO_TX_DATA_CLK_GPIO   0x40


// EZRADIOPRO_FREQUENCY_DEVIATION                              0x72
// no bits or mask

// EZRADIOPRO_FREQUENCY_OFFSET_1                               0x73
// no bits or mask

// EZRADIOPRO_FREQUENCY_OFFSET_2                               0x74
#define  EZRADIOPRO_FO_MASK            0x03

// EZRADIOPRO_FREQUENCY_BAND_SELECTRESERVED                    0x75
#define  EZRADIOPRO_FB_MASK            0x1F
#define  EZRADIOPRO_HBSEL              0x20
#define  EZRADIOPRO_SBSEL              0x40

// EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1                      0x76
// no bits or mask

// EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0                      0x77
// no bits or mask

// EZRADIOPRO_FREQUENCY_HOPPING_CHANNEL_SELECT                 0x79
// no bits or mask

// EZRADIOPRO_FREQUENCY_HOPPING_STEP_SIZE                      0x7A
// no bits or mask

// EZRADIOPRO_TX_FIFO_CONTROL_1                                0x7C
#define  EZRADIOPRO_TXAFTHR_MASK       0x3F

// EZRADIOPRO_TX_FIFO_CONTROL_2                                0x7D
#define  EZRADIOPRO_TXAETHR_MASK       0x3F

// EZRADIOPRO_RX_FIFO_CONTROL                                  0x7E
#define  EZRADIOPRO_RXAFTHR_MASK       0x3F
#define  EZRADIOPRO_RESERVED           0x40

// EZRADIOPRO_FIFO_ACCESS                                      0x7F
// no bits or mask

//-----------------------------------------------------------------------------
// Header File PreProcessor Directive
//-----------------------------------------------------------------------------
/********************************************************************
#ifndef  PP_PHY_IF_FILTER_BANDWIDTH_H
#define  PP_PHY_IF_FILTER_BANDWIDTH_H
**********************************************************************/
//-----------------------------------------------------------------------------
#ifndef TRX_DATA_RATE
#error  "TRX_DATA_RATE must be defined before including this file"
#endif
#ifndef TRX_DEVIATION
#error  "TRX_DEVIATION must be defined before including this file"
#endif
#ifndef RX_BANDWIDTH
#error  "RX_BANDWIDTH must be defined before including this file"
#endif
//-----------------------------------------------------------------------------
// First calculate the MODULATION_INDEX
//-----------------------------------------------------------------------------
#define MODULATION_INDEX ((TRX_DEVIATION*2)/TRX_DATA_RATE)
//-----------------------------------------------------------------------------
// Optional macros for calulating RX_BANDWIDTH based on XTAL_TOLERANCE
//
// These macros have been depreciated. Please use the Excel calculator to
// determine the optimium RX bandwith for a particular application.
//-----------------------------------------------------------------------------
//#define IDEAL_BANDWIDTH (TRX_DATA_RATE + (2*TRX_DEVIATION))
//#define FREQUENCY_ERROR (TRX_FREQUENCY/1000*XTAL_TOLERANCE*4/1000)
//#if(FREQUENCY_ERROR>IDEAL_BANDWIDTH)
//#define RX_BANDWIDTH FREQUENCY_ERROR
//#else
//#define RX_BANDWIDTH IDEAL_BANDWIDTH
//#endif
//-----------------------------------------------------------------------------
// MODULATION_INDEX < 2 (Narrowband)
//-----------------------------------------------------------------------------
#if (MODULATION_INDEX<2)
#if (RX_BANDWIDTH<2600)
#define IF_FILTER_BANDWIDTH 0x51
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<2800)
#define IF_FILTER_BANDWIDTH 0x52
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3100)
#define IF_FILTER_BANDWIDTH 0x53
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3200)
#define IF_FILTER_BANDWIDTH 0x54
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3700)
#define IF_FILTER_BANDWIDTH 0x55
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<4900)
#define IF_FILTER_BANDWIDTH 0x41
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5400)
#define IF_FILTER_BANDWIDTH 0x42
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5900)
#define IF_FILTER_BANDWIDTH 0x43
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<6100)
#define IF_FILTER_BANDWIDTH 0x44
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<7200)
#define IF_FILTER_BANDWIDTH 0x45
#define AFC_LIMIT 0x03
#elif (RX_BANDWIDTH<9500)
#define IF_FILTER_BANDWIDTH 0x31
#define AFC_LIMIT 0x03
#elif (RX_BANDWIDTH<10600)
#define IF_FILTER_BANDWIDTH 0x32
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<11500)
#define IF_FILTER_BANDWIDTH 0x33
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<12100)
#define IF_FILTER_BANDWIDTH 0x34
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<14200)
#define IF_FILTER_BANDWIDTH 0x35
#define AFC_LIMIT 0x05
#elif (RX_BANDWIDTH<16200)
#define IF_FILTER_BANDWIDTH 0x36
#define AFC_LIMIT 0x06
#elif (RX_BANDWIDTH<17500)
#define IF_FILTER_BANDWIDTH 0x3A
#define AFC_LIMIT 0x06
#elif (RX_BANDWIDTH<19400)
#define IF_FILTER_BANDWIDTH 0x3B
#define AFC_LIMIT 0x07
#elif (RX_BANDWIDTH<21400)
#define IF_FILTER_BANDWIDTH 0x3C
#define AFC_LIMIT 0x08
#elif (RX_BANDWIDTH<23900)
#define IF_FILTER_BANDWIDTH 0x3D
#define AFC_LIMIT 0x08
#elif (RX_BANDWIDTH<25700)
#define IF_FILTER_BANDWIDTH 0x3E
#define AFC_LIMIT 0x09
#elif (RX_BANDWIDTH<28200)
#define IF_FILTER_BANDWIDTH 0x25
#define AFC_LIMIT 0x0A
#elif (RX_BANDWIDTH<32200)
#define IF_FILTER_BANDWIDTH 0x26
#define AFC_LIMIT 0x0B
#elif (RX_BANDWIDTH<34700)
#define IF_FILTER_BANDWIDTH 0x27
#define AFC_LIMIT 0x0C
#elif (RX_BANDWIDTH<38600)
#define IF_FILTER_BANDWIDTH 0x2B
#define AFC_LIMIT 0x0E
#elif (RX_BANDWIDTH<42700)
#define IF_FILTER_BANDWIDTH 0x2C
#define AFC_LIMIT 0x0F
#elif (RX_BANDWIDTH<47700)
#define IF_FILTER_BANDWIDTH 0x2D
#define AFC_LIMIT 0x11
#elif (RX_BANDWIDTH<51200)
#define IF_FILTER_BANDWIDTH 0x2E
#define AFC_LIMIT 0x12
#elif (RX_BANDWIDTH<56200)
#define IF_FILTER_BANDWIDTH 0x15
#define AFC_LIMIT 0x14
#elif (RX_BANDWIDTH<64100)
#define IF_FILTER_BANDWIDTH 0x16
#define AFC_LIMIT 0x17
#elif (RX_BANDWIDTH<69200)
#define IF_FILTER_BANDWIDTH 0x17
#define AFC_LIMIT 0x19
#elif (RX_BANDWIDTH<75200)
#define IF_FILTER_BANDWIDTH 0x01
#define AFC_LIMIT 0x1B
#elif (RX_BANDWIDTH<83200)
#define IF_FILTER_BANDWIDTH 0x02
#define AFC_LIMIT 0x1E
#elif (RX_BANDWIDTH<90000)
#define IF_FILTER_BANDWIDTH 0x03
#define AFC_LIMIT 0x20
#elif (RX_BANDWIDTH<95300)
#define IF_FILTER_BANDWIDTH 0x04
#define AFC_LIMIT 0x22
#elif (RX_BANDWIDTH<112100)
#define IF_FILTER_BANDWIDTH 0x05
#define AFC_LIMIT 0x28
#elif (RX_BANDWIDTH<127900)
#define IF_FILTER_BANDWIDTH 0x06
#define AFC_LIMIT 0x2D
#elif (RX_BANDWIDTH<137900)
#define IF_FILTER_BANDWIDTH 0x07
#define AFC_LIMIT 0x31
#elif (RX_BANDWIDTH<142800)
#define IF_FILTER_BANDWIDTH 0x94
#define AFC_LIMIT 0x33
#elif (RX_BANDWIDTH<167800)
#define IF_FILTER_BANDWIDTH 0x95
#define AFC_LIMIT 0x3C
#elif (RX_BANDWIDTH<181100)
#define IF_FILTER_BANDWIDTH 0x99
#define AFC_LIMIT 0x40
#elif (RX_BANDWIDTH<191500)
#define IF_FILTER_BANDWIDTH 0x96
#define AFC_LIMIT 0x44
#elif (RX_BANDWIDTH<208400)
#define IF_FILTER_BANDWIDTH 0x9A
#define AFC_LIMIT 0x48
#elif (RX_BANDWIDTH<225100)
#define IF_FILTER_BANDWIDTH 0x81
#define AFC_LIMIT 0x50
#elif (RX_BANDWIDTH<248800)
#define IF_FILTER_BANDWIDTH 0x82
#define AFC_LIMIT 0x58
#elif (RX_BANDWIDTH<269300)
#define IF_FILTER_BANDWIDTH 0x83
#define AFC_LIMIT 0x60
#elif (RX_BANDWIDTH<284900)
#define IF_FILTER_BANDWIDTH 0x84
#define AFC_LIMIT 0x65
#elif (RX_BANDWIDTH<335500)
#define IF_FILTER_BANDWIDTH 0x88
#define AFC_LIMIT 0x77
#elif (RX_BANDWIDTH<361800)
#define IF_FILTER_BANDWIDTH 0x89
#define AFC_LIMIT 0x81
#elif (RX_BANDWIDTH<420200)
#define IF_FILTER_BANDWIDTH 0x8A
#define AFC_LIMIT 0x95
#elif (RX_BANDWIDTH<468400)
#define IF_FILTER_BANDWIDTH 0x8B
#define AFC_LIMIT 0xA7
#elif (RX_BANDWIDTH<518800)
#define IF_FILTER_BANDWIDTH 0x8C
#define AFC_LIMIT 0xB8
#elif (RX_BANDWIDTH<577000)
#define IF_FILTER_BANDWIDTH 0x8D
#define AFC_LIMIT 0xCD
#else
#define IF_FILTER_BANDWIDTH 0x8E
#define AFC_LIMIT 0xDD
#endif
//-----------------------------------------------------------------------------
// MODULATION_INDEX < 10
//-----------------------------------------------------------------------------
#elif (MODULATION_INDEX<10)
#if (RX_BANDWIDTH<2600)
#define IF_FILTER_BANDWIDTH 0x51
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<2800)
#define IF_FILTER_BANDWIDTH 0x52
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3100)
#define IF_FILTER_BANDWIDTH 0x53
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3200)
#define IF_FILTER_BANDWIDTH 0x54
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3700)
#define IF_FILTER_BANDWIDTH 0x55
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<4900)
#define IF_FILTER_BANDWIDTH 0x41
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5400)
#define IF_FILTER_BANDWIDTH 0x42
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5900)
#define IF_FILTER_BANDWIDTH 0x43
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<6100)
#define IF_FILTER_BANDWIDTH 0x44
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<7200)
#define IF_FILTER_BANDWIDTH 0x45
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<9500)
#define IF_FILTER_BANDWIDTH 0x31
#define AFC_LIMIT 0x03
#elif (RX_BANDWIDTH<10600)
#define IF_FILTER_BANDWIDTH 0x32
#define AFC_LIMIT 0x03
#elif (RX_BANDWIDTH<11500)
#define IF_FILTER_BANDWIDTH 0x33
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<12100)
#define IF_FILTER_BANDWIDTH 0x34
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<14200)
#define IF_FILTER_BANDWIDTH 0x35
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<16200)
#define IF_FILTER_BANDWIDTH 0x36
#define AFC_LIMIT 0x05
#elif (RX_BANDWIDTH<17500)
#define IF_FILTER_BANDWIDTH 0x37
#define AFC_LIMIT 0x05
#elif (RX_BANDWIDTH<18900)
#define IF_FILTER_BANDWIDTH 0x21
#define AFC_LIMIT 0x06
#elif (RX_BANDWIDTH<21600)
#define IF_FILTER_BANDWIDTH 0x22
#define AFC_LIMIT 0x07
#elif (RX_BANDWIDTH<22700)
#define IF_FILTER_BANDWIDTH 0x23
#define AFC_LIMIT 0x07
#elif (RX_BANDWIDTH<24000)
#define IF_FILTER_BANDWIDTH 0x24
#define AFC_LIMIT 0x07
#elif (RX_BANDWIDTH<28200)
#define IF_FILTER_BANDWIDTH 0x2A
#define AFC_LIMIT 0x09
#elif (RX_BANDWIDTH<31900)
#define IF_FILTER_BANDWIDTH 0x1F
#define AFC_LIMIT 0x0A
#elif (RX_BANDWIDTH<34700)
#define IF_FILTER_BANDWIDTH 0x27
#define AFC_LIMIT 0x0B
#elif (RX_BANDWIDTH<38600)
#define IF_FILTER_BANDWIDTH 0x2B
#define AFC_LIMIT 0x0C
#elif (RX_BANDWIDTH<42700)
#define IF_FILTER_BANDWIDTH 0x2C
#define AFC_LIMIT 0x0D
#elif (RX_BANDWIDTH<47700)
#define IF_FILTER_BANDWIDTH 0x2D
#define AFC_LIMIT 0x0F
#elif (RX_BANDWIDTH<51200)
#define IF_FILTER_BANDWIDTH 0x2E
#define AFC_LIMIT 0x10
#elif (RX_BANDWIDTH<56200)
#define IF_FILTER_BANDWIDTH 0x15
#define AFC_LIMIT 0x11
#elif (RX_BANDWIDTH<64100)
#define IF_FILTER_BANDWIDTH 0x16
#define AFC_LIMIT 0x14
#elif (RX_BANDWIDTH<69200)
#define IF_FILTER_BANDWIDTH 0x17
#define AFC_LIMIT 0x16
#elif (RX_BANDWIDTH<75200)
#define IF_FILTER_BANDWIDTH 0x01
#define AFC_LIMIT 0x17
#elif (RX_BANDWIDTH<83200)
#define IF_FILTER_BANDWIDTH 0x02
#define AFC_LIMIT 0x1A
#elif (RX_BANDWIDTH<90000)
#define IF_FILTER_BANDWIDTH 0x03
#define AFC_LIMIT 0x1C
#elif (RX_BANDWIDTH<95300)
#define IF_FILTER_BANDWIDTH 0x04
#define AFC_LIMIT 0x1E
#elif (RX_BANDWIDTH<112100)
#define IF_FILTER_BANDWIDTH 0x05
#define AFC_LIMIT 0x23
#elif (RX_BANDWIDTH<127900)
#define IF_FILTER_BANDWIDTH 0x06
#define AFC_LIMIT 0x28
#elif (RX_BANDWIDTH<137900)
#define IF_FILTER_BANDWIDTH 0x07
#define AFC_LIMIT 0x2B
#elif (RX_BANDWIDTH<138700)
#define IF_FILTER_BANDWIDTH 0x0A
#define AFC_LIMIT 0x2B
#elif (RX_BANDWIDTH<154200)
#define IF_FILTER_BANDWIDTH 0x0B
#define AFC_LIMIT 0x30
#elif (RX_BANDWIDTH<168000)
#define IF_FILTER_BANDWIDTH 0x98
#define AFC_LIMIT 0x34
#elif (RX_BANDWIDTH<181100)
#define IF_FILTER_BANDWIDTH 0x99
#define AFC_LIMIT 0x38
#elif (RX_BANDWIDTH<208400)
#define IF_FILTER_BANDWIDTH 0x9A
#define AFC_LIMIT 0x41
#elif (RX_BANDWIDTH<232000)
#define IF_FILTER_BANDWIDTH 0x9B
#define AFC_LIMIT 0x48
#elif (RX_BANDWIDTH<256000)
#define IF_FILTER_BANDWIDTH 0x9C
#define AFC_LIMIT 0x50
#elif (RX_BANDWIDTH<269300)
#define IF_FILTER_BANDWIDTH 0x83
#define AFC_LIMIT 0x54
#elif (RX_BANDWIDTH<284900)
#define IF_FILTER_BANDWIDTH 0x84
#define AFC_LIMIT 0x59
#elif (RX_BANDWIDTH<335500)
#define IF_FILTER_BANDWIDTH 0x88
#define AFC_LIMIT 0x68
#elif (RX_BANDWIDTH<361800)
#define IF_FILTER_BANDWIDTH 0x89
#define AFC_LIMIT 0x71
#elif (RX_BANDWIDTH<420200)
#define IF_FILTER_BANDWIDTH 0x8A
#define AFC_LIMIT 0x83
#elif (RX_BANDWIDTH<468400)
#define IF_FILTER_BANDWIDTH 0x8B
#define AFC_LIMIT 0x92
#elif (RX_BANDWIDTH<518800)
#define IF_FILTER_BANDWIDTH 0x8C
#define AFC_LIMIT 0xA1
#elif (RX_BANDWIDTH<577000)
#define IF_FILTER_BANDWIDTH 0x8D
#define AFC_LIMIT 0xB4
#else
#define IF_FILTER_BANDWIDTH 0x8E
#define AFC_LIMIT 0xC1
#endif
//-----------------------------------------------------------------------------
// MODULATION_INDEX > 10
//-----------------------------------------------------------------------------
#else
#if (RX_BANDWIDTH<2600)
#define IF_FILTER_BANDWIDTH 0x51
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<2800)
#define IF_FILTER_BANDWIDTH 0x52
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3100)
#define IF_FILTER_BANDWIDTH 0x53
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3200)
#define IF_FILTER_BANDWIDTH 0x54
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<3700)
#define IF_FILTER_BANDWIDTH 0x55
#define AFC_LIMIT 0x01
#elif (RX_BANDWIDTH<4900)
#define IF_FILTER_BANDWIDTH 0x41
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5400)
#define IF_FILTER_BANDWIDTH 0x42
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<5900)
#define IF_FILTER_BANDWIDTH 0x43
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<6100)
#define IF_FILTER_BANDWIDTH 0x44
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<7200)
#define IF_FILTER_BANDWIDTH 0x45
#define AFC_LIMIT 0x02
#elif (RX_BANDWIDTH<9500)
#define IF_FILTER_BANDWIDTH 0x31
#define AFC_LIMIT 0x03
#elif (RX_BANDWIDTH<10600)
#define IF_FILTER_BANDWIDTH 0x32
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<11500)
#define IF_FILTER_BANDWIDTH 0x33
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<12100)
#define IF_FILTER_BANDWIDTH 0x34
#define AFC_LIMIT 0x04
#elif (RX_BANDWIDTH<14200)
#define IF_FILTER_BANDWIDTH 0x35
#define AFC_LIMIT 0x05
#elif (RX_BANDWIDTH<16200)
#define IF_FILTER_BANDWIDTH 0x36
#define AFC_LIMIT 0x06
#elif (RX_BANDWIDTH<17500)
#define IF_FILTER_BANDWIDTH 0x3A
#define AFC_LIMIT 0x06
#elif (RX_BANDWIDTH<19400)
#define IF_FILTER_BANDWIDTH 0x3B
#define AFC_LIMIT 0x07
#elif (RX_BANDWIDTH<21400)
#define IF_FILTER_BANDWIDTH 0x3C
#define AFC_LIMIT 0x08
#elif (RX_BANDWIDTH<23900)
#define IF_FILTER_BANDWIDTH 0x3D
#define AFC_LIMIT 0x08
#elif (RX_BANDWIDTH<25700)
#define IF_FILTER_BANDWIDTH 0x3E
#define AFC_LIMIT 0x09
#elif (RX_BANDWIDTH<28200)
#define IF_FILTER_BANDWIDTH 0x25
#define AFC_LIMIT 0x0A
#elif (RX_BANDWIDTH<32200)
#define IF_FILTER_BANDWIDTH 0x26
#define AFC_LIMIT 0x0B
#elif (RX_BANDWIDTH<34700)
#define IF_FILTER_BANDWIDTH 0x27
#define AFC_LIMIT 0x0C
#elif (RX_BANDWIDTH<38600)
#define IF_FILTER_BANDWIDTH 0x2B
#define AFC_LIMIT 0x0E
#elif (RX_BANDWIDTH<42700)
#define IF_FILTER_BANDWIDTH 0x2C
#define AFC_LIMIT 0x0F
#elif (RX_BANDWIDTH<47700)
#define IF_FILTER_BANDWIDTH 0x2D
#define AFC_LIMIT 0x0F
#elif (RX_BANDWIDTH<51200)
#define IF_FILTER_BANDWIDTH 0x2E
#define AFC_LIMIT 0x10
#elif (RX_BANDWIDTH<56200)
#define IF_FILTER_BANDWIDTH 0x15
#define AFC_LIMIT 0x11
#elif (RX_BANDWIDTH<64100)
#define IF_FILTER_BANDWIDTH 0x16
#define AFC_LIMIT 0x14
#elif (RX_BANDWIDTH<69400)
#define IF_FILTER_BANDWIDTH 0x1A
#define AFC_LIMIT 0x16
#elif (RX_BANDWIDTH<77100)
#define IF_FILTER_BANDWIDTH 0x1B
#define AFC_LIMIT 0x18
#elif (RX_BANDWIDTH<85100)
#define IF_FILTER_BANDWIDTH 0x1C
#define AFC_LIMIT 0x1A
#elif (RX_BANDWIDTH<95300)
#define IF_FILTER_BANDWIDTH 0x1D
#define AFC_LIMIT 0x1E
#elif (RX_BANDWIDTH<102200)
#define IF_FILTER_BANDWIDTH 0x1E
#define AFC_LIMIT 0x20
#elif (RX_BANDWIDTH<115600)
#define IF_FILTER_BANDWIDTH 0xAB
#define AFC_LIMIT 0x24
#elif (RX_BANDWIDTH<127700)
#define IF_FILTER_BANDWIDTH 0xAC
#define AFC_LIMIT 0x28
#elif (RX_BANDWIDTH<142800)
#define IF_FILTER_BANDWIDTH 0xAD
#define AFC_LIMIT 0x2C
#elif (RX_BANDWIDTH<153300)
#define IF_FILTER_BANDWIDTH 0xAE
#define AFC_LIMIT 0x30
#elif (RX_BANDWIDTH<168000)
#define IF_FILTER_BANDWIDTH 0x98
#define AFC_LIMIT 0x34
#elif (RX_BANDWIDTH<181100)
#define IF_FILTER_BANDWIDTH 0x99
#define AFC_LIMIT 0x38
#elif (RX_BANDWIDTH<208400)
#define IF_FILTER_BANDWIDTH 0x9A
#define AFC_LIMIT 0x41
#elif (RX_BANDWIDTH<232000)
#define IF_FILTER_BANDWIDTH 0x9B
#define AFC_LIMIT 0x48
#elif (RX_BANDWIDTH<256000)
#define IF_FILTER_BANDWIDTH 0x9C
#define AFC_LIMIT 0x50
#elif (RX_BANDWIDTH<269300)
#define IF_FILTER_BANDWIDTH 0x83
#define AFC_LIMIT 0x54
#elif (RX_BANDWIDTH<284900)
#define IF_FILTER_BANDWIDTH 0x84
#define AFC_LIMIT 0x59
#elif (RX_BANDWIDTH<335500)
#define IF_FILTER_BANDWIDTH 0x88
#define AFC_LIMIT 0x68
#elif (RX_BANDWIDTH<361800)
#define IF_FILTER_BANDWIDTH 0x89
#define AFC_LIMIT 0x71
#elif (RX_BANDWIDTH<420200)
#define IF_FILTER_BANDWIDTH 0x8A
#define AFC_LIMIT 0x83
#elif (RX_BANDWIDTH<468400)
#define IF_FILTER_BANDWIDTH 0x8B
#define AFC_LIMIT 0x92
#elif (RX_BANDWIDTH<518800)
#define IF_FILTER_BANDWIDTH 0x8C
#define AFC_LIMIT 0xA1
#elif (RX_BANDWIDTH<577000)
#define IF_FILTER_BANDWIDTH 0x8D
#define AFC_LIMIT 0xB4
#else
#define IF_FILTER_BANDWIDTH 0x8E
#define AFC_LIMIT 0xC1
#endif
#endif
//-----------------------------------------------------------------------------
// define RX_DWN3_BYP & RX_NDEC_EXP based on IF_FILTER_BANDWIDTH from Table
//-----------------------------------------------------------------------------
#define RX_DWN3_BYP  (IF_FILTER_BANDWIDTH>>7)
#define RX_NDEC_EXP  ((IF_FILTER_BANDWIDTH>>4)&0x03)
//=============================================================================
extern bit RxPacketReceived;
extern unsigned char data RxPacketLength;
extern unsigned char xdata RxErrors;
//-----------------------------------------------------------------------------
// Public function prototypes (API)
//-----------------------------------------------------------------------------
SI4432_STATUS  ppPhyInit (void);
SI4432_STATUS  ppPhyInitRadio (void);
SI4432_STATUS  ppPhyStandby (void);
SI4432_STATUS  ppPhyIdle (void);
SI4432_STATUS  ppPhyShutDown (void);
SI4432_STATUS  ppPhyReStart (void);
SI4432_STATUS  ppPhyTx (U8, VARIABLE_SEGMENT_POINTER(txBuffer, U8, BUFFER_MSPACE));
SI4432_STATUS  ppPhyRxOn (void);
SI4432_STATUS  ppPhyRxOff (void);
SI4432_STATUS  ppPhyGetRxPacket (U8, VARIABLE_SEGMENT_POINTER(rxBuffer, U8, BUFFER_MSPACE));
//-----------------------------------------------------------------------------
// Phy primitive readn and write - use with caution
//-----------------------------------------------------------------------------
void  phyWrite (U8, U8);
U8    phyRead (U8);
void  burst_phyWrite (U8, U8);
U8    burst_phyRead (U8);
void  phyWriteFIFO (U8, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE));
void  phyReadFIFO (U8, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE));
unsigned char SPI_Read(void);
void SPI_Write(unsigned char txdata);
U8   RxIntPhyRead (U8);
void  RxIntPhyWrite (U8, U8);
void  RxIntphyReadFIFO (U8, VARIABLE_SEGMENT_POINTER(buffer, U8, BUFFER_MSPACE));
// End
//=============================================================================
#endif