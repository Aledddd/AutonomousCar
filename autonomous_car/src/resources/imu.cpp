#include "imu.h"

void IMU::initialization(void)
{
    fd = open(device.c_str(),O_RDWR);
    if(fd < 0)
    {
            std::cerr << "Failed to open I2C BUS : " << device << std::endl;
            return;
    }
    std::cout << "Opened !" << std::endl; 
    std::cout << "Setting up I2C Mode for " << reinterpret_cast<void*>(BNO055_I2C_ADDR_LO) << std::endl;

    if(ioctl(fd, I2C_SLAVE, BNO055_I2C_ADDR_LO) < 0)
    {
            std::cerr << "Failed to set I2C Mode (Master/Slave) !" << std::endl;
            close(fd);
            return;
    }
    std::cout << "Set up to I2C Mode !" << std::endl << "Configurating device..." << std::endl;
    configuration(fd);
    if(fcntl(fd, F_GETFL) == -1 && errno == EBADF) {std::cout << "Configuration failed because the file was closed !" << std::endl;};
}


void IMU::configuration()
{
    bool config = 0;
    // Configuration
    uint8_t opr_config[2] = {BNO055_OPR_MODE, to_u8(OPRMode::CONFIGMODE)};

    // Select config mode
    if (write(fd, opr_config, 2) != 2) {
        close(fd);
        std::cerr << "Failed to write to the i2c bus" << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);

    /* There's no need to setup sensors as it is done automatically
       by BNO055 microcontroller in NDOF MODE /!\
       Only one thing to setup : G Range for Accelerometer */

    // Select Page 1 to configure sensors
    uint8_t conf_page1[2] = {BNO055_PAGE_ID, 0x01};
    if (write(fd, conf_page1, 2) != 2)
    {
        close(fd);
        std::cerr << "Failed to write Page 1 Selection to the i2c bus" << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);

    // Full scale acceleration
    uint8_t acc_config[2] = {BNO055_ACC_CONFIG, to_u8(Ascale::AFS_4G)};
    if (write(fd, acc_config, 2) != 2)
    {
        close(fd);
        std::cerr << "Failed to write Accelerometer Configuration to the i2c bus " << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);

    //Selet Page 0
    uint8_t conf_page0[2] = {BNO055_PAGE_ID, 0x00};
    if (write(fd, conf_page0, 2) != 2)
    {
        close(fd);
        std::cerr << "Failed to write Page 0 Selection to the i2c bus" << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);

    //Select BNO055 system power mode (Page 0; 0x3E)
    uint8_t pwr_mode[2] = {BNO055_PWR_MODE, to_u8(PWRMode::Normalpwr)};
    if (write(fd, pwr_mode, 2) != 2)
    {
        close(fd);
        std::cerr << "Failed to write Power Mode to the i2c bus" << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);

    //Select BNO055 system operation mode (Page 0; 0x3D)
    uint8_t opr_mode[2] = {BNO055_OPR_MODE, to_u8(OPRMode::NDOF)};
    if (write(fd, opr_mode, 2) != 2)
    {
        close(fd);
        std::cerr << "Failed to write Operation Mode to the i2c bus" << std::endl;
        return;
    }
    //Wait for 10 ms to continue
    usleep(10000);
    std::cout << "Configuration Done, BNO055 up and running !" << std::endl;
}


void IMU::getRawData()
{
    uint8_t register = BNO055_ACC_DATA_X_LSB;
    if(write(fd, &reg, 1) != 1)
    {
        close(fd);
        exit(-1);
    }
    if(read(fd, bno055_data, 18) != 18)
    {
        close(fd);
        exit(-1);
    }
}

int16_t IMU::convert_to_i16(int msb, int lsb)
{
    return (((int16_t)((uint8_t *)(bno055_data))[lsb] << 8) | 
                      ((uint8_t *)(bno055_data))[msb]);
}


void IMU::getReadableData()
{
    acc_data[0] = ((float)convert_to_i16(1,0))/100.0f;
    acc_data[1] = ((float)convert_to_i16(3,2))/100.0f;
    acc_data[2] = ((float)convert_to_i16(5,4))/100.0f;

    gyr_data[0] = ((float)convert_to_i16(11,12))/16.0f;
    gyr_data[1] = ((float)convert_to_i16(13,14))/16.0f;
    gyr_data[2] = ((float)convert_to_i16(15,16))/16.0f;
}
