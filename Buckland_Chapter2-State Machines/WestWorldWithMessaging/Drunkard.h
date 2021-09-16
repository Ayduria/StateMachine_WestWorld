#ifndef DRUNKARD_H
#define DRUNKARD_H
//------------------------------------------------------------------------
//
//  Name: Drunkard.h
//
//  Desc: class to implement the drunkard.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//  and Vincent Blais, Ann-Laurie Lapointe et Francis Tremblay
//
//------------------------------------------------------------------------

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "DrunkardOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

//above this value a miner is thirsty
const int DrunkLevel = 3;
//above this value a drunkard is sleepy
const int DrunkardTirednessThreshold = 5;

class Drunkard : public BaseGameEntity
{
private:

    //an instance of the state machine class
    StateMachine<Drunkard>* m_pStateMachine;

    location_type   m_Location;

    //the higher the value, the more tired the drunkard
    int             m_iFatigue;

    //the higher the value, the more drunk the drunkard
    int             m_iDrunk;


public:

    Drunkard(int id) :m_Location(saloon),
        m_iFatigue(0),
        m_iDrunk(0),
        BaseGameEntity(id)

    {
        //set up the state machine
        m_pStateMachine = new StateMachine<Drunkard>(this);

        m_pStateMachine->SetCurrentState(LivingHisBestLife::Instance());

        m_pStateMachine->SetGlobalState(DrunkardGlobalState::Instance());
    }

    ~Drunkard() { delete m_pStateMachine; }


    //this must be implemented
    void          Update();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);

    StateMachine<Drunkard>* GetFSM()const { return m_pStateMachine; }

    //----------------------------------------------------accessors
    location_type Location()const { return m_Location; }
    void          ChangeLocation(location_type loc) { m_Location = loc; }

    bool          Fatigued()const;
    void          DecreaseFatigue() { m_iFatigue -= 1; }
    void          IncreaseFatigue() { m_iFatigue += 1; }

    bool          Drunked()const;
    void          DecreaseDrunk() { m_iDrunk -= 1; }
    void          IncreaseDrunk() { m_iDrunk += 1; }
};

#endif
