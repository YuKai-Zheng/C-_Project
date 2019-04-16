#ifndef __ThirteenZhang_GameLogic_h__
#define __ThirteenZhang_GameLogic_h__

//#include "HNNetExport.h"

#include <vector>
#include <string>
#include "HNBaseType.h"

namespace ThirteenZhang
{
    
    enum cardShape
    {
        SHAPE_HIGH_CARD = 1,		//高牌
        SHAPE_DOUBLE_YI = 2,		//一对
        SHAPE_DOUBLE_LIANG = 3,		//两对
        SHAPE_STRIP_SAN = 4,		//三条
        SHAPE_STRAIGHT = 5,			//顺子
        SHAPE_FLOWER = 6,			//同花
        SHAPE_THREE_DOUBLE = 7,		//葫芦
        SHAPE_STRIP_SI = 8,			//四条
        SHAPE_STRAIGHT_FLUSH = 9	//同花顺
    };
    
    enum specialCardShape
    {
        COMMON_CARD = 10,			//不是特殊牌
        FLOWER_THREE = 11,			//三同花
        FLUSH_THREE = 12,			//三顺子
        DOUBLE_SIX_HALF = 13,		//六对半
        DOUBLE_FIVE_STRIP = 14,		//五对三条
        STRIP_FOUR = 15,			//四套三条
        SAME_COLOR = 16,			//凑一色
        ALL_SMALL = 17,				//全小
        ALL_BIG = 18,				//全大
        STRIP_WORLD = 19,			//三分天下
        STRAIGHT_FLUSH_THREE = 20,	//三同花顺    //有问题
        KINSMEN_TWENTEEN = 21,		//十二皇族
        DRAGON_ONE = 22,			//一条龙
        DRAGON_ONLY = 23			//清龙
    };
    
    const static std::string SpecialCard[13] = {
        "TZ_SC_FLOWER_THREE",			//三同花
        "TZ_SC_FLUSH_THREE",			//三顺子
        "TZ_SC_DOUBLE_SIX_HALF",		//六对半
        "TZ_SC_DOUBLE_FIVE_STRIP",      //五对三条
        "TZ_SC_STRIP_FOUR",             //四套三条
        "TZ_SC_SAME_COLOR",             //凑一色
        "TZ_SC_ALL_SMALL",              //全小
        "TZ_SC_ALL_BIG",				//全大
        "TZ_SC_STRIP_WORLD",			//三分天下
        "TZ_SC_STRAIGHT_FLUSH_THREE",	//三同花顺
        "TZ_SC_KINSMEN_TWENTEEN",		//十二皇族
        "TZ_SC_DRAGON_ONE",             //一条龙
        "TZ_SC_DRAGON_ONLY"             //清龙
    };
    
    const static int specialCardNameIndexOffsetValue = 11;  //获取特殊牌型对应的英文标识，应该使用特殊牌枚举序号减去对应的索引偏移值，才能从数组中获得

    
    //操作掩码
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111
    
    //扑克花色
#define UG_FANG_KUAI				0x00			//方块	0000 0000
#define UG_MEI_HUA					0x10			//梅花	0001 0000
#define UG_HONG_TAO					0x20			//红桃	0010 0000
#define UG_HEI_TAO					0x30			//黑桃	0011 0000
#define UG_NT_CARD					0x40			//主牌	0100 0000
#define UG_ERROR_HUA				0xF0			//错误  1111 0000
    
