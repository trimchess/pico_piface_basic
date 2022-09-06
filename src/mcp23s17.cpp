/**
 * @file mcp23s17.cpp
 * @author pycodrr.maker@gmail.com
 * @brief
 * @version 0.1
 * @date 2022-09-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "MCP23S17.h" // Header files for this class
#include <iostream>

using namespace std;

/**
 * @brief Construct a new MCP23S17::MCP23S17 object with default data
 *
 */
MCP23S17::MCP23S17()
{
    _address = 0x40;
    _ss = 5;
    //_modeCache = 0xFFFF;   // Default I/O mode is all input, 0xFFFF
    _modeCache = 0x000;    // Default I/O mode is all output, 0x0000
    _outputCache = 0x0000; // Default output state is all off, 0x0000
    _pullupCache = 0x0000; // Default pull-up state is all off, 0x0000
    _invertCache = 0x0000; // Default input inversion state is not inverted, 0x0000
};

/**
 * @brief Construct a new MCP23S17::MCP23S17 object
 *
 * @param address
 * @param chipSelect
 * @param ioMode
 * @param defaultOutputState
 * @param pullUPChache
 * @param inputInversionState
 */
MCP23S17::MCP23S17(uint8_t address, uint8_t chipSelect, unsigned int ioMode, unsigned int defaultOutputState, unsigned int pullUpCache, unsigned int inputInversionState)
{
    _address = address;
    _ss = chipSelect;
    //_modeCache = 0xFFFF;   // Default I/O mode is all input, 0xFFFF
    _modeCache = ioMode;                // Default I/O mode is all output, 0x0000
    _outputCache = defaultOutputState;  // Default output state is all off, 0x0000
    _pullupCache = pullUpCache;         // Default pull-up state is all off, 0x0000
    _invertCache = inputInversionState; // Default input inversion state is not inverted, 0x0000
};

/**
 * @brief mcp begin with initaial values
 *        GPIO2    SCK
 *        GPIO3    MOSI
 *        GPIO4    MISO
 *        GPIO5    CS (_ss)
 */
void MCP23S17::begin()
{
    spi_init(spi0, 100 * 1000);          // Baudrate 100k
    gpio_set_function(2, GPIO_FUNC_SPI); // SCK
    gpio_set_function(3, GPIO_FUNC_SPI); //
    gpio_set_function(4, GPIO_FUNC_SPI);
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    gpio_init(_ss);
    gpio_set_dir(_ss, GPIO_OUT);
    gpio_put(_ss, HIGH);
}

/**
 * @brief mcp begin
 *
 * @param SPIID
 * @param SCK
 * @param MOSI
 * @param MISO
 * @param BAUDRATE
 */
void MCP23S17::begin(const uint8_t SPIID, const uint8_t SCK, const uint8_t MOSI, const uint8_t MISO, const ulong BAUDRATE)
{
    spi_init(spi0, BAUDRATE); // Baudrate 100k
    gpio_set_function(SCK, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    if (SPIID == 0)
    {
        spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    }
    else
    {
        spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    }
    spi_set_format(spi0, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 8 data bits.
    gpio_init(_ss);
    gpio_set_dir(_ss, GPIO_OUT);
    gpio_put(_ss, HIGH);
}

/**
 * @brief write a value to a specific register
 *
 * @param reg
 * @param value
 */

void MCP23S17::writeRegister(uint8_t reg, uint8_t value)
{ // Accept the register and byte
    uint8_t adr = (_address & WRITE_CMD);
    uint8_t wBuff[3] = {adr, reg, value};
    // Take slave-select low
    gpio_put(_ss, LOW);
    sleep_us(1);
    int n = spi_write_blocking(spi0, wBuff, 3);
    gpio_put(_ss, HIGH);
}

/**
 * @brief read a value from a specific register
 *
 * @param reg
 * @return uint8_t
 */
uint8_t MCP23S17::readRegister(uint8_t reg)
{
    // This function will read a single register, and return it
    uint8_t adr = (_address | READ_CMD);
    uint8_t wBuff[2] = {adr, reg};
    uint8_t rBuff[8];
    gpio_put(_ss, LOW); // Take slave-select low
    sleep_us(1);        // Take slave-select low
    spi_write_blocking(spi0, wBuff, 2);
    sleep_ms(10);
    int n = spi_read_blocking(spi0, 0, rBuff, 8);
    gpio_put(_ss, HIGH);
    sleep_us(1);     // Take slave-select high
    return rBuff[0]; // Return the constructed word, the format is 0x(register value)
}
