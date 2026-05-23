#include "common.h"
#include "dish.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void testCreateNewDish() {
    Dish* d = createNewDish("MON01", "2026-05-23 12:00:00", 2, "It hanh");
    assert(d != NULL);
    assert(strcmp(d->maMon, "MON01") == 0);
    assert(d->soLuongDat == 2);
    assert(d->soLuongTra == 0);
    assert(d->trangThai == MOI_TAO);
    assert(strcmp(d->ghiChu, "It hanh") == 0);
    assert(d->next == NULL);
    
    // Giải phóng bộ nhớ và gán NULL
    free(d);
    d = NULL;
    printf("testCreateNewDish passed!\n");
}

int main() {
    printf("Running Dish tests...\n");
    testCreateNewDish();
    printf("All Dish tests passed successfully!\n");
    return 0;
}
