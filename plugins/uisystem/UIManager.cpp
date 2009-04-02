#include "PCH.h"

//bool UIManager::notifyMouseMove(f32 deltaX, f32 deltaY)
//{
//    MouseEvent event(uiEVT_MOTION);
//    event.
//
//    Window* win = 0;
//    win->processEvent(event);
//
//
//    MouseEventArgs ma(0);
//    MouseCursor& mouse = MouseCursor::getSingleton();
//
//    ma.moveDelta.d_x = delta_x * d_mouseScalingFactor;
//    ma.moveDelta.d_y = delta_y * d_mouseScalingFactor;
//
//    // only continue setup and injection if mouse position has changed
//    if ((ma.moveDelta.d_x != 0) || (ma.moveDelta.d_y != 0))
//    {
//        ma.sysKeys = d_sysKeys;
//        ma.wheelChange = 0;
//        ma.clickCount = 0;
//        ma.button = NoButton;
//
//        // move the mouse cursor & update position in args.
//        mouse.offsetPosition(ma.moveDelta);
//        ma.position = mouse.getPosition();
//
//        return mouseMoveInjection_impl(ma);
//    }
//
//    return false;
//}
//bool UIManager::notifyMouseLeaves();
//bool UIManager::notifyMouseWheel(f32 delta);
//bool UIManager::notifyMousePosition(f32 posX, f32 posY);
//bool UIManager::notifyMouseEvent();
//
//bool UIManager::notifyKeyDown(u32 key);
//bool UIManager::notifyKeyUp(u32 key);
//bool UIManager::notifyChar(u32 cp);