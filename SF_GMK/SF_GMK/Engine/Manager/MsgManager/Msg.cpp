/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		20/10/2015
@brief		Fichier : contient la definition de la classe msg

--------------------------------------------------------------------------------------------------*/
#include "Msg.hpp"

Msg::Msg():
i_Emettor_ID(ID_NULL), i_Receptor_ID(ID_NULL), f_Delay(0.0f),
b_Received(false), b_Autodestruct(true), b_IsGroupMsg(false)
{
	p_Data = NULL;
}

Msg::~Msg() {
	if(p_Data != NULL)
		delete p_Data;
}

Msg::Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct) :
	i_Emettor_ID(_EmettorID), i_Receptor_ID(_ReceptorID), i_Data_Size(_DataSize), f_Delay(_Delay),
	b_Received(false), b_Autodestruct(_Autodestruct), i_Tag(TAG_UNKNOWN), b_IsGroupMsg(false)
{
	p_Data = _Data;
}

Msg::Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct, MSG_TAG _Tag) :
	i_Emettor_ID(_EmettorID), i_Receptor_ID(_ReceptorID), i_Data_Size(_DataSize), f_Delay(_Delay),
	b_Received(false), b_Autodestruct(_Autodestruct), i_Tag(_Tag), b_IsGroupMsg(false)
{
	p_Data = _Data;
}
Msg::Msg(int _EmettorID, int _ReceptorID, void* _Data, int _DataSize, float _Delay, bool _Autodestruct, MSG_TAG _Tag, bool _group) :
	i_Emettor_ID(_EmettorID), i_Receptor_ID(_ReceptorID), i_Data_Size (_DataSize), f_Delay(_Delay),
	b_Received(false), b_Autodestruct(_Autodestruct), i_Tag(_Tag), b_IsGroupMsg(true)
{
	p_Data = _Data;
}

// Fonctions D'Etats -------------------------------------------------------------------

void Msg::OnReception(){
	b_Received = true;
}

void Msg::OnTransition() {
	OnMsgUpdate();
}

void Msg::OnEmission() {
	OnMsgUpdate();
}

void Msg::OnMsgUpdate() {
		f_Delay -= 1;
}
// Accesseurs/Mutateurs-----------------------------------------------------------------
void* Msg::GetData() {
	return p_Data;
}
void Msg::SetData(void* _data) {
	if (p_Data != nullptr)
		delete (p_Data);

	p_Data = _data;
}
void Msg::SetDataUnsafe(void* _data) {
	p_Data = _data;
}

MSG_TAG Msg::GetTag() {
	return i_Tag;
}

void Msg::SetTag(MSG_TAG _Tag) {
	i_Tag = _Tag;
}

int Msg::GetEmettorID() {
	return i_Emettor_ID;
}

void Msg::SetEmettorID(int _newID){
	i_Emettor_ID = _newID;
}

int Msg::GetReceptorID(){
	return i_Receptor_ID;
}

void Msg::SetReceptorID(int _newID) {
	i_Receptor_ID = _newID;
}

float Msg::GetDelay(){
	return f_Delay;
}

void Msg::SetDelay(float _Delay) {
	f_Delay = _Delay;
}

bool Msg::GetReceived() {
	return b_Received;
}

void Msg::SetReceived(bool _Received) {
	b_Received = _Received;
}

bool Msg::GetAutodestruct() {
	return b_Autodestruct;
}

void Msg::SetAutodestruct(bool _Autodestruct) {
	b_Autodestruct = _Autodestruct;
}

bool Msg::GetIsGroup() {
	return b_IsGroupMsg;
}

void Msg::SetIsGroup(bool _b) {
	b_IsGroupMsg = _b;
}

int Msg::GetDataSize() {
	return i_Data_Size;
}

void Msg::SetDataSize(int i_NewData) {
	i_Data_Size = i_NewData;
}