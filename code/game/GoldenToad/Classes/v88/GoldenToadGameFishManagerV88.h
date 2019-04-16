#ifndef __GoldenToad_FISH_MANAGER_V88_H__
#define __GoldenToad_FISH_MANAGER_V88_H__

#include "GoldenToadGameFishObject.h"
#include "GoldenToadMessageHead.h"

namespace GoldenToad
{
    
    
    class GameTableUIV88;
    

    
    class GameFishManagerV88
    {
        
//        constexpr static const float groupFishSpeed = 3.0f;
        
//        typedef struct  tagFishSpirteInfo
//        {
//            std::string name;
//            BYTE count;
//            BYTE dieCount;
//        }T_FISHSPRITEINFO;
//        
//        enum LockType
//        {
//            NORMAL = 0,
//            REDFISH,
//            BIGFISH
//        };
        
    public:
        typedef std::function<void (INT fishId)> LockFishCallBack;
        LockFishCallBack  onLockFishCallBack;
        
        typedef std::function<void (INT fishId)> UnLockCallBack;
        UnLockCallBack  unLockFishCallBack;
        
    public:
        GameFishManagerV88(GameTableUIV88* gameTableUI);
        ~GameFishManagerV88();
        
        //添加鱼
        Fish* addFish(FishKind fishKind, int fishId, std::vector<Vec2>* initPos, std::vector<FPointAngle> fishTraceList,
                      FishKind fishKind_ex[5], TraceType type = TRACE_LINEAR, bool isGroup = false);
        
        //设置鱼属性配置
        void setConfig(CMD_S_GameConfig* pcene);
        //场景切换
        void setSwitchScene(ImageView* gameBg, bool bSwitch);
        //暂停所有鱼运动
        void setAllFishIsPause(bool isPause = false);
        ////设置锁定鱼
        //void setLockFish(Vec2 pos, LockType type);
        //自动锁定最大倍率的鱼
        bool autoLockBigFish();
		//切换其他可以锁定的鱼
		bool changeLockBigFish(INT lockFishID);
        //自动切换时，根据玩家点击的位置，获得其所点击的鱼的信息
        Fish* switchTargetBigFish(Vec2 pos);
        //void autoSwitchLockedBigFish();
        void manualSwitchLockedBigFish(INT fishID);
        void manualUunlockBigFish();
        void lockBigFishFail();
        
        //通过id获取一条鱼
        Fish* getFishById(int id);
        //通过id获取一组鱼
        Vector<Fish*> getFishListById(int id);
        //通过kind获取一组鱼
        Vector<Fish*> getFishListByKind(int kind);
        //通过id获取鱼的位置
        Vec2 getFishPositionByFishId(INT fishId);
        
        //抓到大鱼时反馈当前屏幕信息
        CMD_C_CatchSweepFish catchSweepFish(WORD chairId, int fishId);
        //移除或者显示鱼死亡
        void removeOrSetFishDieById(INT fishId, bool bDied);
        void removeAllFish();
        
    private:
        void init();
        
    private:
        GameTableUIV88*		_gameTableUI;
        INT					_fishMultiple[FISH_KIND_COUNT];
        INT					_fishSpeed[FISH_KIND_COUNT + 1];
        Size				_fishSize[FISH_KIND_COUNT];
        INT 				_fishHitRadius[FISH_KIND_COUNT];
        bool				_isSwitch;
        
    public:
        //当前鱼列表
        std::vector<Fish*>	_fishList;
        //锁定过的鱼的id列表
        std::vector<int>	_lockFishIdList;	
        //屏幕内可以锁定的鱼的 ID 列表
        std::map<int, CMD_COM_ALLOW_LOCK_FISH*>		_canLockFishInfoList;
        void updateLockFishInfoListFromServer(bool isInsert, CMD_COM_ALLOW_LOCK_FISH* cmd);
        
    private:
        void updateLockFishInfoList(bool isInsert, CMD_COM_ALLOW_LOCK_FISH* cmd);
        
    private:
        void sendDataToServerOnLockFishInfoList(CMD_COM_ALLOW_LOCK_FISH* cmd);
        
    };
    
}

#endif // !__FISH_MANAGER_H__
