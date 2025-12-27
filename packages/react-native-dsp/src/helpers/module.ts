import { ReservedKeyword } from '../enums/module'

export const getReservedKeywords = () =>
  Object.values(ReservedKeyword) as string[]
