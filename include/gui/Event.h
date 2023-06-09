#ifndef _EVENT_H_
#define _EVENT_H_

#include "UIPrerequisites.h"

namespace hare
{
    enum uiPropagationState
    {
        // don't propagate it at all
        uiEvent_Propagate_None = 0,
        // propagate it until it is processed
        uiEvent_Propagate_Max = INT_MAX
    };

    typedef int32 EventType;

    extern UI_API EventType newEventType();
    extern UI_API int32 newId();
    extern UI_API int32 newId(const String& name);
    // ---------------------------------------------------------------------------
    // Event Types
    // ---------------------------------------------------------------------------
    extern UI_API const EventType uiEVT_NULL;
    extern UI_API const EventType uiEVT_FIRST;
    extern UI_API const EventType uiEVT_USER_FIRST;

    // Mouse Events
    extern UI_API const EventType uiEVT_LEFT_DOWN;
    extern UI_API const EventType uiEVT_LEFT_UP;
    extern UI_API const EventType uiEVT_LEFT_CLICK;
    extern UI_API const EventType uiEVT_LEFT_DCLICK;
    extern UI_API const EventType uiEVT_LEFT_TCLICK;

    extern UI_API const EventType uiEVT_RIGHT_DOWN;
    extern UI_API const EventType uiEVT_RIGHT_UP;
    extern UI_API const EventType uiEVT_RIGHT_CLICK;
    extern UI_API const EventType uiEVT_RIGHT_DCLICK;
    extern UI_API const EventType uiEVT_RIGHT_TCLICK;

    extern UI_API const EventType uiEVT_MIDDLE_DOWN;
    extern UI_API const EventType uiEVT_MIDDLE_UP;
    extern UI_API const EventType uiEVT_MIDDLE_CLICK;
    extern UI_API const EventType uiEVT_MIDDLE_DCLICK;
    extern UI_API const EventType uiEVT_MIDDLE_TCLICK;

    extern UI_API const EventType uiEVT_MOTION;
    extern UI_API const EventType uiEVT_LEAVE_WINDOW;
    extern UI_API const EventType uiEVT_ENTER_WINDOW;
    extern UI_API const EventType uiEVT_MOUSEWHEEL;

    extern UI_API const EventType uiEVT_CHAR;
    extern UI_API const EventType uiEVT_KEY_DOWN;
    extern UI_API const EventType uiEVT_KEY_UP;
    // ---------------------------------------------------------------------------
    // Event Functions
    // ---------------------------------------------------------------------------
    typedef void (EventHandler::*EventFunction)(Event&);
    typedef void (EventHandler::*MouseEventFunction)(MouseEvent&);
    
    // ---------------------------------------------------------------------------
    // EventHandler
    // ---------------------------------------------------------------------------
    class UI_API EventHandler : public Object
    {
    public:
        EventHandler();
        virtual ~EventHandler();

        EventHandler* getNextHandler() const { return nextEvtHandler; }
        EventHandler* getPrevHandler() const { return prevEvtHandler; }
        void setNextHandler(EventHandler *handler) { nextEvtHandler = handler; }
        void setPrevHandler(EventHandler *handler) { prevEvtHandler = handler; }

        void setEvtHandlerEnabled(bool enabled) { handlerEnabled = enabled; }
        bool getEvtHandlerEnabled() const { return handlerEnabled; }

        // process an event right now
        virtual bool processEvent(Event& event);

        // add an event to be processed later
        void addPendingEvent(Event& event);

        void processPendingEvents();

        void connect(int id, int lastId, int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0);

        void connect(int id, int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0)
        {
            connect(id, uiID_Any, eventType, func, userData, eventSink);
        }

        void connect(int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0)
        {
            connect(uiID_Any, uiID_Any, eventType, func, userData, eventSink);
        }

        void disconnect(int id, int lastId, int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0);

        void disconnect(int id, int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0)
        {
            disconnect(id, uiID_Any, eventType, func, userData, eventSink);
        }

