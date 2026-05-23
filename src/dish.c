#include "dish.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Dish* createNewDish(const char* ma_mon, const char* thoi_gian, int so_luong_dat, const char* ghi_chu) {
    Dish* new_dish = (Dish*)malloc(sizeof(Dish));
    
    //Check malloc
    if (new_dish == NULL) {
        printf("Loi: Khong the cap phat bo nho cho mon an moi\n");
        return NULL;
    }

    if (ma_mon != NULL) {
        strncpy(new_dish->maMon, ma_mon, MAX_NAME - 1);
        new_dish->maMon[MAX_NAME - 1] = '\0'; 
    } 
    else {
        new_dish->maMon[0] = '\0';
    }

    if (thoi_gian != NULL) {
        strncpy(new_dish->thoiGian, thoi_gian, sizeof(new_dish->thoiGian) - 1);
        new_dish->thoiGian[sizeof(new_dish->thoiGian) - 1] = '\0';
        
        strncpy(new_dish->thoiGianCapNhat, thoi_gian, sizeof(new_dish->thoiGianCapNhat) - 1);
        new_dish->thoiGianCapNhat[sizeof(new_dish->thoiGianCapNhat) - 1] = '\0';
    } 
    else {
        new_dish->thoiGian[0] = '\0';
        new_dish->thoiGianCapNhat[0] = '\0';
    }

    new_dish->soLuongDat = so_luong_dat;
    new_dish->soLuongTra = 0;

    if (ghi_chu != NULL) {
        strncpy(new_dish->ghiChu, ghi_chu, MAX_NOTE - 1);
        new_dish->ghiChu[MAX_NOTE - 1] = '\0';
    } else {
        new_dish->ghiChu[0] = '\0';
    }

    new_dish->trangThai = MOI_TAO;
    new_dish->next = NULL;

    return new_dish;
}

void insertDishAtTail(Dish** head, Dish* new_dish) {
    if (head == NULL || new_dish == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = new_dish;
        return;
    }

    Dish* current_node = *head;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = new_dish;
}
