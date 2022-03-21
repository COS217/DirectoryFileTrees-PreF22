/*--------------------------------------------------------------------*/
/* dt_client.c                                                        */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dt.h"

/* Tests the DT implementation with an assortment of checks.
   Prints the status of the data structure along the way to stderr.
   Returns 0. */
int main(void) {
  char* temp;

  /* Before the data structure is initialized, insertPath, removePath,
     and destroy operations should return INITIALIZATION_ERROR, and
     containsPath should return FALSE for any non-NULL argument, and
     toString should return NULL. */
  assert(DT_insertPath("a/bb/c") == INITIALIZATION_ERROR);
  assert(DT_containsPath("a/bb/c") == FALSE);
  assert((temp = DT_toString()) == NULL);

  /* After initialization, the data structure is empty, so
     containsPath should still return FALSE for any non-NULL string,
     and toString should return the empty string.*/
  assert(DT_init() == SUCCESS);
  assert(DT_containsPath("a/bb/c") == FALSE);
  assert((temp = DT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* After insertion, the data structure should contain every prefix
     of the inserted path, toString should return a string with these
     prefixes, trying to insert it again should return
     ALREADY_IN_TREE, and trying to insert some other root should
     return CONFLICTING_PATH. */
  assert(DT_insertPath("a/bb/c") == SUCCESS);
  assert(DT_containsPath("a") == TRUE);
  assert(DT_containsPath("a/bb") == TRUE);
  assert(DT_containsPath("a/bb/c") == TRUE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(DT_insertPath("a/bb/c") == ALREADY_IN_TREE);
  assert(DT_insertPath("d/e/f") == CONFLICTING_PATH);

  /* After inserting a second child to a node in the hierarchy, trying
     to insert a third child should still succeed, unlike in BDT */
  assert(DT_insertPath("a/bb/d") == SUCCESS);
  assert(DT_containsPath("a/bb/d") == TRUE);
  assert(DT_insertPath("a/bb/e") == SUCCESS);
  assert(DT_containsPath("a/bb/e") == TRUE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  /* Paths must be unique, but individual directory names needn't be */
  assert(DT_insertPath("a/bb/d/e") == SUCCESS);
  assert(DT_containsPath("a/bb/d/e") == TRUE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(DT_insertPath("a/bb/d/e/e") == SUCCESS);
  assert(DT_containsPath("a/bb/d/e/e") == TRUE);
  assert(DT_containsPath("a/bb/d/e/f") == FALSE);
  assert(DT_containsPath("a/bb/d/e/e/e") == FALSE);
  assert(DT_containsPath("a/bb/d/e/e/f") == FALSE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  /* calling rmPath on a directory that doesn't exist should return
     NO_SUCH_PATH, but on a directory that does exist should return
     SUCCESS and remove entire subtree rooted at that directory  */
  assert(DT_containsPath("a/bb/d/e") == TRUE);
  assert(DT_containsPath("a/bb/d/e/e") == TRUE);
  assert(DT_containsPath("a/bb/d/e/f") == FALSE);
  assert(DT_rmPath("a/bb/d/e/f") == NO_SUCH_PATH);
  assert(DT_rmPath("a/bb/d/e") == SUCCESS);
  assert(DT_containsPath("a/bb/d") == TRUE);
  assert(DT_containsPath("a/bb/d/e") == FALSE);
  assert(DT_containsPath("a/bb/d/e/e") == FALSE);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  /* children should be stored in lexicographic order */
  assert(DT_insertPath("a/y") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(DT_rmPath("a/bb") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(DT_insertPath("a/x") == SUCCESS);
  assert((temp = DT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  assert(DT_destroy() == SUCCESS);
  assert(DT_destroy() == INITIALIZATION_ERROR);
  assert(DT_containsPath("a") == FALSE);

  return 0;
}
