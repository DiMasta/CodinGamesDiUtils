//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

class Collision {
public:
	Collision();

	Collision(
		Entity* entityA,
		Entity* entityB,
		float time
	);

	~Collision();

	Entity* getEntityA() const {
		return entityA;
	}

	Entity* getEntityB() const {
		return entityB;
	}

	float getTime() const {
		return time;
	}

	void setEntityA(Entity* entityA) { this->entityA = entityA; }
	void setEntityB(Entity* entityB) { this->entityB = entityB; }
	void setTime(float time) { this->time = time; }

private:
	Entity* entityA;
	Entity* entityB;
	float time;
};

//*************************************************************************************************************
//*************************************************************************************************************

Collision::Collision() :
	entityA(nullptr),
	entityB(nullptr),
	time(TURN_START_TIME)
{
}

//*************************************************************************************************************
//*************************************************************************************************************

Collision::Collision(
	Entity* entityA,
	Entity* entityB,
	float time
) :
	entityA(entityA),
	entityB(entityB),
	time(time)
{

}

//*************************************************************************************************************
//*************************************************************************************************************

Collision::~Collision() {
}