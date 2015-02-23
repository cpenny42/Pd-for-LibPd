# PD-for-LIBPD
A collection of Pure-Data patches to provide extra functionality while also being LIBPD-compatible.  

To use these patches, add the /pd-extended and /pd-for-libpd folders to Pure Data's search path. Most of the patches have documentation, but there are still more help files to be added. 

Note many pd-extended patches have been added - most of these are vanilla compatible, but some require external files be added to the libpd project.  You can add these external files to a libpd project - see [github.com/libpd/pd-for-ios](https://github.com/libpd/pd-for-ios) for more info.

The sinesum executable will print the sinesum coefficients for a given wave type & number of harmonics for sinesum messages to arrays.

To use this framework in a LibPd project, call addToSearchPath() and add both folders in /source. 