#ifndef __GameNotice_h__
#define __GameNotice_h__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/HNSprite.h"



struct NotifyItem {
    int nType;
    int repeat;
    std::string message;
    NotifyItem() {};
    NotifyItem(const std::string& message, int repeat, int nType) : message(message), repeat(repeat), nType(nType) {}
};

class GameNotice : public HNLayer {
    enum {
		SilverSaleManNotice = -11,			//银商公告
        WholeServerFixNotice = 0,
        WuHuaNiuNotice = 1,
    };
protected:
    GameNotice();
    virtual ~GameNotice();
public:
    /*
     * create a notice dialog.
     * @param parent node.
     * @param dialog's context.
     * @param dialog's title.
     */

    //static GameNotice* createGameNotice(const char* stencil_Path);
    CREATE_FUNC(GameNotice);
    //virtual bool init(const char* stencil_Path);
    virtual bool init();
    void postMessage(const std::string& message, int nType, int repeat = 0);
    void setCallBack(std::function<void ()> sure);
    void setNoticTextColor(Color4B color);

    void openNotic(Node* parent, INT tag, const std::string message, INT count, int nType = 0);
    static GameNotice* getNotic(Node* parent);

    void actionOut(float dt);
    void actionIn(float dt);


private:
    HNSprite* _bg;
    Vec2 _bg_pos;
    Size _showSize;
    Size _wordSize;
    Label* _text;
    std::list<NotifyItem> _notifyMsgs;
    NotifyItem	_lastNotifyItem;
    bool _runing;

private:
    void start();
    void stop();
    void updateNextMessage();
    //void updateResForCN(bool forCN);
	void updateResForCN(int iType);

private:
    void moveWord(float delta);

private:
    std::function<void ()> _sure;
};

#endif // __GameNotice_h__
