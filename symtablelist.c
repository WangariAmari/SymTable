/*--------------------------------------------------------------------*/
/* symtablelist.c                                                        */
/* Author: Wangari Ashley Karani                                      */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------*/

/* Each binding is stored in a SymTableNode. SymtableNodes are linked 
to form a list. */

struct SymTableNode
{
    /* The binding. */
    const char *pcKey;
    const void *pvValue; 

    /* The address of the next SymTableNode */
    struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTable Node*/

struct SymTable 
{
    /* The address of the first SymTableNode */
    struct SymTableNode *psFirstNode;
};

/*--------------------------------------------------------------------*/

/* Return a new SymTable object with no bindings, or NULL if 
insufficient memory is available. */

SymTable_T SymTable_new(void)
{
    SymTable_T oSymTable;

    oSymTable = (SymTable_T)calloc(1, sizeof(struct SymTable));

    if (oSymTable == NULL)
    {
        return NULL;
    }

    /* oSymTable->psFirstNode->pcKey = '\0';
    oSymTable->psFirstNode->pvValue = NULL;
    oSymTable->psFirstNode->psNextNode = NULL; */

    return oSymTable;
}

/*--------------------------------------------------------------------*/

/* Free all memory occupied by oSymTable */

void SymTable_free(SymTable_T oSymTable)
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        psNextNode = psCurrentNode->psNextNode;
        free(psCurrentNode);
    }

    free (oSymTable);
}

/*--------------------------------------------------------------------*/

/* Return the number of bindings in oSymTable */

size_t SymTable_getLength(SymTable_T oSymTable)
{
    size_t keyCount = 0;
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        keyCount++;
        psNextNode = psCurrentNode->psNextNode;
    }
    
    return keyCount;
}

/*--------------------------------------------------------------------*/

/* Add a new binding to oSymTable consisting of key pcKey and value 
pvValue and return 1 (TRUE) if oSymTable does not already contain a 
binding with key pcKey. Otherwise, if either oSymTable contains such a 
binding or insufficient memory is available, leave oSymTable unchanged 
and return 0 (FALSE). */

int SymTable_put(SymTable_T oSymTable, const char *pcKey, const void 
*pvValue) 
{
    struct SymTableNode *psNewNode;

    /* defensive copy */

    assert (oSymTable != NULL);

    psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));

    if (psNewNode == NULL) 
    {
        return 0;
    }

    psNewNode->pcKey = pcKey;
    psNewNode->pvValue = pvValue;
    psNewNode->psNextNode = oSymTable->psFirstNode;
    oSymTable->psFirstNode = psNewNode;
    return 1;
}

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, replace the binding's
value with pvValue and return the old value. Otherwise leave oSymTable 
unchanged and return NULL. */

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, const 
void *pvValue) 
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        if (strcmp(psCurrentNode->pcKey, pcKey) == 0) {
            void *oldval = (void *) psCurrentNode->pvValue;
            psCurrentNode->pvValue = pvValue;
            return oldval;
        }
        psNextNode = psCurrentNode->psNextNode;
    }

    return NULL;
}

/*--------------------------------------------------------------------*/

/* Return 1 (TRUE) if oSymTable contains a binding whose key is pcKey, 
or 0 (FALSE) if otherwise. */

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        if (psCurrentNode->pcKey == pcKey) {
            return 1;
        }
        psNextNode = psCurrentNode->psNextNode;
    }

    return 0;
}

/*--------------------------------------------------------------------*/

/* Return the value of the binding within oSymTable whose key is pcKey, 
or NULL if no such binding exists. */

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        if (psCurrentNode->pcKey == pcKey) {
            return psCurrentNode->pvValue;
        }
        psNextNode = psCurrentNode->psNextNode;
    }

    return NULL;
}

/*--------------------------------------------------------------------*/

/* If oSymTable contains a binding with key pcKey, remove that binding 
from oSymTable and return the binding's value. Otherwise leave oSymTable
 unchanged and return NULL. */

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey)
{
    /* keep track of a prev pter, initially null*/
    /* two cases, rming 1st node vs any other node */
    /* we don't just free node, also key */
    struct SymTableNode *psNextNode;

    assert(oSymTable != NULL);
    assert(oSymTable->psFirstNode != NULL);

    pcKey = oSymTable->psFirstNode->pcKey;
    psNextNode = oSymTable->psFirstNode->psNextNode;
    free(oSymTable->psFirstNode);
    oSymTable->psFirstNode = psNextNode;
    return (void*) pcKey;
    
}

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each element of oSymTable, passing pvExtra
as an extra argument. That is, for each element pcKey and its value 
pvValue of oSymTable, call (*pfApply) (pcKey, pvValue, pvExtra).  */

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char 
*pcKey, void *pvValue, void *pvExtra), const void *pvExtra)
{
    struct SymTableNode *psCurrentNode;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);

    for (psCurrentNode = oSymTable->psFirstNode; psCurrentNode != NULL; 
    psCurrentNode = psCurrentNode->psNextNode) 
    {
        (*pfApply) ((char *) psCurrentNode->pcKey, (void*) psCurrentNode->pvValue, (void*) pvExtra);
    }
}
