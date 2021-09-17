#include "Drunkard.h"

bool Drunkard::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Drunkard::Update()
{
	//set text color to green
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_pStateMachine->Update();
}

bool Drunkard::Fatigued()const
{
	if (m_iFatigue > DrunkardTirednessThreshold)
	{
		return true;
	}

	return false;
}
bool Drunkard::ReadyToLeaveBed()const
{
	return m_iFatigue <= 0;
}

bool Drunkard::Drunked() const
{
	return m_iDrunk > DrunkLevel;
}