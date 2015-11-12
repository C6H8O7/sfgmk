/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		20/10/2015
@brief		Fichier : contient la definition de la classe msg

--------------------------------------------------------------------------------------------------*/
#include "MsgActor.hpp"
namespace sfgmk
{
	namespace engine
	{
		// Fonctions Private ---------------------------------------------

		void MsgActor::OpenMsg(Msg* _msg) {
			_msg->OnReception();
		}

		// Fonctions Public ----------------------------------------------

		MsgActor::MsgActor()
		{
			MESSAGE_MANAGER->Register(this);
		}

		Msg* MsgActor::GetLastMessage() {
			CleanVecMsg();

			if (vec_Msg.size() >= 1 && vec_Msg[0] != NULL)
			{
				Msg* p_Msg;
				p_Msg = vec_Msg[0];
				OpenMsg(p_Msg);
				return p_Msg;
			}
			return 0;
		}

		void MsgActor::CleanVecMsg() {
			for ( size_t i = 0; i < vec_Msg.size(); ++i) {
				if (vec_Msg[i]->GetReceived() == true) {
					delete vec_Msg[i];
					vec_Msg[i] = NULL;
					if (vec_Msg.size() == 1)
						vec_Msg.pop_back();
					else {
						vec_Msg[i] = vec_Msg[vec_Msg.size() - 1];
						vec_Msg.pop_back();
						//i = 0;
					}

				}
			}

		}

		void* MsgActor::GetLastMessageData() {

			CleanVecMsg();

			if (vec_Msg.size() >= 1 && vec_Msg[0] != NULL)
			{
				Msg* p_Msg;
				p_Msg = vec_Msg[0];
				OpenMsg(p_Msg);
				return p_Msg->GetData();
			}
			//else std::cout << "Pas de messages recus" << std::endl;
			return 0;

		}

		void MsgActor::AddnewMessageToVector(Msg* _msg) {
			vec_Msg.push_back(_msg);
		}

		void MsgActor::AddGroup(int _ID_Group) {
			bool b_CanBeAdded = true;
			for ( size_t i = 0; i < vec_groups.size(); ++i) {
				if (vec_groups[i] == _ID_Group)
					b_CanBeAdded = false;
			}

			if (b_CanBeAdded) {
				vec_groups.push_back(_ID_Group);
			}
			else {
				std::cout << "Erreur : l'ID a ajouter est deja present sur cet actor" << std::endl;
			}
		}

		void MsgActor::Withdrawgroup(int _ID_Group) {

			int _place = -1;

			for ( size_t i = 0; i < vec_groups.size(); ++i) {
				if (vec_groups[i] == _ID_Group)
					_place = i;
			}

			if (_place != -1) {
				vec_groups[_place] = vec_groups[vec_groups.size() - 1];
				vec_groups.pop_back();
			}
			else
				std::cout << "Erreur : le groupe a supprimer n'a pas ete trouve" << std::endl;
		}

		std::vector<int> MsgActor::GetGroups() {
			return vec_groups;
		}

		void MsgActor::ShowGroupsInConsole() {
			std::cout << "Groupes de l'actor : ";
			for ( size_t i = 0; i < vec_groups.size(); ++i) {
				std::cout << vec_groups[i] << ", ";
			}
			std::cout << std::endl;
		}

		bool MsgActor::IsInGroup(int _ID_Group) {
			for ( size_t i = 0; i < vec_groups.size(); ++i) {
				if (vec_groups[i] == _ID_Group)
					return true;
			}
			return false;
		}

		int MsgActor::GetID() {
			return i_ID;
		}

		void MsgActor::SetID(int _newID) {
			i_ID = _newID;
		}
		// Debug

		int MsgActor::GetNumberOfMessages() {
			return  (vec_Msg.size());
		}

	}
}