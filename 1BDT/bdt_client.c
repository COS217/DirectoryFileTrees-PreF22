/*--------------------------------------------------------------------*/
/* bdt_client.c                                                       */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdt.h"

/* Tests the BDT implementation with an assortment of checks.
   Prints the status of the data structure along the way to stderr.
   Returns 0. */
int main(void) {
  char* temp;

  /* Before the data structure is initialized, insertPath, removePath,
     and destroy operations should return INITIALIZATION_ERROR, and
     containsPath should return FALSE for any non-NULL argument, and
     toString should return NULL. */
  assert(BDT_insertPath("a/b/c") == INITIALIZATION_ERROR);
  assert(BDT_containsPath("a/b/c") == FALSE);
  assert((temp = BDT_toString()) == NULL);

  /* After initialization, the data structure is empty, so
     containsPath should still return FALSE for any non-NULL string,
     and toString should return the empty string.*/
  assert(BDT_init() == SUCCESS);
  assert(BDT_containsPath("a/b/c") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* After insertion, the data structure should contain every prefix
     of the inserted path, toString should return a string with these
     prefixes, trying to insert it again should return
     ALREADY_IN_TREE, and trying to insert some other root should
     return CONFLICTING_PATH. */
  assert(BDT_insertPath("a/b/c") == SUCCESS);
  assert(BDT_containsPath("a") == TRUE);
  assert(BDT_containsPath("a/b") == TRUE);
  assert(BDT_containsPath("a/b/c") == TRUE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(BDT_insertPath("a/b/c") == ALREADY_IN_TREE);
  assert(BDT_insertPath("d/e/f") == CONFLICTING_PATH);


  /* After inserting a second child to a node in the hierarchy, trying
     to insert a third child should fail with PARENT_CHILD_ERROR */
  assert(BDT_insertPath("a/b/d") == SUCCESS);
  assert(BDT_containsPath("a/b/d") == TRUE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(BDT_insertPath("a/b/e") == PARENT_CHILD_ERROR);
  assert(BDT_containsPath("a/b/e") == FALSE);
  
  /* Paths must be unique, but individual directory names needn't be */
  assert(BDT_insertPath("a/b/d/e") == SUCCESS);
  assert(BDT_containsPath("a/b/d/e") == TRUE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(BDT_insertPath("a/b/d/e/e") == SUCCESS);
  assert(BDT_containsPath("a/b/d/e/e") == TRUE);
  assert(BDT_containsPath("a/b/d/e/f") == FALSE);
  assert(BDT_containsPath("a/b/d/e/e/e") == FALSE);
  assert(BDT_containsPath("a/b/d/e/e/f") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  /* calling rmPath on a directory that doesn't exist should return
     NO_SUCH_PATH, but on a directory that does exist should return
     SUCCESS and remove entire subtree rooted at that directory  */
  assert(BDT_containsPath("a/b/d/e") == TRUE);
  assert(BDT_containsPath("a/b/d/e/e") == TRUE);
  assert(BDT_containsPath("a/b/d/e/f") == FALSE);
  assert(BDT_rmPath("a/b/d/e/f") == NO_SUCH_PATH);
  assert(BDT_rmPath("a/b/d/e") == SUCCESS);
  assert(BDT_containsPath("a/b/d") == TRUE);
  assert(BDT_containsPath("a/b/d/e") == FALSE);
  assert(BDT_containsPath("a/b/d/e/e") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  /* removing a first child should cause the second child to become
     the first child and remain so until it is itself removed. */
  assert(BDT_insertPath("a/y") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);
  assert(BDT_rmPath("a/b") == SUCCESS);
  assert(BDT_insertPath("a/x") == SUCCESS);
  assert((temp = BDT_toString()) != NULL);
  fprintf(stderr, "%s\n", temp);
  free(temp);

  assert(BDT_destroy() == SUCCESS);
  assert(BDT_destroy() == INITIALIZATION_ERROR);
  assert(BDT_containsPath("a") == FALSE);

  return 0;
}
