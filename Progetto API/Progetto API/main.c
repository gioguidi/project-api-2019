//
//  main.c
//  Progetto API
//
//  Created by Giovanni Guidi on 27/07/2019.
//  Copyright © 2019 Giovanni Guidi. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

# define N 512


int n_max_entita = N;


typedef char miastringa[64];


miastringa* testa_nomi_entita = NULL;
char nome_entita[64];

struct relazione_lista {
    char nome_entita[64];
    struct relazione_lista *prossimo_elemento;
};

struct relazione {
    char nome_entita[64];
    int numero_occorrenze;
    struct relazione *prossimo_elemento;
    struct relazione_lista *prossimo_elemento_ricevente;
};

struct relazioni_trovate {
    char nome_relazione[64];
    struct relazione *testa_relazione_attuale;
    struct relazioni_trovate *prossima_relazione_trovata;
};

struct relazioni_trovate *testa_relazioni_trovate = NULL;



void aggiungi_entita (char entita[64]);
int verifica_presenza_entita (char entita[64]);
void aggiungi_relazione (char relazione[64], char entita1[64], char entita2[64]);
void aggiungi_elementi_relazione (char entita1[64], char entita2[64], struct relazioni_trovate *relazione_attuale);
void elimina_relazione (char relazione[64], char entita1[64], char entita2[64]);
void elimina_entita (char entita[64]);

int verifica_presenza_entita (char entita[64]) {
    
    for (int i=0; i<n_max_entita; i++) {
        
        if (strcmp(testa_nomi_entita[i], entita) == 0) //caso entità già presente
            return 1;
    }
    
    return 0;
}

void aggiungi_entita (char entita[64]) {
    
    if (verifica_presenza_entita(entita)) {
        return;
    }
    
    for (int i=0; i<n_max_entita; i++) {

        if (strcmp(testa_nomi_entita[i], "") == 0) { //caso entità non presente e spazio nel vettore
            strcpy(testa_nomi_entita[i], entita);
            return;
        }
    }
    
    
    n_max_entita = n_max_entita * 2; //caso entità non presente ma vettore pieno
    testa_nomi_entita = realloc(testa_nomi_entita, n_max_entita*sizeof(nome_entita));
    
    memset(testa_nomi_entita[n_max_entita/2], 0, sizeof(miastringa)*(n_max_entita/2));
    
    strcpy(testa_nomi_entita[n_max_entita/2], entita);
    return;
}





void aggiungi_relazione (char relazione[64], char entita1[64], char entita2[64]) {
    
    if (!verifica_presenza_entita(entita1) || !verifica_presenza_entita(entita2)) {
        return;
    }
    
    if (testa_relazioni_trovate == NULL) { //inserimento lista vuota
        testa_relazioni_trovate = malloc(sizeof(struct relazioni_trovate));
        strcpy(testa_relazioni_trovate->nome_relazione, relazione);
        testa_relazioni_trovate->prossima_relazione_trovata = NULL;

        testa_relazioni_trovate->testa_relazione_attuale = NULL;
        aggiungi_elementi_relazione(entita1, entita2, testa_relazioni_trovate);
    } else { //inserimento in lista relazioni non vuota
        struct relazioni_trovate *temp, *prev = NULL;
        temp = testa_relazioni_trovate;
        
        while (temp != NULL) { //caso relazione già presente, la presenza anche della rel tra la entita va controllata in aggiungi_elementi
            if (strcmp(temp->nome_relazione, relazione) == 0) {
                aggiungi_elementi_relazione(entita1, entita2, temp);
                return;
            }
            
            temp = temp -> prossima_relazione_trovata;
        }
       
        temp = testa_relazioni_trovate; //caso relazione non presente
        
        while (temp != NULL && strcmp(temp->nome_relazione, relazione) < 0) { //vado al giusto posto nell'ordine
            prev = temp;
            temp = temp->prossima_relazione_trovata;
        }
        
        if (temp == testa_relazioni_trovate) { //inserimento in testa
            testa_relazioni_trovate = malloc(sizeof(struct relazioni_trovate));
            strcpy(testa_relazioni_trovate->nome_relazione, relazione);
            testa_relazioni_trovate->prossima_relazione_trovata = temp;
            testa_relazioni_trovate->testa_relazione_attuale = NULL;

            aggiungi_elementi_relazione(entita1, entita2, testa_relazioni_trovate);
        } else { //inserimento non in testa
            prev->prossima_relazione_trovata = malloc(sizeof(struct relazioni_trovate));
            strcpy(prev->prossima_relazione_trovata->nome_relazione, relazione);
            prev->prossima_relazione_trovata->prossima_relazione_trovata = temp;
            prev->prossima_relazione_trovata->testa_relazione_attuale = NULL;
           
            aggiungi_elementi_relazione(entita1, entita2, prev->prossima_relazione_trovata);
        }
        
    }
}