        void disconnect(int eventType, EventFunction func, 
            void* userData = 0, EventHandler* eventSink = 0)
        {
            disconnect(uiID_Any, uiID_Any, eventType, func, userData, eventSink);
        }

        static bool processEventIfMatches(const EventTableEntryBase& entry, 
            EventHandler* handler, Event& event);

    protected:
        bool handleDynamicEvent(Event& event);
        bool handleEvent(Event& event);

    protected:
        EventHandler* nextEvtHandler;
        EventHandler* prevEvtHandler;

        bool handlerEnabled;

        typedef std::list<DynamicEventTableEntry*> DynamicEventList;

        DynamicEventList* dynamicEvents;

        typedef std::list<Event*> PendingEventList;

        PendingEventList* pendingEvents;

        typedef std::list<EventHandler*> EventHandlerList;

        static EventHandlerList* pendingEventHandlers;

        static const EventTable eventTable;
        virtual const EventTable* getEventTable() const;

        virtual bool tryParent(Event& event);

    private:
        static const EventTableEntry eventTableEntries[];
    };

    // ---------------------------------------------------------------------------
    // EventTable
    // ---------------------------------------------------------------------------
    struct UI_API EventTable
    {
        const EventTable *baseTable;    // base event table (next in chain)
        const EventTableEntry *entries; // bottom of entry array
    };

    // ---------------------------------------------------------------------------
    // EventTableEntryBase
    // ---------------------------------------------------------------------------
    struct UI_API EventTableEntryBase
    {
    private:
        EventTableEntryBase& operator=(const EventTableEntryBase& event);

    public:
        EventTableEntryBase(int winid, int idLast,
            EventFunction fn, void* data)
            : id(winid),
            lastId(idLast),
            func(fn),
            userData(data)
        { }

        EventTableEntryBase(const EventTableEntryBase& rhs)
            : id(rhs.id),
            lastId(rhs.lastId),
            func(rhs.func),
            userData(rhs.userData)
        { }

        int id, lastId;
        EventFunction func;
        void* userData;
    };

    // ---------------------------------------------------------------------------
    // EventTableEntry
    // ---------------------------------------------------------------------------
    struct UI_API EventTableEntry : public EventTableEntryBase
    {
        EventTableEntry(const int& evType, int winid, int idLast,
            EventFunction fn, void* data)
            : EventTableEntryBase(winid, idLast, fn, data),
            eventType(evType)
        { }

        const int& eventType;

    private:
        EventTableEntry& operator=(const EventTableEntry&);
    };

    // ---------------------------------------------------------------------------
    // DynamicEventTableEntry
    // ---------------------------------------------------------------------------
    struct UI_API DynamicEventTableEntry : public EventTableEntryBase
    {
        DynamicEventTableEntry(int evType, int winid, int idLast,
            EventFunction fn, void* data, EventHandler* eventSink)
            : EventTableEntryBase(winid, idLast, fn, data),
            eventType(evType),
            eventSink(eventSink)
        { }

        int eventType;
        EventHandler* eventSink;

        HARE_DECLARE_NO_COPY_CLASS(DynamicEventTableEntry)
    };

    // ---------------------------------------------------------------------------
    // Event
    // ---------------------------------------------------------------------------
    class UI_API Event : public Object
    {
        HARE_DECLARE_ABSTRACT_CLASS(Event)

        friend class PropagateOnce;
    private:
        Event& operator=(const Event&);
    protected:
        Event(const Event&);

    public:
        Event(int winid = 0, EventType commandType = uiEVT_NULL);

        virtual Event *cloneEvent() const = 0;

        void setEventType(EventType type) 
        { 
            eventType = type; 
        }
        
        EventType getEventType() const 
        { 
            return eventType; 
        }

        Object* getEventSender() const 
        { 
            return eventSender; 
        }
        
        void setEventSender(Object* sender) 
        { 
            eventSender = sender; 
        }
        
