#include "PCH.h"
#include "Event.h"
#include "Window.h"

namespace hare_ui
{
    EventType newEventType()
    {
        static int lastUsedEventType = uiEVT_FIRST;
        return lastUsedEventType++;
    }

    const EventType uiEVT_NULL = 0;
    const EventType uiEVT_FIRST = 10000;
    const EventType uiEVT_USER_FIRST = uiEVT_FIRST + 2000;
    
    // --------------------------------------------------------
    // Event
    // --------------------------------------------------------
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

    HARE_IMPLEMENT_DYNAMIC_CLASS(EventHandler, Object, 0)
    {
    }

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
        u16 val = u16(((u32)tablePtr ^ event.getEventType()) & 0xffff);
        int idx = val & (EventCacheHashMax - 1);
        InternalEventCache* cache = &eventCache[idx];

        if (cache->tablePtr == tablePtr &&
            cache->eventType == event.getEventType())
        {
            // we hit the cache :-)
            const EventTableEntryBase* entry = cache->entry;
            if (processEventIfMatches(*entry, this, event))
                return true;
        }
        else
        {
            cache->eventType = event.getEventType();
            cache->tablePtr = tablePtr;
            for (; tablePtr != NULL; tablePtr = tablePtr->baseTable)
            {
                assert(tablePtr != tablePtr->baseTable);
                
                const EventTableEntryBase* entry = tablePtr->entries;

                while (entry->func)
                {
                    cache->entry = entry;
                    if (processEventIfMatches(*entry, this, event))
                        return true;
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

    //void processPendingEvents();

    //void connect();

    //void disconnect();

}