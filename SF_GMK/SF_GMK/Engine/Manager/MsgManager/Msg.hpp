/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		20/10/2015
@brief		Fichier d'entête de Msg : contient la classe des données de base de la pompe à messages

--------------------------------------------------------------------------------------------------*/


#ifndef MSG_HPP
#define MSG_HPP
#include <Windows.h>

#define ID_NULL -1

#define MSG_TIME_DELTA GetTickCount()
#define MSG_TIME_TO_LIVE 5 // temps de vie (en s) maximum d'un message. Au delà le message s'autodétruit. NB : les messages hors-circuit de la pompe ne se détruisent pas d'eux mêmes.


// le tag est optionnel et permet d'identifier grossièrement le contenu du message si il est inconnu
namespace sfgmk
{
	namespace engine
	{
		enum MSG_TAG {
			TAG_UNKNOWN = -1,
			TAG_BOOL = 0,
			TAG_CHAR = 1,
			TAG_INT = 2,
			TAG_FLOAT = 3,
			TAG_DOUBLE = 4,
			TAG_TAB = 5,
			TAG_STRUCT = 6
		};


		class SFGMK_API Msg : WARDEN(Msg)
		{
		private:
			MSG_TAG i_Tag; //

			int i_Emettor_ID;
			int i_Receptor_ID;

			void* p_Data; // pointeur sur données

			float f_Delay; // retardement du message en ms
			float f_Time_Lived; // temps depuis la création du message

			bool b_Received; // T = reçu | F = pas reçu
			bool b_Autodestruct; // True : autodestruction après la 1ere lecture
			bool b_IsGroupMsg; // Est un message destiné à un groupe


			int i_Data_Size;

		public:
			Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct); // on transmet la data par pointeur
			Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct, MSG_TAG _Tag);
			Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct, MSG_TAG _Tag, bool b_IsGroupMsg); // pour messages groupes

			Msg(); // tous les paramètres sont nulls
			~Msg();

			template<typename T>
			void Destruct(T) {
				free((T*)p_Data);
			}

			// Fonctions relatives aux 3 différents états d'un message : envoi, transit puis reception
			void OnReception();
			void OnTransition();
			void OnEmission();
			void OnMsgUpdate();

			//Accesseurs / mutateurs 

			MSG_TAG GetTag();
			void SetTag(MSG_TAG);

			int GetEmettorID();
			void SetEmettorID(int);

			int GetReceptorID();
			void SetReceptorID(int);

			void* GetData();
			void SetData(void*);
			void SetDataUnsafe(void*);

			float GetDelay();
			void SetDelay(float);

			float GetLifeTime();
			void SetLifeTime(float);

			bool GetReceived();
			void SetReceived(bool);

			bool GetAutodestruct();
			void SetAutodestruct(bool);

			bool GetIsGroup();
			void SetIsGroup(bool);

			int GetDataSize();
			void SetDataSize(int);
		};

	}
}

#endif
