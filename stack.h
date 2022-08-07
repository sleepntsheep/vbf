#pragma once
#ifndef S_STACK_H
#define S_STACK_H

typedef struct stack Stack;

struct stack {
    struct stack *btm;
    void *ptr;
};

Stack *stack_init(void *ptr);

Stack *stack_push(Stack *stack, void *ptr);

Stack *stack_pop(Stack *stack);

#endif  /* S_STACK_H */