void aggiungi_elementi_relazione (char entita1[64], char entita2[64], struct relazioni_trovate *relazione_attuale) {
    
    bool flag_entita_1 = 0;
    bool flag_entita_2 = 0;
    
    if (relazione_attuale->testa_relazione_attuale == NULL) { //caso struttura dati vuota
        
        relazione_attuale->testa_relazione_attuale = malloc(sizeof(struct relazione));
        
        if (strcmp(entita1, entita2) < 0) {
            strcpy(relazione_attuale->testa_relazione_attuale->nome_entita, entita1);
            relazione_attuale->testa_relazione_attuale->numero_occorrenze = 0;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente = malloc(sizeof(struct relazione_lista));
            strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->nome_entita, entita2);
            relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->prossimo_elemento = NULL;
            
            relazione_attuale->testa_relazione_attuale->prossimo_elemento = malloc(sizeof(struct relazione));
            strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento->nome_entita, entita2);
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->numero_occorrenze = 1;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento = NULL;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento_ricevente = NULL;
            
        } else if  (strcmp(entita1, entita2) > 0) {
            strcpy(relazione_attuale->testa_relazione_attuale->nome_entita, entita2);
            relazione_attuale->testa_relazione_attuale->numero_occorrenze = 1;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente = NULL;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento = malloc(sizeof(struct relazione));
            
            strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento->nome_entita, entita1);
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->numero_occorrenze = 0;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento = NULL;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento_ricevente = malloc(sizeof(struct relazione_lista));
            strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento_ricevente->nome_entita, entita2);
            relazione_attuale->testa_relazione_attuale->prossimo_elemento->prossimo_elemento_ricevente->prossimo_elemento = NULL;
            
        } else { //caso uguali
            strcpy(relazione_attuale->testa_relazione_attuale->nome_entita, entita1);
            relazione_attuale->testa_relazione_attuale->numero_occorrenze = 1;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento = NULL;
            relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente = malloc(sizeof(struct relazione_lista));
            strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->nome_entita, entita2);
            relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->prossimo_elemento = NULL;
        }
        
        return;
        
    } else {
        struct relazione *temp, *prev = NULL, *rel_entita_1 = NULL, *rel_entita_2 = NULL;
        temp = relazione_attuale->testa_relazione_attuale;
        
        while (temp != NULL) {
            
            if (strcmp(temp->nome_entita, entita1) == 0) {
                rel_entita_1 = temp;
                flag_entita_1 = 1;
                if (flag_entita_1 && flag_entita_2) {
                    break;
                }
            }
            
            if (strcmp(temp->nome_entita, entita2) == 0) {
                rel_entita_2 = temp;
                flag_entita_2 = 1;
                if (flag_entita_1 && flag_entita_2) {
                    break;
                }
            }
            
            temp = temp->prossimo_elemento;
        }
        
        bool flag_gia_presente = 0;
        
        if (flag_entita_1) {
            struct relazione_lista *relazione_iterate, *rel_iterate_prev = NULL;
            relazione_iterate = rel_entita_1->prossimo_elemento_ricevente;
            
            while (relazione_iterate != NULL) {
                if (strcmp(relazione_iterate->nome_entita, entita2) == 0) {
                    flag_gia_presente = 1;
                    break;
                }
                relazione_iterate = relazione_iterate->prossimo_elemento;
            }
            
            if (!flag_gia_presente) {
                struct relazione_lista *nuovo_el;
                nuovo_el = malloc(sizeof(struct relazione_lista));
                strcpy(nuovo_el->nome_entita, entita2);
                
                
                relazione_iterate = rel_entita_1->prossimo_elemento_ricevente;
                while (relazione_iterate != NULL && strcmp(relazione_iterate->nome_entita, entita2) < 0) {
                    rel_iterate_prev = relazione_iterate;
                    relazione_iterate = relazione_iterate->prossimo_elemento;
                }
                
                if (relazione_iterate == rel_entita_1->prossimo_elemento_ricevente) { //inserimento in testa
                    nuovo_el->prossimo_elemento=rel_entita_1->prossimo_elemento_ricevente;
                    rel_entita_1->prossimo_elemento_ricevente = nuovo_el;
                } else {
                    rel_iterate_prev->prossimo_elemento = nuovo_el;
                    nuovo_el->prossimo_elemento = relazione_iterate;
                }
                
            }
            
            if (!flag_gia_presente && strcmp(entita1, entita2) == 0) {
                rel_entita_1->numero_occorrenze++;
                return;
            }
            
        }
        
        if (flag_entita_2) {
            if (!flag_gia_presente) {
                rel_entita_2->numero_occorrenze++;
            }
        }
        
        if (!flag_entita_1) {
            temp = relazione_attuale->testa_relazione_attuale;
            
            while (temp != NULL && strcmp(temp->nome_entita, entita1) < 0) {
                prev = temp;
                temp = temp->prossimo_elemento;
            }
            
            if (temp == relazione_attuale->testa_relazione_attuale) { //inserimento in testa
                relazione_attuale->testa_relazione_attuale = malloc(sizeof(struct relazione));
                strcpy(relazione_attuale->testa_relazione_attuale->nome_entita, entita1);
                relazione_attuale->testa_relazione_attuale->numero_occorrenze = 0;
                relazione_attuale->testa_relazione_attuale->prossimo_elemento=temp;
                relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente = malloc(sizeof(struct relazione_lista));
                strcpy(relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->nome_entita, entita2);
                relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente->prossimo_elemento = NULL;
                
                if (strcmp(entita1, entita2) == 0) {
                    relazione_attuale->testa_relazione_attuale->numero_occorrenze = 1;
                    return;
                }
                
            } else { //inserimento non in testa
                prev->prossimo_elemento = malloc(sizeof(struct relazione));
                strcpy(prev->prossimo_elemento->nome_entita, entita1);
                prev->prossimo_elemento->numero_occorrenze = 0;
                prev->prossimo_elemento->prossimo_elemento = temp;
                prev->prossimo_elemento->prossimo_elemento_ricevente = malloc(sizeof(struct relazione_lista));
                strcpy(prev->prossimo_elemento->prossimo_elemento_ricevente->nome_entita, entita2);
                prev->prossimo_elemento->prossimo_elemento_ricevente->prossimo_elemento = NULL;
                
                if (strcmp(entita1, entita2) == 0) {
                    prev->prossimo_elemento->numero_occorrenze = 1;
                    return;
                }
            }
        }
        
        if (!flag_entita_2) {
            temp = relazione_attuale->testa_relazione_attuale;
            
            while (temp != NULL && strcmp(temp->nome_entita, entita2) < 0) {
                prev = temp;
                temp = temp->prossimo_elemento;
            }
            
             if (temp == relazione_attuale->testa_relazione_attuale) {
                 relazione_attuale->testa_relazione_attuale = malloc(sizeof(struct relazione));
                 strcpy(relazione_attuale->testa_relazione_attuale->nome_entita, entita2);
                 relazione_attuale->testa_relazione_attuale->numero_occorrenze = 1;
                 relazione_attuale->testa_relazione_attuale->prossimo_elemento = temp;
                 relazione_attuale->testa_relazione_attuale->prossimo_elemento_ricevente = NULL;
             } else {
                prev->prossimo_elemento = malloc(sizeof(struct relazione));
                strcpy(prev->prossimo_elemento->nome_entita, entita2);
                prev->prossimo_elemento->numero_occorrenze = 1;
                prev->prossimo_elemento->prossimo_elemento = temp;
                prev->prossimo_elemento->prossimo_elemento_ricevente = NULL;
             }
        }
    }
}