        int getId() const 
        { 
            return id; 
        }
        
        void setId(int Id) 
        { 
            id = Id; 
        }

        void skip(bool skip = true) 
        { 
            skipped = skip; 
        }
        
        bool isSkipped() const 
        { 
            return skipped; 
        }

        bool isCommandEvent() const 
        { 
            return isCmdEvent; 
        }

        bool shouldPropagate() const
        { 
            return propagationLevel != uiEvent_Propagate_None; 
        }

        int stopPropagation()
        {
            int oldLevel = propagationLevel;
            propagationLevel = uiEvent_Propagate_None;
            return oldLevel;
        }

        void resumePropagation(int level)
        {
            propagationLevel = level;
        }

    protected:
        friend class EventHandler;

        Object* eventSender;
        EventType eventType;
        int id;

    public:
        void* userData;

    protected:
        int propagationLevel;

    protected:
        bool skipped;
        bool isCmdEvent;
    };

    class UI_API PropagateOnce
    {
    public:
        PropagateOnce(Event& event) : event(event)
        {
            assert(event.propagationLevel > 0);
            event.propagationLevel--;
        }

        ~PropagateOnce()
        {
            event.propagationLevel++;
        }

    private:
        Event& event;

        HARE_DECLARE_NO_COPY_CLASS(PropagateOnce)
    };

    class UI_API MouseEvent : public Event
    {
        HARE_DECLARE_ABSTRACT_CLASS(MouseEvent)

    public:
        MouseEvent(EventType commandType);

        MouseEvent(const MouseEvent& rhs) : Event(rhs)
        { 
            assign(rhs); 
        }

        bool isLeftDown() const { return leftDown; }
        bool isMiddleDown() const { return middleDown; }
        bool isRightDown() const { return rightDown; }

        bool isEnteringEvent() const 
        { 
            return eventType == uiEVT_ENTER_WINDOW; 
        }

        bool isLeavingEvent() const 
        { 
            return eventType == uiEVT_LEAVE_WINDOW;
        }

        float getWheelDelta() const 
        {
            return wheelDelta; 
        }

        PointF getPosition() const
        {
            return position;
        }

        virtual Event *cloneEvent() const { return new MouseEvent(*this); }

        MouseEvent& operator=(const MouseEvent& rhs) { assign(rhs); return *this; }

    protected:
        void assign(const MouseEvent& rhs);

    protected:
        friend class GUISystem;

        bool leftDown;
        bool middleDown;
        bool rightDown;

        float wheelDelta;
        PointF position;
    };

    class UI_API KeyEvent : public Event
    {
        HARE_DECLARE_ABSTRACT_CLASS(KeyEvent)
    public:
        KeyEvent(EventType keyType = uiEVT_NULL);
        KeyEvent(const KeyEvent& rhs);

        uint32 getKeyCode() const { return keyCode; }
        uint32 getChar() const { return text; }

        virtual Event *cloneEvent() const { return new KeyEvent(*this); }

    public:
        uint32 keyCode;
        uint32 text;
    };


