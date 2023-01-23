# fast-list
Naive linked lists are infamous for being extremely slow for a variety of reasons, including but not limited to:
1. Requestiong allocations for every single node.
2. Scattering nodes/allocations all around the heap, resulting in poor cache locality and potential fragmentation.
3. Requiring the overhead of 2 pointers(in a doubly linked list) along with your data.
##
This list implementation solves those issues through the following:
1. Allocating a large, contiguous, intrusive buffer to store all of the nodes
2. Determining next/previous nodes via unsigned ints that store indexes into the buffer rather than pointers.
##
The intrusive buffer results in all of the nodes being placed close together in memory, dramatically improving the cache locality of the structure. Since this removes the need to allocate all over the heap, it also eliminates additional, unintended memory overhead caused by the potential fragmentation.

The buffer is able to keep track of free slots for future nodes to fit into with almost zero additional memory overhead by maintaining an intrusive free list within the buffer itself. The only additional memory overhead this approach requires is a single unsigned int which points to the head of the free list.

Another cheap but valuable optimization that was able to be made as a result of using the buffer was the ability to track nodes by an index rather than a pointer, resulting in each node only requiring 2 unsigned ints instead of 2 full pointers. This does result in the list being limited to a maximum of 4,294,967,296 individual nodes... But such a restriction is reasonable for just about any possible usecase.

#
Some rough performance benchmarks indicate that my assumptions about how such a structure would improve upon conventional linked lists were true:

1. inserting 100,000 elements into the beginning, removing 40,000 elements from the beginning, then inserting 100,000 elements into the beginning once again:
```
fast-list:   0.650076ms
std::list:   2.20763ms
std::vector: 643.546ms
```

2. inserting 100,000 elements into the end, removing 40,000 elements from the end, then inserting 100,000 elements into the end once again:
```
fast-list:   0.704752ms
std::list:   2.24493ms
std::vector: 0.0741679ms
```

3. iteration over 100,000 uint32_ts:
```
fast-list:   0.369868ms
std::list:   1.32952ms
std::vector: 0.0130653ms
```

4. iteration over 100,000 uint32_t[5000]s:
```
fast-list:   9.28189ms
std::list:   3001.55ms
std::vector: 1.85372ms
```

std::list's especially poor performance for the last benchmark was rather peculiar. A deeper investigation into the results led to the discovery that its scattered allocations resulted in massive amounts of fragmentation occuring for such a large structure, leading it to eventually use swap to store the rest of its data and thrash the disk while iterating. Adding an extra 8GB of memory to my system solved this issue and allowed for a more reasonable measurement:
```
std::list: 26.1818ms
```

#

While this list implementation is signficantly faster than the standard std::list, there are some drawbacks and potential improvements that can still be made:
1. This implementation doesn't support resizing the buffer, however this should be as trivial as using mmap to request a large amount of virtual memory and reallocing once the capacity is filled. Such an approach will allow the structure to avoid having to allocate a new buffer and copy its contents over in the majority of cases.
2. Right now, the implementation is designed around storing ints. Creating templates for this should be fairly trivial.
3. Accessing nodes within the list requires ugly syntax. A templated iterator that implements the `next()` and `prev()` functions would solve this, but it may introduce overhead that the compiler can't optimize away.
