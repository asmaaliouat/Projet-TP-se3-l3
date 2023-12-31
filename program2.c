#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n1 3 // Taille de la première dimension de la matrice B
#define m1 3 // Taille de la deuxième dimension de la matrice B
#define n2 3 // Taille de la première dimension de la matrice C
#define m2 3 // Taille de la deuxième dimension de la matrice C
#define N (n1 * m2) // Taille du tampon

int B[n1][m1];
int C[n2][m2];
int A[n1][m2];
int T[N];
/*vous qui enter les valeure de matrice B et C*/
void *producer(void *arg) {
    int row = *((int *)arg);

    for (int j = 0; j < m2; j++) {
        int result = 0;
        for (int k = 0; k < m1; k++) {
            result += B[row][k] * C[k][j];
        }
        T[row * m2 + j] = result;
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int index = *((int *)arg);
    int row = index / m2;
    int col = index % m2;

    A[row][col] = T[index];

    pthread_exit(NULL);
}

void print_matrix(char *matrix_name, int rows, int cols, int matrix[rows][cols]) {
    printf("%s :\n", matrix_name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Entrez les valeurs pour la matrice :\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%d", &matrix[i][j]);
        }
    }
}

int main() {
    printf("Lecture de la matrice B :\n");
    read_matrix(n1, m1, B);

    printf("\nLecture de la matrice C :\n");
    read_matrix(n2, m2, C);

    printf("\n*******Matrice B*******:\n");
    print_matrix("Matrice B", n1, m1, B);

    printf("\n******Matrice C****** :\n");
    print_matrix("Matrice C", n2, m2, C);

     pthread_t producers[n1], consumers[N];

    for (int i = 0; i < n1; i++) {
        pthread_create(&producers[i], NULL, producer, &i);
    }

    for (int i = 0; i < N; i++) {
        pthread_create(&consumers[i], NULL, consumer, &i);
    }

    for (int i = 0; i < n1; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Affichage du contenu du tampon T après le remplissage
    printf("\nContenu du tampon T après le remplissage :\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", T[i]);
    }
    printf("\n");

    // Affichage de la matrice résultante A
    printf("\nMatrice résultante A :\n");
    print_matrix("Matrice résultante A", n1, m2, A);

    getchar();
    getchar();

    return 0;
}
