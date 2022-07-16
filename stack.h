#pragma once

typedef struct stack Stack;

struct stack {
    struct stack *btm;
    void *ptr;
};

Stack *stack_init(void *ptr);

Stack *stack_push(Stack *stack, void *ptr);

Stack *stack_pop(Stack *stack);

