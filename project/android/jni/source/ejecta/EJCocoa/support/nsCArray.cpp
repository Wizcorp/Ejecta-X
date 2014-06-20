/****************************************************************************

****************************************************************************/

#include "nsCArray.h"
#include "../NSObject.h"

NS_NS_BEGIN

/** Allocates and initializes a new array with specified capacity */
nsArray* nsArrayNew(unsigned int capacity) 
{
	if (capacity == 0)
		capacity = 1;
	
	nsArray *arr = (nsArray*)malloc( sizeof(nsArray) );
	arr->num = 0;
	arr->arr =  (NSObject**)calloc(capacity, sizeof(NSObject*));
	arr->max = capacity;
	
	return arr;
}

/** Frees array after removing all remaining objects. Silently ignores NULL arr. */
void nsArrayFree(nsArray*& arr)
{
    if( arr == NULL ) 
    {
        return;
    }
	nsArrayRemoveAllObjects(arr);
	
	free(arr->arr);
	free(arr);

    arr = NULL;
}

void nsArrayDoubleCapacity(nsArray *arr)
{
	arr->max *= 2;
	NSObject** newArr = (NSObject**)realloc( arr->arr, arr->max * sizeof(NSObject*) );
	// will fail when there's not enough memory
    NSAssert(newArr != 0, "nsArrayDoubleCapacity failed. Not enough memory");
	arr->arr = newArr;
}

void nsArrayEnsureExtraCapacity(nsArray *arr, unsigned int extra)
{
	while (arr->max < arr->num + extra)
    {
		nsArrayDoubleCapacity(arr);
    }
}

void nsArrayShrink(nsArray *arr)
{
    unsigned int newSize = 0;
	
	//only resize when necessary
	if (arr->max > arr->num && !(arr->num==0 && arr->max==1))
	{
		if (arr->num!=0)
		{
			newSize=arr->num;
			arr->max=arr->num;
		}
		else
		{//minimum capacity of 1, with 0 elements the array would be free'd by realloc
			newSize=1;
			arr->max=1;
		}
		
		arr->arr = (NSObject**)realloc(arr->arr,newSize * sizeof(NSObject*) );
		NSAssert(arr->arr!=NULL,"could not reallocate the memory");
	}
}

/** Returns index of first onsurrence of object, NS_INVALID_INDEX if object not found. */
unsigned int nsArrayGetIndexOfObject(nsArray *arr, NSObject* object)
{
	for(unsigned int i = 0; i < arr->num; i++)
    {
		if( arr->arr[i] == object ) return i;
    }

	return NS_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether object is present in array. */
bool nsArrayContainsObject(nsArray *arr, NSObject* object)
{
	return nsArrayGetIndexOfObject(arr, object) != NS_INVALID_INDEX;
}

/** Appends an object. Behavior undefined if array doesn't have enough capacity. */
void nsArrayAppendObject(nsArray *arr, NSObject* object)
{
    NSAssert(object != NULL, "Invalid parameter!");
    object->retain();
	arr->arr[arr->num] = object;
	arr->num++;
}

/** Appends an object. Capacity of arr is increased if needed. */
void nsArrayAppendObjectWithResize(nsArray *arr, NSObject* object)
{
	nsArrayEnsureExtraCapacity(arr, 1);
	nsArrayAppendObject(arr, object);
}

/** Appends objects from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void nsArrayAppendArray(nsArray *arr, nsArray *plusArr)
{
	for(unsigned int i = 0; i < plusArr->num; i++)
    {
		nsArrayAppendObject(arr, plusArr->arr[i]);
    }
}

/** Appends objects from plusArr to arr. Capacity of arr is increased if needed. */
void nsArrayAppendArrayWithResize(nsArray *arr, nsArray *plusArr)
{
	nsArrayEnsureExtraCapacity(arr, plusArr->num);
	nsArrayAppendArray(arr, plusArr);
}

/** Inserts an object at index */
void nsArrayInsertObjectAtIndex(nsArray *arr, NSObject* object, unsigned int index)
{
	NSAssert(index<=arr->num, "Invalid index. Out of bounds");
	NSAssert(object != NULL, "Invalid parameter!");

	nsArrayEnsureExtraCapacity(arr, 1);
	
	unsigned int remaining = arr->num - index;
	if( remaining > 0)
    {
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(NSObject*) * remaining );
    }

    object->retain();
	arr->arr[index] = object;
	arr->num++;
}

