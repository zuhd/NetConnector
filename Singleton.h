/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   单体类的实现
/************************************************************************/
#ifndef SINGLETON_H
#define SINGLETON_H
/********************************************************************
example:

@code
class player : public CSingleton<player>
{
void display();
};
main()
{
player::GetInstance()->display();
}
@endcode

*********************************************************************/
namespace leogon
{
	template <typename T> class CSingleton
	{
	protected:

		static T* ms_Singleton;

	public:
		CSingleton( void )
		{
			assert( !ms_Singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
			// 静态成员的地址偏移
			int offset = (int)(T*)1 - (int)(CSingleton <T>*)(T*)1;
			ms_Singleton = (T*)((int)this + offset);
#else
			ms_Singleton = static_cast< T* >( this );
#endif
		}
		~CSingleton( void )
		{  assert( ms_Singleton );  ms_Singleton = 0;  }
		static T& GetInstance( void )
		{	assert( ms_Singleton );  return ( *ms_Singleton ); }
		static T* GetInstancePtr()
		{
			assert(ms_Singleton); return ms_Singleton;
		}

	};
}
#endif
