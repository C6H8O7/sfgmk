/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		2015
	@brief		Classe de tableau dynamique

--------------------------------------------------------------------------------------------------*/


#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP


namespace sfgmk
{
	#define DYNAMIC_ARRAY_INITIAL_SIZE 32U

	template <typename T>
	class SFGMK_API DynamicArray
	{
		private:
			T* m_Array;
			unsigned int m_uiElementNumber;
			unsigned int m_uiSize;

		public:
			DynamicArray::DynamicArray() : m_Array(NULL), m_uiElementNumber(0U)
			{
				m_uiSize = DYNAMIC_ARRAY_INITIAL_SIZE;
				m_Array = (T*)calloc(1, m_uiSize);
			}

			DynamicArray::~DynamicArray()
			{
				if( m_uiSize > 0 ) 
					resize(0);
			}


			T& DynamicArray<T>::operator [] (const unsigned int& _Index)
			{
				return m_Array[_Index];
			}

			T* getArray()
			{
				return m_Array;
			}

			void DynamicArray::print()
			{
				for( int i(0); i < m_uiElementNumber; i++ )
					std::cout << m_Array[i] << std::endl;

				std::cout << std::endl;
			}


			void DynamicArray::resize(const int& _NewSize)
			{
				m_uiSize = _NewSize;
				m_Array = (T*)realloc(m_Array, m_uiSize);

				if( m_uiSize == 0 )
					m_Array = NULL;
			}

			bool DynamicArray::doubleSize()
			{
				if( !m_uiSize )
					return false;
				else
					m_uiSize = m_uiSize << 1;

				m_Array = (T*)realloc(m_Array, m_uiSize);

				return true;
			}

			const unsigned int& DynamicArray::getElementNumber()
			{
				return m_uiElementNumber;
			}

			const unsigned int& DynamicArray::getSize()
			{
				return m_iSize;
			}


			T* DynamicArray::findElement(const T& _Element)
			{
				for( unsigned int i(0U); i < m_uiElementNumber; i++ )
				{
					if( m_Array[i] == _Element )
						return &m_Array[i];
				}

				return NULL;
			}

			int DynamicArray::findElementIndex(const T& _Element)
			{
				for( unsigned int i(0U); i < m_uiElementNumber; i++ )
				{
					if( m_Array[i] == _Element )
						return i;
				}

				return -1;
			}

			bool DynamicArray::swapIndex(const unsigned int& _Index1, const unsigned int& _Index2)
			{
				if( _Index1 > m_uiElementNumber || _Index2 > m_uiElementNumber )
					return false;

				T Temp = m_Array[_Index1];
				m_Array[_Index1] = m_Array[_Index2];
				m_Array[_Index2] = Temp;

				return true;
			}

			bool DynamicArray::swapElement(const T& _Element1, const T& _Element2)
			{
				int iIndex1 = findElementIndex(_Element1), 
					iIndex2 = findElementIndex(_Element2);

				if( iIndex1 > -1 && iIndex2 > -1 )
				{
					swapIndex(iIndex1, iIndex2);
					return true;
				}

				return false;
			}

			bool DynamicArray::removeElement(const T& _Element)
			{
				int iElementIndex;

				if( (iElementIndex = findElementIndex(_Element)) == -1 )
					return false;

				else if( iElementIndex == 0 )
					popFront();
				else if( iElementIndex == m_uiElementNumber - 1U )
					popBack();
				else
				{
					swapIndex(iElementIndex, m_uiElementNumber - 1U);
					popBack();
				}

				return true;
			}

			void DynamicArray::sort()
			{
				bool bSort(true);

				while( bSort )
				{
					bSort = false;

					for( unsigned int i(0); i < m_uiElementNumber - 1; i++ )
					{
						if( m_Array[i] > m_Array[i + 1] )
						{
							bSort = true;
							T temp = m_Array[i];
							m_Array[i] = m_Array[i + 1];
							m_Array[i + 1] = temp;
						}
					}
				}
			}


			void DynamicArray::pushBack(const T& _Element)
			{
				m_uiElementNumber++;
				if( (m_uiElementNumber * sizeof(T)) > m_uiSize )
					doubleSize();

				m_Array[m_uiElementNumber - 1U] = _Element;
			}

			bool DynamicArray::popBack()
			{
				if( m_uiElementNumber > 0U )
				{
					m_Array[m_uiElementNumber - 1U] = NULL;
					m_uiElementNumber--;
					return true;
				}

				return false;
			}

			void DynamicArray::pushFront(const T& _Element)
			{
				m_iElementNumber++;
				if( (m_uiElementNumber * sizeof(T)) > m_iSize )
					doubleSize();

				for( int i(m_uiElementNumber - 1); i > 0; i-- )
					m_Array[i] = m_Array[i - 1];

				m_Array[0] = _Element;
			}

			bool DynamicArray::popFront()
			{
				if( m_uiElementNumber > 0 )
				{
					for( unsigned int i(0U); i < m_uiElementNumber; i++ )
						m_Array[i] = m_Array[i + 1];

					m_uiElementNumber--;
					return true;
				}

				return false;
			}


			void DynamicArray::clear()
			{
				if( m_uiElementNumber > 0U )
				{
					m_uiElementNumber = 0U;
					resize(DYNAMIC_ARRAY_INITIAL_SIZE);
				}
			}

			void DynamicArray::deleteContent()
			{
				for( unsigned int i(0U); i < m_uiElementNumber; i++ )
				{
					delete m_Array[i];
					m_Array[i] = NULL;
				}

				m_uiElementNumber = 0;
			}
	};
}


#endif