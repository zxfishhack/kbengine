/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KBE_FORWARD_MESSAGEBUFFER_H
#define KBE_FORWARD_MESSAGEBUFFER_H

#include "common/common.h"
#include "common/tasks.h"
#include "common/singleton.h"
#include "helper/debug_helper.h"
#include "server/components.h"
#include "network/bundle.h"

namespace KBEngine { 
namespace Network
{
class Bundle;
class NetworkInterface;
class EventDispatcher;
}

/*
	如果在app上没有找到任何cellapp或者baseapp这个模块将一些消息缓存起来， 
	等待有新的cellapp或者baseapp加入则开始将指令转发。
*/


/*
	当一个消息被成功转寄则调用这个handler处理剩余的事情
	需要重写process
*/
class ForwardMessageOverHandler
{
public:
	virtual void process() = 0;
	virtual ~ForwardMessageOverHandler(){}
};

struct ForwardItem
{
	Network::Bundle* pBundle;
	ForwardMessageOverHandler* pHandler;
};

/*
	转发缓存消息到指定组件上
*/
class ForwardComponent_MessageBuffer : public Task, 
						public Singleton<ForwardComponent_MessageBuffer>
{
public:
	ForwardComponent_MessageBuffer(Network::NetworkInterface & networkInterface);
	virtual ~ForwardComponent_MessageBuffer();

	Network::EventDispatcher & dispatcher();

	void push(COMPONENT_ID componentID, ForwardItem* pHandler);
	
	bool process();
private:
	Network::NetworkInterface & networkInterface_;
	bool start_;
	
	typedef std::vector<ForwardItem*> MSGMAP_ITEM;
	typedef std::map<COMPONENT_ID, MSGMAP_ITEM> MSGMAP;
	MSGMAP pMap_;

};

/*
	转发缓存消息到同类型任意组件上
*/
class ForwardAnywhere_MessageBuffer : public Task, 
						public Singleton<ForwardAnywhere_MessageBuffer>
{
public:
	ForwardAnywhere_MessageBuffer(Network::NetworkInterface & networkInterface, COMPONENT_TYPE forwardComponentType);
	virtual ~ForwardAnywhere_MessageBuffer();

	Network::EventDispatcher & dispatcher();

	void push(ForwardItem* pHandler);
	
	bool process();
private:
	Network::NetworkInterface & networkInterface_;
	COMPONENT_TYPE forwardComponentType_;
	bool start_;
	
	std::vector<ForwardItem*> pBundles_;

};

}

#endif // KBE_FORWARD_MESSAGEBUFFER_H
