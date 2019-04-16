#include "GoldenToadGameFishManagerV88.h"
#include "GoldenToadGameTableUIV88.h"
#include "GoldenToadGameMathAide.h"
#include "GoldenToadGameAudio.h"

//using namespace GoldenToadGameAudio;

namespace GoldenToad
{
    const std::vector<int> _fishFirstLockKind = {FISH_KIND_24,FISH_KIND_23,FISH_KIND_19,FISH_KIND_26,FISH_KIND_25,FISH_KIND_29,FISH_KIND_30,FISH_KIND_27,FISH_KIND_28,FISH_KIND_32,FISH_KIND_22,FISH_KIND_38};
    const static int FISH_KIND_23_AREA_BOOM = 300; //局部炸弹半径爆炸范围
    GameFishManagerV88::GameFishManagerV88(GameTableUIV88* gameTableUI)
    : _gameTableUI(gameTableUI)
    , _isSwitch (false)
    , onLockFishCallBack (nullptr)
    , unLockFishCallBack (nullptr)
    //, _canLockFishInfoList(nullptr)
    {
        init();
    }
    
    GameFishManagerV88::~GameFishManagerV88()
    {
        for(auto fish: _fishList)
        {
            fish->release();
        }
        _fishList.clear();
//        for (int i = 0; i < _lockFishIdList.size(); i++) {
//            _lockFishIdList.push_back(i);
//        }
//        _lockFishIdList.clear();
        auto it = _canLockFishInfoList.begin();
        while (it != _canLockFishInfoList.end()) {
            _canLockFishInfoList.erase(it++);
        }
        _canLockFishInfoList.clear();
    }
    
    Fish* GameFishManagerV88::addFish(FishKind fishKind, int fishId, std::vector<Vec2>* initPos, std::vector<FPointAngle> fishTraceList, FishKind fishKind_ex[5], TraceType type, bool isGroup)
    {
        if (fishKind >= FISH_KIND_COUNT || fishKind < FISH_KIND_1 || _isSwitch)
        {
            return nullptr;
        }
        //fishKind = FISH_KIND_32;
        Fish* fish = nullptr;
        //小鱼重复利用
        if (fishKind <= FISH_KIND_26)
        {
            for (auto fishTemp : _fishList)
            {
                if (fishTemp == nullptr)
                {
                    continue;
                }
                if (fishTemp->getFishKind() == fishKind && !fishTemp->isVisible())
                {
                    fish = fishTemp;
                    fish->setFishID(fishId);
                    break;
                }
            }
        }
        
        //没有可重复利用的则新创建一条鱼
        if (nullptr == fish)
        {
            fish = Fish::create(fishId, fishKind, fishKind_ex);
            _fishList.push_back(fish);
        }
        
        //把鱼从父节点上移除，然后重新添加，防止重复利用的鱼渲染层级不对
        if (nullptr != fish->getParent())
        {
            fish->removeFromParentAndCleanup(false);
        }
        
        _gameTableUI->addChild(fish, FISH_ZORDER);
        fish->setPosition(Vec2(0, -200));
        
        auto winSize = Director::getInstance()->getWinSize();
        auto scaleX = winSize.width / kResolutionWidth;
        auto scaleY = winSize.height / kResolutionHeight;
        //适配
        if (winSize.width / winSize.height <= 1.5f)
        {
            fish->setScale(scaleX);
        }
        else
        {
            fish->setScale(scaleY);
        }
        
        if (!fishTraceList.empty())
        {
            //设置鱼是按照鱼阵路径运动
            fish->setFishTraceList(fishTraceList, false);
        }
        float tmpScale = 0.98f;
        float fishW = _fishSize[fishKind].width * tmpScale;
        float fishH = _fishSize[fishKind].height * tmpScale;
        //给鱼儿身上绑定一个矩形，用来检测子弹碰撞，大小为服务端发来
        fish->setFishSize(Size(fishW, fishH));
        //如果是鱼阵，则速度固定
        if (isGroup)
        {
            fish->playFishAction(initPos, type, groupFishSpeed);
        }
        else
        {
            fish->playFishAction(initPos, type, _fishSpeed[fishKind]);
        }
        
        //鱼是否可以被锁定的状态发生变更时调用
        fish->onUpdateFishScoreCallBack = [this](bool isInsert, int fishID, FishKind fishKind)
        {
            CMD_COM_ALLOW_LOCK_FISH cmd;
            cmd.isInsert = isInsert;
            cmd.fishID = fishID;
            cmd.fishKind = fishKind;
            updateLockFishInfoList(isInsert, (CMD_COM_ALLOW_LOCK_FISH*)&cmd);
        };
        
        //鱼游动到目的地之后回调
        fish->onMoveOutCallBack = [this](int fishId){
            removeOrSetFishDieById(fishId, false);
        };
        
        //鱼游出屏幕外或者被捕获，自动切换其他鱼进行锁定
        fish->onUnLockCallBack = [this](int fishId)
        {
            if (nullptr != unLockFishCallBack)
            {
                //_lockFishIdList.clear();
                unLockFishCallBack(fishId);
                manualUunlockBigFish();//先去除原有的鱼的锁定，再自动重新锁定
                
                if (!autoLockBigFish())
                {
                    //自动切换的时候没有可以锁定的鱼，取消锁定
                    lockBigFishFail();
                }
            }
        };
        return fish;
    }
    
