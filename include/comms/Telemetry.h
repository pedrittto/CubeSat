#pragma once
#include <stdint.h>

struct __attribute__((packed)) EpsTelemetry{
    uint8_t sync_byte;
    uint16_t v_batt_mv;
    uint16_t i_solar_ma;
    uint8_t system_state;
    uint8_t checkSum;
};