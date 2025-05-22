#include "config.h"
#include "miner.h"

/*
 * Provide weak definitions for driver structures when their real
 * implementations aren't compiled. Using the DRIVER_PARSE_COMMANDS macro
 * keeps this list automatically in sync with miner.h.
 */

#define DRIVER_WEAK_DEF(X) \
    struct device_drv X##_drv __attribute__((weak));

DRIVER_PARSE_COMMANDS(DRIVER_WEAK_DEF)

