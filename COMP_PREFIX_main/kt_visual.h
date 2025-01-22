#pragma once
#ifndef KT_VISUAL_H
#define KT_VISUAL_H

#include "bucket.h"
#include "SWS.h"

void init_kf(statistics& stat);
void PREFIX_multiple(statistics& stat);
void SWS_kt(statistics& stat);

//Used for checking the correctness of the results
void init_check_multiple_time(statistics& stat);

#endif