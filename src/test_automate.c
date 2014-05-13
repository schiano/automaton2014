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

	print_automate(automate);

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

	print_automate(automate);
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

	Automate* suffixes = creer_automate_des_suffixes(automate2);
	printf("\n\n\nAutomate des suffixes de Automate2\n");
	print_automate(suffixes);
	
	Automate* prefix = creer_automate_des_prefixes(automate2);
	printf("\n\n\nAutomate des prefixes de Automate2\n");
	print_automate(prefix);

	Automate* facteur = creer_automate_des_facteurs(automate2);
	printf("\n\n\nAutomate des facteurs de Automate2\n");
	print_automate(facteur);
	
	Automate* surmot = creer_automate_des_sur_mot(automate2, NULL);
	printf("\n\n\nAutomate des sur mots de Automate2\n");
	print_automate(surmot);
	
	Automate* sousmot = creer_automate_des_sous_mots(automate2);
	printf("\n\n\nAutomate des sous mots de Automate2\n");
	print_automate(sousmot);
	
	print_separation_tests();
	
	// Test mot_automate
	printf("\nAutomate du mot abcde\n");
	Automate* mot_automate = mot_to_automate("abcde");
	print_automate(mot_automate);
	print_separation_tests();
	
	// Test creer_automate_du_melange
	printf("\nAutomate du mélange de l'automate qui reconnait aa, et de l'automate qui reconnait bb\n");
	Automate* aaa = creer_automate();
	ajouter_etat_initial(aaa, 0);
	ajouter_transition(aaa, 0, 'a', 1);
	ajouter_transition(aaa, 1, 'a', 2);
	ajouter_etat_final(aaa, 2);
	
	Automate* bbb = creer_automate();
	ajouter_etat_initial(bbb, 0);
	ajouter_transition(bbb, 0, 'b', 1);
	ajouter_transition(bbb, 1, 'b', 2);
	ajouter_etat_final(bbb, 2);
	
	
	Automate* melange = creer_automate_du_melange(aaa, bbb);
	print_automate(melange);

	liberer_automate(mot_automate);
	liberer_automate(suffixes);
	liberer_automate(prefix);
	liberer_automate(facteur);
	liberer_automate(surmot);
	liberer_automate(sousmot);
	liberer_automate(aaa);
	liberer_automate(bbb);
	liberer_automate(melange);

	// Test creer_automate_de_concatenation
	print_separation_tests();
	Automate* concatenable1 = creer_automate();
	Automate* concatenable2 = creer_automate();

	printf("Automate à concaténer 2 fois :\n");

	ajouter_etat_initial(concatenable1, 1);
	ajouter_etat_initial(concatenable1, 2);
	ajouter_transition(concatenable1, 1, 'a', 3);
	ajouter_transition(concatenable1, 2, 'b', 3);
	ajouter_transition(concatenable1, 2, 'c', 4);
	ajouter_transition(concatenable1, 3, 'd', 5);
	ajouter_transition(concatenable1, 4, 'e', 6);
	ajouter_etat_final(concatenable1, 5);
	ajouter_etat_final(concatenable1, 6);

	print_automate(concatenable1);

	ajouter_etat_initial(concatenable2, 1);
	ajouter_etat_initial(concatenable2, 2);
	ajouter_transition(concatenable2, 1, 'a', 3);
	ajouter_transition(concatenable2, 2, 'b', 3);
	ajouter_transition(concatenable2, 2, 'c', 4);
	ajouter_transition(concatenable2, 3, 'd', 5);
	ajouter_transition(concatenable2, 4, 'e', 6);
	ajouter_etat_final(concatenable2, 5);
	ajouter_etat_final(concatenable2, 6);

	Automate* concatenation = creer_automate_de_concatenation(concatenable1, concatenable2);

	printf("Automate concaténé\n");
	print_automate(concatenation);
	exit(1);
}
