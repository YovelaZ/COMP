#pragma once
#ifndef BT_VISUAL_H
#define BT_VISUAL_H

#include "bucket.h"
#include "SWS.h"

void init_bt(statistics& stat);
void init_PREFIX_tuning(statistics& stat);
void PREFIX_tuning(statistics& stat);

void SWS_bt(statistics& stat);
void SS_bt(statistics& stat);

#endif