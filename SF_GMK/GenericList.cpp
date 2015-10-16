namespace sfgmk
{
	GenericList::GenericList() : m_iElementNumber(0), m_LastElement(&m_FirstElement)
	{
		m_FirstElement.pNext = NULL;
		m_FirstElement.pData = NULL;
	}

	GenericList::~GenericList()
	{
	}


	const unsigned int& GenericList::getElementNumber()
	{
		return m_iElementNumber;
	}

	sGenericList* GenericList::getElement(const unsigned int _ElementIndex)
	{
		if( _ElementIndex > m_iElementNumber )
			return NULL;

		sGenericList* Temp = &m_FirstElement;

		for( unsigned int i(0); i <= _ElementIndex; i++ )
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
		//Création nouveau maillon
		sGenericList* NewElement = new sGenericList;
		NewElement->pNext = NULL;
		NewElement->pData = malloc(_ElementSize);
		memcpy(NewElement->pData, _ElementToAdd, _ElementSize);
		
		//Chaîne vide
		if( m_iElementNumber == 0 )
			m_FirstElement.pNext = m_LastElement = NewElement;

		//Insertion en début de chaîne
		else if( _Position == 1 )
		{
			NewElement->pNext = m_FirstElement.pNext;
			m_FirstElement.pNext = NewElement;
		}

		//Insertion en fin de chaîne
		else if( _Position == m_iElementNumber + 1 )
			m_LastElement = m_LastElement->pNext = NewElement;

		//Insertion en milieu de chaîne
		else
		{
			sGenericList* Temp = getElement(_Position - 2);
			NewElement->pNext = Temp->pNext;
			Temp->pNext = NewElement;
		}

		m_iElementNumber++;
	}

	bool GenericList::removeElementAtPosition(const unsigned int _Position)
	{
		//Si chaîne vide, rien à supprimer
		if( m_iElementNumber == 0 )
			return false;

		sGenericList* TempToDelete = NULL;

		//Suppression en début de chaîne
		if( _Position == 1 )
		{
			TempToDelete = m_FirstElement.pNext;
			m_FirstElement.pNext = TempToDelete->pNext;
		}

		//Suppression en fin de chaîne
		else if( _Position == m_iElementNumber )
		{
			TempToDelete = m_LastElement;
			m_LastElement = getElement(m_iElementNumber - 2);
			m_LastElement->pNext = NULL;
		}

		//Suppression en milieu de chaîne
		else
		{
			sGenericList* Temp = getElement(_Position - 2);
			TempToDelete = Temp->pNext;
			Temp->pNext = TempToDelete->pNext;
		}
	
		//free(TempToDelete->pData);
		delete TempToDelete;
		m_iElementNumber--;

		return true;
	}


	void GenericList::addElement(void* _ElementToAdd, const size_t _ElementSize, const unsigned int _Position)
	{
		if( _Position == 0 || _Position > m_iElementNumber + 1 ) //0 = valeur par défaut si rien spécifié lors de l'appel de la fonction, _Position > (m_iElementNumber + 1) veut dire qu'on demande à insérer plus loin que la longueur de la chaîne
			pushBack(_ElementToAdd, _ElementSize);
		else
			addElementAtPosition(_ElementToAdd, _ElementSize, _Position);
	}

	void GenericList::pushBack(void* _ElementToAdd, const size_t _ElementSize)
	{
		addElementAtPosition(_ElementToAdd, _ElementSize, m_iElementNumber + 1);
	}

	void GenericList::pushFront(void* _ElementToAdd, const size_t _ElementSize)
	{
		addElementAtPosition(_ElementToAdd, _ElementSize, 1);
	}


	bool GenericList::removeElement(const unsigned int _Position)
	{
		if( _Position == 0 || _Position > m_iElementNumber )
			return false;
		else if( _Position == 1 )
			return popFront();
		else if( _Position == m_iElementNumber )
			return popBack();
		else
			return removeElementAtPosition(_Position);
	}

	bool GenericList::popBack()
	{
		return removeElementAtPosition(m_iElementNumber);
	}

	bool GenericList::popFront()
	{
		return removeElementAtPosition(1);
	}


	int GenericList::clearList()
	{
		if( m_iElementNumber == 0 )
			return -1;

		int iElementCleared = 0;
		sGenericList *Temp = m_FirstElement.pNext, *TempToDelete = NULL;

		for( unsigned int i(0); i < m_iElementNumber; i++ )
		{
			TempToDelete = Temp;
			Temp = Temp->pNext;
			free(TempToDelete);
			iElementCleared++;
		}

		m_iElementNumber -= iElementCleared;

		return iElementCleared;
	}
}