To install:

Get pip (it comes with python in homebrew):

	brew install python

Download dependencies:

	sudo pip install fluidsynth
	sudo pip install numpy


To unpack a soundfont:

	python import_sf.py your_soundfont.sf2

It will unpack .wav files for each note & velocity in a separate folder.  The folder with the .wav files needs to be in Pd's search path.

The patch uses the file name of each .wav to determine which note & velocity to play, and the soundfont's file name determines the name of the soundfont in Pure Data.

To use the soundfont, send [autosampler~] the message: "set your_soundfont".

Run ./testsf.sh to automatically install a few soundfonts and open test_autosampler~.pd to try them out.

This could take a lot of space! You can change the samples, velocities, and durations of the .wav files in import_sf.py if you want to save space.  There is a more efficient solution that will be used when deploying to a device.

There will be an equivalent external object that reads the soundfonts directly to avoid needing to bundle many .wav files with your app.