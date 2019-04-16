#include "ThirteenZhangResourceMgrv88.h"
#include "ThirteenZhangGameLogic.h"

//////////////////////////////////////////////////////////////////////////

namespace ThirteenZhang
{
    std::pair<std::string, std::string> ResourceMgrV88::getCardTypeResource(bool sex, BYTE cardType, bool forResult)
    {
        std::pair<std::string, std::string> cardTypeResource;
        switch (cardType)
        {
            case SHAPE_HIGH_CARD:		//高牌
                return std::make_pair(forResult ? "HIGH_CARD.png" : "HIGH_CARD_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common1.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common1.mp3");
                break;
            case SHAPE_DOUBLE_YI:		//一对
                return std::make_pair(forResult ? "SHAPE_DOUBLE_YI.png" : "SHAPE_DOUBLE_YI_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common2.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common2.mp3");
                break;
            case SHAPE_DOUBLE_LIANG:	//两对
                return std::make_pair(forResult ? "SHAPE_DOUBLE_LIANG.png" : "SHAPE_DOUBLE_LIANG_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common3.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common3.mp3");
                break;
            case SHAPE_STRIP_SAN:		//三条
                return std::make_pair(forResult ? "SHAPE_STRIP_SAN.png" : "SHAPE_STRIP_SAN_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common4.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common4.mp3");
                break;
            case SHAPE_STRAIGHT:		//顺子
                return std::make_pair(forResult ? "SHAPE_STRAIGHT.png" : "SHAPE_STRAIGHT_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common5.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common5.mp3");
                break;
            case SHAPE_FLOWER:			//同花
                return std::make_pair(forResult ? "SHAPE_FLOWER.png" : "SHAPE_FLOWER_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common6.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common6.mp3");
                break;
            case SHAPE_THREE_DOUBLE:	//葫芦
                return std::make_pair(forResult ? "SHAPE_THREE_DOUBLE.png" : "SHAPE_THREE_DOUBLE_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common7.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common7.mp3");
                break;
            case SHAPE_STRIP_SI:		//四条
                return std::make_pair(forResult ? "SHAPE_STRIP_SI.png" : "SHAPE_STRIP_SI_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common8.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common8.mp3");
                break;
            case SHAPE_STRAIGHT_FLUSH:	//同花顺
                return std::make_pair(forResult ? "SHAPE_STRAIGHT_FLUSH.png" : "SHAPE_STRAIGHT_FLUSH_1.png",
                                      sex ? "v66/music/ThirteenZhang_v66/Common_Card/female/common9.mp3"
                                      : "v66/music/ThirteenZhang_v66/Common_Card/male/common9.mp3");
                break;
            case FLOWER_THREE:          //三同花
                return std::make_pair("FLOWER_THREE.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/FLOWER_THREE_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/FLOWER_THREE_%d.mp3");
                break;
            case FLUSH_THREE:           //三顺子
                return std::make_pair("FLUSH_THREE.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/FLUSH_THREE_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/FLUSH_THREE_%d.mp3");
                break;
            case DOUBLE_SIX_HALF:       //六对半
                return std::make_pair("DOUBLE_SIX_HALF.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/DOUBLE_SIX_HALF_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/DOUBLE_SIX_HALF_%d.mp3");
                break;
            case DOUBLE_FIVE_STRIP:     //五对三条
                return std::make_pair("DOUBLE_FIVE_STRIP.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/DOUBLE_FIVE_STRIP_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/DOUBLE_FIVE_STRIP_%d.mp3");
                break;
            case STRIP_FOUR:            //四套三条
                return std::make_pair("STRIP_FOUR.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/STRIP_FOUR_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/STRIP_FOUR_%d.mp3");
                break;
            case SAME_COLOR:            //凑一色
                return std::make_pair("SAME_COLOR.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/SAME_COLOR_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/SAME_COLOR_%d.mp3");
                break;
            case ALL_SMALL:             //全小
                return std::make_pair("ALL_SMALL.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/ALL_SMALL_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/ALL_SMALL_%d.mp3");
                break;
            case ALL_BIG:               //全大
                return std::make_pair("ALL_BIG.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/ALL_BIG_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/ALL_BIG_%d.mp3");
                break;
            case STRIP_WORLD:           //三分天下
                return std::make_pair("STRIP_WORLD.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/STRIP_WORLD_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/STRIP_WORLD_%d.mp3");
                break;
            case STRAIGHT_FLUSH_THREE:  //三同花顺
                return std::make_pair("STRAIGHT_FLUSH_THREE.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/STRAIGHT_FLUSH_THREE_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/STRAIGHT_FLUSH_THREE_%d.mp3");
                break;
            case KINSMEN_TWENTEEN:      //十二皇族
                return std::make_pair("KINSMEN_TWENTEEN.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/KINSMEN_TWENTEEN_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/KINSMEN_TWENTEEN_%d.mp3");
                break;
            case DRAGON_ONE:            //一条龙
                return std::make_pair("DRAGON_ONE.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/DRAGON_ONE_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/DRAGON_ONE_%d.mp3");
                break;
            case DRAGON_ONLY:           //清龙
                return std::make_pair("DRAGON_ONLY.png",
                                      sex ? "music/ThirteenZhang/sound/SpecialShape/woman/DRAGON_ONLY_%d.mp3"
                                      : "music/ThirteenZhang/sound/SpecialShape/man/DRAGON_ONLY_%d.mp3");
                break;
            default:
                break;
        }
        return cardTypeResource;
    }
    
