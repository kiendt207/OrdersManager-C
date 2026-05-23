#include "common.h"
#include "dish.h"
#include "order.h"
#include "kitchen.h"
#include "file_handler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_FILE "data/orders.txt"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void readStringInput(char* dest, int max_len) {
    if (dest == NULL || max_len <= 0) return;
    fgets(dest, max_len, stdin);
    dest[strcspn(dest, "\r\n")] = '\0';
}

const char* getTrangThaiDonHangString(TrangThaiDonHang tt) {
    switch (tt) {
        case DANG_PHUC_VU: return "DANG PHUC VU";
        case DA_THANH_TOAN: return "DA THANH TOAN";
        case DON_HUY: return "DON HUY";
        default: return "KHONG XAC DINH";
    }
}

const char* getTrangThaiMonAnString(TrangThaiMonAn tt) {
    switch (tt) {
        case MOI_TAO: return "MOI TAO";
        case DANG_LAM: return "DANG LAM";
        case DA_LAM_XONG: return "DA LAM XONG";
        case DA_TRA: return "DA TRA (SERVED)";
        case BI_HUY: return "BI HUY";
        default: return "KHONG XAC DINH";
    }
}

void displayMenu() {
    printf("\n=======================================================\n");
    printf("          HE THONG QUAN LY DON HANG NHA HANG           \n");
    printf("=======================================================\n");
    printf(" 1. Tao Don Hang Moi (Theo Ban)\n");
    printf(" 2. Dat Mon An Moi (Them mon vao Ban dang dung)\n");
    printf(" 3. Cap Nhat Trang Thai Mon An (Kitchen Workflow)\n");
    printf(" 4. Xem Danh Sach Ban & Don Hang Dang Hoat Dong\n");
    printf(" 5. In Hoa Don & Thanh Toan (Chot so thuc te)\n");
    printf(" 6. Luu Toan Bo Du Lieu vao Tap Tin data/orders.txt\n");
    printf(" 7. Nap Lai Du Lieu tu data/orders.txt\n");
    printf(" 8. Thoat Chuong Trinh (Xoa bo nho Heap)\n");
    printf("=======================================================\n");
    printf("Lua chon cua ban (1-8): ");
}

