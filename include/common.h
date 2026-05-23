#ifndef COMMON_H
#define COMMON_H

#define MAX_NAME 50
#define MAX_NOTE 100

// Order Status
typedef enum {
    DANG_PHUC_VU,
    DA_THANH_TOAN,
    DON_HUY
} TrangThaiDonHang;

// Dish Status
typedef enum {
    MOI_TAO,
    DANG_LAM,
    DA_LAM_XONG,
    DA_TRA,
    BI_HUY
} TrangThaiMonAn;

#endif
