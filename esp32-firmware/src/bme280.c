#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include "bme280.h"
#include "i2c.h"

esp_err_t bme280_burst_read(void)
{
    /*
     * BME280 datasheet suggests using burst read will be faster than
     * individually reading out registers.
     * 
     * bursts:
     *  0xF7 to 0xFC (temperature and pressure)
     *  0xF7 to 0xFE (temperature, pressure, humidity)
     */
    // static esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size)
    // if (size == 0)
    // {
    //     return ESP_OK;
    // }
    /*
     * 3 bytes pressure, 3 bytes temp, 2 bytes humidity
     */
    size_t size = 8;
    uint8_t data_rd[8] = {0};
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t bme280_read_id(void)
{
    /*
     * Register 0xD0 "id"
     * 
     * contains the chip identificatio number chip_id[7:0], which is 0x60
     * this number can be read as soon as the device finished the
     * power-on-reset
     */
    uint8_t data = 0;
    uint8_t reg_addr = 0xD0;


    i2c_cmd_handle_t cmd = i2c_cmd_link_create();


    i2c_master_start(cmd);                                                              // START
    i2c_master_write_byte(cmd, (BME280_SENSOR_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);    // WRITE
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);                                 // reg_addr

    i2c_master_start(cmd);                                                              // START
    i2c_master_write_byte(cmd, (BME280_SENSOR_ADDR << 1) | READ_BIT, ACK_CHECK_EN);     // READ
    i2c_master_read_byte(cmd, &data, NACK_VAL);                                         // data byte
    i2c_master_stop(cmd);                                                               // STOP

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 5000 / portTICK_RATE_MS);

    if (ret == ESP_OK)
    {
        printf("hardware id: 0x%x\n", data);
    }
    else
    {
        const char *err_name = esp_err_to_name(ret);

        printf("error in i2c. code: [0x%x] %s\n", ret, err_name);
    }
    i2c_cmd_link_delete(cmd);
    return ret;
}