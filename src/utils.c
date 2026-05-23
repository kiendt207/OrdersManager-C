#include "utils.h"
#include <string.h>
#include <time.h>

double getDishPrice(const char* ma_mon) {
    if (ma_mon == NULL) {
        return 0.0;
    }

    if (strcmp(ma_mon, "MON01") == 0 || strcmp(ma_mon, "M01") == 0) return 45000.0;  // Phở Bò
    if (strcmp(ma_mon, "MON02") == 0 || strcmp(ma_mon, "M02") == 0) return 50000.0;  // Bún Chả
    if (strcmp(ma_mon, "MON03") == 0 || strcmp(ma_mon, "M03") == 0) return 35000.0;  // Cơm Tấm
    if (strcmp(ma_mon, "MON04") == 0 || strcmp(ma_mon, "M04") == 0) return 15000.0;  // Nước Ngọt Coca
    if (strcmp(ma_mon, "MON05") == 0 || strcmp(ma_mon, "M05") == 0) return 20000.0;  // Bánh Mì

    return 30000.0;
}

void getCurrentTimeString(char* dest, int max_len) {
    if (dest == NULL || max_len <= 0) {
        return;
    }
    time_t raw_time = time(NULL);
    struct tm* time_info = localtime(&raw_time);
    if (time_info != NULL) {
        strftime(dest, max_len, "%Y-%m-%d %H:%M:%S", time_info);
    } else {
        strncpy(dest, "N/A", max_len - 1);
        dest[max_len - 1] = '\0';
    }
}
