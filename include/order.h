#ifndef ORDER_H
#define ORDER_H

#define SYMBOL_MAX_LENGTH 16

typedef enum{
    ORDER_BUY,
    ORDER_SELL
} OrderSide;

typedef struct{
    long id;
    OrderSide side;
    char symbol[SYMBOL_MAX_LENGTH];
    int quantity;
    double price;
}Order;

Order * create_order(long id,OrderSide side,const char *symbol,int quantity,double price);

void print_order(const Order *order);

void destroy_order(Order *order);

#endif