    std::string ResourceMgrV88::getCardTextureFileName(BYTE cardValue)
    {
        char filename[64];
        if(PRESSED_TEXTURE){
            sprintf(filename, "v66/common/card_AMax/%02x.png", cardValue);
        }else{
            sprintf(filename, "%02x.png", cardValue);
        }
        return filename;
    }
    
    std::string ResourceMgrV88::getTypeTextureFileName(std::pair<std::string, std::string> res) {
        char filename[64];
        if(PRESSED_TEXTURE){
            sprintf(filename, "Games/12100004/13zhang/teshupai/%s", res.first.c_str());
        }else{
            sprintf(filename, "Games/12100004/13zhang/paixing2/%s", res.first.c_str());
        }
        return filename;
    }

//    std::string ResourceMgrV88::getCardTyeCellResource(BYTE cardValue)
//    {
//        switch (cardValue)
//        {
//
//        case SHAPE_HIGH_CARD:        //高牌
//            return "Games/12100004/paixing2/HIGH_CARD.png";
//            break;
//        case SHAPE_DOUBLE_YI:        //一对
//            return "Games/12100004/paixing2/SHAPE_DOUBLE_YI.png";
//            break;
//        case SHAPE_DOUBLE_LIANG:    //两对
//            return "v66/13zhang/paixing2/SHAPE_DOUBLE_LIANG.png";
//            break;
//        case SHAPE_STRIP_SAN:        //三条
//            return "v66/13zhang/paixing2/SHAPE_STRIP_SAN.png";
//            break;
//        case SHAPE_STRAIGHT:        //顺子
//            return "v66/13zhang/paixing2/SHAPE_STRAIGHT.png";
//            break;
//        case SHAPE_FLOWER:            //同花
//            return "v66/13zhang/paixing2/SHAPE_FLOWER.png";
//            break;
//        case SHAPE_THREE_DOUBLE:    //葫芦
//            return "v66/13zhang/paixing2/SHAPE_THREE_DOUBLE.png";
//            break;
//        case SHAPE_STRIP_SI:        //四条
//            return "v66/13zhang/paixing2/SHAPE_STRIP_SI.png";
//            break;
//        case SHAPE_STRAIGHT_FLUSH:    //同花顺
//            return "v66/13zhang/paixing2/SHAPE_STRAIGHT_FLUSH.png";
//            break;
//        default:
//            break;
//
//        }
//
//
//    }



	
}
