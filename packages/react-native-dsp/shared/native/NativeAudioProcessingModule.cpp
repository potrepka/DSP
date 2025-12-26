#include "NativeAudioProcessingModule.h"

#include <audioapi/HostObjects/BaseAudioContextHostObject.h>

#include <functional>
#include <iostream>
#include <memory>

#include "EngineNode.h"
#include "EngineNodeHostObject.h"

namespace facebook::react {

NativeAudioProcessingModule::NativeAudioProcessingModule(
    std::shared_ptr<CallInvoker> jsInvoker)
    : NativeAudioProcessingModuleCxxSpec(std::move(jsInvoker)) {}

void NativeAudioProcessingModule::injectCustomProcessorInstaller(
    jsi::Runtime& runtime) {
  auto installer = createInstaller(runtime);
  runtime.global().setProperty(runtime, "createCustomProcessorNode", installer);
}

jsi::Function NativeAudioProcessingModule::createInstaller(
    jsi::Runtime& runtime) {
  return jsi::Function::createFromHostFunction(
      runtime, jsi::PropNameID::forAscii(runtime, "createCustomProcessorNode"),
      0,
      [](jsi::Runtime& runtime, const jsi::Value& thisVal,
         const jsi::Value* args, size_t count) {
        auto object = args[0].getObject(runtime);
        auto context =
            object.getHostObject<audioapi::BaseAudioContextHostObject>(runtime);
        if (context != nullptr) {
          auto node =
              std::make_shared<audioapi::EngineNode>(context->context_.get());
          auto nodeHostObject =
              std::make_shared<audioapi::EngineNodeHostObject>(node);
          return jsi::Object::createFromHostObject(runtime, nodeHostObject);
        }
        return jsi::Object::createFromHostObject(runtime, nullptr);
      });
}
} // namespace facebook::react
