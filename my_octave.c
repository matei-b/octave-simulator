//Copyright Matei-Cristian Balan 312CAb 2023-2024
#include <stdio.h>
#include <stdlib.h>

struct matrices {
	int **mat;
};

struct dimensions_matrix {
	int n, m;
};

//functia de alocare a memoriei vectorului de pointeri catre matricele retinute
struct matrices **mem_alloc_vec_mat(int index, struct matrices **v_m)
{
	//realocarea vectorului cu o pozitie adaugata pentru matricea noua
	struct matrices **v_m_new;
	v_m_new = realloc(v_m, (index + 1) * sizeof(struct matrices *));
	if (!v_m_new) {
		fprintf(stderr, "Eroare la alocare dinamica.\n");
		exit(-1);
	}
	v_m = v_m_new;
	return v_m;
}

//functia de alocare a memoriei vectorului
//cu dimensiunile fiecarei matrice retinute
struct dimensions_matrix *mem_alloc_vec_dim_mat(int *index_ptr,
												struct dimensions_matrix *v_d_m)
{
	//realocarea vectorului de dimensiuni cu o pozitie noua
	int index = *index_ptr;
	struct dimensions_matrix *v_d_m_new;
	//incrementarea indexului cu 1 pentru a marca numarul curent de matrice
	//(-1 fata de cel real pentru conveninta utilizarii lui index ca indice)
	index++;
	v_d_m_new = realloc(v_d_m, (index + 1) * sizeof(struct dimensions_matrix));
	if (!v_d_m_new) {
		fprintf(stderr, "Error reallocating memory.\n");
		exit(-1);
	}
	v_d_m = v_d_m_new;
	*index_ptr = index;
	return v_d_m;
}

//functia de alocare a memoriei unei matrice (ca struct)
struct matrices *mem_alloc_mat(int n, int m)
{
	struct matrices *matrix;
	matrix = malloc(sizeof(struct matrices));
	if (!matrix) {
		fprintf(stderr, "Eroare la alocare dinamica.\n");
		exit(-1);
	}
	matrix->mat = malloc(n * sizeof(int *));
	if (!matrix->mat) {
		fprintf(stderr, "Eroare la alocare dinamica.\n");
		exit(-1);
	}
	for (int i = 0; i < n; i++) {
		matrix->mat[i] = malloc(m * sizeof(int));
		if (!matrix->mat[i]) {
			fprintf(stderr, "Eroare la alocare dinamica.\n");
			exit(-1);
		}
	}
	return matrix;
}

//functia de citire a elementelor unei matrice
void read_array(int n, int m, struct matrices *matrix)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			scanf("%d", &matrix->mat[i][j]);
}

//functia de retinere a dimensiunilor unei matrice
void dim_mat(int *n_ptr, int *m_ptr, int index, struct dimensions_matrix *v_d_m)
{
	int n, m;
	scanf("%d%d", &n, &m);
	v_d_m[index].n = n;
	v_d_m[index].m = m;
	*n_ptr = n;
	*m_ptr = m;
}

//functia prin care calculez suma elementelor unei matrice
//pentru functia de sortare
int sum_elements_matrix(int n, int m, int **mat)
{
	int sum = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			sum += mat[i][j];
	sum = sum % 10007;
	if (sum < 0)
		sum += 10007;
	return sum;
}

//functia prin care adaug in vectorul de matrice o matrice
void save_to_memory(int *index_ptr,
					struct matrices ***v_m, struct dimensions_matrix **v_d_m)
{
	int index = *index_ptr, n, m;
	//aloc memorie pentru dimensiuni si incrementez index(indicele)
	(*v_d_m) = mem_alloc_vec_dim_mat(&index, (*v_d_m));
	//retin dimensiunile
	dim_mat(&n, &m, index, (*v_d_m));
	//aloc memorie pentru noua matrice
	(*v_m) = mem_alloc_vec_mat(index, (*v_m));
	//adaug in vectorul de matrice
	//adresa strcturii ce retine matricea proaspat citita
	(*v_m)[index] = mem_alloc_mat(n, m);
	//citesc elemente matricei
	read_array(n, m, (*v_m)[index]);
	*index_ptr = index;
}

//functia de afisare a dimensiunilor unei matrice
void dimensions_of_given_matrix(int *index_ptr, struct dimensions_matrix *v_d_m)
{
	int index;
	scanf("%d", &index);
	if (index > *index_ptr || index < 0)
		printf("No matrix with the given index\n");
	else
		printf("%d %d\n", v_d_m[index].n, v_d_m[index].m);
}

