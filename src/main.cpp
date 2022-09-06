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

#define LED_PIN 10

MCP23S17 mcp;

using namespace std;

void printValue(int, const char *);

void led10_task()
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

void mcp_out_task()
{
    mcp.writeRegister(IODIRB, 0xFF);                    // port B (pins 1-7) all bits OUTPUT
    mcp.writeRegister(IPOLB, 0xFF);                     // port B bits 7...0 inverted
    mcp.writeRegister(GPPUB, 0xFF);                     // port B pull up all inputs.
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 500ms
    while(1)
    {
        vTaskDelay(xDelay * 2);
        uint8_t readResult = mcp.readRegister(GPIOB); // read input direct via GPIO. No latch for inputs!
        printValue((int)readResult, "Read Data Register GPIOB: 0x");
    }

}

void mcp_in_task()
{
    mcp.writeRegister(IODIRA, 0x00);                    // port A (pins 17-24) all bits INPUT
    const TickType_t xDelay = 500 / portTICK_PERIOD_MS; // 500ms
    while (1)
    {
        mcp.writeRegister(OLATA, 0xAA); // set output via latch register
        vTaskDelay(xDelay * 2);
        mcp.writeRegister(OLATA, 0x55); // set output via latch register
        vTaskDelay(xDelay * 2);
    }
}

/**
 * @brief Printaout formated value
 *
 * @param value
 */
void printValue(int value, const char *theString)
{
    cout << theString << hex
         << uppercase
         << setw(2)
         << setfill('0')
         << value
         << endl;
}

/**
 * @brief main function
 *
 * @return int
 */
int main(void)
{
    stdio_init_all();
    mcp.begin();
    xTaskCreate((TaskFunction_t)led10_task, "LED10_TASK", 256, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)mcp_in_task, "MCPIN_TASK", 256, NULL, 1, NULL);
    xTaskCreate((TaskFunction_t)mcp_out_task, "MCPOUT_TASK", 256, NULL, 1, NULL);
    vTaskStartScheduler();
}
