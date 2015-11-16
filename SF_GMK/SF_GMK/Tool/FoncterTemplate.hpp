/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		01/11/2015
	@brief		Classes permettant de créer un conteneur de fonction générique

--------------------------------------------------------------------------------------------------*/

#ifndef FONCTER_TEMPLATE_HPP
#define FONCTER_TEMPLATE_HPP


#define CLASS_NULL FoncterTemplateNullInstance

namespace sfgmk
{
	class FoncterTemplateNullInstance
	{
	};

	class FoncterTemplate
	{
		public:
			FoncterTemplate() {}
			virtual ~FoncterTemplate() {}

			//Déclarer ici les prototypes de fonctions
			virtual inline void Execute() {}
			virtual inline void Execute(const int&) {}
			virtual inline void Execute(const float&) {}
			virtual inline bool Execute(const std::string&) { return false; }
	};

	template<typename InstanceType, typename T, typename... Args>
	class FoncterTemplateInstance : public FoncterTemplate
	{
		public:
			FoncterTemplateInstance(InstanceType* _Instance, T(InstanceType::*_Method)(Args...)) : m_Instance(_Instance), m_MethodPtr(_Method), m_FunctionPtr(NULL) {} //Constructeur methode
			FoncterTemplateInstance(T(*m_Function)(Args...)) : m_Instance(NULL), m_MethodPtr(NULL), m_FunctionPtr(m_Function) {} //Constructeur fonction
			~FoncterTemplateInstance() {}

		private:
			void* m_Instance;
			T(*m_FunctionPtr)(Args...);
			T(InstanceType::*m_MethodPtr)(Args...);

		public:
			inline T Execute(Args... _Args) 
			{ 
				if( m_Instance )
					return (((InstanceType*)(m_Instance))->*m_MethodPtr)(_Args...); 
				else
					return (*m_FunctionPtr)(_Args...);
			}
	};

	class FoncterTemplateArray
	{
		public:
			FoncterTemplateArray() {}
			~FoncterTemplateArray() { m_FunctionsArray.deleteAndClear(); }

			DynamicArray<FoncterTemplate*> m_FunctionsArray;

			inline auto FoncterTemplateArray::operator [] (const unsigned int& _Index) { return m_FunctionsArray[_Index]->Execute(); }
	};
}


#endif