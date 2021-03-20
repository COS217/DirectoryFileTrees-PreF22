/*--------------------------------------------------------------------*/
/* bdt.h                                                              */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#ifndef BDT_INCLUDED
#define BDT_INCLUDED

/*
  The Binary Directory Tree is a representation of a directory
  hierarchy in which each directory has only 0, 1, or 2 subdirectories.
*/

/* Return statuses */
enum { SUCCESS,
       INITIALIZATION_ERROR, PARENT_CHILD_ERROR , ALREADY_IN_TREE,
       NO_SUCH_PATH, CONFLICTING_PATH, MEMORY_ERROR
};

/* In lieu of a proper boolean datatype */
typedef enum bool { FALSE, TRUE } boolean;

/*
   Inserts a new path into the tree, if possible.
   Returns SUCCESS if the new path is inserted,
   INITIALIZATION_ERROR if not in an initialized state,
   ALREADY_IN_TREE if the path is already found in the data structure,
   CONFLICTING_PATH if the path conflicts with the existing root,
   PARENT_CHILD_ERROR if the path would require a 3rd child of a node,
   or  MEMORY_ERROR if unable to allocate memory for a new node.
*/
int BDT_insertPath(char* path);

/*
  Returns TRUE if the tree contains a node
  representing the full path parameter
  and FALSE otherwise.
*/
boolean BDT_containsPath(char* path);

/*
  Removes the directory hierarchy rooted at path.
  Returns SUCCESS if able to be found and removed,
  INITIALIZATION_ERROR if not in an initialized state,
  or NO_SUCH_PATH if path is not present in the data structure.
*/
int BDT_rmPath(char* path);

/*
  Sets the data structure to initialized status
  The data structure is initially empty.
  Returns INITIALIZATION_ERROR if already initialized,
  and SUCCESS otherwise.
*/
int BDT_init(void);

/*
  Removes all contents of the data structure and
  returns it to uninitialized status.
  Returns INITIALIZATION_ERROR if not already initialized,
  and SUCCESS otherwise.
*/
int BDT_destroy(void);

/*
  Returns a string representation of the
  data structure, or NULL if the structure is
  not initialized or there is an allocation error.

  Allocates memory for the returned string,
  which is then owned by client!
*/
char* BDT_toString(void);

#endif
