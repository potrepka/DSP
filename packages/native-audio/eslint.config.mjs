import js from '@eslint/js'
import tsPlugin from '@typescript-eslint/eslint-plugin'
import tsParser from '@typescript-eslint/parser'
import perfectionist from 'eslint-plugin-perfectionist'
import unusedImports from 'eslint-plugin-unused-imports'

const importRules = {
  'unused-imports/no-unused-imports': 'error',
  'unused-imports/no-unused-vars': [
    'warn',
    {
      args: 'after-used',
      argsIgnorePattern: '^_',
      vars: 'all',
      varsIgnorePattern: '^_',
    },
  ],
  'perfectionist/sort-imports': [
    'error',
    {
      newlinesBetween: 0,
      groups: ['builtin', 'external', ['parent', 'sibling', 'index']],
    },
  ],
  'perfectionist/sort-named-imports': 'error',
  'perfectionist/sort-exports': 'error',
  'perfectionist/sort-named-exports': 'error',
}

export default [
  js.configs.recommended,
  {
    ignores: ['dist/**/*'],
  },
  {
    files: ['**/*.js', '**/*.jsx', '**/*.mjs'],
    plugins: {
      perfectionist,
      'unused-imports': unusedImports,
    },
    rules: {
      ...importRules,
    },
  },
  {
    files: ['**/*.ts', '**/*.tsx'],
    languageOptions: {
      parser: tsParser,
      parserOptions: {
        project: './tsconfig.eslint.json',
        sourceType: 'module',
        ecmaVersion: 'latest',
      },
    },
    plugins: {
      '@typescript-eslint': tsPlugin,
      perfectionist,
      'unused-imports': unusedImports,
    },
    rules: {
      ...tsPlugin.configs.recommended.rules,
      '@typescript-eslint/ban-ts-comment': 'off',
      ...importRules,
    },
  },
]
