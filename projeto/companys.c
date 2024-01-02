#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "branchs.h"
#include "companys.h"

int verifyNif(Companies companies, int nif) {
    int i;
    for (i = 0; i < companies.counter; i++) {
        if (companies.companies[i].nif == nif) {
            return i;
        }
    }
    return -1;
}

int verify_PostalCode(int *postalCode) {
    int i;

    for (i = 0; i < 8; i++) {
        if (i == 4) {
            if (postalCode[i] != '-') {
                for (i = 7; i >= 4; i--) {
                    postalCode[i] = postalCode[i - 1];
                }
                postalCode[i] = '-';
            }
        }

    }

    return 1;
}

int createCompany(Companies *companies, ActivityBranchs *branchs) {
    int nif;

    nif = getInt(MIN_NIF, MAX_NIF, MSG_NIF);

    if (verifyNif(*companies, nif) == -1) {
        companies->companies[companies->counter].nif = nif;

        readString(companies->companies[companies->counter].name, MAX_COMPANY_NAME_SIZE, MSG_NAME);

        companies->companies[companies->counter].category = getInt(0, 3, MSG_CATEGORY);

        listActivityBranchs(*branchs);
        companies->companies[companies->counter].branch = getInt(1, branchs->counter, MSG_BRANCH);

        readString(companies->companies[companies->counter].street, 50, MSG_STREET);
        readString(companies->companies[companies->counter].locality, 50, MSG_LOCALITY);

        companies->companies[companies->counter].postalCode = getInt(MIN_POSTALCODE, MAX_POSTALCODE, MSG_POSTALCODE);
        verify_PostalCode(&(companies->companies[companies->counter].postalCode));
        
        companies->companies[companies->counter].state = getInt(0, 1, MSG_STATE);
        companies->companies[companies->counter].views = 0;
        companies->companies[companies->counter].rate = 0;


        return companies->counter++;
    }
    return -1;
}

void createCompanies(Companies *companies, ActivityBranchs *branchs) {
    
    if (branchs->counter > 0) {
        if (companies->counter < MAX_COMPANIES) {
            if (createCompany(companies, branchs) == -1) {
                puts("COMPANIE DOESN'T EXIST\n");
            }
        } else {
            puts(FULL_LIST);
        }
    } else {
        puts("ERROR: No Activity Branchs created.");
    }
    
}

void printCompany(Company company, ActivityBranchs branchs) {
    int i;
    
    printf("\n%-10d %-20s ", company.nif, company.name);

    switch (company.category) {
        case MICRO:
            printf("%-15s", "MICRO");
            break;
        case SME:
            printf("%-15s", "SME");
            break;
        case BIG_COMPANY:
            printf("%-15s", "BIG COMPANY");
            break;
    }
    
    for (i = 0; i < branchs.counter; i++) {
        
        if (company.branch == branchs.branchs[i].code) {
            printf("%-15s ", branchs.branchs[i].name);
        }
        
    }
    
    printf("%-20s %-20s %-15d %-15.2f %-15d", company.street, company.locality, company.postalCode, company.rate, company.views);
    
    if (company.state == 0) {
        printf("%-10s", "Inactive\n");
    } else {
        printf("%-10s", "Active\n");
    }
    
}

void listCompanies(Companies companies, ActivityBranchs branchs) {
    int i;

    printf("\n%-10s %-20s %-15s %-15s %-20s %-20s %-15s %-15s %-15s %-10s\n", "NIF", "NAME", "CATEGORY", "BRANCH", "STREET", "LOCALITY", "POSTAL CODE", "RATE", "VIEWS", "STATE");
    if (companies.counter > 0) {
        for (i = 0; i < companies.counter; i++) {
            printCompany(companies.companies[i], branchs);
        }
    } else {
        puts(EMPTY_LIST);
    }
}


void updateCompany(Company *company) {

    readString(company->name, MAX_COMPANY_NAME_SIZE, MSG_NAME);
    company->category = getInt(0, 3, MSG_CATEGORY);
    company->branch = getInt(0, 50, MSG_BRANCH); //alterar
    readString(company->street, 50, MSG_STREET);
    readString(company->locality, 50, MSG_LOCALITY);
    company->postalCode = getInt(MIN_POSTALCODE, MAX_POSTALCODE, MSG_POSTALCODE);
    verify_PostalCode(&company->postalCode);
    company->state = getInt(0, 1, MSG_STATE);
}

void updateCompanies(Companies *companies) {

    int nif = verifyNif(*companies, getInt(MIN_NIF, MAX_NIF, MSG_NIF));

    if (nif != -1) {
        updateCompany(&companies->companies[nif]);
    } else {

        puts("ERROR");
    }
}

//alterar função quando houver comentarios de utilizadores

void deleteCompanyData(Company *company) {

    company->nif = 0;
    strcpy(company->name, "");
    company->category = 0;
    company ->branch = 0;
    strcpy(company->street, "");
    strcpy(company->locality, "");
    company->postalCode = 0;
}

