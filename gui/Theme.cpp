#include "PCH.h"
#include "Theme.h"

namespace hare
{
    HARE_IMPLEMENT_ABSTRACT_CLASS(Theme, Object, 0)
    {
        HARE_META(name, String)
    }

    HARE_IMPLEMENT_ABSTRACT_CLASS(MaterialTheme, Theme, 0)
    {
        HARE_OBJ(material, Material)
    }
}


