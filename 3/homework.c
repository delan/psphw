/**
 * @file homework.c
 * @brief PSP1 homework by Delan Azabani (#17065012)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct psp_node {
	double size_per_item;
	struct psp_node *next;
} *psp_node;

typedef struct psp_list {
	psp_node head;
} *psp_list;

typedef struct psp_range {
	double vs;
	double s;
	double m;
	double l;
	double vl;
} psp_range;

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

psp_node	psp_node_new(double size_per_item);

/**
 * @brief Frees the given node.
 */

void		psp_node_free(psp_node node);

/**
 * @brief Prepends the given node onto the beginning of the given list.
 */

void		psp_insert(psp_list list, psp_node node);

/**
 * @brief Reads input data from the given file into the given list.
 * The text file must assume the following format:
 * $x_0$
 * $x_i$
 */

void		psp_read(psp_list list, FILE *file);

/**
 * @brief Calculates and returns the mean of all values in the given list.
 */

double		psp_mean(psp_list list);

/**
 * @brief Calculates and returns the sample variance of the given list.
 */

double		psp_variance(psp_list list);

/**
 * @brief Calculates and returns the sample standard deviation.
 */

double		psp_stdev(psp_list list);

/**
 * @brief Sets all members of the given list to their natural logarithm.
 */

void		psp_ln(psp_list list);

/**
 * @brief Calculates and returns the PSP logarithmic ranges.
 * This function will modify the given list because it calls psp_ln().
 */

psp_range	psp_get_range(psp_list list);

/**
 * @brief Runs tests on the homework code.
 */

int		main(void);

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

psp_node	psp_node_new(double size_per_item) {
	psp_node node = malloc(sizeof(struct psp_node));
	node->size_per_item = size_per_item;
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
	double size_per_item;
	while (fscanf(file, "%lf", &size_per_item) == 1)
		psp_insert(list, psp_node_new(size_per_item));
}

double		psp_mean(psp_list list) {
	size_t count = 0;
	double sum = 0.0;
	psp_node node = list->head;
	while (node) {
		count++;
		sum += node->size_per_item;
		node = node->next;
	}
	return sum / count;
}

double		psp_variance(psp_list list) {
	size_t count = 0;
	double sum = 0.0;
	double mean = psp_mean(list);
	psp_node node = list->head;
	while (node) {
		double diff = node->size_per_item - mean;
		count++;
		sum += diff * diff;
		node = node->next;
	}
	return sum / (count - 1);
}

double		psp_stdev(psp_list list) {
	return sqrt(psp_variance(list));
}

void		psp_ln(psp_list list) {
	psp_node node = list->head;
	while (node) {
		node->size_per_item = log(node->size_per_item);
		node = node->next;
	}
}

psp_range	psp_get_range(psp_list list) {
	double mean, stdev;
	psp_range result;
	psp_ln(list);
	mean = psp_mean(list);
	stdev = psp_stdev(list);
	result.vs = exp(mean - 2 * stdev);
	result.s = exp(mean - stdev);
	result.m = exp(mean);
	result.l = exp(mean + stdev);
	result.vl = exp(mean + 2 * stdev);
	return result;
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
			psp_range range;
			list = psp_list_new();
			psp_read(list, file);
			range = psp_get_range(list);
			printf("[%04d] mean of ln(x):             %f\n",
				i, psp_mean(list));
			printf("[%04d] sample variance of ln(x):  %f\n",
				i, psp_variance(list));
			printf("[%04d] very small:                %f\n",
				i, range.vs);
			printf("[%04d] small:                     %f\n",
				i, range.s);
			printf("[%04d] medium:                    %f\n",
				i, range.m);
			printf("[%04d] large:                     %f\n",
				i, range.l);
			printf("[%04d] very large:                %f\n",
				i, range.vl);
			putchar('\n');
			psp_list_free(list);
			fclose(file);
		}
	} while (file);
	return 0;
}