/** Swaps two objects */
void nsArraySwapObjectsAtIndexes(nsArray *arr, unsigned int index1, unsigned int index2)
{
	NSAssert(index1 < arr->num, "(1) Invalid index. Out of bounds");
	NSAssert(index2 < arr->num, "(2) Invalid index. Out of bounds");
	
	NSObject* object1 = arr->arr[index1];
	
	arr->arr[index1] = arr->arr[index2];
	arr->arr[index2] = object1;
}

/** Removes all objects from arr */
void nsArrayRemoveAllObjects(nsArray *arr)
{
	while( arr->num > 0 )
    {
		(arr->arr[--arr->num])->release();
    }
}

/** Removes object at specified index and pushes back all subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void nsArrayRemoveObjectAtIndex(nsArray *arr, unsigned int index, bool bReleaseObj/* = true*/)
{
    NSAssert(arr && arr->num > 0 && index < arr->num, "Invalid index. Out of bounds");
    if (bReleaseObj)
    {
        NS_SAFE_RELEASE(arr->arr[index]);
    }
    
	arr->num--;
	
	unsigned int remaining = arr->num - index;
	if(remaining>0)
    {
		memmove((void *)&arr->arr[index], (void *)&arr->arr[index+1], remaining * sizeof(NSObject*));
    }
}

/** Removes object at specified index and fills the gap with the last object,
 thereby avoiding the need to push back subsequent objects.
 Behavior undefined if index outside [0, num-1]. */
