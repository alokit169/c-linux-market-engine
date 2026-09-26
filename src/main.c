#include <stdio.h>

#include "order.h"
#include "order_book.h"

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

    OrderBook *book = order_book_create();

    if (book == NULL) {
        fprintf(stderr, "Failed to create order book.\n");
        return 1;
    }

    if (book->buy_head != NULL ||
        book->sell_head != NULL ||
        book->order_count != 0) {
        fprintf(stderr, "New order book is not empty.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Empty order book created successfully.\n");

    Order *buy = create_order(2, ORDER_BUY, "RELIANCE", 5, 3000.00);

    if (buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, buy)) {
        destroy_order(buy);       /* Caller still owns it on failure. */
        order_book_destroy(book);
        return 1;
    }   

/* The book owns buy from here onward. */
    if (book->buy_head == NULL ||
        book->buy_head->order != buy ||
        book->buy_head->next != NULL ||
        book->sell_head != NULL ||
        book->order_count != 1) {
        fprintf(stderr, "Single-order book state is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }   

    printf("Single BUY order added successfully.\n");
   // print_order(buy);

        Order *higher_buy = create_order(
        3, ORDER_BUY, "RELIANCE", 4, 3100.00
    );

    if (higher_buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, higher_buy)) {
        destroy_order(higher_buy);
        order_book_destroy(book);
        return 1;
    }

    if (book->buy_head->order != higher_buy ||
        book->buy_head->next == NULL ||
        book->buy_head->next->order != buy ||
        book->buy_head->next->next != NULL ||
        book->order_count != 2) {
        fprintf(stderr, "BUY price order is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Higher-priced BUY inserted at front.\n");
    //print_order(higher_buy);

        Order *middle_buy = create_order(
        4, ORDER_BUY, "RELIANCE", 6, 3050.00
    );

    if (middle_buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, middle_buy)) {
        destroy_order(middle_buy);
        order_book_destroy(book);
        return 1;
    }

    if (book->buy_head->order != higher_buy ||
        book->buy_head->next == NULL ||
        book->buy_head->next->order != middle_buy ||
        book->buy_head->next->next == NULL ||
        book->buy_head->next->next->order != buy ||
        book->buy_head->next->next->next != NULL ||
        book->order_count != 3) {
        fprintf(stderr, "Middle BUY insertion is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Middle-priced BUY inserted correctly.\n");

        Order *equal_buy = create_order(
        5, ORDER_BUY, "RELIANCE", 7, 3050.00
    );

    if (equal_buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, equal_buy)) {
        destroy_order(equal_buy);
        order_book_destroy(book);
        return 1;
    }

    OrderNode *first_3050 = book->buy_head->next;

    if (first_3050->order != middle_buy ||
        first_3050->next == NULL ||
        first_3050->next->order != equal_buy ||
        first_3050->next->next == NULL ||
        first_3050->next->next->order != buy ||
        book->order_count != 4) {
        fprintf(stderr, "Equal-price BUY order is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Equal-price BUY kept arrival order.\n");

        Order *sell_high = create_order(
        6, ORDER_SELL, "RELIANCE", 3, 3200.00
    );
    Order *sell_low = create_order(
        7, ORDER_SELL, "RELIANCE", 2, 3150.00
    );

    if (sell_high == NULL || sell_low == NULL) {
        destroy_order(sell_high);
        destroy_order(sell_low);
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, sell_high)) {
        destroy_order(sell_high);
        destroy_order(sell_low);
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, sell_low)) {
        destroy_order(sell_low);
        order_book_destroy(book);
        return 1;
    }

    if (book->sell_head == NULL ||
        book->sell_head->order != sell_low ||
        book->sell_head->next == NULL ||
        book->sell_head->next->order != sell_high ||
        book->sell_head->next->next != NULL ||
        book->order_count != 6) {
        fprintf(stderr, "SELL price order is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Lower-priced SELL inserted at front.\n");

    const Order *found = order_book_find(book, 7);
    const Order *missing = order_book_find(book, 999);

    if (found != sell_low || missing != NULL) {
        fprintf(stderr, "Order search is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Order search found ID 7 and rejected missing ID 999.\n");

        if (!order_book_remove(book, 3) ||
        book->buy_head == NULL ||
        book->buy_head->order != middle_buy ||
        book->order_count != 5 ||
        order_book_find(book, 3) != NULL) {
        fprintf(stderr, "BUY head removal is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("BUY head removed correctly.\n");

        if (!order_book_remove(book, 5) ||
        book->buy_head->order != middle_buy ||
        book->buy_head->next == NULL ||
        book->buy_head->next->order != buy ||
        book->buy_head->next->next != NULL ||
        book->order_count != 4 ||
        order_book_find(book, 5) != NULL) {
        fprintf(stderr, "BUY middle removal is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("BUY middle node removed correctly.\n");

    if (!order_book_remove(book, 2) ||
        book->buy_head->order != middle_buy ||
        book->buy_head->next != NULL ||
        book->order_count != 3 ||
        order_book_find(book, 2) != NULL) {
        fprintf(stderr, "BUY tail removal is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("BUY tail removed correctly.\n");

        if (order_book_remove(book, 999) != 0 ||
        book->order_count != 3) {
        fprintf(stderr, "Missing-order removal changed the book.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Missing ID left the book unchanged.\n");

    if (!order_book_remove(book, 7) ||
        book->sell_head == NULL ||
        book->sell_head->order != sell_high ||
        book->sell_head->next != NULL ||
        book->order_count != 2 ||
        order_book_find(book, 7) != NULL) {
        fprintf(stderr, "SELL head removal is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("SELL head removed correctly.\n");


        Order *duplicate = create_order(
        4, ORDER_SELL, "RELIANCE", 1, 3300.00
    );

    if (duplicate == NULL) {
        order_book_destroy(book);
        return 1;
    }

    int added = order_book_add(book, duplicate);

    if (added != 0 ||
        book->order_count != 2 ||
        order_book_find(book, 4) != middle_buy) {
        fprintf(stderr, "Duplicate ID was not rejected.\n");
        if (!added) {
            destroy_order(duplicate);
        }
        order_book_destroy(book);
        return 1;
    }

    destroy_order(duplicate);
    printf("Duplicate ID rejected.\n");

    if (order_book_can_match(book)) {
        fprintf(stderr, "Non-crossing orders were marked matchable.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Non-crossing orders do not match.\n");

    Order *crossing_sell = create_order(8, ORDER_SELL, "RELIANCE", 12, 3040.00);

    if (crossing_sell == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, crossing_sell)) {
        destroy_order(crossing_sell);
        order_book_destroy(book);
        return 1;
    }

    if (book->sell_head->order != crossing_sell ||
        book->order_count != 3 ||
        !order_book_can_match(book)) {
        fprintf(stderr, "Crossing orders were not recognized.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Crossing BUY and SELL can match.\n");

    Trade trade;

    if (!order_book_match_once(book, &trade) ||
        trade.buy_id != 4 ||
        trade.sell_id != 8 ||
        trade.quantity != 6 ||
        trade.price != 3040.00 ||
        book->buy_head != NULL ||
        book->sell_head->order != crossing_sell ||
        crossing_sell->quantity != 6 ||
        book->order_count != 2 ||
        order_book_can_match(book)) {
        fprintf(stderr, "Single trade is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Trade: BUY %ld, SELL %ld, %d units at %.2f\n",
           trade.buy_id, trade.sell_id, trade.quantity, trade.price);    

    Order *exact_buy = create_order(
        9, ORDER_BUY, "RELIANCE", 6, 3050.00
    );

    if (exact_buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, exact_buy)) {
        destroy_order(exact_buy);
        order_book_destroy(book);
        return 1;
    }

    Trade exact_trade;

    if (!order_book_match_once(book, &exact_trade) ||
        exact_trade.buy_id != 9 ||
        exact_trade.sell_id != 8 ||
        exact_trade.quantity != 6 ||
        exact_trade.price != 3040.00 ||
        book->buy_head != NULL ||
        book->sell_head == NULL ||
        book->sell_head->order != sell_high ||
        book->order_count != 1) {
        fprintf(stderr, "Exact-fill trade is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Exact fill removed both matched orders.\n");

    Order *larger_buy = create_order(
        10, ORDER_BUY, "RELIANCE", 5, 3300.00
    );

    if (larger_buy == NULL) {
        order_book_destroy(book);
        return 1;
    }

    if (!order_book_add(book, larger_buy)) {
        destroy_order(larger_buy);
        order_book_destroy(book);
        return 1;
    }

    Trade final_trade;

    if (!order_book_match_once(book, &final_trade) ||
        final_trade.buy_id != 10 ||
        final_trade.sell_id != 6 ||
        final_trade.quantity != 3 ||
        final_trade.price != 3200.00 ||
        book->buy_head == NULL ||
        book->buy_head->order != larger_buy ||
        larger_buy->quantity != 2 ||
        book->sell_head != NULL ||
        book->order_count != 1) {
        fprintf(stderr, "Partial BUY fill is incorrect.\n");
        order_book_destroy(book);
        return 1;
    }

    printf("Partial fill left 2 units on the BUY order.\n");

    order_book_destroy(book);
    book = NULL;

    return 0;
}
