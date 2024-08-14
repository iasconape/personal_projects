
//this is a work in progress for a personal finance manager! 
// compile with:
//gcc -o PersonalFinanceManager main.c user.c finance.c report.c forecast.c
//and run program with:
//./PersonalFinanceManager

// main.c
#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "finance.h"
#include "report.h"
#include "forecast.h"

void showMenu() {
    printf("\n=== Personal Finance Manager ===\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Enter Income/Expense\n");
    printf("4. View Financial Report\n");
    printf("5. Forecast Savings\n");
    printf("6. Exit\n");
    printf("Choose an option: ");
}

int main() {
    int choice;
    int isAuthenticated = 0;

    loadUsers();
    loadFinanceData();

    while (1) {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                isAuthenticated = login();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                if (isAuthenticated) {
                    enterFinanceData();
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 4:
                if (isAuthenticated) {
                    viewReport();
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 5:
                if (isAuthenticated) {
                    forecastSavings();
                } else {
                    printf("Please login first.\n");
                }
                break;
            case 6:
                printf("Exiting...\n");
                saveUsers();
                saveFinanceData();
                exit(0);
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

// user.h
#ifndef USER_H
#define USER_H

int login();
void registerUser();
void loadUsers();
void saveUsers();

#endif


// user.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"

#define MAX_USERS 100
#define USERNAME_LEN 50
#define PASSWORD_LEN 50
#define USER_DATA_FILE "data/users.dat"

typedef struct {
    char username[USERNAME_LEN];
    char password[PASSWORD_LEN];
} User;

User users[MAX_USERS];
int userCount = 0;

void loadUsers() {
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file != NULL) {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

void saveUsers() {
    FILE *file = fopen(USER_DATA_FILE, "wb");
    if (file != NULL) {
        fwrite(&userCount, sizeof(int), 1, file);
        fwrite(users, sizeof(User), userCount, file);
        fclose(file);
    }
}

int login() {
    char username[USERNAME_LEN], password[PASSWORD_LEN];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login successful.\n");
            return 1;
        }
    }
    printf("Invalid username or password.\n");
    return 0;
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("User limit reached.\n");
        return;
    }

    char username[USERNAME_LEN], password[PASSWORD_LEN];
    printf("Enter a new username: ");
    scanf("%s", username);
    printf("Enter a new password: ");
    scanf("%s", password);

    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    userCount++;

    saveUsers();
    printf("Registration successful.\n");
}

// finance.h
#ifndef FINANCE_H
#define FINANCE_H

void enterFinanceData();
void loadFinanceData();
void saveFinanceData();

#endif

// finance.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "finance.h"

#define MAX_ENTRIES 100
#define DESCRIPTION_LEN 100
#define FINANCE_DATA_FILE "data/finance.dat"

typedef struct {
    char description[DESCRIPTION_LEN];
    float amount;
    char type; // 'I' for Income, 'E' for Expense
} FinanceEntry;

FinanceEntry financeEntries[MAX_ENTRIES];
int entryCount = 0;

void loadFinanceData() {
    FILE *file = fopen(FINANCE_DATA_FILE, "rb");
    if (file != NULL) {
        fread(&entryCount, sizeof(int), 1, file);
        fread(financeEntries, sizeof(FinanceEntry), entryCount, file);
        fclose(file);
    }
}

void saveFinanceData() {
    FILE *file = fopen(FINANCE_DATA_FILE, "wb");
    if (file != NULL) {
        fwrite(&entryCount, sizeof(int), 1, file);
        fwrite(financeEntries, sizeof(FinanceEntry), entryCount, file);
        fclose(file);
    }
}

void enterFinanceData() {
    if (entryCount >= MAX_ENTRIES) {
        printf("Entry limit reached.\n");
        return;
    }

    char description[DESCRIPTION_LEN];
    float amount;
    char type;

    printf("Enter description: ");
    scanf(" %[^\n]s", description);
    printf("Enter amount: ");
    scanf("%f", &amount);
    printf("Is this Income (I) or Expense (E)? ");
    scanf(" %c", &type);

    financeEntries[entryCount].amount = amount;
    strcpy(financeEntries[entryCount].description, description);
    financeEntries[entryCount].type = type;
    entryCount++;

    saveFinanceData();
    printf("Entry saved.\n");
}

// report.h
#ifndef REPORT_H
#define REPORT_H

void viewReport();

#endif

// report.c
#include <stdio.h>
#include "finance.h"

void viewReport() {
    float totalIncome = 0, totalExpense = 0;

    printf("\n=== Financial Report ===\n");
    for (int i = 0; i < entryCount; i++) {
        if (financeEntries[i].type == 'I') {
            totalIncome += financeEntries[i].amount;
        } else if (financeEntries[i].type == 'E') {
            totalExpense += financeEntries[i].amount;
        }
    }

    printf("Total Income: $%.2f\n", totalIncome);
    printf("Total Expenses: $%.2f\n", totalExpense);
    printf("Net Savings: $%.2f\n", totalIncome - totalExpense);
}

// forecast.h
#ifndef FORECAST_H
#define FORECAST_H

void forecastSavings();

#endif

// forecast.c
#include <stdio.h>
#include "finance.h"

void forecastSavings() {
    float totalIncome = 0, totalExpense = 0;
    int months = 0;

    for (int i = 0; i < entryCount; i++) {
        if (financeEntries[i].type == 'I') {
            totalIncome += financeEntries[i].amount;
        } else if (financeEntries[i].type == 'E') {
            totalExpense += financeEntries[i].amount;
        }
    }

    if (entryCount > 0) {
        months = entryCount;  // Assuming one entry per month for simplicity
        float avgSavings = (totalIncome - totalExpense) / months;
        printf("\n=== Savings Forecast ===\n");
        printf("Average Monthly Savings: $%.2f\n", avgSavings);
        printf("Estimated Savings in 6 months: $%.2f\n", avgSavings * 6);
        printf("Estimated Savings in 1 year: $%.2f\n", avgSavings * 12);
    } else {
        printf("No financial data available for forecasting.\n");
    }
}
