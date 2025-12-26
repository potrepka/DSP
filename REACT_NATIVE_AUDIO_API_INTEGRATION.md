# React Native Audio API Integration Strategy

## Executive Summary

This document outlines the recommended approach for integrating the DSP library with React Native Audio API while maintaining the complete DSP architecture intact.

**Recommended Approach**: Single wrapper node with full DSP graph control (Approach 1)

---

## Current DSP Architecture

### Three-Layer Architecture

**Layer 1**: AudioWorkletProcessor (Web) / JSI Bridge (React Native)
**Layer 2**: Message-passing control API
**Layer 3**: C++ DSP Engine with NodeProcessor + 60+ audio nodes

### Key Features

- Graph-based architecture with NodeProcessor coordinating all nodes
- Nodes connect via Input/Output/Parameter objects
- Complex internal routing and summing algebra
- Comprehensive API for node creation, connection, and parameter control
- Support for 60+ DSP node types across multiple categories:
  - Analyzers: Recorder
  - Channels: ChannelMerger, ChannelSplitter, MidSide, Spread, StereoPanner
  - Delays: Convolver, VariableDelay
  - Dynamics: Clipper, CompressorGate, DryWet, Envelope, Lag, Shaper
  - Filters: Biquad (8 modes), Crossover, OnePole
  - Generators: FunctionOscillator, MoorerOscillator, Noise, Phasor, SamplePlayer, TableOscillator
  - Math: AbsoluteValue, Division, Multiplication, FFT operations, Trigonometric, etc.
  - Triggers: ClockTrigger, ResetTrigger, SampleAndHold, Sequencer, etc.
  - MIDI: MidiInput, MidiOutput

---

## React Native Audio API Pattern

### Canonical Custom Node Structure

- Single custom node extending `AudioNode`
- Implements `processNode()` method to process audio samples
- C++ code with JSI bridge via Host Objects
- TypeScript interface for type-safe access

### Integration Requirements

- React Native's "pure C++ turbo-module" approach
- Codegen configuration for native bindings
- Platform-specific compilation (iOS and Android)
- Registration in global scope for JavaScript access

---

## Approach Comparison

### Approach 1: Single Wrapper Node (RECOMMENDED)

#### Architecture

```
React Native Audio API Graph:
  AudioSource → DSPProcessorNode → Destination

Inside DSPProcessorNode.processNode():
  NodeProcessor coordinates 60+ internal DSP nodes
  with custom routing/summing
```

#### Implementation Overview

- Create ONE custom `DSPProcessorNode` extending `AudioNode`
- The `processNode()` method calls `nodeProcessor.process()`
- Expose control API via JSI Host Object:
  - `createNode(type, ...args)`
  - `connectNodes(sourceId, destId, ...)`
  - `setParameter(nodeId, param, value)`
  - `deleteNode(nodeId)`

#### Advantages

- ✅ Keeps entire DSP library intact
- ✅ Full control over summing algebra and computation
- ✅ Only need to create ONE custom AudioNode
- ✅ Maintains existing NodeProcessor architecture
- ✅ Can reuse same API surface as current WASM implementation
- ✅ Complex internal routing without RN Audio API limitations
- ✅ Better performance (fewer JS/Native boundary crossings)

#### Disadvantages

- ⚠️ More complex JSI bridge setup
- ⚠️ Control API requires more message-passing infrastructure
- ⚠️ Debugging is harder (internal graph not visible to RN Audio API inspector)

---

### Approach 2: Individual Wrapper Nodes

#### Architecture

```
React Native Audio API Graph:
  AudioSource → PhasorNode → OscillatorNode → FilterNode → Destination

Each node's processNode() processes independently
```

#### Implementation Overview

- Create 60+ custom AudioNode classes, one per DSP node type
- Each `processNode()` wraps a single DSP node
- Connections via RN Audio API's `.connect()` method
- Parameters exposed as JSI properties

#### Advantages

