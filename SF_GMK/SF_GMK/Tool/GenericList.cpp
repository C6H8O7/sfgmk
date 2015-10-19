namespace sfgmk
{
	GenericList::GenericList() : m_uiElementNumber(0U), m_LastElement(&m_FirstElement)
	{
		m_FirstElement.pNext = NULL;
		m_FirstElement.pData = NULL;
	}

	GenericList::~GenericList()
	{
	}


	const unsigned int& GenericList::getElementNumber()
	{
		return m_uiElementNumber;
	}

	sGenericList* GenericList::getElement(const unsigned int _ElementIndex)
	{
		if( _ElementIndex > m_uiElementNumber )
			return NULL;

		sGenericList* Temp = &m_FirstElement;

		for( unsigned int i(0U); i <= _ElementIndex; i++ )
			Temp = Temp->pNext;

		return Temp;
	}

	sGenericList* GenericList::operator[](const unsigned int _ElementIndex)
	{
		return getElement(_ElementIndex);
	}

	void* GenericList::getElementData(const unsigned int _ElementIndex)
	{
		return getElement(_ElementIndex)->pData;
	}


	void GenericList::addElementAtPosition(void* _ElementToAdd, const size_t _ElementSize, const unsigned int _Position)
	{
		//Cr�ation nouveau maillon
		sGenericList* NewElement = new sGenericList;
		NewElement->pNext = NULL;
		NewElement->pData = malloc(_ElementSize);
		memcpy(NewElement->pData, _ElementToAdd, _ElementSize);
		
		//Cha�ne vide
		if( m_uiElementNumber == 0U )
			m_FirstElement.pNext = m_LastElement = NewElement;

		//Insertion en d�but de cha�ne
		else if( _Position == 1U )
		{
			NewElement->pNext = m_FirstElement.pNext;
			m_FirstElement.pNext = NewElement;
		}

		//Insertion en fin de cha�ne
		else if( _Position == m_uiElementNumber + 1U )
			m_LastElement = m_LastElement->pNext = NewElement;

		//Insertion en milieu de cha�ne
		else
		{
			sGenericList* Temp = getElement(_Position - 2U);
			NewElement->pNext = Temp->pNext;
			Temp->pNext = NewElement;
		}

		m_uiElementNumber++;
	}

	bool GenericList::removeElementAtPosition(const unsigned int _Position)
	{
		//Si cha�ne vide, rien � supprimer
		if( m_uiElementNumber == 0U )
			return false;

		sGenericList* TempToDelete = NULL;

		//Suppression en d�but de cha�ne
		if( _Position == 1 )
		{
			TempToDelete = m_FirstElement.pNext;
			m_FirstElement.pNext = TempToDelete->pNext;
		}

		//Suppression en fin de cha�ne
		else if( _Position == m_uiElementNumber )
		{
			TempToDelete = m_LastElement;
			m_LastElement = getElement(m_uiElementNumber - 2U);
			m_LastElement->pNext = NULL;
		}

		//Suppression en milieu de cha�ne
		else
		{
			sGenericList* Temp = getElement(_Position - 2U);
			TempToDelete = Temp->pNext;
			Temp->pNext = TempToDelete->pNext;
		}
	
		//free(TempToDelete->pData);
		delete TempToDelete;
		m_uiElementNumber--;

		return true;
	}


	void GenericList::addElement(void* _ElementToAdd, const size_t _ElementSize, const unsigned int _Position)
	{
		if( _Position == 0U || _Position > m_uiElementNumber + 1U ) //0 = valeur par d�faut si rien sp�cifi� lors de l'appel de la fonction, _Position > (m_iElementNumber + 1) veut dire qu'on demande � ins�rer plus loin que la longueur de la cha�ne
			pushBack(_ElementToAdd, _ElementSize);
		else
			addElementAtPosition(_ElementToAdd, _ElementSize, _Position);
	}

	void GenericList::pushBack(void* _ElementToAdd, const size_t _ElementSize)
	{
		addElementAtPosition(_ElementToAdd, _ElementSize, m_uiElementNumber + 1U);
	}

	void GenericList::pushFront(void* _ElementToAdd, const size_t _ElementSize)
	{
		addElementAtPosition(_ElementToAdd, _ElementSize, 1U);
	}


	bool GenericList::removeElement(const unsigned int _Position)
	{
		if( _Position == 0U || _Position > m_uiElementNumber )
			return false;
		else if( _Position == 1U )
			return popFront();
		else if( _Position == m_uiElementNumber )
			return popBack();
		else
			return removeElementAtPosition(_Position);
	}

	bool GenericList::popBack()
	{
		return removeElementAtPosition(m_uiElementNumber);
	}

	bool GenericList::popFront()
	{
		return removeElementAtPosition(1);
	}


	int GenericList::clearList()
	{
		if( m_uiElementNumber == 0U )
			return -1;

		unsigned int uiElementCleared = 0U;
		sGenericList *Temp = m_FirstElement.pNext, *TempToDelete = NULL;

		for( unsigned int i(0U); i < m_uiElementNumber; i++ )
		{
			TempToDelete = Temp;
			Temp = Temp->pNext;
			free(TempToDelete);
			uiElementCleared++;
		}

		m_uiElementNumber -= uiElementCleared;

		return uiElementCleared;
	}
}