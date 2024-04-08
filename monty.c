#include "monty.h"
#define STACK_SIZE 1000
#define EXIT_FAILURE 1

/**
 * get_instruction- determines the instruction to be called
 * @stack: pointer to the stack
 * @arg_str: sting used to determine the instruction to call
 * @line_number: the current line number in the file
*/

void get_instruction(stack_t **stack, char arg_str, unsigned int line_number)
{
	instruction_t instructions[] = {
		{"push", &push},
		{"pall", &pall},
		{NULL, NULL}
		};

	while (fgets(line, sizeof(line), file))
	{
		line_number++;
		opcode = strtok(line, " \n");

		if (opcode == NULL)
		{
			continue;
		}
		if (strcmp(opcode, "push") == 0)
		{
			arg_str = strtok(NULL, " \n");

			push(&stack, arg_str, line_number);
		}
		else
		{
			for (i = 0; instructions[i].opcode != NULL; i++)
			{
				if (strcmp(opcode, instructions[i].opcode) == 0)
				{
					instructions[i].f(&stack, arg_str, line_number);
					break;
				}
			}
			if (instructions[i].opcode == NULL)
			{
				fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
				exit(EXIT_FAILURE);
			}
		}
	}
}

/**
 * push- pushes an int to the stack
 * @stack: pointer to the stack
 * @arg_str: argument containing the int we need to push
 * @line_number: the int to be added to the stack
*/
void push(stack_t **stack, char arg_str, unsigned int line_number)
{
	stack_t *new_node = malloc(sizeof(stack_t));

	if (arg_str == NULL)
	{
		fprintf(stderr, "L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}
	else if (!isdigit(*arg_str) && *arg_str != '-' && *arg_str != '+')
	{
		fprintf(stderr, "L%d: usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}
	arg = atoi(arg_str);

	if (new_node == NULL)
	{
		fprintf(stderr, "Error: malloc failure\n");
		clear_stack(*stack);
		exit(EXIT_FAILURE);
	}

	new_node->n = arg;
	new_node->prev = NULL;
	if (*stack == NULL)
	{
		new_node->next = NULL;
		*stack = new_node;
	}
	else
	{
		new_node->next = *stack;
		(*stack)->prev = new_node;
		*stack = new_node;
	}
	*stack = new_node;
}

/**
 * pall- prints the entire stack from top to bottom
 * @stack: pointer to the stack
 * @arg_str: argument containing the push command
 * @line_number: line number in the file
*/
void pall(stack_t **stack, char arg_str, unsigned int line_number)
{
	stack_t *drifter;

	drifter = *stack;
	while (drifter)
	{
		printf("%d\n", drifter->n);
		drifter = drifter->next;
	}
	(void)arg_str;
	(void)line_number;
}

/**
 * clear_stack - frees memory allocated for the stack nodes
 * @stack: pointer to top of stack
 * Return: void
 */
void clear_stack(stack_t *stack)
{
	stack_t *temp;

	while (stack != NULL)
	{
		temp = stack;
		stack = stack->next;
		free(temp);
	}
}

/**
 * main- entry point for the monty interpreter
 * @argc: number of arguments supplied
 * @argv: the arguments that were supplied to the interpreter
 *
 * Return: 0 on success 1 on failure
*/
int main(int argc, char *argv[])
{
	stack_t *stack = NULL;
	char line[100], *opcode, *arg_str;
	int arg, line_number = 1, i;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		return (EXIT_FAILURE);
	}
	FILE *file = fopen(argv[1], "r");

	if (file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		return (EXIT_FAILURE);
	}

	get_instruction(stack, arg_str, line_number);
	clear_stack(stack);
	fclose(file);
	return (0);
}
