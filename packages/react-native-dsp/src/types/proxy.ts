export type ProxyContext = {
  sendMessage: <T>(message: {
    message: string
    requestId: string
    [key: string]: unknown
  }) => Promise<T>
}

export type Chainable<T> = Promise<T> & {
  [K in keyof T]: T[K] extends (...args: infer A) => infer R
    ? (...args: A) => Chainable<Awaited<R>>
    : () => Chainable<T[K]>
}
