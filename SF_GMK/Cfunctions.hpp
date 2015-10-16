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
		//Retourne la longueur de la cha�ne de caract�res pass�e en param�tre
		const unsigned int mystrlen(const char* _String); 

		//Retourne la longueur du premier nombre contenue dans la cha�ne de caract�res pass�e en param�tre
		const unsigned int mystrnumlen(const char* _String);

		//Retourne un int dont la valeur est le premier nombre contenu dans la cha�ne de caract�res pass�e en param�tre
		unsigned long int myatoi(const char* _String);
		
		//Compare 2 cha�nes de caract�res, renvoi 0 si �quivalente, 2 si longueurs diff�rentes, 1 ou -1 selon que le premier caract�re divergeant de la premi�re cha�ne de caract�res est sup�rieur ou inf�rieur � celui de la seconde cha�ne
		const int mystrcmp(const char* __StringOne, const char* __StringTwo);

		//Copie une cha�ne de caract�res dans une autre, sp�cifier nombre de caract�res � copier et taille du buffer de destination
		char* mystrcpy(const char* _Source, char* _Dest, const size_t _CharacterToCopy, const unsigned int _DestSize);

		//Copie une cha�ne de caract�res � la suite d'une autre
		char* mystrcat(const char* _Source, char* _Dest);

		//Retourne un pointeur vers la premi�re occurence du caract�re pass� en param�tre dans la cha�ne de caract�re sp�cifi�e
		char* mystrchr(const char* _String, int _Character);

		//D�coupe une cha�ne en identifiant les tokens, pass�s en param�tre, dans celle-ci
		char* mystrtok(char* _String, const char* _Tokens);
}


#endif