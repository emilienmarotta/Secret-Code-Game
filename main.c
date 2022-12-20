/* Jeu du code mystère */
/*** Fonctionnement du jeu : 
* Un code mystère composé de 5 chiffres compris entre 0 et 9 est choisi aléatoirement. Chaque chiffre ne peut apparaître qu'une fois.
* Le joueur doit retrouver ce code en proposant des combinaisons de 5 chiffres.
* Le programme indique ensuite si des chiffres sont bien placés, mal placés ou inexistants dans ce code.
* Le joueur propose des combinaisons jusqu'à trouver le bon code.
***/

//# Directives de préprocesseurs

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//# Constantes

#define NB_DIGITS 5 // Nombre de chiffres dans le code secret
#define MAX_NB_PARTIES 256 // Nombre maximum de parties

//# Prototypes des fonctions

int are_codes_equal(int secretCode[], int codeInputByPlayer[]);
void reset_code(int code[]);
void assign_random_values(int secretCode[]);
void display_secretCode(int secretCode[]);
int is_in_secretCode(int secretCode[], int value, int position);
void input_code(int codeInputByPlayer[]);
int check_if_input_is_valid(int codeInput);
void display_result_V1(int secretCode[], int codeInputByPlayer[]); // Affichage du résultat pour la première version (version "facile")
void display_result_V2(int secretCode[], int codeInputByPlayer[]); // Affichage du résultat pour la deuxième version (version "difficile")
int is_at_correct_position(int secretCode[], int value, int position);
float calculate_average_nb_attempts(int nbParties, int nbAttemptsTable[]);
int get_smallestValue(int nbParties, int nbAttemptsTable[]);
int get_largestValue(int nbParties, int nbAttemptsTable[]);
void display_statistics(int nbParties, int largestValue, int smallestValue, float averageNbAttempts, int nbAttemptsTable[]);

//# Boucle principale

int main(void) {

    int secretCode[NB_DIGITS];
    int codeInputByPlayer[NB_DIGITS];
    int menu_option;
    int difficulty;
    int nbParties = 0;
    int nbAttemptsTable[MAX_NB_PARTIES]; // Tableau contenant le nombre de tentatives pour chaque partie 
    int nbAttempts = 0; // Contient le nombre de tentatives pour la partie en cours
    float averageNbAttempts; // Contient le nombre moyen de tentatives
    int smallestValue; // Contient le nombre minimum de tentatives 
    int largestValue; // Contient le nombre maximum de tentatives

    srand(time(NULL));

    do {
        do {
            printf("Menu :\n1 - Nouvelle partie\n2 - Quitter le jeu\n>> ");
            scanf("%d", &menu_option);
            if (menu_option != 1 && menu_option != 2) {
                printf("Valeur incorrecte...\n");
            }
        } while (menu_option != 1 && menu_option != 2);

        if (menu_option == 1) {

            nbParties ++; // Nouvelle partie => incrémentation du nombre de parties
            reset_code(secretCode); // Réinitialisation du code secret
            assign_random_values(secretCode); // Génération du code secret de façon aléatoire
            
            do {
                printf("Difficulte :\n1 - Facile\n2 - Difficile\n>> ");
                scanf("%d", &difficulty);
                if (difficulty != 1 && difficulty != 2) {
                    printf("Valeur incorrecte...\n");
                }
            } while (difficulty != 1 && difficulty != 2);
            
            do {
                nbAttempts ++;
                input_code(codeInputByPlayer); // Proposition du joueur

                display_secretCode(codeInputByPlayer); // Affichage de sa proposition 
                printf("\n");
                // display_secretCode(secretCode);

                if (are_codes_equal(secretCode, codeInputByPlayer) == 0) {
                    if (difficulty == 1) {
                        display_result_V1(secretCode, codeInputByPlayer);
                    } else {
                        display_result_V2(secretCode, codeInputByPlayer);
                    }
                } 

                printf("\n");

                if (are_codes_equal(secretCode, codeInputByPlayer) == 1) {
                    printf("Code correct !\n");
                    nbAttemptsTable[nbParties - 1] = nbAttempts; // Le nombre de tentatives de la partie i est stocké dans le tableau nbAttemptsTable[] à l'indice i - 1 (car un tableau commence à l'indice 0)
                    nbAttempts = 0; // Réinitialisation du nombre de tentatives (car la partie est terminée)
                } else {
                    printf("Code incorrect...\n");
                }

            } while (are_codes_equal(secretCode, codeInputByPlayer) == 0); 
            
        } else if (menu_option == 2) {
            if (nbParties == 0) {
                printf("Vous n'avez joue aucune partie...\n");
            } else {
                display_statistics(nbParties, largestValue, smallestValue, averageNbAttempts, nbAttemptsTable);
            }
        }
    } while (menu_option != 2 && nbParties < MAX_NB_PARTIES); // Continuer à jouer tant que le joueur n'a pas choisi l'option <quitter> et que le nombre de parties est inférieur au nombre maximum de parties
    
    if (nbParties == MAX_NB_PARTIES) {
        printf("Vous avez deja joue %d parties ! Jetez un coup d'oeil a vos statistiques et relancer le jeu si vous souhaitez continuer a jouer ;)\n", MAX_NB_PARTIES);
        display_statistics(nbParties, largestValue, smallestValue, averageNbAttempts, nbAttemptsTable);
    }
    
    return 0;
}

