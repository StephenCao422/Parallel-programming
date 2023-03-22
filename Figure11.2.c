#include <stdlib.h>
#include <stdio.h>
int x; // Value that gets updated.
typedef void notifyProcedure(int); // Type of notify proc.
struct element {
    notifyProcedure* listener; // Pointer to notify procedure.
    struct element* next; // Pointer to the next item.
};
typedef struct element element_t; // Type of list elements.
element_t* head = 0; // Pointer to start of list.
element_t* tail = 0; // Pointer to end of list.

// Procedure to add a listener.
void addListener(notifyProcedure* listener) {
    if (head == 0) {
        head = malloc(sizeof(element_t));
        head->listener = listener;
        head->next = 0;
        tail = head; 
    }   else {
        tail->next = malloc(sizeof(element_t));
        tail = tail->next;
        tail->listener = listener;
        tail->next = 0;
    }
}
// Procedure to update x.
void update(int newx) {
    x = newx;
    // Notify listeners.
    element_t* element = head;
    while (element != 0) {
        (*(element->listener))(newx);
        element = element->next;
    }
}
// Example of notify procedure.
void print(int arg) {
    printf("%d ", arg);
}



void procedure1(int arg) {
    int lock_a_acquired = 0;
    pthread_mutex_trylock(&lock_a);
    if (lock_a_acquired) {
        if (a == arg) {
            pthread_mutex_lock(&lock_b);
            procedure2(arg);
            pthread_mutex_unlock(&lock_b);
        }
        pthread_mutex_unlock(&lock_a);
  }
}


