#include <stdio.h>

#include "order.h"

int main(void)
{
    Order *order = create_order(1,ORDER_BUY,"RELIANCE",10,2950.50);

    if (order == NULL) {
        fprintf(stderr, "Failed to create order.\n");
        return 1;
    }

    print_order(order);

    destroy_order(order);
    order = NULL;

    return 0;
}