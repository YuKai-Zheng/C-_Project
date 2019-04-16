#include "ThirteenZhangWrapperv88.h"

namespace ThirteenZhang
{
    bool IWrapperV88::doLoad(Node* component)
    {
        CCAssert(nullptr != component, "nullptr == _component");
        if (nullptr == component) return false;
        
        _component = component;
        
        load();
        
        return true;
    }
    
    void IWrapperV88::doUnload()
    {
        unload();
        _component = nullptr;
    }
    
    void IWrapperV88::doRestore()
    {
        restore();
    }
    
    void IWrapperV88::setVisible(bool visible)
    {
        if (_visible != visible)
        {
            _visible = visible;
            _component->setVisible(_visible);
        }
    }
    
    bool IWrapperV88::isVisible() const
    {
        return _visible;
    }
    
    int IWrapperV88::getLocalZOrder() const
    {
        return _component->getLocalZOrder();
    }
    
    const Size& IWrapperV88::getContentSize() const
    {
        return _component->getContentSize();
    }
    
    void IWrapperV88::setContentSize(const Size& contentSize)
    {
        _component->setContentSize(contentSize);
    }
    
    const Vec2& IWrapperV88::getPosition() const
    {
        return _component->getPosition();
    }
    
    void IWrapperV88::getPosition(float* x, float* y) const
    {
        _component->getPosition(x, y);
    }
    
    void IWrapperV88::setPosition(float x, float y)
    {
        _component->setPosition(x, y);
    }
    
    void IWrapperV88::setPosition(const Vec2 &position)
    {
        _component->setPosition(position);
    }
    
    void IWrapperV88::setTag(int tag)
    {
        _component->setTag(tag);
    }
    
    int IWrapperV88::getTag() const
    {
        return _component->getTag();
    }
    
    void IWrapperV88::setName(const std::string& name)
    {
        _component->setName(name);
    }
    
    std::string IWrapperV88::getName()
    {
        return _component->getName();
    }
    
    void IWrapperV88::addChild(Node * child)
    {
        _component->addChild(child);
    }
    
    void IWrapperV88::addChild(Node * child, int localZOrder)
    {
        _component->addChild(child, localZOrder);
    }
    
    void IWrapperV88::addChild(Node* child, int localZOrder, int tag)
    {
        _component->addChild(child, localZOrder, tag);
    }
    
    void IWrapperV88::addChild(Node* child, int localZOrder, const std::string &name)
    {
        _component->addChild(child, localZOrder, name);
    }
    
    void IWrapperV88::removeChild(Node* child, bool cleanup)
    {
        _component->removeChild(child, cleanup);
    }
    
    void IWrapperV88::removeChild(int tag, bool cleanup)
    {
        _component->removeChildByTag(tag, cleanup);
    }
    
    void IWrapperV88::removeChild(const std::string &name, bool cleanup)
    {
        _component->removeChildByName(name, cleanup);
    }
}
