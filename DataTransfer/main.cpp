#include <iostream>
#include "Common/inc/CommonUtil.h"

using namespace std;

int main(void)
{
	cout << "DataTransfer Start" << endl;

	std::string strIPCConfigPath = "../Resource/ipc.cfg";	
	std::string strMemInfo = CCommonUtility::GetValue(strIPCConfigPath, "SHARED_MEMORY_INFO", "SIM_TO_NET_SHM_KEY");
	cout << strMemInfo << endl;

	return 0;
}
