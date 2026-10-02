#pragma once

#include <memory>
#include <string>

#include "SharedMemControl.h"
#include "NetworkManager.h"
#include "ICommonInterface.h"

class DataTrsfModel : public ICommonInterface
{
public:
	static DataTrsfModel* GetInstance();	

	void Start(const int nStartMode);

	void StartIPC(void);

	friend std::unique_ptr<DataTrsfModel> std::make_unique<DataTrsfModel>();
	friend std::unique_ptr<DataTrsfModel>::deleter_type;

	virtual void OnRecvControlMessage(std::string recvMessage) override;

private:
	static std::unique_ptr<DataTrsfModel> pDataTrsfModel;
	std::unique_ptr<SharedMemControl> pSharedMemControl;
	std::unique_ptr<NetworkManager> pNetworkManager;

private:
	explicit DataTrsfModel();
	virtual ~DataTrsfModel();
};