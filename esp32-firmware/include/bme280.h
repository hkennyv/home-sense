#ifndef BME280_H
#define BME280_H

/*
 * BME280 operation
 * 
 * BME280 starts in sleep mode on power up, we want to switch it to normal mode.
 * 
 * TODO: set ctrl_meas register @ 0xF4 with temp oversampling config, pressure
 *  oversampling config, and NORMAL mode
 * TODO: set config register @ 0xF5 with inactive duration, IIR time constant,
 *  and SPI enable (disable)
 * 
 * TODO: implement burst read for TPH measurements
 * 
 * TODO: implement soft reset @ 0xE0
 * 
 * 
 */

/*
 * bme280_burst_read
 * 
 * does a burst read of temp and pressure
 */

esp_err_t bme280_burst_read(void);

/*
 * bme280_read_id
 * 
 * does a one-time read of the bme280's ID. it should return 0x60
 */
esp_err_t bme280_read_id(void);

#endif