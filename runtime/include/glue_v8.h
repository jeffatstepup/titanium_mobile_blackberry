#include <v8.h>

class JSValue {
 public:
   JSValue(v8::Handle<v8::Value> value)
     : mValue(value)
   { }

   v8::Handle<v8::Value> V8Value() const {
     return mValue;
   }

   bool IsUndefined() const {
     return mValue->IsUndefined();
   }

   bool IsNull() const {
     return mValue->IsNull();
   }

   bool IsNumber() const {
     return mValue->IsNumber();
   }

   double ToNumberValue() const {
     return mValue->NumberValue();
   }

 protected:
   v8::Handle<v8::Value> mValue;
};

class JSNumber : public JSValue {
 public:
   JSNumber(double value)
     : JSValue(v8::Number::New(value))
   { }
};

class JSObject : public JSValue {
 public:
   JSObject(v8::Handle<v8::Value> value)
     : JSValue(value)
   { }

   // TODO: support setting property attributes.
   void Set(JSValue key, JSValue value) {
     mValue.As<v8::Object>()->Set(key.V8Value(), value.V8Value());
   }
};

typedef v8::InvocationCallback JSNativeFunctionCallback;

class JSFunction : public JSObject {
 public:
   JSFunction(v8::Handle<v8::Value> value)
     : JSObject(value)
   { }

   static JSFunction NewNative(JSNativeFunctionCallback cb) {
     v8::Local<v8::Function> func = v8::FunctionTemplate::New(cb)->GetFunction();
     return JSFunction(func);
   }
};

class JSInvocationInfo {
 public:
   JSInvocationInfo(const v8::Arguments& args)
     : mArgs(args)
   { }

   JSValue getArgument(int index) {
     return JSValue(mArgs[index]);
   }

 private:
   const v8::Arguments& mArgs;
};

#define JS_FUNCTION_CB(name) \
v8::Handle<v8::Value> name(const v8::Arguments& G_args) { \
  v8::HandleScope G_handleScope; \
  JSInvocationInfo info(G_args);

#define JS_FUNCTION_END \
}

#define JS_RETURN(value) \
  return G_handleScope.Close(value.V8Value());

