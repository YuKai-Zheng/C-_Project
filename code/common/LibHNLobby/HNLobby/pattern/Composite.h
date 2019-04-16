#ifndef _COMPOSITE_H_
#define _COMPOSITE_H_

#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

/*
Component抽象基类，为组合中的对象声明接口,声明了类共有接口的缺省行为(如这里的Add,Remove,GetChild函数),
声明一个接口函数可以访问Component的子组件.
*/
class IComponent {
public:
    //纯虚函数，只提供接口，没有默认的实现
    virtual void Operation() = 0;

    // 虚函数,提供接口,有默认的实现就是什么都不做
    virtual void Add(IComponent*);
    virtual void Remove(IComponent*);
    virtual IComponent* GetChild(int index);
    virtual ~IComponent();

protected:
    IComponent();
};


//Leaf是叶子结点,也就是不含有子组件的结点类，所以不用实现Add、Remove、GetChild等方法
class Leaf :public IComponent, public Node {
public:
    //只实现Operation接口
    virtual void Operation();
    Leaf();
    ~Leaf();
};

//Composite：含有子组件的类
class Composite :public IComponent ,public Node {
public:
    Composite();
    ~Composite();
    //实现所有接口
    void Operation();
    void Add(IComponent*);
    void Remove(IComponent*);
    IComponent* GetChild(int index);

private:
    //这里采用vector来保存子组件
    vector<IComponent*> m_ComVec;
};
#endif