/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "admin.h"

/**********************************COMPANY********************************************/

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

int createCompany(Companies *companies) {
    int nif;

    nif = getInt(MIN_NIF, MAX_NIF, MSG_NIF);

    if (verifyNif(*companies, nif) == -1) {
        companies->companies[companies->counter].nif = nif;

        readString(companies->companies[companies->counter].name, 50, MSG_NAME);

        companies->companies[companies->counter].category = getInt(0, 3, MSG_CATEGORY);

        //listActivityBranch();
        companies->companies[companies->counter].branch = getInt(0, 3, MSG_BRANCH); //adicionar o contador e trocar o 3

        readString(companies->companies[companies->counter].street, 50, MSG_STREET);
        readString(companies->companies[companies->counter].locality, 50, MSG_LOCALITY);

        companies->companies[companies->counter].postalCode = getInt(MIN_POSTALCODE, MAX_POSTALCODE, MSG_POSTALCODE);
        verify_PostalCode(&(companies->companies[companies->counter].postalCode));


        return companies->counter++;
    }
    return -1;
}

void createCompanies(Companies *companies) {
    if (companies->counter < MAX_COMPANIES) {
        if (createCompany(companies) == -1) {
            puts("COMPANIE DOESN'T EXIST\n");
        }
    } else {
        puts(FULL_LIST);
    }
}

void printCompany(Company company) {
    printf("\n%5d %10s ", company.nif, company.name);

    switch (company.category) {
        case MICRO:
            printf("MICRO");
            break;
        case SME:
            printf("SME");
            break;
        case BIG_COMPANY:
            printf("BIG COMPANY");
            break;
    }
    printf("%5d %10s %10s %5d", company.branch, company.street, company.locality, company.postalCode);

}

void listCompanies(Companies companies) {
    int i;

    if (companies.counter > 0) {
        for (i = 0; i < companies.counter; i++) {
            printCompany(companies.companies[i]);
        }
    } else {
        puts(EMPTY_LIST);
    }
}

void updateCompany(Company *company) {

    readString(company->name, 50, MSG_NAME);
    company->category = getInt(0, 3, MSG_CATEGORY);
    company->branch = getInt(0, 50, MSG_BRANCH); //alterar
    readString(company->street, 50, MSG_STREET);
    readString(company->locality, 50, MSG_LOCALITY);
    company->postalCode = getInt(MAX_POSTALCODE, MIN_POSTALCODE, MSG_POSTALCODE);
    verify_PostalCode(&company->postalCode);
}

void updateCompanies(Companies *companies) {

    int i, nif = verifyNif(*companies, getInt(MIN_NIF, MAX_NIF, MSG_NIF));

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
    Company *companyA = (Company *)a;
    Company *companyB = (Company *)b;
    return companyB->views - companyA->views;
}

void top5lookedCompanys(Companies *companies){
    int i;
    
    if (companies->counter > 0) {
        //esta a ordenar a lista de empresas decrescente com base nas visualizações usando o quick sort
        qsort(companies->companies, companies->counter, sizeof(Company), compare_company);
        
        for (i = 0; i < companies->counter && i <= TOP_5; i++) {
            printf("%s\n", companies->companies[i].name);
        }
    } else {
        puts(ERROR_EMPTY_LIST);
    }
}

/**********************************ACTIVITY BRANCH************************************/


char convertLowercase(ActivityBranchs *name) {
    int counter, i;

    while ((name->branchs[name->counter].name)[counter] != '\0') {
        counter++;
    }

    if ((name->branchs[name->counter].name)[counter - 1] == '\n') {
        (name->branchs[name->counter].name)[--counter] = '\0';
    }

    for (i = 0; i < counter; i++) {
        if (name->branchs[name->counter].name[i] >= 65 && name->branchs[name->counter].name[i] <= 90) {

           return name->branchs[name->counter].name[i] += 32;
        }
    }

    return 0;
}

char compareString(ActivityBranchs *name, char readName) {
    if (strcmp(&name, readName) == 0) {
        return 1;  
    }
    return 0;  
}

void createActivityBranchs(ActivityBranchs *branchs){
    
    if (branchs->counter < MAX_ACTIVITY_BRANCHS) {
        
        if (createActivityBranch(branchs) == -1) {
            puts(AB_ALREADY_EXISTS);
        } 
        
    } else {
        puts(FULL_LIST);
    }
    
}

int createActivityBranch(ActivityBranchs *branchs){
    
    int code = getInt(MIN_AB_CODE_VALUE, MAX_AB_CODE_VALUE, CODE_MSG);
    
    if (searchActivityBranch(*branchs, code) == -1) {
        branchs->branchs[branchs->counter].code = code;
        
        readString(branchs->branchs[branchs->counter].name, MAX_AB_NAME_SIZE, NAME_MSG);
        
        branchs->branchs[branchs->counter].state = getInt(MIN_STATE_VALUE, MAX_STATE_VALUE, STATE_MSG);
        
        return branchs->counter++;
    }
    return -1;
}

int searchActivityBranch(ActivityBranchs branchs, int code) {
    int i;
    for (i = 0; i < branchs.counter; i++) {
        if (branchs.branchs[i].code == code) {
            return i;
        }
    }
    return -1;
}

void listActivityBranchs(ActivityBranchs branchs){
    if (branchs.counter > 0) {
        int i;
        printf("\n%-5s %-15s %-10s\n", "CODE", "NAME", "STATE");
        for (i = 0; i < branchs.counter; i++) {
            printActivityBranch(branchs.branchs[i]);
        }
    } else {
        puts(EMPTY_LIST);
    }
}

void printActivityBranch(ActivityBranch branch){
    
    printf("%-5d %-15s", branch.code, branch.name);
    
    if (branch.state == 1) {
        printf("%-10s\n", "Active");
    } else {
        printf("%-10s\n", "Inactive");
    }
}

void updateActivityBranchs(ActivityBranchs *branchs){
    
    int code = searchActivityBranch(*branchs, getInt(MIN_AB_CODE_VALUE, MAX_AB_CODE_VALUE, CODE_MSG));
    
    if (code != -1) {
        updateActivityBranch(&branchs->branchs[code]);
    } else {
        puts(AB_DOES_NOT_EXIST);
    }
    
}

void updateActivityBranch(ActivityBranch *branch){
    
    branch->state = getInt(MIN_STATE_VALUE, MAX_STATE_VALUE, STATE_MSG);
    
}

void deleteActivityBranchs(ActivityBranchs *branchs){
    int i, code = searchActivityBranch(*branchs, getInt(MIN_AB_CODE_VALUE, MAX_AB_CODE_VALUE, CODE_MSG));
    
    if (code != -1) {
        
        for (i = code; i < branchs->counter - 1; i++) {
            branchs->branchs[i] = branchs->branchs[i + 1];
        }
        
        deleteActivityBranch(&branchs->branchs[i]);
        
        branchs->counter--;
        
    } else {
        puts(AB_DOES_NOT_EXIST);
    }
    
}

void deleteActivityBranch(ActivityBranch *branch){
    branch->code = 0;
    strcpy(branch->name, "");
    branch->state = 0;
}