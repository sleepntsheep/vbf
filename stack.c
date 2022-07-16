#include "stack.h"
#include <stdlib.h>

Stack *stack_init(void *ptr) {
    return stack_push(NULL, ptr);
}

Stack *stack_push(Stack *stack, void *ptr) {
    Stack *new = malloc(sizeof(Stack));
    new->btm = stack;
    new->ptr = ptr;
    return new;
}

Stack *stack_pop(Stack *stack) {
    Stack *btm = stack->btm;
    free(stack);
    return btm;
}

