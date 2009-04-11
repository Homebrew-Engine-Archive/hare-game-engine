#include "PCH.h"
#include "ImageCodec.h"

namespace hare_graphics
{
	std::map<String, ImageCodec*> ImageCodec::codecMap;

	ImageCodec::ImageCodec(const String& type, uint32 devILTypeID)
		:codecType(type)
		,typeID(devILTypeID)
	{
	}

	ImageCodec::~ImageCodec(void)
	{
	}

	void ImageCodec::registerCodec(ImageCodec* imageCodec)
	{
		codecMap[imageCodec->getType()] = imageCodec;
	}

	void ImageCodec::unregisterCodec(ImageCodec* imageCodec)
	{
		codecMap.erase(imageCodec->getType());
	}

	StringVector ImageCodec::getExtensions()
	{
		StringVector results;
		results.reserve(codecMap.size());
		CodecMap::const_iterator it = codecMap.begin();
		for (;it != codecMap.end(); ++it){
			results.push_back(it->first);
		}
		return results;
	}

	ImageCodec* ImageCodec::getCodec(const String& extension)
	{
		String lwrcase = extension;
		StringUtil::toLowerCase(lwrcase);
		CodecMap::const_iterator it = codecMap.find(lwrcase);
		if (it == codecMap.end())
		{
			//exception
		}

		return it->second;
	}

}

