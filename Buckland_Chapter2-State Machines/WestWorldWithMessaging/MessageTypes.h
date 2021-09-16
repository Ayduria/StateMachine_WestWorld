#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_GreetDrunkard,
  Msg_GreetMiner,
  Msg_ProvokeMiner,
  Msg_HurtDrunkard,
  Msg_KillDrunkard
};


inline std::string MsgToStr(int msg)
{
	switch (msg)
	{
	case 0:

		return "HiHoneyImHome";

	case 1:

		return "StewReady";

	case 2:

		return "GreetDrunkard";

	case 3:

		return "GreetMiner";
	
	case 4:

		return "ProvokeMiner";

	case 5:

		return "HurtDrunkard";
	case 6:

		return "KillDrunkard";

	default:

		return "Not recognized!";
	}
}

#endif