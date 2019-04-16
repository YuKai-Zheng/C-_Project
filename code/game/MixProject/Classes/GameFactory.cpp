#include "GameFactory.h"
#include "HNPlatform/HNGameCreator.h"
#include "VersionInfo/VersionInfo.h"

#include "FruitSlotGameTableUI.h"
#include "OnePieceSlotGameTableUI.h"

//v88
#include "../v88/CSLLGameTableUIV88.h"
#include "../v88/NNGameTableUIv88.h"
#include "../v88/BRNNGameTableUIV88.h"
//#include "../v88/BBGameTableUIV88.h"
#include "../v99/BBGameTableUIV99.h"
#include "../v88/GoldenToadGameTableUIV88.h"
#include "../v88/ThirteenZhangGameTableUIv88.h"
#include "../v88/BJLGameTableUIV88.h"
#include "../v99/FQZSGameTableUIV99.h"
#include "../v88/GoldenFlowerGameTableUIV88.h"
#include "../v88/BRXJGameTableUIV88.h"
#include "../v88/FruitGameTableUIV88.h"
#include "../v88/TurnTablreGameTableUIV88.h"
#include "../v88/SHZGameTableUIV88.h"
#include "../v88/GameTable/StudGameTableUIV88.h"
#include "../v88/NLGameTableUIV88.h"
#include "../v88/JSXSGameTableUIV88.h"
#include "../v88/MedusaGameTableUIV88.h"
#include "../v88/TigerGameTableUIV88.h"
#include "../v88/HonorGameTableUIV88.h"

#include "Pubg/Classes/v88/PubgGameTableUIV88.h"
#include "EightSlotGameTableUI.h"
#include "RedPacketGameTableUI.h"
#include "../v99/D21GameTableUIV99.h"

#ifdef FILTER_GAME
//#include "D21GameTableUI.h"
//#include "NewYearGameTableUI.h"
//#include "LEGameTableUI.h"
#endif

GAME_CREATE_SELECTOR GameFactory::create(GameType type) {
    switch (type) {
        case Likui_GAME:
        case GoldenToad_GAME:
        case Monkey_GAME: {
            return createGoldenToadGame();
        }
        break;
        case STUD_GAME: {
            return createStudGame();
        }
        break;
        case TWO_NN_GAME:
        case FOUR_NN_NAME:
        case SIX_NN_GAME: {
            return createNNGame();
        }
        break;
        case BRNN_GAME: {
            return createBRNNGame();
        }
        break;
        case ThirteenZhang_GAME: {
            return createThirteenZhangGame();
        }
        break;
        case BB_GAME: {
            return createBBGame();
        }
        break;
        case BJL_GAME: {
            return createBJLGame();
        }
        break;
        case FQZS_GAME: {
            return createFQZSGame();
        }
        break;
        case GoldenFlower_GAME: {
            return createGoldenFlowerGame();
        }
        break;
        case BRXJ_GAME: {
            return createBRXJGame();
        }
            break;
        case FRUIT_GAME: {
            return createFruitGame();
        }
            break;
        case SHZ_GAME: {
            return createSHZGame();
        }
            break;
        case FruitSlot_GAME: {
            return createFruitSlotGame();
        }
            break;
        case OnePieceSlot_GAME: {
            return createOnePieceSlotGame();
        }
            break;
        case NL_GAME: {
            return createNLGame();
        }
            break;
        case JSXS_GAME: {
            return createJSXSGame();
        }
            break;
        case CSLL_GAME:
        {
            return createCSLLGame();
        }
            break;
        case Medusa_GAME:
            return GAME_CREATE_SELECTOR(Medusa::GameTableUIV88::create);
        case Tiger_GAME:
            return GAME_CREATE_SELECTOR(Tiger::GameTableUIV88::create);
        case Pubg_GAME:
            return GAME_CREATE_SELECTOR(Pubg::GameTableUIV88::create);
        case EightSlot_GAME:
            return GAME_CREATE_SELECTOR(EightSlot::GameTableUI::create);
		case Honor_GAME:
			return GAME_CREATE_SELECTOR(Honor::GameTableUIV88::create);
		case RedPacket_GAME: {
			return GAME_CREATE_SELECTOR(RedPacket::GameTableUI::create);
		}
		case D21_GAME:
			return GAME_CREATE_SELECTOR(D21::GameTableUIV99::create);

		case NewYearActivity_GAME:
			return createNewYearActivityGame();

        default:
            break;
    }
	return nullptr;
}

GAME_CREATE_SELECTOR GameFactory::createGoldenToadGame() {
    return GAME_CREATE_SELECTOR(GoldenToad::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createStudGame() {
    return GAME_CREATE_SELECTOR(STUD::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createNNGame() {
    return GAME_CREATE_SELECTOR(NN::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createBRNNGame() {
    return GAME_CREATE_SELECTOR(BRNN::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createThirteenZhangGame() {
    return GAME_CREATE_SELECTOR(ThirteenZhang::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createBBGame() {
    //return GAME_CREATE_SELECTOR(BB::GameTableUIV88::create);
	return GAME_CREATE_SELECTOR(BB::GameTableUIV99::create);
}

GAME_CREATE_SELECTOR GameFactory::createBJLGame() {
    return GAME_CREATE_SELECTOR(BJL::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createFQZSGame() {
    return GAME_CREATE_SELECTOR(FQZS::GameTableUIV99::create);
}

GAME_CREATE_SELECTOR GameFactory::createGoldenFlowerGame() {
    return GAME_CREATE_SELECTOR(goldenflower::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createFruitGame() {
    return GAME_CREATE_SELECTOR(Fruit::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createBRXJGame() {
    return GAME_CREATE_SELECTOR(BRXJ::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createSHZGame() {
    return GAME_CREATE_SELECTOR(SHZ::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createFruitSlotGame() {
    return GAME_CREATE_SELECTOR(FruitSlot::GameTableUI::create);
}

GAME_CREATE_SELECTOR GameFactory::createOnePieceSlotGame() {
    return GAME_CREATE_SELECTOR(OnePieceSlot::GameTableUI::create);
}

GAME_CREATE_SELECTOR GameFactory::createNLGame() {
    return GAME_CREATE_SELECTOR(NL::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createJSXSGame() {
    return GAME_CREATE_SELECTOR(JSXS::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createCSLLGame()
{
    return GAME_CREATE_SELECTOR(CSLL::GameTableUIV88::create);
}

GAME_CREATE_SELECTOR GameFactory::createNewYearActivityGame() {
	return GAME_CREATE_SELECTOR(NewYearActivity::LcukyTurnTableV88::create);
}