void elimina_relazione (char relazione[64], char entita1[64], char entita2[64]) {
    
    if (!verifica_presenza_entita(entita1) || !verifica_presenza_entita(entita2)) {
        return;
    }
    
    bool el_trovato = 0;
    
    struct relazioni_trovate *temp;
    temp = testa_relazioni_trovate;
    
    while (temp != NULL) {
        if (strcmp(temp->nome_relazione, relazione) == 0) {
            break;
        }
        
        temp = temp->prossima_relazione_trovata;
    }
    
    if (temp == NULL) {
        return;
    } else {
        struct relazione *temp_relazione, *temp_relazione_riduci_occorrenze;
        temp_relazione = temp->testa_relazione_attuale;
        
        while (temp_relazione != NULL) {
            if (strcmp(temp_relazione->nome_entita, entita1) == 0) {
                struct relazione_lista *temp_rel_lista, *prev_rel_lista = NULL;
                temp_rel_lista = temp_relazione->prossimo_elemento_ricevente;
                
                while (temp_rel_lista != NULL) {
                    if (strcmp(temp_rel_lista->nome_entita, entita2) == 0) {
                        el_trovato = 1;
                        
                        if (temp_rel_lista == temp_relazione->prossimo_elemento_ricevente) { //caso primo elemento in lista
                            temp_relazione->prossimo_elemento_ricevente = temp_rel_lista->prossimo_elemento;
                            free(temp_rel_lista);
                            break;
                        } else { //caso non primo el lista
                            prev_rel_lista->prossimo_elemento = temp_rel_lista->prossimo_elemento;
                            free(temp_rel_lista);
                            break;
                        }
                    }
                    prev_rel_lista = temp_rel_lista;
                    temp_rel_lista = temp_rel_lista->prossimo_elemento;
                }
                
                if (el_trovato) {
                    temp_relazione_riduci_occorrenze = temp->testa_relazione_attuale;
                    while (temp_relazione_riduci_occorrenze != NULL) {
                        if (strcmp(temp_relazione_riduci_occorrenze->nome_entita, entita2) == 0) {
                            temp_relazione_riduci_occorrenze->numero_occorrenze--;
                            break;
                        }
                        
                        temp_relazione_riduci_occorrenze = temp_relazione_riduci_occorrenze->prossimo_elemento;
                    }
                }
                
                break;
            }
            
            temp_relazione = temp_relazione->prossimo_elemento;
        }
    }
    
}


