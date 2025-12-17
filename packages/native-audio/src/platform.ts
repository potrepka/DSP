/**
 * Platform detection for Web and React Native
 */

export interface PlatformInfo {
  readonly isWeb: boolean;
  readonly isNative: boolean;
  readonly name: 'web' | 'ios' | 'android' | 'unknown';
}

/**
 * Detects the current platform
 */
function detectPlatform(): PlatformInfo {
  // Check if we're in a browser environment
  if (typeof window !== 'undefined' && typeof document !== 'undefined') {
    return {
      isWeb: true,
      isNative: false,
      name: 'web',
    };
  }

  // Check if we're in React Native
  if (typeof navigator !== 'undefined' && navigator.product === 'ReactNative') {
    // Try to detect iOS vs Android
    const isIOS = typeof navigator.platform !== 'undefined' &&
      /iPad|iPhone|iPod/.test(navigator.platform);
    const isAndroid = typeof navigator.userAgent !== 'undefined' &&
      /Android/.test(navigator.userAgent);

    return {
      isWeb: false,
      isNative: true,
      name: isIOS ? 'ios' : isAndroid ? 'android' : 'unknown',
    };
  }

  // Fallback for Node.js or other environments
  return {
    isWeb: false,
    isNative: false,
    name: 'unknown',
  };
}

/**
 * Platform singleton
 */
export const Platform: PlatformInfo = detectPlatform();
