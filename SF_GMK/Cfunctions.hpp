/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		05/05/2015
	@brief		Fonctions C faites maison

--------------------------------------------------------------------------------------------------*/

#ifndef C_FUNCTION_HPP
#define C_FUNCTION_HPP


namespace sfgmk
{
	//Strings
		//Retourne la longueur de la chaîne de caractères passée en paramètre
		const unsigned int mystrlen(const char* _String); 

		//Retourne la longueur du premier nombre contenue dans la chaîne de caractères passée en paramètre
		const unsigned int mystrnumlen(const char* _String);

		//Retourne un int dont la valeur est le premier nombre contenu dans la chaîne de caractères passée en paramètre
		unsigned long int myatoi(const char* _String);
		
		//Compare 2 chaînes de caractères, renvoi 0 si équivalente, 2 si longueurs différentes, 1 ou -1 selon que le premier caractère divergeant de la première chaîne de caractères est supérieur ou inférieur à celui de la seconde chaîne
		const int mystrcmp(const char* __StringOne, const char* __StringTwo);

		//Copie une chaîne de caractères dans une autre, spécifier nombre de caractères à copier et taille du buffer de destination
		char* mystrcpy(const char* _Source, char* _Dest, const size_t _CharacterToCopy, const unsigned int _DestSize);

		//Copie une chaîne de caractères à la suite d'une autre
		char* mystrcat(const char* _Source, char* _Dest);

		//Retourne un pointeur vers la première occurence du caractère passé en paramètre dans la chaîne de caractère spécifiée
		char* mystrchr(const char* _String, int _Character);

		//Découpe une chaîne en identifiant les tokens, passés en paramètre, dans celle-ci
		char* mystrtok(char* _String, const char* _Tokens);
}


#endif