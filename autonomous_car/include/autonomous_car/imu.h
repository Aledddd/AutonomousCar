#include <linux/i2c-dev.h> // I2C_SLAVE
#include <sys/ioctl.h> // ioctl
#include <unistd.h> // write, read, ...
#include <fcntl.h> // O_RDWR, ...
#include <errno.h>

#include <iostream>
#include <cstdbool>

#include "bno055_address.h"

class IMU {
public:
    IMU();
    void initialization();

private:
    int fd = 0;
    std::string device = "/dev/i2c-5";
    unsigned long mode = I2C_SLAVE;
    
    uint8_t bno055_data[12];
    float acc_data[3];
    float gyr_data[3];

    template<typename E>
    uint8_t to_u8(E e)
    {
        return static_cast<uint8_t>(e);
    };

    void configuration(int fd);
    int16_t convert_to_i16(int msb = 0, int lsb = 0);

    void getAccelerationRawData();
    void getGyroscopeRawData();
    void getReadableData();
};
