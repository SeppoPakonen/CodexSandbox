#ifndef _Core_ECS_h_
#define _Core_ECS_h_



class EcsGenericSystem {
	
public:
    virtual void Initialize() = 0;
    virtual void Start() = 0;
    virtual void Update(float /*dt*/) = 0;
    virtual void Stop() = 0;
    virtual void Uninitialize() = 0;
};

enum {
	UNKNOWN,
	LEFT_DOWN,
	LEFT_DOUBLE,
	LEFT_TRIPLE,
	LEFT_DRAG,
	LEFT_HOLD,
	LEFT_REPEAT,
	LEFT_UP,
	RIGHT_DOWN,
	RIGHT_DOUBLE,
	RIGHT_TRIPLE,
	RIGHT_DRAG,
	RIGHT_HOLD,
	RIGHT_REPEAT,
	RIGHT_UP,
	MIDDLE_DOWN,
	MIDDLE_DOUBLE,
	MIDDLE_TRIPLE,
	MIDDLE_DRAG,
	MIDDLE_HOLD,
	MIDDLE_REPEAT,
	MIDDLE_UP,
};

enum {
	EVENT_INVALID,
	EVENT_SHUTDOWN,
};


struct Event {
	int type = 0;
};

class Screen : public EcsGenericSystem {
	
	
public:
	virtual ~Screen() {}
	
	virtual bool Poll(Event& e) = 0;
	virtual void Render() = 0;
	
};

#endif
