export class WeakValueMap<K, V extends object> implements Map<K, V> {
  #map = new Map<K, WeakRef<V>>()
  #registry = new FinalizationRegistry<K>(this.#map.delete.bind(this.#map))

  constructor(entries?: readonly (readonly [K, V])[] | null)
  constructor(iterable: Iterable<readonly [K, V]>)
  constructor(iterable?: Iterable<readonly [K, V]> | null) {
    for (const [key, value] of iterable ?? []) {
      this.set(key, value)
    }
  }

  get size(): number {
    let count = 0
    for (const ref of this.#map.values()) {
      if (ref.deref() !== undefined) {
        count++
      }
    }
    return count
  }

  clear(): void {
    for (const key of this.#map.keys()) {
      this.delete(key)
    }
  }

  delete(key: K): boolean {
    const ref = this.#map.get(key)
    if (ref) {
      this.#map.delete(key)
      this.#registry.unregister(ref)
      return true
    }
    return false
  }

  get(key: K): V | undefined {
    return this.#map.get(key)?.deref()
  }

  has(key: K): boolean {
    return this.get(key) !== undefined
  }

  set(key: K, value: V): this {
    const ref = new WeakRef(value)
    this.#map.set(key, ref)
    this.#registry.register(value, key, ref)
    return this
  }

  get [Symbol.toStringTag](): string {
    return 'WeakValueMap'
  }

  forEach(
    callbackfn: (value: V, key: K, map: Map<K, V>) => void,
    thisArg?: unknown,
  ): void {
    for (const [key, value] of this) {
      callbackfn.call(thisArg, value, key, this)
    }
  }

  *[Symbol.iterator](): MapIterator<[K, V]> {
    for (const [key, ref] of this.#map) {
      const value = ref.deref()
      if (value !== undefined) {
        yield [key, value]
      }
    }
  }

  entries(): MapIterator<[K, V]> {
    return this[Symbol.iterator]()
  }

  *keys(): MapIterator<K> {
    for (const [key, ref] of this.#map) {
      if (ref.deref() !== undefined) {
        yield key
      }
    }
  }

  *values(): MapIterator<V> {
    for (const ref of this.#map.values()) {
      const value = ref.deref()
      if (value !== undefined) {
        yield value
      }
    }
  }
}
