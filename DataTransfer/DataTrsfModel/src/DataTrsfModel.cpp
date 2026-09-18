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

void DataTrsfModel::Start()
{
	//pSharedMemControl = std::make_unique<CSharedMemControl>();
	pNetworkManager = std::make_unique<NetworkManager>();
	pNetworkManager->StartMasterServer();
}

void DataTrsfModel::StartIPC(void)
{

}