//alterar função quando criarmos função para os comenatarios

void deleteCompanies(Companies *companies) {

    int i, nif = verifyNif(*companies, getInt(MIN_NIF, MAX_NIF, MSG_NIF));

    if (nif != -1) {
        for (i = nif; i < companies->counter - 1; i++) {
            companies->companies[i] = companies->companies[i + 1];
        }

        deleteCompanyData(&companies->companies[i]);

        companies->counter--;
    } else {
        puts("ERROR");
    }
}

/**
 * @brief Comparison function for sorting companies by number of views.
 *
 * This function is used as a comparison function for sorting an array of 'Company' structures
 * in descending order based on the number of views. It is intended to be used with the
 * qsort function or similar sorting algorithms.
 *
 * @param a Pointer to the first company structure to compare.
 * @param b Pointer to the second company structure to compare.
 * @return An integer value representing the comparison result:
 *         - Negative value if 'b' has more views than 'a'.
 *         - Zero if 'a' and 'b' have the same number of views.
 *         - Positive value if 'a' has more views than 'b'.
 */

int compare_company(const void *a, const void *b) {
    const Company *companyA = (const Company *)a;
    const Company *companyB = (const Company *)b;
    return companyB->views - companyA->views;
}

void top5lookedCompanies(Companies *companies) {
    if (companies->counter > 0) {
        // Sorting the list of companies in descending order based on views using quicksort
        qsort(companies->companies, companies->counter, sizeof(Company), compare_company);

        printf("The most looked companies are:\n");

        int numPrinted = 0;
        for (int i = 0; i < companies->counter && numPrinted < TOP_5; i++) {
            printf("%s\n", companies->companies[i].name);
            numPrinted++;
        }
    } else {
        puts(ERROR_EMPTY_LIST);
    }
}


int compare_rate(const void *a, const void *b) {
    const Company *companyA = (const Company *)a;
    const Company *companyB = (const Company *)b;

    if (companyA->rate < companyB->rate) return 1;
    if (companyA->rate > companyB->rate) return -1;
    return 0;
}

void top5bestCompanies(Companies *companies) {
    if (companies->counter > 0) {
        // Sorting the list of companies in descending order based on rate using quicksort
        qsort(companies->companies, companies->counter, sizeof(Company), compare_rate);

        printf("Top 5 best companies are:\n");

        int numPrinted = 0;
        for (int i = 0; i < companies->counter && numPrinted < TOP_5; i++) {
            printf("%s\n", companies->companies[i].name);
            numPrinted++;
        }
    } else {
        puts(ERROR_EMPTY_LIST);
    }
}

