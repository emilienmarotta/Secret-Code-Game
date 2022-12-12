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

#define NB_CHIFFRES 5

//# Prototypes des fonctions

void display_secretCode(int secretCode[]);
int is_in_secretCode(int secretCode[], int value);
void input_code(int codeInputByPlayer[]);
    int check_if_input_is_valid(int codeInput); 


//# Boucle principale

int main(void) {

    int secretCode[NB_CHIFFRES];
    int _randomValue; // Variable temporaire qui permettra de vérifier si la valeur est déjà dans le code (lors de l'initialisation du code)
    int codeInputByPlayer[NB_CHIFFRES];

    srand(time(NULL));

    /* Initialisation des valeurs du code secret avec des chiffres aléatoires compris entre 0 et 9 */
    
    for (int i = 0; i < NB_CHIFFRES; i ++) {
        do {
            _randomValue = rand() % 10;
        } while (is_in_secretCode(secretCode, _randomValue));
        secretCode[i] = _randomValue;
    }

    input_code(codeInputByPlayer);

    display_secretCode(codeInputByPlayer);

    return 0;
}

//# Fonctions

/* Affichage du code secret */
void display_secretCode(int secretCode[]) {
    for (int i = 0; i < NB_CHIFFRES; i ++) {
        printf("%d", secretCode[i]);
        if (i < NB_CHIFFRES - 1) {
            printf(" | ");
        }
    }
}

/* Renvoie <vrai> si la valeur est présente dans le code */
int is_in_secretCode(int secretCode[], int value) {
    int i = 0;
    int found = 0;
    while ((found == 0) && (i < NB_CHIFFRES)) {
        if (value == secretCode[i]) found = 1;
        else i ++;
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


/* Améliorations
- gestion des saisies
*/