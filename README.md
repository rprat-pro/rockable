The program uses very few (if any) third-party libraries. The calculation code itself is self-sufficient. It relies only on a few small "headers-only" libraries embedded in the folder `common`. On the other hand, the 3D visualization software (`see`) uses the OpenGL programming interface.

The use of the code is not interfaced by any tool (like lua, python or any graphical interface) to facilitate its use, except the input format as described [here](https://richefeu.gitbook.io/cdm/dem/format-of-configuration-files-conf-files). This makes it particularly streamlined and greatly facilitates its integration with other calculation codes. It is in this sense that Rockable is quaified of academic code.