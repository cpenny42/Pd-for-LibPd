import sys
import wave
import numpy
import fluidsynth
import shutil
import os

if len(sys.argv) != 2:
	sys.stderr.write('\nError: incorrect arguments: python import_sf.py soundfont_to_unpack.sf2\n\n')
else:

	notes = range(0, 127)
	velocities = [1, 31, 61, 91, 111, 127]
	fs = 44100
	duration = 4
	decay = 1
	pack = sys.argv[1].split('.')[0]
	wav_parameters = (2, 2, fs, 0, 'NONE', 'not compressed')

	if os.path.exists(pack):
		shutil.rmtree(pack)

	os.makedirs(pack)
	config = open('%s/config.txt' % pack, 'w')

	synth = fluidsynth.Synth()

	sfid = synth.sfload(sys.argv[1])
	synth.program_select(0, sfid, 0, 0)

	sys.stderr.write('\nUnpacking Soundfonts to .wav files.......')

	for note in notes:
		config.write('\n%d ' % note)
		sys.stderr.write('\n%d ' % note)
		for velocity in velocities:

			samples = []

			synth.noteon(0, note, velocity)
			samples = numpy.append(samples, synth.get_samples(duration * fs))

			synth.noteoff(0, note)
			samples = numpy.append(samples, synth.get_samples(decay * fs))

			s = fluidsynth.raw_audio_string(samples)

			writer = wave.open('%s/%d_%d.wav' % (pack, note, velocity), 'wb')
			writer.setparams(wav_parameters)
			writer.writeframesraw(s)
			writer.close()

			config.write('%d ' % velocity) 
			sys.stderr.write('%d ' % velocity) 

	synth.delete()
	config.write('\n') 
	sys.stderr.write('\n\n')
