#pragma once

#include <memory>
#include "SharedMemControl.h"
#include "NetworkManager.h"

class DataTrsfModel 
{
public:
	static DataTrsfModel* GetInstance();	

	void Start(const int nStartMode);

	void StartIPC(void);

	friend std::unique_ptr<DataTrsfModel> std::make_unique<DataTrsfModel>();
	friend std::unique_ptr<DataTrsfModel>::deleter_type;

private:
	static std::unique_ptr<DataTrsfModel> pDataTrsfModel;
	std::unique_ptr<SharedMemControl> pSharedMemControl;
	std::unique_ptr<NetworkManager> pNetworkManager;

private:
	explicit DataTrsfModel();
	virtual ~DataTrsfModel();
};