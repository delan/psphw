/**
 * @file homework.c
 * @brief PSP0 homework by Delan Azabani (#17065012)
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
 * This function is only defined when the TEST preprocessor macro is defined.
 */

#ifdef TEST
int		main(void);
#endif

psp_list	psp_list_new(void) {
	psp_list list = malloc(sizeof(struct psp_list));
	list->head = NULL;
	return list;
}

void		psp_list_free(psp_list list) {
	psp_node node = list->head;
	psp_node next;
	while (node) {
		next = node->next;
		psp_node_free(node);
		node = next;
	}
	free(list);
}

psp_node	psp_node_new(double data) {
	psp_node node = malloc(sizeof(struct psp_node));
	node->data = data;
	node->next = NULL;
	return node;
}

void		psp_node_free(psp_node node) {
	free(node);
}

void		psp_insert(psp_list list, psp_node node) {
	if (list->head)
		node->next = list->head;
	list->head = node;
}

void		psp_read(psp_list list, FILE *file) {
	double data;
	while (fscanf(file, "%lf", &data) == 1)
		psp_insert(list, psp_node_new(data));
}

double		psp_mean(psp_list list) {
	size_t count = 0;
	double sum = 0.0;
	psp_node node = list->head;
	while (node) {
		count++;
		sum += node->data;
		node = node->next;
	}
	return sum / count;
}

double		psp_sample_stdev(psp_list list) {
	double mean = psp_mean(list);
	size_t count = 0;
	double sum = 0.0;
	psp_node node = list->head;
	while (node) {
		count++;
		sum += (node->data - mean) * (node->data - mean);
		node = node->next;
	}
	return sqrt(sum / (count - 1));
}

int		main(void) {
	int i = 0;
	char filename[256];
	FILE *file;
	psp_list list;
	do {
		i++;
		sprintf(filename, "test%04d.txt", i);
		file = fopen(filename, "r");
		if (file) {
			list = psp_list_new();
			psp_read(list, file);
			printf("[%04d] mean:                      %f\n",
				i, psp_mean(list));
			printf("[%04d] sample standard deviation: %f\n",
				i, psp_sample_stdev(list));
			psp_list_free(list);
			fclose(file);
		}
	} while (file);
	return 0;
}
