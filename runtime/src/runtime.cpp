#include "runtime.h"

#include <string.h>
#include <stdio.h>

#include <v8.h>
using namespace v8;

#include "glue.h"

void Runtime::Initialize() {
  V8::Initialize();
}

const char* mainScript = "var r = add(2,2); add(r, 10);";

static Local<String> MainSource() {
  HandleScope scope;
  return scope.Close(String::New(mainScript, strlen(mainScript)));
}

JS_FUNCTION_CB(add)
  double x = info.getArgument(0).ToNumberValue(),
         y = info.getArgument(1).ToNumberValue();

  printf("result is %f\n", x + y);

  JS_RETURN(JSNumber(x + y));
JS_FUNCTION_END

void Runtime::Run() {
  HandleScope scope;

  Persistent<Context> context = Context::New();
  Context::Scope contextScope(context);

  Local<Object> global = context->Global();
  global->Set(String::NewSymbol("add"), JSFunction::NewNative(add).V8Value());

  Local<Script> script = Script::Compile(MainSource());
  if (script.IsEmpty()) {
    // TODO: report error
    return;
  }

  Local<Value> result = script->Run();
  if (result.IsEmpty()) {
    // TODO: report exception
    return;
  }
}

