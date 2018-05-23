/************************************************************************/
/* CopyRight @ 2010 by Anyone. No rights is reserved.
/* @Author: ZUHD
/* @Date: 11/11/2010
/* @Description:   基础事件虚基类
/************************************************************************/
#ifndef IBASEEVENT_H
#define IBASEEVENT_H
namespace leogon
{
	class IBaseEvent
	{
	public:
		virtual ~IBaseEvent(){}
		virtual void Process() = 0;
		virtual void Release() = 0;		
	};
}
#endif
