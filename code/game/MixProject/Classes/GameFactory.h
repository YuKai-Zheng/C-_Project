#ifndef _GAME_FACTORY_H_
#define _GAME_FACTORY_H_

#include "HNPlatform/HNGameCreator.h"

//#define FILTER_GAME

enum GameType {
    GoldenToad_GAME,
    Likui_GAME,
    Monkey_GAME,
    TWO_NN_GAME,
    FOUR_NN_NAME,
    SIX_NN_GAME,
    BRNN_GAME,
    BJL_GAME,
    STUD_GAME,
    BRXJ_GAME,
    NewYearActivity_GAME,
    //LuckyEnvelope_GAME,
    FQZS_GAME,
    ThirteenZhang_GAME,
    GoldenFlower_GAME,
    BB_GAME,
    FRUIT_GAME,
    //D21_GAME,
    //RedPacket_GAME,
    SHZ_GAME,
    FruitSlot_GAME,
    OnePieceSlot_GAME,
    NL_GAME,
    JSXS_GAME,
    CSLL_GAME,
    Medusa_GAME,
    Tiger_GAME,
    Pubg_GAME,
    EightSlot_GAME,
	Honor_GAME,
	RedPacket_GAME,
	D21_GAME,
    
};

/*
 
 */
class GameFactory {
public:
    static GAME_CREATE_SELECTOR create(GameType type);
    static GAME_CREATE_SELECTOR createGoldenToadGame();
    static GAME_CREATE_SELECTOR createStudGame();
    static GAME_CREATE_SELECTOR createNNGame();
    static GAME_CREATE_SELECTOR createBRNNGame();
    static GAME_CREATE_SELECTOR createThirteenZhangGame();
    static GAME_CREATE_SELECTOR createBBGame();
    static GAME_CREATE_SELECTOR createBJLGame();
    static GAME_CREATE_SELECTOR createFQZSGame();
    static GAME_CREATE_SELECTOR createGoldenFlowerGame();
    static GAME_CREATE_SELECTOR createBRXJGame();
    static GAME_CREATE_SELECTOR createFruitGame();
    static GAME_CREATE_SELECTOR createNewYearActivityGame();
//    static GAME_CREATE_SELECTOR createRedPacketGame();
    static GAME_CREATE_SELECTOR createSHZGame();
    static GAME_CREATE_SELECTOR createFruitSlotGame();
    static GAME_CREATE_SELECTOR createOnePieceSlotGame();
    static GAME_CREATE_SELECTOR createNLGame();
    static GAME_CREATE_SELECTOR createJSXSGame();
    static GAME_CREATE_SELECTOR createCSLLGame();
//#ifdef  FILTER_GAME
//    static GAME_CREATE_SELECTOR createD21Game();
//    static GAME_CREATE_SELECTOR createLuckyEnvelopeGame();
//#endif
    
};
#endif //_GAME_FACTORY_H_

