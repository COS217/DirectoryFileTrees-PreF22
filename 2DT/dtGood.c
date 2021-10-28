/*--------------------------------------------------------------------*/
/* dt.c                                                               */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "dynarray.h"
#include "dt.h"
#include "node.h"
#include "checkerDT.h"

/* A Directory Tree is an AO with 3 state variables: */
/* a flag for if it is in an initialized state (TRUE) or not (FALSE) */
static boolean isInitialized;
/* a pointer to the root node in the hierarchy */
static Node_T root;
/* a counter of the number of nodes in the hierarchy */
static size_t count;

/*
   Starting at the parameter curr, traverses as far down
   the hierarchy as possible while still matching the path
   parameter.

   Returns a pointer to the farthest matching node down that path,
   or NULL if there is no node in curr's hierarchy that matches
   a prefix of the path
*/
static Node_T DT_traversePathFrom(char* path, Node_T curr) {
   Node_T found;
   size_t i;

   assert(path != NULL);

   if(curr == NULL)
      return NULL;

   else if(!strcmp(path,Node_getPath(curr)))
      return curr;

   else if(!strncmp(path, Node_getPath(curr), strlen(Node_getPath(curr)))) {
      for(i = 0; i < Node_getNumChildren(curr); i++) {
         found = DT_traversePathFrom(path,
                                Node_getChild(curr, i));
         if(found != NULL)
            return found;
      }
      return curr;
   }
   return NULL;
}

/*
   Returns the farthest node reachable from the root following a given
   path, or NULL if there is no node in the hierarchy that matches a
   prefix of the path.
*/
static Node_T DT_traversePath(char* path) {
   assert(path != NULL);
   return DT_traversePathFrom(path, root);
}

/*
   Destroys the entire hierarchy of nodes rooted at curr,
   including curr itself.
*/
static void DT_removePathFrom(Node_T curr) {
   if(curr != NULL) {
      count -= Node_destroy(curr);
   }
}

/*
   Given a prospective parent and child node,
   adds child to parent's children list, if possible

   If not possible, destroys the hierarchy rooted at child
   and returns PARENT_CHILD_ERROR, otherwise, returns SUCCESS.
*/
static int DT_linkParentToChild(Node_T parent, Node_T child) {

   assert(parent != NULL);

   if(Node_linkChild(parent, child) != SUCCESS) {
      (void) Node_destroy(child);
      return PARENT_CHILD_ERROR;
   }

   return SUCCESS;
}

/*
   Inserts a new path into the tree rooted at parent, or, if
   parent is NULL, as the root of the data structure.

   If a node representing path already exists, returns ALREADY_IN_TREE

   If there is an allocation error in creating any of the new nodes or
   their fields, returns MEMORY_ERROR

   If there is an error linking any of the new nodes,
   returns PARENT_CHILD_ERROR

   Otherwise, returns SUCCESS
*/
static int DT_insertRestOfPath(char* path, Node_T parent) {

   Node_T curr = parent;
   Node_T firstNew = NULL;
   Node_T new;
   char* copyPath;
   char* restPath = path;
   char* dirToken;
   int result;
   size_t newCount = 0;

   assert(path != NULL);

   if(curr == NULL) {
      if(root != NULL) {
         return CONFLICTING_PATH;
      }
   }
   else {
      if(!strcmp(path, Node_getPath(curr)))
         return ALREADY_IN_TREE;

      restPath += (strlen(Node_getPath(curr)) + 1);
   }

   copyPath = malloc(strlen(restPath)+1);
   if(copyPath == NULL)
      return MEMORY_ERROR;
   strcpy(copyPath, restPath);
   dirToken = strtok(copyPath, "/");

   while(dirToken != NULL) {
      new = Node_create(dirToken, curr);

      if(new == NULL) {
         if(firstNew != NULL)
            (void) Node_destroy(firstNew);
         free(copyPath);
         return MEMORY_ERROR;
      }

      newCount++;

      if(firstNew == NULL)
         firstNew = new;
      else {
         result = DT_linkParentToChild(curr, new);
         if(result != SUCCESS) {
            (void) Node_destroy(new);
            (void) Node_destroy(firstNew);
            free(copyPath);
            return result;
         }
      }

      curr = new;
      dirToken = strtok(NULL, "/");
   }

   free(copyPath);

   if(parent == NULL) {
      root = firstNew;
      count = newCount;
      return SUCCESS;
   }
   else {
      result = DT_linkParentToChild(parent, firstNew);
      if(result == SUCCESS)
         count += newCount;
      else
         (void) Node_destroy(firstNew);

      return result;
   }
}

