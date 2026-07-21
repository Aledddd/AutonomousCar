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
    void initialization();
    void configuration();

    float* getData() { return data; };
    float data[9];
    
private:
    int fd = 0;
    std::string device = "/dev/i2c-5";
    unsigned long mode = I2C_SLAVE;

    const static int size = 38;
    uint8_t bno055_data[size]; 
    
    template<typename E>
    uint8_t to_u8(E e)
    {
        return static_cast<uint8_t>(e);
    }
    
    int16_t convert_to_i16(int msb = 0, int lsb = 0);
    
    void getRawData();
    void getReadableData();
};
