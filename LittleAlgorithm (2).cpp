#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tuple>
#include <vector>

using namespace std;

const unsigned int ARRAY_SIZE = 9 + 1;
const int MAX_INF = INT_MAX;
const int MIN_INF = INT_MIN;

typedef tuple<int, int, int> ROAD;

struct Cord
{
	int x;
	int y;
};

int **Inicialize(void)
{
	int tmp[ARRAY_SIZE][ARRAY_SIZE] = {
		{-1, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		{1, MAX_INF, 5, MIN_INF, MIN_INF, MIN_INF, MIN_INF, MIN_INF, 10,MIN_INF},
		{2, 5, MAX_INF, 45, 45, MIN_INF, MIN_INF, MIN_INF, 25,MIN_INF},
		{3, MIN_INF, 45, MAX_INF, 53, MIN_INF, 50, MIN_INF, MIN_INF,MIN_INF},
		{4, MIN_INF, 45, 53, MAX_INF, 26, MIN_INF, MIN_INF, MIN_INF,55},
		{5, MIN_INF, MIN_INF, MIN_INF, 26, MAX_INF, 61, MIN_INF, MIN_INF,10},
		{6, MIN_INF, MIN_INF, 50, MIN_INF, 61, MAX_INF, 73, MIN_INF,87},
		{7, MIN_INF, MIN_INF, MIN_INF, MIN_INF, MIN_INF, 73, MAX_INF, 42,MIN_INF},
		{8, 10, 25, MIN_INF, MIN_INF, MIN_INF, MIN_INF, 42, MAX_INF,MIN_INF},
		{9, MIN_INF, MIN_INF, MIN_INF, 55, 10, 87, MIN_INF, MIN_INF, MAX_INF}
	};

	int **tmp_matrix = new int *[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		tmp_matrix[i] = new int[ARRAY_SIZE];
		for (int k = 0; k < ARRAY_SIZE; k++) tmp_matrix[i][k] = tmp[i][k];
	}
	return tmp_matrix;
}

int MinFunc(int *arr, int n)
{
	int min = MAX_INF;
	for (int i = 0; i < n; i++)
		if ((min > arr[i]) && (arr[i] >= 0))
			min = arr[i];
	return min;
}

void PrintMatrix(int **matrix, int len)
{
	for (int i = 0; i < len; i++)
	{
		for (int k = 0; k < len; k++)
			if (matrix[i][k] == MAX_INF) printf("+++  ");
			else if (matrix[i][k] == MIN_INF) printf("---  ");
			else if (matrix[i][k] == -1) printf("  _  ");
			else printf("%3i  ", matrix[i][k]);
		printf("\n\n");
	}
}

int **GetMatr(int **mas, int a, int b, int size) {
	int **result = new int *[size - 1];
	int di = 0, dj = 0;

	for (int i = 0; i < size - 1; i++)
	{
		result[i] = new int[size - 1];
		if (i >= a)di = 1;
		else di = 0;
		for (int j = 0; j < size - 1; j++)
		{
			if (j >= b)dj = 1;
			else dj = 0;
			result[i][j] = mas[i + di][j + dj];
		}
	}
	return result;
}

Cord *MapXY(int **matrix, int size)
{
	Cord *res = new Cord;
	int *strings_min = new int[size - 1];
	int *columns_min = new int[size - 1];
	int **matrix_dup = new int *[size];

	for (int i = 0; i < size; i++) {
		matrix_dup[i] = new int[size];
		for (int k = 0; k < size; k++) matrix_dup[i][k] = matrix[i][k];
	}
	PrintMatrix(matrix_dup, size);

	for (int i = 1; i < size; i++)
	{
		int *tmp_arr = new int[size - 1];
		for (int j = 1; j < size; j++) tmp_arr[j - 1] = matrix_dup[i][j];
		strings_min[i - 1] = MinFunc(tmp_arr, size);
		for (int k = 1; k < size; k++)
			if ((matrix_dup[i][k] != MAX_INF) && (matrix_dup[i][k] != MIN_INF) && (matrix_dup[i][k] != -1))
				matrix_dup[i][k] -= strings_min[i - 1];
	}

	for (int i = 1; i < size; i++)
	{
		int *tmp_arr = new int[size - 1];
		for (int j = 1; j < size; j++) tmp_arr[j - 1] = matrix_dup[j][i];
		columns_min[i - 1] = MinFunc(tmp_arr, size);
		for (int k = 1; k < size; k++)
			if ((matrix_dup[k][i] != MAX_INF) && (matrix_dup[k][i] != MIN_INF) && (matrix_dup[k][i] != -1))
				matrix_dup[k][i] -= columns_min[i - 1];
	}

	res->y = -1, res->y = -1;
	int max_ij = 0;
	for (int i = 1; i < size; i++)
	{
		for (int k = 1; k < size; k++)
			if (matrix_dup[i][k] == 0)
			{
				if (max_ij < strings_min[i - 1] + columns_min[k - 1])
				{
					max_ij = strings_min[i - 1] + columns_min[k - 1];
					res->x = i;
					res->y = k;
				}
			}
	}
	printf("Min for srtings and columns:\n");
	for (int i = 0; i < size - 1; i++)
		if (strings_min[i] != MAX_INF) printf("%3i\n", strings_min[i]);
		else printf("INF\n");
	printf("  ");
	for (int i = 0; i < size - 1; i++)
		if (columns_min[i] != MAX_INF) printf("%3i", columns_min[i]);
		else printf("INF");
	printf("\n\n");
	PrintMatrix(matrix_dup, size);

	printf("max is %i with (%i; %i)\n", max_ij, matrix[res->x][0], matrix[0][res->y]);
	printf("-------------------------------------------\n\n");
	return res;
}

vector<ROAD> Little(int **matrix, int size)
{
	vector<ROAD> road;
	if (size <= 2) return road;
	if (size > 2) {
		Cord *cord = MapXY(matrix, size);
		int **p = GetMatr(matrix, cord->x, cord->y, size);

		for (int i = 0, j = 0; j < ARRAY_SIZE && i < ARRAY_SIZE; i++, j++)
			if ((p[0][i] == matrix[0][cord->x]) && (p[j][0] == matrix[cord->y][0]))
				p[i][j] = MAX_INF;

		road = Little(p, size - 1);
		road.push_back(ROAD(matrix[0][cord->y], matrix[cord->x][0], matrix[cord->x][cord->y]));
		return road;
	}
	return road;
}

ROAD FindNextTown(vector<ROAD> tour, ROAD now)
{
	for (auto i : tour)
		if (get<1>(now) == get<0>(i))
			return i;
	return ROAD();
}

void PrintTour(vector<ROAD> tour)
{
	ROAD now;
	int passed[ARRAY_SIZE - 1] = { 0 };

	for (auto i : tour)
	{
		passed[get<0>(i) - 1]++;
		passed[get<1>(i) - 1]++;
	}

	for (auto i : tour)
		if ((passed[get<0>(i) - 1] == 1))
		{
			now = i;
			break;
		}

	int sum = get<2>(now);
	printf("%i(%i)", get<0>(now), get<2>(now));
	for (int i = 0; i < (int)tour.size() - 1; i++)
	{
		now = FindNextTown(tour, now);
		printf("->%i(%i)", get<0>(now), get<2>(now));
		sum += get<2>(now);
	}
	printf("->%i\n\n", get<1>(now));
	printf("Lenght of the trip is %i\n\n", sum);
}

int main(void)
{
	int **matrix = Inicialize();
	auto result = Little(matrix, ARRAY_SIZE);
	PrintTour(result);
	system("pause");
	delete matrix;
	return 0;
}