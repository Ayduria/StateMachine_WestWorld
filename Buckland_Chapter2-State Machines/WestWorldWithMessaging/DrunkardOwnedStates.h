#ifndef DRUNKARD_OWNED_STATES_H
#define DRUNKARD_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinersWifeOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunkard class
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//	and Vincent Blais, Ann-Laurie Lapointe et Francis Tremblay
//
//------------------------------------------------------------------------
#include "fsm/State.h"

class Drunkard;



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class DrunkardGlobalState : public State<Drunkard>
{
private:

	DrunkardGlobalState() {}

	//copy ctor and assignment should be private
	DrunkardGlobalState(const DrunkardGlobalState&);
	DrunkardGlobalState& operator=(const DrunkardGlobalState&);

public:

	//this is a singleton
	static DrunkardGlobalState* Instance();

	virtual void Enter(Drunkard* drunkard) {}

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard) {}

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};


//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class DoHouseWorkC : public State<Drunkard>
{
private:

	DoHouseWorkC() {}

	//copy ctor and assignment should be private
	DoHouseWorkC(const DoHouseWorkC&);
	DoHouseWorkC& operator=(const DoHouseWorkC&);

public:

	//this is a singleton
	static DoHouseWorkC* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);

};



//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class VisitBathroomC : public State<Drunkard>
{
private:

	VisitBathroomC() {}

	//copy ctor and assignment should be private
	VisitBathroomC(const VisitBathroomC&);
	VisitBathroomC& operator=(const VisitBathroomC&);

public:

	//this is a singleton
	static VisitBathroomC* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);

};


//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class CookStewC : public State<Drunkard>
{
private:

	CookStewC() {}

	//copy ctor and assignment should be private
	CookStewC(const CookStewC&);
	CookStewC& operator=(const CookStewC&);

public:

	//this is a singleton
	static CookStewC* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class SleepInRoom : public State<Drunkard>
{
private:

	SleepInRoom() {}

	//copy ctor and assignment should be private
	SleepInRoom(const SleepInRoom&);
	SleepInRoom& operator=(const SleepInRoom&);

public:

	//this is a singleton
	static SleepInRoom* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class LivingHisBestLife : public State<Drunkard>
{
private:

	LivingHisBestLife() {}

	//copy ctor and assignment should be private
	LivingHisBestLife(const LivingHisBestLife&);
	LivingHisBestLife& operator=(const LivingHisBestLife&);

public:

	//this is a singleton
	static LivingHisBestLife* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class Drunkenness : public State<Drunkard>
{
private:

	Drunkenness() {}

	//copy ctor and assignment should be private
	Drunkenness(const Drunkenness&);
	Drunkenness& operator=(const Drunkenness&);

public:

	//this is a singleton
	static Drunkenness* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};

//------------------------------------------------------------------------
//

//------------------------------------------------------------------------
class ProvocativeBehaviour : public State<Drunkard>
{
private:

	ProvocativeBehaviour() {}

	//copy ctor and assignment should be private
	ProvocativeBehaviour(const ProvocativeBehaviour&);
	ProvocativeBehaviour& operator=(const ProvocativeBehaviour&);

public:

	//this is a singleton
	static Drunkenness* Instance();

	virtual void Enter(Drunkard* drunkard);

	virtual void Execute(Drunkard* drunkard);

	virtual void Exit(Drunkard* drunkard);

	virtual bool OnMessage(Drunkard* drunkard, const Telegram& msg);
};


#endif