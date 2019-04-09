#include "jmpexceptions.hpp"

JeObject::JeObject()
{
    // push to the stack during creation object 
    
    if (_je_stack_root != nullptr) {
        prev = _je_stack_root->obj;
        _je_stack_root->obj = this;
    }
}

JeObject::~JeObject()
{
    if (_je_stack_root != nullptr && prev != nullptr) {
        _je_stack_root->obj = prev;
        prev = nullptr;
    }
}

_JeStackElement::_JeStackElement() :
    obj(nullptr), prev(_je_stack_root)
{
    _je_stack_root = this;
}

_JeStackElement::~_JeStackElement()
{
    _je_stack_root = prev;
}

void je_unstack(int code)
{
    if (code == 0) {
        return;
    }
    
    _JeStackElement* curr_element = _je_stack_root;
    if (_je_stack_root == nullptr) {
        std::cout << "Unhandled exception, finishing program" << std::endl;
        exit(-1);
        return;
    }
    
    JeObject* curr_obj = _je_stack_root->obj;
    
    // destructing objects
    while (curr_obj != nullptr) { 
        JeObject* tmp = curr_obj;
        curr_obj = curr_obj->prev;
        tmp->~JeObject();
    }
    if (curr_element != nullptr) {
        _je_stack_root = curr_element->prev;
    }
    
    longjmp(curr_element->buf, code); // back to state
    
    return;
}