- ✅ Matches canonical RN Audio API pattern
- ✅ Each node visible in audio graph inspector
- ✅ Simpler per-node implementation
- ✅ More familiar to Web Audio API developers

#### Disadvantages

- ❌ 60+ custom nodes to implement, maintain, and test
- ❌ Lose control over summing algebra (relies on RN Audio API mixer)
- ❌ Cannot leverage NodeProcessor's sophisticated graph coordination
- ❌ More JS/Native boundary crossings (performance cost)
- ❌ Limited flexibility for complex internal routing
- ❌ Cannot easily implement modular synthesis features (parameter modulation)

---

## Recommended Implementation: Hybrid Strategy

### Phase 1: Core Wrapper Node (MVP)

#### C++ Processing Core

**DSPProcessorNode.h**

```cpp
#pragma once
#include <ReactCommon/CallInvoker.h>
#include <jsi/jsi.h>
#include "AudioNode.h"
#include "NodeProcessor.h"
#include "Data.h"
#include "MidiBuffer.h"
#include <unordered_map>
#include <memory>

namespace dsp {

class DSPProcessorNode : public AudioNode {
private:
  // Core DSP engine
  std::unique_ptr<NodeProcessor> nodeProcessor_;
  std::unique_ptr<Data> audioBuffer_;
  std::unique_ptr<MidiBuffer> midiBuffer_;

  // Node management
  std::unordered_map<int, std::unique_ptr<Node>> nodes_;
  int nextNodeId_ = 0;

  // Configuration
  int sampleRate_;
  int numChannels_;
  int bufferSize_;

public:
  DSPProcessorNode(int sampleRate, int numChannels, int bufferSize);
  ~DSPProcessorNode() override;

  // React Native Audio API integration
  void processNode(AudioBus* processingBus, size_t framesToProcess) override;

  // Control API (exposed via JSI Host Object)
  int createNode(const std::string& type, const std::vector<jsi::Value>& args);
  void connectNodes(int sourceId, int destId,
                   const std::string& sourceParam = "output",
                   const std::string& destParam = "input");
  void setParameterValue(int nodeId, const std::string& param, double value);
  void setParameterAllChannels(int nodeId, const std::string& param, double value);
  void deleteNode(int nodeId);

  // Introspection
  std::vector<std::string> getAvailableNodeTypes() const;
  jsi::Object getNodeInfo(jsi::Runtime& rt, int nodeId) const;
  jsi::Object getParameterValue(jsi::Runtime& rt, int nodeId, const std::string& param) const;
};

} // namespace dsp
```

**DSPProcessorNode.cpp**

