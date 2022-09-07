/**
 * @file main.cpp
 * @author pycoder.maker@gmail.com
 * @brief  Application to test MCP27S17 driever class (MCP27S17.h, MCP27S17.cpp).
 *         This class supports only basic register functions (IN/OUT registers) and no interrupts
 *
 * @version 0.1
 * @date 2022-09-05
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp23s17.h"
#include <iostream>
#include <iomanip>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define LED_PIN 25

MCP23S17 mcp;
static SemaphoreHandle_t mutex;

/**
 * @brief Printaout formated value
 *
 * @param value
 */
void printValue(int value, const char *theString)
{
    std::cout << theString << std::hex
              << std::uppercase
              << std::setw(2)
              << std::setfill('0')
              << value
              << std::endl;
}

void led_task()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 500ms
    while (true)
    {
        gpio_put(LED_PIN, HIGH);
        vTaskDelay(2 * xDelay);
        gpio_put(LED_PIN, LOW);
        vTaskDelay(2 * xDelay);
    }
}

void mcp_port_B_in_task()
{
    uint8_t readResult;
    uint8_t oldValue;
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);
    if (mutex != nullptr)
    {
        if (xSemaphoreTake(mutex, 10) == pdTRUE)
        {
            mcp.writeRegister(IODIRB, 0xFF); // port B (pins 1-7) all bits INPUT
            mcp.writeRegister(IPOLB, 0xFF);  // port B bits 7...0 inverted
            mcp.writeRegister(GPPUB, 0xFF);  // port B pull up all inputs.
            readResult = mcp.readRegister(GPIOB);
            xSemaphoreGive(mutex);
        }
    }
    oldValue = readResult;
    while (1)
    {
        vTaskDelay(xDelay / 5);
        if (mutex != nullptr)
        {
            if (xSemaphoreTake(mutex, 10) == pdTRUE)
            {
                readResult = mcp.readRegister(GPIOB); // read input direct via GPIO. No latch for inputs!
                xSemaphoreGive(mutex);
            }
        }
        if (oldValue != readResult)
        {
            printValue((int)readResult, "Port B changed, readResult: 0x");
        }
        oldValue = readResult;
    }
}

void mcp_port_A_out_task()
{
    if (mutex != nullptr)
    {
        if (xSemaphoreTake(mutex, 10) == pdTRUE)
        {
            mcp.writeRegister(IODIRA, 0x00); // port A (pins 17-24) all bits OUTPUT
            xSemaphoreGive(mutex);
        }
    }
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 500ms
    vTaskDelay(xDelay * 3);
    while (1)
    {
        if (mutex != nullptr)
        {
            if (xSemaphoreTake(mutex, 10) == pdTRUE)
            {
                mcp.writeRegister(OLATA, 0xAA); // set output via latch register
                xSemaphoreGive(mutex);
            }
        }
        vTaskDelay(xDelay * 2);
        if (mutex != nullptr)
        {
            if (xSemaphoreTake(mutex, 10) == pdTRUE)
            {
                mcp.writeRegister(OLATA, 0x55); // set output via latch register
                xSemaphoreGive(mutex);
            }
        }
        vTaskDelay(xDelay * 2);
    }
}

/**
 * @brief main function
 *
 * @return int
 */
int main(void)
{
    stdio_init_all();
    mutex = xSemaphoreCreateMutex();
    mcp.begin();
    xTaskCreate((TaskFunction_t)led_task, "LED_TASK", 256, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)mcp_port_B_in_task, "MCPIN_TASK", 256, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)mcp_port_A_out_task, "MCPOUT_TASK", 256, NULL, 1, NULL);
    vTaskStartScheduler();
}
