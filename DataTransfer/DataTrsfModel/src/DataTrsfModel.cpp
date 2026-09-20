#include "DataTrsfModel.h"

std::unique_ptr<DataTrsfModel> DataTrsfModel::pDataTrsfModel = nullptr;

DataTrsfModel::DataTrsfModel()
	: pSharedMemControl(nullptr)
{
}

DataTrsfModel::~DataTrsfModel()
{
}

DataTrsfModel* DataTrsfModel::GetInstance()
{
	if (nullptr == pDataTrsfModel)
	{	
		pDataTrsfModel = std::make_unique<DataTrsfModel>();
	}

	return pDataTrsfModel.get();
}

void DataTrsfModel::Start(const int nStartMode)
{
	switch (nStartMode)
	{
	case 0:
		{
			pSharedMemControl = std::make_unique<CSharedMemControl>();
			pSharedMemControl->InitializeShm();
			
			pNetworkManager = std::make_unique<NetworkManager>();
			pNetworkManager->StartMasterServer();
		}
		break;

	case 1:
		break;

	default:
		break;
	}
}

void DataTrsfModel::StartIPC(void)
{

}
