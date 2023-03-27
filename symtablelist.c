/*--------------------------------------------------------------------*/
/* symtablelist.c                                                        */
/* Author: Wangari Ashley Karani                                      */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <assert.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

/* Comment */

struct SymTableNode {
    /* The item. */
    const void *pcKey;
    
    /* Q: Do I need one for the value as well? */
    const void *pvValue;

    /* The address of the next SymTableNode */
    struct SymTableNode *psNextNode;
}

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTable Node */

struct SymTable 
{
    /* The address of the first SymTableNode */
    struct SymTableNode *psFirstNode;
}

/*--------------------------------------------------------------------*/

/* Return a new SymTable object with no bindings, or NULL if 
insufficient memory is available. */

SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/

/* Free all memory occupied by oSymTable */

void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Return the number of bindings in oSymTable */

size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/

/* Add a new binding to oSymTable consisting of key pcKey and value 
pvValue and return 1 (TRUE) if oSymTable does not already contain a 
binding with key pcKey. Otherwise, if either oSymTable contains such a 
binding or insufficient memory is available, leave oSymTable unchanged 
and return 0 (FALSE). */

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, replace the binding's
 value with pvValue and return the old value. Otherwise leave oSymTable 
 unchanged and return NULL. */

void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/

/* Return 1 (TRUE) if oSymTable contains a binding whose key is pcKey, 
or 0 (FALSE) if otherwise. */

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Return the value of the binding within oSymTable whose key is pcKey, 
or NULL if no such binding exists. */

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, remove that binding 
from oSymTable and return the binding's value. Otherwise leave oSymTable
 unchanged and return NULL. */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each element of oSymTable, passing pvExtra
as an extra argument. That is, for each element pcKey and its value 
pvValue of oSymTable, call (*pfApply) (pcKey, pvValue, pvExtra).  */

void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);


