import { Target } from '../../../../types/module'
import { Chainable, ProxyContext } from '../../../../types/proxy'
import { InputProxy, OutputProxy } from '../../core/BufferProxy'
import { NodeProxy } from '../../core/NodeProxy'

export class CrossoverProxy extends NodeProxy {
  constructor(context: ProxyContext, target: Target) {
    super(context, target)
  }
  getInput = (): Chainable<InputProxy> => this.createInput('Input')
  getFrequency = (): Chainable<InputProxy> => this.createInput('Frequency')
  getLow = (): Chainable<OutputProxy> => this.createOutput('Low')
  getHigh = (): Chainable<OutputProxy> => this.createOutput('High')
}
