#ifndef __PLATFORM_V2_H__
#define __PLATFORM_V2_H__

#include "HNLobby/GamePlatform.h"

class GamePlatformV2 : public GamePlatform {
public:
    virtual bool init() override;
    CREATE_FUNC(GamePlatformV2);

public:
    virtual void initPlatform()override;
    virtual void createGameListLayer()override;

    virtual void createDeskListLayer(ComRoomInfo *roomInfo)override;
    virtual void updateGameList(bool success, const std::string &message) override;
    virtual void updateGameUserCount(UINT Id, UINT count) override;
    virtual void updateRoomList(bool success, const std::string& message) override;
    virtual void updateRoomUserCount(UINT roomID, UINT userCount) override;
    virtual void onSpreadInfo(bool result, bool bInfo, const std::string& msg) override;
    virtual void createGameListAndRoom()override;
    virtual void handleReturnButton() override;
    virtual void openNewYearActivity()override;
    virtual void setLayerType(LayerType layerType)override;

public:
    //顶部功能栏的变幻动作
    void showTopAction(bool isOut);
    //底部功能栏的变幻动作
    void showBottomAction(bool isOut);
    //创建房间列表
    void createRoomListLayer();

private:

protected:
    virtual void createGameTypeLayer()override;
};

#endif // __PLATFORM_V2_H__