//functia de afisare a elementelor unei matrice
void print_matrix(int *index_ptr,
				  struct matrices **v_m, struct dimensions_matrix *v_d_m)
{
	int index;
	scanf("%d", &index);
	if (index > *index_ptr || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	for (int i = 0; i < v_d_m[index].n; i++) {
		for (int j = 0; j < v_d_m[index].m; j++)
			printf("%d ", v_m[index]->mat[i][j]);
			printf("\n");
		}
}

//functia de redimensionare a unei matrice
void resize_matrix(int *index_ptr,
				   struct matrices **v_mat, struct dimensions_matrix *v_d_m)
{
	int index, new_n, new_m;
	//vectorii de linii si coloane pe baza carora reconstruiesc matricea
	int *v_n, *v_m;
	struct matrices *matrix;
	scanf("%d", &index);
	scanf("%d", &new_n);
	v_n = malloc(new_n * sizeof(int));
	if (!v_n) {
		fprintf(stderr, "Eroare la alocare dinamica.\n");
		exit(-1);
	}
	for (int i = 0; i < new_n; i++)
		scanf("%d", &v_n[i]);
	scanf("%d", &new_m);
	v_m = malloc(new_m * sizeof(int));
	if (!v_m) {
		fprintf(stderr, "Eroare la alocare dinamica.\n");
		exit(-1);
	}
	for (int j = 0; j < new_m; j++)
		scanf("%d", &v_m[j]);
	if (index > *index_ptr || index < 0) {
		printf("No matrix with the given index\n");
		free(v_n);
		free(v_m);
		return;
	}
	matrix = mem_alloc_mat(new_n, new_m);
	for (int i = 0; i < new_n; i++)
		for (int j = 0; j < new_m; j++)
			matrix->mat[i][j] = v_mat[index]->mat[v_n[i]][v_m[j]];
	for (int i = 0; i < v_d_m[index].n; i++)
		free(v_mat[index]->mat[i]);
	free(v_mat[index]->mat);
	free(v_mat[index]);
	v_mat[index] = matrix;
	v_d_m[index].n = new_n;
	v_d_m[index].m = new_m;
	free(v_n);
	free(v_m);
}

//functia de inmultire a doua matrice
void multiply_matrices(int *index_ptr,
					   struct matrices ***v_m, struct dimensions_matrix **v_d_m)
{
	int index1, index2;
	struct matrices *matrix;
	scanf("%d%d", &index1, &index2);
	if (index1 > *index_ptr || index2 > *index_ptr ||
		index1 < 0 || index2 < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	if ((*v_d_m)[index1].m != (*v_d_m)[index2].n) {
		printf("Cannot perform matrix multiplication\n");
		return;
	}
	(*v_d_m) = mem_alloc_vec_dim_mat(index_ptr, (*v_d_m));
	(*v_m) = mem_alloc_vec_mat(*index_ptr, (*v_m));
	int l = (*v_d_m)[index1].n, c = (*v_d_m)[index2].m;
	(*v_d_m)[*index_ptr].n = l;
	(*v_d_m)[*index_ptr].m = c;
	matrix = mem_alloc_mat(l, c);
	for (int i = 0; i < l; i++)
		for (int j = 0; j < c; j++) {
			int sum = 0, prod;
			for (int k = 0; k < (*v_d_m)[index1].m; k++) {
				int t1, t2;
				t1 = (*v_m)[index1]->mat[i][k];
				t2 = (*v_m)[index2]->mat[k][j];
				prod = (t1 * t2) % 10007;
				if (prod < 0)
					prod += 10007;
				sum = (sum + prod) % 10007;
				}
				matrix->mat[i][j] = sum;
			}
	(*v_m)[*index_ptr] = matrix;
}

//functia de sortare a matricelor (folosind insertion sort)
void sort_matrices(int index,
				   struct matrices ***v_m, struct dimensions_matrix **v_d_m)
{
	int *sums;
	int n, m;
	sums = malloc((index + 1) * sizeof(int));
	for (int i = 0; i <= index; i++) {
		n = (*v_d_m)[i].n;
		m = (*v_d_m)[i].m;
		sums[i] = sum_elements_matrix(n, m, (*v_m)[i]->mat);
	}
	for (int i = 0; i <= index; i++) {
		int minij = i;
		for (int j = i + 1; j <= index; j++)
			if (sums[j] < sums[minij])
				minij = j;
		if (minij != i) {
			struct matrices *aux1 = (*v_m)[i];
			(*v_m)[i] = (*v_m)[minij];
			(*v_m)[minij] = aux1;
			struct dimensions_matrix aux2 = (*v_d_m)[i];
			(*v_d_m)[i] = (*v_d_m)[minij];
			(*v_d_m)[minij] = aux2;
			int aux3 = sums[i];
			sums[i] = sums[minij];
			sums[minij] = aux3;
		}
	}
	free(sums);
}

//functia de transpunere a matricelor
void transpose_matrix(int *index_ptr,
					  struct matrices **v_m, struct dimensions_matrix *v_d_m)
{
	int index;
	struct matrices *matrix;
	scanf("%d", &index);
	if (index > *index_ptr || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	matrix = mem_alloc_mat(v_d_m[index].m, v_d_m[index].n);
	for (int i = 0; i < v_d_m[index].m; i++)
		for (int j = 0; j < v_d_m[index].n; j++)
			matrix->mat[i][j] = v_m[index]->mat[j][i];
	for (int i = 0; i < v_d_m[index].n; i++)
		free(v_m[index]->mat[i]);
	free(v_m[index]->mat);
	free(v_m[index]);
	v_m[index] = matrix;
	int aux = v_d_m[index].n;
	v_d_m[index].n = v_d_m[index].m;
	v_d_m[index].m = aux;
}

//functia de stergere a unei matrice
void delete_matrix(int *index_ptr,
				   struct matrices ***v_m, struct dimensions_matrix **v_d_m)
{
	int index;
	scanf("%d", &index);
	if (index > *index_ptr || index < 0) {
		printf("No matrix with the given index\n");
		return;
	}
	for (int i = 0; i < (*v_d_m)[index].n; i++)
		free((*v_m)[index]->mat[i]);
	free((*v_m)[index]->mat);
	free((*v_m)[index]);
	for (int i = index; i <= (*index_ptr) - 1; i++) {
		(*v_m)[i] = (*v_m)[i + 1];
		(*v_d_m)[i] = (*v_d_m)[i + 1];
	}
	(*index_ptr)--;
}

//functia oprire a programului
void stop_prog(int *index_ptr,
			   struct matrices **v_m, struct dimensions_matrix *v_d_m)
{
	for (int j = 0; j <= *index_ptr; j++) {
		for (int i = 0; i < v_d_m[j].n; i++)
			free(v_m[j]->mat[i]);
		free(v_m[j]->mat);
		free(v_m[j]);
	}
	free(v_d_m);
	free(v_m);
	exit(0);
}

//functia de stabilire a operatiei ce trebuie realizata
void task_assignment(int *index_ptr,
					 struct matrices **v_m, struct dimensions_matrix *v_d_m)
{
	char task;
	while (1) {
		scanf("%c", &task);
		int ok = 0;
		if (task == 'L') {
			save_to_memory(index_ptr, &v_m, &v_d_m);
			ok = 1;
		}
		if (task == 'D') {
			dimensions_of_given_matrix(index_ptr, v_d_m);
			ok = 1;
		}
		if (task == 'P') {
			print_matrix(index_ptr, v_m, v_d_m);
			ok = 1;
		}
		if (task == 'C') {
			resize_matrix(index_ptr, v_m, v_d_m);
			ok = 1;
		}
		if (task == 'M') {
			multiply_matrices(index_ptr, &v_m, &v_d_m);
			ok = 1;
		}
		if (task == 'O') {
			sort_matrices(*index_ptr, &v_m, &v_d_m);
			ok = 1;
		}
		if (task == 'T') {
			transpose_matrix(index_ptr, v_m, v_d_m);
			ok = 1;
		}
		if (task == 'F') {
			delete_matrix(index_ptr, &v_m, &v_d_m);
			ok = 1;
		}
		if (task == 'Q') {
			stop_prog(index_ptr, v_m, v_d_m);
			ok = 1;
		}
		if (ok == 0)
			printf("Unrecognized command\n");
		scanf("\n");
	}
}

int main(void)
{
	int index = -1, n, m;
	struct matrices **v_m = NULL;
	struct dimensions_matrix *v_d_m = NULL;
	v_m = malloc(sizeof(struct matrices *));
	v_d_m = malloc(sizeof(struct dimensions_matrix));
	task_assignment(&index, v_m, v_d_m);
	return 0;
}
