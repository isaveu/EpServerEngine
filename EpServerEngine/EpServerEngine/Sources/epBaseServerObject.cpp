/*! 
BaseServerObject for the EpServerEngine
Copyright (C) 2012  Woong Gyu La <juhgiyo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "epBaseServerObject.h"
#include "epServerObjectList.h"
using namespace epse;

BaseServerObject::BaseServerObject(unsigned int waitTimeMilliSec,epl::LockPolicy lockPolicyType):epl::SmartObject(lockPolicyType),epl::Thread(EP_THREAD_PRIORITY_NORMAL,lockPolicyType)
{
	m_waitTime=waitTimeMilliSec;
	m_syncPolicy=SYNC_POLICY_ASYNCHRONOUS;
	m_lockPolicy=lockPolicyType;
	m_container=NULL;
	switch(lockPolicyType)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_containerLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_containerLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_containerLock=EP_NEW epl::NoLock();
		break;
	default:
		m_containerLock=NULL;
		break;
	}
}

BaseServerObject::BaseServerObject(const BaseServerObject& b):SmartObject(b),Thread(b)
{
	m_waitTime=b.m_waitTime;
	m_syncPolicy=b.m_syncPolicy;
	m_container=b.m_container;
	m_lockPolicy=b.m_lockPolicy;
	switch(m_lockPolicy)
	{
	case epl::LOCK_POLICY_CRITICALSECTION:
		m_containerLock=EP_NEW epl::CriticalSectionEx();
		break;
	case epl::LOCK_POLICY_MUTEX:
		m_containerLock=EP_NEW epl::Mutex();
		break;
	case epl::LOCK_POLICY_NONE:
		m_containerLock=EP_NEW epl::NoLock();
		break;
	default:
		m_containerLock=NULL;
		break;
	}
}
BaseServerObject::~BaseServerObject()
{
}
BaseServerObject & BaseServerObject::operator=(const BaseServerObject&b)
{
	if(this!=&b)
	{
		Thread::operator=(b);
		SmartObject::operator =(b);
		m_waitTime=b.m_waitTime;
		m_syncPolicy=b.m_syncPolicy;
		m_container=b.m_container;
		m_lockPolicy=b.m_lockPolicy;
		switch(m_lockPolicy)
		{
		case epl::LOCK_POLICY_CRITICALSECTION:
			m_containerLock=EP_NEW epl::CriticalSectionEx();
			break;
		case epl::LOCK_POLICY_MUTEX:
			m_containerLock=EP_NEW epl::Mutex();
			break;
		case epl::LOCK_POLICY_NONE:
			m_containerLock=EP_NEW epl::NoLock();
			break;
		default:
			m_containerLock=NULL;
			break;
		}
	}
	return *this;
}

void BaseServerObject::SetWaitTime(unsigned int milliSec)
{
	m_waitTime=milliSec;
}
unsigned int BaseServerObject::GetWaitTime()
{
	return m_waitTime;
}

void BaseServerObject::setContainer(ServerObjectList *container)
{
	LockObj lock(m_containerLock);
	m_container=container;
}
bool BaseServerObject::removeSelfFromContainer()
{
	LockObj lock(m_containerLock);
	bool ret=false;
	if(m_container)
		ret=m_container->Remove(this);
	m_container=NULL;
	return ret;
}

void BaseServerObject::setSyncPolicy(SyncPolicy syncPolicy)
{
	m_syncPolicy=syncPolicy;
}