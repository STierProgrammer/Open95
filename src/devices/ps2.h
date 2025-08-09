#ifndef OPEN95_DEVICES_PS2
#define OPEN95_DEVICES_PS2

#include "misc.h"
#include <stdint.h>

#define DATA_PORT 0x60
#define STATUS_REGISTER 0x64
#define COMMAND_REGISTER 0x64

/// Status Output Buffer Full 
#define STATUS_OBF 0x01
/// Staut Input Buffer Full
#define STATUS_IBF 0x02

#define CMD_TEST_PS2_CONTROLLER 0xAA

#define CMD_DISABLE_FIRST_PS2_PORT 0xAD
#define CMD_ENABLE_FIRST_PS2_PORT 0xAE
#define CMD_TEST_FIRST_PS2_PORT 0xAA

#define CMD_DISABLE_SECOND_PS2_PORT 0xA7
#define CMD_ENABLE_SECOND_PS2_PORT 0xA8
#define CMD_TEST_SECOND_PS2_PORT 0xA9

uint8_t read_data_port(void);
void write_data_port(uint8_t data);
uint8_t read_status_register(void);
void write_command_register(uint8_t command);

void ps2_wait_input_empty(void);
void ps2_wait_output_full(void);

/// Returns 0x55 on success, 0xFC on failure
uint8_t ps2_test_controller(void);

void disable_second_ps2_port(void);
void enable_second_ps2_port(void);

/// Possible returns:
/// 0x00 test passed 
/// 0x01 clock line stuck low 
/// 0x02 clock line stuck high 
/// 0x03 data line stuck low 
/// 0x04 data line stuck high 
uint8_t test_second_ps2_port(void); 

void disable_first_ps2_port(void);
void enable_first_ps2_port(void);

/// Possible returns:
/// 0x00 test passed 
/// 0x01 clock line stuck low 
/// 0x02 clock line stuck high 
/// 0x03 data line stuck low 
/// 0x04 data line stuck high 
uint8_t test_first_ps2_port(void); 

void init_ps2_controller();

#endif
