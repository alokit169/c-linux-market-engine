#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <stddef.h>
#include "order.h"

typedef struct OrderNode{
    Order *order;
    struct OrderNode *next;
} OrderNode;

typedef struct{
    OrderNode *buy_head;
    OrderNode *sell_head;
    size_t order_count;
} OrderBook;

typedef struct {
    long buy_id;
    long sell_id;
    int quantity;
    double price;
} Trade;

int order_book_match_once(OrderBook *book, Trade *trade);

OrderBook * order_book_create(void);

int order_book_add(OrderBook *book, Order *order);
const Order *order_book_find(const OrderBook *book, long id);
int order_book_remove(OrderBook *book, long id);
int order_book_can_match(const OrderBook *book);
void order_book_destroy(OrderBook *book);

#endif