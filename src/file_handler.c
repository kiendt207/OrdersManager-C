#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Save ORDER & DISH Information to .txt file
 * Use '|' for easy tracking
 */
int saveOrdersToFile(const char* file_path, Order* head) {
    if (file_path == NULL) {
        return 0;
    }

    FILE* file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Loi: Khong the mo file de ghi du lieu");
        return 0;
    }

    Order* curr_order = head;
    while (curr_order != NULL) {
        //Order Information
        fprintf(file, "ORDER|%s|%s|%s|%d|%d|%d|%d|%s|%d\n",
                curr_order->thoiGianTao,
                curr_order->maNhanVien,
                curr_order->maBan,
                curr_order->tongSoMonDat,
                curr_order->tongSoDiaDat,
                curr_order->tongSoMonTra,
                curr_order->tongSoDiaTra,
                curr_order->thoiGianCapNhatCuoi,
                curr_order->trangThai);

        // Dish Information
        Dish* curr_dish = curr_order->danhSachMon;
        while (curr_dish != NULL) {
            char safe_ghi_chu[MAX_NOTE];
            strncpy(safe_ghi_chu, curr_dish->ghiChu, sizeof(safe_ghi_chu) - 1);
            safe_ghi_chu[sizeof(safe_ghi_chu) - 1] = '\0';
            for (int i = 0; safe_ghi_chu[i] != '\0'; i++) {
                if (safe_ghi_chu[i] == '|') {
                    safe_ghi_chu[i] = ' ';
                }
            }

            fprintf(file, "DISH|%s|%s|%d|%d|%s|%s|%d\n",
                    curr_dish->maMon,
                    curr_dish->thoiGian,
                    curr_dish->soLuongDat,
                    curr_dish->soLuongTra,
                    curr_dish->thoiGianCapNhat,
                    safe_ghi_chu,
                    curr_dish->trangThai);
            curr_dish = curr_dish->next;
        }
        curr_order = curr_order->next;
    }

    fclose(file);
    return 1;
}

/**
 * Load all ORDERS & DISHES from file.
 * Reconstruct data
 */
Order* loadOrdersFromFile(const char* file_path) {
    if (file_path == NULL) {
        return NULL;
    }

    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        return NULL;
    }

    Order* head = NULL;
    Order* tail = NULL;
    Order* last_order = NULL;

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        char* token = strtok(line, "|");
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, "ORDER") == 0) {
            char thoi_gian_tao[20] = "";
            char ma_nhan_vien[20] = "";
            char ma_ban[20] = "";
            int tong_so_mon_dat = 0;
            int tong_so_dia_dat = 0;
            int tong_so_mon_tra = 0;
            int tong_so_dia_tra = 0;
            char thoi_gian_cap_nhat_cuoi[20] = "";
            int trang_thai_don = 0;

            char* t = strtok(NULL, "|");
            if (t) strncpy(thoi_gian_tao, t, sizeof(thoi_gian_tao) - 1);
            t = strtok(NULL, "|");
            if (t) strncpy(ma_nhan_vien, t, sizeof(ma_nhan_vien) - 1);
            t = strtok(NULL, "|");
            if (t) strncpy(ma_ban, t, sizeof(ma_ban) - 1);

            t = strtok(NULL, "|");
            if (t) tong_so_mon_dat = atoi(t);
            t = strtok(NULL, "|");
            if (t) tong_so_dia_dat = atoi(t);
            t = strtok(NULL, "|");
            if (t) tong_so_mon_tra = atoi(t);
            t = strtok(NULL, "|");
            if (t) tong_so_dia_tra = atoi(t);

            t = strtok(NULL, "|");
            if (t) strncpy(thoi_gian_cap_nhat_cuoi, t, sizeof(thoi_gian_cap_nhat_cuoi) - 1);

            t = strtok(NULL, "|");
            if (t) trang_thai_don = atoi(t);

            Order* new_order = createNewOrder(thoi_gian_tao, ma_nhan_vien, ma_ban);
            if (new_order != NULL) {
                new_order->tongSoMonDat = tong_so_mon_dat;
                new_order->tongSoDiaDat = tong_so_dia_dat;
                new_order->tongSoMonTra = tong_so_mon_tra;
                new_order->tongSoDiaTra = tong_so_dia_tra;
                strncpy(new_order->thoiGianCapNhatCuoi, thoi_gian_cap_nhat_cuoi, sizeof(new_order->thoiGianCapNhatCuoi) - 1);
                new_order->trangThai = (TrangThaiDonHang)trang_thai_don;

                if (head == NULL) {
                    head = new_order;
                    tail = new_order;
                } 
                else {
                    tail->next = new_order;
                    tail = new_order;
                }
                last_order = new_order;
            }
        } 
        else if (strcmp(token, "DISH") == 0) {
            if (last_order == NULL) {
                continue;
            }

            char ma_mon[MAX_NAME] = "";
            char thoi_gian[20] = "";
            int so_luong_dat = 0;
            int so_luong_tra = 0;
            char thoi_gian_cap_nhat[20] = "";
            char ghi_chu[MAX_NOTE] = "";
            int trang_thai_mon = 0;

            char* t = strtok(NULL, "|");
            if (t) strncpy(ma_mon, t, sizeof(ma_mon) - 1);
            t = strtok(NULL, "|");
            if (t) strncpy(thoi_gian, t, sizeof(thoi_gian) - 1);

            t = strtok(NULL, "|");
            if (t) so_luong_dat = atoi(t);
            t = strtok(NULL, "|");
            if (t) so_luong_tra = atoi(t);

            t = strtok(NULL, "|");
            if (t) strncpy(thoi_gian_cap_nhat, t, sizeof(thoi_gian_cap_nhat) - 1);
            
            t = strtok(NULL, "|");
            if (t) {
                strncpy(ghi_chu, t, sizeof(ghi_chu) - 1);
            } 
            else {
                ghi_chu[0] = '\0';
            }

            t = strtok(NULL, "|");
            if (t) trang_thai_mon = atoi(t);

            Dish* new_dish = createNewDish(ma_mon, thoi_gian, so_luong_dat, ghi_chu);
            if (new_dish != NULL) {
                new_dish->soLuongTra = so_luong_tra;
                strncpy(new_dish->thoiGianCapNhat, thoi_gian_cap_nhat, sizeof(new_dish->thoiGianCapNhat) - 1);
                new_dish->trangThai = (TrangThaiMonAn)trang_thai_mon;

                insertDishAtTail(&(last_order->danhSachMon), new_dish);
            }
        }
    }

    fclose(file);
    return head;
}