void elimina_entita (char entita[64]) {

    
    bool flag_trovato = 0;
    int indice_entita = 0;
    
    for (int i=0; i<n_max_entita; i++) {
        
        if (strcmp(testa_nomi_entita[i], entita) == 0) { //caso entità già presente
            flag_trovato = 1;
            indice_entita = i;
            break;
        }
    }
    
    if (!flag_trovato) {
        return;
    } else {
    
        memset(testa_nomi_entita[indice_entita], 0, sizeof(miastringa));
        
        
        struct relazioni_trovate *temp_rel;
        struct relazione *temp_relazione_attuale, *prec_rel_attuale = NULL, *temp_rel_occorrenze;
        struct relazione_lista *temp_rel_lista, *next_rel_lista, *prec_rel_lista = NULL;
        
        temp_rel = testa_relazioni_trovate;
        while (temp_rel != NULL) {
            temp_relazione_attuale = temp_rel->testa_relazione_attuale;
            
            bool nuova_testa = false;
            
            while (temp_relazione_attuale != NULL) {
                
                
                
                if (strcmp(temp_relazione_attuale->nome_entita, entita) == 0) { //caso entita ha rel uscenti
                    
    
                    temp_rel_lista = temp_relazione_attuale->prossimo_elemento_ricevente;
                    temp_rel_occorrenze = temp_rel->testa_relazione_attuale;
                    
                    while (temp_rel_lista != NULL)
                    {
                        next_rel_lista = temp_rel_lista->prossimo_elemento;
                        
                        while (strcmp(temp_rel_occorrenze->nome_entita, temp_rel_lista->nome_entita) != 0) {
                            temp_rel_occorrenze = temp_rel_occorrenze->prossimo_elemento; //se non è = controllo prossimo
                        }
                        
                        temp_rel_occorrenze->numero_occorrenze--;
                        
                        free(temp_rel_lista);
                        temp_rel_lista = next_rel_lista;
                    }
                    
                    if (temp_relazione_attuale == temp_rel->testa_relazione_attuale) { //caso primo elemento
                        temp_rel->testa_relazione_attuale = temp_relazione_attuale->prossimo_elemento;
                        free(temp_relazione_attuale);
                        temp_relazione_attuale = temp_rel->testa_relazione_attuale;
                        nuova_testa = 1;
        
                    } else { //altri casi
                        prec_rel_attuale->prossimo_elemento = temp_relazione_attuale->prossimo_elemento;
                        free(temp_relazione_attuale);
                        temp_relazione_attuale = prec_rel_attuale;
                    }
                } else { //caso entita è nella lista delle rel entranti
                    temp_rel_lista = temp_relazione_attuale->prossimo_elemento_ricevente;
                    
                    while (temp_rel_lista != NULL) {
        
                        if (strcmp(temp_rel_lista->nome_entita, entita) == 0) {
                            if (temp_rel_lista == temp_relazione_attuale->prossimo_elemento_ricevente) { //caso primo elemento
                                temp_relazione_attuale->prossimo_elemento_ricevente = temp_rel_lista->prossimo_elemento;
                                free(temp_rel_lista);
                                break;
                            } else { //altro caso
                                prec_rel_lista->prossimo_elemento = temp_rel_lista->prossimo_elemento;
                                free(temp_rel_lista);
                                break;
                            }
                        }
                        
                            prec_rel_lista = temp_rel_lista;
                            temp_rel_lista = temp_rel_lista->prossimo_elemento;
                    }
                }
                
                if (!nuova_testa) {
                    prec_rel_attuale = temp_relazione_attuale;
                    temp_relazione_attuale = temp_relazione_attuale->prossimo_elemento;
                }
                
                if (nuova_testa) {
                    nuova_testa = 0;
                }
            }
            
            temp_rel = temp_rel->prossima_relazione_trovata;
        }
    }
}




