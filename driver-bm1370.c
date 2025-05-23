/*
 * Skeleton driver for Bitmain BM1370 ASICs
 */

#include "config.h"
#include <unistd.h>
#include "miner.h"
#include "driver-bm1370.h"

/* --- Internal helpers ---------------------------------------------------- */

/* Allocate device info structure if needed */
static struct bm1370_info *bm1370_get_info(struct cgpu_info *cgpu)
{
    struct bm1370_info *info = cgpu->device_data;

    if (!info) {
        info = cgcalloc(1, sizeof(*info));
        info->chips = 1; /* default single chip */
        info->frequency = 0;
        info->voltage = 0;
        info->temp = -1.0; /* unknown temperature */
        cgpu->device_data = info;
    }
    return info;
}

/* --- Driver API ---------------------------------------------------------- */

bool bm1370_detect(struct cgpu_info *cgpu)
{
    struct bm1370_info *info = bm1370_get_info(cgpu);

    /* In a real driver hardware detection would occur here */
    applog(LOG_INFO, "BM1370 device %d detected", cgpu->device_id);

    /* placeholder values */
    info->version_mask = 0xFFFFFFFF;
    return true;
}

void bm1370_init(struct cgpu_info *cgpu)
{
    struct bm1370_info *info = bm1370_get_info(cgpu);

    /* Placeholder for hardware initialisation sequence */
    applog(LOG_INFO, "Initialising BM1370 device %d", cgpu->device_id);
    info->frequency = 500; /* MHz default */
    info->voltage = 800;   /* mV default */
}

bool bm1370_prepare_work(__maybe_unused struct thr_info *thr, __maybe_unused struct work *work)
{
    /* Real implementation would push work to the ASIC */
    return true;
}

int64_t bm1370_scanhash(__maybe_unused struct thr_info *thr, __maybe_unused struct work *work,
                       __maybe_unused int64_t max_nonce)
{
    /* Hardware hash scanning would go here */
    cgsleep_ms(50);
    return -1; /* no nonce found */
}

void bm1370_set_frequency(struct cgpu_info *cgpu, int frequency)
{
    struct bm1370_info *info = bm1370_get_info(cgpu);
    info->frequency = frequency;
    applog(LOG_DEBUG, "BM1370 %d frequency set to %d MHz", cgpu->device_id, frequency);
}

void bm1370_shutdown(struct thr_info *thr)
{
    if (!thr || !thr->cgpu)
        return;

    applog(LOG_INFO, "Shutting down BM1370 device %d", thr->cgpu->device_id);
}

void bm1370_get_statline_before(char *buf, size_t bufsiz, struct cgpu_info *cgpu)
{
    struct bm1370_info *info = bm1370_get_info(cgpu);
    tailsprintf(buf, bufsiz, "%dMHz %.1fC", info->frequency, info->temp);
}

struct api_data *bm1370_api_stats(struct cgpu_info *cgpu)
{
    struct bm1370_info *info = bm1370_get_info(cgpu);
    struct api_data *root = api_add_int(NULL, "frequency", info->frequency);
    api_add_int(root, "voltage", info->voltage);
    api_add_int(root, "chips", info->chips);
    api_add_double(root, "temp", info->temp);
    return root;
}

/* --- cgminer driver callbacks ------------------------------------------- */

static void bm1370_drv_detect(bool hotplug)
{
    struct cgpu_info *cgpu;

    if (hotplug)
        return; /* Hotplug not supported */

    cgpu = cgcalloc(1, sizeof(*cgpu));
    cgpu->drv = &bm1370_drv;
    cgpu->deven = DEV_ENABLED;
    cgpu->threads = 1;

    bm1370_detect(cgpu);
    bm1370_init(cgpu);

    add_cgpu(cgpu);

    applog(LOG_INFO, "BM1370 driver loaded (device %d)", cgpu->device_id);
}

static bool bm1370_prepare(struct thr_info *thr)
{
    if (!thr || !thr->cgpu)
        return false;

    bm1370_init(thr->cgpu);
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
    .drv_detect = bm1370_drv_detect,
    .thread_prepare = bm1370_prepare,
    .hash_work = hash_driver_work,
    .scanwork = bm1370_scanwork,
    .get_api_stats = bm1370_api_stats,
    .get_statline_before = bm1370_get_statline_before,
    .thread_shutdown = bm1370_shutdown
};

