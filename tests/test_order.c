#include "common.h"
#include "order.h"
#include "dish.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void testCreateNewOrder() {
    Order* o = createNewOrder("2026-05-23 12:00:00", "NV001", "BAN01");
    assert(o != NULL);
    assert(strcmp(o->maBan, "BAN01") == 0);
    assert(strcmp(o->maNhanVien, "NV001") == 0);
    assert(o->danhSachMon == NULL);
    assert(o->tongSoMonDat == 0);
    assert(o->trangThai == DANG_PHUC_VU);
    
    // Giải phóng bộ nhớ và gán NULL
    free(o);
    o = NULL;
    printf("testCreateNewOrder passed!\n");
}

int main() {
    printf("Running Order tests...\n");
    testCreateNewOrder();
    printf("All Order tests passed successfully!\n");
    return 0;
}
