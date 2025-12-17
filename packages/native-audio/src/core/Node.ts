/**
 * Node - Base class for all DSP processing nodes
 * Note: This is an abstract class that wraps the C++ Node class
 * All TypeScript nodes extend this to match C++ inheritance hierarchy
 */

import { Disposable } from './Disposable';

export abstract class Node extends Disposable {
  /**
   * Check if this node is currently active (processing audio)
   */
  isActive(): boolean {
    this.checkDisposed();
    return this.instance.isActive();
  }

  /**
   * Set whether this node is active (processing audio)
   * @param active - True to enable processing, false to disable
   */
  setActive(active: boolean): void {
    this.checkDisposed();
    this.instance.setActive(active);
  }

  /**
   * Get the number of channels for this node
   */
  getNumChannels(): number {
    this.checkDisposed();
    return this.instance.getNumChannels();
  }

  /**
   * Set the number of channels for this node
   * @param numChannels - Number of channels (typically 1-8)
   */
  setNumChannels(numChannels: number): void {
    this.checkDisposed();
    this.instance.setNumChannels(numChannels);
  }

  /**
   * Get the number of input channels
   */
  getNumInputChannels(): number {
    this.checkDisposed();
    return this.instance.getNumInputChannels();
  }

  /**
   * Set the number of input channels
   * @param numChannels - Number of input channels
   */
  setNumInputChannels(numChannels: number): void {
    this.checkDisposed();
    this.instance.setNumInputChannels(numChannels);
  }

  /**
   * Get the number of output channels
   */
  getNumOutputChannels(): number {
    this.checkDisposed();
    return this.instance.getNumOutputChannels();
  }

  /**
   * Set the number of output channels
   * @param numChannels - Number of output channels
   */
  setNumOutputChannels(numChannels: number): void {
    this.checkDisposed();
    this.instance.setNumOutputChannels(numChannels);
  }

  /**
   * Get the number of samples per processing block
   */
  getNumSamples(): number {
    this.checkDisposed();
    return this.instance.getNumSamples();
  }

  /**
   * Set the number of samples per processing block
   * @param numSamples - Number of samples (buffer size)
   */
  setNumSamples(numSamples: number): void {
    this.checkDisposed();
    this.instance.setNumSamples(numSamples);
  }

  /**
   * Get the sample rate
   */
  getSampleRate(): number {
    this.checkDisposed();
    return this.instance.getSampleRate();
  }

  /**
   * Set the sample rate
   * @param sampleRate - Sample rate in Hz (e.g., 44100, 48000)
   */
  setSampleRate(sampleRate: number): void {
    this.checkDisposed();
    this.instance.setSampleRate(sampleRate);
  }

  /**
   * Get the reciprocal of the number of samples (1 / numSamples)
   * Useful for normalized time calculations
   */
  getOneOverNumSamples(): number {
    this.checkDisposed();
    return this.instance.getOneOverNumSamples();
  }

  /**
   * Get the reciprocal of the sample rate (1 / sampleRate)
   * Useful for time calculations
   */
  getOneOverSampleRate(): number {
    this.checkDisposed();
    return this.instance.getOneOverSampleRate();
  }

  /**
   * Get all input connections for this node
   * @returns Array of Input instances
   */
  getInputs(): any[] {
    this.checkDisposed();
    return this.instance.getInputs();
  }

  /**
   * Get all output connections for this node
   * @returns Array of Output instances
   */
  getOutputs(): any[] {
    this.checkDisposed();
    return this.instance.getOutputs();
  }

  /**
   * Get all child nodes
   * @returns Array of child Node instances
   */
  getChildren(): any[] {
    this.checkDisposed();
    return this.instance.getChildren();
  }

  /**
   * Add a child node to this node's processing graph
   * @param child - Node to add as a child
   */
  addChild(child: any): void {
    this.checkDisposed();
    this.instance.addChild(child);
  }

  /**
   * Remove a child node from this node's processing graph
   * @param child - Node to remove
   */
  removeChild(child: any): void {
    this.checkDisposed();
    this.instance.removeChild(child);
  }

  /**
   * Sort children nodes (for processing order)
   */
  sortChildren(): void {
    this.checkDisposed();
    this.instance.sortChildren();
  }

  /**
   * Disconnect all inputs and outputs
   */
  disconnectAll(): void {
    this.checkDisposed();
    this.instance.disconnectAll();
  }

  /**
   * Process audio through this node
   * Note: This is typically called by NodeProcessor, not directly
   */
  process(): void {
    this.checkDisposed();
    this.instance.process();
  }
}
