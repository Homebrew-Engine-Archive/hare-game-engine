#ifndef _DEVICEMANAGER_H_
#define _DEVICEMANAGER_H_


class DeviceObject
{
public:
	virtual void beforeResetDevice() = 0;
	virtual void afterResetDevice() = 0;
};

class DeviceManager : public Singleton<DeviceManager>
{
	HARE_DECLARE_SINGLETON(DeviceManager)
public:
	DeviceManager();
	~DeviceManager();

	void registerDeviceObject(DeviceObject* object);
	void unregisterDeviceObject(DeviceObject* object);

	void beforeResetDevice();
	void afterResetDevice();

protected:
	typedef std::list<DeviceObject*> DeviceObjectList;
	DeviceObjectList deviceObjects;
};


#endif