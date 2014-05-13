/*
 *   Ce fichier fait parti d'un projet de programmation donné en Licence 3 
 *   à l'Université de Bordeaux 1
 *
 *   Copyright (C) 2014 Adrien Boussicault
 *
 *    This Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This Library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this Library.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file automate.c
 * \brief Détail de l'implémentation de Automate
 */

#include "automate.h"
#include "table.h"
#include "ensemble.h"
#include "outils.h"

#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> 

#include <math.h>

struct _Automate {
	Ensemble * vide;
	Ensemble * etats;
	Ensemble * alphabet;
	Table* transitions;
	Ensemble * initiaux;
	Ensemble * finaux;
};

typedef struct _Cle {
	int origine;
	int lettre;
} Cle;

int comparer_cle(const Cle *a, const Cle *b) {
	if( a->origine < b->origine )
		return -1;
	if( a->origine > b->origine )
		return 1;
	if( a->lettre < b->lettre )
		return -1;
	if( a->lettre > b->lettre )
		return 1;
	return 0;
}

void print_cle( const Cle * a){
	printf( "(%d, %c)" , a->origine, (char) (a->lettre) );
}

void supprimer_cle( Cle* cle ){
	xfree( cle );
}

void initialiser_cle( Cle* cle, int origine, char lettre ){
	cle->origine = origine;
	cle->lettre = (int) lettre;
}

Cle * creer_cle( int origine, char lettre ){
	Cle * result = xmalloc( sizeof(Cle) );
	initialiser_cle( result, origine, lettre );
	return result;
}

Cle * copier_cle( const Cle* cle ){
	return creer_cle( cle->origine, cle->lettre );
}

Automate * creer_automate(){
	Automate * automate = xmalloc( sizeof(Automate) );
	automate->etats = creer_ensemble( NULL, NULL, NULL );
	automate->alphabet = creer_ensemble( NULL, NULL, NULL );
	automate->transitions = creer_table(
		( int(*)(const intptr_t, const intptr_t) ) comparer_cle , 
		( intptr_t (*)( const intptr_t ) ) copier_cle,
		( void(*)(intptr_t) ) supprimer_cle
	);
	automate->initiaux = creer_ensemble( NULL, NULL, NULL );
	automate->finaux = creer_ensemble( NULL, NULL, NULL );
	automate->vide = creer_ensemble( NULL, NULL, NULL ); 
	return automate;
}


void liberer_automate( Automate * automate ){
	liberer_ensemble( automate->vide );
	liberer_ensemble( automate->finaux );
	liberer_ensemble( automate->initiaux );
	pour_toute_valeur_table(
		automate->transitions, ( void(*)(intptr_t) ) liberer_ensemble
	);
	liberer_table( automate->transitions );
	liberer_ensemble( automate->alphabet );
	liberer_ensemble( automate->etats );
	xfree(automate);
}

const Ensemble * get_etats( const Automate* automate ){
	return automate->etats;
}

const Ensemble * get_initiaux( const Automate* automate ){
	return automate->initiaux;
}

const Ensemble * get_finaux( const Automate* automate ){
	return automate->finaux;
}

const Ensemble * get_alphabet( const Automate* automate ){
	return automate->alphabet;
}

void ajouter_etat( Automate * automate, int etat ){
	ajouter_element( automate->etats, etat );
}

void ajouter_lettre( Automate * automate, char lettre ){
	ajouter_element(automate->alphabet, lettre);
}
	
void ajouter_transition(
	Automate * automate, int origine, char lettre, int fin
){
	ajouter_etat( automate, origine );
	ajouter_etat( automate, fin );
	ajouter_lettre( automate, lettre );

	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	Ensemble * ens;
	if( iterateur_est_vide( it ) ){
		ens = creer_ensemble( NULL, NULL, NULL );
		add_table( automate->transitions, (intptr_t) &cle, (intptr_t) ens );
	}else{
		ens = (Ensemble*) get_valeur( it );
	}
	ajouter_element( ens, fin );
}

void ajouter_epsilon_transition(Automate * automate, int origine, int fin)
{
	ajouter_transition(automate, origine, '#' ,fin);
}

void ajouter_etat_final(
	Automate * automate, int etat_final
){
	ajouter_element(automate->finaux, etat_final);
}

void ajouter_etat_initial(
	Automate * automate, int etat_initial
){
	ajouter_element(automate->initiaux, etat_initial);
}

