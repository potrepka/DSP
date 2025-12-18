#!/bin/bash
set -e

echo "🔨 Building native-audio for Web (WASM)..."

# Configuration
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PACKAGE_DIR="$(dirname "$SCRIPT_DIR")"
OUTPUT_DIR="$PACKAGE_DIR/web/build"
CPP_SRC="$PACKAGE_DIR/../../src"
CPP_LIB="$PACKAGE_DIR/../../lib"
CPP_BINDINGS="$PACKAGE_DIR/cpp/web"

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null; then
    echo "❌ Error: Emscripten not found. Please install Emscripten first."
    exit 1
fi

echo "✓ Emscripten found: $(emcc --version | head -n 1)"

# Create output directory
mkdir -p "$OUTPUT_DIR"

# ---------------------------------------------------------
# 1. GATHER SOURCE FILES
# ---------------------------------------------------------
# We need to compile bindings.cpp PLUS all your library code in lib/ and src/
echo "🔎 Collecting source files..."

# Get all .cpp files in CPP_LIB
LIB_FILES=$(find "$CPP_LIB" -name "*.cpp")

# Get all .cpp files in CPP_SRC
SRC_FILES=$(find "$CPP_SRC" -name "*.cpp")

ALL_SOURCES="$CPP_BINDINGS/bindings.cpp $LIB_FILES $SRC_FILES"

# Count files to be sure
NUM_FILES=$(echo "$ALL_SOURCES" | wc -w)
echo "✓ Found $NUM_FILES C++ source files to compile."

# ---------------------------------------------------------
# 2. COMPILE
# ---------------------------------------------------------
echo "🔧 Compiling..."

emcc \
  -O3 \
  -std=c++17 \
  -I"$CPP_SRC" \
  -I"$CPP_LIB" \
  -lembind \
  $ALL_SOURCES \
  -o "$OUTPUT_DIR/native-audio.js" \
  -DDSP_NO_RTMIDI=1 \
  -DDSP_NO_RTAUDIO=1 \
  -s WASM=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME="createAudioModule" \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s MAXIMUM_MEMORY=2GB \
  -s EXPORTED_FUNCTIONS='["_malloc","_free"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -s ENVIRONMENT='web,worker' \
  -s NO_EXIT_RUNTIME=1

if [ $? -eq 0 ]; then
    echo "✅ Web build complete!"
    echo "   Output: $OUTPUT_DIR/native-audio.js"
    echo "   Output: $OUTPUT_DIR/native-audio.wasm"
else
    echo "❌ Build failed"
    exit 1
fi
