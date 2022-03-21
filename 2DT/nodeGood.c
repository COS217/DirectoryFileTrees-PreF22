/*--------------------------------------------------------------------*/
/* node.c                                                             */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "dynarray.h"
#include "node.h"
#include "checkerDT.h"

/*
   A node structure represents a directory in the directory tree
*/
struct node {
   /* the full path of this directory */
   char* path;

   /* the parent directory of this directory
      NULL for the root of the directory tree */
   Node_T parent;

   /* the subdirectories of this directory
      stored in sorted order by pathname */
   DynArray_T children;
};


/*
  returns a path with contents
  n->path/dir
  or NULL if there is an allocation error.

  Allocates memory for the returned string,
  which is then owned by the caller!
*/
static char* Node_buildPath(Node_T n, const char* dir) {
   char* path;

   assert(dir != NULL);

   if(n == NULL)
      path = malloc(strlen(dir)+1);
   else
      path = malloc(strlen(n->path) + 1 + strlen(dir) + 1);

   if(path == NULL)
      return NULL;
   *path = '\0';

   if(n != NULL) {
      strcpy(path, n->path);
      strcat(path, "/");
   }
   strcat(path, dir);

   return path;
}

/* see node.h for specification */
Node_T Node_create(const char* dir, Node_T parent){
   Node_T new;

   assert(parent == NULL || CheckerDT_Node_isValid(parent));
   assert(dir != NULL);

   new = malloc(sizeof(struct node));
   if(new == NULL) {
      assert(parent == NULL || CheckerDT_Node_isValid(parent));
      return NULL;
   }

   new->path = Node_buildPath(parent, dir);

   if(new->path == NULL) {
      free(new);
      assert(parent == NULL || CheckerDT_Node_isValid(parent));
      return NULL;
   }

   new->parent = parent;
   new->children = DynArray_new(0);
   if(new->children == NULL) {
      free(new->path);
      free(new);
      assert(parent == NULL || CheckerDT_Node_isValid(parent));
      return NULL;
   }

   assert(parent == NULL || CheckerDT_Node_isValid(parent));
   assert(CheckerDT_Node_isValid(new));
   return new;
}

/* see node.h for specification */
size_t Node_destroy(Node_T n) {
   size_t i;
   size_t count = 0;
   Node_T c;

   assert(n != NULL);

   for(i = 0; i < DynArray_getLength(n->children); i++)
   {
      c = DynArray_get(n->children, i);
      count += Node_destroy(c);
   }
   DynArray_free(n->children);

   free(n->path);
   free(n);
   count++;

   return count;
}

/* see node.h for specification */
/*char* Node_getPath(Node_T n) {
   char* pathCopy;

   assert(n != NULL);

   pathCopy = malloc(strlen(n->path)+1);
   if(pathCopy == NULL)
      return NULL;

   return strcpy(pathCopy, n->path);
}
*/
const char* Node_getPath(Node_T n) {
   assert(n != NULL);

   return n->path;
}

/* see node.h for specification */
int Node_compare(Node_T node1, Node_T node2) {
   assert(node1 != NULL);
   assert(node2 != NULL);

   return strcmp(node1->path, node2->path);
}

/* see node.h for specification */
size_t Node_getNumChildren(Node_T n) {
   assert(n != NULL);

   return DynArray_getLength(n->children);
}

/* see node.h for specification */
int Node_hasChild(Node_T n, const char* path, size_t* childID) {
   size_t index;
   int result;
   Node_T checker;

   assert(n != NULL);
   assert(path != NULL);

   checker = Node_create(path, NULL);
   if(checker == NULL) {
      return -1;
   }
   result = DynArray_bsearch(n->children, checker, &index,
                    (int (*)(const void*, const void*)) Node_compare);
   (void) Node_destroy(checker);

   if(childID != NULL)
      *childID = index;

   return result;
}

/* see node.h for specification */
Node_T Node_getChild(Node_T n, size_t childID) {
   assert(n != NULL);

   if(DynArray_getLength(n->children) > childID) {
      return DynArray_get(n->children, childID);
   }
   else {
      return NULL;
   }
}

/* see node.h for specification */
Node_T Node_getParent(Node_T n) {
   assert(n != NULL);

   return n->parent;
}

/* see node.h for specification */
int Node_linkChild(Node_T parent, Node_T child) {
   size_t i;
   int result;
   char* rest;

   assert(parent != NULL);
   assert(child != NULL);
   assert(CheckerDT_Node_isValid(parent));
   assert(CheckerDT_Node_isValid(child));

   result = Node_hasChild(parent, child->path, NULL);
   if(result == 1) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return ALREADY_IN_TREE;
   }
   if(result == -1) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }

   i = strlen(parent->path);
   if(strncmp(child->path, parent->path, i)) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }
   rest = child->path + i;
   if(strlen(child->path) >= i && rest[0] != '/') {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }
   rest++;
   if(strstr(rest, "/") != NULL) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }
   child->parent = parent;

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 1) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return ALREADY_IN_TREE;
   }

   if(DynArray_addAt(parent->children, i, child) == TRUE) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return SUCCESS;
   }
   else {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }
}

/* see node.h for specification */
int  Node_unlinkChild(Node_T parent, Node_T child) {
   size_t i;

   assert(parent != NULL);
   assert(child != NULL);
   assert(CheckerDT_Node_isValid(parent));
   assert(CheckerDT_Node_isValid(child));

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 0) {
      assert(CheckerDT_Node_isValid(parent));
      assert(CheckerDT_Node_isValid(child));
      return PARENT_CHILD_ERROR;
   }

   (void) DynArray_removeAt(parent->children, i);

   assert(CheckerDT_Node_isValid(parent));
   assert(CheckerDT_Node_isValid(child));
   return SUCCESS;
}


/* see node.h for specification */
int Node_addChild(Node_T parent, const char* dir) {
   Node_T new;
   int result;

   assert(parent != NULL);
   assert(dir != NULL);
   assert(CheckerDT_Node_isValid(parent));

   new = Node_create(dir, parent);
   if(new == NULL) {
      assert(CheckerDT_Node_isValid(parent));
      return PARENT_CHILD_ERROR;
   }
   result = Node_linkChild(parent, new);
   if(result != SUCCESS)
      (void) Node_destroy(new);
   else
      assert(CheckerDT_Node_isValid(new));

   assert(CheckerDT_Node_isValid(parent));
   return result;
}


/* see node.h for specification */
char* Node_toString(Node_T n) {
   char* copyPath;

   assert(n != NULL);

   copyPath = malloc(strlen(n->path)+1);
   if(copyPath == NULL) {
      return NULL;
   }
   else {
      return strcpy(copyPath, n->path);
   }
}