void report() {
    
    if (testa_relazioni_trovate == NULL) {
        printf("none\n");
        return;
    }
    
    bool flag = false;
    bool almeno_1 = false;
    
    struct relazioni_trovate *temp_rel_trovate;
    struct relazione *temp_rel;
    temp_rel_trovate = testa_relazioni_trovate;
    
    while (temp_rel_trovate != NULL) {
        int occ_max = 0;
        temp_rel = temp_rel_trovate->testa_relazione_attuale;
        
        while (temp_rel != NULL) {
            if (temp_rel->numero_occorrenze > occ_max) {
                occ_max = temp_rel->numero_occorrenze;
            }
            temp_rel = temp_rel->prossimo_elemento;
        }
        
        if (occ_max != 0) {
            
            if (!almeno_1) {
                almeno_1 = true;
            }
            
            temp_rel = temp_rel_trovate->testa_relazione_attuale;
            
            if (flag) {
                printf(" %s ", temp_rel_trovate->nome_relazione);
            } else {
                printf("%s ", temp_rel_trovate->nome_relazione);
                flag = 1;
            }
            
            while (temp_rel != NULL) {
                if (temp_rel->numero_occorrenze == occ_max) {
                    printf("%s ", temp_rel->nome_entita);
                }
                
                temp_rel = temp_rel->prossimo_elemento;
            }
            
            printf("%d;", occ_max);
        }
        
        temp_rel_trovate = temp_rel_trovate->prossima_relazione_trovata;
    }
    
    if (almeno_1) {
        printf("\n");
    }
    
}
 


int main(int argc, const char * argv[]) {

    char entita_letta[64] = {0};
    char relazione_letta[64] = {0};
    char entita_relazione_1[64] = {0};
    char entita_relazione_2[64] = {0};
    char comando[64] = {0};
    
    
    bool fine = 0;
    
    testa_nomi_entita = malloc(N * sizeof(nome_entita));
    memset(testa_nomi_entita, 0, N * sizeof(nome_entita));
    
    while (!fine) {
        scanf("%s", comando);
        
        fflush(stdin);
        
        if (strcmp(comando, "addent") == 0) {
            scanf("%s", entita_letta);
            aggiungi_entita(entita_letta);
        }
        
        if (strcmp(comando, "delent") == 0) {
            scanf("%s", entita_letta);
            elimina_entita(entita_letta);
        }
        
        if (strcmp(comando, "addrel") == 0) {
            scanf("%s %s %s", entita_relazione_1, entita_relazione_2, relazione_letta);
            aggiungi_relazione(relazione_letta, entita_relazione_1, entita_relazione_2);
        }
        
        if (strcmp(comando, "delrel") == 0) {
            scanf("%s %s %s", entita_relazione_1, entita_relazione_2, relazione_letta);
            elimina_relazione(relazione_letta, entita_relazione_1, entita_relazione_2);
        }
        
        if (strcmp(comando, "report") == 0) {
            report();
        }
        
        
        if (strcmp(comando, "end") == 0) {
            fine = 1;
        }
        
        fflush(stdin);

    }
    
    return 0;
   
}
