#ifndef MONTY_H
#define MONTY_H
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * struct stack_s - doubly linked list representation of a stack (or queue)
 * @n: integer
 * @prev: points to the previous element of the stack (or queue)
 * @next: points to the next element of the stack (or queue)
 *
 * Description: doubly linked list node structure
 * for stack, queues, LIFO, FIFO
 */
typedef struct stack_s
{
        int n;
        struct stack_s *prev;
        struct stack_s *next;
} stack_t;

/**
 * struct instruction_s - opcode and its function
 * @opcode: the opcode
 * @f: function to handle the opcode
 *
 * Description: opcode and its function
 * for stack, queues, LIFO, FIFO
 */
typedef struct instruction_s
{
        char *opcode;
        int (*f)(stack_t **stack);
} instruction_t;

void clear_stack(stack_t *stack);
int get_instruction(char *op_code, stack_t **stack);
stack_t *create_node(int n);
int push(stack_t **stack);
int pall(stack_t **stack);
int pint(stack_t **stack);
int pop(stack_t **stack);
int swap(stack_t **stack);
int add(stack_t **stack);
int nop(stack_t **stack);

#endif
