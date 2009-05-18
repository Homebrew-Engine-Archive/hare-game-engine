//***************************************************************
//  File:    ComponentSprite.cpp
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
#include "ComponentSprite.h"

namespace hare
{
    HARE_IMPLEMENT_DYNAMIC_CLASS(ComponentSprite, Sprite, 0)
    {
        HARE_OBJ_LIST(components, Sprite)
    }

    ComponentSprite::ComponentSprite()
    {

    }

    ComponentSprite::~ComponentSprite()
    {

    }

    void ComponentSprite::move(float dx, float dy)
    {
        Sprite::List::iterator it = components.begin();
        for (;it != components.end(); ++it){
            (*it)->move(dx, dy);
        }
        pos.x += dx; pos.y += dy;
    }

    void ComponentSprite::moveTo(float x, float y)
    {
        Sprite::List::iterator it = components.begin();
        for (;it != components.end(); ++it){
            float origoX = origoPos.x;
            float origoY = origoPos.y;

            if (!bFaceX){
                origoX = -origoX;
            }

            if (!bFaceY){
                origoY = -origoY;
            }

            (*it)->moveTo(x + origoX, y + origoY);
        }
        pos.x = x; pos.y = y;
    }

    void ComponentSprite::render()
    {
        moveTo(pos.x, pos.y);
        Sprite::List::iterator it = components.begin();
        for (;it != components.end(); ++it){
            (*it)->render();
        }
    }

    int ComponentSprite::addPart(Sprite* s)
    {
        components.push_back(s);
        return components.size() - 1;
    }

    bool ComponentSprite::removePart(int partID)
    {
        Sprite::List::iterator it = getPartIT(partID);
        if (it != components.end()){
            components.erase(it);
            return true;
        }else{
            return false;
        }
    }

    bool ComponentSprite::removePart(Sprite* s)
    {
        Sprite::List::iterator it = std::find(components.begin(), components.end(), s);
        if (it != components.end()){
            components.erase(it);
            return true;
        }else{
            return false;
        }
    }

    bool ComponentSprite::swapPart(int partID_1, int partID_2)
    {
        if (partID_1 == partID_2)
            return true;

        Sprite::List::iterator it1 = getPartIT(partID_1);
        if (it1 == components.end())
            return false; 

        Sprite::List::iterator it2 = getPartIT(partID_2);
        if (it2 == components.end())
            return false;
        
        Sprite::Ptr s = *it1;
        *it1 = *it2;
        *it2 = s;

        return true;
    }

    Sprite* ComponentSprite::getPart(int partID)
    {
        Sprite::List::iterator it = getPartIT(partID);
        if (it != components.end())
            return *it;
        else
            return NULL;
    }

    Sprite::List::iterator ComponentSprite::getPartIT(int partID)
    {
        if (partID < 0)
            return components.end();

        if (partID >= (int)components.size())
            return components.end();

        Sprite::List::iterator it = components.begin();
        for (int i = 0; i < partID; ++i, ++it);

        return it;
    }

    uint32 ComponentSprite::getPartCount()
    {
        return components.size();
    }

    int ComponentSprite::getPartID(Sprite* s)
    {
        Sprite::List::iterator it = components.begin();
        for (int id = 0; it != components.end(); ++it, ++id){
            if (*it == s){
                return id;
            }
        }

        return -1;
    }
}