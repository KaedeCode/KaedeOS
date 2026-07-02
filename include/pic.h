#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ICW();
void enable_pic(unsigned char pic);
extern unsigned char master_mask;
extern unsigned char slave_mask;


#ifdef __cplusplus
}
#endif