```cpp
#include "DSPProcessorNode.h"
#include "AudioBus.h"
#include "AudioArray.h"
#include <stdexcept>

namespace dsp {

DSPProcessorNode::DSPProcessorNode(int sampleRate, int numChannels, int bufferSize)
  : sampleRate_(sampleRate)
  , numChannels_(numChannels)
  , bufferSize_(bufferSize)
{
  // Initialize NodeProcessor
  nodeProcessor_ = std::make_unique<NodeProcessor>(
    0,  // numInputs
    numChannels,
    bufferSize,
    sampleRate
  );

  // Initialize buffers
  audioBuffer_ = std::make_unique<Data>(numChannels, bufferSize);
  midiBuffer_ = std::make_unique<MidiBuffer>();
}

void DSPProcessorNode::processNode(AudioBus* processingBus, size_t framesToProcess) {
  if (!nodeProcessor_ || !audioBuffer_ || !midiBuffer_) {
    return;
  }

  // Process DSP graph
  nodeProcessor_->process(*audioBuffer_, *midiBuffer_);

  // Copy output to React Native Audio API bus
  for (int channel = 0; channel < numChannels_ && channel < processingBus->getNumberOfChannels(); channel++) {
    auto* channelData = processingBus->getChannel(channel)->getData();
    auto* dspChannelData = audioBuffer_->getReadChannelData(channel);

    for (size_t i = 0; i < framesToProcess && i < bufferSize_; i++) {
      channelData[i] = dspChannelData[i];
    }
  }
}

int DSPProcessorNode::createNode(const std::string& type, const std::vector<jsi::Value>& args) {
  int nodeId = nextNodeId_++;

  std::unique_ptr<Node> node;

  // Factory pattern for all 60+ node types
  if (type == "Phasor") {
    node = std::make_unique<Phasor>();
  } else if (type == "TableOscillator") {
    // Parse args for Type enum
    Type oscillatorType = args.size() > 0
      ? static_cast<Type>(args[0].asNumber())
      : Type::RATIO;
    node = std::make_unique<TableOscillator>(oscillatorType);
  } else if (type == "Biquad") {
    node = std::make_unique<Biquad>();
  } else if (type == "Multiplication") {
    Type mulType = args.size() > 0 ? static_cast<Type>(args[0].asNumber()) : Type::RATIO;
    Space space = args.size() > 1 ? static_cast<Space>(args[1].asNumber()) : Space::TIME;
    node = std::make_unique<Multiplication>(mulType, space);
  }
  // ... handle all 60+ node types
  else {
    throw std::runtime_error("Unknown node type: " + type);
  }

  // Set channel count
  node->setNumChannels(numChannels_);

  // Add to graph
  nodeProcessor_->getDefaultNode()->addChild(node.get());

  // Store in map
  nodes_[nodeId] = std::move(node);

  return nodeId;
}

void DSPProcessorNode::connectNodes(int sourceId, int destId,
                                   const std::string& sourceParam,
                                   const std::string& destParam) {
  auto sourceIt = nodes_.find(sourceId);
  auto destIt = nodes_.find(destId);

  if (sourceIt == nodes_.end() || destIt == nodes_.end()) {
    throw std::runtime_error("Node not found");
  }

  Node* sourceNode = sourceIt->second.get();
  Node* destNode = destIt->second.get();

  // Get source output and dest input based on parameter names
  // This requires reflection or a parameter mapping system
  Output* output = getOutputByName(sourceNode, sourceParam);
  Input* input = getInputByName(destNode, destParam);

  if (output && input) {
    output->connect(*input);
  } else {
    throw std::runtime_error("Invalid connection parameters");
  }
}

void DSPProcessorNode::setParameterAllChannels(int nodeId, const std::string& param, double value) {
  auto it = nodes_.find(nodeId);
  if (it == nodes_.end()) {
    throw std::runtime_error("Node not found");
  }

  // Get parameter by name and set all channels
  // This requires reflection or a parameter mapping system
  Parameter* parameter = getParameterByName(it->second.get(), param);
  if (parameter) {
    parameter->setAllChannelValues(value);
  } else {
    throw std::runtime_error("Invalid parameter: " + param);
  }
}

void DSPProcessorNode::deleteNode(int nodeId) {
  auto it = nodes_.find(nodeId);
  if (it != nodes_.end()) {
    // Remove from graph
    nodeProcessor_->getDefaultNode()->removeChild(it->second.get());
    // Delete from map (unique_ptr handles cleanup)
    nodes_.erase(it);
  }
}

std::vector<std::string> DSPProcessorNode::getAvailableNodeTypes() const {
  return {
    "Phasor", "TableOscillator", "FunctionOscillator", "MoorerOscillator",
    "Noise", "SamplePlayer", "Biquad", "OnePole", "Crossover",
    "Multiplication", "Division", "AbsoluteValue", "Addition", "Subtraction",
    "ChannelMerger", "ChannelSplitter", "MidSide", "Spread", "StereoPanner",
    "Convolver", "VariableDelay", "Clipper", "CompressorGate", "DryWet",
    "Envelope", "Lag", "Shaper", "Recorder", "ClockTrigger", "ResetTrigger",
    "SampleAndHold", "Sequencer", "MidiInput", "MidiOutput"
    // ... all 60+ types
  };
}

} // namespace dsp
```