void nsArrayFastRemoveObjectAtIndex(nsArray *arr, unsigned int index)
{
	NS_SAFE_RELEASE(arr->arr[index]);
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

void nsArrayFastRemoveObject(nsArray *arr, NSObject* object)
{
	unsigned int index = nsArrayGetIndexOfObject(arr, object);
	if (index != NS_INVALID_INDEX)
    {
		nsArrayFastRemoveObjectAtIndex(arr, index);
    }
}

/** Searches for the first onsurrence of object and removes it. If object is not
 found the function has no effect. */
void nsArrayRemoveObject(nsArray *arr, NSObject* object, bool bReleaseObj/* = true*/)
{
	unsigned int index = nsArrayGetIndexOfObject(arr, object);
	if (index != NS_INVALID_INDEX)
    {
		nsArrayRemoveObjectAtIndex(arr, index, bReleaseObj);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, the
 first matching instance in arr will be removed. */
void nsArrayRemoveArray(nsArray *arr, nsArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		nsArrayRemoveObject(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all objects in minusArr. For each object in minusArr, all
 matching instances in arr will be removed. */
void nsArrayFullRemoveArray(nsArray *arr, nsArray *minusArr)
{
	unsigned int back = 0;
	unsigned int i = 0;
	
	for( i = 0; i < arr->num; i++) 
    {
		if( nsArrayContainsObject(minusArr, arr->arr[i]) ) 
        {
			NS_SAFE_RELEASE(arr->arr[i]);
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

// #pragma mark -
// #pragma mark nsCArray for Values (c structures)

/** Allocates and initializes a new C array with specified capacity */
nsCArray* nsCArrayNew(unsigned int capacity)
{
	if (capacity == 0)
    {
		capacity = 1;
    }

	nsCArray *arr = (nsCArray*)malloc( sizeof(nsCArray) );
	arr->num = 0;
	arr->arr = (void**)malloc( capacity * sizeof(void*) );
	arr->max = capacity;
	
	return arr;
}

/** Frees C array after removing all remaining values. Silently ignores NULL arr. */
void nsCArrayFree(nsCArray *arr)
{
    if( arr == NULL ) 
    {
        return;
    }
	nsCArrayRemoveAllValues(arr);
	
	free(arr->arr);
	free(arr);
}

/** Doubles C array capacity */
void nsCArrayDoubleCapacity(nsCArray *arr)
{
    nsArrayDoubleCapacity((nsArray*)arr);
}

/** Increases array capacity such that max >= num + extra. */
void nsCArrayEnsureExtraCapacity(nsCArray *arr, unsigned int extra)
{
    nsArrayEnsureExtraCapacity((nsArray*)arr,extra);
}

/** Returns index of first onsurrence of value, NS_INVALID_INDEX if value not found. */
unsigned int nsCArrayGetIndexOfValue(nsCArray *arr, void* value)
{
	unsigned int i;
	
	for( i = 0; i < arr->num; i++)
    {
		if( arr->arr[i] == value ) return i;
    }
	return NS_INVALID_INDEX;
}

/** Returns a Boolean value that indicates whether value is present in the C array. */
bool nsCArrayContainsValue(nsCArray *arr, void* value)
{
	return nsCArrayGetIndexOfValue(arr, value) != NS_INVALID_INDEX;
}

/** Inserts a value at a certain position. Behavior undefined if array doesn't have enough capacity */
void nsCArrayInsertValueAtIndex( nsCArray *arr, void* value, unsigned int index)
{
	NSAssert( index < arr->max, "nsCArrayInsertValueAtIndex: invalid index");
	
	unsigned int remaining = arr->num - index;
    // make sure it has enough capacity
    if (arr->num + 1 == arr->max)
    {
        nsCArrayDoubleCapacity(arr);
    }
	// last Value doesn't need to be moved
	if( remaining > 0) {
		// tex coordinates
		memmove((void *)&arr->arr[index+1], (void *)&arr->arr[index], sizeof(void*) * remaining );
	}
	
	arr->num++;
	arr->arr[index] = value;
}

/** Appends an value. Behavior undefined if array doesn't have enough capacity. */
void nsCArrayAppendValue(nsCArray *arr, void* value)
{
	arr->arr[arr->num] = value;
	arr->num++;
    // double the capacity for the next append action
    // if the num >= max
    if (arr->num >= arr->max)
    {
        nsCArrayDoubleCapacity(arr);
    }
}

/** Appends an value. Capacity of arr is increased if needed. */
void nsCArrayAppendValueWithResize(nsCArray *arr, void* value)
{
	nsCArrayEnsureExtraCapacity(arr, 1);
	nsCArrayAppendValue(arr, value);
}


/** Appends values from plusArr to arr. Behavior undefined if arr doesn't have
 enough capacity. */
void nsCArrayAppendArray(nsCArray *arr, nsCArray *plusArr)
{
	unsigned int i;
	
	for( i = 0; i < plusArr->num; i++)
    {
		nsCArrayAppendValue(arr, plusArr->arr[i]);
    }
}

/** Appends values from plusArr to arr. Capacity of arr is increased if needed. */
void nsCArrayAppendArrayWithResize(nsCArray *arr, nsCArray *plusArr)
{
	nsCArrayEnsureExtraCapacity(arr, plusArr->num);
	nsCArrayAppendArray(arr, plusArr);
}

/** Removes all values from arr */
void nsCArrayRemoveAllValues(nsCArray *arr)
{
	arr->num = 0;
}

/** Removes value at specified index and pushes back all subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void nsCArrayRemoveValueAtIndex(nsCArray *arr, unsigned int index)
{
	unsigned int last;
	
	for( last = --arr->num; index < last; index++)
    {
		arr->arr[index] = arr->arr[index + 1];
    }
}

/** Removes value at specified index and fills the gap with the last value,
 thereby avoiding the need to push back subsequent values.
 Behavior undefined if index outside [0, num-1].
 @since v0.99.4
 */
void nsCArrayFastRemoveValueAtIndex(nsCArray *arr, unsigned int index)
{
	unsigned int last = --arr->num;
	arr->arr[index] = arr->arr[last];
}

/** Searches for the first onsurrence of value and removes it. If value is not found the function has no effect.
 @since v0.99.4
 */
void nsCArrayRemoveValue(nsCArray *arr, void* value)
{
	unsigned int index = nsCArrayGetIndexOfValue(arr, value);
	if (index != NS_INVALID_INDEX)
    {
		nsCArrayRemoveValueAtIndex(arr, index);
    }
}

/** Removes from arr all values in minusArr. For each Value in minusArr, the first matching instance in arr will be removed.
 @since v0.99.4
 */
void nsCArrayRemoveArray(nsCArray *arr, nsCArray *minusArr)
{
	for(unsigned int i = 0; i < minusArr->num; i++)
    {
		nsCArrayRemoveValue(arr, minusArr->arr[i]);
    }
}

/** Removes from arr all values in minusArr. For each value in minusArr, all matching instances in arr will be removed.
 @since v0.99.4
 */
void nsCArrayFullRemoveArray(nsCArray *arr, nsCArray *minusArr)
{
	unsigned int back = 0;
	
	for(unsigned int i = 0; i < arr->num; i++) 
    {
		if( nsCArrayContainsValue(minusArr, arr->arr[i]) ) 
        {
			back++;
		} 
        else
        {
			arr->arr[i - back] = arr->arr[i];
        }
	}
	
	arr->num -= back;
}

NS_NS_END
