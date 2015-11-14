/*--------------------------------------------------------------------------------------------------

@author		GMK
@date		14/11/2015
@brief		Classe gardien du programme

--------------------------------------------------------------------------------------------------*/

#ifndef SFGMK_WARDEN_HPP
#define SFGMK_WARDEN_HPP

#define SFGMK_WARDEN

#ifdef SFGMK_WARDEN
	#define WARDEN(A) public sfgmk::Warden<A>
#else
	#define WARDEN(A) public sfgmk::WardenDummy
#endif

namespace sfgmk
{
	class SFGMK_API WardenReg : public SingletonTemplate<WardenReg>
	{
	friend class SingletonTemplate<WardenReg>;

	public:

		struct WardenRegEntry
		{
			std::string name;
			unsigned int count;
		};

		DynamicArray<WardenRegEntry*>* getEntriesPtr()
		{
			return &m_entries;
		}

	private:

		WardenReg() {}
		~WardenReg() {}

		DynamicArray<WardenRegEntry*> m_entries;
	};

	class WardenDummy
	{

	};

	template<typename T>
	class Warden
	{
	public:

		Warden()
		{
			if (!s_entry_reg)
			{
				s_entry_reg = true;

				std::string className(typeid(*this).name());

				size_t s = className.find_first_of('<') + 1;
				size_t e = className.find_last_of('>');

				s_entry.name = className.substr(s, e - s);

				s_entry.count = 0;

				WardenReg::getSingleton()->getEntriesPtr()->pushBack(&s_entry);
			}

			s_entry.count++;
		}

		virtual ~Warden()
		{
			s_entry.count--;
		}

	private:

		static bool s_entry_reg;
		static WardenReg::WardenRegEntry s_entry;
	};

	template<typename T>
	bool Warden<T>::s_entry_reg = false;

	template<typename T>
	WardenReg::WardenRegEntry Warden<T>::s_entry;
}

#endif