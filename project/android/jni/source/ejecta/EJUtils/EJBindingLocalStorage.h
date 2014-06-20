#ifndef __EJ_BINDING_LOCALSTORAGE_H__
#define __EJ_BINDING_LOCALSTORAGE_H__

#include "EJBindingBase.h"


class EJBindingLocalStorage : public EJBindingBase {
	
public:

	EJBindingLocalStorage();
	~EJBindingLocalStorage();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingLocalStorage);

	virtual string superclass(){ return EJBindingBase::toString();};

	EJ_BIND_FUNCTION_DEFINE(getItem, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(setItem, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(removeItem, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(clear, ctx, argc, argv );
};

#endif // __EJ_BINDING_LOCALSTORAGE_H__
