#include "Composite.h"
#include <iostream>

using namespace std;


IComponent::IComponent() {
}

IComponent::~IComponent() {
}

void IComponent::Add(IComponent* com) {
    cout << "add" << endl;
}

void IComponent::Remove(IComponent* com) {
}

void IComponent::Operation() {
    cout << "IComponent::Operation" << endl;
}

IComponent* IComponent::GetChild(int index) {
    return NULL;
}


Leaf::Leaf() {
}

Leaf::~Leaf() {
}

void Leaf::Operation() {
    cout << "Leaf::Operation" << endl;
}

Composite::Composite() {
}

Composite::~Composite() {
}

void Composite::Add(IComponent* com) {
    this->m_ComVec.push_back(com);
}

void Composite::Remove(IComponent* com) {
    //this->m_ComVec.erase(com);
    //m_ComVec.erase(com);
}

void Composite::Operation() {
    cout << "Composite::Operation" << endl;
    vector<IComponent*>::iterator iter = this->m_ComVec.begin();
    for (; iter != this->m_ComVec.end(); iter++) {
        (*iter)->Operation();
    }
}

IComponent* Composite::GetChild(int index) {
    if (index < 0 || index > this->m_ComVec.size()) {
        return NULL;
    }
    return this->m_ComVec[index];
}