//# Fonctions

/* Renvoie <vrai> si le code saisi par le joueur est égal au code secret */
int are_codes_equal(int secretCode[], int codeInputByPlayer[]) {
    int areCodesEqual = 1;
    int i = 0;
    while (areCodesEqual == 1 && i < NB_DIGITS) {
        if (secretCode[i] != codeInputByPlayer[i]) {
            areCodesEqual = 0;
        }
        i ++;
    }
    return areCodesEqual;
}

/* Initialisation des valeurs du code à -1 */
void reset_code(int code[]) {
    for (int i = 0; i < NB_DIGITS; i ++) code[i] = -1; 
}

/* Chaque valeur du code secret prend une valeur aléatoire différente des autres comprise entre 0 et 9 */
void assign_random_values(int secretCode[]) {
    int randomValue;
    for (int i = 0; i < NB_DIGITS; i ++) { 
        randomValue = rand() % 10;
        while (is_in_secretCode(secretCode, randomValue, (i + 1)) == 1) {
            randomValue = rand() % 10;
        }
        secretCode[i] = randomValue;
    }
}

/* Affichage du code secret */
void display_secretCode(int secretCode[]) {
    for (int i = 0; i < NB_DIGITS; i ++) {
        printf("%d", secretCode[i]);
        if (i < NB_DIGITS - 1) {
            printf(" | ");
        }
    }
}

/* Renvoie <vrai> si la valeur est présente dans le code */
int is_in_secretCode(int secretCode[], int value, int position){
    int found = 0;
    for (int j = 0; j < position; j ++) {
        if (value == secretCode[j]) found = 1;
    }
    return found;
}

void input_code(int codeInputByPlayer[]) {
    int codeInput;
    do { // Demande à l'utilisateur de saisir sa proposition tant que sa proposition comporte plus de 5 chiffres
        printf("Code : ");
        scanf("%d", &codeInput);
    } while (check_if_input_is_valid(codeInput) == 0);
    /* Séparation des chiffres saisis dans un tableau d'entiers (codeInputByPlayer) */
    for (int i = 4; i >= 0; i --) {
        codeInputByPlayer[i] = codeInput % 10;
        codeInput /= 10;
    }   
}

/* Renvoie faux si le code saisi par l'utilisateur comporte plus de 5 chiffres */
int check_if_input_is_valid(int codeInput) {
    if (log10f(codeInput) >= 5) return 0; // log10f(99999) = 4.999... et log10f(100000) = 5.000...
    else return 1;
}

