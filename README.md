# PD-for-LIBPD
A collection of Pure-Data patches to provide extra functionality while also being LIBPD-compatible.  

Note that many pd-extended patches have been added - most of these are vanilla compatible, but some require external files be added to the libpd project.  Info on this process can be found at: [github.com/libpd/pd-for-ios](https://github.com/libpd/pd-for-ios)

The /Documentation Folder contains organized help patches for all the objects, along with screenshots for viewing outside PD. This will continue to be updated until it is comprehensive of the whole library.

The sinesum executable will print out the fourier series coefficients for a given wave type & number of harmonics that you can use with Pd's sinesum method for arrays.

To use this framework in a LibPd project, call the addToSearchPath function when you initialize PdBase to include all directories in the /source folder above that contain Pd patches. As of now there is the /pd-for-libpd directory with vanilla-guarenteed compatibility, and /pd-extended that has mostly vanilla-compatible patches along with some that need pd-extended objects.  /pd-extended also has many pd-extended gui objects implemented in Pure Data making them libpd compatible.  You can also add both folders to the search path in Pure-Data in the Preferences menu. More info at: [github.com/libpd](https://github.com/libpd/)

This repository is incomplete and will continue to be updated.