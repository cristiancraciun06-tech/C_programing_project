#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

typedef struct {
    char nume[50];
    char tara[50];
    int cantitate;
} Produs;

void salveazaInFisier(Produs inventar[], int nrProduse) {
    FILE *f = fopen("stoc.txt", "w");
    if (f == NULL) return;
    for (int i = 0; i < nrProduse; i++) {
        fprintf(f, "%s %s %d\n", inventar[i].nume, inventar[i].tara, inventar[i].cantitate);
    }
    fclose(f);
}

int incarcaDinFisier(Produs inventar[]) {
    FILE *f = fopen("stoc.txt", "r");
    if (f == NULL) return 0;
    int i = 0;
    while (fscanf(f, "%s %s %d", inventar[i].nume, inventar[i].tara, &inventar[i].cantitate) == 3) {
        i++;
    }
    fclose(f);
    return i;
}

int main() {
    Produs inventar[100];
    int nrProduse = incarcaDinFisier(inventar);
    int optiune = -1;

    // MENIU PRINCIPAL
    while (optiune != 0) {
        system(CLEAR);
        printf("\n===================================\n");
        printf("          MENIU PRINCIPAL          \n");
        printf("===================================\n");
        printf("1. Adaugare\n");
        printf("2. Vizualizare\n");
        printf("3. Cautare\n");
        printf("4. Cumparare\n");
        printf("5. Stergere\n");
        printf("0. Exit\n");
        printf("-----------------------------------\n");
        printf("Alege optiunea: ");

        if (scanf("%d", &optiune) != 1) {
            while (getchar() != '\n'); // Curatam buffer-ul daca se introduce text
            continue; // Optiune invalida -> Revine la Meniu Principal (conform schemei)
        }

        system(CLEAR); // Curata ecranul la intrarea in submeniu

        // 1. ADAUGARE
        if (optiune == 1) {
            printf(">>> SUBMENIU: ADAUGARE <<<\n");
            printf("(Scrie '0' la nume pentru BACK)\n\n");
            
            printf("Nume: "); scanf("%s", inventar[nrProduse].nume);
            if (strcmp(inventar[nrProduse].nume, "0") == 0) continue;

            printf("Tara: "); scanf("%s", inventar[nrProduse].tara);
            printf("Cantitate: "); scanf("%d", &inventar[nrProduse].cantitate);
            
            nrProduse++;
            salveazaInFisier(inventar, nrProduse);
        } 
        
        // 2. VIZUALIZARE (Conform schemei)
        else if (optiune == 2) {
            char back_opt[10];
            int stramInSubmeniu = 1;

            while(stramInSubmeniu) {
                system(CLEAR);
                printf(">>> SUBMENIU: VIZUALIZARE <<<\n\n");
                for (int i = 0; i < nrProduse; i++) {
                    printf("%-15s | %-15s | Stoc: %d\n", inventar[i].nume, inventar[i].tara, inventar[i].cantitate);
                }
                
                printf("\n0. Back\n");
                printf("Alege: ");
                scanf("%s", back_opt);
                
                if (strcmp(back_opt, "0") == 0) {
                    stramInSubmeniu = 0; // Este Back? DA -> Iese din loop si revine la Meniu Principal
                }
            }
        }

        // 3. CAUTARE (Conform schemei, cu submeniu specific)
        else if (optiune == 3) {
            int sub_opt = -1;
            
            while (sub_opt != 0) {
                system(CLEAR);
                printf(">>> SUBMENIU: CAUTARE <<<\n");
                printf("1. Dupa nume\n");
                printf("2. Disponibilitate (tara)\n");
                printf("0. Back\n");
                printf("Alege: ");
                
                if (scanf("%d", &sub_opt) != 1) {
                    while (getchar() != '\n');
                    continue;
                }

                if (sub_opt == 1) {
                    char cautat[50];
                    printf("\nIntrodu numele: "); scanf("%s", cautat);
                    for (int i = 0; i < nrProduse; i++) {
                        if (strcmp(inventar[i].nume, cautat) == 0) {
                            printf("Gasit: %s (%s) - %d buc\n", inventar[i].nume, inventar[i].tara, inventar[i].cantitate);
                        }
                    }
                    printf("\nApasa Enter pentru a continua...");
                    getchar(); getchar();
                } 
                else if (sub_opt == 2) {
                    char cautat[50];
                    printf("\nIntrodu tara (disponibilitate): "); scanf("%s", cautat);
                    for (int i = 0; i < nrProduse; i++) {
                        if (strcmp(inventar[i].tara, cautat) == 0) {
                            printf("Gasit: %s (%s) - %d buc\n", inventar[i].nume, inventar[i].tara, inventar[i].cantitate);
                        }
                    }
                    printf("\nApasa Enter pentru a continua...");
                    getchar(); getchar();
                }
                // Daca sub_opt este 0, loop-ul se sparge si revine la main menu
            }
        }

        // 4. CUMPARARE
        else if (optiune == 4) {
            char nume[50];
            printf(">>> SUBMENIU: CUMPARARE <<<\n");
            printf("(Scrie '0' pentru BACK)\n\n");
            
            printf("Nume produs: "); scanf("%s", nume);
            if (strcmp(nume, "0") == 0) continue;

            printf("Cantitate: ");
            int bucati; scanf("%d", &bucati);

            for (int i = 0; i < nrProduse; i++) {
                if (strcmp(inventar[i].nume, nume) == 0) {
                    if (inventar[i].cantitate >= bucati) {
                        inventar[i].cantitate -= bucati;
                        printf("Tranzactie reusita!\n");
                    } else {
                        printf("Stoc insuficient!\n");
                    }
                }
            }
            salveazaInFisier(inventar, nrProduse);
            printf("\nApasa Enter pentru a continua...");
            getchar(); getchar();
        }

        // 5. STERGERE
        else if (optiune == 5) {
            char nume[50];
            printf(">>> SUBMENIU: STERGERE <<<\n");
            printf("(Scrie '0' pentru BACK)\n\n");
            
            printf("Nume produs: "); scanf("%s", nume);
            if (strcmp(nume, "0") == 0) continue;

            for (int i = 0; i < nrProduse; i++) {
                if (strcmp(inventar[i].nume, nume) == 0) {
                    for (int j = i; j < nrProduse - 1; j++) {
                        inventar[j] = inventar[j + 1];
                    }
                    nrProduse--;
                    printf("Produs sters!\n");
                    break;
                }
            }
            salveazaInFisier(inventar, nrProduse);
            printf("\nApasa Enter pentru a continua...");
            getchar(); getchar();
        }
    }

    // X. EXIT (Ales 0 in meniul principal)
    system(CLEAR);
    printf("Iesire din program. La revedere!\n");
    return 0;
}