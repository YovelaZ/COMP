#pragma once
#ifndef SS_H
#define SS_H

#include "init_visual.h"

double spatial_kernel(double*q, double*p, statistics& stat);
double temporal_kernel(double*q, double*p, statistics& stat);
double SCAN_W_t(statistics& stat);
double SCAN(statistics& stat);

#endif