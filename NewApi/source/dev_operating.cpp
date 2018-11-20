#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//#include <termios.h>
//#include <linux/serial.h>
//#include <sys/ioctl.h>
#include <sys/time.h>
#include "dev_operating.h"

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID		0

// 字符串对比的返回值
#define NOT_THIS    1 // 1Mbps
#define THIS		0

#define CURRENT_ID          0x03
#define RETURN_DELAY        0x05
#define BLINK_CONDITIONS    0x11
#define SHUTDOWN_CONDITIONS 0x12
#define TORQUE              0x22
#define TORQUE_ENABLE       0x18
#define MAX_SPEED           0x20
#define CURRENT_SPEED       0x26
#define GOAL_ANGLE          0x1e
#define CURRENT_ANGLE       0x24
/**/
#define REC_BUFFER_LEN 32
#define SERVO_MAX_PARAMS (REC_BUFFER_LEN - 5)

#define REC_WAIT_START_US    75
#define REC_WAIT_PARAMS_US   (SERVO_MAX_PARAMS * 5)
#define REC_WAIT_MAX_RETRIES 200

#define SERVO_INSTRUCTION_ERROR   (1 << 6)
#define SERVO_OVERLOAD_ERROR      (1 << 5)
#define SERVO_CHECKSUM_ERROR      (1 << 4)
#define SERVO_RANGE_ERROR         (1 << 3)
#define SERVO_OVERHEAT_ERROR      (1 << 2)
#define SERVO_ANGLE_LIMIT_ERROR   (1 << 1)
#define SERVO_INPUT_VOLTAGE_ERROR (1)

dev_operating::~dev_operating()
{

}
dev_operating::dev_operating(void)
{
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
}

int dev_operating::dxl_initialize(int baudnum)
{
    float baudrate;
    baudrate = 2000000.0f / (float)(baudnum + 1);
    if (portHandler->openPort())
    {
        printf("Succeeded to open the port!\n");
        if (portHandler->setBaudRate(baudrate))
        {
          printf("Succeeded to change the baudrate!\n");
          return 1;
        }
        else
        {
          printf("Failed to change the baudrate!\n");
          return 0;
        }
    }
    else
    {
        printf("Failed to open the port!\n");
        return 0;
    }
}

int dev_operating::dxl_read_byte(int id, int addr)
{
    uint8_t *get_data;
    int dxl_comm_result;
    dxl_comm_result = packetHandler->read1ByteTxRx(portHandler, id, addr, get_data, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
      printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
      return -1;
    }
    else if (dxl_error != 0)
    {
      printf("%s\n", packetHandler->getRxPacketError(dxl_error));
      return -1;
    }
    return *get_data;
}

int dev_operating::dxl_read_2byte(int id, int addr)
{
    uint16_t *get_data;
    int dxl_comm_result;
    dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, id, addr, get_data, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
      printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
      return *get_data;
    }
    else if (dxl_error != 0)
    {
      printf("%s\n", packetHandler->getRxPacketError(dxl_error));
      return -1;
    }
}

int dev_operating::dxl_write_byte(int id, int address, int value)
{
    int dxl_comm_result;
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, address, value ,&dxl_error);
    return dxl_comm_result;
}

int dev_operating::dxl_write_2byte(int id, int address, int value)
{
    int dxl_comm_result;
    dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, id, address, value ,&dxl_error);
    return dxl_comm_result;
}

/*舵机通讯初始化：初始化串口*/
int dev_operating::AX_UART_Init(void)
{
    dxl_initialize(DEFAULT_BAUDNUM);
}

// valid torque values are from 0 (free running) to 1023 (max)
void dev_operating::setServoTorque (const unsigned char servoId,
                     const unsigned int torqueValue)
{
    packetHandler->write2ByteTxOnly(portHandler, servoId, TORQUE_ENABLE, torqueValue);
}

// speed values go from 1 (incredibly slow) to 1023 (114 RPM)
// a value of zero will disable velocity control
void dev_operating::setServoSpeed (const unsigned char servoId,
                       const unsigned int speedValue)
{
    packetHandler->write2ByteTxOnly(portHandler, servoId, SPEED, speedValue);
}

// make the servo move to an angle
// valid angles are between 0 and 300 degrees
void dev_operating::setServoAngle (const unsigned char servoId,
                    const float angle)
{
    // angle values go from 0 to 0x3ff (1023)
    const unsigned int angleValue = (unsigned int)(angle * (1023.0 / 300.0));
    packetHandler->write2ByteTxOnly(portHandler, servoId, GOAL_ANGLE, angleValue);
}

void dev_operating::setServoClockwiseLimited (const unsigned char servoId,
                       const unsigned int angleValue)
{
    if((angleValue<0)||(angleValue>1023))
        printf("Not a proper value.");
    else
        packetHandler->write2ByteTxOnly(portHandler, servoId, ANGLE_LIMITED_CLOCKWISE, angleValue);
}

void dev_operating::setServoAntiClockwiseLimited (const unsigned char servoId,
                       const unsigned int angleValue)
{
    if((angleValue<0)||(angleValue>1023))
        printf("Not a proper value.");
    else
        packetHandler->write2ByteTxOnly(portHandler, servoId, ANGLE_LIMITED_ANTICLOCKWISE, angleValue);
}
