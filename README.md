# PD-for-LIBPD
A collection of Pure-Data patches to provide extra functionality while also being LIBPD-compatible.  

The /Documentation Folder contains organized help patches for all the objects, along with screenshots for viewing outside PD.

/Generate_Sinesum is an xcode project for the "sinesum" executable.  It will print out the sine summation coefficients for a given wave type & number of harmonics that you can use with PD's sinesum method for arrays.

Without altering LIBPD to include external patches, the only way to use additional patches in a LIBPD project is to copy them into the directory you're working in.

To use this framework in a LIBPD project, simply copy everything in the /source folder into your Pure-Data project folder.  I recommend naming your main patch with a "_" (such as "_Main.pd") in for easier organization.


This repository will continue to be updated - not all patches have help files yet & there are some additional patches that will be added. Be sure to check for updates periodically.