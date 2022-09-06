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

#define LED_PIN 10

using namespace std;

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
    const uint8_t DEVICE_ADDR = 0x40;

    /**
     * @brief Construct a new stdio init all object
     *
     */
    stdio_init_all();
    // set Pin 10 as flow control LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // instantiate an MCP23S17 object
    MCP23S17 mcp;
    // start mcp with default values
    // spi0, pico GPIO2 = SCK, pico GPIO3 = MOSI, GPIO4 = MISO, pico GPIO = CS
    mcp.begin();

    // PiFace
    // GPA > OUTPUT, MCP23S17 Pin  21...28
    // GPB > INPUT,  MCP23S17 Pin  1...8
    mcp.writeRegister(IODIRA, 0x00); // port A (pins 17-24) all bits INPUT
    mcp.writeRegister(IODIRB, 0xFF); // port B (pins 1-7) all bits OUTPUT
    mcp.writeRegister(IPOLB, 0xFF);  // port B bits 7...0 inverted
    mcp.writeRegister(GPPUB, 0xFF);  // port B pull up all inputs.

    while (1)
    {
        mcp.writeRegister(OLATA, 0xAA);               // set output via latch register
        uint8_t readResult = mcp.readRegister(OLATA); // read output via latch register
        //printValue((int)readResult, "Read Data Register GPIOA: 0x");
        gpio_put(LED_PIN, 1);
        sleep_ms(1000);
        mcp.writeRegister(OLATA, 0x55);       // set output via latch register
        readResult = mcp.readRegister(OLATA); // read output via latch register
        //printValue((int)readResult, "Read Data Register GPIOA: 0x");
        readResult = mcp.readRegister(GPIOB); // read input direct via GPIO. No latch for inputs!
        printValue((int)readResult, "Read Data Register GPIOB: 0x");
        gpio_put(LED_PIN, 0);
        sleep_ms(1000);
    }
}