    //扑克出牌类型
//#define UG_ERROR_KIND							0				//错误
    
#define UG_ONLY_ONE								1				//单张
#define UG_DOUBLE								2				//对牌
    
#define UG_VARIATION_STRAIGHT					3				//变种顺子(A2345)顺子中最小
#define UG_STRAIGHT								4               //顺子,5+张连续牌
#define UG_FLUSH								5				//同花(非连)
#define UG_STRAIGHT_FLUSH						6               //同花顺,花色相同的顺子
    
#define UG_THREE								7				//三张
#define UG_THREE_ONE							8               //3 带 1
#define UG_THREE_TWO							9               //3 带 2
#define UG_THREE_DOUBLE							10				//3 带1对
    
#define UG_VARIATION_DOUBLE_SEQUENCE			11				//变种双顺(AA22)最小
#define UG_DOUBLE_SEQUENCE						12				//连对,2+个连续的对子
    
#define UG_VARIATION_THREE_SEQUENCE				13				//变种三顺(AAA222最小)
#define UG_THREE_SEQUENCE						14				//连三张，2+个连续的三张
    
#define UG_VARIATION_THREE_ONE_SEQUENCE			15				//变种三顺带一
#define UG_THREE_ONE_SEQUENCE					16              //2+个连续的三带一
    
#define UG_VARIATION_THREE_TWO_SEQUENCE			17				//变种三顺带二
#define UG_THREE_TWO_SEQUENCE					18				//2+个连续的三带二
    
#define UG_VARIATION_THREE_DOUBLE_SEQUENCE		19				//变种三连张带对
#define UG_THREE_DOUBLE_SEQUENCE				20				//三连张带对
    
#define UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE		21		//变种蝴蝶(三顺带二顺)
#define UG_THREE_SEQUENCE_DOUBLE_SEQUENCE		22				//蝴蝶(三顺带二顺)
    
#define UG_FOUR_ONE								23				//四带一
#define UG_FOUR_TWO								24				//四带二张
#define UG_FOUR_ONE_DOUBLE						25				//四带一对
#define UG_FOUR_TWO_DOUBLE						26				//四带二对
    
#define UG_VARIATION_FOUR_SEQUENCE				27				//四顺
#define UG_FOUR_SEQUENCE						28				//四顺
    
#define UG_VARIATION_FOUR_ONE_SEQUENCE			29				//四带一顺
#define UG_FOUR_ONE_SEQUENCE					30				//四带一顺
    
#define UG_VARIATION_FOUR_TWO_SEQUENCE			31				//四带二顺
#define UG_FOUR_TWO_SEQUENCE					32				//四带二顺
    
#define UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE	33				//四带对顺
#define UG_FOUR_ONE_DOUBLE_SEQUENCE				34				//四带对顺
    
#define UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE	35				//四带二对顺
#define UG_FOUR_TWO_DOUBLE_SEQUENCE				36				//四带二对顺
    
    
#define UG_SLAVE_510K							37              //510K炸弹,花色不同
#define UG_MASTER_510K							38              //510K同花炸弹
    
#define UG_BOMB									39              //炸弹>=4張
#define UG_BOMB_SAME_HUA						40				//同花炸弹(在四副或以上的牌中出现)
#define UG_KING_BOMB							41				//王炸(最大炸弹)
    
    
#define KING_COUNT								2				//所有王的个数
    
    //510K逻辑类 支持 2 副扑克）
    class CUpGradeGameLogic
    {
        //变量定义
    private:
        //	int				m_iStation[5];	   //相对位置（方块，梅花，红桃，黑桃，主牌）
        BYTE			m_bSortCardStyle;  //排序方式
        int				m_iCondition;			//限制条件
        bool			m_bKingCanReplace;		//王是否可当
        
        UINT           m_iCardShape;	//支持牌型
        //函数定义
    public:
        //构造函数
        CUpGradeGameLogic(void);
        //析构函数
        virtual ~CUpGradeGameLogic();
        
