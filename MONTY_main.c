#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "monty.h"

/**
 * push - Pushes an element to the stack.
 * @stack: Double pointer to the beginning of the stack.
 * @line_number: The line number of the current operation.
 */
void push(stack_t **stack, unsigned int line_number)
{
    int n;
    char *arg = strtok(NULL, " \n\t");

    if (!arg || !isdigit(*arg))
    {
        fprintf(stderr, "L%d: usage: push integer\n", line_number);
        exit(EXIT_FAILURE);
    }

    n = atoi(arg);
    add_node(stack, n);
}

/**
 * pall - Prints all the values on the stack.
 * @stack: Double pointer to the beginning of the stack.
 * @line_number: The line number of the current operation.
 */
void pall(stack_t **stack, unsigned int line_number)
{
    stack_t *current = *stack;

    (void)line_number;

    while (current)
    {
        printf("%d\n", current->n);
        current = current->next;
    }
}
int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    unsigned int line_number = 0;
    stack_t *stack = NULL;
    instruction_t opcodes[] = {
        {"push", push},
        {"pall", pall},
        /* Add more opcodes here */
        {NULL, NULL}
    };
    FILE *file;

    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        return (EXIT_FAILURE);
    }

    file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        return (EXIT_FAILURE);
    }

    while ((read = getline(&line, &len, file)) != -1)
    {
        line_number++;
        char *opcode = strtok(line, " \n\t");

        if (!opcode || opcode[0] == '#')
            continue;

        int i;
        for (i = 0; opcodes[i].opcode != NULL; i++)
        {
            if (strcmp(opcode, opcodes[i].opcode) == 0)
            {
                opcodes[i].f(&stack, line_number);
                break;
            }
        }

        if (opcodes[i].opcode == NULL)
        {
            fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
            free_stack(stack);
            fclose(file);
            free(line);
            return (EXIT_FAILURE);
        }
    }

    fclose(file);
    free(line);
    free_stack(stack);
    return (EXIT_SUCCESS);
}
