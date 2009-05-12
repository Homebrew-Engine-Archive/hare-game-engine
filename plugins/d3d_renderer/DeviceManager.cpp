//***************************************************************
//  File:    DeviceManager.cpp
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#include "PCH.h"
#include "DeviceManager.h"



HARE_IMPLEMENT_SINGLETON(DeviceManager)

DeviceManager::DeviceManager()
{

}

DeviceManager::~DeviceManager()
{

}

void DeviceManager::registerDeviceObject(DeviceObject* object)
{
	if (std::find(deviceObjects.begin(), deviceObjects.end(), object) == deviceObjects.end())
		deviceObjects.push_back(object);
}

void DeviceManager::unregisterDeviceObject(DeviceObject* object)
{
	DeviceObjectList::iterator it = std::find(deviceObjects.begin(), deviceObjects.end(), object);
	if (it != deviceObjects.end())
		deviceObjects.erase(it);
}

void DeviceManager::beforeResetDevice()
{
	DeviceObjectList::iterator it = deviceObjects.begin();
	for (;it != deviceObjects.end(); ++it){
		(*it)->beforeResetDevice();
	}
}

void DeviceManager::afterResetDevice()
{
	DeviceObjectList::iterator it = deviceObjects.begin();
	for (;it != deviceObjects.end(); ++it){
		(*it)->afterResetDevice();
	}	
}
