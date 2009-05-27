%module hare

class SoundParam : public Object
{
public:
	SoundParam();
	virtual ~SoundParam();

	String  fileName;

	float   innerCone;
	float   outerCone;
	float   minGain;
	float   maxGain;
	float   refDistance;
	float   maxDistance;
	float   pitch;
	float   gain;
	float   outerGain;
	float   rolloff;
	bool    bFromStream;
};

class SoundBuffer : public Object
{
public:
	SoundBuffer();
	virtual ~SoundBuffer();

	void setSoundParam(SoundParam* param);

	virtual void load() = 0;

	virtual void unload() = 0;

	virtual void update() = 0;
};

class SoundData : public Object
{
public:
	SoundData();
	virtual ~SoundData();

	void setSoundParam(SoundParam* param);

	SoundBuffer* getSoundBuffer();

	virtual void postLoaded();
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

	void setSoundData(SoundData* data);
	void setPosition(const Vector2& position);
	void setDirection(const Vector2& direction);
	void setVelocity(const Vector2& velocity);
	void setVolume(float val);
     
	SoundData* getSoundData();
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
		
	virtual SoundBuffer* createSoundBuffer() = 0;

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
