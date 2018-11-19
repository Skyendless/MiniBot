#ifndef DEV_OPERATING_H
#define DEV_OPERATING_H

#define ID					(2)
#define LENGTH				(3)
#define INSTRUCTION			(4)
#define ERRBIT				(4)
#define PARAMETER			(5)
#define DEFAULT_BAUDNUMBER	(1)

// Protocol version(通信协议)
#define PROTOCOL_VERSION                1.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define DXL_ID                          1                   // Dynamixel ID: 1
#define BAUDRATE                        1000000
#define DEVICENAME                      "/dev/ttyUSB0"      // Check which port is being used on your controller

// 字符串对比的返回值
#define NOT_THIS    1 // 1Mbps
#define THIS		0

//控制表
#define CURRENT_ID                   0x03
#define RETURN_DELAY                 0x05
#define BLINK_CONDITIONS             0x11
#define SHUTDOWN_CONDITIONS          0x12
#define TORQUE                       0x22
#define SPEED                        0x20
#define CURRENT_SPEED                0x26
#define GOAL_ANGLE                   0x1e
#define CURRENT_ANGLE                0x24
#define ANGLE_LIMITED_CLOCKWISE      0x06
#define ANGLE_LIMITED_ANTICLOCKWISE  0x08

#include "dynamixel_sdk.h"
#include <stdint.h>

class dev_operating
{
    public:
        dev_operating();
        virtual ~dev_operating();
        dynamixel::PortHandler *portHandler;//端口类指针
        dynamixel::PacketHandler *packetHandler;//数据包处理类指针

        /*舵机操作方法*/
        unsigned char dxl_error;//错误提示变量
        int dxl_initialize(int baudnum);
        int dxl_read_byte(int id, int addr);
        int dxl_read_2byte(int id, int addr);
        int dxl_write_byte(int id, int address, int value);
        int dxl_write_2byte(int id, int address, int value);

        /*下面四个函数是直接用来操作的*/
        int AX_UART_Init(void);
        void setServoTorque (const unsigned char servoId,
                             const unsigned int torqueValue);
        void setServoClockwiseLimited (const unsigned char servoId,
                               const unsigned int angleValue);
        void setServoAntiClockwiseLimited (const unsigned char servoId,
                               const unsigned int angleValue);
        void setServoSpeed (const unsigned char servoId,
                               const unsigned int speedValue);
        void setServoAngle (const unsigned char servoId,
                            const float angle);
        /**/
    protected:

    private:


};

#endif // MYDYNAMIXEL_H
