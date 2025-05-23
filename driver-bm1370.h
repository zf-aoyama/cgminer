#ifndef BM1370_H
#define BM1370_H

#include "miner.h"

/* Device specific information for BM1370 ASICs */
struct bm1370_info {
    int chips;           /* number of chips in the chain */
    int frequency;       /* operating frequency in MHz */
    int voltage;         /* current core voltage */
    double temp;         /* board temperature */
    uint32_t version_mask; /* version rolling mask */
};

/* Driver API functions */
bool bm1370_detect(struct cgpu_info *cgpu);
void bm1370_init(struct cgpu_info *cgpu);
bool bm1370_prepare_work(struct thr_info *thr, struct work *work);
int64_t bm1370_scanhash(struct thr_info *thr, struct work *work, int64_t max_nonce);
void bm1370_set_frequency(struct cgpu_info *cgpu, int frequency);
void bm1370_shutdown(struct thr_info *thr);
void bm1370_get_statline_before(char *buf, size_t bufsiz, struct cgpu_info *cgpu);
struct api_data *bm1370_api_stats(struct cgpu_info *cgpu);

extern struct device_drv bm1370_drv;

#endif /* BM1370_H */
