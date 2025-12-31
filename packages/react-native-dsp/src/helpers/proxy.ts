import { Chainable } from '../types/proxy'

export function chainable<T>(promise: Promise<T>): Chainable<T> {
  return new Proxy(promise, {
    get(target, prop, _receiver) {
      if (prop === 'then' || prop === 'catch' || prop === 'finally') {
        const method = target[prop] as (...args: unknown[]) => unknown
        return method.bind(target)
      }
      return (...args: unknown[]) => {
        const chained = target.then((resolved) => {
          const value = (resolved as Record<string | symbol, unknown>)[prop]
          return typeof value === 'function'
            ? value.apply(resolved, args)
            : value
        })
        return chainable(chained)
      }
    },
  }) as Chainable<T>
}
