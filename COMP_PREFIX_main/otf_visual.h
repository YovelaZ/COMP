#pragma once
#ifndef OTF_VISUAL_H
#define OTF_VISUAL_H

#include "init_visual.h"
#include "SS.h"
#include "bucket.h"

void init_otf(statistics& stat);
void SS_otf(statistics& stat);
void SWS_otf(statistics& stat);
void PREFIX_single(statistics& stat);

#endif