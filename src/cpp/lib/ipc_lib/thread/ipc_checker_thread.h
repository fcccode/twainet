#ifndef IPC_CHECKER_THREAD_H
#define IPC_CHECKER_THREAD_H

#include "thread_lib/common/managers_container.h"

class IPCConnector;

class IPCCheckerThread : public DynamicManager
{
public:
	IPCCheckerThread(IPCConnector* connector);
	virtual ~IPCCheckerThread();
protected:
	void ManagerFunc();
	void ManagerStart(){};
	void ManagerStop(){};
private:
	IPCConnector* m_connector;
	int m_count;
};

#endif/*IPC_CHECKER_THREAD_H*/