
class ThemePackage
{


protected:
    Theme::Array themes;
};

class Theme : public Object
{
    HARE_DECLARE_ABSTRACT_CLASS(Theme)
public:
    virtual void drawWindow(Window* window) = 0;

    virtual bool canHandle(Window* window)
    {
        return false;
    }

protected:
    String name;
};