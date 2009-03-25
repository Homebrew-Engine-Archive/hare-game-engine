
class SizerItem
{

};

class Sizer
{
public:
    Sizer();
    virtual ~Sizer();

public:
    SizerItem* add(Window *window);
    SizerItem* addSpacer(int size);
    SizerItem* addStretchSpacer(int prop = 1);

    SizerItem* insert();
    SizerItem* insertSpacer(size_t index, int size);
    SizerItem* insertStretchSpacer(size_t index, int prop = 1);

    SizerItem* prepend();
    SizerItem* prependSpacer();
    SizerItem* prependStretchSpacer();

    void setContainerWindow(Window *window);
    
    Window *getContainerWindow() const 
    { 
        return containerWindow; 
    }
    
protected:
    Window* containerWindow;
};


class BoxSizer : public Sizer
{
public:
    BoxSizer(int orientation);

    void recalcSizes();
    wxSize calcMin();

    int getOrientation() const
    { 
        return orient; 
    }

    void setOrientation(int orientation)
    { 
        orient = orientation; 
    }

protected:
    int orient;
    int stretchable;
    int minWidth;
    int minHeight;
    int fixedWidth;
    int fixedHeight;
};