#include "monty.h"
#define STACK_SIZE 1000
#define EXIT_FAILURE 1

size_t line_ct = 0;

/**
 * get_instruction- determines the instruction to be called
 * @stack: pointer to the stack
 * @arg_str: sting used to determine the instruction to call
 * @line_number: the current line number in the file
*/

int get_instruction(char *op_code, stack_t **stack)
{
	char *op_token;
	unsigned int i = 0;;

	/* array of structs containing instruction and matching function */
	instruction_t ops[] = { 
		{"push", push}, 
		{"pall", pall}, 
		{NULL, NULL} 
		};

	/* tokenize op_code into executable name and number (if exists) */
	op_token = strtok(op_code, " \n");

	while (ops[i].opcode != NULL && op_token != NULL)
	{
		/* checks opcode, runs matching function */
		if (strcmp(op_token, ops[i].opcode) == 0)
		{
			return (ops[i].f(stack));
		}
		i++;
	}
	fprintf(stderr, "L%zu: unknown instruction ", line_ct);
	while (op_token != NULL)
	{
		if (isdigit(op_token) != 0)
			break;
		fprintf(stderr, " %s", op_token);
		op_token = strtok(NULL, " \n");
	}
	fprintf(stderr, "\n");
	return (0);
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
*/
int push(stack_t **stack)
{
	stack_t *new;
	char *tmp;
	int push_num;

	tmp = strtok(NULL, " \n");

	if (tmp != NULL)
		push_num = atoi(tmp);
	/* checks for push without number */
	if (tmp == NULL || ((push_num == 0) && strcmp(tmp, "0") != 0))
	{
		fprintf(stderr, "L%zu: usage: push integer\n", line_ct);
        return (0);
	}

	new = create_node(push_num);

	if (new == NULL)
		return (0);

	if (*stack == NULL)
	{
		*stack = new;
		return (1);
	}

	new->next = *stack;
	(*stack)->prev = new;

	*stack = new;

	return (1);
}

/**
 * pall- prints the entire stack from top to bottom
 * @stack: pointer to the stack
*/
int pall(stack_t **head)
{
	stack_t *tmp = NULL;

	tmp = *head;
	
	while (tmp != NULL)
	{
		printf("%d\n", tmp->n);
		tmp = tmp->next;
	}

	return (1);
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
 * main- entry point for the monty interpreter
 * @argc: number of arguments supplied
 * @argv: the arguments that were supplied to the interpreter
 *
 * Return: 0 on success 1 on failure
*/
int main(int argc, char *argv[])
{
	// char *instruction;
	char *line_buf = NULL;
	FILE *file;
	size_t buf_len = 0;
	ssize_t read;
	stack_t *stack = NULL;
	bool failure = false;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* central loop to get instructions from file */
	while ((read = getline(&line_buf, &buf_len, file)) != -1)
	{
		line_ct++;
		/* if (string_trim(line_buf) == NULL)
			continue;
		instruction = string_trim(line_buf);*/
		if (only_whitespace(line_buf))
			continue;
		if (get_instruction(line_buf, &stack) == 0)
		{
			failure = true;
			break;
		}
	}

	clear_stack(stack);
	stack = NULL;

	fclose(file);
	if (line_buf != NULL)
		free(line_buf);

	if (failure == true)
		exit(EXIT_FAILURE);

	return (0);
}
