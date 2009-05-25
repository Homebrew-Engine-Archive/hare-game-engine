#include "PCH.h"
#include "Event.h"
#include "Window.h"

namespace hare
{
    EventType newEventType()
    {
        static EventType currId = uiEVT_FIRST;
        return currId++;
    }

    int32 newId()
    {
        static int32 currId = 100;
        return currId++;
    }

    int32 newId(const String& name)
    {
        static HashMap<String, int32> hashMap;
        
        HashMap<String, int32>::iterator it = hashMap.find(name);
        
        if (it != hashMap.end())
        {
            return it->second;
        }

        int32 id = newId();
        hashMap[name] = id;

        return id;
    }

    const EventType uiEVT_NULL = 0;
    const EventType uiEVT_FIRST = 10000;
    const EventType uiEVT_USER_FIRST = uiEVT_FIRST + 2000;

    const EventType uiEVT_LEFT_DOWN = newEventType();
    const EventType uiEVT_LEFT_UP = newEventType();
    const EventType uiEVT_LEFT_CLICK = newEventType();
    const EventType uiEVT_LEFT_DCLICK = newEventType();
    const EventType uiEVT_LEFT_TCLICK = newEventType();

    const EventType uiEVT_RIGHT_DOWN = newEventType();
    const EventType uiEVT_RIGHT_UP = newEventType();
    const EventType uiEVT_RIGHT_CLICK = newEventType();
    const EventType uiEVT_RIGHT_DCLICK = newEventType();
    const EventType uiEVT_RIGHT_TCLICK = newEventType();

    const EventType uiEVT_MIDDLE_DOWN = newEventType();
    const EventType uiEVT_MIDDLE_UP = newEventType();
    const EventType uiEVT_MIDDLE_CLICK = newEventType();
    const EventType uiEVT_MIDDLE_DCLICK = newEventType();
    const EventType uiEVT_MIDDLE_TCLICK = newEventType();

    const EventType uiEVT_MOTION = newEventType();
    const EventType uiEVT_LEAVE_WINDOW = newEventType();
    const EventType uiEVT_ENTER_WINDOW = newEventType();
    const EventType uiEVT_MOUSEWHEEL = newEventType();

    const EventType uiEVT_CHAR = newEventType();
    const EventType uiEVT_KEY_DOWN = newEventType();
    const EventType uiEVT_KEY_UP = newEventType();
    
    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(Event, Object, 0) {}

    Event::Event(int winid /* = 0 */, EventType commandType /* = uiEVT_NULL */)
    {
        id = winid;
        eventType = commandType;
        eventSender = 0;
        userData = 0;
        skipped = false;
        isCmdEvent = false;
        propagationLevel = uiEvent_Propagate_None;
    }

    Event::Event(const Event &rhs)
        : id(rhs.id)
        , eventType(rhs.eventType)
        , eventSender(rhs.eventSender)
        , userData(rhs.userData)
        , skipped(rhs.skipped)
        , isCmdEvent(rhs.isCmdEvent)
        , propagationLevel(rhs.propagationLevel)
    {
    }

    // --------------------------------------------------------
    // MouseEvent
    // --------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(MouseEvent, Event, 0) {}

    MouseEvent::MouseEvent(EventType commandType)
    {
        eventType = commandType;
        leftDown = false;
        rightDown = false;
        middleDown = false;
        wheelDelta = 0;
    }

    void MouseEvent::assign(const MouseEvent& rhs)
    {
        eventType = rhs.eventType;
        leftDown = rhs.leftDown;
        rightDown = rhs.rightDown;
        middleDown = rhs.middleDown;
        wheelDelta = rhs.wheelDelta;
        position = rhs.position;
    }

    // --------------------------------------------------------
    // KeyEvent
    // --------------------------------------------------------
    HARE_IMPLEMENT_ABSTRACT_CLASS(KeyEvent, Event, 0) {}

    KeyEvent::KeyEvent(EventType keyType)
    {
        eventType = keyType;
        keyCode = 0;
        text = 0;
    }

    KeyEvent::KeyEvent(const KeyEvent& rhs)
    {
        eventType = rhs.eventType;
        keyCode = rhs.keyCode;
        text = rhs.text;
    }

    // --------------------------------------------------------
    // Cache of most recently sent events
    // --------------------------------------------------------
    #define EventCacheHashMax 512
    
    struct InternalEventCache
    {
        const EventTable* tablePtr;
        EventType eventType;
        const EventTableEntryBase* entry;
    };
    InternalEventCache eventCache[EventCacheHashMax];

    void UI_API resetEventCache()
    {
        memset(eventCache, 0, sizeof(eventCache));
    }

    // --------------------------------------------------------
    // EventHandler
    // --------------------------------------------------------
    EventHandler::EventHandlerList* EventHandler::pendingEventHandlers = 0;
    
    const EventTable EventHandler::eventTable = 
    { (const EventTable*)0, &EventHandler::eventTableEntries[0] };
    
    const EventTableEntry EventHandler::eventTableEntries[] =
    { HARE_DECLARE_EVENT_TABLE_ENTRY(uiEVT_NULL, 0, 0, 0, NULL) };

    EventHandler::EventHandler()
    {
        nextEvtHandler = 0;
        prevEvtHandler = 0;
        handlerEnabled = true;
        dynamicEvents = 0;
        pendingEvents = 0;
    }