    void GameFishManagerV88::setSwitchScene(ImageView* gameBg, bool bSwitch)
    {
        _isSwitch = bSwitch;
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (fish->isVisible())
            {
                fish->setGameBG(gameBg);
                fish->setSwitchScene(bSwitch);
            }
            else
            {
                removeOrSetFishDieById(fish->getFishID(), false);
            }
        }
    }
    
    Vec2 GameFishManagerV88::getFishPositionByFishId(INT fishId)
    {
        auto fish = getFishById(fishId);
        if (nullptr != fish)
        {
            return fish->getPosition();
        }
        return Vec2::ZERO;
    }
    
    Fish* GameFishManagerV88::getFishById(int id)
    {
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (fish->isVisible() && fish->getFishID() == id)
            {
                return fish;
            }
        }
        return nullptr;
    }
    
    Vector<Fish*> GameFishManagerV88::getFishListById(int id)
    {
        Vector<Fish*> fishList;
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (fish->isVisible() && fish->getFishID() == id)
            {
                fishList.pushBack(fish);
            }
        }
        return fishList;
    }
    
    Vector<Fish*> GameFishManagerV88::getFishListByKind(int kind)
    {
        Vector<Fish*> fishList;
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (fish->isVisible() && fish->getFishKind() == kind)
            {
                fishList.pushBack(fish);
            }
        }
        return fishList;
    }
    
    void GameFishManagerV88::removeOrSetFishDieById(INT fishId, bool bDied)
    {
        bool canErase = false;
        Fish* fish = nullptr;
        auto ite = _fishList.begin();
        for (; ite != _fishList.end(); ite++)
        {
            auto fish_ = (Fish*)*ite;
            if (nullptr == fish_) continue;
            
            if (fish_->getFishID() == fishId)
            {
                fish = fish_;
                
                if (fish->getFishKind() > FISH_KIND_26)
                {
                    canErase = true;
                }
                
                if (bDied)
                {
                    if (!fish->isVisible()) continue;
                    //播放死亡动画特效
                    fish->playFishDieAction();
                    //播放鱼死亡音效
                    if (fish->getFishKind() >= FISH_KIND_10 && fish->getFishKind() <= FISH_KIND_17)
                    {
                        _gameTableUI->_audioEngine->playSmallFishDied(fish->getFishKind());
                    }
                }
                else
                {
                    if (fish->getFishKind() > FISH_KIND_26)
                    {
                        fish->removeFromParent();
                    }
                }
                break;
            }
        }
        if (canErase)
        {
            ((Fish*)*ite)->release();
            _fishList.erase(ite);
        }
    }
    
    void GameFishManagerV88::removeAllFish()
    {
//        Fish* fish = nullptr;
//        auto ite = _fishList.begin();
//        for (; ite != _fishList.end(); ite++)
//        {
//            fish = (Fish*)*ite;
//            if (nullptr == fish) continue;
//            fish->release();
//            _fishList.erase(ite);
//        }
    }
    
    void GameFishManagerV88::setAllFishIsPause(bool isPause /* = false */)
    {
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (fish->isVisible())
            {
                fish->setPauseAction(isPause);
            }
        }
    }
    
    //自动切换时，根据玩家点击的位置，获得其所点击的鱼的信息
    //若点击的鱼不可被锁定或者该鱼已经是锁定的鱼，则跳过，返回空值
    Fish* GameFishManagerV88::switchTargetBigFish(Vec2 pos)
    {
        auto winSize = Director::getInstance()->getWinSize();
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            //鱼不可见直接跳过
            if (!fish->getCanTouch() || !fish->isVisible()) continue;
            //鱼的种类级别不可以做为带锁定的目标鱼，跳过
            if (fish->getFishKind() >= FISH_KIND_13)
            {
                //如果鱼不在屏幕中，锁定时要跳过
                if (fish->getPositionX() < 0 || fish->getPositionX() > winSize.width
                    || fish->getPositionY() < 0 || fish->getPositionY() > winSize.height)
                {
                    continue;
                }
                bool canLock = true;
                //如果当前鱼已经被锁定过一次，再次点击锁定的时候就要跳过
                for (auto id : _lockFishIdList)
                {
                    if (id == fish->getFishID())
                    {
                        fish->setFishIsLock(false);
                        canLock = false;
                        break;;
                    }
                }
                if (canLock)
                {
                    //可以锁定的目标鱼没有被锁定过
                    Rect rect = fish->getFishRect();
                    auto currentSize = Size(rect.size.width * 1.8f, rect.size.height * 1.8f);
                    rect.size = currentSize;
                    if (rect.containsPoint(pos)) //鱼检测手指
                    {
                        return fish;
                    }
                }
            }
        }
        return nullptr;//默认返回空NULL，代表当前点击的鱼无法做为切换的目标鱼
    }
    
    //自动锁定最大倍率的鱼
    bool GameFishManagerV88::autoLockBigFish()
    {
        if (_fishList.size() <= 0 || _fishList[0] == nullptr || !_gameTableUI->_isLockFish)
        {
            return false;
        }
        auto winSize = Director::getInstance()->getWinSize();
        Fish* targetFish = nullptr;
        bool hasLockFish = false;
        int maxMultiple = 1;
        //
        bool isFirst = true;
        std::vector<Fish*> _canLockfishList;
        _canLockfishList.clear();
        for (auto fish : _fishList)
        {
            if (fish == nullptr)
            {
                continue;
            }
            if (!fish->getCanTouch() || !fish->isVisible()) continue;
            
            if (fish->getFishKind() >= FISH_KIND_13)
            {
                if (fish->getFishKind() >= FISH_KIND_33) { //大闹天宫新增的6种鱼只有银龙和美人鱼可以锁
                    if (fish->getFishKind() < FISH_KIND_37) {
                        continue;
                    }
                }
                
                bool canLock = true;
                //如果当前鱼已经被锁定过一次，再次点击锁定的时候就要跳过
                for (auto id : _lockFishIdList)
                {
                    if (id == fish->getFishID())
                    {
                        fish->setFishIsLock(false);
                        canLock = false;
                        break;
                    }
                }
                //如果鱼不在屏幕中，锁定时要跳过
                if (fish->getPositionX() < 0 || 
					fish->getPositionX() > winSize.width || 
					fish->getPositionY() < 0 || 
					fish->getPositionY() > winSize.height)
                {
                    canLock = false;
                    continue;
                }
                if (canLock && isFirst)
                {
                    isFirst = false;//此处赋值仅执行一次
                    targetFish = fish;
                    maxMultiple = targetFish->_fish_multiple;
                    _canLockfishList.push_back(fish);
                }
                if (canLock && nullptr != onLockFishCallBack)
                {
                    hasLockFish = true;
                    if (maxMultiple < fish->_fish_multiple)
                    {
                        //新鱼的倍率 大等于 原来选择的最大倍率的鱼，都将其锁定目标进行更新
                        targetFish = fish;
                        maxMultiple = targetFish->_fish_multiple;
                    }
                    _canLockfishList.push_back(fish);
                }
            }
            else
            {
                //鱼不可锁定，继续遍历
                continue;
            }
        }
        if (hasLockFish)
        {
            //优先锁定的顺序集合
            
            bool isAllBreak = false;
            for (int m = 0; m < _fishFirstLockKind.size(); m++) {
                for (int n = 0; n < _canLockfishList.size(); n++) {
                    if (_fishFirstLockKind[m] == _canLockfishList.at(n)->getFishKind()) {
                        targetFish = _canLockfishList.at(n);
                        maxMultiple = _canLockfishList.at(n)->_fish_multiple;
                        isAllBreak = true;
                        break;
                    }
                }
                if (isAllBreak) {
                    break;
                }
            }
            
            //有可锁定的鱼，则继续执行锁定逻辑的后续相关步骤
            targetFish->setFishIsLock(true);
            _lockFishIdList.push_back(targetFish->getFishID());
            //向tableUi报告 锁定的鱼
            onLockFishCallBack(targetFish->getFishID());
        }
        return hasLockFish;//没有可以锁定的鱼，提示没有可以锁定的鱼（首次启动锁定的时候）或者直接取消锁定（自动切换的时候）
    }
    
	/************************************************************************/
	/* 切换其他可以锁定的鱼                                                 */
	/* return false  切换失败，没有可以切换的   true  切换成功              */
	/* 每次切换需要把之前锁定的鱼的ID从锁定的鱼的数组中移除					*/
	/************************************************************************/
	bool GameFishManagerV88::changeLockBigFish(INT lockFishID)
	{
		if (_fishList.size() <= 0 || _fishList[0] == nullptr || !_gameTableUI->_isLockFish)
		{
			return false;
		}
		auto winSize = Director::getInstance()->getWinSize();
		bool hasLockFish = false;
		//
		//有可锁定的鱼，则继续执行锁定逻辑的后续相关步骤
		for (auto fishID : _lockFishIdList)
		{
			auto lockFishOld = getFishById(fishID);
			if (lockFishOld != nullptr)
			{
				lockFishOld->setFishIsLock(false);
			}
			if (unLockFishCallBack != nullptr)
			{
				unLockFishCallBack(fishID);
			}
		}
		_lockFishIdList.clear();
		auto targetFish = getFishById(lockFishID);
		if (targetFish == nullptr)
		{
			unLockFishCallBack(lockFishID);
			return false;
		}

		//临时锁定鱼数组
		std::vector<int> tmpLockFish;
		tmpLockFish.clear();
		tmpLockFish.push_back(lockFishID);

		for (auto fish : _fishList)
		{
			if (fish == nullptr)
			{
				continue;
			}
			if (!fish->getCanTouch() || !fish->isVisible())
			{
				continue;
			}
			if (fish->getFishKind() >= FISH_KIND_13)
			{
                if (fish->getFishKind() >= FISH_KIND_33) { //大闹天宫新增的6种鱼只有银龙和美人鱼可以锁
                    if (fish->getFishKind() < FISH_KIND_37) {
                        continue;
                    }
                }
				//如果鱼不在屏幕中，锁定时要跳过
				if (fish->getPositionX() < 0 || 
					fish->getPositionX() > winSize.width || 
					fish->getPositionY() < 0 || 
					fish->getPositionY() > winSize.height)
				{
					continue;
				}
				hasLockFish = true;
				if (targetFish == fish)
				{
					continue;
				}
				tmpLockFish.push_back(fish->getFishID());
			}
			else
			{
				//鱼不可锁定，继续遍历
				continue;
			}
		}
		if (hasLockFish)
		{
			int count = tmpLockFish.size();
			if (count > 1)
			{
				
                bool isAllBreak = false;
                for (int m = 0; m < _fishFirstLockKind.size(); m++) {
                    for (int n = 0; n < tmpLockFish.size(); n++) {
                        auto _fishTemp = getFishById(tmpLockFish.at(n));
                        if (_fishFirstLockKind[m] == _fishTemp->getFishKind() && _fishTemp->getFishIsHandLock() == false) {
                            targetFish = _fishTemp;
                            isAllBreak = true;
                            _fishTemp->setFishIsHandLock(true);
                            break;
                        }
                    }
                    
                    if (m == _fishFirstLockKind.size() - 1 && isAllBreak == false) {
                        for (int x = 0; x < tmpLockFish.size(); x++) {
                             auto _fishObject = getFishById(tmpLockFish.at(x));
                            if (_fishObject) {
                                _fishObject->setFishIsHandLock(false);
                            }
                        }
                    }
                    
                    if (isAllBreak) {
                        break;
                    }
                }
                if (!isAllBreak) { //没有可优先锁定的鱼
                    int randFishID = -1;
                    do
                    {
                        randFishID = tmpLockFish.at(rand() % count);
                    } while (randFishID == lockFishID);
                    targetFish = getFishById(randFishID);
                }
                
			}
			else
			{
				targetFish = getFishById(lockFishID);
			}
			tmpLockFish.clear();
			targetFish->setFishIsLock(true);
			_lockFishIdList.push_back(targetFish->getFishID());
			//向tableUi报告 锁定的鱼
			onLockFishCallBack(targetFish->getFishID());
			return true;
		}
		//默认返回false，切换失败
		return false;
	}

    void GameFishManagerV88::manualSwitchLockedBigFish(INT fishID)
    {
		for (auto fishID : _lockFishIdList)
		{
			auto lockFishOld = getFishById(fishID);
			if (lockFishOld != nullptr)
			{
				lockFishOld->setFishIsLock(false);
			}
			if (unLockFishCallBack != nullptr)
			{
				unLockFishCallBack(fishID);	//
			}
		}
		_lockFishIdList.clear();
        auto fish = getFishById(fishID);
		if (fish != nullptr)
		{
			fish->setFishIsLock(true);
			_lockFishIdList.push_back(fishID);
			onLockFishCallBack(fishID);
		}
    }
    
    void GameFishManagerV88::manualUunlockBigFish()
    {
        if (_lockFishIdList.size() <= 0)
        {
            return;
        }
        for (auto fishID : _lockFishIdList)
        {
            auto fish = getFishById(fishID);
            if (fish != nullptr)
            {
                fish->setFishIsLock(false);
            }
            if (unLockFishCallBack != nullptr)
            {
                unLockFishCallBack(fishID);	//
            }
        }
        _lockFishIdList.clear();
    }
    
    void GameFishManagerV88::lockBigFishFail()
    {
        //强制取消锁定，且切换锁定按钮状态
        _gameTableUI->publicControlLockedFish(false);
    }
    
    CMD_C_CatchSweepFish GameFishManagerV88::catchSweepFish(WORD chairId , int fishId)
    {
        CMD_C_CatchSweepFish catch_sweep_fish;
        
        Fish* fish = getFishById(fishId);
        
        if (chairId >= PLAY_COUNT)
        {
            return catch_sweep_fish;
        }
        
        catch_sweep_fish.chair_id = chairId;
        catch_sweep_fish.fish_id = fishId;
        
        if (fish->getFishKind() == FISH_KIND_24)
        {
            for (auto smallFish : _fishList)
            {
                if (smallFish->isVisible())
                {
                    catch_sweep_fish.catch_fish_id[catch_sweep_fish.catch_fish_count++] = smallFish->getFishID();
                    if (catch_sweep_fish.catch_fish_count >= arraysize(catch_sweep_fish.catch_fish_id))
                    {
                        break;
                    }
                }
            }
        }
        else if(fish->getFishKind() == FISH_KIND_23)
        {
            auto fishPos = fish->getPosition();
            for (auto smallFish : _fishList)
            {
                if (smallFish->isVisible())
                {
                    if (abs(smallFish->getPositionX() - fishPos.x) <= FISH_KIND_23_AREA_BOOM && abs(smallFish->getPositionY() - fishPos.y) <= FISH_KIND_23_AREA_BOOM) {
                        catch_sweep_fish.catch_fish_id[catch_sweep_fish.catch_fish_count++] = smallFish->getFishID();
                        if (catch_sweep_fish.catch_fish_count >= arraysize(catch_sweep_fish.catch_fish_id))
                        {
                            break;
                        }
                        
                    }
                   
                }
            }
            
        }
        else if (/*fish->getFishKind() == FISH_KIND_31 || */fish->getFishKind() == FISH_KIND_32)
        {
            FishKind kind = fish->getFishKindEx();
            Vector<Fish*> pFish = getFishListByKind(kind);
            for (auto smallFish : pFish)
            {
                catch_sweep_fish.catch_fish_id[catch_sweep_fish.catch_fish_count++] = smallFish->getFishID();
                if (catch_sweep_fish.catch_fish_count >= arraysize(catch_sweep_fish.catch_fish_id))
                {
                    break;
                }
            }
        }
        
        return catch_sweep_fish;
    }
    
    void GameFishManagerV88::setConfig(CMD_S_GameConfig* pcene)
    {
        auto winSize = Director::getInstance()->getWinSize();
        BYTE i = 0;
        for ( ; i < FISH_KIND_COUNT; i++)
        {
            _fishMultiple[i]	= pcene->fish_multiple[i];
            _fishSpeed[i]		= pcene->fish_speed[i];
            auto scaleX = winSize.width / kResolutionWidth;
            auto scaleY = winSize.height / kResolutionHeight;
            float scale = 0.0f;
            if (winSize.width / winSize.height <= 1.5f)
            {
                //scaleX
                scale = scaleX;
            }
            else
            {
                scale = scaleY;
            }
            _fishSize[i].width	= pcene->fish_bounding_box_width[i] * scale;//winSize.width / kResolutionWidth;
            _fishSize[i].height = pcene->fish_bounding_box_height[i] * scale;//winSize.height / kResolutionHeight;
            _fishHitRadius[i]	= pcene->fish_hit_radius[i];
        }
        _fishSpeed[i] = 1.0f;
    }
    
    void GameFishManagerV88::init()
    {
        memset(_fishMultiple, 0x0, sizeof(_fishMultiple));
        memset(_fishSpeed, 0x0, sizeof(_fishSpeed));
        memset(_fishSize, 0x0, sizeof(_fishSize));
        memset(_fishHitRadius, 0x0, sizeof(_fishHitRadius));
    }
    
    /**
     *	本方法仅用于更新本地维护的数据，而不再继续将变动上传到服务器。
     *	这个方法将用于接收服务端下发的更新
     */
    void GameFishManagerV88::updateLockFishInfoListFromServer(bool isInsert, CMD_COM_ALLOW_LOCK_FISH* allowFish)
    {
        if (isInsert)
        {
            if (_canLockFishInfoList.find(allowFish->fishID) == _canLockFishInfoList.end())
            {
                //如果没有，则添加
                pair<int, CMD_COM_ALLOW_LOCK_FISH*> fishPair;
                fishPair.first = allowFish->fishID;
                fishPair.second = allowFish;
                _canLockFishInfoList.insert(fishPair);
                sendDataToServerOnLockFishInfoList(allowFish);
            }
        }
        else
        {
            //如果map已经有这个key值了，则移除
            if (_canLockFishInfoList.find(allowFish->fishID) != _canLockFishInfoList.end())
            {
                _canLockFishInfoList.erase(allowFish->fishID);
                sendDataToServerOnLockFishInfoList(allowFish);
            }
        }
    }
    
    /**
     *	对于可以锁定的鱼的map进行更新，不管是本地更新，还是服务端下发的，都直接丢进去，让其自己处理。
     */
    void GameFishManagerV88::updateLockFishInfoList(bool isInsert, CMD_COM_ALLOW_LOCK_FISH* cmd)
    {
        updateLockFishInfoListFromServer(isInsert, cmd);
    }
    
    void GameFishManagerV88::sendDataToServerOnLockFishInfoList(CMD_COM_ALLOW_LOCK_FISH* cmd)
    {
        RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, SUB_C_UPDATE_LOCK_FISH, cmd, sizeof(CMD_COM_ALLOW_LOCK_FISH));
    }
    
}
