#ifndef _EVENT_H_
#define _EVENT_H_

#include "UIPrerequisites.h"

namespace hare_ui
{
    enum uiPropagationState
    {
        // don't propagate it at all
        uiEvent_Propagate_None = 0,
        // propagate it until it is processed
        uiEvent_Propagate_Max = INT_MAX
    };

    typedef int EventType;

    extern UI_API EventType newEventType();
    
    // ---------------------------------------------------------------------------
    // EventHandler
    // ---------------------------------------------------------------------------
    class UI_API EventHandler : public Object
    {
        HARE_DECLARE_DYNAMIC_CLASS(EventHandler)

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

        void connect();

        void disconnect();

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
    private:
        static const EventTableEntry eventTableEntries[];
    };

    typedef void (EventHandler::*EventFunction)(Event&);

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


    extern UI_API const EventType uiEVT_NULL;
    extern UI_API const EventType uiEVT_FIRST;
    extern UI_API const EventType uiEVT_USER_FIRST;

    // ---------------------------------------------------------------------------
    // Event
    // ---------------------------------------------------------------------------
    class UI_API Event
    {
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

        void* getEventSender() const 
        { 
            return eventSender; 
        }
        
        void setEventSender(void* sender) 
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

        void* eventSender;
        EventType eventType;
        int id;
        void* userData;

    protected:
        int propagationLevel;

    protected:
        bool skipped;
        bool isCmdEvent;
    };

    class UI_API MouseEvent : public Event
    {
    public:
        PointF getPosition() const
        {
            return pos;
        }

    protected:
        PointF pos;
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

#define HARE_EVT_MOUSE_EVENTS(func) \
    HARE_EVT_LEFT_DOWN(func) \
    HARE_EVT_LEFT_UP(func) \
    HARE_EVT_MIDDLE_DOWN(func) \
    HARE_EVT_MIDDLE_UP(func) \
    HARE_EVT_RIGHT_DOWN(func) \
    HARE_EVT_RIGHT_UP(func) \
    HARE_EVT_MOTION(func) \
    HARE_EVT_LEFT_DCLICK(func) \
    HARE_EVT_MIDDLE_DCLICK(func) \
    HARE_EVT_RIGHT_DCLICK(func) \
    HARE_EVT_LEAVE_WINDOW(func) \
    HARE_EVT_ENTER_WINDOW(func) \
    HARE_EVT_MOUSEWHEEL(func)

}

#endif
