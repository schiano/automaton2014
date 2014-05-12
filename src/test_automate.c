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

int main(){
	Automate* automate = creer_automate();

	int i;

	for (i = 0; i < 5; i++)
	{
		ajouter_etat(automate, i);
	}

	ajouter_etat_initial(automate, 0);
	ajouter_transition(automate, 0, 'a', 1);
	ajouter_transition(automate, 0, 'b', 0);
	ajouter_transition(automate, 0, 'c', 2);
	ajouter_transition(automate, 1, 'b', 2);
	ajouter_transition(automate, 1, 'c', 1);
	ajouter_transition(automate, 2, 'a', 3);
	ajouter_transition(automate, 3, 'b', 4);
	ajouter_transition(automate, 3, 'c', 1);
	ajouter_transition(automate, 4, 'a', 4);
	ajouter_epsilon_transition(automate, 4, 4);	
	ajouter_etat_final(automate, 4);


	Automate* reverse = miroir(automate);
	
	printf("Automate....\n");
	print_automate(automate);
	printf("Miroir......\n");
	print_automate(reverse);

	exit(1);
}
