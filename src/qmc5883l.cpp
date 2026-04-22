#include "qmc5883l.h"


bool QMC5883L::set_mode(qmc5883l_mode_t mode){
    uint8_t value;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    value &= 0b11111100;
    value |= mode;

    if (!i2c_sendByte(QMC5883L_CONTROL_1_REG, value))
        return false;

    return true;
}

bool QMC5883L::set_datarate(qmc5883l_datarate_t rate){
    uint8_t value;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    value &= 0b11110011;
    value |= (rate << 2);

    if (!i2c_sendByte(QMC5883L_CONTROL_1_REG, value))
        return false;

    return true;
}

bool QMC5883L::set_range(qmc5883l_range_t range){
    uint8_t value;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    value &= 0b11001111;
    value |= (range << 4);

    if (!i2c_sendByte(QMC5883L_CONTROL_1_REG, value))
        return false;


    // Update a transition coefficient 
    switch (range){
        case QMC5883L_RANGE_2GA:
            mg_per_lsb = 0.00833;
        break;

        case QMC5883L_RANGE_8GA:
            mg_per_lsb = 0.0333;
        break;
    }
    
    return true;
}

bool QMC5883L::set_oversampling(qmc5883l_oversampling_t osr){
    uint8_t value;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    value &= 0b00111111;
    value |= (osr << 6);
    
    if (!i2c_sendByte(QMC5883L_CONTROL_1_REG, value))
        return false;

    return true;
}

bool QMC5883L::get_mode(qmc5883l_mode_t* mode){
    uint8_t value;

    if (mode == nullptr)
        return false;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    *mode = (qmc5883l_mode_t)(value & 0x03);
    return true;
}

bool QMC5883L::get_rate(qmc5883l_datarate_t* rate){
    uint8_t value;

    if (rate == nullptr)
        return false;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    *rate = (qmc5883l_datarate_t)((value >> 2) & 0x03);
    return true;
}

bool QMC5883L::get_range(qmc5883l_range_t* range){
    uint8_t value;

    if (range == nullptr)
        return false;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    *range = (qmc5883l_range_t)((value >> 4) & 0x03);
    return true;
}

bool QMC5883L::get_oversampling(qmc5883l_oversampling_t* osr){
    uint8_t value;

    if (osr == nullptr)
        return false;

    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &value))
        return false;

    *osr = (qmc5883l_oversampling_t)((value >> 6) & 0x03);
    return true;
}

bool QMC5883L::begin(qmc5883l_mode_t mode, qmc5883l_datarate_t rate, 
                     qmc5883l_range_t range, qmc5883l_oversampling_t osr){
    
    uint8_t value;
    uint8_t state;

    if (!i2c_readByte(QMC5883L_CHIP_ID_REG, &value))
        return false;

    if (value != QMC5883L_CHIP_ID_VAL)
        return false;

    if (!set_mode(mode))
        return false;
    
    if (!set_datarate(rate))
        return false;

    if (!set_range(range))
        return false;
    
    if (!set_oversampling(osr))
        return false;

    // make sure that values are written into control register
    if (!i2c_readByte(QMC5883L_CONTROL_1_REG, &state))
        return false;

    if ( ((state & 0x03) != mode) ||
        (((state >> 2) & 0x03) != rate) ||
        (((state >> 4) & 0x03) != range) ||
        (((state >> 6) & 0x03) != osr) )
        return false;
    
    return true;
}

bool QMC5883L::get_status(uint8_t* status){

    if (!i2c_readByte(QMC5883L_STATUS_REG, status))
        return 0;   

    *status &= 0b00000111;
    
    return true;
}

int8_t QMC5883L::isReady(){
    uint8_t status;

    if(!get_status(&status))
        return -1;
    
    return status & QMC5883L_DRDY_STATUS;
}

int8_t QMC5883L::isOverflowed(){
    uint8_t status;

    if(!get_status(&status))
        return -1;
    
    return status & QMC5883L_OVL_STATUS;
}

int8_t QMC5883L::isDataMissed(){
    uint8_t status;

    if(!get_status(&status))
        return -1;
    
    return status & QMC5883L_DOR_STATUS;
}

bool QMC5883L::get_rawReadings(int16_t (&buf)[3]){
    uint8_t temp[6];
    
    // read all data from data output registers
    if (!i2c_readBytes(QMC5883L_OUT_X_LSB_REG, 6, temp))
        return 0;
    
    buf[0] = (int16_t)(((uint16_t)temp[1] << 8) | temp[0]);
    buf[1] = (int16_t)(((uint16_t)temp[3] << 8) | temp[2]);
    buf[2] = (int16_t)(((uint16_t)temp[5] << 8) | temp[4]);

    return 1;
}

bool QMC5883L::get_magneticField_uT(float (&buf)[3]){

    int16_t raw_readings[3];

    if (!get_rawReadings(raw_readings))
        return 0;

    buf[0] = raw_readings[0] * mg_per_lsb;
    buf[1] = raw_readings[1] * mg_per_lsb;
    buf[2] = raw_readings[2] * mg_per_lsb;

    return 1;
}

// I2C routines
bool QMC5883L::i2c_sendByte(uint8_t reg, uint8_t data){
    // send register address for writing
    Wire.beginTransmission(QMC5883L_I2C_ADR); 
    // Fill in the internal i2c buffer
    Wire.write(reg);
    Wire.write(data);

    if (Wire.endTransmission() != 0)
        return 0;
    else
        return 1;
}

bool QMC5883L::i2c_readByte(uint8_t reg, uint8_t* data){
    // send register address for reading
    Wire.beginTransmission(QMC5883L_I2C_ADR); 
    
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0)
        return 0;
    
    // directly read the data from requested register
    if (Wire.requestFrom(QMC5883L_I2C_ADR, (uint8_t)1) != 1)
        return 0;

    if (Wire.available())
        *data = Wire.read();         
    else
        return 0;

    return 1;
}

bool QMC5883L::i2c_readBytes(uint8_t reg, uint8_t num, uint8_t* buf){
    // send register address for reading
    Wire.beginTransmission(QMC5883L_I2C_ADR); 
    
    Wire.write(reg);      
    
    if (Wire.endTransmission(false) != 0)
        return 0;

    // directly read the data from requested register
    if (Wire.requestFrom(QMC5883L_I2C_ADR, num) != num)
        return 0;

    // receive requested data 
    for (uint8_t i = 0; i < num; i++){
        if (Wire.available())
            buf[i] = Wire.read();
        else
            return 0;   
    }

    return 1;
}