    EventHandler::~EventHandler()
    {
        if (prevEvtHandler)
            prevEvtHandler->nextEvtHandler = nextEvtHandler;

        if (nextEvtHandler)
            nextEvtHandler->prevEvtHandler = prevEvtHandler;

        if (dynamicEvents)
        {
            for (DynamicEventList::iterator it = dynamicEvents->begin();
                it != dynamicEvents->end(); ++it)
            {
                delete *it;
            }
            delete dynamicEvents;
            dynamicEvents = 0;
        }

        if (pendingEvents)
        {
            for (PendingEventList::iterator it = pendingEvents->begin();
                it != pendingEvents->end(); ++it)
            {
                delete *it;
            }
            delete pendingEvents;
            pendingEvents = 0;
        }

        if (pendingEventHandlers)
        {
            pendingEventHandlers->remove(this);
        }
    }

    const EventTable* EventHandler::getEventTable() const
    { 
        return &EventHandler::eventTable; 
    }

    bool EventHandler::handleEvent(Event& event)
    {
        const EventTable* tablePtr = getEventTable();
        uint16 val = uint16(((uint32)tablePtr ^ event.getEventType()) & 0xffff);
        int idx = val & (EventCacheHashMax - 1);
        InternalEventCache* cache = &eventCache[idx];

        if (cache->tablePtr == tablePtr &&
            cache->eventType == event.getEventType())
        {
            // we hit the cache :-)
            const EventTableEntryBase* entry = cache->entry;

            if (!entry)
                return false;

            if (processEventIfMatches(*entry, this, event))
                return true;
        }
        else
        {
            cache->eventType = event.getEventType();
            cache->tablePtr = tablePtr;
            cache->entry = NULL;
            for (; tablePtr != NULL; tablePtr = tablePtr->baseTable)
            {
                assert(tablePtr != tablePtr->baseTable);
                
                const EventTableEntry* entry = (EventTableEntry*)tablePtr->entries;

                while (entry->func)
                {
                    if (entry->eventType == event.getEventType())
                    {
                        cache->entry = entry;
                        if (processEventIfMatches(*entry, this, event))
                            return true;
                    }
                    entry++;
                }
            }
        }

        return false;
    }

    bool EventHandler::processEventIfMatches(const EventTableEntryBase& entry, 
         EventHandler* handler, Event& event)
    {
        int tableId1 = entry.id,
            tableId2 = entry.lastId;

        if ((tableId1 == uiID_Any) ||
            (tableId2 == uiID_Any && tableId1 == event.getId()) ||
            (tableId2 != uiID_Any && (event.getId() >= tableId1 && event.getId() <= tableId2)))
        {
            event.skip(false);
            event.userData = entry.userData;

            (handler->*((EventFunction)(entry.func)))(event);

            if (!event.isSkipped())
                return true;
        }

        return false;
    }

    bool EventHandler::handleDynamicEvent(Event& event)
    {
        for (DynamicEventList::iterator it = dynamicEvents->begin();
            it != dynamicEvents->end(); ++it)
        {
            DynamicEventTableEntry* entry = *it;

            if (entry->eventType == event.getEventType() &&
                entry->func)
            {
                EventHandler* handler = entry->eventSink ? entry->eventSink : this;

                if (processEventIfMatches(*entry, handler, event))
                    return true;
            }
        }

        return false;
    }

    bool EventHandler::processEvent(Event& event)
    {
        if (getEvtHandlerEnabled())
        {
            if (dynamicEvents && handleDynamicEvent(event))
                return true;

            if (handleEvent(event))
                return true;
        }

        if (getNextHandler())
        {
            if (getNextHandler()->processEvent(event))
                return true;
        }

        return tryParent(event);
    }

    bool EventHandler::tryParent(Event& event)
    {
        return false;
    }

    void EventHandler::addPendingEvent(Event& event)
    {
        Event *eventCopy = event.cloneEvent();

        if (!pendingEvents)
            pendingEvents = new PendingEventList;

        pendingEvents->push_back(eventCopy);

        if (!pendingEventHandlers)
            pendingEventHandlers = new EventHandlerList;
        pendingEventHandlers->push_back(this);
    }

    void EventHandler::processPendingEvents()
    {
        size_t n = pendingEvents->size();
        for (PendingEventList::iterator it = pendingEvents->begin();
            it != pendingEvents->end(); ++it)
        {
            Event* event = *it;
            pendingEvents->erase(it);
            processEvent(*event);
            if (--n == 0)
                break;
        }
    }

    void EventHandler::connect(int id, int lastId, int eventType, EventFunction func, 
        void* userData, EventHandler* eventSink)
    {
        DynamicEventTableEntry *entry =
            new DynamicEventTableEntry(eventType, id, lastId, func, userData, eventSink);

        if (!dynamicEvents)
            dynamicEvents = new DynamicEventList;

        dynamicEvents->push_back(entry);
    }

    void EventHandler::disconnect(int id, int lastId, int eventType, EventFunction func, 
        void* userData, EventHandler* eventSink)
    {
        if (!dynamicEvents)
            return;

        for (DynamicEventList::iterator it = dynamicEvents->begin();
            it != dynamicEvents->end(); ++it)
        {
            DynamicEventTableEntry* entry = *it;

            if ((entry->id == id) &&
                ((entry->lastId == lastId) || (lastId == uiID_Any)) &&
                ((entry->eventType == eventType) || (eventType == uiEVT_NULL)) &&
                ((entry->func == func) || (func == 0)) &&
                ((entry->eventSink == eventSink) || (eventSink == 0)) &&
                ((entry->userData == userData) || (userData == 0)))
            {
                delete entry;
                it = dynamicEvents->erase(it);
            }
        }
    }

}