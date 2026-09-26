#include <stdlib.h>
#include <string.h>

#include "order_book.h"

OrderBook *order_book_create(void)
{
    OrderBook *book = malloc(sizeof(OrderBook));

    if (book == NULL) {
        return NULL;
    }

    book->buy_head = NULL;
    book->sell_head = NULL;
    book->order_count = 0;

    return book;
}

int order_book_add(OrderBook *book, Order *order){
    if(book == NULL || order == NULL){
        return 0;
    }

    if (order->side != ORDER_BUY &&
        order->side != ORDER_SELL) {
        return 0;
    }
    
    if (order_book_find(book, order->id) != NULL) {
        return 0;
    }

    OrderNode *new_node = malloc(sizeof(OrderNode));

    if(new_node == NULL)
    {
        return 0;
    }

    new_node->order = order;
    new_node->next = NULL;

    OrderNode **head;

    if(order->side == ORDER_BUY){
        head = &book->buy_head;
    }else{
        head = &book->sell_head;
    }
    
    // Empty List
    if(*head == NULL){
        *head = new_node;
        book->order_count++;
        return 1;
    }

    int insert_at_front;

     if (order->side == ORDER_BUY) {
        insert_at_front = order->price > (*head)->order->price;
    } else {
        insert_at_front = order->price < (*head)->order->price;
    }

    if(insert_at_front){
        new_node->next= *head;
        *head= new_node;
        book->order_count++;
        return 1;
    }

    OrderNode *current = *head;

    while(current->next != NULL){
        double next_price = current->next->order->price;
        int next_has_priority;

        if(order->side == ORDER_BUY){
            next_has_priority = next_price >= order->price;
        }else{
            next_has_priority = next_price <= order->price;
        }
        if(!next_has_priority){
            break;
        }
        current = current->next;
    }
    
    new_node->next = current->next;
    current->next = new_node;
     
    book->order_count++;
   
    return 1;
}

const Order *order_book_find(const OrderBook *book, long id)
{
    if (book == NULL || id <= 0) {
        return NULL;
    }

    const OrderNode *current = book->buy_head;

    while (current != NULL) {
        if (current->order->id == id) {
            return current->order;
        }
        current = current->next;
    }

    current = book->sell_head;

    while (current != NULL) {
        if (current->order->id == id) {
            return current->order;
        }
        current = current->next;
    }

    return NULL;
}

static int remove_from_list(OrderNode **head, long id)
{
    OrderNode **link = head;

    while (*link != NULL) {
        OrderNode *current = *link;

        if (current->order->id == id) {
            *link = current->next;
            destroy_order(current->order);
            free(current);
            return 1;
        }

        link = &current->next;
    }

    return 0;
}

int order_book_remove(OrderBook *book, long id)
{
    if (book == NULL || id <= 0) {
        return 0;
    }

    if (remove_from_list(&book->buy_head, id) ||
        remove_from_list(&book->sell_head, id)) {
        book->order_count--;
        return 1;
    }

    return 0;
}

int order_book_can_match(const OrderBook *book)
{
    if (book == NULL ||
        book->buy_head == NULL ||
        book->sell_head == NULL) {
        return 0;
    }

    const Order *buy = book->buy_head->order;
    const Order *sell = book->sell_head->order;

    return strcmp(buy->symbol, sell->symbol) == 0 &&
           buy->price >= sell->price;
}


int order_book_match_once(OrderBook *book, Trade *trade)
{
    if (trade == NULL || !order_book_can_match(book)) {
        return 0;
    }

    Order *buy = book->buy_head->order;
    Order *sell = book->sell_head->order;

    int quantity = buy->quantity < sell->quantity
        ? buy->quantity
        : sell->quantity;

    /* Save the result before a filled order may be freed. */
    trade->buy_id = buy->id;
    trade->sell_id = sell->id;
    trade->quantity = quantity;
    trade->price = sell->price;

    buy->quantity -= quantity;
    sell->quantity -= quantity;

    if (buy->quantity == 0) {
        order_book_remove(book, trade->buy_id);
    }

    if (sell->quantity == 0) {
        order_book_remove(book, trade->sell_id);
    }

    return 1;
}

void order_book_destroy(OrderBook *book)
{
    if (book == NULL) {
        return;
    }

    OrderNode *current = book->buy_head;

    while (current != NULL) {
        OrderNode *next = current->next;
        destroy_order(current->order);
        free(current);
        current = next;
    }

    current = book->sell_head;

    while (current != NULL) {
        OrderNode *next = current->next;
        destroy_order(current->order);
        free(current);
        current = next;
    }

    free(book);
}