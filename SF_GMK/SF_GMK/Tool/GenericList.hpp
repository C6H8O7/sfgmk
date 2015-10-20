/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		18/06/2015
	@brief		Liste chaînée générique, permettant de stocker des éléments de types différents

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_GENERIC_LIST_HPP
#define SFGMK_GENERIC_LIST_HPP


namespace sfgmk
{
	struct sGenericList
	{
		sGenericList* pNext;
		void* pData;
	};

	class SFGMK_API GenericList
	{
		private:
			sGenericList m_FirstElement;
			sGenericList* m_LastElement;
			unsigned int m_uiElementNumber;

			void addElementAtPosition(void* _ElementToAdd, const size_t _ElementSize, const unsigned int _Position);
			bool removeElementAtPosition(const unsigned int _Position);

		public:
			GenericList();
			~GenericList();

			const unsigned int& getElementNumber();
			sGenericList* getElement(const unsigned int _ElementIndex);
			sGenericList* operator[](const unsigned int _ElementIndex);
			void* getElementData(const unsigned int _ElementIndex);

			void addElement(void* _ElementToAdd, const size_t _ElementSize, const unsigned int _Position = 0);
			void pushBack(void* _ElementToAdd, const size_t _ElementSize);
			void pushFront(void* _ElementToAdd, const size_t _ElementSize);

			bool removeElement(const unsigned int _Position);
			bool popBack();
			bool popFront();

			int clearList();
	};
}


#endif