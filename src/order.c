#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "order.h"

Order * create_order(long id,OrderSide side,const char *symbol,int quantity,double price)
{
    if (id <= 0) {
        return NULL;
    }

    if (side != ORDER_BUY && side != ORDER_SELL) {
        return NULL;
    }

    if (symbol == NULL) {
        return NULL;
    }

    size_t symbol_length = strlen(symbol);

    if (symbol_length == 0 ||
        symbol_length >= SYMBOL_MAX_LENGTH) {
        return NULL;
    }

    if (quantity <= 0) {
        return NULL;
    }

    if (price <= 0.0) {
        return NULL;
    }

    Order *order = malloc(sizeof(Order));

    if (order == NULL) {
        return NULL;
    }

    order->id = id;
    order->side = side;
    order->quantity = quantity;
    order->price = price;

    snprintf(
        order->symbol,
        sizeof(order->symbol),
        "%s",
        symbol
    );

    return order;
}

void print_order(const Order *order)
{
    if (order == NULL) {
        return;
    }

    printf("Order ID: %ld\n", order->id);
    printf("Side: %s\n",order->side == ORDER_BUY ? "BUY" : "SELL");
    printf("Symbol: %s\n", order->symbol);
    printf("Quantity: %d\n", order->quantity);
    printf("Price: %.2f\n", order->price);
}

void destroy_order(Order *order)
{
    free(order);
}