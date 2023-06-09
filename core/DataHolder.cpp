//***************************************************************
//  File:    DataHolder.cpp
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
#include "DataHolder.h"

namespace hare
{
	DataHolder::DataHolder() : data(NULL), size(0)
	{
	}

	DataHolder::~DataHolder()
	{
		clear();
	}

	//void DataHolder::set(uint8* data, uint32 size)
	//{
	//	assert(data && size >  0);

	//	clear();

	//	this->data = data;
	//	this->size = size;
	//}

	void DataHolder::allocate(uint32 size)
	{
		assert(size > 0);

		clear();

		this->data = new uint8[size];
		this->size = size;
		memset(this->data, 0, this->size);
	}

	void DataHolder::clear()
	{
        if (data)
        {
            delete data;
            data = NULL;
            size = 0;
        }
	}
}
