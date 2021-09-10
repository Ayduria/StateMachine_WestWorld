#include "DrunkardOwnedStates.h"
#include "MinerOwnedStates.h"
#include "Drunkard.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//-----------------------------------------------------------------------Global state

DrunkardGlobalState* DrunkardGlobalState::Instance()
{
    static DrunkardGlobalState instance;

    return &instance;
}


void DrunkardGlobalState::Execute(Drunkard* drunkard)
{
    //1 in 10 chance of needing the bathroom (provided she is not already
    //in the bathroom)
    if ((RandFloat() < 0.1) &&
        !drunkard->GetFSM()->isInState(*VisitBathroomC::Instance()))
    {
        drunkard->GetFSM()->ChangeState(VisitBathroomC::Instance());
    }
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_HiHoneyImHome:
    {
        cout << "\nMessage handled by " << GetNameOfEntity(drunkard->ID()) << " at time: "
            << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(drunkard->ID()) <<
            ": Hi honey. Let me make you some of mah fine country stew";

        drunkard->GetFSM()->ChangeState(CookStewC::Instance());
    }

    return true;

    }//end switch

    return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoHouseWorkC* DoHouseWorkC::Instance()
{
    static DoHouseWorkC instance;

    return &instance;
}


void DoHouseWorkC::Enter(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Time to do some more housework!";
}


void DoHouseWorkC::Execute(Drunkard* drunkard)
{
    switch (RandInt(0, 2))
    {
    case 0:

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Moppin' the floor";

        break;

    case 1:

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Washin' the dishes";

        break;

    case 2:

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Makin' the bed";

        break;
    }
}

void DoHouseWorkC::Exit(Drunkard* drunkard)
{
}

bool DoHouseWorkC::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroomC* VisitBathroomC::Instance()
{
    static VisitBathroomC instance;

    return &instance;
}


void VisitBathroomC::Enter(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Walkin' to the can. Need to powda mah pretty li'lle nose";
}


void VisitBathroomC::Execute(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Ahhhhhh! Sweet relief!";

    drunkard->GetFSM()->RevertToPreviousState();
}

void VisitBathroomC::Exit(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Leavin' the Jon";
}


bool VisitBathroomC::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    return false;
}


//------------------------------------------------------------------------CookStew

CookStewC* CookStewC::Instance()
{
    static CookStewC instance;

    return &instance;
}


void CookStewC::Enter(Drunkard* drunkard)
{
    //if not already cooking put the stew in the oven
    if (!drunkard->Cooking())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Putting the stew in the oven";

        //send a delayed message myself so that I know when to take the stew
        //out of the oven
        Dispatch->DispatchMessage(1.5,                  //time delay
            drunkard->ID(),           //sender ID
            drunkard->ID(),           //receiver ID
            Msg_StewReady,        //msg
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(true);
    }
}


void CookStewC::Execute(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Fussin' over food";
}

void CookStewC::Exit(Drunkard* drunkard)
{
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Puttin' the stew on the table";
}


bool CookStewC::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_StewReady:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": StewReady! Lets eat";

        //let hubby know the stew is ready
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_StewReady,
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(false);

        drunkard->GetFSM()->ChangeState(DoHouseWorkC::Instance());
    }

    return true;

    }//end switch

    return false;
}


//------------------------------------------------------------------------SleepInRoom

SleepInRoom* SleepInRoom::Instance()
{
    static SleepInRoom instance;

    return &instance;
}


void SleepInRoom::Enter(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Anotha' day ova' in thas shaty town";
}


void SleepInRoom::Execute(Drunkard* drunkard)
{
    //if miner is not fatigued start to dig for nuggets again.
    if (!drunkard->Fatigued())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "Am too soba, me need to drank!";

        drunkard->GetFSM()->ChangeState(LivingHisBestLife::Instance());
    }
    else
    {
        //sleep
        drunkard->DecreaseFatigue();

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": " << "ZZZz... Beeuuurgh... Fack'in shiet... ZZZz... ";
    }
}

void SleepInRoom::Exit(Drunkard* drunkard)
{
}


bool SleepInRoom::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------LivingHisBestLife

LivingHisBestLife* LivingHisBestLife::Instance()
{
    static LivingHisBestLife instance;

    return &instance;
}


void LivingHisBestLife::Enter(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Gima tha cheapast drank u gat!";
}


void LivingHisBestLife::Execute(Drunkard* drunkard)
{
    //if miner is not fatigued start to dig for nuggets again.
    if (drunkard->Drunked())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "Ah shiet! I'm startang to see doubla!";

        drunkard->GetFSM()->ChangeState(Drunkenness::Instance());
    }
    else
    {
        //drink
        drunkard->IncreaseDrunk();

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": " << "I gat sa'm mare in ma! Gim'me anatha one!";
    }
}

// ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!!
// ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!!
// ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!! ALLO ON EST RENDU LÀ !!!

void LivingHisBestLife::Exit(Drunkard* drunkard)
{
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Puttin' the stew on the table";
}


bool LivingHisBestLife::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_StewReady:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": StewReady! Lets eat";

        //let hubby know the stew is ready
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_StewReady,
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(false);

        drunkard->GetFSM()->ChangeState(DoHouseWorkC::Instance());
    }

    return true;

    }//end switch

    return false;
}

//------------------------------------------------------------------------Drunkenness

Drunkenness* Drunkenness::Instance()
{
    static Drunkenness instance;

    return &instance;
}


void Drunkenness::Enter(Drunkard* drunkard)
{
    //if not already cooking put the stew in the oven
    if (!drunkard->Cooking())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Putting the stew in the oven";

        //send a delayed message myself so that I know when to take the stew
        //out of the oven
        Dispatch->DispatchMessage(1.5,                  //time delay
            drunkard->ID(),           //sender ID
            drunkard->ID(),           //receiver ID
            Msg_StewReady,        //msg
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(true);
    }
}


void Drunkenness::Execute(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Fussin' over food";
}

void Drunkenness::Exit(Drunkard* drunkard)
{
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Puttin' the stew on the table";
}


bool Drunkenness::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_StewReady:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": StewReady! Lets eat";

        //let hubby know the stew is ready
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_StewReady,
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(false);

        drunkard->GetFSM()->ChangeState(DoHouseWorkC::Instance());
    }

    return true;

    }//end switch

    return false;
}

//------------------------------------------------------------------------ProvocativeBehaviour

ProvocativeBehaviour* ProvocativeBehaviour::Instance()
{
    static ProvocativeBehaviour instance;

    return &instance;
}


void ProvocativeBehaviour::Enter(Drunkard* drunkard)
{
    //if not already cooking put the stew in the oven
    if (!drunkard->Cooking())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Putting the stew in the oven";

        //send a delayed message myself so that I know when to take the stew
        //out of the oven
        Dispatch->DispatchMessage(1.5,                  //time delay
            drunkard->ID(),           //sender ID
            drunkard->ID(),           //receiver ID
            Msg_StewReady,        //msg
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(true);
    }
}


void ProvocativeBehaviour::Execute(Drunkard* drunkard)
{
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Fussin' over food";
}

void ProvocativeBehaviour::Exit(Drunkard* drunkard)
{
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Puttin' the stew on the table";
}


bool ProvocativeBehaviour::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_StewReady:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": StewReady! Lets eat";

        //let hubby know the stew is ready
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_StewReady,
            NO_ADDITIONAL_INFO);

        drunkard->SetCooking(false);

        drunkard->GetFSM()->ChangeState(DoHouseWorkC::Instance());
    }

    return true;

    }//end switch

    return false;
}