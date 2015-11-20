/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		20/10/2015
@brief		Fichier : contient la definition de la pompe. 

--------------------------------------------------------------------------------------------------*/
#include "MsgManager.hpp"
namespace sfgmk
{
	namespace engine
	{
		MsgManager* MsgManager::p_Instance = NULL;

		MsgManager::MsgManager() {
			for (int i = 0; i < ID_BUFFER_SIZE; ++i) {
				tab_Buffer_ID[i] = ID_VIRGIN;
				tab_Receiver[i] = NULL;
			}

			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				SetMsgEmpty(i);
			}
		}

		MsgManager::~MsgManager() {

		}

		void MsgManager::Update() {
			// lancement de l'Update des Messages
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				if (tab_Buffer_Msg[i] != NULL) {
					if (tab_Buffer_Msg[i]->GetEmettorID() != ID_EMPTY && tab_Buffer_Msg[i]->GetEmettorID() != ID_VIRGIN) {
						tab_Buffer_Msg[i]->OnTransition();
						if (tab_Buffer_Msg[i]->GetDelay() <= 0.0f)
						{
							DeliverMsg(tab_Buffer_Msg[i], i);
							SetMsgEmpty(i);
						}
					}
					if (tab_Buffer_Msg[i] != NULL)
						if (tab_Buffer_Msg[i]->GetEmettorID() == ID_VIRGIN) {
							i = MSG_BUFFER_SIZE;
						}
					Clean(i);
				}
			}
		}

		void MsgManager::DeliverMsg(Msg* _Message, int _MsgPlace) {
			for (int i = 0; i < ID_BUFFER_SIZE; ++i) {
				if (_Message->GetIsGroup() == false) { // si msg individuel
					if (tab_Buffer_ID[i] == _Message->GetReceptorID()) {
						tab_Receiver[i]->AddnewMessageToVector(_Message);

					}
				}
				if (_Message->GetIsGroup() == true) {// si message multiple
					if (tab_Receiver[i] != NULL)
						if (tab_Receiver[i]->IsInGroup(_Message->GetReceptorID())) {
							// on créé une copie pour chaque destinataire (car les messages sont consommés à la reception)
							Msg* _Copy = new Msg;
							*_Copy = *_Message;
							void* _dataCpy = malloc((_Message->GetDataSize()));
							memcpy(_dataCpy, _Message->GetData(), (_Message->GetDataSize()));
							_Copy->SetDataUnsafe(_dataCpy);
							tab_Receiver[i]->AddnewMessageToVector(_Copy);
						}
				}

			}

			if (_Message->GetIsGroup()) {
				delete _Message;
				SetMsgEmpty(_MsgPlace);
			}
		}

		void MsgManager::Clean(int i) {
			if (tab_Buffer_Msg[i] != NULL)
				if (tab_Buffer_Msg[i]->GetAutodestruct() == true && tab_Buffer_Msg[i]->GetReceived() == true) {
					// clean des autodestructs	
					SetMsgEmpty(i);
				}
			if (tab_Buffer_Msg[i] != NULL)
				if (tab_Buffer_Msg[i]->GetDelay() <= -5) {
					// si un message "traine" (erreur lors d'une destruction d'Actor ou autre?) on le détruit
					SetMsgEmpty(i);
				}
			if (tab_Buffer_Msg[i] != NULL)
				if (tab_Receiver[tab_Buffer_Msg[i]->GetReceptorID()] == NULL) // si le recepteur n'existe pas/plus
				{
					SetMsgEmpty(i);
				}

		}

		void MsgManager::SetMsgEmpty(int i) {
			tab_Buffer_Msg[i] = NULL;
		}

		void MsgManager::ShowBufferIDInConsole() {
			for (int i = 0; i < ID_BUFFER_SIZE; ++i) {
				printf("tab_Buffer_ID[%d] : %d\n", i, tab_Buffer_ID[i]);
			}
		}

		void MsgManager::ShowBufferMSGInConsole() {
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				cout << "Adresse " << i << " : ";
				if (tab_Buffer_Msg[i] != NULL) {
					cout << "ID emettor : " << tab_Buffer_Msg[i]->GetEmettorID()
						<< " ID receptor : " << tab_Buffer_Msg[i]->GetReceptorID()
						<< " Delay : " << tab_Buffer_Msg[i]->GetDelay()
						<< " received : " << tab_Buffer_Msg[i]->GetReceived() << endl;
				}
				else
					cout << "NULL" << endl;
			}

		}

		int MsgManager::Register(MsgActor* _rec) {
			int _newID = 0;
			int _lastEmpty = -2;
			bool _IsRegistered = false;
			for (int i = 0; i < ID_BUFFER_SIZE; ++i) {

				if (_newID == tab_Buffer_ID[i]) {
					_newID++;
					i = 0;
				}
				if (tab_Buffer_ID[i] == ID_EMPTY) {
					_lastEmpty = i;
				}

				if (tab_Buffer_ID[i] == ID_VIRGIN) {
					if (_lastEmpty != -2) {
						_IsRegistered = true;
						tab_Buffer_ID[_lastEmpty] = _newID;
						tab_Receiver[_lastEmpty] = _rec;
						i = ID_BUFFER_SIZE;
					}
					else {
						_IsRegistered = true;
						tab_Buffer_ID[i] = _newID;
						tab_Receiver[i] = _rec;
						i = ID_BUFFER_SIZE;
					}
				}
				if (i == ID_BUFFER_SIZE - 1 && _IsRegistered == false && _lastEmpty != -2) {
					tab_Buffer_ID[_lastEmpty] = _newID;
					tab_Receiver[_lastEmpty] = _rec;
					_IsRegistered = true;
				}
			}
			if (_IsRegistered == false) {
				cerr << "Erreur : tableau de la pompe a messages PLEIN. Augmenter taille MSG_BUFFER_SIZE" << endl;
			}

			_rec->SetID(_newID);
			return _newID;
		}

		void MsgManager::Withdraw(int _ID) {
			for (int i = 0; i < ID_BUFFER_SIZE; ++i) {
				if (tab_Buffer_ID[i] == _ID)
				{
					tab_Buffer_ID[i] = ID_EMPTY;
					tab_Receiver[i] = nullptr;
				}
			}

		}

		//void MsgManager::Respond(Msg _Msg_Initial, void* _p_Data) {
		//
		//}

		void MsgManager::SendMsg(int _i_ID_Emettor, int _i_ID_Receptor, void* _p_Data, int _DataSize,
			float _f_Delay, bool _b_autodestruct) {
			// Recherche d'emplacement libre dans p_buffer_msg
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				if (tab_Buffer_Msg[i] == NULL) // si c'est vide youpi on remplit
				{
					tab_Buffer_Msg[i] = new Msg(_i_ID_Emettor, _i_ID_Receptor, _p_Data, _DataSize, _f_Delay, _b_autodestruct);
					tab_Buffer_Msg[i]->OnEmission(); // process du message sur Emission
					i = MSG_BUFFER_SIZE;
				}
				if (i == MSG_BUFFER_SIZE - 1)
					cout << "Error - Plus assez de place dans tab_Buffer_Msg de MsgManager.hpp" << endl;
			}

		}

		void MsgManager::SendTagMsg(int _i_ID_Emettor, int _i_ID_Receptor, void* _p_Data, int _DataSize,
			float _f_Delay, bool _b_autodestruct, MSG_TAG _TAG) {
			// Recherche d'emplacement libre dans p_buffer_msg
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				if (tab_Buffer_Msg[i] == NULL) // si c'est vide youpi on remplit
				{
					tab_Buffer_Msg[i] = new Msg(_i_ID_Emettor, _i_ID_Receptor, _p_Data, _DataSize, _f_Delay, _b_autodestruct, _TAG);
					tab_Buffer_Msg[i]->OnEmission(); // process du message sur Emission
					i = MSG_BUFFER_SIZE;
				}
			}

		}

		void MsgManager::SendMsgToGroup(int _i_ID_Emettor, int _i_ID_Group, void* _p_Data, int _DataSize,
			float _f_Delay, MSG_TAG _Tag, bool _b_autodestruct) {
			// Recherche d'emplacement libre dans p_buffer_msg
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				if (tab_Buffer_Msg[i] == NULL) // si c'est vide youpi on remplit
				{
					tab_Buffer_Msg[i] = new Msg(_i_ID_Emettor, _i_ID_Group, _p_Data, _DataSize, _f_Delay, _b_autodestruct, _Tag, true);
					tab_Buffer_Msg[i]->OnEmission(); // process du message sur Emission
					i = MSG_BUFFER_SIZE;
				}
				if (i == MSG_BUFFER_SIZE - 1)
					cout << "Error - Plus assez de place dans tab_Buffer_Msg de MsgManager.hpp" << endl;
			}

		}

		bool MsgManager::SendMsgToEntity(unsigned int _EntityId, void* _p_Data, int _DataSize,
							 MSG_TAG _Tag, bool _b_autodestruct)
		{
			Entity* Target = NULL;

			if( (Target = ENTITY_MANAGER->getEntity(_EntityId)) )
			{
				Msg* Message = new Msg(0, 0, _p_Data, _DataSize, 0.0f, _b_autodestruct, _Tag, true);
				Target->getMsgActor().AddnewMessageToVector(Message);
				return true;
			}

			return false;
		}

		void MsgManager::MoveMessageFromTo(int _From, int _To) {
			Msg* _Mem = tab_Buffer_Msg[_From];
			tab_Buffer_Msg[_From] = tab_Buffer_Msg[_To];
			tab_Buffer_Msg[_To] = _Mem;
		}

		void MsgManager::Organize() {
			int _FirstEmpty = -1;
			for (int i = 0; i < MSG_BUFFER_SIZE; ++i) {
				if (tab_Buffer_Msg[i] == NULL && _FirstEmpty == -1) {
					_FirstEmpty = i;
				}
				if (_FirstEmpty != -1 && tab_Buffer_Msg[i] != NULL) {
					if (_FirstEmpty != i)
						MoveMessageFromTo(i, _FirstEmpty);
				}
			}
		}
	}
}