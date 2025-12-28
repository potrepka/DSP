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
    case 'Multiplication':
      // @ts-ignore
      node = new module.Multiplication(props.type, props.space)
      break
    default:
      throw new Error(`Unsupported node type: ${nodeType}`)
  }
  return node
}
