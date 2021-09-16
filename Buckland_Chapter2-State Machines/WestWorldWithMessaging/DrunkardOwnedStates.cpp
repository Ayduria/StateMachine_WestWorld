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
    if ((RandFloat() < 0.5) &&
        !drunkard->GetFSM()->isInState(*SleepInRoom::Instance()))
    {
        drunkard->IncreaseFatigue();
    }

    if (drunkard->Fatigued() &&
        !drunkard->GetFSM()->isInState(*ProvocativeBehaviour::Instance()))
    {
        drunkard->GetFSM()->ChangeState(SleepInRoom::Instance());
    }
}

bool DrunkardGlobalState::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
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
        drunkard->GetFSM()->ChangeState(Drunkenness::Instance());
    }
    else
    {
        //drink
        drunkard->IncreaseDrunk();

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": " << "I gat sa'm mare in ma! Gim'me anatha one!";
    }
}

void LivingHisBestLife::Exit(Drunkard* drunkard)
{
}


bool LivingHisBestLife::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_GreetDrunkard:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Ay budda, if it isn't ma drankin' part'na!";

        //Drunkard greet the miner
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_GreetMiner,
            NO_ADDITIONAL_INFO);
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
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": tha drink is marvaleus ! Tha naght onla began !";
}


void Drunkenness::Execute(Drunkard* drunkard)
{
    SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    if (drunkard->Fatigued())
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "thas enough fer today, me guts ned same rest.";
        drunkard->GetFSM()->ChangeState(ProvocativeBehaviour::Instance());
    }

    double r = RandFloat(); 

    if (r < 0.33)
    {

        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "Ah shiet! I'm startang to see doubla!";
    }
    else if (r < 0.66)
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "Ah' could stay hare forava!";
    }
    else
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": "
            << "Ma fatha always thold ma to stay away from thas devil's drink... I naver listened !";
    }
}

void Drunkenness::Exit(Drunkard* drunkard)
{
}


bool Drunkenness::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_GreetDrunkard:
    {
        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": That's it lad, ima break ya neck!";

        //let hubby know the stew is ready
        Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
            drunkard->ID(),
            ent_Miner_Bob,
            Msg_ProvokeMiner,
            NO_ADDITIONAL_INFO);

        drunkard->GetFSM()->ChangeState(ProvocativeBehaviour::Instance());
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
}


void ProvocativeBehaviour::Execute(Drunkard* drunkard)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    
    double f = RandFloat();

    if (f < 0.33)
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << " takes a bottle and smashes it on " << GetNameOfEntity(ent_Miner_Bob) << "'s forehead.";
    }
    else if (f < 0.66)
    {
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << " jumps on the nearest table and drop kicks " << GetNameOfEntity(ent_Miner_Bob) << " in the chest.";
    }
    else
    {
        cout << "\nby unknown means, " << GetNameOfEntity(drunkard->ID()) << " cast a fire ball upon " << GetNameOfEntity(ent_Miner_Bob) << " and flames engulf the salon!";
    }
}

void ProvocativeBehaviour::Exit(Drunkard* drunkard)
{
}


bool ProvocativeBehaviour::OnMessage(Drunkard* drunkard, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

        cout << "\nMessage received by " << GetNameOfEntity(drunkard->ID()) <<
            " at time: " << Clock->GetCurrentTime();
    switch (msg.Msg)
    {
    case Msg_HurtDrunkard:
    {
        SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": Tis ain't the last you'll see of me! ya bastard...";

        drunkard->GetFSM()->ChangeState(SleepInRoom::Instance());
    }
    case Msg_KillDrunkard:
    {
        SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << ": BLarGH...!";
        
        SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        cout << "\n" << GetNameOfEntity(drunkard->ID()) << " lies on the ground, pickaxe through the chest.";

        //delete drunkard;
    }


    return true;

    }//end switch

    return false;
}