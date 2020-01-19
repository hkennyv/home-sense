/* i2c - Example
   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples
   See README.md file to get detailed usage of this example.
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"

#include "i2c.h"

static const char *TAG = "i2c-example";

SemaphoreHandle_t print_mux = NULL;

/**
 * @brief test code to read esp-i2c-slave
 *        We need to fill the buffer of esp slave device, then master can read them out.
 *
 * _______________________________________________________________________________________
 * | start | slave_addr + rd_bit +ack | read n-1 bytes + ack | read 1 byte + nack | stop |
 * --------|--------------------------|----------------------|--------------------|------|
 *
 */
esp_err_t i2c_master_read_slave(i2c_port_t i2c_num, uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief Test code to write esp-i2c-slave
 *        Master device write data to slave(both esp32),
 *        the data will be stored in slave buffer.
 *        We can read them out from slave buffer.
 *
 * ___________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write n bytes + ack  | stop |
 * --------|---------------------------|----------------------|------|
 *
 */
esp_err_t i2c_master_write_slave(i2c_port_t i2c_num, uint8_t *data_wr, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ESP_SLAVE_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(cmd, data_wr, size, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief test code to operate on BME280 sensor
 *
 * 1. set operation mode(e.g One time L-resolution mode)
 * _________________________________________________________________
 * | start | slave_addr + wr_bit + ack | write 1 byte + ack  | stop |
 * --------|---------------------------|---------------------|------|
 * 2. wait more than 24 ms
 * 3. read data
 * ______________________________________________________________________________________
 * | start | slave_addr + rd_bit + ack | read 1 byte + ack  | read 1 byte + nack | stop |
 * --------|---------------------------|--------------------|--------------------|------|
 */
esp_err_t i2c_master_sensor_test(i2c_port_t i2c_num, uint8_t *data_h, uint8_t *data_l)
{
    int ret;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BME280_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0xD0, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK)
    {
        return ret;
    }
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, BME280_SENSOR_ADDR << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, data_h, ACK_VAL);
    i2c_master_read_byte(cmd, data_l, NACK_VAL);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(i2c_num, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief i2c master initialization
 */
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

/**
 * @brief test function to show buffer
 */
// static void disp_buf(uint8_t *buf, int len)
// {
//     int i;
//     for (i = 0; i < len; i++)
//     {
//         printf("%02x ", buf[i]);
//         if ((i + 1) % 16 == 0)
//         {
//             printf("\n");
//         }
//     }
//     printf("\n");
// }

// static void i2c_test_task(void *arg)
// {
//     int i = 0;
//     int ret;
//     uint32_t task_idx = (uint32_t)arg;
//     uint8_t *data = (uint8_t *)malloc(DATA_LENGTH);
//     uint8_t *data_wr = (uint8_t *)malloc(DATA_LENGTH);
//     uint8_t *data_rd = (uint8_t *)malloc(DATA_LENGTH);
//     uint8_t sensor_data_h, sensor_data_l;
//     int cnt = 0;
//     while (1)
//     {
//         ESP_LOGI(TAG, "TASK[%d] test cnt: %d", task_idx, cnt++);
//         ret = i2c_master_sensor_test(I2C_MASTER_NUM, &sensor_data_h, &sensor_data_l);
//         xSemaphoreTake(print_mux, portMAX_DELAY);
//         if (ret == ESP_ERR_TIMEOUT)
//         {
//             ESP_LOGE(TAG, "I2C Timeout");
//         }
//         else if (ret == ESP_OK)
//         {
//             printf("*******************\n");
//             printf("TASK[%d]  MASTER READ SENSOR( BME280 )\n", task_idx);
//             printf("*******************\n");
//             printf("data_h: %02x\n", sensor_data_h);
//             printf("data_l: %02x\n", sensor_data_l);
//             printf("sensor val: %.02f [Lux]\n", (sensor_data_h << 8 | sensor_data_l) / 1.2);
//         }
//         else
//         {
//             ESP_LOGW(TAG, "%s: No ack, sensor not connected...skip...", esp_err_to_name(ret));
//         }
//         xSemaphoreGive(print_mux);
//         vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
//         //---------------------------------------------------
//         for (i = 0; i < DATA_LENGTH; i++)
//         {
//             data[i] = i;
//         }
//         xSemaphoreTake(print_mux, portMAX_DELAY);
//         size_t d_size = i2c_slave_write_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
//         if (d_size == 0)
//         {
//             ESP_LOGW(TAG, "i2c slave tx buffer full");
//             ret = i2c_master_read_slave(I2C_MASTER_NUM, data_rd, DATA_LENGTH);
//         }
//         else
//         {
//             ret = i2c_master_read_slave(I2C_MASTER_NUM, data_rd, RW_TEST_LENGTH);
//         }

//         if (ret == ESP_ERR_TIMEOUT)
//         {
//             ESP_LOGE(TAG, "I2C Timeout");
//         }
//         else if (ret == ESP_OK)
//         {
//             printf("*******************\n");
//             printf("TASK[%d]  MASTER READ FROM SLAVE\n", task_idx);
//             printf("*******************\n");
//             printf("====TASK[%d] Slave buffer data ====\n", task_idx);
//             disp_buf(data, d_size);
//             printf("====TASK[%d] Master read ====\n", task_idx);
//             disp_buf(data_rd, d_size);
//         }
//         else
//         {
//             ESP_LOGW(TAG, "TASK[%d] %s: Master read slave error, IO not connected...\n",
//                      task_idx, esp_err_to_name(ret));
//         }
//         xSemaphoreGive(print_mux);
//         vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
//         //---------------------------------------------------
//         int size;
//         for (i = 0; i < DATA_LENGTH; i++)
//         {
//             data_wr[i] = i + 10;
//         }
//         xSemaphoreTake(print_mux, portMAX_DELAY);
//         //we need to fill the slave buffer so that master can read later
//         ret = i2c_master_write_slave(I2C_MASTER_NUM, data_wr, RW_TEST_LENGTH);
//         if (ret == ESP_OK)
//         {
//             size = i2c_slave_read_buffer(I2C_SLAVE_NUM, data, RW_TEST_LENGTH, 1000 / portTICK_RATE_MS);
//         }
//         if (ret == ESP_ERR_TIMEOUT)
//         {
//             ESP_LOGE(TAG, "I2C Timeout");
//         }
//         else if (ret == ESP_OK)
//         {
//             printf("*******************\n");
//             printf("TASK[%d]  MASTER WRITE TO SLAVE\n", task_idx);
//             printf("*******************\n");
//             printf("----TASK[%d] Master write ----\n", task_idx);
//             disp_buf(data_wr, RW_TEST_LENGTH);
//             printf("----TASK[%d] Slave read: [%d] bytes ----\n", task_idx, size);
//             disp_buf(data, size);
//         }
//         else
//         {
//             ESP_LOGW(TAG, "TASK[%d] %s: Master write slave error, IO not connected....\n",
//                      task_idx, esp_err_to_name(ret));
//         }
//         xSemaphoreGive(print_mux);
//         vTaskDelay((DELAY_TIME_BETWEEN_ITEMS_MS * (task_idx + 1)) / portTICK_RATE_MS);
//     }
//     vSemaphoreDelete(print_mux);
//     vTaskDelete(NULL);
// }

// void app_main(void)
// {
//     print_mux = xSemaphoreCreateMutex();
//     ESP_ERROR_CHECK(i2c_slave_init());
//     ESP_ERROR_CHECK(i2c_master_init());
//     xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, (void *)0, 10, NULL);
//     xTaskCreate(i2c_test_task, "i2c_test_task_1", 1024 * 2, (void *)1, 10, NULL);
// }
