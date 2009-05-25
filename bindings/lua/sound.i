%module hare

class SoundBuffer : public Object
{
public:
	SoundBuffer();
	SoundBuffer(const String& name);
	virtual ~SoundBuffer();

	virtual void load(bool bStream = false) = 0;

	virtual void unload() = 0;

	virtual void update() = 0;

	const String& getFileName();

public:
	void setCone(float inner, float outer);
	void setGain(float gain, float outerGain,
		float min = 0.0f, float max = 1.0f);
	void setDistance(float ref, float max);
	void setPitch(float pitch);
	void setRolloffFactor(float rolloff);

	float getGain() const;
	float getGainOuter() const;
	float getGainMin() const;
	float getGainMax() const;
	float getRolloff() const;
	float getReferenceDistance() const;
	float getMaxDistance() const;
	float getPitch() const;
	float getInnerCone() const;
	float getOuterCone() const;
};

class SoundObject : public Object
{
public:
	SoundObject();
	virtual ~SoundObject();
	
	virtual void play() = 0;
	virtual void stop() = 0;
	virtual void pause()= 0;
	virtual void resume()=0;
	virtual void setLopping(bool state) = 0;
	virtual void update() = 0;
	virtual bool getPlaying() = 0;

	virtual void postEdited(Attribute *attr);

	virtual void postLoaded();

	void setSoundBuffer(SoundBuffer* buffer);
	void setPosition(const Vector2& position);
	void setDirection(const Vector2& direction);
	void setVelocity(const Vector2& velocity);
	void setVolume(float val);
     
	SoundBuffer* getSoundBuffer();
	const Vector2& getPosition();
	const Vector2& getDirection();
	const Vector2& getVelocity();
	float getVolume();
	bool  getLooping() const;
	float getPrecomputedDistanceSquared();
};

class SoundListener
{	
public:
	SoundListener();
	virtual ~SoundListener();
		
	virtual void update() = 0;

	void setPosition(const Vector2& position);

	void setVelocity(const Vector2& velocity);

	void setGain(float gain);

	Vector2 getPosition() const;

	Vector2 getVelocity() const;

	float getGain() const;
};

class SoundManager : public Object
{
public:
	SoundManager();
	virtual ~SoundManager();

	virtual void initialize() = 0;

	virtual void release() = 0;

	virtual void update() = 0;

	virtual SoundObject* createSoundObject() = 0;
		
	virtual SoundBuffer* createSoundBuffer(const String& name) = 0;

	virtual SoundBuffer* getSoundBuffer(const String& name);

	virtual SoundListener* getSoundListener();

	bool registSoundBuffer(SoundBuffer* buffer);
         
	bool registSoundObject(SoundObject* object);

	bool unregistSoundBuffer(SoundBuffer* buffer);

	bool unregistSoundObject(SoundObject* object);
};

class SoundApp
{	
public:
	SoundManager* getSoundManager();

};

SoundApp* getSoundApp();
