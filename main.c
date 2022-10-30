#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * t9search.c
 * Projekt 1 - Prace s textem
 * Denys Dumych (xdumyc00)
 */

struct PhoneBook {
    char name[101];
    char number[101];
};
struct PhoneBook book[100];

char cheats[10][5] = { "+", " ", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

int SIZE = 0;
int finalIndex = 0;

void example(char arr[], char result[], int finalResult[], int num);
void check(char array[], int finalResult[]);
void bubbleSort(int arr[], int finSize);
void swap(int* x, int* y);

int main(int argc, char *argv[]) {
    int index = 0;
    while(fgets((index%2 == 0)?(book[(index)/2].name):(book[(index-1)/2].number),120, stdin)) {
        if (index % 2 == 0 && book[(index)/2].name[0] != '\n'){SIZE++;}
        index++;
    }

    if(argc == 1) {
        printf("Your contacts:\n");
        for (int i = 0; i < SIZE; ++i) {
            printf("%d) ", i+1);
            int q = 0;
            while (book[i].name[q] != '\n'){
                printf("%c", book[i].name[q]);
                q++;
            }
            printf(", %s", book[i].number);
        }
        return 0;
    } else if (argc > 2){
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    for (int i = 0; i < SIZE; ++i) {
        int s = 0;
        while (book[i].name[s] != '\n') {
            book[i].name[s] = (char)tolower(book[i].name[s]);
            s++;
        }
    }

    int size = 0;
    char numbers[(int)strlen(argv[1])+1];
    while(*argv[1] != '\0') {
        if(!isdigit(*argv[1])){
            fprintf(stderr, "Invalid input\n");
            return 1;
        }
        numbers[size] = *argv[1];
        size++;
        argv[1]++;
    }
    numbers[size] = '\0';

    char result[size+1];
    memset(result, 0, sizeof result);
    int finalResult[SIZE];
    memset(finalResult, 0, sizeof finalResult);

    check(numbers, finalResult);
    example(numbers, result, finalResult, 0);
    bubbleSort(finalResult, finalIndex);

    if(finalIndex == 0){
        printf("No contacts found.\n");
    } else {
        printf("Your contacts:\n");
        for (int i = 0; i < finalIndex; ++i) {

            printf("%d) ", i+1);
            int q = 0;
            while (book[finalResult[i]-1].name[q] != '\n'){
                printf("%c", book[finalResult[i]-1].name[q]);
                q++;
            }
            printf(", %s", book[finalResult[i]-1].number);

        }
    }
    return 0;
}

void example(char arr[], char result[], int finalResult[], int num){
    if (num == strlen(arr)) {
        check(result, finalResult);
        return;
    }

    for (int i = 0; i < (int)strlen(cheats[arr[num] - '0']); i++) {
        result[num] = cheats[arr[num] - '0'][i];
        example(arr, result, finalResult, num+1);
    }

}

void check(char array[], int finalResult[]){
    for (int i = 0; i < SIZE; ++i) {
        if (strstr(book[i].name, array) != NULL || strstr(book[i].number, array) != NULL) {
            for (int j = 0; finalResult[j] != 0; ++j) {
                if (finalResult[j] == i + 1) {
                    goto jump;
                }
            }

            if (isalpha(array[0])) {
                char *e;
                int q;
                e = strstr(book[i].name, array);
                q = (int) (e - book[i].name);
                for (int j = q; j < q + strlen(array); ++j) {
                    book[i].name[j] = (char)toupper(book[i].name[j]);
                }
            }

            finalResult[finalIndex] = i + 1;
            finalIndex++;

            jump:;
        }
    }
}

void bubbleSort(int arr[], int finSize){
    for (int i = 0; i < finSize-1; ++i) {
        for (int j = 0; j < finSize - i - 1; ++j) {
            if(arr[j] > arr[j+1]){
                swap(&arr[j], &arr[j+1]);
            }
        }
    }
}

void swap(int* x, int* y){
    int value = *x;
    *x = *y;
    *y = value;
}
