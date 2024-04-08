#include "monty.h"
#define STACK_SIZE 1000
#define EXIT_FAILURE 1

/**
 * push- pushes an int to the stack
 * @stack: pointer to the stack
 * @line_number: the int to be added to the stack
*/
void push(stack_t **stack, unsigned int line_number)
{
	stack_t *new_node = malloc(sizeof(stack_t));

	if (new_node == NULL)
	{
		fprintf(stderr, "Error: malloc failure\n");
		clear_stack(*stack);
		exit(EXIT_FAILURE);
	}

	new_node->n = line_number;
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
 * @line_number: line number in the file
*/
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *drifter;

	drifter = *stack;
	while (drifter)
	{
		printf("%d\n", drifter->n);
		drifter = drifter->next;
	}
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

int main(int argc, char *argv[])
{
	stack_t *stack = NULL;
	char line[100], *opcode, *arg_str;
	int arg, line_number = 0, i;

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

			push(&stack, arg);
		}
		else
		{
			for (i = 0; instructions[i].opcode != NULL; i++)
			{
				if (strcmp(opcode, instructions[i].opcode) == 0)
				{
					instructions[i].f(&stack, line_number);
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
	clear_stack(stack);
	fclose(file);
	return (0);
}
