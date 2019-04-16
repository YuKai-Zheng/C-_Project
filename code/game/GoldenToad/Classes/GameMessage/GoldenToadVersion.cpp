#include "GoldenToadVersion.h"
#include "GoldenToadEnums.h"
#include "GoldenToadMessageHead.h"

using namespace Enums;
using namespace GoldenToad;

namespace Version {

	UINT GoldenToadVersion::getGameVersionName() {
		if (canPrintDebugLog) {
			//CCLOG("CURRENT GAME NAME ID:%u", GAME_VERSION_NAME);
		}
		return GAME_VERSION_NAME;
	}

	void GoldenToadVersion::setGameVersionName(UINT value) {
		GAME_VERSION_NAME = value;
	}

}
