/*--------------------------------------------------------------------*/
/* symtablehash.c                                                        */
/* Author: Wangari Ashley Karani                                      */
/*--------------------------------------------------------------------*/

#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

/*--------------------------------------------------------------------*/

/* Declaration for a global variable that stores the bucket counts */

const size_t uBucketCounts[] = {509, 1021, 2039, 4093, 8191, 16381, 
32749, 65521};

/*--------------------------------------------------------------------*/

/* Each binding is stored in a SymTableNode. SymtableNodes are linked 
to form a list. */

struct SymTableNode
{
    /* The binding's key. */
    const char *pcKey;
    
    /* The value associated with the binding's key. */
    const void *pvValue; 

    /* The address of the next SymTableNode */
    struct SymTableNode *psNextNode;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" node that points to the first SymTable Node*/

struct SymTable 
{
    /* The address of the first SymTableNode */
    struct SymTableNode **psFirstNode;

    /* The bucket number . */
    size_t uBucketCount;

    /* number of nodes in symtable */
    size_t length;
};

/*--------------------------------------------------------------------*/

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
   {
        uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];
   }

   return uHash % uBucketCount;
}

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

    oSymTable->psFirstNode = calloc(uBucketCounts[0], sizeof(struct SymTableNode*));
    oSymTable->uBucketCount = uBucketCounts[0];

    return oSymTable;
}

/*--------------------------------------------------------------------*/

/* Free all memory occupied by oSymTable */

void SymTable_free(SymTable_T oSymTable)
{
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psNextNode;
    size_t i;
    assert(oSymTable != NULL);

    for (i = 0; i < oSymTable->uBucketCount; i++) 
    {
        for (psCurrentNode = oSymTable->psFirstNode[i]; psCurrentNode != NULL; 
        psCurrentNode = psNextNode) 
        {
            psNextNode = psCurrentNode->psNextNode;
            free((char *) psCurrentNode->pcKey);
            free(psCurrentNode);
        }
    }

    free (oSymTable);
}

/*--------------------------------------------------------------------*/

/* Return the number of bindings in oSymTable */

size_t SymTable_getLength(SymTable_T oSymTable)
{
    assert(oSymTable != NULL);

    return oSymTable->length;
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
    size_t hashcode;

    assert (oSymTable != NULL);
    assert (pcKey != NULL);

    hashcode = SymTable_hash(pcKey, oSymTable->uBucketCount);

    if (SymTable_contains(oSymTable, pcKey)) {
        return 0;
    }

    /* defensive copy */
    psNewNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));

    if (psNewNode == NULL) 
    {
        return 0;
    }

    psNewNode->pcKey = (char*)malloc(strlen(pcKey) + 1);

    if (psNewNode->pcKey == NULL) 
    {
        free (psNewNode);
        return 0;
    }

    strcpy((char *)psNewNode->pcKey, pcKey);
    psNewNode->pvValue = pvValue;
    psNewNode->psNextNode = oSymTable->psFirstNode[hashcode];
    oSymTable->psFirstNode[hashcode] = psNewNode;
    oSymTable->length++;
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
    size_t hashcode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashcode = SymTable_hash(pcKey, oSymTable->uBucketCount);

    for (psCurrentNode = oSymTable->psFirstNode[hashcode]; psCurrentNode != NULL; 
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
    size_t hashcode;

    assert(oSymTable != NULL);
    assert (pcKey != NULL);

    hashcode = SymTable_hash(pcKey, oSymTable->uBucketCount);

    for (psCurrentNode = oSymTable->psFirstNode[hashcode]; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        if (strcmp(psCurrentNode->pcKey, pcKey) == 0) {
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
    size_t hashcode;

    assert(oSymTable != NULL);
    assert (pcKey != NULL);

    hashcode = SymTable_hash(pcKey, oSymTable->uBucketCount);

    for (psCurrentNode = oSymTable->psFirstNode[hashcode]; psCurrentNode != NULL; 
    psCurrentNode = psNextNode) 
    {
        if (strcmp(psCurrentNode->pcKey, pcKey) == 0) {
            return (void *) psCurrentNode->pvValue;
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
    struct SymTableNode *psCurrentNode;
    struct SymTableNode *psPrevNode = NULL;
    size_t hashcode;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashcode = SymTable_hash(pcKey, oSymTable->uBucketCount);

    for (psCurrentNode = oSymTable->psFirstNode[hashcode]; psCurrentNode != NULL; 
    psCurrentNode = psCurrentNode->psNextNode) 
    {
        if (strcmp(psCurrentNode->pcKey, pcKey) == 0) {
            void *oldval = (void *) psCurrentNode->pvValue;
            /* relink to remove current node */
            if (psPrevNode == NULL) {
                oSymTable->psFirstNode[hashcode] = psCurrentNode->psNextNode;
            }
            else {
                psPrevNode->psNextNode = psCurrentNode->psNextNode;
            }
            free ((char *) psCurrentNode->pcKey);
            free (psCurrentNode);
            oSymTable->length--;
            return oldval;
        }
        psPrevNode = psCurrentNode;
    }
    
    return NULL;
}

/*--------------------------------------------------------------------*/

/* Apply function *pfApply to each element of oSymTable, passing pvExtra
as an extra argument. That is, for each element pcKey and its value 
pvValue of oSymTable, call (*pfApply) (pcKey, pvValue, pvExtra).  */

void SymTable_map(SymTable_T oSymTable, void (*pfApply)(const char 
*pcKey, void *pvValue, void *pvExtra), const void *pvExtra)
{
    struct SymTableNode *psCurrentNode;
    size_t i;
    assert(oSymTable != NULL);
    assert(pfApply != NULL);    

    for (i = 0; i < oSymTable->uBucketCount; i++) 
    {
        for (psCurrentNode = oSymTable->psFirstNode[i]; psCurrentNode != NULL; 
        psCurrentNode = psCurrentNode->psNextNode) 
        {
            (*pfApply) ((char *) psCurrentNode->pcKey, (void*) psCurrentNode->pvValue, (void*) pvExtra);
        }
    }   
}
