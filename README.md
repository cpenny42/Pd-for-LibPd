# PD-for-LIBPD
A collection of Pure-Data patches to provide extra functionality while also being LIBPD-compatible.  

The /Documentation Folder contains organized help patches for all the objects, along with screenshots for viewing outside PD. This will continue to be updated until it is comprehensive of the whole library.

/Generate_Sinesum is an xcode project for the "sinesum" executable.  It will print out the fourier series coefficients for a given wave type & number of harmonics that you can use with Pd's sinesum method for arrays.


To use this framework in a LibPd project, call the addToSearchPath function when you initialize PdBase to include the /source folder above that contains the Pd patches.  You can also add the folder the search path in Pure-Data in the Preferences menu.


This repository will continue to be updated - not all patches have help files yet & there are some additional patches that will be added. Be sure to check for updates periodically.
