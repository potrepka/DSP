#pragma once

#include <audioapi/HostObjects/AudioNodeHostObject.h>

#include <memory>
#include <vector>

#include "EngineNode.h"

namespace audioapi {
using namespace facebook;

class EngineNodeHostObject : public AudioNodeHostObject {
public:
  explicit EngineNodeHostObject(const std::shared_ptr<EngineNode>& node)
      : AudioNodeHostObject(node) {
    // addGetters(JSI_EXPORT_PROPERTY_GETTER(EngineNodeHostObject, getter));
    // addSetters(JSI_EXPORT_PROPERTY_SETTER(EngineNodeHostObject, setter));
    // addFunctions(JSI_EXPORT_FUNCTION(EngineNodeHostObject, function));
  }

  // JSI_PROPERTY_GETTER(getter) {
  //   auto engineNode = std::static_pointer_cast<EngineNode>(node_);
  //   return {engineNode->someGetter()};
  // }

  // JSI_PROPERTY_SETTER(gain) {
  //   auto engineNode = std::static_pointer_cast<EngineNode>(node_);
  //   engineNode->someSetter(value.getNumber());
  // }

  // JSI_HOST_FUNCTION(function) {
  //  auto obj = args[0].getObject(runtime);
  //}
};
} // namespace audioapi
