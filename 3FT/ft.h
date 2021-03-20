/*--------------------------------------------------------------------*/
/* ft.h                                                               */
/* Author: Christopher Moretti and Vikash Modi '23                    */
/*--------------------------------------------------------------------*/

#ifndef FT_INCLUDED
#define FT_INCLUDED

/*
  A File Tree is a representation of a hierarchy of directories and
  files: the File Tree is rooted at a directory, directories
  may be leaves or non-leaves, and files are always leaves.
*/

#include <stddef.h>
#include "a4def.h"

/*
   Inserts a new directory into the tree at path, if possible.
   Returns SUCCESS if the new directory is inserted.
   Returns INITIALIZATION_ERROR if not in an initialized state.
   Returns CONFLICTING_PATH if path is not underneath existing root.
   Returns NOT_A_DIRECTORY if a proper prefix of path exists as a file.
   Returns ALREADY_IN_TREE if the path already exists (as dir or file).
   Returns PARENT_CHILD_ERROR if a new child cannot be added in path.
   Returns MEMORY_ERROR if unable to allocate sufficient memory.
*/
int FT_insertDir(char *path);

/*
  Returns TRUE if the tree contains the full path parameter as a
  directory and FALSE otherwise.
*/
boolean FT_containsDir(char *path);

/*
  Removes the FT hierarchy rooted at the directory path.
  Returns SUCCESS if found and removed.
  Returns INITIALIZATION_ERROR if not in an initialized state.
  Returns NOT_A_DIRECTORY if path exists but is a file not a directory.
  Returns NO_SUCH_PATH if the path does not exist in the hierarchy.
*/
int FT_rmDir(char *path);

/*
   Inserts a new file into the hierarchy at the given path, with the
   given contents of size length bytes.
   Returns SUCCESS if the new file is inserted.
   Returns INITIALIZATION_ERROR if not in an initialized state.
   Returns CONFLICTING_PATH if path is not underneath existing root, 
                            or if path would be the FT root.
   Returns NOT_A_DIRECTORY if a proper prefix of path exists as a file.
   Returns ALREADY_IN_TREE if the path already exists (as dir or file).
   Returns PARENT_CHILD_ERROR if a new child cannot be added in path.
   Returns MEMORY_ERROR if unable to allocate sufficient memory.
*/
int FT_insertFile(char *path, void *contents, size_t length);

/*
  Returns TRUE if the tree contains the full path parameter as a
  file and FALSE otherwise.
*/
boolean FT_containsFile(char *path);

/*
  Removes the FT file at path.
  Returns SUCCESS if found and removed.
  Returns INITIALIZATION_ERROR if not in an initialized state.
  Returns NOT_A_FILE if path exists but is a directory not a file.
  Returns NO_SUCH_PATH if the path does not exist in the hierarchy.
*/
int FT_rmFile(char *path);

/*
  Returns the contents of the file at the full path parameter.
  Returns NULL if the path does not exist or is a directory.

  Note: checking for a non-NULL return is not an appropriate
  contains check -- the contents of a file may be NULL.
*/
void *FT_getFileContents(char *path);

/*
  Replaces current contents of the file at the full path parameter with
  the parameter newContents of size newLength.
  Returns the old contents if successful. (Note: contents may be NULL.)
  Returns NULL if the path does not already exist or is a directory.
*/
void *FT_replaceFileContents(char *path, void *newContents,
                             size_t newLength);

/*
  Returns SUCCESS if path exists in the hierarchy,
  returns NO_SUCH_PATH if it does not, and
  returns INITIALIZATION_ERROR if the structure is not initialized.

  When returning SUCCESSS,
  if path is a directory: *type is set to FALSE, *length is unchanged
  if path is a file: *type is set to TRUE, and
                     *length is set to the length of file's contents.

  When returning a non-SUCCESS status, *type and *length are unchanged.
 */
int FT_stat(char *path, boolean *type, size_t *length);

/*
  Sets the data structure to initialized status.
  The data structure is initially empty.
  Returns INITIALIZATION_ERROR if already initialized,
  and SUCCESS otherwise.
*/
int FT_init(void);

/*
  Removes all contents of the data structure and
  returns it to uninitialized status.
  Returns INITIALIZATION_ERROR if not already initialized,
  and SUCCESS otherwise.
*/
int FT_destroy(void);

/*
  Returns a string representation of the
  data structure, or NULL if the structure is
  not initialized or there is an allocation error.

  Allocates memory for the returned string,
  which is then owned by client!
*/
char *FT_toString(void);

#endif
