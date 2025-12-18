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
  GenericNode
} from '@potrepka/native-audio';

async function main() {
  // Initialize the library (Web only, no-op on Native)
  await initialize();

  console.log('Initializing Engine...');
  const engine = new Engine();

  // Configuration
  // Use -1 for default devices
  const inputDevice = -1;
  const outputDevice = -1;
  const sampleRate = 44100;
  const numChannels = 2;
  const numSamples = 512;

  engine.setup(inputDevice, outputDevice, numSamples, sampleRate);
  engine.start();

  console.log(`Engine started.`);
  console.log(`Input Device: ${engine.getInputDeviceName()}`);
  console.log(`Output Device: ${engine.getOutputDeviceName()}`);
  console.log(`Sample Rate: ${engine.getSampleRate()}`);
  console.log(`Buffer Size: ${engine.getNumSamples()}`);

  const processor = engine.getNodeProcessor();
  const root = processor.getDefaultNode();

  const phasor = new Phasor();
  const oscillator = new TableOscillator();
  const filter = new Biquad();
  const gain = new Gain();

  phasor.setNumChannels(numChannels);
  oscillator.setNumChannels(numChannels);
  filter.setNumChannels(numChannels);
  gain.setNumChannels(numChannels);

  phasor.getFrequency().setAllChannelValues(440);
  filter.getFrequency().setAllChannelValues(1000.0);
  filter.getResonance().setAllChannelValues(1.0);
  gain.getFactor().setAllChannelValues(0.5);

  root.addChild(phasor);
  root.addChild(oscillator);
  root.addChild(filter);
  root.addChild(gain);

  phasor.connect(oscillator);
  oscillator.connect(filter);
  filter.connect(gain);
  gain.connect(root);

  console.log('Graph setup complete.');

  await new Promise(r => setTimeout(r, 1000));

  engine.dispose();
  phasor.dispose();
  oscillator.dispose();
  filter.dispose();
  gain.dispose();
}

main().catch(console.error);
