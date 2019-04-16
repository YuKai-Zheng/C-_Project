#include "BRXJLuZhiUiV88.h"
#include "HNLobby/globel.h"
#include "cocostudio/CocoStudio.h"

namespace BRXJ {
    
#define LUZICOUNT 72
    
    const static char* ROOT = "Games/11100600/luzi_node_v88.csb";
    
    LuZhiV88::LuZhiV88() {
        _vecNtTitles.clear();
        _vecXianTitles.clear();
        _vecHeTitles.clear();

        shang = 0;
        tian = 0;
        di = 0;
        count = 0;
        luziIdx = 0;
    }

    LuZhiV88::~LuZhiV88() {

    }

    bool LuZhiV88::init() {
        if (!HNNode::create())
            return false;

        auto root = CSLoader::createNode(ROOT);
        this->addChild(root);

        _luZhiUi.Panel = static_cast<Layout*>(root->getChildByName("Panel"));

        initLuzhi();

        return true;
    }

    void LuZhiV88::initLuzhi() {
        _luZhiUi.nt = static_cast<Node*>(_luZhiUi.Panel->getChildByName("nt"));
        _luZhiUi.xian = static_cast<Node*>(_luZhiUi.Panel->getChildByName("xian"));
        _luZhiUi.he = static_cast<Node*>(_luZhiUi.Panel->getChildByName("he"));

        for (int i = 0; i < _luZhiUi.nt->getChildrenCount(); i++) {
            auto top = static_cast<Sprite*>(_luZhiUi.nt->getChildByName(to_string(i)));
            auto sky = static_cast<Sprite*>(_luZhiUi.xian->getChildByName(to_string(i)));
            auto bottom = static_cast<Sprite*>(_luZhiUi.he->getChildByName(to_string(i)));
            top->setVisible(false);
            sky->setVisible(false);
            bottom->setVisible(false);
            _vecNtTitles.push_back(top);
            _vecXianTitles.push_back(sky);
            _vecHeTitles.push_back(bottom);
        }

        _luZhiUi.leftButton = static_cast<Button*>(_luZhiUi.Panel->getChildByName("lefBtn"));
        _luZhiUi.rightButton = static_cast<Button*>(_luZhiUi.Panel->getChildByName("rightBtn"));
        _luZhiUi.leftButton->addTouchEventListener(CC_CALLBACK_2(LuZhiV88::btnCallBack, this));
        _luZhiUi.rightButton->addTouchEventListener(CC_CALLBACK_2(LuZhiV88::btnCallBack, this));
        _luZhiUi.rightButton->setVisible(false);
        _luZhiUi.Text_hePer = static_cast<Text*>(_luZhiUi.Panel->getChildByName("Text_hePer"));
        _luZhiUi.Text_xianPer = static_cast<Text*>(_luZhiUi.Panel->getChildByName("Text_xianPer"));
        _luZhiUi.Text_zhuangPer = static_cast<Text*>(_luZhiUi.Panel->getChildByName("Text_zhuangPer"));
    }

    Size LuZhiV88::GetLuziLayoutSize()
    {
        return _luZhiUi.Panel->getContentSize();
    }

    void LuZhiV88::updateLuzi(TGameLuzi* data) {
        _vecTGameLuZi.clear();
        shang = 0;
        tian = 0;
        di = 0;

        auto count = 0;
        for (int i =  0; i < LUZICOUNT; i++) {
            if (data[i].iShangMen < -1 || data[i].iShangMen > 1 || data[i].iTianMen < -1 || data[i].iTianMen > 1 || data[i].iXiaMen < -1 || data[i].iXiaMen > 1) {
                continue;
            }
            if (data[i].iShangMen == 0 && data[i].iTianMen == 0 && data[i].iXiaMen == 0) {
                continue;
            }

            if (data[i].iShangMen == 1)
                shang += 1;
            if (data[i].iTianMen == 1)
                tian += 1;
            if (data[i].iXiaMen == 1)
                di += 1;
            if (data[i].iShangMen == 1 || data[i].iTianMen == 1 || data[i].iXiaMen == 1)
                count++;

            _vecTGameLuZi.push_back(data[i]);
        }
        luziIdx = (int)_vecTGameLuZi.size() - 13;
        for (int i = luziIdx, j = 0; i < _vecTGameLuZi.size(); i++, j++) {
            _vecNtTitles.at(j)->setVisible(_vecTGameLuZi[i].iShangMen == 1);
            _vecHeTitles.at(j)->setVisible(_vecTGameLuZi[i].iTianMen == 1);
            _vecXianTitles.at(j)->setVisible(_vecTGameLuZi[i].iXiaMen == 1);
        }
        _luZhiUi.leftButton->setVisible(true);
        _luZhiUi.rightButton->setVisible(false);

        auto end = count < 72 ? count : 72;
        char str[30] = { 0 };
        sprintf(str, "%d%%", (int)(shang / end * 100));
        _luZhiUi.Text_zhuangPer->setString(str);
        sprintf(str, "%d%%", (int)(tian / end * 100));
        _luZhiUi.Text_xianPer->setString(str);
        sprintf(str, "%d%%", (int)(di / end * 100));
        _luZhiUi.Text_hePer->setString(str);
    }

    void LuZhiV88::btnCallBack(Ref* pShender, Widget::TouchEventType  type) {
        if (type != Widget::TouchEventType::ENDED)
            return;
        auto curSelectBet = static_cast<Button*>(pShender);
        auto name = curSelectBet->getName();
        if (name == "lefBtn") {
            luziIdx = luziIdx - 13;
            _luZhiUi.rightButton->setVisible(true);
            if (luziIdx <= 0) {
                luziIdx = 0;
                _luZhiUi.leftButton->setVisible(false);
                _luZhiUi.rightButton->setVisible(true);
            }
        } else if (name == "rightBtn") {
            luziIdx = luziIdx + 13;
            _luZhiUi.leftButton->setVisible(true);
            if (luziIdx >= (int)_vecTGameLuZi.size() - 13) {
                luziIdx = (int)_vecTGameLuZi.size() - 13;
                _luZhiUi.rightButton->setVisible(false);
                _luZhiUi.leftButton->setVisible(true);
            }
        }

        auto end = luziIdx + 13;
        if (end > _vecTGameLuZi.size()) {
            end = (int)_vecTGameLuZi.size();
        }
        for (int i = luziIdx, j = 0; i < end; i++, j++) {
            _vecNtTitles.at(j)->setVisible(_vecTGameLuZi[i].iShangMen == 1);
            _vecHeTitles.at(j)->setVisible(_vecTGameLuZi[i].iTianMen == 1);
            _vecXianTitles.at(j)->setVisible(_vecTGameLuZi[i].iXiaMen == 1);
        }
    }
}
