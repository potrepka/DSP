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

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null; then
    echo "❌ Error: Emscripten not found. Please install Emscripten first."
    exit 1
fi

echo "✓ Emscripten found: $(emcc --version | head -n 1)"

# Copy files to shared directory
rm -rf "$LIB_DIR" "$SRC_DIR"
cp -r "$ORIGINAL_LIB_DIR" "$LIB_DIR"
cp -r "$ORIGINAL_SRC_DIR" "$SRC_DIR"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# ---------------------------------------------------------
# 1. GATHER SOURCE FILES
# ---------------------------------------------------------
# We need to compile bindings.cpp PLUS all your library code in lib/ and src/
echo "🔎 Collecting source files..."

# Get all .cpp files in LIB_DIR
LIB_FILES=$(find "$LIB_DIR" -name "*.cpp")

# Get all .cpp files in SRC_DIR
SRC_FILES=$(find "$SRC_DIR" -name "*.cpp")

ALL_SOURCES="$BINDINGS_FILE $LIB_FILES $SRC_FILES"

# Count files to be sure
NUM_FILES=$(echo "$ALL_SOURCES" | wc -w)
echo "✓ Found $NUM_FILES C++ source files to compile."

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
  -s MODULARIZE=1 \
  -s EXPORT_ES6=1 \
  -s EXPORT_NAME="createAudioModule" \
  -s ENVIRONMENT="web,worker" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MAXIMUM_MEMORY=2147483648 \
  -s MALLOC="emmalloc" \

if [ $? -eq 0 ]; then
    echo "✅ Web build complete!"
    echo "   Output: $OUTPUT_DIR/react-native-dsp.js"
    echo "   Output: $OUTPUT_DIR/react-native-dsp.wasm"
else
    echo "❌ Build failed"
    exit 1
fi
