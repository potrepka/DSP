#!/bin/bash
set -e

echo "🔨 Building react-native-dsp for Web (WASM)..."

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PACKAGE_DIR="$(dirname "$SCRIPT_DIR")"
OUTPUT_DIR="$PACKAGE_DIR/web/build"
ORIGINAL_LIB_DIR="$PACKAGE_DIR/../../lib"
ORIGINAL_SRC_DIR="$PACKAGE_DIR/../../src"
SHARED_DIR="$PACKAGE_DIR/shared"
LIB_DIR="$SHARED_DIR/lib"
SRC_DIR="$SHARED_DIR/src"
BINDINGS_FILE="$SHARED_DIR/web/bindings.cpp"
PROCESSOR_FILE="$PACKAGE_DIR/dist/classes/web/WebAudioProcessor.js"

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null; then
    echo "❌ Error: Emscripten not found. Please install Emscripten first."
    exit 1
fi

echo "✓ Emscripten found: $(emcc --version | head -n 1)"

# Check if TypeScript has been compiled
if [ ! -f "$PROCESSOR_FILE" ]; then
    echo "❌ Error: WebAudioProcessor.js not found. Run 'npm run build' first."
    exit 1
fi

echo "✓ WebAudioProcessor.js found"

# Bundle the processor into IIFE format (no ES module syntax for Emscripten)
BUNDLED_PROCESSOR="$OUTPUT_DIR/WebAudioProcessor.bundle.js"
echo "📦 Bundling WebAudioProcessor..."

esbuild "$PROCESSOR_FILE" \
  --bundle \
  --format=esm \
  --platform=browser \
  --target=esnext \
  --outfile="$BUNDLED_PROCESSOR"

echo "✓ Processor bundled"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# ---------------------------------------------------------
# 1. GATHER SOURCE FILES
# ---------------------------------------------------------

# Get all .cpp files in LIB_DIR
LIB_FILES=$(find "$LIB_DIR" -name "*.cpp")

# Get all .cpp files in SRC_DIR
SRC_FILES=$(find "$SRC_DIR" -name "*.cpp")

ALL_SOURCES="$BINDINGS_FILE $LIB_FILES $SRC_FILES"

# Count files to be sure
NUM_FILES=$(echo "$ALL_SOURCES" | wc -w | xargs)

echo "✓ $NUM_FILES C++ source files found"

# ---------------------------------------------------------
# 2. COMPILE
# ---------------------------------------------------------
echo "🔧 Compiling..."

emcc \
  -O3 \
  -std=c++20 \
  -D DSP_NO_RTAUDIO=1 \
  -D DSP_NO_RTMIDI=1 \
  -I "$LIB_DIR" \
  -I "$SRC_DIR" \
  $ALL_SOURCES \
  -l embind \
  -o "$OUTPUT_DIR/react-native-dsp.js" \
  -s WASM=1 \
  -s SINGLE_FILE=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME="createAudioModule" \
  -s ENVIRONMENT="web,worker" \
  -s BINARYEN_ASYNC_COMPILATION=0 \
  -s MALLOC="emmalloc" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MAXIMUM_MEMORY=2147483648 \
  --extern-post-js "$BUNDLED_PROCESSOR"

rm "$BUNDLED_PROCESSOR"

if [ $? -eq 0 ]; then
    echo "✓ Web build complete"
    echo "✓ Output: $OUTPUT_DIR/react-native-dsp.js"
else
    echo "❌ Build failed"
    exit 1
fi
