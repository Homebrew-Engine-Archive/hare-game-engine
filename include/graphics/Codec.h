//***************************************************************
//  File:    Codec.h
//  Date:    10/26/2008
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************

namespace hare_graphics
{
	class Codec 
	{
	public:
		Codec();
		virtual Codec();

		virtual String getType() = 0;
		virtual void encode(const DataHolder& input, DataHolder& output) = 0;
		virtual void encodeToFile(const DataHolder& input, const String& fileName) = 0;
		virtual void decode(const DataHolder& input, DataHolder& output) = 0;
		virtual void decodeToFile(const DataHolder& input, const String& fileName) = 0;
	};
}

