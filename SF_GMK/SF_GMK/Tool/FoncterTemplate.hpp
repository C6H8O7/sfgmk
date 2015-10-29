#pragma once

template<typename InstanceType, typename T, typename... Args>
class FoncterTemplate
{
	private:
		InstanceType* m_Instance;
		T(InstanceType::*m_FuncPtr)(Args);

	public:
		FoncterTemplate(InstanceType* _Instance, T(InstanceType::*_FuncPtr)(Args)) : m_Instance(_Instance), m_FuncPtr(_FuncPtr) 
		{
		}

		~FoncterTemplate() 
		{
		}


		inline T Execute(Args... _Args){ return m_Instance->*m_FuncPtr)(_Args...); }
};



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