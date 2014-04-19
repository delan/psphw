/**
 * @file homework.c
 * @brief PSP0.1 homework by Delan Azabani (#17065012)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct psp_node {
	double x;
	double y;
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

psp_node	psp_node_new(double x, double y);

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
 * $x_k$
 * $x_0$ $y_0$
 * $x_i$ $y_i$
 * @return the value $x_k$ to be estimated.
 */

double		psp_read(psp_list list, FILE *file);

/**
 * @brief Calculates and returns the mean of all x values in the given list.
 */

double		psp_mean_x(psp_list list);

/**
 * @brief Calculates and returns the mean of all y values in the given list.
 */

double		psp_mean_y(psp_list list);

/**
 * @brief Calculates and returns $r_{x,y}$, the correlation coefficient.
 */

double		psp_correlation(psp_list list);

/**
 * @brief Calculates and returns the intercept of the linear regression line.
 */

double		psp_reg_intercept(psp_list list);

/**
 * @brief Calculates and returns the gradient of the linear regression line.
 */

double		psp_reg_gradient(psp_list list);

/**
 * @brief Calculates and returns an estimate for y based on a regression line.
 */

double		psp_reg_estimate(psp_list list, double x);

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

psp_node	psp_node_new(double x, double y) {
	psp_node node = malloc(sizeof(struct psp_node));
	node->x = x;
	node->y = y;
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

double		psp_read(psp_list list, FILE *file) {
	double output, x, y;
	fscanf(file, "%lf", &output);
	while (fscanf(file, "%lf %lf", &x, &y) == 2)
		psp_insert(list, psp_node_new(x, y));
	return output;
}

double		psp_mean_x(psp_list list) {
	size_t count = 0;
	double sum = 0.0;
	psp_node node = list->head;
	while (node) {
		count++;
		sum += node->x;
		node = node->next;
	}
	return sum / count;
}

double		psp_mean_y(psp_list list) {
	size_t count = 0;
	double sum = 0.0;
	psp_node node = list->head;
	while (node) {
		count++;
		sum += node->y;
		node = node->next;
	}
	return sum / count;
}

double		psp_correlation(psp_list list) {
	size_t n = 0;
	double sum_x = 0.0;
	double sum_y = 0.0;
	double sum_x_y = 0.0;
	double sum_x_squared = 0.0;
	double sum_y_squared = 0.0;
	double left;
	double right;
	double num;
	double den;
	psp_node node = list->head;
	while (node) {
		double x = node->x;
		double y = node->y;
		n++;
		sum_x += x;
		sum_y += y;
		sum_x_y += x * y;
		sum_x_squared += x * x;
		sum_y_squared += y * y;
		node = node->next;
	}
	left = n * sum_x_squared - sum_x * sum_x;
	right = n * sum_y_squared - sum_y * sum_y;
	num = n * sum_x_y - sum_x * sum_y;
	den = sqrt(left * right);
	return num / den;
}

double		psp_reg_intercept(psp_list list) {
	double x_avg = psp_mean_x(list);
	double y_avg = psp_mean_y(list);
	return y_avg - x_avg * psp_reg_gradient(list);
}

double		psp_reg_gradient(psp_list list) {
	size_t n = 0;
	double sum_x_y = 0.0;
	double sum_x_squared = 0.0;
	double x_avg = psp_mean_x(list);
	double y_avg = psp_mean_y(list);
	double num;
	double den;
	psp_node node = list->head;
	while (node) {
		double x = node->x;
		double y = node->y;
		n++;
		sum_x_y += x * y;
		sum_x_squared += x * x;
		node = node->next;
	}
	num = sum_x_y - n * x_avg * y_avg;
	den = sum_x_squared - n * x_avg * x_avg;
	return num / den;
}

double		psp_reg_estimate(psp_list list, double x) {
	double intercept = psp_reg_intercept(list);
	double gradient = psp_reg_gradient(list);
	return intercept + gradient * x;
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
			double estimate;
			list = psp_list_new();
			estimate = psp_read(list, file);
			printf("[%04d] to estimate x:             %f\n",
				i, estimate);
			printf("[%04d] mean x:                    %f\n",
				i, psp_mean_x(list));
			printf("[%04d] mean y:                    %f\n",
				i, psp_mean_y(list));
			printf("[%04d] intercept β₀:              %f\n",
				i, psp_reg_intercept(list));
			printf("[%04d] gradient β₁:               %f\n",
				i, psp_reg_gradient(list));
			printf("[%04d] correlation r:             %f\n",
				i, psp_correlation(list));
			printf("[%04d] correlation r²:            %f\n",
				i, psp_correlation(list) *
				psp_correlation(list));
			printf("[%04d] estimate y:                %f\n",
				i, psp_reg_estimate(list, estimate));
			putchar('\n');
			psp_list_free(list);
			fclose(file);
		}
	} while (file);
	return 0;
}