const Ensemble * voisins( const Automate* automate, int origine, char lettre ){
	Cle cle;
	initialiser_cle( &cle, origine, lettre );
	Table_iterateur it = trouver_table( automate->transitions, (intptr_t) &cle );
	if( ! iterateur_est_vide( it ) ){
		return (Ensemble*) get_valeur( it );
	}else{
		return automate->vide;
	}
}

Ensemble * delta1(
	const Automate* automate, int origine, char lettre
){
	Ensemble * res = creer_ensemble( NULL, NULL, NULL );
	ajouter_elements( res, voisins( automate, origine, lettre ) );
	return res; 
}

Ensemble * delta(
	const Automate* automate, const Ensemble * etats_courants, char lettre
){
	Ensemble * res = creer_ensemble( NULL, NULL, NULL );

	Ensemble_iterateur it;
	for( 
		it = premier_iterateur_ensemble( etats_courants );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		const Ensemble * fins = voisins(
			automate, get_element( it ), lettre
		);
		ajouter_elements( res, fins );
	}

	return res;
}

Ensemble * delta_star(
	const Automate* automate, const Ensemble * etats_courants, const char* mot
){
	Ensemble * res = creer_ensemble(NULL, NULL, NULL);
	Ensemble * fins = copier_ensemble(etats_courants);
	int i;
	for(i = 0; i < strlen(mot); i++)
	{
		fins = delta(automate, fins, mot[i]);
		ajouter_elements(res, fins);
	}

	return res;
}

void pour_toute_transition(
	const Automate* automate,
	void (* action )( int origine, char lettre, int fin, void* data ),
	void* data
){
	Table_iterateur it1;
	Ensemble_iterateur it2;
	for(
		it1 = premier_iterateur_table( automate->transitions );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		Cle * cle = (Cle*) get_cle( it1 );
		Ensemble * fins = (Ensemble*) get_valeur( it1 );
		for(
			it2 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it2 );
			it2 = iterateur_suivant_ensemble( it2 )
		){
			int fin = get_element( it2 );
			action( cle->origine, cle->lettre, fin, data );
		}
	};
}

