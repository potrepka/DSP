import { PlatformInfo } from '../types'

export const getPlatformInfo = (): PlatformInfo => {
  if (typeof window !== 'undefined' && typeof document !== 'undefined') {
    return {
      isWeb: true,
      isNative: false,
      name: 'web',
    }
  }
  if (typeof navigator !== 'undefined') {
    const userAgent = navigator.userAgent
    if (/android/i.test(userAgent)) {
      return {
        isWeb: false,
        isNative: true,
        name: 'android',
      }
    }
    if (/iPad|iPhone|iPod/.test(userAgent)) {
      return {
        isWeb: false,
        isNative: true,
        name: 'ios',
      }
    }
  }
  return {
    isWeb: false,
    isNative: false,
    name: 'unknown',
  }
}

export const Platform = getPlatformInfo()
