/// <reference types="@types/bun" />
import { describe, expect, test } from 'bun:test'
import { WeakValueMap } from './WeakValueMap'

describe('WeakValueMap', () => {
  describe('constructor', () => {
    test('creates empty map with no arguments', () => {
      const map = new WeakValueMap<string, object>()
      expect(map.size).toBe(0)
    })

    test('creates empty map with null', () => {
      const map = new WeakValueMap<string, object>(null)
      expect(map.size).toBe(0)
    })

    test('creates map from entries array', () => {
      const obj1 = { id: 1 }
      const obj2 = { id: 2 }
      const entries: [string, object][] = [
        ['a', obj1],
        ['b', obj2],
      ]
      const map = new WeakValueMap(entries)
      expect(map.size).toBe(2)
      expect(map.get('a')).toBe(obj1)
      expect(map.get('b')).toBe(obj2)
    })

    test('creates map from iterable', () => {
      const obj1 = { id: 1 }
      const obj2 = { id: 2 }
      const iterable = new Map<string, object>([
        ['x', obj1],
        ['y', obj2],
      ])
      const map = new WeakValueMap(iterable)
      expect(map.size).toBe(2)
      expect(map.get('x')).toBe(obj1)
      expect(map.get('y')).toBe(obj2)
    })
  })

  describe('set', () => {
    test('adds new entry', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { value: 'test' }
      map.set('key', obj)
      expect(map.get('key')).toBe(obj)
    })

    test('returns this for chaining', () => {
      const map = new WeakValueMap<string, object>()
      const result = map.set('key', { value: 1 })
      expect(result).toBe(map)
    })

    test('overwrites existing entry', () => {
      const map = new WeakValueMap<string, object>()
      const obj1 = { value: 1 }
      const obj2 = { value: 2 }
      map.set('key', obj1)
      map.set('key', obj2)
      expect(map.get('key')).toBe(obj2)
    })

    test('supports chaining', () => {
      const map = new WeakValueMap<string, object>()
      const obj1 = { a: 1 }
      const obj2 = { b: 2 }
      map.set('a', obj1).set('b', obj2)
      expect(map.size).toBe(2)
    })
  })

  describe('get', () => {
    test('returns value for existing key', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { data: 'hello' }
      map.set('mykey', obj)
      expect(map.get('mykey')).toBe(obj)
    })

    test('returns undefined for non-existent key', () => {
      const map = new WeakValueMap<string, object>()
      expect(map.get('nonexistent')).toBeUndefined()
    })
  })

  describe('has', () => {
    test('returns true for existing key', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { test: true }
      map.set('exists', obj)
      expect(map.has('exists')).toBe(true)
    })

    test('returns false for non-existent key', () => {
      const map = new WeakValueMap<string, object>()
      expect(map.has('missing')).toBe(false)
    })
  })

  describe('delete', () => {
    test('removes existing entry and returns true', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { remove: 'me' }
      map.set('toDelete', obj)
      expect(map.delete('toDelete')).toBe(true)
      expect(map.has('toDelete')).toBe(false)
    })

    test('returns false for non-existent key', () => {
      const map = new WeakValueMap<string, object>()
      expect(map.delete('nothere')).toBe(false)
    })

    test('decrements size after deletion', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { x: 1 }
      map.set('key', obj)
      expect(map.size).toBe(1)
      map.delete('key')
      expect(map.size).toBe(0)
    })
  })

  describe('clear', () => {
    test('removes all entries', () => {
      const map = new WeakValueMap<string, object>()
      const obj1 = { a: 1 }
      const obj2 = { b: 2 }
      const obj3 = { c: 3 }
      map.set('a', obj1)
      map.set('b', obj2)
      map.set('c', obj3)
      expect(map.size).toBe(3)
      map.clear()
      expect(map.size).toBe(0)
      expect(map.has('a')).toBe(false)
      expect(map.has('b')).toBe(false)
      expect(map.has('c')).toBe(false)
    })

    test('works on empty map', () => {
      const map = new WeakValueMap<string, object>()
      map.clear()
      expect(map.size).toBe(0)
    })
  })

  describe('size', () => {
    test('returns 0 for empty map', () => {
      const map = new WeakValueMap<string, object>()
      expect(map.size).toBe(0)
    })

    test('returns correct count', () => {
      const map = new WeakValueMap<string, object>()
      const obj1 = { n: 1 }
      const obj2 = { n: 2 }
      const obj3 = { n: 3 }
      map.set('one', obj1)
      expect(map.size).toBe(1)
      map.set('two', obj2)
      expect(map.size).toBe(2)
      map.set('three', obj3)
      expect(map.size).toBe(3)
    })
  })

  describe('forEach', () => {
    test('iterates over all entries', () => {
      const map = new WeakValueMap<string, { id: number }>()
      const obj1 = { id: 1 }
      const obj2 = { id: 2 }
      map.set('a', obj1)
      map.set('b', obj2)
      const results: [string, { id: number }][] = []
      map.forEach((value, key) => {
        results.push([key, value])
      })
      expect(results).toHaveLength(2)
      expect(results).toContainEqual(['a', obj1])
      expect(results).toContainEqual(['b', obj2])
    })

    test('uses thisArg correctly', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { val: 1 }
      map.set('key', obj)
      const context = { multiplier: 10, captured: false }
      map.forEach(function (this: typeof context) {
        this.captured = true
      }, context)
      expect(context.captured).toBe(true)
    })

    test('does nothing on empty map', () => {
      const map = new WeakValueMap<string, object>()
      let callCount = 0
      map.forEach(() => {
        callCount++
      })
      expect(callCount).toBe(0)
    })
  })

  describe('Symbol.iterator', () => {
    test('iterates over entries', () => {
      const map = new WeakValueMap<string, { v: number }>()
      const obj1 = { v: 1 }
      const obj2 = { v: 2 }
      map.set('x', obj1)
      map.set('y', obj2)
      const entries = [...map]
      expect(entries).toHaveLength(2)
      expect(entries).toContainEqual(['x', obj1])
      expect(entries).toContainEqual(['y', obj2])
    })

    test('works with for...of', () => {
      const map = new WeakValueMap<string, object>()
      const obj = { data: 'test' }
      map.set('single', obj)
      const collected: [string, object][] = []
      for (const entry of map) {
        collected.push(entry)
      }
      expect(collected).toEqual([['single', obj]])
    })
  })

  describe('entries', () => {
    test('returns iterator of key-value pairs', () => {
      const map = new WeakValueMap<string, { n: number }>()
      const obj1 = { n: 10 }
      const obj2 = { n: 20 }
      map.set('first', obj1)
      map.set('second', obj2)
      const entries = [...map.entries()]
      expect(entries).toHaveLength(2)
      expect(entries).toContainEqual(['first', obj1])
      expect(entries).toContainEqual(['second', obj2])
    })
  })

  describe('keys', () => {
    test('returns iterator of keys', () => {
      const map = new WeakValueMap<string, object>()
      const obj1 = { k: 1 }
      const obj2 = { k: 2 }
      map.set('key1', obj1)
      map.set('key2', obj2)
      const keys = [...map.keys()]
      expect(keys).toHaveLength(2)
      expect(keys).toContain('key1')
      expect(keys).toContain('key2')
    })
  })

  describe('values', () => {
    test('returns iterator of values', () => {
      const map = new WeakValueMap<string, { id: string }>()
      const obj1 = { id: 'alpha' }
      const obj2 = { id: 'beta' }
      map.set('a', obj1)
      map.set('b', obj2)
      const values = [...map.values()]
      expect(values).toHaveLength(2)
      expect(values).toContain(obj1)
      expect(values).toContain(obj2)
    })
  })

  describe('Symbol.toStringTag', () => {
    test('returns WeakValueMap', () => {
      const map = new WeakValueMap<string, object>()
      expect(map[Symbol.toStringTag]).toBe('WeakValueMap')
    })

    test('affects Object.prototype.toString', () => {
      const map = new WeakValueMap<string, object>()
      expect(Object.prototype.toString.call(map)).toBe('[object WeakValueMap]')
    })
  })

  describe('key types', () => {
    test('supports number keys', () => {
      const map = new WeakValueMap<number, object>()
      const obj = { test: true }
      map.set(42, obj)
      expect(map.get(42)).toBe(obj)
    })

    test('supports symbol keys', () => {
      const map = new WeakValueMap<symbol, object>()
      const key = Symbol('testKey')
      const obj = { sym: true }
      map.set(key, obj)
      expect(map.get(key)).toBe(obj)
    })

    test('supports object keys', () => {
      const map = new WeakValueMap<object, object>()
      const keyObj = { key: true }
      const valueObj = { value: true }
      map.set(keyObj, valueObj)
      expect(map.get(keyObj)).toBe(valueObj)
    })
  })
})
