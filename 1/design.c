/**
 * @file design.c
 * @brief PSP0 homework by Delan Azabani (#17065012), design phase
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct psp_node {
	double data;
	struct psp_node *next;
} *psp_node;

typedef struct psp_list {
	psp_node head;
} *psp_list;

/**
 * @brief Allocates and returns a new, empty linked list from the heap.
 */

psp_list	psp_list_new(void);

/**
 * @brief Frees all nodes in the given list, then the list itself.
 */

void		psp_list_free(psp_list list);

/**
 * @brief Allocates and returns a new list node with the given data.
 */

psp_node	psp_node_new(double data);

/**
 * @brief Frees the given node.
 */

void		psp_node_free(psp_node node);

/**
 * @brief Prepends the given node onto the beginning of the given list.
 */

void		psp_insert(psp_list list, psp_node node);

/**
 * @brief Scans doubles from the given file into the given list.
 */

void		psp_read(psp_list list, FILE *file);

/**
 * @brief Calculates and returns the mean of all numbers in the given list.
 */

double		psp_mean(psp_list list);

/**
 * @brief Calculates and returns the sample standard deviation of a list.
 */

double		psp_sample_stdev(psp_list list);

/**
 * @brief Runs tests on the homework code.
 */

int		main(void);
