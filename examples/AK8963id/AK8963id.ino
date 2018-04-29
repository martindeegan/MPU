#include <Wire.h>

#include "ArduinoTransfer.h"

// Define I2C addresses of MPU9250
#define ADO 0
#if ADO
static const uint8_t MPU9250_ADDRESS  = 0x69;   // Device address when ADO = 1
#else
static const uint8_t MPU9250_ADDRESS  = 0x68;  // Device address when ADO = 0
#endif  

// MPU9250 registers
const uint8_t EXT_SENS_DATA_00 = 0x49;

const uint8_t USER_CTRL = 0x6A;
const uint8_t I2C_MST_EN = 0x20;
const uint8_t I2C_SLV0_ADDR = 0x25;
const uint8_t I2C_SLV0_REG = 0x26;
const uint8_t I2C_SLV0_CTRL = 0x27;
const uint8_t I2C_SLV0_EN = 0x80;
const uint8_t I2C_READ_FLAG = 0x80;

// AK8963 registers
const uint8_t AK8963_I2C_ADDR = 0x0C;

const uint8_t AK8963_WHO_AM_I = 0x00;

ArduinoWire bt;

/* reads registers from the AK8963 */
static void readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    // set slave 0 to the AK8963 and set for read
    bt.writeRegister(MPU9250_ADDRESS, I2C_SLV0_ADDR,AK8963_I2C_ADDR | I2C_READ_FLAG);

    // set the register to the desired AK8963 sub address
    bt.writeRegister(MPU9250_ADDRESS, I2C_SLV0_REG,subAddress);

    // enable I2C and request the bytes
    bt.writeRegister(MPU9250_ADDRESS, I2C_SLV0_CTRL,I2C_SLV0_EN | count);

    delay(1); // takes some time for these registers to fill

    // read the bytes off the MPU9250 EXT_SENS_DATA registers
    bt.readRegisters(MPU9250_ADDRESS, EXT_SENS_DATA_00,count,dest); 
}

static int whoAmIAK8963()
{
    uint8_t buffer = 0;

    // read the WHO AM I register
    readAK8963Registers(AK8963_WHO_AM_I,1, &buffer);

    // return the register value
    return buffer;
}

void setup(void)
{
    Serial.begin(115200);

    Wire.begin();

    Wire.setClock(400000);

    // enable I2C master mode
    bt.writeRegister(MPU9250_ADDRESS, USER_CTRL,I2C_MST_EN);

    // check AK8963 WHO AM I register, expected value is 0x48 (decimal 72)
    uint8_t addr = whoAmIAK8963();

    while (true) {
        Serial.print("0x");
        Serial.println(addr, HEX);
    }
}

void loop(void)
{
}