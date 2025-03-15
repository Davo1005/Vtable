#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>



class TypeInfo
{
public:
    TypeInfo(const std::string& name, const std::string& base_name) : class_name(name), base_name(base_name) {}
    virtual ~TypeInfo() {}
    const std::string& GetName() const { return class_name; }
    const std::string& GetBaseName() const { return base_name; }
    std::string class_name; 
    std::string base_name;
    virtual void print() {
        std::cout << "TypeInfo" << std::endl;
    }
};

struct VTable {
    void (*printFunc)(void* obj);  
    TypeInfo* typeInfo;   
    int callOffset;
    int topOffset;      
    int baseOffset;   
};

class Base
{
public:
    static VTable vtable;
    static TypeInfo typeInfo;
    
    virtual ~Base() {}
    virtual void print() 
    { 
        std::cout << "Base" << std::endl; 
    }

    virtual VTable* getVTable() {
        return &vtable;
    }
};

TypeInfo Base::typeInfo("Base", "");
VTable Base::vtable = {
    [](void* obj) { static_cast<Base*>(obj)->print(); }, 
    &Base::typeInfo, TypeInfo
    0,
    0,
    0
};


class Derived : public Base
{
public:
    static VTable vtable;
    static TypeInfo typeInfo;
    
    virtual ~Derived() {}
    virtual void print() 
    { 
        std::cout << "Derived" << std::endl; 
    }

    virtual VTable* getVTable() {
        return &vtable;
    }
};

TypeInfo Derived::typeInfo("Derived", "Base");
VTable Derived::vtable = {
    [](void* obj) { static_cast<Derived*>(obj)->print(); }, 
    &Derived::typeInfo, 
    sizeof(Base),
    0,
    0
};

int main() {
    Base* obj = new Derived();

    
    VTable* vtbl = obj->getVTable();

    
    vtbl->printFunc(obj); 
   
    vtbl->typeInfo->print(); 

    std::cout << "Class Name: " << vtbl->typeInfo->GetName() << std::endl;
    std::cout << "Base Class Name: " << vtbl->typeInfo->GetBaseName() << std::endl;
    std::cout << "Call Offset: " << vtbl->callOffset << std::endl;
    std::cout << "Top Offset: " << vtbl->topOffset << std::endl;
    std::cout << "Base Offset: " << vtbl->baseOffset << std::endl;

    delete obj;
    return 0;
}