#### JSI Host Object Bridge

**DSPProcessorNodeHostObject.h**

```cpp
#pragma once
#include <jsi/jsi.h>
#include "DSPProcessorNode.h"
#include <memory>

namespace dsp {

class DSPProcessorNodeHostObject : public jsi::HostObject {
private:
  std::shared_ptr<DSPProcessorNode> node_;

public:
  explicit DSPProcessorNodeHostObject(std::shared_ptr<DSPProcessorNode> node)
    : node_(std::move(node)) {}

  jsi::Value get(jsi::Runtime& runtime, const jsi::PropNameID& name) override {
    auto propName = name.utf8(runtime);

    if (propName == "createNode") {
      return jsi::Function::createFromHostFunction(
        runtime,
        name,
        2,  // min params: type, args array
        [this](jsi::Runtime& rt, const jsi::Value&, const jsi::Value* args, size_t count) -> jsi::Value {
          std::string type = args[0].asString(rt).utf8(rt);

          std::vector<jsi::Value> nodeArgs;
          if (count > 1 && args[1].isObject()) {
            auto arr = args[1].asObject(rt).asArray(rt);
            size_t len = arr.length(rt);
            for (size_t i = 0; i < len; i++) {
              nodeArgs.push_back(arr.getValueAtIndex(rt, i));
            }
          }

          int nodeId = node_->createNode(type, nodeArgs);
          return jsi::Value(nodeId);
        }
      );
    }

    if (propName == "connectNodes") {
      return jsi::Function::createFromHostFunction(
        runtime,
        name,
        2,  // min params: sourceId, destId
        [this](jsi::Runtime& rt, const jsi::Value&, const jsi::Value* args, size_t count) -> jsi::Value {
          int sourceId = args[0].asNumber();
          int destId = args[1].asNumber();
          std::string sourceParam = count > 2 ? args[2].asString(rt).utf8(rt) : "output";
          std::string destParam = count > 3 ? args[3].asString(rt).utf8(rt) : "input";

          node_->connectNodes(sourceId, destId, sourceParam, destParam);
          return jsi::Value::undefined();
        }
      );
    }

    if (propName == "setParameter") {
      return jsi::Function::createFromHostFunction(
        runtime,
        name,
        3,  // nodeId, param, value
        [this](jsi::Runtime& rt, const jsi::Value&, const jsi::Value* args, size_t) -> jsi::Value {
          int nodeId = args[0].asNumber();
          std::string param = args[1].asString(rt).utf8(rt);
          double value = args[2].asNumber();

          node_->setParameterAllChannels(nodeId, param, value);
          return jsi::Value::undefined();
        }
      );
    }

    if (propName == "deleteNode") {
      return jsi::Function::createFromHostFunction(
        runtime,
        name,
        1,
        [this](jsi::Runtime&, const jsi::Value&, const jsi::Value* args, size_t) -> jsi::Value {
          int nodeId = args[0].asNumber();
          node_->deleteNode(nodeId);
          return jsi::Value::undefined();
        }
      );
    }

    if (propName == "getAvailableNodeTypes") {
      return jsi::Function::createFromHostFunction(
        runtime,
        name,
        0,
        [this](jsi::Runtime& rt, const jsi::Value&, const jsi::Value*, size_t) -> jsi::Value {
          auto types = node_->getAvailableNodeTypes();
          auto arr = jsi::Array(rt, types.size());
          for (size_t i = 0; i < types.size(); i++) {
            arr.setValueAtIndex(rt, i, jsi::String::createFromUtf8(rt, types[i]));
          }
          return arr;
        }
      );
    }

    return jsi::Value::undefined();
  }
};

} // namespace dsp
```

#### TypeScript/JavaScript Interface

**DSPProcessorNode.ts**

