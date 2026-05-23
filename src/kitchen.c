#include "kitchen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Update DISH status and sync data from ORDER
 * Delete DISH in different positions in linked list
 */
void updateDishStatus(Order* order, const char* ma_mon, TrangThaiMonAn new_status, int so_luong, const char* thoi_gian_cap_nhat) {
    if (order == NULL || ma_mon == NULL) {
        return;
    }

    if (thoi_gian_cap_nhat != NULL) {
        strncpy(order->thoiGianCapNhatCuoi, thoi_gian_cap_nhat, sizeof(order->thoiGianCapNhatCuoi) - 1);
        order->thoiGianCapNhatCuoi[sizeof(order->thoiGianCapNhatCuoi) - 1] = '\0';
    }

    // STATUS: BI_HUY
    if (new_status == BI_HUY) {
        Dish* prev = NULL;
        Dish* curr = order->danhSachMon;

        while (curr != NULL) {
            if (strcmp(curr->maMon, ma_mon) == 0) {
                order->tongSoDiaDat -= curr->soLuongDat;
                order->tongSoDiaTra -= curr->soLuongTra;
                order->tongSoMonDat--;
                if (curr->soLuongTra > 0) {
                    order->tongSoMonTra--;
                }

                if (prev == NULL) {
                    order->danhSachMon = curr->next;
                } 
                else {
                    prev->next = curr->next;
                }

                free(curr);
                curr = NULL;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        return;
    }

    // STATUS: DANG_LAM, DA_LAM_XONG, DA_TRA
    Dish* curr = order->danhSachMon;
    while (curr != NULL) {
        if (strcmp(curr->maMon, ma_mon) == 0) {
            if (thoi_gian_cap_nhat != NULL) {
                strncpy(curr->thoiGianCapNhat, thoi_gian_cap_nhat, sizeof(curr->thoiGianCapNhat) - 1);
                curr->thoiGianCapNhat[sizeof(curr->thoiGianCapNhat) - 1] = '\0';
            }

            curr->trangThai = new_status;

            // DA_TRA
            if (new_status == DA_TRA) {
                int so_luong_served;

                if(so_luong > 0){
                    so_luong_served = so_luong;
                }
                else{
                    so_luong_served = curr->soLuongDat - curr->soLuongTra;
                }

                int so_luong_con_lai = curr->soLuongDat - curr->soLuongTra;
                if (so_luong_served > so_luong_con_lai) {
                    so_luong_served = so_luong_con_lai;
                }

                if (so_luong_served > 0) {
                    if (curr->soLuongTra == 0) {
                        order->tongSoMonTra++;
                    }
                    
                    curr->soLuongTra += so_luong_served;
                    order->tongSoDiaTra += so_luong_served;
                }
            }
            return;
        }
        curr = curr->next;
    }
}
