# PD-for-LIBPD
A framework for using Pure Data as an audio engine for mobile apps.

There are many Pure Data patches to help with development - These are found in /source/pd-for-libpd.  To use them in Pure Data, add that folder to Pd's search path.  They must be in your main resource bundle for use in libpd.

There are a few externals available - /source/externals/use has the c files & the PdExternals class needed to use them in libpd.  The /compiled folder contains binaries of any additional externals for use in Pure Data - you can find the source code in source/externals/build to build them on your own machine.

Soundfonts are currently a work in progress.  To use them in Pure Data, use the import_sf.py script in /soundfonts to unpack a soundfont into .wav files for use with autosampler~ and add the /soundfonts folder to Pd's search path.  In a later update, you will be able to use an external object (autosampler~.c) to use the soundfonts in libpd without needing to include the unpacked .wav files.

There is also a program for generating sinesum coefficients of different types of waves - this can be used with the sinesum array method in Pd for generating waveforms.