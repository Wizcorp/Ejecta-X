#include "EJBindingBase.h"

void _ej_class_finalize(JSObjectRef object) {
	EJBindingBase* instance = (EJBindingBase*) JSObjectGetPrivate(object);
	instance->autorelease();
}

char * NSDataFromString( NSString *str ) {
	int len = str->length() + 1;
	char * d = (char *)malloc(len * sizeof(char));
	memset(d, 0, len);
#ifdef _WINDOWS
	_snprintf_s(d, len, len, "%s", str->getCString());
#else
	snprintf(d, len, "%s", str->getCString());
#endif
	return d;
}

// @implementation EJBindingBase
EJBindingBase::EJBindingBase() : jsObject(0)
{

}

EJBindingBase::EJBindingBase(JSContextRef ctxp, JSObjectRef obj, size_t argc, const JSValueRef argv[])
{
	jsObject = obj;
}

EJBindingBase::~EJBindingBase()
{

}

void EJBindingBase::initWithContext(JSContextRef ctxp, JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
	jsObject = obj;
}

JSObjectRef EJBindingBase::createJSObjectWithContext(JSContextRef ctx, EJBindingBase* instance)
{
 	JSClassRef bindingClass = EJApp::instance()->getJSClassForClass(instance);
 	JSObjectRef obj = JSObjectMake( ctx, bindingClass, NULL );
 	JSValueProtect(ctx, obj);
 	// Attach the native instance to the js object
 	JSObjectSetPrivate( obj, (void *)instance );
 	JSValueUnprotect(ctx, obj);

 	return obj;
}

JSClassRef EJBindingBase::getJSClass (EJBindingBase* ej_obj){
	// Gather all class methods that return C callbacks for this class or it's parents
	NSDictionary * methods = new NSDictionary();
	NSDictionary * properties = new NSDictionary();

	string base_obj = ej_obj->toString();

	NSObjectFactory::fuc_map_type* base = NSObjectFactory::getFunctionMap();
	for(NSObjectFactory::fuc_map_type::iterator it = base->begin(); it != base->end(); it++)
	{
		string name = it->first;
		string base_obj_tmp = base_obj;
		if( name.find("_func_") != string::npos ) {
			int pos = name.find("_func_");
			int is_member_func = name.find(base_obj_tmp);
			bool n_pos = (is_member_func == string::npos);
			while(n_pos) {
				EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(base_obj_tmp);
				base_obj_tmp = pClass->superclass();
				is_member_func = name.find(base_obj_tmp);
				if ((is_member_func != string::npos) || (base_obj_tmp.find("EJBindingBase") != string::npos))
				{
					n_pos = false;
				}
			}
			if (is_member_func != string::npos)
			{
			   	methods->setObject(NSStringMake(name.substr(pos + strlen("_func_"))), name);
			}
		}
		else if( name.find("_get_") != string::npos ) {
			int pos = name.find("_get_");
			int is_member_func = name.find(base_obj_tmp);
			bool n_pos = (is_member_func == string::npos);
			while(n_pos) {
				EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(base_obj_tmp);
				base_obj_tmp = pClass->superclass();
				is_member_func = name.find(base_obj_tmp);
				if ((is_member_func != string::npos) || (base_obj_tmp.find("EJBindingBase") != string::npos))
				{
					n_pos = false;
				}
			}
			if (is_member_func != string::npos)
			{
				// We only look for getters - a property that has a setter, but no getter will be ignored
			  	properties->setObject(NSStringMake(name.substr(pos + strlen("_get_"))), name);
			}
		}
	} 
	
	
	// Set up the JSStaticValue struct array
	JSStaticValue * values = (JSStaticValue *)calloc( properties->count() + 1, sizeof(JSStaticValue) );
	
	int i = 0;
	NSDictElement* pElement = NULL;
	NSObject* pObject = NULL;

	NSDICT_FOREACH(properties, pElement)
	{
		pObject = pElement->getObject()->copy();
		string key_name = pElement->getStrKey();
	    NSString* name = (NSString*)pObject;

		char * nameData = NSDataFromString( name );
		
		char** p_name = const_cast<char**>(&values[i].name);
		*p_name = nameData;
		values[i].attributes = kJSPropertyAttributeDontDelete;
		SEL get = NSSelectorFromString(key_name);
		values[i].getProperty = (JSObjectGetPropertyCallback)get;
		
		// Property has a setter? Otherwise mark as read only
		int pos = key_name.find("_get_");
		key_name = key_name.replace(pos, strlen("_get_"), "_set_");
		SEL set = NSSelectorFromString(key_name);
		if( set ) {
			values[i].setProperty = (JSObjectSetPropertyCallback)set;
		}
		else {
			values[i].attributes |= kJSPropertyAttributeReadOnly;
		}

		i++;
	}
	
	// Set up the JSStaticFunction struct array
	JSStaticFunction * functions = (JSStaticFunction *)calloc( methods->count() + 1, sizeof(JSStaticFunction) );
	
	i = 0;
	pElement = NULL;
	pObject = NULL;

	NSDICT_FOREACH(methods, pElement)
	{

		pObject = pElement->getObject()->copy();
		string key_name = pElement->getStrKey();
	    NSString* name = (NSString*)pObject;

		char * nameData = NSDataFromString( name );

		char** p_name = const_cast<char**>(&functions[i].name);
		*p_name = nameData;
		functions[i].attributes = kJSPropertyAttributeDontDelete;
		
		SEL call = NSSelectorFromString(key_name);
		functions[i].callAsFunction = (JSObjectCallAsFunctionCallback)call;

		i++;
	}
	
	JSClassDefinition classDef = kJSClassDefinitionEmpty;
	classDef.finalize = _ej_class_finalize;
	classDef.staticValues = values;
	classDef.staticFunctions = functions;
	JSClassRef js_class = JSClassCreate(&classDef);
	
	free( values );
	free( functions );
	
	properties->release();
	methods->release();

	return js_class;
}

REFECTION_CLASS_IMPLEMENT(EJBindingBase);
