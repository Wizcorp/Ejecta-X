#ifndef __NS_OBJECT_FACTORYE_H__
#define __NS_OBJECT_FACTORYE_H__

#include <map>
#include <string>


class NSObject;

typedef void* (*SEL)(void);
typedef SEL (*callFuc)(void);
typedef NSObject *(*funcPtr)(void);

struct NSObjectFactory {

    typedef std::map<std::string, funcPtr> map_type;
    typedef std::map<std::string, callFuc> fuc_map_type;

private:

    static map_type * m_map;
    static fuc_map_type * fuc_map;

protected:

public:

    static NSObject * createInstance(std::string const& s){
        map_type::iterator it = getMap()->find(s);
        if(it == getMap()->end())
            return 0;
        return it->second();
    }
 
    static SEL callFaction(std::string const& s) {
        fuc_map_type::iterator it = getFunctionMap()->find(s);
        if(it == getFunctionMap()->end())
            return 0;
        return it->second();
 
    }

    static map_type * getMap(){
        // never delete'ed. (exist until program termination)
        // because we can't guarantee correct destruction order 
        if(!m_map) { m_map = new map_type; } 
        return m_map;
    }

    static fuc_map_type* getFunctionMap() {
 
        if(!fuc_map) { fuc_map = new fuc_map_type(); } 
 
        return fuc_map; 
 
    }
    
};

template<typename T> NSObject * createT() { return (NSObject *)(new T); }


template<typename T>
struct NSObjectRegister : NSObjectFactory { 
    NSObjectRegister(std::string const& s) { 
		funcPtr func = &createT<T>;
        getMap()->insert(std::make_pair(s, func));
    }
};

#define REFECTION_CLASS_IMPLEMENT_DEFINE(NAME) \
    virtual std::string toString(){return #NAME;}; \
    static NSObjectRegister<NAME> registerNSObject

#define REFECTION_CLASS_IMPLEMENT(NAME) \
    NSObjectRegister<NAME> NAME::registerNSObject(#NAME)

#define NSClassFromString(NAME) \
    NSObjectFactory::createInstance(NAME)

struct FuctionRegister : NSObjectFactory { 
    FuctionRegister(std::string const& s, callFuc f) { 
        getFunctionMap()->insert(std::make_pair(s, f));
    }
 
};
 
#define REFECTION_FUNCTION_IMPLEMENT(NAME) \
    static FuctionRegister* f_reg_##NAME = new FuctionRegister(#NAME, (callFuc)NAME)

#define NSSelectorFromString(NAME) \
    NSObjectFactory::callFaction(NAME)

#endif // __NS_OBJECT_FACTORYE_H__
