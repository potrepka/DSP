# Development Guide

This guide covers building and developing the @potrepka/native-audio package.

## Prerequisites

### Required

- **Node.js**: 18+
- **TypeScript**: 5.0+
- **Git**: For version control

### Platform-Specific

#### Web Build (WASM)
- **Emscripten**: Latest stable version
  ```bash
  # Install via emsdk
  git clone https://github.com/emscripten-core/emsdk.git
  cd emsdk
  ./emsdk install latest
  ./emsdk activate latest
  source ./emsdk_env.sh
  ```

#### iOS Build
- **Xcode**: 14+ (macOS only)
- **CocoaPods**: `gem install cocoapods`

#### Android Build
- **Android Studio**: Latest version
- **Android NDK**: r23+ (installed via Android Studio)
- **CMake**: 3.18+ (installed via Android Studio)

## Project Structure

```
packages/native-audio/
├── cpp/                    # C++ bindings layer
│   ├── core/              # Core wrappers (NodeWrapper.h)
│   ├── web/               # Web/WASM bindings (Emscripten)
│   └── native/            # React Native bindings (JSI) [TODO]
├── src/                   # TypeScript source
│   ├── core/              # Base classes (Node, Graph, AudioBuffer)
│   ├── nodes/             # DSP node wrappers
│   ├── types.ts           # Type definitions
│   ├── platform.ts        # Platform detection
│   └── index.ts           # Main exports
├── examples/              # Usage examples
│   ├── basic-usage.ts     # TypeScript example
│   └── test.html          # Browser test page
├── scripts/               # Build scripts
│   └── build-web.sh       # Web/WASM build script
├── web/                   # Web build outputs
├── dist/                  # TypeScript build outputs
└── [config files]
```

## Building

### 1. Install Dependencies

```bash
cd packages/native-audio
npm install
```

### 2. Build Web (WASM)

```bash
# Make sure Emscripten is activated
source ~/emsdk/emsdk_env.sh

# Build WASM
npm run build:web

# Output will be in web/build/:
#   - native-audio.js
#   - native-audio.wasm
```

**Note**: The first build may take a few minutes as Emscripten compiles the entire DSP library.

### 3. Build TypeScript

```bash
npm run build:ts

# Output will be in dist/:
#   - index.js
#   - index.d.ts
#   - [all source files compiled]
```

### 4. Build Everything

```bash
npm run build
```

## Testing

### Web Platform

1. Build the WASM module:
   ```bash
   npm run build:web
   ```

2. Start a local HTTP server:
   ```bash
   # Using Python
   cd examples
   python3 -m http.server 8000

   # Or using Node.js
   npx http-server
   ```

3. Open http://localhost:8000/test.html in your browser

4. Click "Run Test" to run the basic audio processing test

5. Click "Play Audio" to hear a 2-second synthesized tone

### React Native Platform

**TODO**: React Native JSI bindings not yet implemented.

## Current Status

### ✅ Implemented (Proof of Concept)

- [x] Package structure and configuration
- [x] TypeScript core infrastructure
  - [x] Base Node class with dispose() pattern
  - [x] Platform detection (Web/Native)
  - [x] Audio buffer utilities
  - [x] Node graph management
- [x] C++ wrapper infrastructure
  - [x] NodeWrapper base class
  - [x] TransformerWrapper template
  - [x] ProducerWrapper template
- [x] Web platform (WASM) bindings
  - [x] Emscripten/Embind definitions
  - [x] AudioProcessor helper
  - [x] Build script
- [x] Proof-of-concept nodes (3 nodes)
  - [x] Biquad filter (8 filter types)
  - [x] Gain (Multiplication)
  - [x] TableOscillator
- [x] Examples and tests
  - [x] Basic usage example
  - [x] Interactive HTML test page

### 🚧 In Progress

- [ ] Verify WASM build (requires Emscripten)
- [ ] Test memory management
- [ ] Verify audio processing correctness

### 📋 TODO

#### Phase 1: Complete Web Platform
- [ ] Fix any WASM build issues
- [ ] Optimize buffer transfer (consider SharedArrayBuffer)
- [ ] Add more comprehensive tests
- [ ] Performance benchmarking

