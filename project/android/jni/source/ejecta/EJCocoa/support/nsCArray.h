/****************************************************************************

****************************************************************************/

/**
 @file
 based on Chipmunk cpArray.
 nsArray is a faster alternative to NSMutableArray, it does pretty much the
 same thing (stores NSObjects and retains/releases them appropriately). It's
 faster because:
 - it uses a plain C interface so it doesn't incur Objective-c messaging overhead
 - it assumes you know what you're doing, so it doesn't spend time on safety checks
 (index out of bounds, required capacity etc.)
 - comparisons are done using pointer equality instead of isEqual

 There are 2 kind of functions:
 - nsArray functions that manipulates objective-c objects (retain and release are performed)
 - nsCArray functions that manipulates values like if they were standard C structures (no retain/release is performed)
 */

#ifndef NS_ARRAY_H
#define NS_ARRAY_H

#include "nsMacros.h"
#include "../NSObject.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

NS_NS_BEGIN

#define NS_INVALID_INDEX 0xffffffff

// Easy integration
#define NSARRAYDATA_FOREACH(__array__, __object__)															\
__object__=__array__->arr[0]; for(unsigned int i=0, num=__array__->num; i<num; i++, __object__=__array__->arr[i])	\


typedef struct _nsArray {
	unsigned int num, max;
	NSObject** arr;
} nsArray;

/** Allocates and initializes a new array with specified capacity */
nsArray* nsArrayNew(unsigned int capacity);

/** Frees array after removing all remaining objects. Silently ignores nil arr. */
void nsArrayFree(nsArray*& arr);

/** Doubles array capacity */
void nsArrayDoubleCapacity(nsArray *arr);

/** Increases array capacity such that max >= num + extra. */
void nsArrayEnsureExtraCapacity(nsArray *arr, unsigned int extra);

/** shrinks the array so the memory footprint corresponds with the number of items */
void nsArrayShrink(nsArray *arr);

/** Returns index of first onsurrence of object, NSNotFound if object not found. */
unsigned int nsArrayGetIndexOfObject(nsArray *arr, NSObject* object);

/** Returns a Boolean value that indicates whether object is present in array. */
bool nsArrayContainsObject(nsArray *arr, NSObject* object);

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void nsArrayAppendObject(nsArray *arr, NSObject* object);

/** Appends an object. Capacity of arr is increased if needed. */
void nsArrayAppendObjectWithResize(nsArray *arr, NSObject* object);

/** Appends objects from plusArr to arr. 
 Behavior undefined if arr doesn't have enough capacity. */
void nsArrayAppendArray(nsArray *arr, nsArray *plusArr);

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void nsArrayAppendArrayWithResize(nsArray *arr, nsArray *plusArr);

/** Inserts an object at index */
void nsArrayInsertObjectAtIndex(nsArray *arr, NSObject* object, unsigned int index);

/** Swaps two objects */
void nsArraySwapObjectsAtIndexes(nsArray *arr, unsigned int index1, unsigned int index2);

/** Removes all objects from arr */
void nsArrayRemoveAllObjects(nsArray *arr);

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void nsArrayRemoveObjectAtIndex(nsArray *arr, unsigned int index, bool bReleaseObj = true);

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void nsArrayFastRemoveObjectAtIndex(nsArray *arr, unsigned int index);

void nsArrayFastRemoveObject(nsArray *arr, NSObject* object);

/** Searches for the first onsurrence of object and removes it. If object is not
 found the function has no effect. */
void nsArrayRemoveObject(nsArray *arr, NSObject* object, bool bReleaseObj = true);

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void nsArrayRemoveArray(nsArray *arr, nsArray *minusArr);

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void nsArrayFullRemoveArray(nsArray *arr, nsArray *minusArr);

// #pragma mark -
// #pragma mark nsCArray for Values (c structures)

typedef struct _nsCArray {
    unsigned int num, max;
    void** arr;
} nsCArray;

/** Allocates and initializes a new C array with specified capacity */
nsCArray* nsCArrayNew(unsigned int capacity);

/** Frees C array after removing all remaining values. Silently ignores nil arr. */
void nsCArrayFree(nsCArray *arr);

/** Doubles C array capacity */
void nsCArrayDoubleCapacity(nsCArray *arr);

/** Increases array capacity such that max >= num + extra. */
void nsCArrayEnsureExtraCapacity(nsCArray *arr, unsigned int extra);

/** Returns index of first onsurrence of value, NSNotFound if value not found. */
unsigned int nsCArrayGetIndexOfValue(nsCArray *arr, void* value);

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool nsCArrayContainsValue(nsCArray *arr, void* value);

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void nsCArrayInsertValueAtIndex( nsCArray *arr, void* value, unsigned int index);

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void nsCArrayAppendValue(nsCArray *arr, void* value);

/** Appends an value. Capacity of arr is increased if needed. */
void nsCArrayAppendValueWithResize(nsCArray *arr, void* value);

/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void nsCArrayAppendArray(nsCArray *arr, nsCArray *plusArr);

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void nsCArrayAppendArrayWithResize(nsCArray *arr, nsCArray *plusArr);

/** Removes all values from arr */
void nsCArrayRemoveAllValues(nsCArray *arr);

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void nsCArrayRemoveValueAtIndex(nsCArray *arr, unsigned int index);

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void nsCArrayFastRemoveValueAtIndex(nsCArray *arr, unsigned int index);

/** Searches for the first onsurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void nsCArrayRemoveValue(nsCArray *arr, void* value);

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void nsCArrayRemoveArray(nsCArray *arr, nsCArray *minusArr);

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void nsCArrayFullRemoveArray(nsCArray *arr, nsCArray *minusArr);

NS_NS_END
	
#endif // NS_ARRAY_H