Automate* copier_automate( const Automate* automate ){
	Automate * res = creer_automate();
	Ensemble_iterateur it1;
	// On ajoute les états de l'automate
	for(
		it1 = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat( res, get_element( it1 ) );
	}
	// On ajoute les états initiaux
	for(
		it1 = premier_iterateur_ensemble( get_initiaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_initial( res, get_element( it1 ) );
	}
	// On ajoute les états finaux
	for(
		it1 = premier_iterateur_ensemble( get_finaux( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_etat_final( res, get_element( it1 ) );
	}
	// On ajoute les lettres
	for(
		it1 = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		ajouter_lettre( res, (char) get_element( it1 ) );
	}
	// On ajoute les transitions
	Table_iterateur it2;
	for(
		it2 = premier_iterateur_table( automate->transitions );
		! iterateur_ensemble_est_vide( it2 );
		it2 = iterateur_suivant_ensemble( it2 )
	){
		Cle * cle = (Cle*) get_cle( it2 );
		Ensemble * fins = (Ensemble*) get_valeur( it2 );
		for(
			it1 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it1 );
			it1 = iterateur_suivant_ensemble( it1 )
		){
			int fin = get_element( it1 );
			ajouter_transition( res, cle->origine, cle->lettre, fin );
		}
	};
	return res;
}

Automate * translater_etat( const Automate* automate, int n ){
	Automate * res = creer_automate();

	Ensemble_iterateur it;
	for( 
		it = premier_iterateur_ensemble( get_etats( automate ) );
		! iterateur_ensemble_est_vide( it );
		it = iterateur_suivant_ensemble( it )
	){
		ajouter_etat( res, get_element( it ) + n );
	}

	Table_iterateur it1;
	Ensemble_iterateur it2;
	for(
		it1 = premier_iterateur_table( automate->transitions );
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		Cle * cle = (Cle*) get_cle( it1 );
		Ensemble * fins = (Ensemble*) get_valeur( it1 );
		for(
			it2 = premier_iterateur_ensemble( fins );
			! iterateur_ensemble_est_vide( it2 );
			it2 = iterateur_suivant_ensemble( it2 )
		){
			int fin = get_valeur( it2 );
			ajouter_transition(
				res, cle->origine + n, cle->lettre, fin + n
			);
		}
	};
	return res;
}


void action_get_max_etat( const intptr_t element, void* data ){
	int * max = (int*) data;
	if( *max < element ) *max = element;
}

int get_max_etat( const Automate* automate ){
	int max = INT_MIN;
	
	pour_tout_element( automate->etats, action_get_max_etat, &max );

	return max;
}


void action_get_min_etat( const intptr_t element, void* data ){
	int * min = (int*) data;
	if( *min > element ) *min = element;
}

int get_min_etat( const Automate* automate ){
	int min = INT_MAX;

	pour_tout_element( automate->etats, action_get_min_etat, &min );

	return min;
}

/**
 * \par Implémentation
 * La fonction ne fait qu'ajouter des transitions à un automate vide. </br>
 * \par
 * Pour chaque lettre du mot, on part de l'état <em>[position de la lettre]+1</em>, on 
 * passe par la lettre sélectionnée et on fini dans l'état 
 * <em>[position de la lettre]+2</em>
 * @param  mot
 * @return
 */
Automate * mot_to_automate( const char * mot ){
	Automate* res = creer_automate();
	int length = sizeof(mot) / sizeof(char);
	int i;
	for (i = 0; i < length; ++i)
	{
		ajouter_transition(res, i+1, mot[i], i+2);
	}
	return res;
}


Automate * creer_automate_des_sur_mots(
	const Automate* automate, Ensemble * alphabet
){
	A_FAIRE_RETURN(NULL);
}


Ensemble* etats_accessibles( const Automate * automate, int etat ){
	Ensemble * res = creer_ensemble(NULL, NULL, NULL);
	Ensemble * etape = creer_ensemble(NULL, NULL, NULL);	
	ajouter_element(etape, etat);
	Ensemble_iterateur it_lettre;
	Ensemble_iterateur it_etat;

	// Tant que des états sont à traiter
	while (taille_ensemble(etape) > 0)
	{
		Ensemble * trouves = creer_ensemble(NULL, NULL, NULL);
		for(
			it_lettre = premier_iterateur_ensemble( get_alphabet( automate ) );
		! iterateur_ensemble_est_vide( it_lettre );
		it_lettre = iterateur_suivant_ensemble( it_lettre )
		){
			for(
				it_etat = premier_iterateur_ensemble(etape);
			! iterateur_ensemble_est_vide( it_etat );
			it_etat = iterateur_suivant_ensemble( it_etat )
			){
				// on ajoute les voisins aux éléments trouvés
				ajouter_elements(trouves, voisins(automate, 
									get_element(it_etat), 
									get_element(it_lettre)
									));				
			}
		}
		etape = creer_difference_ensemble(trouves, res);
		ajouter_elements(res, etape);
	}	

	return res;
}

Automate *automate_accessible( const Automate * automate){
	A_FAIRE_RETURN(NULL);
}

void reverse_transition(int origine, char lettre, int fin, void* automate)
{
	ajouter_transition(automate, fin, lettre, origine);
}

Automate *miroir( const Automate * automate){
	Automate* clone = creer_automate();
	ajouter_elements(clone->initiaux, automate->finaux);
	ajouter_elements(clone->finaux, automate->initiaux);

	pour_toute_transition(automate, reverse_transition, clone);
	return clone;
}

Automate *automate_co_accessible( const Automate * automate){
	A_FAIRE_RETURN(NULL);
}

Automate * creer_automate_des_prefixes( const Automate* automate ){
	Automate * prefixe = copier_automate(automate);
	Ensemble * finaux = prefixe->finaux;
	Ensemble_iterateur it1, it2;
	int etat_actuel, est_ok;
	
	est_ok = 0;
	
	for (it1 = premier_iterateur_ensemble(get_etats(prefixe)); ! iterateur_ensemble_est_vide(it1); it1 = iterateur_suivant_ensemble(it1)){
			
		etat_actuel = get_element(it1);
		for (it2 = premier_iterateur_ensemble(finaux); !iterateur_ensemble_est_vide(it2) && !est_ok; it2 = iterateur_suivant_ensemble(it2)){
			
			if (est_dans_l_ensemble(etats_accessibles(prefixe, etat_actuel), get_element(it2))) {

				est_ok = 1;
				ajouter_etat_final(prefixe, etat_actuel);
			}
		}
		
		est_ok = 0;
	}
	
	return prefixe;
}

Automate * creer_automate_des_suffixes( const Automate* automate ){
	Automate * suffixe = copier_automate(automate);
	Ensemble * finaux = suffixe->finaux;
	Ensemble_iterateur it1, it2;
	int etat_actuel, est_ok;
	
	est_ok = 0;
	
	for (it1 = premier_iterateur_ensemble(get_etats(suffixe)); ! iterateur_ensemble_est_vide(it1); it1 = iterateur_suivant_ensemble(it1)){
			
		etat_actuel = get_element(it1);
		for (it2 = premier_iterateur_ensemble(finaux); !iterateur_ensemble_est_vide(it2) && !est_ok; it2 = iterateur_suivant_ensemble(it2)){
			
			if (est_dans_l_ensemble(etats_accessibles(suffixe, etat_actuel), get_element(it2))) {

				est_ok = 1;
				ajouter_etat_initial(suffixe, etat_actuel);
			}
		}
		
		est_ok = 0;
	}
	
	return suffixe;
}

Automate * creer_automate_des_facteurs( const Automate* automate ){
	Automate * facteur = copier_automate(automate);
	Ensemble * finaux = facteur->finaux;
	Ensemble_iterateur it1, it2;
	int etat_actuel, est_ok;
	
	est_ok = 0;
	
	for (it1 = premier_iterateur_ensemble(get_etats(facteur)); ! iterateur_ensemble_est_vide(it1); it1 = iterateur_suivant_ensemble(it1)){
			
		etat_actuel = get_element(it1);
		for (it2 = premier_iterateur_ensemble(finaux); !iterateur_ensemble_est_vide(it2) && !est_ok; it2 = iterateur_suivant_ensemble(it2)){
			
			if (est_dans_l_ensemble(etats_accessibles(facteur, etat_actuel), get_element(it2))) {

				est_ok = 1;
				ajouter_etat_initial(facteur, etat_actuel);
				ajouter_etat_final(facteur, etat_actuel);
			}
		}
		
		est_ok = 0;
	}
	
	return facteur;
}

Automate * creer_automate_des_sur_mot(
	const Automate* automate, Ensemble * alphabet
){
	A_FAIRE_RETURN(NULL);
}

Automate * creer_automate_de_concatenation(
	const Automate* automate1, const Automate* automate2
){
	A_FAIRE_RETURN(NULL);
}

Automate * creer_automate_des_sous_mots( const Automate* automate ){
	A_FAIRE_RETURN(NULL);
}

Automate * creer_automate_du_melange(
	const Automate* automate1,  const Automate* automate2
){
	A_FAIRE_RETURN(NULL);
}

int est_une_transition_de_l_automate(
	const Automate* automate,
	int origine, char lettre, int fin
){
	return est_dans_l_ensemble( voisins( automate, origine, lettre ), fin );
}

int est_un_etat_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble( get_etats( automate ), etat );
}

int est_un_etat_initial_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble(get_initiaux(automate), etat);
}