#### Phase 2: React Native Platform
- [ ] Implement JSI bindings (cpp/native/jsi-bindings.cpp)
- [ ] Create iOS CocoaPods spec
- [ ] Create Android CMake configuration
- [ ] Test on iOS devices
- [ ] Test on Android devices

#### Phase 3: Complete Node Coverage
- [ ] Add remaining 57+ nodes
  - [ ] Analyzers (1 node): Recorder
  - [ ] Channels (5 nodes): ChannelMerger, ChannelSplitter, MidSide, Spread, StereoPanner
  - [ ] Delays (2 nodes): Convolver, VariableDelay
  - [ ] Dynamics (6 nodes): Clipper, CompressorGate, DryWet, Envelope, Lag, Shaper
  - [ ] Filters (2 nodes): Crossover, OnePole
  - [ ] Generators (5 nodes): FunctionOscillator, MoorerOscillator, Noise, Phasor, SamplePlayer
  - [ ] Math (19 nodes): AbsoluteValue, Division, FFT, etc.
  - [ ] Triggers (8 nodes): ClockTrigger, Sequencer, etc.
  - [ ] Variables (4 nodes): SampleRate, BufferRate, etc.
- [ ] Add unit tests for each node
- [ ] Document each node's parameters

#### Phase 4: Documentation & Polish
- [ ] Complete API documentation
- [ ] Add more usage examples
- [ ] Create migration guide from C++
- [ ] Add TypeScript JSDoc comments
- [ ] Create video tutorials

#### Phase 5: Optimization
- [ ] Bundle size optimization
- [ ] Zero-copy buffer transfer where possible
- [ ] SIMD optimization hints
- [ ] Code splitting for on-demand node loading

## Known Issues

### WASM Build

The WASM build may fail with errors related to:

1. **Missing Emscripten**: Make sure Emscripten is installed and activated
2. **Include paths**: The build script assumes the DSP source is at `../../src`
3. **Buffer transfer**: The current AudioProcessor implementation may need refinement for typed array access

### Memory Management

- **Web**: Requires manual `dispose()` calls to prevent memory leaks
- **Native**: Not yet implemented, but will use automatic GC

## Debugging

### Enable WASM Debugging

Add `-g` flag to emcc command in `scripts/build-web.sh`:

```bash
emcc -g -O0 ...
```

This enables source maps and debugging symbols (but increases file size significantly).

### TypeScript Debugging

Use VS Code with the following launch configuration:

```json
{
  "type": "node",
  "request": "launch",
  "name": "Debug TypeScript",
  "program": "${workspaceFolder}/examples/basic-usage.ts",
  "preLaunchTask": "tsc: build - tsconfig.json",
  "outFiles": ["${workspaceFolder}/dist/**/*.js"]
}
```

## Contributing

### Code Style

- **TypeScript**: Follow the existing code style (use Prettier)
- **C++**: Follow the existing DSP library style (use clang-format)
- **Naming**: Use camelCase for TypeScript, snake_case for C++

### Adding New Nodes

1. Create C++ wrapper in `cpp/web/bindings.cpp` (or `cpp/native/jsi-bindings.cpp`)
2. Add Embind/JSI binding definition
3. Create TypeScript wrapper in `src/nodes/[category]/[NodeName].ts`
4. Export from `src/index.ts`
5. Add example usage
6. Add tests

### Pull Requests

- Include tests for new features
- Update documentation
- Follow the existing code style
- Test on both Web and Native (once Native is implemented)

## Performance Tips

### For Users

1. **Use larger buffer sizes**: Reduces overhead (512-2048 samples)
2. **Reuse buffers**: Allocate once, reuse many times
3. **Batch processing**: Process multiple buffers in sequence
4. **Profile first**: Use browser DevTools or React Native profiler

### For Developers

1. **Minimize buffer copies**: Use SharedArrayBuffer on Web when possible
2. **Optimize hot paths**: Focus on the `process()` function
3. **Consider SIMD**: The DSP library supports SIMD on some platforms
4. **Lazy initialization**: Only create nodes when needed

## Resources

- [Emscripten Documentation](https://emscripten.org/docs/)
- [Embind Guide](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html)
- [React Native JSI](https://reactnative.dev/docs/the-new-architecture/pillars-turbomodules)
- [Web Audio API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Audio_API)

## License

MIT © Nathaniel Potrepka