/* Affiche "+" si le chiffre i est bien placé, "~" si i est mal placé, sinon "-" si i est inexistant dans le code  */
void display_result_V1(int secretCode[], int codeInputByPlayer[]) {
    for (int i = 0; i < NB_DIGITS; i ++) {
        if (is_in_secretCode(secretCode, codeInputByPlayer[i], NB_DIGITS) == 0) {
            printf("\n> [%d] : -", codeInputByPlayer[i]);
        } 
        else if (is_at_correct_position(secretCode, codeInputByPlayer[i], i)) {
            printf("\n> [%d] : +", codeInputByPlayer[i]);
        }
        else {
            printf("\n> [%d] : ~", codeInputByPlayer[i]);
        }
    }
}

/* Affiche le nombre de chiffres bien placés ainsi que le nombre de chiffres mal placés */
void display_result_V2(int secretCode[], int codeInputByPlayer[]) {
    int nbDigitsAtCorrectPosition = 0; // Contient le nombre de chiffres bien placés
    int nbDigitsAtBadPosition = 0; // Contient le nombre de chiffres mal placés
    for (int i = 0; i < NB_DIGITS; i ++) {
        if (is_at_correct_position(secretCode, codeInputByPlayer[i], i)) {
            nbDigitsAtCorrectPosition ++;
        }
        else if (is_in_secretCode(secretCode, codeInputByPlayer[i], NB_DIGITS) == 1){
            nbDigitsAtBadPosition ++;
        }
    }

    if (nbDigitsAtCorrectPosition == 0) {
        printf("\n> Aucun chiffre bien place");
    } else if (nbDigitsAtCorrectPosition == 1) {
        printf("\n> Un seul chiffre bien place");
    } else {
        printf("\n> %d chiffres bien places", nbDigitsAtCorrectPosition);
    }

    if (nbDigitsAtBadPosition == 0) {
        printf("\n> Aucun chiffre mal place");
    } else if (nbDigitsAtBadPosition == 1) {
        printf("\n> Un seul chiffre mal place");
    } else {
        printf("\n> %d chiffres mal places", nbDigitsAtBadPosition);
    }
}

int is_at_correct_position(int secretCode[], int value, int position) {
    if (value == secretCode[position]) return 1;
    else return 0;
}

// Renvoie le nombre moyen de tentatives
float calculate_average_nb_attempts(int nbParties, int nbAttemptsTable[]) {
    float averageNbAttempts = 0; // Contient le nombre moyen de tentatives
    for (int i = 0; i < nbParties; i ++) {
        averageNbAttempts += nbAttemptsTable[i];
    }
    averageNbAttempts /= nbParties;
    return averageNbAttempts;
}

/* Retourne le plus petit nombre de tentatives */
int get_smallestValue(int nbParties, int nbAttemptsTable[]) {
    int smallestValue = nbAttemptsTable[0];
    for (int i = 1; i < nbParties; i ++) {
        if (nbAttemptsTable[i] < smallestValue) {
            smallestValue = nbAttemptsTable[i];
        }
    }
    return smallestValue;
}

/* Retourne le plus grand nombre de tentatives */
int get_largestValue(int nbParties, int nbAttemptsTable[]) {
    int largestValue = nbAttemptsTable[0];
    for (int i = 1; i < nbParties; i ++) {
        if (nbAttemptsTable[i] > largestValue) {
            largestValue = nbAttemptsTable[i];
        }
    }
    return largestValue;
}

void display_statistics(int nbParties, int largestValue, int smallestValue, float averageNbAttempts, int nbAttemptsTable[]) {
    printf("Nombre de parties : %d\n", nbParties);
    averageNbAttempts = calculate_average_nb_attempts(nbParties, nbAttemptsTable);
    printf("Nombre moyen de tentatives : %.1f\n", averageNbAttempts);
    smallestValue = get_smallestValue(nbParties, nbAttemptsTable);
    printf("Nombre minimum de tentatives : %d\n", smallestValue);
    largestValue = get_largestValue(nbParties, nbAttemptsTable);
    printf("Nombre maximum de tentatives : %d\n", largestValue);
}


/* Améliorations
- design
- gestion des saisies
- lien vers GitHub
*/