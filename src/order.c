#include "order.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Order* createNewOrder(const char* thoi_gian_tao, const char* ma_nhan_vien, const char* ma_ban) {
    Order* new_order = (Order*)malloc(sizeof(Order));
    
    //Check malloc
    if (new_order == NULL) {
        printf("Loi: Khong the cap phat bo nho cho don hang moi");
        return NULL;
    }

    if (thoi_gian_tao != NULL) {
        strncpy(new_order->thoiGianTao, thoi_gian_tao, sizeof(new_order->thoiGianTao) - 1);
        new_order->thoiGianTao[sizeof(new_order->thoiGianTao) - 1] = '\0';
        
        strncpy(new_order->thoiGianCapNhatCuoi, thoi_gian_tao, sizeof(new_order->thoiGianCapNhatCuoi) - 1);
        new_order->thoiGianCapNhatCuoi[sizeof(new_order->thoiGianCapNhatCuoi) - 1] = '\0';
    } 
    else {
        new_order->thoiGianTao[0] = '\0';
        new_order->thoiGianCapNhatCuoi[0] = '\0';
    }

    if (ma_nhan_vien != NULL) {
        strncpy(new_order->maNhanVien, ma_nhan_vien, sizeof(new_order->maNhanVien) - 1);
        new_order->maNhanVien[sizeof(new_order->maNhanVien) - 1] = '\0';
    } 
    else {
        new_order->maNhanVien[0] = '\0';
    }

    if (ma_ban != NULL) {
        strncpy(new_order->maBan, ma_ban, sizeof(new_order->maBan) - 1);
        new_order->maBan[sizeof(new_order->maBan) - 1] = '\0';
    } else {
        new_order->maBan[0] = '\0';
    }

    new_order->danhSachMon = NULL;
    new_order->tongSoMonDat = 0;
    new_order->tongSoDiaDat = 0;
    new_order->tongSoMonTra = 0;
    new_order->tongSoDiaTra = 0;
    new_order->trangThai = DANG_PHUC_VU;
    new_order->next = NULL;

    return new_order;
}

/**
 * Chèn đơn hàng mới vào danh sách liên kết các đơn hàng, sắp xếp tăng dần theo mã bàn (maBan).
 * Giúp quản lý danh sách các bàn gọn gàng và dễ tra cứu.
 */
void insertOrderSorted(Order** head, Order* new_order) {
    if (head == NULL || new_order == NULL) {
        return;
    }

    // Trường hợp danh sách đơn hàng trống
    if (*head == NULL) {
        *head = new_order;
        return;
    }

    // Trường hợp đơn hàng mới có mã bàn nhỏ hơn đầu danh sách (chèn vào đầu)
    if (strcmp(new_order->maBan, (*head)->maBan) < 0) {
        new_order->next = *head;
        *head = new_order;
        return;
    }

    // Duyệt tìm vị trí thích hợp để chèn (giữa hoặc cuối danh sách)
    Order* current_node = *head;
    while (current_node->next != NULL && strcmp(current_node->next->maBan, new_order->maBan) < 0) {
        current_node = current_node->next;
    }

    // Thực hiện liên kết chèn node
    new_order->next = current_node->next;
    current_node->next = new_order;
}

/**
 * Tìm kiếm đơn hàng của một bàn cụ thể đang trong trạng thái hoạt động ("ĐANG_PHUC_VU").
 * Trả về con trỏ tới Order của bàn đó nếu thấy, ngược lại trả về NULL.
 */
Order* findOrderByTable(Order* head, const char* ma_ban) {
    if (head == NULL || ma_ban == NULL) {
        return NULL;
    }

    Order* current_node = head;
    while (current_node != NULL) {
        // Chỉ tìm những đơn hàng có cùng mã bàn và đang hoạt động (Đang phục vụ)
        if (strcmp(current_node->maBan, ma_ban) == 0 && current_node->trangThai == DANG_PHUC_VU) {
            return current_node;
        }
        current_node = current_node->next;
    }

    return NULL;
}

/**
 * Tính tổng tiền hóa đơn cho đơn hàng.
 * Logic tính tiền dựa trên số lượng đĩa thực tế mà nhà bếp đã làm và trả cho khách (soLuongTra).
 * Hoàn toàn không tính tiền các món ăn đã bị hủy (BI_HUY).
 */
double calculateTotalBill(Order* order) {
    if (order == NULL || order->danhSachMon == NULL) {
        return 0.0;
    }

    double total_bill = 0.0;
    Dish* curr = order->danhSachMon;

    while (curr != NULL) {
        // Ràng buộc: Không tính tiền món bị hủy
        if (curr->trangThai != BI_HUY) {
            double price = getDishPrice(curr->maMon);
            // Dựa trên số lượng đĩa thực tế đã làm và trả (soLuongTra)
            total_bill += curr->soLuongTra * price;
        }
        curr = curr->next;
    }

    return total_bill;
}

/**
 * Giải phóng sạch sẽ toàn bộ danh sách liên kết các đơn hàng và danh sách món ăn lồng bên trong.
 * Đảm bảo gán lại tất cả các con trỏ sau khi free để ngăn chặn rò rỉ bộ nhớ.
 * Trả về NULL để người gọi có thể gán con trỏ gốc về NULL một cách an toàn.
 */
Order* freeAllOrders(Order* head) {
    Order* curr_order = head;
    while (curr_order != NULL) {
        Order* next_order = curr_order->next;

        // Giải phóng danh sách các món ăn con lồng bên trong Order này
        Dish* curr_dish = curr_order->danhSachMon;
        while (curr_dish != NULL) {
            Dish* next_dish = curr_dish->next;
            free(curr_dish);
            curr_dish = NULL; // Gán NULL ngay sau khi free nút con
            curr_dish = next_dish;
        }
        curr_order->danhSachMon = NULL; // Đảm bảo thuộc tính trỏ tới danh sách con được xóa

        // Giải phóng đơn hàng hiện tại
        free(curr_order);
        curr_order = NULL; // Gán NULL ngay sau khi free nút đơn hàng
        
        curr_order = next_order;
    }
    return NULL; // Luôn trả về NULL
}
