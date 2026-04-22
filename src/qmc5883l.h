#pragma once


#include "Wire.h"


#define QMC5883L_I2C_ADR            0x0D  

// Register Map
// Data Output Registers
#define QMC5883L_OUT_X_LSB_REG      0x00  
#define QMC5883L_OUT_X_MSB_REG      0x01  
#define QMC5883L_OUT_Y_LSB_REG      0x02  
#define QMC5883L_OUT_Y_MSB_REG      0x03  
#define QMC5883L_OUT_Z_LSB_REG      0x04  
#define QMC5883L_OUT_Z_MSB_REG      0x05  

// Status Register
#define QMC5883L_STATUS_REG         0x06  

// Temperature Output Registers
#define QMC5883L_TEMP_LSB_REG       0x07  
#define QMC5883L_TEMP_MSB_REG       0x08  

// Configuration Registers
#define QMC5883L_CONTROL_1_REG      0x09  
#define QMC5883L_CONTROL_2_REG      0x0A  
#define QMC5883L_SET_RESET_REG      0x0B  

// Identification Register
#define QMC5883L_CHIP_ID_REG        0x0D  // Chip ID Register
#define QMC5883L_CHIP_ID_VAL        0XFF


typedef enum {
    QMC5883L_STANDBY_MODE = 0,    // default value in the register
    QMC5883L_CONTINIOUS_MODE,
} qmc5883l_mode_t;

typedef enum {
    QMC5883L_DATARATE_10_HZ = 0,    // default value in the register
    QMC5883L_DATARATE_50_HZ,
    QMC5883L_DATARATE_100_HZ,
    QMC5883L_DATARATE_200_HZ,
} qmc5883l_datarate_t;

typedef enum {
    QMC5883L_RANGE_2GA = 0,    // default value in the register
    QMC5883L_RANGE_8GA,
} qmc5883l_range_t;

typedef enum {
    QMC5883L_OSR_64 = 0,    // default value in the register
    QMC5883L_OSR_128,
    QMC5883L_OSR_256,
    QMC5883L_OSR_512,
} qmc5883l_oversampling_t;

typedef enum{
    QMC5883L_DRDY_STATUS = 1,
    QMC5883L_OVL_STATUS = 1 << 1,
    QMC5883L_DOR_STATUS = 1 << 2,
} qmc5883l_status_t;


class QMC5883L{
    private:
        float mg_per_lsb;

        // I2C routines 
        bool i2c_sendByte(uint8_t reg, uint8_t data);
        bool i2c_readByte(uint8_t reg, uint8_t* data);
        bool i2c_readBytes(uint8_t reg, uint8_t num, uint8_t* buf);

    public:
        QMC5883L(): mg_per_lsb(0.0333f){}
        bool set_mode(qmc5883l_mode_t mode);
        bool set_datarate(qmc5883l_datarate_t rate);
        bool set_range(qmc5883l_range_t range);
        bool set_oversampling(qmc5883l_oversampling_t osr);

        bool get_mode(qmc5883l_mode_t* mode);
        bool get_rate(qmc5883l_datarate_t* rate);
        bool get_range(qmc5883l_range_t* range);
        bool get_oversampling(qmc5883l_oversampling_t* osr);

        bool begin(qmc5883l_mode_t         mode    = QMC5883L_CONTINIOUS_MODE, 
                   qmc5883l_datarate_t     rate    = QMC5883L_DATARATE_200_HZ, 
                   qmc5883l_range_t        range   = QMC5883L_RANGE_8GA,
                   qmc5883l_oversampling_t osr     = QMC5883L_OSR_512);
        
        bool get_status(uint8_t* status);
        int8_t isReady();
        int8_t isOverflowed();
        int8_t isDataMissed();

        bool get_rawReadings(int16_t (&buf)[3]);
        bool get_magneticField_uT(float (&buf)[3]);
};
