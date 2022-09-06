
/**
 * @brief Class for MPC27S17 driver
 *
 */
#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include <hardware/spi.h>

// REGISTERS ARE DEFINED HERE SO THAT THEY MAY BE USED IN THE MAIN PROGRAM
// Only valid for ICON.BANK = 0

#define IODIRA (0x00) // MCP23x17 I/O Direction Register
#define IODIRB (0x01) // 1 = Input (default), 0 = Output

#define IPOLA (0x02) // MCP23x17 Input Polarity Register
#define IPOLB (0x03) // 0 = Normal (default)(low reads as 0), 1 = Inverted (low reads as 1)

#define GPINTENA (0x04) // MCP23x17 Interrupt on Change Pin Assignements
#define GPINTENB (0x05) // 0 = No Interrupt on Change (default), 1 = Interrupt on Change

#define DEFVALA (0x06) // MCP23x17 Default Compare Register for Interrupt on Change
#define DEFVALB (0x07) // Opposite of what is here will trigger an interrupt (default = 0)

#define INTCONA (0x08) // MCP23x17 Interrupt on Change Control Register
#define INTCONB (0x09) // 1 = pin is compared to DEFVAL, 0 = pin is compared to previous state (default)

#define IOCON (0x0A) // MCP23x17 Configuration Register
//                   (0x0B)      //     Also Configuration Register

#define GPPUA (0x0C) // MCP23x17 Weak Pull-Up Resistor Register
#define GPPUB (0x0D) // INPUT ONLY: 0 = No Internal 100k Pull-Up (default) 1 = Internal 100k Pull-Up

#define INTFA (0x0E) // MCP23x17 Interrupt Flag Register
#define INTFB (0x0F) // READ ONLY: 1 = This Pin Triggered the Interrupt

#define INTCAPA (0x10) // MCP23x17 Interrupt Captured Value for Port Register
#define INTCAPB (0x11) // READ ONLY: State of the Pin at the Time the Interrupt Occurred

#define GPIOA (0x12) // MCP23x17 GPIO Port Register
#define GPIOB (0x13) // Value on the Port - Writing Sets Bits in the Output Latch

#define OLATA (0x14) // MCP23x17 Output Latch Register.
#define OLATB (0x15) // 1 = Latch High, 0 = Latch Low (default) Reading Returns Latch State, Not Port Value!
                     // The OLAT register provides access to the output latches. A read from this register results in a read of the
                     // OLAT and not the port itself.A write to this register modifies the output latches that modifies the pins configured as outputs.

// commands
#define WRITE_CMD (0xFE)
#define READ_CMD (0x01)

// default address
#define DEVICE_ADDRESS = 0xA0

// Defines to keep logical information symbolic go here
#ifndef HIGH
#define HIGH (1)
#endif

#ifndef LOW
#define LOW (0)
#endif

#ifndef ON
#define ON (1)
#endif

#ifndef OFF
#define OFF (0)
#endif

/**
 * @brief class for MCP23S17 driver
 *
 */
class MCP23S17
{
public:
    /**
     * @brief Construct a new MCP23S17 object
     *
     */
    MCP23S17();

    /**
     * @brief Construct a new MCP23S17 object
     *
     * @param address
     * @param chipSelect
     * @param ioMode
     * @param defaultOutputState
     * @param pullUPChache
     * @param inputInversionState
     */
    MCP23S17(uint8_t address, uint8_t chipSelect, unsigned int ioMode, unsigned int defaultOutputState, unsigned int pullUPChache, unsigned int inputInversionState);

    /**
     * @brief Destroy the MCP23S17 object
     *
     */
    ~MCP23S17();

    /**
     * @brief begin with default parameter values
     *
     */
    void begin();

    /**
     * @brief begin with specific values
     *
     * @param SPIID
     * @param SCK
     * @param MOSI
     * @param MISO
     * @param BAUDRATE
     */
    void begin(const uint8_t SPIID, const uint8_t SCK, const uint8_t MOSI, const uint8_t MISO, const ulong BAUDRATE = 100000);

    /**
     * @brief write a value to a specific register
     *
     * @param register
     * @param value
     */
    void writeRegister(uint8_t register, uint8_t value);

    /**
     * @brief write a value to a specific register
     *
     * @param register
     * @return uint8_t
     */
    uint8_t readRegister(uint8_t register);

private:
    uint8_t _address;          // Address of the MCP23S17 in use
    uint8_t _ss;               // Slave-select pin
    unsigned int _modeCache;   // Caches the mode (input/output) configuration of I/O pins
    unsigned int _pullupCache; // Caches the internal pull-up configuration of input pins (values persist across mode changes)
    unsigned int _invertCache; // Caches the input pin inversion selection (values persist across mode changes)
    unsigned int _outputCache; // Caches the output pin state of pins
};
