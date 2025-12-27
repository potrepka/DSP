export type PlatformName = 'web' | 'ios' | 'android' | 'unknown'

export type PlatformInfo = {
  readonly isWeb: boolean
  readonly isNative: boolean
  readonly name: PlatformName
}
