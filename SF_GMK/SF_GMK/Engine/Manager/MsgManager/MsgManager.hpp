/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		20/10/2015
	@brief		Fichier d'ent�te de MsgManager : contient la classe de la pompe � messages

--------------------------------------------------------------------------------------------------*/

#ifndef MSGMANAGER_HPP
#define MSGMANAGER_HPP

// on pr�f�re un tableau � un vector par commodit� : plus rapide et la pompe � message 
// aura une taille variable en fonction de l'application. Pas besoin d'un vector si 20 ennemis communiquent.
#define ID_BUFFER_SIZE 1024 // <------- Nombre d'inscrits � la poste max
#define MSG_BUFFER_SIZE 25 // <----- Courrier max en circulation 

#define ID_EMPTY -1 // l'emplacement du tableau est vide
#define ID_VIRGIN -2 // l'emplacement du tableau n'a JAMAIS �t� �crit : pas besoin d'aller plus loin

#include "Msg.hpp"
#include "MsgActor.hpp"
#include <iostream>

using namespace std;

namespace sfgmk
{
	namespace engine
	{
		class SFGMK_API MsgManager // ---/!\--- doit encore h�riter d'un SINGLETON------------------------------------
		{
			friend class MsgActor;

		private:

			Msg* tab_Buffer_Msg[MSG_BUFFER_SIZE];
			int tab_Buffer_ID[ID_BUFFER_SIZE];
			MsgActor* tab_Receiver[ID_BUFFER_SIZE];


			void DeliverMsg(Msg* _Message, int _MsgPlace);
			void Clean(int); // dans l'update. D�termine la vie / mort d'un message dans la file
			void SetMsgEmpty(int tab_Num);
			void MoveMessageFromTo(int _From, int _To); // bouge un �l�ment de tab_buffer_msg d'un endroit � un autre (Cf Organize)

			static MsgManager* p_Instance;

		public:
			MsgManager();
			~MsgManager();

			static MsgManager* GetInstance() {
				if (p_Instance == NULL)
					p_Instance = new MsgManager;
				return p_Instance;
			}

			void ShowBufferIDInConsole();

			void ShowBufferMSGInConsole();

			int Register(MsgActor* _rec); // Cr�� un nouvel Identifiant et l'associe � un receveur. Retourne son ID;
			void Withdraw(int _ID); // Retire un �l�ment avec l'ID indiqu�.


			void Organize(); // r�agence la liste des messages pour qu'elle soit plus rapide.

			//void Respond(Msg _Msg_Initial, void* _p_Data); // renvoie msg avec les ID inverses de celui re�u � l'envoyeur (plus rapide � �crire)

			void SendMsg(int _i_ID_Emettor, int _i_ID_Receptor, void* _p_Data, int _DataSize,
				float _f_Delay = 0.0f, bool _b_autodestruct = true); //envoie un message

			void SendTagMsg(int _i_ID_Emettor, int _i_ID_Receptor, void* _p_Data, int _DataSize,
				float _f_Delay = 0.0f, bool _b_autodestruct = true, MSG_TAG _Tag = TAG_UNKNOWN); //envoie un message avec TAG

			void SendMsgToGroup(int _i_ID_Emettor, int _i_ID_Group, void* _p_Data, int _DataSize,
				float _f_Delay = 0.0f, MSG_TAG _Tag = TAG_UNKNOWN, bool _b_autodestruct = true); //envoie un message � un groupe

			bool SendMsgToEntity(unsigned int _EntityId, void* _p_Data, int _DataSize,
								 MSG_TAG _Tag = TAG_UNKNOWN, bool _b_autodestruct = true); //envoie un message � une entit�

			void Update();
		};


	}
}
#endif