        //功能函数（公共函数）
    public:
        //[设置相关]
        //获取扑克数字
        inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK) + 1; }
        //获取扑克花色(默认为真实花色)
        BYTE GetCardHuaKind(BYTE iCard, bool bTrueHua = true);
        //获取扑克相对大小(默认为牌大小,非排序大小)
        int GetCardBulk(BYTE iCard, bool bExtVal = false);
        //获取扑克牌通过相对大小
        BYTE GetCardByValue(int iCardValue);
        //設置王可以當牌
        void SetKingCanReplace(bool bKingCanReplace = false){ m_bKingCanReplace = bKingCanReplace; }
        //獵取王是否可以當牌
        bool GetKingCanReplace(){ return m_bKingCanReplace; }
        //设置排序方式
        void SetSortCardStyle(BYTE SortCardStyle){ m_bSortCardStyle = SortCardStyle; }
        //获取排序方式
        BYTE GetSortCardStyle(){ return m_bSortCardStyle; }
        
        //[排序]
    public:
        //排列扑克,按大小(保留系统序例)
        bool SortCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount, bool bSysSort = false);
        //反转牌顺(从低->高)
        bool ReverseCard(BYTE iCardList[], BYTE bUp[], BYTE iCardCount);
        //按牌型排序
        bool SortCardByStyle(BYTE iCardList[], BYTE iCardCount);
        //按花色排序
        bool SortCardByKind(BYTE iCardList[], BYTE iCardCount);
        //设置排序方式
        void SetSortKind(BYTE bSortCardStyle);
    public:
        //混乱扑克
        BYTE RandCard(BYTE iCard[], int iCardCount, int iRoomId, bool bHaveKing = false);		//bHaveKing表示是否有大小猫,false无,ture有
        //删除扑克
        int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
        //增加扑克牌数据
        int	AddCardToList(BYTE byAddCard[], int &iAddCount, BYTE iCardList[], int iCardCount);
    private:
        //清除 0 位扑克
        int RemoveNummCard(BYTE iCardList[], int iCardCount);
        
    public://[辅助函数]
        //对比单牌
        bool CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);
        //查找分数
        int FindPoint(BYTE iCardList[], int iCardCount);
        //是否为同一数字牌
        bool IsSameNumCard(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //是否为三张同一花色
        bool IsSameHuaThreeKind(BYTE iCardList[],int iCardCount,bool bExtVal=false);
        //是否为同一花色
        bool IsSameHuaKind(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //查找 >=4 炸弹的数量炸弹基数
        BYTE GetBombCount(BYTE iCardList[], int iCardCount, int iNumCount = 4, bool bExtVal = false);
        //获取指定大小牌个数
        BYTE GetCountBySpecifyNumCount(BYTE iCardList[], int iCardCount, int Num);
        //获取指定牌个数
        BYTE GetCountBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE bCard);
        //获取指定牌张数牌大小(队例中只能够有一种牌的张数为iCount,不然传出去的将是第一个指定张数的值)
        BYTE GetBulkBySpecifyCardCount(BYTE iCardList[], int iCardCount, int iCount);
        //是否为某指定的顺子(变种顺子)
        bool IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount);
        //是否为三张顺子
        bool IsStraightThree(BYTE iCardList[],int iCardCount);
        //是否为某指定的顺子
        bool IsSequence(BYTE iCardList[], int iCardCount, bool bExtVal);
        //是否为某指定的同花大顺
        bool IsBigSequence(BYTE iCardList[], int iCardCount);
        //提取指定的牌
        BYTE TackOutBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE bCardBuffer[], int &iResultCardCount, BYTE bCard);
        //提取某张指定数字的牌
        bool TackOutCardBySpecifyCardNum(BYTE iCardList[], int iCardCount, BYTE iBuffer[], int &iBufferCardCount, BYTE iCard, bool bExtVal = false);
        //提取所有符合条件的牌,单张,对牌,三张,4炸弹牌型
        int TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BYTE bCardNum, bool bExtVal = false);
        //提取指定花色牌
        int TackOutByCardKind(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], BYTE iCardKind);
        //拆出(将手中牌多的拆成少的)
        int TackOutMuchToFew(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[], int &iBufferCardCount, BYTE iCardMuch, BYTE iCardFew);
        //查找大于iCard的单牌所在iCardList中的序号
        BYTE GetSerialByMoreThanSpecifyCard(BYTE iCardList[], int iCardCount, BYTE iCard, BYTE iBaseCardCount, bool bExtValue = false);
        //查找==iCard的单牌所在iCardList中的序号(起始位置,到終點位置)
        int GetSerialBySpecifyCard(BYTE iCardList[], int iStart, int iCardCount, BYTE iCard);
        //获取指定顺子中牌点最小值(iSequence 代表顺子的牌数最多为
        BYTE GetBulkBySpecifySequence(BYTE iCardList[], int iCardCount, int iSequence = 3);
        //获取指定顺子中牌点最大值变种顺子
        BYTE GetBulkBySpecifyVariationSequence(BYTE iCardList[], int iCardCount, int iSequence = 3);
        //查找最小 (1) or 最大 (255) 牌
        int	GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax, bool bExtVal = false);
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        //[牌型相关]
    public:
        //获取牌型
        BYTE GetCardShape(BYTE iCardList[], int iCardCount, bool bExlVal = false);
        //是否单牌
        inline bool IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount == 1; };
        //是否对牌
        bool IsDouble(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //3 带 1or2(带一对带二单张或带一单张
        bool IsThreeX(BYTE iCardList[], int iCardCount, int iX/*1or2*/, bool bExtVal = false);
        //王炸
        bool IsKingBomb(BYTE iCardList[], int iCardCount);
        //4+张牌 炸弹
        bool IsBomb(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //同花炸弹
        bool IsBombSameHua(BYTE iCardList[], int iCardCount);
        //同花(非顺子)
        bool IsFlush(BYTE iCardList[], int iCardCount);
        //是否是同花顺
        bool IsStraightFlush(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        
        
        /************************************************************************/
        /* 特殊牌型检测  by HBC                                                 */
        /************************************************************************/
        int GetSpecialCardStyle(BYTE iCardList[], int iCardCount = 13);
        
        //是否是三同花
        bool IsFlowerThree(BYTE iCardList[],int iCardCount);
        //是否是三顺子
        bool IsFlushThree(BYTE iCardList[],int iCardCount);
        //是否是六对半
        bool IsDoubleSixHalf(BYTE iCardList[],int iCardCount);
        //是否是五对三条
        bool IsDoubleFiveStrip(BYTE iCardList[],int iCardCount);
        //是否是四套三条
        bool IsStripFour(BYTE iCardList[],int iCardCount);
        //是否是凑一色
        bool IsSameColor(BYTE iCardList[],int iCardCount);
        //是否是同花色
        bool IsSameFlower(BYTE iCardList[], int iCardCount);
        //是否是全小
        bool IsAllSmall(BYTE iCardList[],int iCardCount);
        //是否是全大
        bool IsAllBig(BYTE iCardList[],int iCardCount);
        //是否是三分天下
        bool IsStripWorld(BYTE iCardList[],int iCardCount);
        //是否是三同花顺
        bool IsStraightFlushThree(BYTE iCardList[],int iCardCount);
        //是否是十二皇族
        bool IsKinsmenTwenteen(BYTE iCardList[],int iCardCount);
        //是否是一条龙
        bool IsDragonOne(BYTE iCardList[],int iCardCount);
        //是否是清龙
        bool IsDragonOnly(BYTE iCardList[],int iCardCount);
        
        //是否为 2-A 一条龙
        bool IsDragon(BYTE iCardList[], int iCardCount);
        //获得同牌值牌数目  即可以找对子、三条、铁支
        BYTE GetSameCardsNum(BYTE iCardList[], int iCardCount,int iSameNumber);
        //获取同花色牌数目
        void GetSameColorCardsNum(BYTE iCardList[], BYTE returnList[], int arrNum = 13);
        //获取同牌值数组
        void GetSameValueCardsNum(BYTE iCardList[], BYTE returnList[], int arrNum = 13);
        /************************************************************************/
        /* 特殊牌型检测方法  END  by HBC                                        */
        /************************************************************************/
        
        
        //变种单甩
        bool IsVariationStraight(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //单甩
        bool IsStraight(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //变种对甩
        bool IsVariationDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //对甩 //连对?
        bool IsDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        
        //是否变种是连续的三带X(0,1,2,3)
        bool IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/, bool bExtVal = false);
        
        //是否是连续的三带X(0,1,2,3)
        bool IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/, bool bExtVal = false);
        //是否三顺带二顺(蝴蝶)
        bool IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //是否三顺带二顺(蝴蝶)
        bool IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //510K 炸弹
        bool IsSlave510K(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //510K 同花炸弹
        bool IsMaster510K(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //四带一或者四带二
        bool IsFourX(BYTE iCardList[], int iCardCount, int iX/*1or 2*/);//单张1,任意张2,一对子,2对4
        //是否变种四带X顺
        bool IsVariationFourXSequence(BYTE iCardList[], int iCardCount, int iSeqX);
        //四带一或者四带二的顺子
        bool IsFourXSequence(BYTE iCardList[], int iCardCount, int iSeqX);
        //[出牌相关]
    public:
        //是否可以出牌
        bool CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount, bool bFirstOut = false);
        //查找比当前出牌大的
        bool TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount,
                                     BYTE iResultCard[], int & iResultCardCount, bool bExtVal = false);
        
        //[提取牌]
    public:
        //提取单个的三带1 or 2or 3(单,一对,或二单张)
        BYTE TackOutThreeX(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int  xValue);
        //提取2个以上连续的三带1,2
        bool TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue);
        //提取2个以上连续的三带1,2
        bool TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
        //提取2个以上连续的三带1,2
        //BYTE TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, bool bExtVal=false);
        //提取单张的顺子,连对 or 连三
        //BYTE TackOutSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
        //获取顺子中最小位置值(xSequence表示默认单顺)
        int GetSequenceStartPostion(BYTE iCardList[], int iCardCount, int xSequence = 1);
        //提取单张的顺子,连对顺子,连三顺子
        bool TackOutSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xSequence, bool bNoComp = false);
        //提取同花顺
        bool TackOutStraightFlush(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
        //BYTE TackOutStraightFlush(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int bExtVal=false);
        //提取所的炸弹
        bool TackOutAllBomb(BYTE iCardList[], int iCardCount,
                            BYTE iResultCard[], int &iResultCardCount, int iNumCount = 4);
        //提取炸弹(张数默认为4)
        bool TackOutBomb(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, int iNumCount = 4);
        //提取王炸
        bool TackOutKingBomb(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount);
        //提取510K
        bool TrackOut510K(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, bool bExtVal = false);
        //测试510K
        bool Test510K(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //拷背
        bool Copy510K(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount);
        //拆大桌面牌
        bool TackOutCardByNoSameShape(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, BYTE iBaseCard[], int iBaseCardCount);
        ///提取指定的牌
        bool TackOutCardBySpecifyCard(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iSepcifyCard);
        //用大的牌牌大桌面上的牌
        bool TackOutMoreThanLastShape(BYTE iCardList[], int iCardCount, BYTE iResultCard[], int &iResultCardCount, BYTE iBaseCard[], int iBaseCardCount);
        //分析函数
        //BOOL AnalyseCard(BYTE iCardList[], int iCardCount, CardAnalyseStruct & Analyse);
        
    public:
        
        //查找牌型中有几个三张的牌
        //int SearchCountOfThree(BYTE iCardList[],int iCardCount,bool bExtVal=false);
        //查找三张的牌点
        //int SearchThreeCard(BYTE iCardList[],int iCardCount,bool bExtVal=false);
        //查找三连牌中最小的那个三张的牌点
        int SearchMinThreeSeq(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //重置限制条件
        void ResetCondition();
        
        ///设置游戏牌型
        void SetCardShape(UINT iCardShape){ m_iCardShape = iCardShape; }
        
        /// 拖牌自动检查
        void DragCardAutoSetValidCard(BYTE iUpCardList[], int iUpCardCount, BYTE bResult[], int & bResultCount);
        
        ///查找最小 (1) or 最大 (255) 牌值
        ///
        /// [@param in bExtVal] 真，不考虑2、王
        BYTE GetCardMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax, bool bExtVal = false);
    public:
        //获取指定牌中的对子个数
        BYTE GetDoubleNums(BYTE iCardList[], int iCardCount, bool bExtVal = false);
        //获取指定牌中的相同牌的最大个数
        int GetSameCardMaxNums(BYTE iCardList[], int iCardCount);
        //获得牌型
        int GetCardStyle(BYTE iCardList[], int iCardCount);
        ///比较两手中牌的大小
        int CompareCard(BYTE wFirstCard[], int iFirstCount, BYTE wSecondCard[], int iSecondCount);
        ///得到手牌中最大的牌(含花色)
        BYTE GetMaxCard(BYTE  wCardList[], int iCardCount);
        //判断同类型的牌是否有倒水
        bool hasDaoshuiWithSameType(BYTE type, BYTE  firstCardList[], BYTE  lastCardList[], int iFirstCount, int iLastCount);
        
        
        BYTE GetMaxSingleCards(BYTE iHandCard[], int iHandCardCount);//获得最大的单牌
        BYTE GetSecondSingleCards(BYTE iHandCard[], int iHandCardCount);
        BYTE GetThirdSingleCards(BYTE iHandCard[], int iHandCardCount);
        BYTE GetFourthSingleCards(BYTE iHandCard[], int iHandCardCount);
        BYTE GetFifthSingleCards(BYTE iHandCard[], int iHandCardCount);
        BYTE GetDoubleCardValue(BYTE iHandCard[], int iHandCardCount);//获得对子的牌值
        BYTE GetMaxDoubleCardValue(BYTE iHandCard[], int iHandCardCount);//获得两对中大的对子
        BYTE GetMinDoubleCardValue(BYTE iHandCard[], int iHandCardCount);//获得2对子中最小的对子
        BYTE GetThreeCardValue(BYTE iHandCard[], int iHandCardCount);//获得三条的牌值
        BYTE GetFourCardValue(BYTE iHandCard[], int iHandCardCount);//获得四条的牌值
        
        bool IsMaxSequenceCards(BYTE iHandCard[], int iHandCardCount);//是否是最大的顺子
        bool IsSecondSequenceCards(BYTE iHandCard[], int iHandCardCount);
        
        //自动出牌函数
        bool AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //验证游戏中数据是否有效
        bool TestData(int iData, int iStyle);
        //计算有效牌个数
        int CountUserCard(BYTE bCardList[], int iCount);
        //从指定数组中删除一张牌
        void DeleteACard(BYTE bCardList[], int iCount, BYTE bDeleteCard);
    public:
        //查找同花顺
        bool FindSameFlowerFlush(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找四条
        bool FindFourStrip(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找葫芦
        bool FindCalabash(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找同花
        bool FindSameFlower(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找顺子
        bool FindStraight(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找三条
        bool FindThreeStrip(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找三张牌组成的三条
        bool FindThreeThreeStrip(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找两对
        bool FindSecondDouble(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找一对
        bool FindOneDouble(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        // 查找三张牌组成的一对
        bool FindThreeOneDouble(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
        //查找牌值最大的高牌
        bool FindMaxHighCard(BYTE iHandCard[], int iHandCardCount, BYTE iResultCard[], int & iResultCardCount);
    };
    
}



//////////////////////////////////////////////////////////////////////////

#endif // GameLogic_h__
