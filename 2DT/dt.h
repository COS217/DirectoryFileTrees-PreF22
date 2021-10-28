/*--------------------------------------------------------------------*/
/* dt.h                                                               */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef DT_INCLUDED
#define DT_INCLUDED

/*
  A Directory Tree is a representation of a directory hierarchy.
*/

#include "a4def.h"

/*
   Inserts a new directory into the tree at path, if possible.
   Returns SUCCESS if the new path is inserted, otherwise:
   returns INITIALIZATION_ERROR if not in an initialized state,
   returns CONFLICTING_PATH if path is not underneath existing root,
   returns ALREADY_IN_TREE if the path already exists,
   Returns MEMORY_ERROR if unable to allocate any node or any field.
   Returns PARENT_CHILD_ERROR if a parent cannot link to a new child.
*/
int DT_insertPath(char* path);

/*
  Returns TRUE if the tree contains the full path parameter
  and FALSE otherwise.
*/
boolean DT_containsPath(char* path);

/*
  Removes the directory hierarchy rooted at path.
  Returns SUCCESS if found and removed, otherwise:
  returns INITIALIZATION_ERROR if not in an initialized state,
  returns NO_SUCH_PATH if not found.
*/
int DT_rmPath(char* path);

/*
  Sets the data structure to initialized status.
  The data structure is initially empty.
  Returns INITIALIZATION_ERROR if already initialized,
  and SUCCESS otherwise.
*/
int DT_init(void);

/*
  Removes all contents of the data structure and
  returns it to uninitialized status.
  Returns INITIALIZATION_ERROR if not already initialized,
  and SUCCESS otherwise.
*/
int DT_destroy(void);

/*
  Returns a string representation of the
  data structure, or NULL if the structure is
  not initialized or there is an allocation error.

  Allocates memory for the returned string,
  which is then owned by client!
*/
char* DT_toString(void);

#endif
