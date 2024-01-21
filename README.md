# CStructures
Common Data Structures implemented in C

**WHY?**
I was told that C is not a great language for coding algorithm-heavy problems, as it didn't have any of the pre-implemented data structures that languages like C++/Python/Java do. However, I felt that this was a pretty poor excuse to not use my favorite language. So, in order to remedy this problem, I decided to just put those data structures in C myself!

Notes about the ones that I included here:

**Queue/Stack**
Supports treating the structure as either a queue or a stack (based on how you add the elements to it). I included the standard methods that one might use this for, but of course more could be added. QueueNodes are left open in implementation, so that you could repurpose them for any kind of problem. For example, I have used each queue node in graph traversal problems to include a distance from the root and a value of a vertex in the graph. I have also used each node to include a start and an end pointer for different subarrays in an array. Finally, I have included a free method to automatically free the whole queue, to avoid any memory leaks.

**Dynamic Array (ArrayList)**
Size normalized to be a power of 2 at all times. This allows O(1) amortized resizing with the following method: 

Let the array originally have size n. After adding the n+1th element, 2n elements of memory are allocated and the first n elements are copied (using the in-built memcpy). Then the n+1th element is inserted. The next n-1 elements can be added for free, as there is extra space in the array. The resulting complexities are shown below:
Copying: $O(\frac{n}{n+1}) = O(1)$ (amortized)

Allocating: $O(\frac{2n}{n+1}) = O(1)$ (amortized)

The ArrayList has random access to elements, and includes basic getter and setter methods. I have not included support for adding an element at a specific index or adding an array of elements at a specific index, however both of these can be easily implemented as needed. In the future, I will also implement a sorting method.

**Hash Map**

Probably the most useful and the most interesting data structure implemented here. My hashmap utilizes a universal hashing function to hash keys. I will show a proof sketch here:

Suppose we have two 32 bit integers x, y labeled as: x31 -> x0, y31 -> y0, x != y. In order for h(x) = h(y), the last S bits must be equal to each other (as a result of the final modulo function), where 2^S is the size of the map. The probability of this happening is $\frac{{2^{32-S} \choose 2}}{{2^32 \choose 2}}$
