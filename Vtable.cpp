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
    void (*printFunc[2])(void* obj);  
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
    void print() 
    { 
        std::cout << "Base" << std::endl; 
    }

    void print2() 
    { 
        std::cout << "Base2" << std::endl; 
    }

    virtual VTable* getVTable() {
        return &vtable;
    }
};

TypeInfo Base::typeInfo("Base", "");
VTable Base::vtable = {
    {
        [](void* obj) { static_cast<Base*>(obj)->print(); },
        [](void* obj) { static_cast<Base*>(obj)->print2(); },
    },
    &Base::typeInfo, 
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
    void print() 
    { 
        std::cout << "Derived" << std::endl; 
    }

    void print2() 
    { 
        std::cout << "Derived2" << std::endl; 
    }

    virtual VTable* getVTable() {
        return &vtable;
    }
};

TypeInfo Derived::typeInfo("Derived", "Base");
VTable Derived::vtable = {
    {
        [](void* obj) { static_cast<Derived*>(obj)->print(); }, 
        [](void* obj) { static_cast<Derived*>(obj)->print2(); },
    },
    &Derived::typeInfo, 
    sizeof(Base),
    0,
    0
};
int index = 0;
template <typename t>
void print(t* obj) {
    VTable* vtbl = obj->getVTable();
    vtbl->printFunc[index](obj);
}


int main() {
    Base* obj = new Derived();
   
    VTable* vtbl = obj->getVTable();
    
    print(obj);

   
    vtbl->typeInfo->print(); 

    std::cout << "Class Name: " << vtbl->typeInfo->GetName() << std::endl;
    std::cout << "Base Class Name: " << vtbl->typeInfo->GetBaseName() << std::endl;
    std::cout << "Call Offset: " << vtbl->callOffset << std::endl;
    std::cout << "Top Offset: " << vtbl->topOffset << std::endl;
    std::cout << "Base Offset: " << vtbl->baseOffset << std::endl;

    delete obj;
    return 0;
}