```typescript
import { AudioNode, AudioContext } from "react-react-native-dsp-api";

export type DSPNodeType =
  | "Phasor"
  | "TableOscillator"
  | "FunctionOscillator"
  | "MoorerOscillator"
  | "Noise"
  | "SamplePlayer"
  | "Biquad"
  | "OnePole"
  | "Crossover"
  | "Multiplication"
  | "Division"
  | "AbsoluteValue"
  | "Addition"
  | "Subtraction"
  | "ChannelMerger"
  | "ChannelSplitter"
  | "MidSide"
  | "Spread"
  | "StereoPanner"
  | "Convolver"
  | "VariableDelay"
  | "Clipper"
  | "CompressorGate"
  | "DryWet"
  | "Envelope"
  | "Lag"
  | "Shaper"
  | "Recorder"
  | "ClockTrigger"
  | "ResetTrigger"
  | "SampleAndHold"
  | "Sequencer"
  | "MidiInput"
  | "MidiOutput";
// ... all 60+ types

export type DSPNodeId = number;

export interface DSPProcessorNodeControl {
  createNode(type: DSPNodeType, ...args: number[]): DSPNodeId;
  connectNodes(
    sourceId: DSPNodeId,
    destId: DSPNodeId,
    sourceParam?: string,
    destParam?: string
  ): void;
  setParameter(nodeId: DSPNodeId, param: string, value: number): void;
  deleteNode(nodeId: DSPNodeId): void;
  getAvailableNodeTypes(): string[];
}

export class DSPProcessorNode extends AudioNode {
  private control: DSPProcessorNodeControl;

  constructor(
    context: AudioContext,
    options?: {
      sampleRate?: number;
      numChannels?: number;
      bufferSize?: number;
    }
  ) {
    // Native module creates the node and returns control interface
    const nativeNode = global.NativeDSPModule.createDSPProcessorNode(
      context,
      options?.sampleRate ?? context.sampleRate,
      options?.numChannels ?? 2,
      options?.bufferSize ?? 128
    );

    super(context, nativeNode);
    this.control = nativeNode.control;
  }

  createNode(type: DSPNodeType, ...args: number[]): DSPNodeId {
    return this.control.createNode(type, ...args);
  }

  connectNodes(
    sourceId: DSPNodeId,
    destId: DSPNodeId,
    sourceParam = "output",
    destParam = "input"
  ): void {
    this.control.connectNodes(sourceId, destId, sourceParam, destParam);
  }

  setParameter(nodeId: DSPNodeId, param: string, value: number): void {
    this.control.setParameter(nodeId, param, value);
  }

  deleteNode(nodeId: DSPNodeId): void {
    this.control.deleteNode(nodeId);
  }

  getAvailableNodeTypes(): string[] {
    return this.control.getAvailableNodeTypes();
  }
}
```

#### Usage Example

```typescript
import { AudioContext } from "react-react-native-dsp-api";
import { DSPProcessorNode } from "./DSPProcessorNode";
import { Type, Space, BIQUAD_MODE_LOWPASS } from "./enums";

// Initialize
const context = new AudioContext();
const dspNode = new DSPProcessorNode(context, {
  sampleRate: 48000,
  numChannels: 2,
  bufferSize: 128,
});

// Create nodes (same as Web version)
const phasorId = dspNode.createNode("Phasor");
const oscId = dspNode.createNode("TableOscillator", Type.RATIO);
const filterId = dspNode.createNode("Biquad");
const gainId = dspNode.createNode("Multiplication", Type.RATIO, Space.TIME);

// Set parameters
dspNode.setParameter(phasorId, "frequency", 55);
dspNode.setParameter(filterId, "frequency", 880);
dspNode.setParameter(filterId, "mode", BIQUAD_MODE_LOWPASS);
dspNode.setParameter(gainId, "factor", 0.5);

// Create wavetable (requires additional API for buffer management)
const sawtoothId = dspNode.createWavetable("sawtooth", 2048);

// Connect graph
dspNode.connectNodes(phasorId, oscId, "output", "phase");
dspNode.connectNodes(oscId, filterId, "output", "input");
dspNode.connectNodes(filterId, gainId, "output", "input");
dspNode.connectToOutput(gainId); // Special method to connect to node processor output

// Connect to audio context
dspNode.connect(context.destination);
```

