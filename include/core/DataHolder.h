//***************************************************************
//  File:    DataHolder.h
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
#ifndef _DATAHOLDER_H_
#define _DATAHOLDER_H_

#include "CorePrerequisites.h"

namespace hare
{
	class CORE_API DataHolder
	{
	public:
		DataHolder();
		DataHolder(uint8* data, uint32 size){ set(data, size); }
		virtual ~DataHolder();

		void set(uint8* data, uint32 size);

		void allocate(uint32 size);
		void clear();

		void* getData() const {
			return data;
		}

		uint32 getSize() const {
			return size;
		}

	protected:
		uint8* data;
		uint32 size;
	};
}

#endif
