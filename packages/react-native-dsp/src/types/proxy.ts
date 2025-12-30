import type { Target } from './module'

export type ProxyPort = {
  postMessage: (message: unknown) => void
}

export type ProxyContext = {
  port: ProxyPort
  call: <T>(target: Target, functionName: string, args: unknown[]) => Promise<T>
}
