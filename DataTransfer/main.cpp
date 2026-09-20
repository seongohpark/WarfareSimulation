#include <iostream>

#include "DataTrsfModel/inc/DataTrsfModel.h"
#include "Common/inc/CommonUtil.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "DataTransfer Start" << endl;

	if (argc < 2)
	{
		cout << "like DataTransfer 0 " << endl;
		return 1;
	}

	const int nStartMode = stoi(argv[1], nullptr);
	DataTrsfModel::GetInstance()->Start(nStartMode);

	return 0;
}
