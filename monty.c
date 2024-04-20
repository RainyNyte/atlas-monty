#include "monty.h"

#define STACK_SIZE 100

void push(int value, int line_number)
{
	if (stack.top >= STACK_SIZE)
	{
		fprintf(stderr, "L%d: Error: Stack overflow\n", line_number);
		exit(EXIT_FAILURE);
	}
	stack.array[++stack.top] = value;
}

void pall()
{
	if (stack.top == -1)
	{
		return;
	}
	for (int i = stack.top; i >= 0; i--)
	{
		printf("%d\n", stack.array[i]);
	}
}

void process_instruction(const char *instruction, int line_number)
{
	if (strncmp(instruction, "push", 4) == 0)
	{
		char *token = strtok(instruction, " ");
		token = strtok(NULL, " "); // Get the integer value
	if (token == NULL)
	{
		fprintf(stderr, "L%d: Error: Usage: push integer\n", line_number);
		exit(EXIT_FAILURE);
	}
	int value = atoi(token);
	push(value, line_number);
	}
	else if (strcmp(instruction, "pall") == 0)
	{
		pall();
	}
	else
	{
		fprintf(stderr, "L%d: Error: Unknown instruction: %s\n", line_number, instruction);
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
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

	char line[100];
	int line_number = 0;

	stack.top = -1; // Initialize stack

	while (fgets(line, sizeof(line), file))
	{
		line_number++;
		line[strcspn(line, "\n")] = '\0'; // Remove newline character
		process_instruction(line, line_number);
	}

	fclose(file);

	return (EXIT_SUCCESS);
}