int main() {
    Order* head = NULL;
    int choice = 0;
    char temp_time[20];

    head = loadOrdersFromFile(DATA_FILE);
    if (head != NULL) {
        printf("[He thong] Da nap thanh cong du lieu tu file %s!\n", DATA_FILE);
    } else {
        printf("[He thong] Khong co du lieu san hoac file trong. Bat dau voi danh sach trong.\n");
    }

    while (1) {
        displayMenu();
        if (scanf("%d", &choice) != 1) {
            printf("[Loi] Vui long nhap mot so nguyen hop le!\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            // 1. Create new ORDER
            case 1: { 
                char ma_ban[20];
                char ma_nhan_vien[20];

                printf("Nhap ma ban (vi du: BAN01, BAN02): ");
                readStringInput(ma_ban, sizeof(ma_ban));
                if (strlen(ma_ban) == 0) {
                    printf("[Loi] Ma ban khong hop le!\n");
                    break;
                }

                // Check duplicate table
                Order* duplicate = findOrderByTable(head, ma_ban);
                if (duplicate != NULL) {
                    printf("%s hien tai dang co khach dung va chua thanh toan!\n", ma_ban);
                    break;
                }

                printf("Nhap ma nhan vien phuc vu: ");
                readStringInput(ma_nhan_vien, sizeof(ma_nhan_vien));

                getCurrentTimeString(temp_time, sizeof(temp_time));
                Order* new_order = createNewOrder(temp_time, ma_nhan_vien, ma_ban);
                if (new_order != NULL) {
                    insertOrderSorted(&head, new_order);
                    printf("[Thanh cong] Da mo hoa don moi cho %s luc %s.\n", ma_ban, temp_time);
                }
                break;
            }
            // 2. Order new dish
            case 2: {
                char ma_ban[20];
                char ma_mon[MAX_NAME];
                int so_luong = 0;
                char ghi_chu[MAX_NOTE];

                printf("Nhap ma ban muon dat mon: ");
                readStringInput(ma_ban, sizeof(ma_ban));

                // Check table
                Order* order = findOrderByTable(head, ma_ban);
                if (order == NULL) {
                    printf("[Thong bao] Khong tim thay don hang hoat dong nao cho %s.\n", ma_ban);
                    printf("Ban co muon tu dong mo hoa don moi cho %s khong? (y/n): ", ma_ban);
                    char confirm;
                    if (scanf("%c", &confirm) != 1) confirm = 'n';
                    clearInputBuffer();

                    if (confirm == 'y' || confirm == 'Y') {
                        char ma_nhan_vien[20];
                        printf("Nhap ma nhan vien phuc vu: ");
                        readStringInput(ma_nhan_vien, sizeof(ma_nhan_vien));
                        getCurrentTimeString(temp_time, sizeof(temp_time));
                        order = createNewOrder(temp_time, ma_nhan_vien, ma_ban);
                        if (order != NULL) {
                            insertOrderSorted(&head, order);
                            printf("[Thanh cong] Tu dong mo hoa don cho %s.\n", ma_ban);
                        } 
                        else {
                            printf("[Loi] Khong the tao hoa don moi!\n");
                            break;
                        }
                    } 
                    else {
                        printf("[Huy] Khong dat mon.\n");
                        break;
                    }
                }

                printf("Nhap ma mon an (vi du: M01, M02): ");
                readStringInput(ma_mon, sizeof(ma_mon));
                if (strlen(ma_mon) == 0) {
                    printf("[Loi] Ma mon khong duoc de trong!\n");
                    break;
                }

                printf("Nhap so luong dat: ");
                if (scanf("%d", &so_luong) != 1 || so_luong <= 0) {
                    printf("[Loi] So luong dat phai la so nguyen duong!\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();

                printf("Nhap ghi chu dat mon (neu co): ");
                readStringInput(ghi_chu, sizeof(ghi_chu));

                getCurrentTimeString(temp_time, sizeof(temp_time));
                Dish* new_dish = createNewDish(ma_mon, temp_time, so_luong, ghi_chu);
                if (new_dish != NULL) {
                    insertDishAtTail(&(order->danhSachMon), new_dish);

                    // Update order
                    order->tongSoMonDat++;
                    order->tongSoDiaDat += so_luong;
                    strncpy(order->thoiGianCapNhatCuoi, temp_time, sizeof(order->thoiGianCapNhatCuoi) - 1);
                    order->thoiGianCapNhatCuoi[sizeof(order->thoiGianCapNhatCuoi) - 1] = '\0';

                    printf("[Thanh cong] Da them mon %s (SL: %d) vao don cua %s luc %s.\n", ma_mon, so_luong, ma_ban, temp_time);
                }
                break;
            }
            // 3. Update dish status
            case 3: { 
                char ma_ban[20];
                char ma_mon[MAX_NAME];

                printf("Nhap ma ban can cap nhat: ");
                readStringInput(ma_ban, sizeof(ma_ban));

                Order* order = findOrderByTable(head, ma_ban);
                if (order == NULL) {
                    printf("[Loi] Khong tim thay don hang hoat dong cua ban %s!\n", ma_ban);
                    break;
                }

                if (order->danhSachMon == NULL) {
                    printf("[Thong bao] Don hang cua ban %s hien tai chua co mon an nao!\n", ma_ban);
                    break;
                }

                // Print dish list
                printf("--- Cac mon cua ban %s ---\n", ma_ban);
                Dish* temp = order->danhSachMon;
                while (temp != NULL) {
                    printf("  + Ma mon: %s | Dat: %d | Tra: %d | Trang thai: [%s] | Ghi chu: %s\n",
                           temp->maMon, temp->soLuongDat, temp->soLuongTra,
                           getTrangThaiMonAnString(temp->trangThai), temp->ghiChu);
                    temp = temp->next;
                }

                printf("Nhap ma mon muon thay doi trang thai: ");
                readStringInput(ma_mon, sizeof(ma_mon));

                // Check dish avalability
                Dish* target_dish = order->danhSachMon;
                while (target_dish != NULL) {
                    if (strcmp(target_dish->maMon, ma_mon) == 0) {
                        break;
                    }
                    target_dish = target_dish->next;
                }

                if (target_dish == NULL) {
                    printf("[Loi] Mon an %s khong co trong don hang cua %s!\n", ma_mon, ma_ban);
                    break;
                }

                printf("Chon trang thai moi muon chuyen doi:\n");
                printf(" 0. MOI_TAO (Moi dat)\n");
                printf(" 1. DANG_LAM (Dang nau)\n");
                printf(" 2. DA_LAM_XONG (Nau xong o bep)\n");
                printf(" 3. DA_TRA (Da bung be phuc vu)\n");
                printf(" 4. BI_HUY (Huy mon an - Xoa khoi danh sach)\n");
                printf("Lua chon cua ban (0-4): ");
                int status_val = 0;
                if (scanf("%d", &status_val) != 1 || status_val < 0 || status_val > 4) {
                    printf("[Loi] Chon lua khong hop le!\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();

                TrangThaiMonAn new_status = (TrangThaiMonAn)status_val;
                int count_served = 0;
                if (new_status == DA_TRA) {
                    printf("Nhap so luong dia muon tra thuc te (Nhap 0 de tra toan bo phan con lai %d dia): ",
                           target_dish->soLuongDat - target_dish->soLuongTra);
                    if (scanf("%d", &count_served) != 1 || count_served < 0) {
                        printf("[Loi] So luong dia khong hop le!\n");
                        clearInputBuffer();
                        break;
                    }
                    clearInputBuffer();
                }

                getCurrentTimeString(temp_time, sizeof(temp_time));
                updateDishStatus(order, ma_mon, new_status, count_served, temp_time);
                printf("[Thanh cong] Da cap nhat luc %s!\n", temp_time);
                break;
            }
            // 4. Check active table
            case 4: { 
                if (head == NULL) {
                    printf("[Thong bao] Hien tai khong co don hang nao trong danh sach.\n");
                    break;
                }

                printf("\n=================== DANH SACH DON HANG DANG HOAT DONG ===================\n");
                Order* curr_order = head;
                int count_active = 0;
                while (curr_order != NULL) {
                    if (curr_order->trangThai == DANG_PHUC_VU) {
                        count_active++;
                        printf("Ban: %-6s | NV: %-8s | Tao luc: %s | Cap nhat: %s\n",
                               curr_order->maBan, curr_order->maNhanVien,
                               curr_order->thoiGianTao, curr_order->thoiGianCapNhatCuoi);
                        printf("Danh sach mon an dat:\n");
                        
                        Dish* curr_dish = curr_order->danhSachMon;
                        if (curr_dish == NULL) {
                            printf("   (Chua co mon an nao duoc dat)\n");
                        } 
                        else {
                            while (curr_dish != NULL) {
                                double price = getDishPrice(curr_dish->maMon);
                                printf("   + %-10s | Dat: %2d | Tra: %2d | Don gia: %8.0f VND | Trang thai: [%s]\n",
                                       curr_dish->maMon, curr_dish->soLuongDat, curr_dish->soLuongTra,
                                       price, getTrangThaiMonAnString(curr_dish->trangThai));
                                curr_dish = curr_dish->next;
                            }
                        }
                        printf("-------------------------------------------------------------------------\n");
                    }
                    curr_order = curr_order->next;
                }
                if (count_active == 0) {
                    printf("[Thong bao] Khong co ban nao dang trong trang thai DANG PHUC VU.\n");
                }
                break;
            }
            // 5. Print bill
            case 5: {
                char ma_ban[20];
                printf("Nhap ma ban muon in hoa don thanh toan: ");
                readStringInput(ma_ban, sizeof(ma_ban));

                Order* order = findOrderByTable(head, ma_ban);
                if (order == NULL) {
                    printf("[Loi] Khong tim thay don hang hoat dong cua ban %s!\n", ma_ban);
                    break;
                }

                double total = calculateTotalBill(order);
                printf("\n=======================================================\n");
                printf("                HOA DON THANH TOAN                     \n");
                printf("=======================================================\n");
                printf("Ma Ban: %-20s | Nhan Vien: %s\n", order->maBan, order->maNhanVien);
                printf("Thoi Gian Mo: %-18s | Thanh Toan Luc: (Hien tai)\n", order->thoiGianTao);
                printf("-------------------------------------------------------\n");
                printf("%-12s | %-8s | %-8s | %-12s | %-12s\n", "Ma Mon", "Dat (dia)", "Tra (dia)", "Don Gia", "Thanh Tien");
                printf("-------------------------------------------------------\n");
                
                Dish* curr_dish = order->danhSachMon;
                while (curr_dish != NULL) {
                    if (curr_dish->trangThai != BI_HUY) {
                        double price = getDishPrice(curr_dish->maMon);
                        double sub_total = curr_dish->soLuongTra * price;
                        printf("%-12s | %8d | %8d | %10.0f d | %10.0f d\n",
                               curr_dish->maMon, curr_dish->soLuongDat, curr_dish->soLuongTra,
                               price, sub_total);
                    }
                    curr_dish = curr_dish->next;
                }
                printf("-------------------------------------------------------\n");
                printf(" TONG CONG THANH TOAN: %15.0f VND\n", total);
                printf("=======================================================\n");

                printf("Xac nhan thanh toan va đóng ban nay? (y/n): ");
                char confirm;
                if (scanf("%c", &confirm) != 1) confirm = 'n';
                clearInputBuffer();

                if (confirm == 'y' || confirm == 'Y') {
                    order->trangThai = DA_THANH_TOAN;
                    getCurrentTimeString(temp_time, sizeof(temp_time));
                    strncpy(order->thoiGianCapNhatCuoi, temp_time, sizeof(order->thoiGianCapNhatCuoi) - 1);
                    order->thoiGianCapNhatCuoi[sizeof(order->thoiGianCapNhatCuoi) - 1] = '\0';
                    printf("[Thanh cong] Ban %s da duoc thanh toan va dong hoa don luc %s.\n", ma_ban, temp_time);
                } 
                else {
                    printf("[Huy] Chua hoan thanh thanh toan.\n");
                }
                break;
            }
            // 6. Save orders to file
            case 6: { 
                if (saveOrdersToFile(DATA_FILE, head)) {
                    printf("[Thanh cong] Da luu toan bo du lieu ra file %s!\n", DATA_FILE);
                } 
                else {
                    printf("[Loi] Khong the luu du lieu ra file %s!\n", DATA_FILE);
                }
                break;
            }
            // 7. load orders from file
            case 7: { 
                printf("[Canh bao] Hanh dong nay se xoa toan bo du lieu dang thay doi trong bo nho RAM!\n");
                printf("Ban co chac chan muon nap lai du lieu tu file orders.txt? (y/n): ");
                char confirm;
                if (scanf("%c", &confirm) != 1) confirm = 'n';
                clearInputBuffer();

                if (confirm == 'y' || confirm == 'Y') {
                    head = freeAllOrders(head);
                    head = loadOrdersFromFile(DATA_FILE);
                    if (head != NULL) {
                        printf("[Thanh cong] Da nap lai du lieu tu file %s!\n", DATA_FILE);
                    } else {
                        printf("[Thong bao] Nap file hoan tat (File rong hoac khong ton tai).\n");
                    }
                }
                break;
            }
            // 8. Free all orders and exit
            case 8: { 
                printf("Dang luu lai du lieu truoc khi thoat...\n");
                saveOrdersToFile(DATA_FILE, head);
                printf("Dang giai phong bo nho RAM...\n");
                head = freeAllOrders(head);
                printf("[Thanh cong] Da don dep bo nho Heap sach se. Tam biet!\n");
                return 0;
            }

            default: {
                printf("[Loi] Lua chon khong hop le! Vui long nhap tu 1 den 8.\n");
                break;
            }
        }
    }

    return 0;
}
