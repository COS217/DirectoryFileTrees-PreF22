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

/*
   A node structure represents a directory in the directory tree
*/
struct node {
   /* the full path of this directory */
   char* path;

   /* the parent directory of this directory
      NULL for the root of the directory tree */
   Node parent;

   /* the subdirectories of this directory
      stored in sorted order by pathname */
   DynArray_T children;
};


/*
  returns a path with contents
  n->path/dir
  or NULL if there is an allocation error.

  Allocates memory for the returned string,
  which is then owened by the caller!
*/
static char* Node_buildPath(Node n, const char* dir) {
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
Node Node_create(const char* dir, Node parent){

   Node new;

   assert(dir != NULL);

   new = malloc(sizeof(struct node));
   if(new == NULL)
      return NULL;

   new->path = Node_buildPath(parent, dir);

   if(new->path == NULL) {
      free(new);
      return NULL;
   }

   new->parent = parent;
   new->children = DynArray_new(0);
   if(new->children == NULL) {
      free(new->path);
      free(new);
      return NULL;
   }

   return new;
}

/* see node.h for specification */
size_t Node_destroy(Node n) {
   size_t i;
   size_t count = 0;
   Node c;

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
/*char* Node_getPath(Node n) {
   char* pathCopy;

   assert(n != NULL);

   pathCopy = malloc(strlen(n->path)+1);
   if(pathCopy == NULL)
      return NULL;

   return strcpy(pathCopy, n->path);
}
*/
const char* Node_getPath(Node n) {

   assert(n != NULL);
   return n->path;
}

/* see node.h for specification */
int Node_compare(Node node1, Node node2) {
   assert(node1 != NULL);
   assert(node2 != NULL);

   return strcmp(node1->path, node2->path);
}



/* see node.h for specification */
size_t Node_getNumChildren(Node n) {
   assert(n != NULL);

   return DynArray_getLength(n->children);
}

/* see node.h for specification */
int Node_hasChild(Node n, const char* path, size_t* childID) {
   size_t index;
   int result;
   Node checker;

   assert(n != NULL);
   assert(path != NULL);

   checker = Node_create(path, NULL);
   if(checker == NULL)
      return -1;
   result = DynArray_bsearch(n->children, checker, &index,
                    (int (*)(const void*, const void*)) Node_compare);
   (void) Node_destroy(checker);

   if(childID != NULL)
      *childID = index;
   return result;
}

/* see node.h for specification */
Node Node_getChild(Node n, size_t childID) {
   assert(n != NULL);

   if(DynArray_getLength(n->children) > childID)
      return DynArray_get(n->children, childID);
   else
      return NULL;
}

/* see node.h for specification */
Node Node_getParent(Node n) {
   assert(n != NULL);

   return n->parent;
}

/* see node.h for specification */
int Node_linkChild(Node parent, Node child) {
   size_t i;
   char* rest;

   assert(parent != NULL);
   assert(child != NULL);

   if(Node_hasChild(parent, child->path, NULL))
      return ALREADY_IN_TREE;
   i = strlen(parent->path);
   if(strncmp(child->path, parent->path, i))
      return PARENT_CHILD_ERROR;
   rest = child->path + i;
   if(strlen(child->path) >= i && rest[0] != '/')
      return PARENT_CHILD_ERROR;
   rest++;
   if(strstr(rest, "/") != NULL)
      return PARENT_CHILD_ERROR;

   child->parent = parent;

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 1)
      return ALREADY_IN_TREE;

   if(DynArray_addAt(parent->children, i, child) == TRUE)
      return SUCCESS;
   else
      return PARENT_CHILD_ERROR;
}

/* see node.h for specification */
int  Node_unlinkChild(Node parent, Node child) {
   size_t i;

   assert(parent != NULL);
   assert(child != NULL);

   if(DynArray_bsearch(parent->children, child, &i,
         (int (*)(const void*, const void*)) Node_compare) == 0)
      return PARENT_CHILD_ERROR;

   (void) DynArray_removeAt(parent->children, i);
   return SUCCESS;
}


/* see node.h for specification */
int Node_addChild(Node parent, const char* dir) {
   Node new;
   int result;

   assert(parent != NULL);
   assert(dir != NULL);

   new = Node_create(dir, parent);
   if(new == NULL)
      return PARENT_CHILD_ERROR;

   result = Node_linkChild(parent, new);
   if(result != SUCCESS)
      (void) Node_destroy(new);

   return result;
}


/* see node.h for specification */
char* Node_toString(Node n) {
   char* copyPath;

   assert(n != NULL);

   copyPath = malloc(strlen(n->path)+1);
   if(copyPath == NULL)
      return NULL;
   else
      return strcpy(copyPath, n->path);
}
