/*
 * BM1370 test driver
 */

#include "config.h"
#include "miner.h"
#include <unistd.h>

static void bm1370_detect(__maybe_unused bool hotplug)
{
    applog(LOG_INFO, "BM1370 test driver detected (hotplug=%d)", hotplug);
}

static bool bm1370_prepare(__maybe_unused struct thr_info *thr)
{
    return true;
}

static int64_t bm1370_scanwork(__maybe_unused struct thr_info *thr)
{
    cgsleep_ms(1000);
    return 0;
}

struct device_drv bm1370_drv = {
    .drv_id = DRIVER_bm1370,
    .dname = "bm1370",
    .name = "B70",
    .drv_detect = bm1370_detect,
    .thread_prepare = bm1370_prepare,
    .hash_work = hash_driver_work,
    .scanwork = bm1370_scanwork,
};
