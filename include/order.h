#ifndef ORDER_H
#define ORDER_H

#include "common.h"
#include "dish.h"

// Order Node Structure
typedef struct Order {
    char thoiGianTao[20];
    char maNhanVien[MAX_NAME];
    char maBan[20];
    struct Dish* danhSachMon;
    int tongSoMonDat;
    int tongSoDiaDat;
    int tongSoMonTra;
    int tongSoDiaTra;
    char thoiGianCapNhatCuoi[20];
    TrangThaiDonHang trangThai;
    struct Order* next;
} Order;

Order* createNewOrder(const char* thoi_gian_tao, const char* ma_nhan_vien, const char* ma_ban);
void insertOrderSorted(Order** head, Order* new_order);
Order* findOrderByTable(Order* head, const char* ma_ban);
double calculateTotalBill(Order* order);
Order* freeAllOrders(Order* head);

#endif
