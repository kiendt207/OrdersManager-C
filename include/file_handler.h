#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "order.h"

int saveOrdersToFile(const char* file_path, Order* head);
Order* loadOrdersFromFile(const char* file_path);

#endif
