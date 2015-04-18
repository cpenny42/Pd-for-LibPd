# PD-for-LIBPD
A framework for using Pure Data as an audio engine for mobile apps.

There are many Pure Data patches to help with development - These are found in /source/pd-for-libpd.  To use them in Pure Data, add that folder to Pd's search path.  They must be in your main resource bundle for use in libpd.

Some externals are available - /source/externals/use has the c files & the PdExternals class needed to use them in libpd. Import this entire folder into your project to use the externals.  The /compiled folder contains binaries of any additional externals for use in Pure Data - you can find the source code in source/externals/build to build them on your own machine.

There is an external object that allows you to read soundfonts, and there are a few .sf2 files provided in the /soundfonts folder.

There is also a program for generating sinesum coefficients of different types of waves - this can be used with the sinesum array method in Pd for generating waveforms.

This is a work in progress and will continue to be updated.