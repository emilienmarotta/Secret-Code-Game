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
#define MAX_NB_PARTIES 20 // Nombre maximum de parties

//# Prototypes des fonctions

int are_codes_equal(int secretCode[], int codeInputByPlayer[]);
void reset_code(int code[]);
void assign_random_values(int secretCode[]);
void display_secretCode(int secretCode[]);
int is_in_secretCode(int secretCode[], int value, int position);
void input_code(int codeInputByPlayer[]);
int check_if_input_is_valid(int codeInput);
void display_result(int secretCode[], int codeInputByPlayer[]);
int is_at_correct_position(int secretCode[], int value, int position);
float calculate_average_nb_attempts(int nbParties, int nbAttemptsTable[]);
int get_smallest_value(int nbParties, int nbAttemptsTable[]);
int get_largest_value(int nbParties, int nbAttemptsTable[]);

//# Boucle principale

int main(void) {

    int secretCode[NB_DIGITS];
    int codeInputByPlayer[NB_DIGITS];
    int menu_option;
    int nbParties = 0;
    int nbAttemptsTable[MAX_NB_PARTIES]; // Tableau contenant le nombre de tentatives pour chaque partie 
    int nbAttempts = 0; // Contient le nombre de tentatives pour la partie en cours
    float averageNbAttempts; // Contient le nombre moyen de tentatives
    int smallest_value; // Contient le nombre minimum de tentatives 
    int largest_value; // Contient le nombre maximum de tentatives

    srand(time(NULL));

    do {
        do {
            printf("Menu : \n1 - Nouvelle partie\n2 - Quitter le jeu\n>> ");
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
                nbAttempts ++;
                input_code(codeInputByPlayer); // Proposition du joueur

                display_secretCode(codeInputByPlayer); // Affichage de sa proposition 
                printf("\n");
                display_secretCode(secretCode);
                // printf("\n");
                display_result(secretCode, codeInputByPlayer);
                
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
                printf("Nombre de parties : %d\n", nbParties);
                averageNbAttempts = calculate_average_nb_attempts(nbParties, nbAttemptsTable);
                printf("Nombre moyen de tentatives : %.1f\n", averageNbAttempts);
                smallest_value = get_smallest_value(nbParties, nbAttemptsTable);
                printf("Nombre minimum de tentatives : %d\n", smallest_value);
                largest_value = get_largest_value(nbParties, nbAttemptsTable);
                printf("Nombre maximum de tentatives : %d\n", largest_value);
            }
        }
    } while (menu_option != 2);
    
    
    return 0;
}

//# Fonctions

/* Renvoie <vrai> si le code saisi par le joueur est égal au code secret */
int are_codes_equal(int secretCode[], int codeInputByPlayer[]) {
    int areCodesEqual = 1;
    int i = 0;
    while (areCodesEqual == 1 && (i < NB_DIGITS)) {
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
void display_result(int secretCode[], int codeInputByPlayer[]) {
    for (int i = 0; i < NB_DIGITS; i ++) {
        if (is_in_secretCode(secretCode, codeInputByPlayer[i], NB_DIGITS) == 0 ) {
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
int get_smallest_value(int nbParties, int nbAttemptsTable[]) {
    int smallest_value = nbAttemptsTable[0];
    for (int i = 1; i < nbParties; i ++) {
        if (nbAttemptsTable[i] < smallest_value) {
            smallest_value = nbAttemptsTable[i];
        }
    }
    return smallest_value;
}

/* Retourne le plus grand nombre de tentatives */
int get_largest_value(int nbParties, int nbAttemptsTable[]) {
    int largest_value = nbAttemptsTable[0];
    for (int i = 1; i < nbParties; i ++) {
        if (nbAttemptsTable[i] > largest_value) {
            largest_value = nbAttemptsTable[i];
        }
    }
    return largest_value;
}


/* Améliorations
- gestion des saisies
- paramètres des fonctions
- lien vers GitHub
- allocation dynamique
*/