### Phase 2: Convenience Wrapper Nodes (Optional)

Create facade nodes for common patterns that wrap the core processor:

```cpp
// Convenience nodes that delegate to core processor
class DSPOscillatorNode : public AudioNode {
private:
  std::shared_ptr<DSPProcessorNode> processor_;
  int oscNodeId_;

public:
  void processNode(AudioBus* bus, size_t frames) override {
    // Delegate to processor
    processor_->processNode(bus, frames);
  }

  void setFrequency(double freq) {
    processor_->setParameter(oscNodeId_, "frequency", freq);
  }
};
```

This gives users:

- Familiar RN Audio API interface for simple cases
- Full power of NodeProcessor for complex cases
- Choice of preferred abstraction level

---

## Why This Approach Wins

### 1. Architectural Consistency

- Same NodeProcessor coordination as Web version
- Same node connection model
- Can share most C++ code between Web (WASM) and Native (JSI)

### 2. Performance

- Single `processNode()` call per audio callback
- All routing happens in C++ (zero JS bridge overhead for audio processing)
- Control API (create/connect/setParameter) happens off the audio thread

### 3. Flexibility

- Full control over summing algebra (maintained in NodeProcessor)
- Can implement parameter modulation (audio-rate control)
- Can add custom nodes without touching React Native Audio API

### 4. Maintainability

- One custom node type to maintain
- Existing DSP library stays intact
- Web and Native versions share control API design

### 5. Developer Experience

```typescript
// Simple for basic use
const osc = dspNode.createNode("TableOscillator");

// Powerful for complex graphs
const synth = new SynthVoice(dspNode); // Encapsulates complex graph
synth.noteOn(60, 100);
```

---

## Migration Path

### Phase 1: Core Processor Node

- Implement `DSPProcessorNode` class extending `AudioNode`
- Create JSI Host Object bridge for control API
- Implement factory pattern for all 60+ node types
- Add TypeScript interface and type definitions

### Phase 2: Convenience Methods

- Add helper methods for common operations
- Create preset graph templates
- Implement higher-level abstractions

### Phase 3: Optional Facade Nodes

- Create individual wrapper nodes for Web Audio API familiarity
- Maintain backward compatibility with existing patterns
- Provide migration guides

### Phase 4: Visual Graph Editor

- Build visual editor that works with control API
- Enable drag-and-drop node creation and connection
- Add real-time parameter visualization

---

## Implementation Checklist

### Core Requirements

- [ ] Set up React Native pure C++ turbo-module
- [ ] Configure Codegen for native bindings
- [ ] Implement `DSPProcessorNode` C++ class
- [ ] Create JSI Host Object bridge
- [ ] Implement node factory for all types
- [ ] Add parameter mapping/reflection system
- [ ] Create TypeScript interface
- [ ] Write usage documentation

### Testing

- [ ] Unit tests for node creation
- [ ] Integration tests for node connections
- [ ] Performance benchmarks
- [ ] Memory leak tests
- [ ] Cross-platform validation (iOS/Android)

### Documentation

- [ ] API reference documentation
- [ ] Migration guide from Web version
- [ ] Example projects
- [ ] Performance optimization guide

---

## Conclusion

The recommended hybrid strategy provides the best balance of:

- **Preservation**: Entire DSP library remains intact
- **Performance**: Optimized for mobile with minimal overhead
- **Flexibility**: Full control over audio processing and routing
- **Maintainability**: Single integration point with clear separation of concerns
- **Scalability**: Can add features without redesigning architecture

The single wrapper node approach maintains the sophisticated architecture of the DSP library while providing a clean integration with React Native Audio API, enabling the same powerful graph-based audio processing on mobile as on the web.
