/**
 * Node graph management utilities
 */

import type { Node } from './Node';

/**
 * Graph represents a collection of connected nodes
 */
export class Graph {
  private nodes: Set<Node> = new Set();

  /**
   * Add a node to the graph
   */
  addNode(node: Node): void {
    this.nodes.add(node);
  }

  /**
   * Remove a node from the graph
   */
  removeNode(node: Node): void {
    // Disconnect the node from everything
    node.disconnect();

    // Remove from our set
    this.nodes.delete(node);
  }

  /**
   * Get all nodes in the graph
   */
  getNodes(): readonly Node[] {
    return Array.from(this.nodes);
  }

  /**
   * Get the number of nodes in the graph
   */
  size(): number {
    return this.nodes.size;
  }

  /**
   * Clear all nodes and disconnect everything
   */
  clear(): void {
    for (const node of this.nodes) {
      node.disconnect();
    }
    this.nodes.clear();
  }

  /**
   * Dispose all nodes in the graph
   */
  dispose(): void {
    for (const node of this.nodes) {
      node.dispose();
    }
    this.nodes.clear();
  }

  /**
   * Find cycles in the graph (for debugging)
   * Returns true if a cycle is detected
   */
  hasCycle(): boolean {
    const visited = new Set<Node>();
    const recursionStack = new Set<Node>();

    const dfs = (node: Node): boolean => {
      visited.add(node);
      recursionStack.add(node);

      for (const output of node.getOutputs()) {
        if (!visited.has(output)) {
          if (dfs(output)) {
            return true;
          }
        } else if (recursionStack.has(output)) {
          return true; // Cycle detected
        }
      }

      recursionStack.delete(node);
      return false;
    };

    for (const node of this.nodes) {
      if (!visited.has(node)) {
        if (dfs(node)) {
          return true;
        }
      }
    }

    return false;
  }

  /**
   * Get topologically sorted nodes (for processing order)
   * Throws if graph has cycles
   */
  getProcessingOrder(): Node[] {
    if (this.hasCycle()) {
      throw new Error('Cannot process graph with cycles');
    }

    const visited = new Set<Node>();
    const result: Node[] = [];

    const dfs = (node: Node): void => {
      if (visited.has(node)) {
        return;
      }

      visited.add(node);

      // Visit all outputs first (depth-first)
      for (const output of node.getOutputs()) {
        dfs(output);
      }

      // Add this node after all its outputs
      result.unshift(node);
    };

    for (const node of this.nodes) {
      dfs(node);
    }

    return result;
  }
}
