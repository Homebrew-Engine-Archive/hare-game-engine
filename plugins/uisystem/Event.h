
namespace hare_ui
{
    typedef int EventType;

    typedef void (EventHandler::*EventFunction)(Event&);

    extern EventType newEventType();

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
            : id(rhs.m_id),
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

        void connect();

        void disconnect();

    protected:
        EventHandler* nextEvtHandler;
        EventHandler* prevEvtHandler;

        bool handlerEnabled;

        static const EventTable eventTable;
    };

    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
    extern UI_API const EventType uiEVT_NULL;
    extern UI_API const EventType uiEVT_FIRST;
    extern UI_API const EventType uiEVT_USER_FIRST;

    class UI_API Event
    {
    private:
        Event& operator=(const Event&);
    protected:
        Event(const Event&);

    public:
        Event(int winid = 0, EventType commandType = uiEVT_NULL);

        void setEventType(EventType type) { eventType = type; }
        EventType getEventType() const { return eventType; }
        void* getEventSender() const { return eventSender; }
        void setEventSender(void* sender) { eventSender = sender; }
        int getId() const { return id; }
        void setId(int Id) { id = Id; }

        void skip(bool skip = true) { skipped = skip; }
        bool isSkipped() const { return skipped; }

        virtual Event *clone() const = 0;

        bool isCommandEvent() const { return isCmdEvent; }

        bool shouldPropagate() const
        { return propagationLevel != uiEVENT_PROPAGATE_NONE; }

        int stopPropagation()
        {
            int oldLevel = propagationLevel;
            propagationLevel = uiEVENT_PROPAGATE_NONE;
            return oldLevel;
        }

        void resumePropagation(int level)
        {
            propagationLevel = level;
        }

    protected:
        void* eventSender;
        EventType eventType;
        int id;

    protected:
        int propagationLevel;

    protected:
        bool skipped;
        bool isCmdEvent;
    };

    class UI_API MouseEvent : public Event
    {
    public:
        PointF getPosition() const;

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