int searchCompanyByName(Companies companies, char *name) {
    int i;
    for (i = 0; i < companies.counter; i++) {
        if (strcmp(companies.companies[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

float company_average(Company *company, float rate) {
    float media;

    media = (company->rate + rate) / 2;

    return media;
}

void rate_company(Companies *companies) {
    char name[MAX_NAME];
    float rate;
    int code;

    readString(name, MAX_NAME, MSG_NAME_COMP);

    code = searchCompanyByName(*companies, name);
    
    if (code != -1) {

        rate = getFloat(MIN_RATING, MAX_RATING, MSG_RATING);
        if (companies->companies[code].rate == 0) {
            companies->companies[code].rate = rate;
        } else {
            companies->companies[code].rate = company_average(&companies->companies[code], rate);
        }
        printf("%.2f", companies->companies[code].rate);
    } else {
        puts(ERROR_COMPANY_NOT_FOUND);
    }
}

void listCompaniesByName(Companies *companies, ActivityBranchs branchs) {

    int companyCode = 0;
    char name[MAX_COMPANY_NAME_SIZE];
    readString(name, MAX_COMPANY_NAME_SIZE, MSG_NAME);

    companyCode = searchCompanyByName(*companies, name);

    if (companyCode != -1) {
        printf("\n%-10s %-20s %-15s %-15s %-20s %-20s %-15s %-15s %-15s %-10s\n", "NIF", "NAME", "CATEGORY", "BRANCH", "STREET", "LOCALITY", "POSTAL CODE", "RATE", "VIEWS", "STATE");
        companies->companies[companyCode].views++;
        printCompany(companies->companies[companyCode], branchs);

    } else {

        puts("ERROR: Company not found.");

    }

}

int searchCompanyByCategory(Companies companies, int category) {
    int i;
    for (i = 0; i < companies.counter; i++) {
        if (companies.companies[i].category == category) {
            return i;
        }
    }
    return -1;
}

void listCompaniesByCategory(Companies *companies, ActivityBranchs branchs) {

    int i = 0, category = 0, counter = 0;

    category = getInt(0, 2, MSG_CATEGORY);

    printf("\n%-10s %-20s %-15s %-15s %-20s %-20s %-15s %-15s %-15s %-10s\n", "NIF", "NAME", "CATEGORY", "BRANCH", "STREET", "LOCALITY", "POSTAL CODE", "RATE", "VIEWS", "STATE");
    for (i = 0; i < companies->counter; i++) {
        if (companies->companies[i].category == category) {
            companies->companies[i].views++;
            printCompany(companies->companies[i], branchs);
            counter++;
        }
    }

    if (!counter) {
        puts("No companies found.");
    }

}

void listCompaniesByLocality(Companies *companies, ActivityBranchs branchs) {

    int i, counter = 0;
    char locality[MAX_COMPANY_LOCALITY_SIZE];
    readString(locality, MAX_COMPANY_LOCALITY_SIZE, MSG_LOCALITY);

    printf("\n%-10s %-20s %-15s %-15s %-20s %-20s %-15s %-15s %-15s %-10s\n", "NIF", "NAME", "CATEGORY", "BRANCH", "STREET", "LOCALITY", "POSTAL CODE", "RATE", "VIEWS", "STATE");
    for (i = 0; i < companies->counter; i++) {
        if (strcmp(companies->companies[i].locality, locality) == 0) {
            companies->companies[i].views++;
            printCompany(companies->companies[i], branchs);
            counter++;
        }
    }

    if (!counter) {
        puts("No companies found.");
    }

}

/**
 * @brief 
 * @param email
 * @return 
 */
int verifyEmail(char *email) {
    int i, tam, x;
    char *atSign = NULL, *point = NULL;

    tam = strlen(email);

    for (i = 0; i < tam; i++) {
        if (email[i] == '@') {
            atSign = email + i;
            x = i;
            break;
        }
    }

    if (atSign == NULL) {
        return 0;
    }

    for (i = x; i < tam; i++) {
        if (email[i] == '.') {
            point = email + i;
            break;
        }
    }

    if (point == NULL) {
        return 0;
    }
    if (point == atSign + 1) {
        return 0;
    }

    if (point + 1 == email + tam) {
        return 0;
    }

    return 1;
}

void addComment(Company *company) {
    
    //char email[MAX_EMAIL], name[MAX_NAME], comment[MAX_COMMENT_CARACTER];
    
//    if (company->last_comment_position < MAX_COMMENTS_SIZE) {
//        readString(company->comments[company->last_comment_position].email, MAX_EMAIL, MSG_EMAIL);
//        readString(company->comments[company->last_comment_position].name, MAX_NAME, MSG_NAME);
//        readString(company->comments[company->last_comment_position].comment, MAX_COMMENT_CARACTER, MSG_COMMENT);
//        
//        company->last_comment_position++;
//    } else {
//        puts("ERROR: Maximum of comments reached.");
//    }
    
}

//void expandCommentsCapacity(Comments *comments) {
//    int tam = (comments->size) == 0 ? MAX_COMMENTS_SIZE : comments->size * 2;
//    Comment *temp = (Comment*) realloc(comments->comments, sizeof (Comment) * (tam));
//    if (temp != NULL) {
//        comments->size = tam;
//        comments->comments = temp;
//    }
//}

void addComments(Companies *companies) {
    
    int companyCode = 0;
    char name[MAX_COMPANY_NAME_SIZE];
    readString(name, MAX_COMPANY_NAME_SIZE, MSG_NAME);

    companyCode = searchCompanyByName(*companies, name);

    if (companyCode != -1) {
        addComment(&companies->companies[companyCode]);
    } else {
        puts("ERROR: Company not found.");
    }
}

int selectCompany(Companies companies) {
    
    int companyCode = 0;
    char name[MAX_COMPANY_NAME_SIZE];
    readString(name, MAX_COMPANY_NAME_SIZE, MSG_NAME);

    companyCode = searchCompanyByName(companies, name);

    if (companyCode != -1) {

        return companyCode;

    } else {

        puts("ERROR: Company not found.");

    }
    return -1;
}

void saveCompanies(Companies *companies, char *file){
    int i;

    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    
    fwrite(&companies->counter, sizeof (int), 1, fp);
    
    for (i = 0; i < companies->counter; i++) {
        fwrite(&companies->companies[i], sizeof (Company), 1, fp);
    }
    
    fclose(fp);
}

void freeCompanies(Companies *companies) {
    if (companies->companies) {
        free(companies);
    }

    companies = NULL;
}

void loadCompanies(Companies *companies, char *file) {
     int i, success = 0;
     
    FILE *fp = fopen(file, "rb");
    if (fp != NULL) {
        fread(&companies->counter, sizeof (int), 1, fp);

        for (i = 0; i < companies->counter; i++) {
            fread(&companies->companies[i], sizeof (Company), 1, fp);
        }
        
        success = 1;
        fclose(fp);
    } else {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    if (!success) {
        fp = fopen(file, "wb");
        
        if (fp != NULL) {

            //companies->companies = (Company*) malloc(ROOMS_INITIAL_CAPACITY * sizeof (Company));
            companies->counter = 0;

            fclose(fp);
        }
        
    }
    
}