/**
 * @file i2c.h
 * @brief Header file for I2C master operations.
 *
 * This file contains the function declarations for initializing the I2C master,
 * adding devices, and performing read and write operations.
 */
#pragma once

#include <driver/i2c_master.h>
#include <driver/i2c_slave.h>
#include <esp_log.h>

#define I2C_MASTER_SCL_IO GPIO_NUM_22   /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21   /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0        /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000       /*!< I2C master clock frequency */
#define I2C_TIMEOUT 5000                    /*!< I2C timeout in ms */

/**
 * @brief Initialize the I2C master.
 *
 * This function initializes the I2C master interface, setting up the necessary
 * configurations for communication.
 *
 * @return
 *     - ESP_OK: Success
 *     - ESP_FAIL: Initialization failed
 */
esp_err_t i2c_master_init(void);

/**
 * @brief Add a device to the I2C master.
 *
 * This function registers a device with the specified address to the I2C master.
 *
 * @param addr The address of the I2C device.
 * @param dev_handle The handle pointer to the I2C device.
 *
 * @return
 *     - ESP_OK: Success
 *     - ESP_FAIL: Failed to add device
 */
esp_err_t i2c_master_add_device(uint8_t addr, i2c_master_dev_handle_t* dev_handle);

/**
 * @brief Write data to an I2C device.
 *
 * This function writes data to the specified I2C device.
 *
 * @param dev_handle The handle to the I2C device.
 * @param data_wr Pointer to the data to be written.
 * @param size The size of the data to be written.
 *
 * @return
 *     - ESP_OK: Success
 *     - ESP_FAIL: Write operation failed
 */
esp_err_t i2c_masterWrite(i2c_master_dev_handle_t dev_handle, uint8_t *data_wr, size_t size);

/**
 * @brief Read data from an I2C device.
 *
 * This function reads data from the specified I2C device.
 *
 * @param dev_handle The handle to the I2C device.
 * @param data_rd Pointer to the buffer where the read data will be stored.
 * @param size The size of the data to be read.
 *
 * @return
 *     - ESP_OK: Success
 *     - ESP_FAIL: Read operation failed
 */
esp_err_t i2c_masterRead(i2c_master_dev_handle_t dev_handle, uint8_t *data_rd, size_t size);