#pragma once
#include <audioapi/core/AudioNode.h>

namespace audioapi {
class AudioBus;

class EngineNode : public AudioNode {
public:
  explicit EngineNode(BaseAudioContext* context);

protected:
  std::shared_ptr<AudioBus> processNode(const std::shared_ptr<AudioBus>& bus,
                                        int framesToProcess) override;
};
} // namespace audioapi
