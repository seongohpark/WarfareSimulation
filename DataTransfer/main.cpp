#include <iostream>

#include "DataTrsfModel/inc/DataTrsfModel.h"
#include "Common/inc/CommonUtil.h"

using namespace std;

int main(void)
{
	cout << "DataTransfer Start" << endl;

	DataTrsfModel::GetInstance()->Start();

	return 0;
}
