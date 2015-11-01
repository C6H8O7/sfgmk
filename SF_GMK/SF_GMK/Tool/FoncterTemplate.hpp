/*--------------------------------------------------------------------------------------------------

	@author		GMK
	@date		01/11/2015
	@brief		Classes permettant de créer un conteneur de fonction générique

--------------------------------------------------------------------------------------------------*/

#ifndef FONCTER_TEMPLATE_HPP
#define FONCTER_TEMPLATE_HPP


namespace sfgmk
{
	class FoncterTemplate
	{
		public:
			FoncterTemplate() {}
			virtual ~FoncterTemplate() {}

			//Déclarer ici les prototypes de fonctions
			virtual inline void Execute() {}
	};

	template<typename T, typename... Args>
	class FoncterFunctionTemplate : public FoncterTemplate
	{
		private:
			T(*m_FuncPtr)(Args...);

		public:
			FoncterFunctionTemplate(T(*_FuncPtr)(Args...)) : m_FuncPtr(_FuncPtr) {}
			~FoncterFunctionTemplate() {}

			inline T Execute(Args... _Args) { return (*m_FuncPtr)(_Args...); }
	};

	template<typename InstanceType, typename T, typename... Args>
	class FoncterMethodTemplate : public FoncterTemplate
	{
		private:
			void* m_Instance;
			T(InstanceType::*m_FuncPtr)(Args...);

		public:
			FoncterMethodTemplate(InstanceType* _Instance, T(InstanceType::*_FuncPtr)(Args...)) : m_Instance(_Instance), m_FuncPtr(_FuncPtr) {}
			~FoncterMethodTemplate() {}

			inline T Execute(Args... _Args) { return (((InstanceType*)(m_Instance))->*m_FuncPtr)(_Args...); }
	};

	class FoncterTemplateArray
	{
		public:
			FoncterTemplateArray() {}
			~FoncterTemplateArray() { m_FunctionsArray.clear(); }

			DynamicArray<FoncterTemplate*> m_FunctionsArray;

			inline void FoncterTemplateArray::operator [] (const unsigned int& _Index) { m_FunctionsArray[_Index]->Execute(); }
	};
}


#endif



/*if( state < 0 ) 
{ 
	if(0) 
	{
		return( true ); 
	} 
} 
else if( a == state ) 
{ 
	if(0) 
	{
		return( true ); 
	} 
	else if( a == event ) 
	{
		OnEvent( EVENT_Enter )
		OnEvent( EVENT_Update )
		OnEvent( EVENT_Exit )

		return(true); 
	} 
}
else 
{
	assert(0); return(false); 
}  
return(false);*/