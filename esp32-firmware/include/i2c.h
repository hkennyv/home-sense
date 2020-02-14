#ifndef I2C_H
#define I2C_H

#include "driver/i2c.h"

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_SLAVE_SCL_IO 22                                 /*!< gpio number for i2c slave clock */
#define I2C_SLAVE_SDA_IO 21                                 /*!< gpio number for i2c slave data */
#define I2C_SLAVE_NUM I2C_NUMBER(CONFIG_I2C_SLAVE_PORT_NUM) /*!< I2C port number for slave dev */
#define I2C_SLAVE_TX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave tx buffer size */
#define I2C_SLAVE_RX_BUF_LEN (2 * DATA_LENGTH)              /*!< I2C slave rx buffer size */

#define I2C_MASTER_SCL_IO 22                /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21                /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(1)        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ I2C_APB_CLK_FREQ /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0         /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0         /*!< I2C master doesn't need buffer */

/*
 * BME280 slave address
 * 
 * SDO = 0      0x76
 * SDO = 1      0x77
 * 
 * REGISTERS (see 5.4 in BME280 datasheet)
 * TODO: fill in more info here
 *  "id"    0xD0
 *  "reset" 0xE0
 *  "ctrl_hum"  0xF2
 *  "status"    0xF3
 *  "ctrl_meas" 0xF4
 *  "config"    0xF5
 *  "press"     0xF7 0xF8 0xF9
 *  "temp"      0xFA 0XFB 0XFC
 *  "hum"       0xFD 0xFE
 * 
 */
#define BME280_SENSOR_ADDR 0x76    /*!< slave address for BME280 sensor */
#define BME280_REG_ID 0xD0         /*!< "id" register */
#define ESP_SLAVE_ADDR 0x76        /*!< ESP32 slave address, you can set any 7bit value */
#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                /*!< I2C ack value */
#define NACK_VAL 0x1               /*!< I2C nack value */

esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size);
esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size);
esp_err_t i2c_master_sensor_test(i2c_port_t i2c_num, uint8_t *data_h, uint8_t *data_l);
esp_err_t i2c_master_init(void);

#endif