    class UI_API CommandEvent : public Event
    {

    };

#define HARE_DECLARE_EVENT_TABLE_ENTRY(type, winid, idLast, fn, data) \
    EventTableEntry(type, winid, idLast, fn, data)

#define HARE_DECLARE_EVENT_TABLE() \
private: \
    static const EventTableEntry eventTableEntries[]; \
protected: \
    static const EventTable eventTable; \
    virtual const EventTable* getEventTable() const; \

#define HARE_BEGIN_EVENT_TABLE(theClass, baseClass) \
    const EventTable theClass::eventTable = \
    { &baseClass::eventTable, &theClass::eventTableEntries[0] }; \
    const EventTable *theClass::getEventTable() const \
    { return &theClass::eventTable; } \
    const EventTableEntry theClass::eventTableEntries[] = { \

#define HARE_END_EVENT_TABLE() HARE_DECLARE_EVENT_TABLE_ENTRY(uiEVT_NULL, 0, 0, 0, 0) };

#define __HARE_DECLARE_EVT2(evt, id1, id2, fn) \
    HARE_DECLARE_EVENT_TABLE_ENTRY(evt, id1, id2, fn, NULL),
#define __HARE_DECLARE_EVT1(evt, id, fn) \
    __HARE_DECLARE_EVT2(evt, id, uiID_Any, fn)
#define __HARE_DECLARE_EVT0(evt, fn) \
    __HARE_DECLARE_EVT1(evt, uiID_Any, fn)

#define MouseEventHandler(func) \
    (EventFunction)static_cast<MouseEventFunction>(&func)

#define HARE_EVT_LEFT_DOWN(func)    __HARE_DECLARE_EVT0(uiEVT_LEFT_DOWN,    MouseEventHandler(func))
#define HARE_EVT_LEFT_UP(func)      __HARE_DECLARE_EVT0(uiEVT_LEFT_UP,      MouseEventHandler(func))
#define HARE_EVT_MIDDLE_DOWN(func)  __HARE_DECLARE_EVT0(uiEVT_MIDDLE_DOWN,  MouseEventHandler(func))
#define HARE_EVT_MIDDLE_UP(func)    __HARE_DECLARE_EVT0(uiEVT_MIDDLE_UP,    MouseEventHandler(func))
#define HARE_EVT_RIGHT_DOWN(func)   __HARE_DECLARE_EVT0(uiEVT_RIGHT_DOWN,   MouseEventHandler(func))
#define HARE_EVT_RIGHT_UP(func)     __HARE_DECLARE_EVT0(uiEVT_RIGHT_UP,     MouseEventHandler(func))
#define HARE_EVT_MOTION(func)       __HARE_DECLARE_EVT0(uiEVT_MOTION,       MouseEventHandler(func))
#define HARE_EVT_LEFT_CLICK(func)   __HARE_DECLARE_EVT0(uiEVT_LEFT_CLICK,   MouseEventHandler(func))
#define HARE_EVT_MIDDLE_CLICK(func) __HARE_DECLARE_EVT0(uiEVT_MIDDLE_CLICK, MouseEventHandler(func))
#define HARE_EVT_RIGHT_CLICK(func)  __HARE_DECLARE_EVT0(uiEVT_RIGHT_CLICK,  MouseEventHandler(func))
#define HARE_EVT_LEFT_DCLICK(func)  __HARE_DECLARE_EVT0(uiEVT_LEFT_DCLICK,  MouseEventHandler(func))
#define HARE_EVT_MIDDLE_DCLICK(func)__HARE_DECLARE_EVT0(uiEVT_MIDDLE_DCLICK,MouseEventHandler(func))
#define HARE_EVT_RIGHT_DCLICK(func) __HARE_DECLARE_EVT0(uiEVT_RIGHT_DCLICK, MouseEventHandler(func))
#define HARE_EVT_LEFT_TCLICK(func)  __HARE_DECLARE_EVT0(uiEVT_LEFT_TCLICK,  MouseEventHandler(func))
#define HARE_EVT_MIDDLE_TCLICK(func)__HARE_DECLARE_EVT0(uiEVT_MIDDLE_TCLICK,MouseEventHandler(func))
#define HARE_EVT_RIGHT_TCLICK(func) __HARE_DECLARE_EVT0(uiEVT_RIGHT_TCLICK, MouseEventHandler(func))
#define HARE_EVT_LEAVE_WINDOW(func) __HARE_DECLARE_EVT0(uiEVT_LEAVE_WINDOW, MouseEventHandler(func))
#define HARE_EVT_ENTER_WINDOW(func) __HARE_DECLARE_EVT0(uiEVT_ENTER_WINDOW, MouseEventHandler(func))
#define HARE_EVT_MOUSEWHEEL(func)   __HARE_DECLARE_EVT0(uiEVT_MOUSEWHEEL,   MouseEventHandler(func))

}

#endif
