#include "DataTrsfModel.h"
#include "SharedMemControl.h"

std::unique_ptr<DataTrsfModel> DataTrsfModel::pDataTrsfModel = nullptr;

DataTrsfModel::DataTrsfModel()
{
}

DataTrsfModel::~DataTrsfModel()
{
}

DataTrsfModel* DataTrsfModel::GetInstance()
{
	if (nullptr == pDataTrsfModel)
	{
		//pDataTrsfModel.reset(new DataTrsfModel());
		pDataTrsfModel = std::make_unique<DataTrsfModel>();
	}

	return pDataTrsfModel.get();
}

void DataTrsfModel::Start()
{

}
