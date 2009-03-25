
namespace hare_ui
{
    typedef int EventType;

    class EventTable;
    class EventTableEntry;

    typedef void (EventHandler::*EventFunction)(Event&);

    extern EventType newEventType();

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

        void connect();

        void disconnect();

    protected:
        EventHandler* nextEvtHandler;
        EventHandler* prevEvtHandler;

        bool handlerEnabled;

        static const EventTable eventTable;
    };

    extern UI_API const EventType uiEVT_NULL;
    extern UI_API const EventType uiEVT_FIRST;
    extern UI_API const EventType uiEVT_USER_FIRST;

    class UI_API Event
    {

    };

#define HARE_DECLARE_EVENT_TABLE_ENTRY(type, winid, idLast, fn, obj) \
    EventTableEntry(type, winid, idLast, fn, obj)

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

}


