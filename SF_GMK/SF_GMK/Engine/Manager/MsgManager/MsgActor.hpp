/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		20/10/2015
@brief		Fichier d'entête de MsgActor : inclure cette classe à une autre pour qu'elle soit inscrite
										   sur la pompe à messages

--------------------------------------------------------------------------------------------------*/
#ifndef MSGACTOR_HPP
#define MSGACTOR_HPP

#include "Msg.hpp"
#include <vector>
#include <iostream>
namespace sfgmk
{
	namespace engine
	{
		enum MSG_GROUPS {
			ALPHA,
			BETA,
			GAMMA
		};

		class SFGMK_API MsgActor {
		private:
			std::vector<Msg*> vec_Msg;
			// un vector pour éviter de gaspiller de la mémoire sur un grand nombre d'entités avec un tableau fixe
			std::vector<int> vec_groups;
			// vector qui recense les groupes auxquels appartient l'actor

			void OpenMsg(Msg* _msg);

			int i_ID;

		public:
			MsgActor();
			~MsgActor();

			Msg* GetLastMessage();

			void* GetLastMessageData();
			void AddnewMessageToVector(Msg* _msg); // fonction utilisée par MsgManager

			void AddGroup(int _ID_Group);
			void Withdrawgroup(int _ID_Group);
			std::vector<int> GetGroups();
			void ShowGroupsInConsole();
			bool IsInGroup(int _ID_Group);

			void CleanVecMsg(); // désalloue la mémoire puis retire le pointeur du vector

			int GetID();
			void SetID(int);

			//---Debug

			int GetNumberOfMessages();

		};


	}
}
#endif