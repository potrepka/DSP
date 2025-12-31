export type ProxyContext = {
  sendMessage: <T>(message: {
    message: string
    requestId: string
    [key: string]: unknown
  }) => Promise<T>
}