int est_un_etat_final_de_l_automate( const Automate* automate, int etat ){
	return est_dans_l_ensemble(get_finaux(automate), etat);
}

int est_une_lettre_de_l_automate( const Automate* automate, char lettre ){
	return est_dans_l_ensemble(get_alphabet(automate), lettre);
}

void print_ensemble_2( const intptr_t ens ){
	print_ensemble( (Ensemble*) ens, NULL );
}

void print_lettre( intptr_t c ){
	printf("%c", (char) c );
}

void print_automate( const Automate * automate ){
	printf("- Etats : ");
	print_ensemble( get_etats( automate ), NULL );
	printf("\n- Initiaux : ");
	print_ensemble( get_initiaux( automate ), NULL );
	printf("\n- Finaux : ");
	print_ensemble( get_finaux( automate ), NULL );
	printf("\n- Alphabet : ");
	print_ensemble( get_alphabet( automate ), print_lettre );
	printf("\n- Transitions : ");
	print_table( 
		automate->transitions,
		( void (*)( const intptr_t ) ) print_cle, 
		( void (*)( const intptr_t ) ) print_ensemble_2,
		""
	);
	printf("\n");
}

int le_mot_est_reconnu( const Automate* automate, const char* mot ){
	Ensemble * fins = copier_ensemble(get_initiaux(automate));
	int i;
	//printf("\n\n[Mot est reconnu]\n");
	for(i = 0; i < strlen(mot); i++)
	{
		// printf("\n[Pre delta] Appel avec fins = ");
		// print_ensemble(fins, NULL);
		// printf(" et mot[i] = '%c'\n", mot[i]);
		fins = delta(automate, fins, mot[i]);		
	}

	Ensemble_iterateur it1;
	// printf("\nPrint ensemble fin\n");
	// print_ensemble(fins, NULL);
	for(
		it1 = premier_iterateur_ensemble(fins);
		! iterateur_ensemble_est_vide( it1 );
		it1 = iterateur_suivant_ensemble( it1 )
	){
		if (est_un_etat_final_de_l_automate(automate, get_element(it1)))
		return 1;		
	}

	return 0;
}
