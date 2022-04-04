/*
 * IMPORTS
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


/*
 * STRUCT INPUT
 */
typedef struct arguments
{
    char* array;
    int arrayIndex;
    int nWords;
    char** words;

}arguments;


/*
 * FUNCTIONS
 */
void* findHorizontal(void *args){
    // declare variables
    struct arguments *inputs = (struct arguments *)args;
    int position;
    char *firstLetter;

    for (int i = 0; i < inputs->nWords; i++)
    {
        // find if the word to be found is a substring of the column
        firstLetter = strstr(inputs->array, inputs->words[i]);

        // word found in line: print its position
        if (firstLetter != NULL)
        {
            // find position of the word found
            position = firstLetter - inputs->array;

            // inform to user
            printf("palavra '%s' encontrada na posição '%d' da linha %d\n",
                   inputs->words[i], position, inputs->arrayIndex);
        }
    }

    // return null pointer
    return NULL;
}

void* findVertical(void *args){
    // declare variables
    struct arguments *inputs = (struct arguments *)args;
    int position;
    char *firstLetter;

    for (int i = 0; i < inputs->nWords; i++)
    {
        // find if the word to be found is a substring of the column
        firstLetter = strstr(inputs->array, inputs->words[i]);

        // check result
        if (firstLetter != NULL){

            // find position of the word found
            position = firstLetter - inputs->array;

            // inform to user
            printf("palavra '%s' encontrada na posição '%d' da coluna %d\n",
                    inputs->words[i], position, inputs->arrayIndex);
        }
    }

    // return null pointer
    return NULL;
}

char* invertColumn(char** matrix, int n, int columnIndex){
    // declare variables
    char* newLine = malloc(n*sizeof(char));

    for (int i = 0; i < n; i++){
        // copy corresponding character to its new position
        newLine[i] = matrix[i][columnIndex];
    }

    // return inverted column
    return newLine;

}


/*
 * CODE
 */
void main(){

    // declare variables
    int n;
    char **matrix;
    int i;
    int nWords;
    char **words;
    arguments *args;
    pthread_t *horizontal_thr;
    pthread_t *vertical_thr;

    // get n from user
    printf("Insert number of rows: ");
    scanf("%d", &n);

    // comunicate to user
    printf("Insert matrix: \n");

    // alloc matrix memory
    matrix = malloc(n * sizeof(char*));

    for (i=0; i<n; i++)
    {
        // alloc matrix row
        matrix[i] = malloc(n * sizeof(char));

        // read word from user
        scanf("%s", matrix[i]);
    }

    // get number of strings
    printf("Insert number of words to find: ");
    scanf("%d", &nWords);

    // alloc words memory
    words = malloc(nWords * sizeof(char*));

    for (i=0; i<nWords; i++)
    {
        // alloc words row
        words[i] = malloc(n * sizeof(char));

        // read word from user
        scanf("%s", words[i]);
    }

    // declare threads horizontal threads
    horizontal_thr = malloc(sizeof(pthread_t) * n);

    // declare threads vertical threads
    vertical_thr = malloc(sizeof(pthread_t) * n);

    // declare struct args
    args = malloc(sizeof(*args));

    // assign de number of to words to be found to nWords
    args->nWords = nWords;

    // assign the words to be found to words
    args->words = words;

    for ( i = 0; i < n; i++)
    {
        // assign the index of the line/column to watch
        args->arrayIndex = i;

        // assign the corresponding row
        args->array = matrix[i];

        // create "horizontal" thread
        pthread_create(&horizontal_thr[i], NULL, findHorizontal, args);

        // join thread
        pthread_join(horizontal_thr[i], NULL);

        // assing the corresponding column making it a row
        args->array = invertColumn(matrix,n,i);

        // create "vertical" thread
        pthread_create(&vertical_thr[i], NULL, findVertical, args);

        // join thread
        pthread_join(vertical_thr[i], NULL);
    }
}