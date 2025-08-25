#include "include/ps2.h"

void ps2_wait_input_empty(void)
{
    while (inb(STATUS_REGISTER) & STATUS_IBF)
        ;
}

void ps2_wait_output_full(void)
{
    while (!(inb(STATUS_REGISTER) & STATUS_OBF))
        ;
}

uint8_t read_data_port(void)
{
    ps2_wait_output_full();
    return inb(DATA_PORT);
}

void write_data_port(uint8_t data)
{
    ps2_wait_input_empty();
    outb(DATA_PORT, data);
}

uint8_t read_status_register(void)
{
    ps2_wait_output_full();
    return inb(STATUS_REGISTER);
}

void write_command_register(uint8_t command)
{
    ps2_wait_input_empty();
    outb(COMMAND_REGISTER, command);
}

uint8_t ps2_test_controller(void)
{
    write_command_register(CMD_TEST_PS2_CONTROLLER);
    return read_data_port();
}

void disable_second_ps2_port(void)
{
    write_command_register(CMD_DISABLE_SECOND_PS2_PORT);
}

void enable_second_ps2_port(void)
{
    write_command_register(CMD_ENABLE_SECOND_PS2_PORT);
}

uint8_t test_second_ps2_port(void)
{
    write_command_register(CMD_TEST_SECOND_PS2_PORT);
    return read_data_port();
}

void disable_first_ps2_port(void)
{
    write_command_register(CMD_DISABLE_FIRST_PS2_PORT);
}

void enable_first_ps2_port(void)
{
    write_command_register(CMD_ENABLE_FIRST_PS2_PORT);
}

uint8_t test_first_ps2_port(void)
{
    write_command_register(CMD_TEST_FIRST_PS2_PORT);
    return read_data_port();
}

void init_ps2_controller()
{
    // https://wiki.osdev.org/I8042_PS/2_Controller
    // TODO: Initialize USB Controllers
    // TODO: Determine if the PS/2 Controller Exists

    disable_first_ps2_port();
    disable_second_ps2_port();

    // TODO: Flush The Output Buffer
}