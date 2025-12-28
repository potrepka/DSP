import { Space, Type } from '../enums'
import { ReservedKeyword } from '../enums/module'
import { Module, Node, NodeProps, NodeType } from '../types'

export const getReservedKeywords = () =>
  Object.values(ReservedKeyword) as string[]

export const constructNode = <T extends NodeType>(
  module: Module,
  nodeType: T,
  props: NodeProps<T>,
) => {
  let node: Node
  switch (nodeType) {
    case 'Phasor':
      node = new module.Phasor()
      break
    case 'Multiplication': {
      const { type, space } = props as NodeProps<'Multiplication'>
      node = new module.Multiplication(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Negative': {
      const { type, space } = props as NodeProps<'Negative'>
      node = new module.Negative(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    default:
      throw new Error(`Unsupported node type: ${nodeType}`)
  }
  return node
}
