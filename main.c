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

int numOfContacts = 0; // the number of contacts added from the file
int finalIndex = 0; // index to control the number of found contacts, used with foundContacts[](array of final contacts for sorting and printing)

void example(char arr[], char storage[], int foundContacts[], int num);
void check(char storage[], int foundContacts[]);
void bubbleSort(int foundContacts[], int foundSize);
void swap(int* x, int* y);

int main(int argc, char *argv[]) {
    // the program read contacts from the input file using the function fgets()
    // depending on the index, the correct string of the structure (name or number) is filled
    int index = 0;
    while(fgets((index%2 == 0)?(book[(index)/2].name):(book[(index-1)/2].number),120, stdin)) {
        if (index % 2 == 0 && book[(index)/2].name[0] != '\n'){numOfContacts++;}
        index++;
    }

    // the program check the number of input arguments
    // it will throw an error if the amount is wrong
    if(argc == 1) {
        printf("Your contacts:\n");
        for (int i = 0; i < numOfContacts; ++i) {
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

    // the program checks the entered numbers
    // and writes numbers to an array numbers[]
    int size = 0;
    char numbers[(int)strlen(argv[1])+1]; // an array of entered numbers to search
    while(*argv[1] != '\0') {
        if(!isdigit(*argv[1])){
            fprintf(stderr, "Invalid input\n"); // it will throw an error if the entered characters are invalid
            return 1;
        }
        numbers[size] = *argv[1];
        size++;
        argv[1]++;
    }
    numbers[size] = '\0';

    // if the input data is valid, the program changes the letter register of the contact names
    for (int i = 0; i < numOfContacts; ++i) {
        int s = 0;
        while (book[i].name[s] != '\n') {
            book[i].name[s] = (char)tolower(book[i].name[s]);
            s++;
        }
    }

    // storage[] is an array in which possible string variants one by one will be recorded, stored, checked, and cleared
    // for example: possible string variants from the entered numbers 23 is: ad, ae, af, bd, be, bf, cd, ce, cf
    char storage[size+1];
    memset(storage, 0, sizeof storage);

    // foundContacts[] is an array for storing, sorting and printing indexes of found contacts
    int foundContacts[numOfContacts];
    memset(foundContacts, 0, sizeof foundContacts);

    // program calls the function check() for the first time, it sends the entered numbers to check if the phone numbers contain those numbers
    check(numbers, foundContacts);
    // program calls the function example() to create all possible string variants
    example(numbers, storage, foundContacts, 0);
    // program calls the function bubbleSort() to sort the found contacts
    bubbleSort(foundContacts, finalIndex);

    // after sorting, the program prints all found contacts
    if(finalIndex == 0){
        printf("No contacts found.\n");
    } else {
        printf("Your contacts:\n");
        for (int i = 0; i < finalIndex; ++i) {
            printf("%d) ", i+1);
            int q = 0;
            while (book[foundContacts[i]-1].name[q] != '\n'){
                printf("%c", book[foundContacts[i]-1].name[q]);
                q++;
            }
            printf(", %s", book[foundContacts[i]-1].number);
        }
    }

    return 0;
}

void example(char numbers[], char storage[], int foundContacts[], int num){
    // the function creates all possible string variants relative to the entered numbers
    // one by one the variants are recorded, checked (calls the function check()), overwritten, checked, overwritten, checked,....
    // for example: possible string variants from the entered numbers 23 is: ad, ae, af, bd, be, bf, cd, ce, cf
    // numbers[] - numbers entered by the user
    // storage[] -  array with possible string contents of the contact
    // foundContacts[] - array for storing indexes of found contacts (will be used for sorting and printing)
    // num - a number to specify the recursion step

    // when a possible string is formed, the function check() is called
    // after that, the storage[] will be overwritten with another option, and the check() will be called again
    if (num == (int)strlen(numbers)) {
        check(storage, foundContacts);
        return;
    }

    // the function uses recursion to collect all possible options
    for (int i = 0; i < (int)strlen(cheats[numbers[num] - '0']); i++) {
        storage[num] = cheats[numbers[num] - '0'][i];
        example(numbers, storage, foundContacts, num+1);
    }

}

void check(char storage[], int foundContacts[]){
    // the function will check if the name or number of contacts contain a possible string(storage[])
    // storage[] - array with possible string contents of the contact, for example: for 23 it will be: 23, ad, ae, af, bd, be, bf, cd, ce, cf
    // foundContacts[] - array for storing indexes of found contacts (will be used for sorting and printing)
    for (int i = 0; i < numOfContacts; ++i) {
        if (strstr(book[i].name, storage) != NULL || strstr(book[i].number, storage) != NULL) {

            // the program checks if the newly found contact has already been found (to prevent repeats)
            for (int j = 0; foundContacts[j] != 0; ++j) {
                if (foundContacts[j] == i + 1) {
                    // if the contact has already been saved, you do not need to add it again
                    goto jump;
                }
            }

            // the program highlights the parts of the name by which it was found
            if (isalpha(storage[0])) {
                char *e;
                int q;
                e = strstr(book[i].name, storage); // pointer to the first entry of storage[] in book[i].name
                q = (int)(e - book[i].name); // index of the first entry of storage[] in book[i].name
                for (int j = q; j < q + (int)strlen(storage); ++j) {
                    book[i].name[j] = (char)toupper(book[i].name[j]);
                }
            }

            // the program adds the index of the newly found contact to the foundContacts[]
            foundContacts[finalIndex] = i + 1;
            finalIndex++;

            jump:;
        }
    }
}

void bubbleSort(int foundContacts[], int foundSize){
    // the function sorts the found contacts according to how they were recorded in the input file
    // the function uses bubble sort algorithm
    // foundContacts[] - array of indexes of found contacts
    // foundSize - number of found contacts
    for (int i = 0; i < foundSize-1; ++i) {
        for (int j = 0; j < foundSize - i - 1; ++j) {
            if(foundContacts[j] > foundContacts[j+1]){
                swap(&foundContacts[j], &foundContacts[j+1]);
            }
        }
    }
}

void swap(int* x, int* y){
    int value = *x;
    *x = *y;
    *y = value;
}
