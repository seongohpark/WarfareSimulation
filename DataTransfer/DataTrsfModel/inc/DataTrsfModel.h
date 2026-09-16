#pragma once

#include <memory>

class SharedMemControl;

class DataTrsfModel 
{
public:
	static DataTrsfModel* GetInstance();	

	void Start();

	friend std::unique_ptr<DataTrsfModel> std::make_unique<DataTrsfModel>();
	friend std::unique_ptr<DataTrsfModel>::deleter_type;

private:
	static std::unique_ptr<DataTrsfModel> pDataTrsfModel;

	SharedMemControl* IPCControl;

private:
	explicit DataTrsfModel();
	virtual ~DataTrsfModel();
};