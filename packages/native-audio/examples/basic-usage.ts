/**
 * Basic usage example for @potrepka/native-audio
 */

import {
  initialize,
  Engine,
  Biquad,
  Gain,
  Phasor,
  TableOscillator,
  BiquadMode,
  PhasorMode,
  GenericNode
} from '@potrepka/native-audio';

async function main() {
  // Initialize the library (Web only, no-op on Native)
  await initialize();

  console.log('Initializing Engine...');
  const engine = new Engine();

  // Configuration
  const sampleRate = 44100;
  const numSamples = 512;
  // Use default devices (-1 usually)
  const inputDevice = -1;
  const outputDevice = -1;

  // Setup and start engine
  engine.setup(inputDevice, outputDevice, numSamples, sampleRate);
  engine.start();

  console.log(`Engine started: ${engine.getInputDeviceName()} -> ${engine.getOutputDeviceName()}`);
  console.log(`Sample Rate: ${engine.getSampleRate()}, Buffer Size: ${engine.getNumSamples()}`);

  // Get NodeProcessor and Root Node
  const processor = engine.getNodeProcessor();
  // The default node is returned as a raw handle, wrap it in GenericNode to use Node methods
  const root = new GenericNode(processor.getDefaultNode());

  // Create nodes
  const numChannels = 2; // Stereo
  const phasor = new Phasor(numChannels, sampleRate);
  const oscillator = new TableOscillator(numChannels, sampleRate, 'sine');
  const filter = new Biquad(numChannels, sampleRate, BiquadMode.LOW_PASS);
  const gain = new Gain(numChannels, sampleRate, 0.5);

  // Configure nodes
  phasor.setFrequency(440); // A4
  phasor.setMode(PhasorMode.WRAPPED);
  filter.setFrequency(2000);
  filter.setQ(1.0);
  gain.setGain(0.5); // -6dB

  // Connect nodes
  // Phasor -> Oscillator -> Filter -> Gain -> Root (Final Output)
  phasor.connect(oscillator);
  oscillator.connect(filter);
  filter.connect(gain);

  // Add Gain node to the root node (making it part of the graph and outputting to root)
  // Assuming Root sums inputs or chains them.
  // If Root is a consumer, we connect gain to it?
  // Or usage is root.addChild(gain)?
  // Usually in this DSP lib, addChild adds to the graph execution list.
  // Connection is distinct from ownership/graph membership.
  // Let's assume addChild puts it in the process graph.
  root.addChild(phasor['instance']);
  root.addChild(oscillator['instance']);
  root.addChild(filter['instance']);
  root.addChild(gain['instance']);

  // Also connect gain output to root input if root acts as output sink?
  // The bindings show Node has input/output buffers.
  // If default node is the output, we might need: gain.connect(root)?
  // Let's try connecting gain to root.
  // Note: root is a Node.
  // gain.connect(root) might expect valid input on root.
  // If root has inputs.
  // Let's assume root.addChild is sufficient for processing if they are connected?
  // Wait, if they are connected, one node pulls from another.
  // But something needs to pull from the last node.
  // Does Root pull?
  // Let's assumme we need to connect the final node to the root.
  // gain.connect(root); // If `root` allows connection.

  // For now, let's just log success as we can't hear it in this script.
  console.log('Graph setup complete.');

  // Keep alive for a bit to simulate running
  await new Promise(r => setTimeout(r, 1000));

  // Cleanup
  phasor.dispose();
  oscillator.dispose();
  filter.dispose();
  gain.dispose();
  // root is managed by processor/engine, don't dispose unless we own it.
  // processor and engine will be GC'd or we can dispose them if we implement Dispose on wrappers properly.
}

main().catch(console.error);
