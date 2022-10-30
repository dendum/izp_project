#include <stdio.h>
#include <string.h>
#include <ctype.h>
//cat .\contacts.txt | .\test.exe 26

/* t9search.c
 * Projekt 1 - Prace s textem
 * Denys Dumych (xdumyc00)
 */

struct PhoneBook {
    char name[101];
    char number[101];
};
struct PhoneBook book[101];

char cheats[10][10] = { "+", " ", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

int SIZE = 0, some = 0;
int finalResult[101] = {0}, size = 0;

void example(char arr[], int num, char result[], int numSize);
void check(char array[]);
void bubbleSort(int arr[], int size);
void swap(int* x, int* y);

int main(int argc, char *argv[]) {

    int index = 0;
    while(fgets((index%2 == 0)?(book[(index)/2].name):(book[(index-1)/2].number),120, stdin)) {
        if (index % 2 == 0 && book[(index)/2].name[0] != '\n'){SIZE++;}
        index++;
    }
    for (int i = 0; i < SIZE; ++i) {
        int s = 0;
        while (book[i].name[s] != '\n') {
            book[i].name[s] = (char)tolower(book[i].name[s]);
            s++;
        }
    }

    if(argc == 1) {
        printf("Your contacts:\n");
        for (int i = 0; i < SIZE; ++i) {
            printf("%d ", i+1);
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

    char numbers[100];
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
    memset(result, 0, size+1);

    check(numbers);
    example(numbers, 0, result, size);

    int finalSize = 0;
    for (int i = 0; finalResult[i] != 0; ++i) {
        finalSize++;
    }
    bubbleSort(finalResult, finalSize);

    if(finalSize == 0){
        printf("No contacts found.\n");
    } else {
        printf("Your contacts:\n");
        for (int i = 0; i < finalSize; ++i) {

            printf("%d ", i+1);
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

void example(char arr[], int num, char result[], int numSize){

    if (num == size) {
        check(result);
        return;
    }

    for (int i = 0; i < (int)strlen(cheats[arr[num] - '0']); i++) {
        result[num] = cheats[arr[num] - '0'][i];
        example(arr, num+1, result, numSize);
    }

}

void check(char array[]){
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
                q = (int)(e - book[i].name);
                for (int j = q; j < q + size; ++j) {
                    book[i].name[j] = (char)toupper(book[i].name[j]);
                }
            }

            finalResult[some] = i + 1;
            some++;

            jump:;
        }
    }
}

void bubbleSort(int *arr, int finalSize){
    for (int i = 0; i < finalSize-1; ++i) {
        for (int j = 0; j < finalSize - i - 1; ++j) {
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

/*
int main(int argc, char *argv[]) {
    printf("Hello, World!\n");
    int size = 10;
    int count=0;
    char numbers_from_terminal[size];
    while(*argv[1] != '\0') {
        numbers_from_terminal[count] = *argv[1];
        count++;
        argv[1]++;
    }
    numbers_from_terminal[count] = '\0';
    printf("%s\n", numbers_from_terminal);

    int num = 2147483647;
    char name[10];
    itoa(num, name, 10);
    printf("%s", name);


//    for (int i = 0; i < size; ++i) {
//        if (!isdigit(numbers_from_terminal[i])){
//            numbers_from_terminal[i] = '\0';
//        }
//    }

    //printf("%s\n", numbers_from_terminal);

    return 0;
}
*/
//int a;
//a = atoi(argv[1]);
//printf("%d", a);
