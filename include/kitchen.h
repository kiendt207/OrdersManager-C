#ifndef KITCHEN_H
#define KITCHEN_H

#include "common.h"
#include "order.h"
#include "dish.h"

void updateDishStatus(Order* order, const char* ma_mon, TrangThaiMonAn new_status, int so_luong, const char* thoi_gian_cap_nhat);

#endif
