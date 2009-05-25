//***************************************************************
//  File:    FMODSoundBuffer.cpp
//  Date:    5/24/2009
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
#include "FMODSoundBuffer.h"
#include "FMODSoundManager.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(FMODSoundBuffer, SoundBuffer, 0)
    {
    
    }

    FMODSoundBuffer::FMODSoundBuffer()
        :pSound(NULL)
    {

    }

    FMODSoundBuffer::FMODSoundBuffer(const String& name)
        :SoundBuffer(name)
        ,pSound(NULL)
    {

    }

    FMODSoundBuffer::~FMODSoundBuffer()
    {
        unload();
    }

    void FMODSoundBuffer::load(bool bStream)
    {
        if (pSound)
            return;

        FileSystem* fs = getFileSystem();
        FileHandle fh = fs->openFile(fileName, FM_Read);

        if (!fh)
            HARE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "filesystem null!", "FMODSoundBuffer::load"); 

        int size = fs->size(fh);
        buffer.allocate(size);
        fs->readFile(fh, (char*)buffer.getData(), size, 1);
        fs->closeFile(fh);

        FMOD_CREATESOUNDEXINFO exinfo;
        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
        exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.length = size;

        FMOD::System* system = ((FMODSoundManager*)(getSoundApp()->getSoundManager()))->getFMODSystem();

        if (bStream){
            system->createStream((const char*)buffer.getData(), 
                FMOD_HARDWARE | FMOD_OPENMEMORY | FMOD_2D, &exinfo, &pSound);
        }else{
            system->createSound((const char*)buffer.getData(), 
                FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_3D, &exinfo, &pSound);
            buffer.clear();
        }
    }

    void FMODSoundBuffer::unload()
    {
        if(pSound){
            pSound->release();
            pSound = NULL;
            buffer.clear();
        }
    }

    void FMODSoundBuffer::update()
    {
        if (!pSound)
            return;

        pSound->set3DMinMaxDistance(refDistance, maxDistance);

        float iCone, oCone, vCone;
        pSound->get3DConeSettings(&iCone, &oCone, &vCone);
        pSound->set3DConeSettings(innerCone, outerCone, vCone);

    }

    FMOD::Sound* FMODSoundBuffer::getSound()
    {
        return pSound;
    }

}