import type { ProxyContext, SerializedValue, Target } from '../../../types'

export abstract class Proxy {
  constructor(
    protected readonly context: ProxyContext,
    public readonly target: Target,
  ) {}

  toTarget = (): Target => this.target

  protected call = <T>(methodName: string, args: unknown[]): Promise<T> => {
    return this.context.sendMessage<T>({
      message: 'callMethod',
      requestId: '',
      target: this.target,
      methodName,
      args: args.map((arg) => {
        if (arg && typeof arg === 'object' && 'toTarget' in arg) {
          return (arg as { toTarget: () => Target }).toTarget()
        }
        return arg
      }) as SerializedValue[],
    })
  }

  delete = (): Promise<void> => {
    return this.context.sendMessage<void>({
      message: 'deleteObject',
      requestId: '',
      objectId: this.target.id,
    })
  }
}
