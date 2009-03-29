

class UIManager
{


    bool notifyMouseMove(f32 deltaX, f32 deltaY);
    bool notifyMouseLeaves();
    
    bool notifyMouseEvent();
    
    bool notifyKeyDown(u32 key);
    bool notifyKeyUp(u32 key);
    bool notifyChar(u32 cp);
    bool notifyMouseWheel(f32 delta);
    bool notifyMousePosition(f32 posX, f32 posY);
};