/** @file
 *  Program bootstrap.
 *
 *  Parses command line arguments, loads any preliminary dependencies, displays splash/loading window/screen, and passes
 *  execution off to the primary Simulation object.
 */
#include "Simulation.hpp"

#ifdef DEBUG
#include <iostream>
#endif // DEBUG
