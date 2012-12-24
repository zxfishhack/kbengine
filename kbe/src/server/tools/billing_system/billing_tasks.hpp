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

#ifndef __BILLING_TASKS_H__
#define __BILLING_TASKS_H__

// common include	
// #define NDEBUG
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "thread/threadtask.hpp"
#include "helper/debug_helper.hpp"
#include "network/address.hpp"

namespace KBEngine{ 

class Orders;

class BillingTask : public thread::TPTask
{
public:
	BillingTask();
	virtual ~BillingTask();
	
	virtual bool process() = 0;
	
	virtual thread::TPTask::TPTaskState presentMainThread();
	
	virtual const char* serviceAddr() const = 0;
	virtual uint16 servicePort() const = 0;

	std::string commitName;			// �ύʱ�õ�����
	std::string accountName;		// ����Ϸ���������ݿ�����account�󶨵�����
	std::string password;			// ����
	std::string postDatas;			// �ύ�ĸ�������
	std::string getDatas;			// ���ظ��ͻ��˵ĸ�������
	COMPONENT_ID baseappID;
	COMPONENT_ID dbmgrID;
	bool success;

	Mercury::Address address;
};

class CreateAccountTask : public BillingTask
{
public:
	CreateAccountTask();
	virtual ~CreateAccountTask();
	
	virtual bool process();

	thread::TPTask::TPTaskState presentMainThread();

	virtual const char* serviceAddr() const{ return g_kbeSrvConfig.billingSystemThirdpartyAccountServiceAddr(); }
	virtual uint16 servicePort() const{ return g_kbeSrvConfig.billingSystemThirdpartyAccountServicePort(); }
protected:
};

class LoginAccountTask : public CreateAccountTask
{
public:
	LoginAccountTask();
	virtual ~LoginAccountTask();
	
	thread::TPTask::TPTaskState presentMainThread();
protected:
};

class ChargeTask : public BillingTask
{
public:
	ChargeTask();
	virtual ~ChargeTask();
	
	virtual bool process();
	thread::TPTask::TPTaskState presentMainThread();

	virtual const char* serviceAddr() const{ return g_kbeSrvConfig.billingSystemThirdpartyChargeServiceAddr(); }
	virtual uint16 servicePort() const{ return g_kbeSrvConfig.billingSystemThirdpartyChargeServicePort(); }

	Orders* pOrders;
};


}
#endif