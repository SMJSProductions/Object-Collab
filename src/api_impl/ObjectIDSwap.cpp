#include <ObjectIDSwap.hpp>

using namespace object_collab;

ObjectIDSwap::ObjectIDSwap(GameObject* object, int newID): m_originalID(object->m_objectID), m_object(object) {
    this->set(newID);
}

ObjectIDSwap::~ObjectIDSwap() {
    this->reset();
}

void ObjectIDSwap::set(int newID) {
    m_object->m_objectID = newID;
}

void ObjectIDSwap::reset() {
    m_object->m_objectID = m_originalID;
}