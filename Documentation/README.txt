Pd for LibPd:

These are a collection of patches to link to your libpd project that provide extra functionality (including some pd-extended externals) when limited to libpd.  Most of the externals will work directly with vanilla pd, but some of them require that you add the external C files to your project first.

This is a work in progress and not all patches have documentation. Work is underway to include these in a fork of libpd.

Note that [expr], [expr~], or [fexpr~] are under a GPL license and can't be used in an iOS project. The patches with this limitation will eventually be modified or removed.