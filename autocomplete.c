#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "autocomplete.h"

int compare_terms(const void *t1, const void *t2){
    term *t1_t = (term *)t1;
    term *t2_t = (term *)t2;
    return strcmp(t1_t->term, t2_t->term);
}

void read_in_terms(term **terms, int *pnterms, char *filename){
    FILE *fp = fopen(filename, "r");
    char line[200];

    // check if file is valid
    if(fp == NULL){
        printf("Error opening file %s\n", filename);
        return;
    }

    // get number of lines in this file
    fgets(line, 200, fp);
    line[strlen(line) - 1] = '\0';
    *pnterms = atoi(line);
    // printf("%d\n", *pnterms);

    // allocate memory for all terms in file
    *terms = (term *)malloc((*pnterms)*(sizeof(term)));

    // read in line by line
    for (int i = 1; i <= *pnterms; i++){
        fgets(line, sizeof(line), fp);
        line[strlen(line) - 1] = '\0';
        // double int_part = 0;
        // char str_part[100];

        char *ptr = strchr(line, '\t');
        if (ptr){
            ptr = ptr + 1;
        }
        
        (*terms)[i-1].weight = atof(line);
        strcpy((*terms)[i-1].term, ptr);
    }

    // sort terms by lexographical order of cities
    qsort(*terms, *pnterms, sizeof(term), compare_terms);
    // print to check qsort
    // for (int i = 1; i <= *pnterms; i++){
    //     printf("WEIGHT: %f, CITY: %s\n", ((*terms)[i-1]).weight, ((*terms)[i-1]).term);
    // }
    fclose (fp);
}

// 82641
int lowest_match(struct term *terms, int nterms, char *substr){
    /*The function returns the index in terms of the first term in lexicographic order that matches
the string substr.*/
    int low = 0;
    int high = nterms - 1;
    while (low <= high){
        int mid = (low + high)/2;
        // printf("Low %d\tHigh %d\tMid %s\n", low, high, terms[mid].term);
        if ((mid == 0 || strncmp(substr, terms[mid - 1].term, strlen(substr)) > 0) && (strncmp(substr, terms[mid].term, strlen(substr)) == 0)){
            // printf("%s", terms[mid].term);
            return mid;
        }
        else if (strncmp(substr, terms[mid].term, strlen(substr)) > 0){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    return -1;
}

// 82819
int highest_match(struct term *terms, int nterms, char *substr){
    /* The function returns the index in terms of the last term in lexicographic order that matches
the string substr.*/
    int low = 0;
    int high = nterms - 1;
    while (low <= high){
        int mid = (low + high)/2;
        // printf("Low %d\tHigh %d\tMid %s\n", low, high, terms[mid].term);
        if ((mid == nterms - 1 || strncmp(substr, terms[mid + 1].term, strlen(substr)) < 0) && (strncmp(substr, terms[mid].term, strlen(substr)) == 0)){
            // printf("%s", terms[mid].term);
            return mid;
        }
        else if (strncmp(substr, terms[mid].term, strlen(substr)) < 0){
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    return -1;
}

int compare_weights(const void *t1, const void *t2){
    term *t1_t = (term *)t1;
    term *t2_t = (term *)t2;
    return (t2_t->weight - t1_t->weight);
}

// 178 answers
void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){
    /* The function takes terms (assume it is sorted lexicographically in increasing order), the number
    of terms nterms, and the query string substr, and places the answers in answer, with *n_answer
    being the number of answers. The answers should be sorted by weight in non-increasing order */
    int lowest = lowest_match(terms, nterms, substr);
    int highest = highest_match(terms, nterms, substr);
    int j = 0;
    *n_answer = 1 + highest - lowest;

    // check cases for when NOT in
    // check cases for when lowest = highest
    if (*n_answer == 0){
        *answer = (term *)malloc(sizeof(term));   
        (*answer)[0].weight = terms[lowest].weight;
        strcpy((*answer)[0].term, terms[lowest].term);             
    }
    else {
        *answer = (term *)malloc(*n_answer * (sizeof(term)));
        for (int i = lowest; i <= highest; i++){
            // printf("%d", terms[i].weight);
            (*answer)[j].weight = terms[i].weight;
            strcpy((*answer)[j].term, terms[i].term);
            j++;
        }
    }

    qsort(*answer, *n_answer, sizeof(term), compare_weights);
    // printf("%s", ((*answer)[0]).term);
    // print to check qsort result
    // for (int i = 1; i <= *n_answer; i++){
    //     printf("WEIGHT: %f, CITY: %s\n", ((*answer)[i]).weight, ((*answer)[i]).term);
    // }

}