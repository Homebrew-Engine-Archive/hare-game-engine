
enum uiAlignment
{
    uiALIGN_DEFAULT           = 0x0000,
    uiALIGN_CENTER_HORIZONTAL = 0x0100,
    uiALIGN_LEFT              = uiALIGN_DEFAULT,
    uiALIGN_TOP               = uiALIGN_DEFAULT,
    uiALIGN_RIGHT             = 0x0200,
    uiALIGN_BOTTOM            = 0x0400,
    uiALIGN_CENTER_VERTICAL   = 0x0800,

    uiALIGN_CENTER            = (uiALIGN_CENTER_HORIZONTAL | uiALIGN_CENTER_VERTICAL),

    uiALIGN_MASK              = 0x0F00
};

enum uiDirection
{
    uiLEFT                  = 0x0010,
    uiRIGHT                 = 0x0020,
    uiUP                    = 0x0040,
    uiDOWN                  = 0x0080,

    uiTOP                   = uiUP,
    uiBOTTOM                = uiDOWN,

    uiNORTH                 = uiUP,
    uiSOUTH                 = uiDOWN,
    uiWEST                  = uiLEFT,
    uiEAST                  = uiRIGHT,

    uiALL                   = uiUP | uiDOWN | uiRIGHT | uiLEFT,
};

enum uiOrientation
{
    uiHORIZONTAL            = 0x0004,
    uiVERTICAL              = 0x0008,
    uiBOTH                  = uiVERTICAL | uiHORIZONTAL,
};

enum uiStretch
{
    uiEXPAND                = 0x2000,
    uiSHAPED                = 0x4000,
    uiFIXED_MINSIZE         = 0x8000,
};

class SizerItem : public Object
{
    HARE_DECLARE_ABSTRACT_CLASS(SizerItem)
public:
    SizerItem(int prop, int flg, int bdr);
    virtual ~SizerItem();

    SizeF getMinSize() const;
    SizeF getMinSizeWithBorder() const;

    void setProportion(int value)
    { proportion = value; }
    int getProportion() const
    { return proportion; }
    void setFlag(int value)
    { flag = value; }
    int getFlag() const
    { return flag; }
    void setBorder(int value)
    { border = value; }
    int getBorder() const
    { return border; }

    virtual SizeF getSize() const = 0;
    virtual SizeF calcMinSize() = 0;
    virtual bool isShown() = 0;

    void setDimension(const PointF& pos, const SizeF& size);

protected:
    int proportion;
    int border;
    int flag;

    PointF pos;
    SizeF minSize;
    RectF rect;
};

class SizerItemWindow : public SizerItem
{
    HARE_DECLARE_DYNAMIC_CLASS(SizerItemWindow)
public:
    SizerItemWindow(Window* win, int prop, int flg, int bdr);
    virtual ~SizerItemWindow();

    virtual SizeF getSize() const;
    virtual SizeF calcMinSize();
protected:
    Window* window;
};

class SizerItemSizer : public SizerItem
{
    HARE_DECLARE_DYNAMIC_CLASS(SizerItemSizer)
public:
    SizerItemSizer(Sizer* szr, int prop, int flg, int bdr);
    virtual ~SizerItemSizer();

    virtual SizeF getSize() const;
    virtual SizeF calcMinSize();
protected:
    Sizer* sizer;
};

class SizerItemSpacer : public SizerItem
{
    HARE_DECLARE_DYNAMIC_CLASS(SizerItemSpacer)
public:
    SizerItemSpacer(f32 w, f32 h, int prop, int flg, int bdr);
    virtual ~SizerItemSpacer();

    virtual SizeF getSize() const;
    virtual SizeF calcMinSize();
protected:
    SizerSpacer* spacer;
};

class SizerSpacer
{

};

class Sizer : public Object
{
    HARE_DECLARE_ABSTRACT_CLASS(Sizer)
public:
    Sizer();
    virtual ~Sizer();

public:
    SizerItem* add(Window *window, 
                   int proportion = 0,
                   int flag = 0,
                   int border = 0);

    SizerItem* add(Sizer *sizer,
                   int proportion = 0,
                   int flag = 0,
                   int border = 0);

    SizerItem* add(int width,
                   int height,
                   int proportion = 0,
                   int flag = 0,
                   int border = 0);

    SizerItem* insert(size_t index,
                      Window *window, 
                      int proportion = 0,
                      int flag = 0,
                      int border = 0);

    SizerItem* insert(size_t index,
                      Sizer *sizer,
                      int proportion = 0,
                      int flag = 0,
                      int border = 0);

    SizerItem* insert(size_t index,
                      int width,
                      int height,
                      int proportion = 0,
                      int flag = 0,
                      int border = 0);

    virtual SizerItem* insert(size_t index, SizerItem* item);

    void setContainerWindow(Window *window);
    
    Window *getContainerWindow() const 
    { 
        return containerWindow;
    }

    SizerItem::List& getChildren()
    {
        return items;
    }

    virtual void layout();

public:
    virtual void recalcSizes() = 0;

    virtual SizeF calcMin() = 0;
    
protected:
    Window* containerWindow;

    SizerItem::List items;
    SizeF size;
    SizeF minSize;
    PointF position;
};