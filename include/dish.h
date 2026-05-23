#ifndef DISH_H
#define DISH_H

#include "common.h"

typedef struct Dish {
    char maMon[MAX_NAME];
    char thoiGian[20];
    int soLuongDat;
    int soLuongTra;
    char thoiGianCapNhat[20];
    char ghiChu[MAX_NOTE];
    TrangThaiMonAn trangThai;
    struct Dish* next;
} Dish;

Dish* createNewDish(const char* ma_mon, const char* thoi_gian, int so_luong_dat, const char* ghi_chu);
void insertDishAtTail(Dish** head, Dish* new_dish);

#endif