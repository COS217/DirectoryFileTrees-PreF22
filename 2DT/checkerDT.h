/*--------------------------------------------------------------------*/
/* checkerDT.h                                                        */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef CHECKER_INCLUDED
#define CHECKER_INCLUDED

#include "node.h"


/*
   Returns TRUE if n represents a directory entry
   in a valid state, or FALSE otherwise.
*/
boolean CheckerDT_Node_isValid(Node_T n);

/*
   Returns TRUE if the hierarchy is in a valid state or FALSE
   otherwise.  The data structure's validity is based on a boolean
   isInit indicating whether it has been initialized, a Node_T root
   representing the root of the hierarchy, and a size_t count
   representing the total number of directories in the hierarchy.
*/
boolean CheckerDT_isValid(boolean isInit, Node_T root, size_t count);

#endif
