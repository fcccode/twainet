#include "managers_container.h"
#include "common/ref.h"

/****************************************************************************************************************/
/*                                              DynamicManager                                                  */
/****************************************************************************************************************/
DynamicManager::DynamicManager()
	: m_isStop(false)
{
}

DynamicManager::~DynamicManager()
{
}

void DynamicManager::Stop()
{
	m_isStop = true;
}

bool DynamicManager::IsDelete()
{
	return true;
}
bool DynamicManager::IsStop()
{
	return m_isStop;
}

/****************************************************************************************************************/
/*                                               StaticManager                                                  */
/****************************************************************************************************************/
StaticManager::StaticManager()
	: m_isStop(false)
{
}

StaticManager::~StaticManager()
{
}

bool StaticManager::IsDelete()
{
	return false;
}

bool StaticManager::IsStop()
{
	return m_isStop;
}

void StaticManager::Stop()
{
	m_isStop = true;
}

/****************************************************************************************************************/
/*                                             ManagersContainer                                                */
/****************************************************************************************************************/
ManagersContainer::ManagersContainer()
: m_isExit(false)
{
	Start();
}

ManagersContainer::~ManagersContainer()
{
	Stop();
	Join();
}

void ManagersContainer::AddManager(IManager* manager)
{
	manager->ManagerStart();
	m_managers.AddObject(manager);
}

void ManagersContainer::RemoveManager(IManager* manager)
{
	manager->ManagerStop();
	m_managers.RemoveObject(manager);
}

bool ManagersContainer::RunManager(const IManager* manager)
{
	const_cast<IManager*>(manager)->ManagerFunc();
	if (const_cast<IManager*>(manager)->IsStop())
	{
		const_cast<IManager*>(manager)->ManagerStop();
		if(const_cast<IManager*>(manager)->IsDelete())
			delete manager;
		return true;
	}

	return false;
}

bool ManagersContainer::CheckManager(const std::vector<IManager*>& managers, const IManager* manager)
{
	IManager* manager_ = const_cast<IManager*>(manager);
	if(manager_->IsDelete())
	{
		const_cast<std::vector<IManager*>&>(managers).push_back(manager_);
	}
	else
	{
		manager_->ManagerStop();
	}
	return true;
}

void ManagersContainer::ThreadFunc()
{
	while(!m_isExit)
	{
		m_managers.CheckObjects(Ref(this, &ManagersContainer::RunManager));
		sleep(200);
	}
	
	std::vector<IManager*> managers;
	m_managers.CheckObjects(Ref(this, &ManagersContainer::CheckManager, managers));
	for(std::vector<IManager*>::iterator it = managers.begin();
		it != managers.end(); it++)
	{
		(*it)->ManagerStop();
		delete (*it);
	}
}

void ManagersContainer::OnStop()
{
}

void ManagersContainer::OnStart()
{
}

void ManagersContainer::Stop()
{
	m_isExit = true;
}
