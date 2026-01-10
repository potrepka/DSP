# Developer Guide

## Project Outline

- `examples/react`: DSP React demo
- `lib/FFTConvolver`: FFT convolver library
- `lib/simd_math_prims`: SIMD math primitives library
- `packages/react-native-dsp`: React Native DSP library
- `src`: C++ source code for DSP library

## Scripts

Always use Bun package manager (`bun`/`bunx`).

- `bun run build`: Build repo
- `bun run clean`: Clean repo
- `bun run lint`: Lint code
- `bun run lint:fix`: Lint code and fix errors
- `bun run lint:format`: Format code
- `bun run lint:format:fix`: Format code and fix errors
- `bun run lint:packages`: Lint package versions
- `bun run lint:packages:fix`: Lint package versions and fix errors
- `bun run lint:packages:format`: Format package list
- `bun run lint:packages:update`: Update packages
- `bun test`: Run tests
