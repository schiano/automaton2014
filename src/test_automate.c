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

#include <stdlib.h>
#include <stdio.h>

#include "automate.h"
#include "outils.h"

void print_separation_tests()
{
	printf("\n\n===========================================================================================\n");
	printf("===========================================================================================\n");
	printf("===========================================================================================\n\n");
}

int main(){
	Automate* automate = creer_automate();

	// Les états seront ajoutés automatiquement

	ajouter_etat_initial(automate, 1);
	ajouter_transition(automate, 1, 'b', 2);
	ajouter_transition(automate, 1, 'a', 3);
	ajouter_transition(automate, 1, 'a', 2);
	ajouter_transition(automate, 4, 'b', 1);	
	ajouter_transition(automate, 2, 'a', 4);
	ajouter_transition(automate, 3, 'a', 2);
	ajouter_transition(automate, 3, 'b', 3);
	ajouter_transition(automate, 4, 'b', 3);
	ajouter_etat_final(automate, 4);
		

	char mot[] = "babbaa";
	char mot2[] = "aaabb";
	char mot3[] = "ababa";
	char mot4[] = "abbaa";
	char mot5[] = "bbaaa";

	if (le_mot_est_reconnu(automate, mot))
		printf("\n'%s' est reconnu....\n", mot);
	else
		printf("\n'%s' n'est pas reconnu....\n", mot);
	
	if (le_mot_est_reconnu(automate, mot2))
		printf("\n'%s' est reconnu....\n", mot2);
	else
		printf("\n'%s' n'est pas reconnu....\n", mot2);
	
	if (le_mot_est_reconnu(automate, mot3))
		printf("\n'%s' est reconnu....\n", mot3);
	else
		printf("\n'%s' n'est pas reconnu....\n", mot3);
	
	if (le_mot_est_reconnu(automate, mot4))
		printf("\n'%s' est reconnu....\n", mot4);
	else
		printf("\n'%s' n'est pas reconnu....\n", mot4);
	
	if (le_mot_est_reconnu(automate, mot5))
		printf("\n'%s' est reconnu....\n", mot5);
	else
		printf("\n'%s' n'est pas reconnu....\n", mot5);

	liberer_automate(automate);		

	print_separation_tests();

	Automate * automate2 = creer_automate();

	ajouter_etat_initial(automate2, 1);
	ajouter_transition(automate2, 1, 'b', 2);
	ajouter_transition(automate2, 1, 'a', 3);
	ajouter_transition(automate2, 2, 'a', 1);
	ajouter_transition(automate2, 2, 'c', 3);
	ajouter_transition(automate2, 4, 'a', 3);
	ajouter_transition(automate2, 4, 'b', 5);
	ajouter_etat_final(automate2, 3);

	printf("Automate2\n\n");
	print_automate(automate2);
	printf("\n\nEnsemble des états accessibles\n");
	print_ensemble(etats_accessibles(automate2, 1), NULL);

	print_separation_tests();

	automate_accessible(automate2);
	
	liberer_automate(automate2);

	Automate* suffixes = creer_automate_des_prefixes(automate2);
	printf("\n\n\nAutomate des prefixes de Automate2\n");
	print_automate(suffixes);

	Automate* facteur = creer_automate_des_facteurs(automate2);
	printf("\n\n\nAutomate des facteurs de Automate2\n");
	print_automate(facteur);
	
	Automate* surmot = creer_automate_des_sur_mot(automate2, NULL);
	printf("\n\n\nAutomate des sur mots de Automate2\n");
	print_automate(surmot);
	
	Automate* sousmot = creer_automate_des_sous_mots(automate2);
	printf("\n\n\nAutomate des sous mots de Automate2\n");
	print_automate(sousmot);

	liberer_automate(suffixes);
	liberer_automate(prefix);
	liberer_automate(facteur);
	liberer_automate(surmot);
	liberer_automate(sousmot);
	
	print_separation_tests();
	
	// Test mot_automate
	Automate* mot_automate = mot_to_automate("abcde");
	print_automate(mot_automate);

	liberer_automate(mot_automate);

	exit(1);
}
