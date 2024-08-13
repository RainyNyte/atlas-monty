#include "monty.h"
#define STACK_SIZE 1000
#define EXIT_FAILURE 1

size_t line_ct = 0;
global_m globm;

/**
 * get_instruction- determines the instruction to be called
 * @stack: pointer to the stack
 * @arg_str: sting used to determine the instruction to call
 * @line_number: the current line number in the file
*/

void get_instruction(char *opcode)
{
	unsigned int i = 0;

	/* array of structs containing instruction and matching function */
	instruction_t ops[] = { 
		{"push", push}, 
		{"pall", pall}, 
		{NULL, NULL} 
		};

	for (i = 0; ops[i].opcode && opcode; i++)
		if (!strcmp(opcode, ops[i].opcode))
		{
			ops[i].f(&(globm.head), globm.line_number);
			return;
		}

	dprintf(2, "L%d: unknown instruction %s\n",
		globm.line_number, opcode);
	exit(EXIT_FAILURE);
}

/**
 * create_node - initializes a new stack_t node
 * @n: integer to initialize new node with
 *
 * Return: pointer to newly created node, NULL if failure
 */
stack_t *create_node(int n)
{
	stack_t *new;

	/* MALLOC ALERT */
	new = malloc(sizeof(stack_t));

	if (new == NULL)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return (NULL);
	}

	new->n = n;
	new->next = NULL;
	new->prev = NULL;

	return (new);
}
	
/**
 * push- pushes an int to the stack
 * @stack: pointer to the stack
 * @line_number: line number in the file
*/
void push(stack_t **stack, unsigned int line_number)
{
	int n;
	stack_t *new;

	if (!isdigit(globm.n[0]) && !(globm.n[0] == '-' && isdigit(globm.n[1])))
	{
		dprintf(2, "L%d: usage: push integer\n", line_number);
		cleanup();
		exit(EXIT_FAILURE);
	}

	n = atoi(globm.n);
	new = create_node(n);

	if (*stack == NULL)
	{
		*stack = new;
	}
	else
	{
		new->next = *stack;
		(*stack)->prev = new;

		*stack = new;
	}
}

/**
 * cleanup - performs exit operations (free/close)
 * Return: No Return
 */
void cleanup(void)
{
	free(globm.gbuff), fclose(globm.fp);
	clear_stack(globm.head);
}

/**
 * pall- prints the entire stack from top to bottom
 * @stack: pointer to the stack
 * line_number: line number of the file
*/
void pall(stack_t **stack, unsigned int line_number)
{
	stack_t *drifter = *stack;

	(void) line_number;
	while (stack && drifter)
	{
		printf("%d\n", drifter->n);
		drifter = drifter->next;
	}
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
 * only_whitespace - find if string is made entirely of whitespace
 * @str: the string to check
 *
 * Return: false if non-whitespace found, true if string is entirely whitespace
 */
bool only_whitespace(char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\n' && *str != '\t')
		{
			return (false);
		}
		str++;
	}
	return (true);
}

/**
 * set_global - defines global variables
 *
 * Return: No return
 */
void set_global(void)
{
	globm.gbuff = NULL;
	globm.n = NULL;
	globm.head = NULL;
	globm.line_number = 0;
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
	
	char *buff = NULL, *dlim = " \n\t", *optok = NULL;
	size_t buff_size = 0;
	ssize_t line_size;
	FILE *fp;

	if (argc != 2)
		dprintf(2, "USAGE: monty file\n"), exit(EXIT_FAILURE);

	fp  = fopen(argv[1], "r");
	if (!fp)
	{
		dprintf(2, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	globm.fp = fp;
	set_global();
	line_size = getline(&buff, &buff_size, globm.fp);
	globm.gbuff = buff;
	while (line_size >= 0)
	{
		globm.line_number += 1;
		optok = strtok(buff, dlim);

		if (optok && optok[0] != '#')
		{
			globm.n = strtok(NULL, dlim);
			get_instruction(optok);
		}
		line_size = getline(&buff, &buff_size, globm.fp);
		optok = NULL, globm.n = NULL;
	}
	cleanup();
	return (0);
}
