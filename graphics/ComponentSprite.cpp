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
    }

    void ComponentSprite::moveTo(float x, float y)
    {
        Sprite::List::iterator it = components.begin();
        for (;it != components.end(); ++it){
            int origoX = origoPos.x;
            int origoY = origoPos.y;

            if (!bFaceX){
                origoX = -origoX;
            }

            if (!bFaceY){
                origoY = -origoY;
            }

            (*it)->moveTo(x + origoX, y + origoY);
        }
    }

    void ComponentSprite::beginScene()
    {

    }

    void ComponentSprite::renderScene()
    {
        Sprite::List::iterator it = components.begin();
        for (;it != components.end(); ++it){
            (*it)->renderScene();
        }
    }

    void ComponentSprite::endScene()
    {

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
}