/* see dt.h for specification */
int DT_insertPath(char* path) {

   Node_T curr;
   int result;

   assert(CheckerDT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return INITIALIZATION_ERROR;
   curr = DT_traversePath(path);
   result = DT_insertRestOfPath(path, curr);
   assert(CheckerDT_isValid(isInitialized,root,count));
   return result;
}

/* see dt.h for specification */
boolean DT_containsPath(char* path) {
   Node_T curr;
   boolean result;

   assert(CheckerDT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return FALSE;

   curr = DT_traversePath(path);


   if(curr == NULL)
      result = FALSE;
   else if(strcmp(path, Node_getPath(curr)))
      result = FALSE;
   else
      result = TRUE;

   assert(CheckerDT_isValid(isInitialized,root,count));
   return result;
}

/*
  Removes the directory hierarchy rooted at path starting from
  curr. If curr is the data structure's root, root becomes NULL.

  Returns NO_SUCH_PATH if curr is not the node for path,
  and SUCCESS otherwise.
 */
static int DT_rmPathAt(char* path, Node_T curr) {

   Node_T parent;

   assert(path != NULL);
   assert(curr != NULL);

   parent = Node_getParent(curr);

   if(!strcmp(path,Node_getPath(curr))) {
      if(parent == NULL)
         root = NULL;
      else
         Node_unlinkChild(parent, curr);

      DT_removePathFrom(curr);

      return SUCCESS;
   }
   else
      return NO_SUCH_PATH;

}

/* see bdt.h for specification */
int DT_rmPath(char* path) {
   Node_T curr;
   int result;

   assert(CheckerDT_isValid(isInitialized,root,count));
   assert(path != NULL);

   if(!isInitialized)
      return INITIALIZATION_ERROR;

   curr = DT_traversePath(path);
   if(curr == NULL)
      result =  NO_SUCH_PATH;
   else
      result = DT_rmPathAt(path, curr);

   assert(CheckerDT_isValid(isInitialized,root,count));
   return result;
}


/* see dt.h for specification */
int DT_init(void) {
   assert(CheckerDT_isValid(isInitialized,root,count));
   if(isInitialized)
      return INITIALIZATION_ERROR;
   isInitialized = 1;
   root = NULL;
   count = 0;
   assert(CheckerDT_isValid(isInitialized,root,count));
   return SUCCESS;
}

/* see dt.h for specification */
int DT_destroy(void) {
   assert(CheckerDT_isValid(isInitialized,root,count));
   if(!isInitialized)
      return INITIALIZATION_ERROR;
   DT_removePathFrom(root);
   root = NULL;
   isInitialized = 0;
   assert(CheckerDT_isValid(isInitialized,root,count));
   return SUCCESS;
}


/*
   Performs a pre-order traversal of the tree rooted at n,
   inserting each payload to DynArray_T d beginning at index i.
   Returns the next unused index in d after the insertion(s).
*/
static size_t DT_preOrderTraversal(Node_T n, DynArray_T d, size_t i) {
   size_t c;

   assert(d != NULL);

   if(n != NULL) {
      (void) DynArray_set(d, i, Node_getPath(n));
      i++;
      for(c = 0; c < Node_getNumChildren(n); c++)
         i = DT_preOrderTraversal(Node_getChild(n, c), d, i);
   }
   return i;
}

/*
   Alternate version of strlen that uses pAcc as an in-out parameter
   to accumulate a string length, rather than returning the length of
   str, and also always adds one more in addition to str's length.
*/
static void DT_strlenAccumulate(char* str, size_t* pAcc) {
   assert(pAcc != NULL);

   if(str != NULL)
      *pAcc += (strlen(str) + 1);
}

/*
   Alternate version of strcat that inverts the typical argument
   order, appending str onto acc, and also always adds a newline at
   the end of the concatenated string.
*/
static void DT_strcatAccumulate(char* str, char* acc) {
   assert(acc != NULL);

   if(str != NULL)
      strcat(acc, str); strcat(acc, "\n");
}

/* see dt.h for specification */
char* DT_toString(void) {
   DynArray_T nodes;
   size_t totalStrlen = 1;
   char* result = NULL;

   assert(CheckerDT_isValid(isInitialized,root,count));

   if(!isInitialized)
      return NULL;

   nodes = DynArray_new(count);
   (void) DT_preOrderTraversal(root, nodes, 0);

   DynArray_map(nodes, (void (*)(void *, void*)) DT_strlenAccumulate, (void*) &totalStrlen);

   result = malloc(totalStrlen);
   if(result == NULL) {
      DynArray_free(nodes);
      assert(CheckerDT_isValid(isInitialized,root,count));
      return NULL;
   }
   *result = '\0';

   DynArray_map(nodes, (void (*)(void *, void*)) DT_strcatAccumulate, (void *) result);

   DynArray_free(nodes);
   assert(CheckerDT_isValid(isInitialized,root,count));
   return result;
}
