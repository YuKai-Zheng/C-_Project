//
//  CycScrollView.cpp
//  LibHNUI
//
//  Created by Sunsong Ye on 16/11/25.
//  Copyright © 2016年 redbird. All rights reserved.
//

#include "CycScrollView.h"

void CycScrollView::onExit() {
    Widget::onExit();

}

bool CycScrollView::init() {
    if (!Widget::init()) {
        return false;
    }
    winSize = Director::getInstance()->getWinSize();
    m_ViewSize = Size(winSize.width, 500);
    firstNode = nullptr;
    return true;
}

void CycScrollView::loadScrollView(std::vector<Button*> listNode,float blankDis) {
    m_blankDis = blankDis;

    int index=0;
    CycNode* prevNode = nullptr;
    for (cocos2d::ui::Button* button : listNode) {//定义成双向链表节点
        CycNode* curNode = new CycNode();
        curNode->node = button;
        //   button->setUserData(&curNode);//将节点反向回双向链表
        curNode->preNode = prevNode;

        if (prevNode) {
            prevNode->nextNode = curNode;
        }

        prevNode = curNode;

        if (firstNode == nullptr) {
            firstNode = curNode;
        }

        if (index == listNode.size() - 1) {
            curNode->nextNode = firstNode;
            firstNode->preNode = curNode;
        }

        index++;
    }


    index = 0;
    for (cocos2d::ui::Button* n : listNode) {
        // n->setAnchorPoint(Point::ZERO);
        n->setPosition(Point(index*(n->getContentSize().width + m_blankDis), m_ViewSize.height*0.5f));
        addChild(n);
        index++;
    }

    canScroll = true;

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(CycScrollView::touchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(CycScrollView::touchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(CycScrollView::touchEnded,this);
    listener->onTouchCancelled = CC_CALLBACK_2(CycScrollView::touchCanceled,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool CycScrollView::touchBegan(Touch* touch,Event*) {
    curPoint = convertToNodeSpace(touch->getLocation());
    selNode = nullptr;
    CycNode* pcurNode = firstNode;
    do {
        Size nodeSize = pcurNode->node->getContentSize();
        Point p = pcurNode->node->getPosition();
        Rect r = Rect(p.x, p.y, nodeSize.width, nodeSize.height);
        if (r.containsPoint(curPoint)) {
            selNode = pcurNode->node;
            if (owner) {
                owner->dragBeginNode(pcurNode->node, touch);
            }
            break;
        }
        pcurNode = pcurNode->nextNode;
    } while (pcurNode != firstNode);

    //}
    if (curPoint.y > 0 && curPoint.y < m_ViewSize.height) {
        return true;
    }
    return false;
}

void CycScrollView::touchEnded(Touch* touch, Event*) {
    Vec2 newPoint = convertToNodeSpace(touch->getLocation());

    if (owner&&selNode) {
        owner->dragEndNode(selNode, touch);

    }
}

void CycScrollView::touchMoved(Touch* touch,Event*) {
    if (!canScroll) {
        return;
    }

    Point newPoint =  convertToNodeSpace(touch->getLocation());
    float deltaX = newPoint.x - curPoint.x;
    CycNode* pmoveFirst = NULL;
    CycNode* pcurNode = this->firstNode;
    int index = 0;
    float y = 0;
    if (deltaX < 0) { //向左移
        float maxX = 0;
        do {
            Point nPoint = (pcurNode->node)->getPosition();
            float preX = nPoint.x;
            if (preX > maxX&&preX <= winSize.width) {
                pmoveFirst = pcurNode;
                maxX = preX;
                y = nPoint.y;
            }
            index++;
            pcurNode = pcurNode->nextNode;
        } while (pcurNode != firstNode);
        pmoveFirst->nextNode->node->setPosition(Point(maxX + (pmoveFirst->node->getContentSize().width+m_blankDis), y));
    }

    if (deltaX > 0) { //向右移
        float minX = 1200;
        do {
            Point nPoint = (pcurNode->node)->getPosition();
            float preX = nPoint.x;
            if (preX < minX&&preX >= 0) {
                pmoveFirst = pcurNode;
                minX = preX;
                y = nPoint.y;
            }
            index++;
            pcurNode = pcurNode->nextNode;
        } while (pcurNode != firstNode);
        pmoveFirst->preNode->node->setPosition(Point(minX - (pmoveFirst->node->getContentSize().width + m_blankDis), y));
    }

    if (pmoveFirst) {
        pcurNode = pmoveFirst;
        do {
            Point nPoint = (pcurNode->node)->getPosition();
            float newX = nPoint.x + deltaX;
            (pcurNode->node)->setPosition(Point(newX, y));
            pcurNode = pcurNode->nextNode;
        } while (pcurNode != pmoveFirst);
    }
    if (owner&&selNode) {
        owner->dragMoveNode(selNode, touch);
    }
    curPoint = newPoint;
}

void CycScrollView::touchCanceled(Touch* touch,Event*) {
    Vec2 newPoint = convertToNodeSpace(touch->getLocation());
    //
    if(owner && selNode) {
        owner->dragEndNode(selNode, touch);
    }
}