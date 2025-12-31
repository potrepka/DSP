# Developer Guide

## Project Outline

- `examples/html`: DSP HTML demo
- `examples/react`: DSP React demo
- `lib/FFTConvolver`: FFT Convolver library
- `lib/simd_math_prims`: SIMD math primitives library
- `packages/react-native-dsp`: React Native DSP library
- `src`: C++ source code for DSP libraries

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

## Code Style

- Do not add comments, except for @ts-ignore and eslint-disable-next-line
  comments as a last ditch effort to fix TypeScript/ESLint errors.
- Do not add blank lines within functions or classes.
- Leave a blank line between top-level definitions.
- Do not terminate statements with semicolons.
- Use single quotes for strings.
- Use trailing commas.
- Strictly use types over interfaces.
- Avoid code duplication. Use helper functions and custom hooks for code reuse.
- Always use a code block for if statements. Do not write single-line if
  statements.
- Prefer arrow functions, e.g. `const doSomething = () => {}` instead of
  `function doSomething() {}`.
- Prefer named exports, e.g. `export const doSomething = () => {}` instead of
  `export default () => {}`, except when a library, such as `convex` or `expo`,
  requires a default export.
- Use and maintain `index.ts` files to export all definitions from a folder.
- Import the top-level folder when possible, e.g.,
  `import { useTheme } from '../../hooks'` instead of
  `import { useTheme } from '../../hooks/useTheme'`.
