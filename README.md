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

Suppose we have two 32 bit integers x, y labeled as: $x_{31},x_{30},...x_0$, $y_{31},y_{30},...y_0$, $x \neq y$. In order for $h(x) = h(y)$, the last S bits must be equal to each other (as a result of the final modulo function), where 2^S is the size of the map. Precisely, $x_S...x_0 = y_S...y_0$. The probability of this happening is $\frac{{2^{32-S} \choose 2}}{{2^{32} \choose 2}}$. This can be simplified to $\frac{(\frac{2^{32-S}-1}{2^{32}-1})}{2^S} \leq \frac{1}{2^S}$ which satisfies the requirement of a universal hash function. Therefore, if we can guarantee that for any set of binary numbers $U, U \subseteq$ {$0,1$}$^{\infty}$, we can pick two numbers, and then transform them completely randomly, our hash function will be universal. This hash function does this by generating a random hash table of 4 constants per hash map (exact implementation can be seen in the code), and then multiplies each section of 8 bits in the two input numbers by those constants. Finally, each section of 8 bits is XORd back together to generate a new, completely random 32 bit integer. This is then put into the hashing function to generate the index.

This hashmap handles collisions using a linked list per collision index (exact implementation shown in the code). There are multiple optimizations that are used in the code of this hashmap, and ones that could still be implemented which I will list below:

1. Modulo optimization: To fit my hash function to the size of the hash map, I used a modulo operator on the size, such that any key would always be hashed to a valid index. I am not sure if there are any other ways to do this, but this is the one that I came up with. The optimization comes by normalizing all hashmaps to have size thats a power of 2. Then, computing modulo is the same as just taking the last |size| bits of the returned value from the hash function, which can be computed with a right shift and a bitwise AND. This is much faster than the normal modulo, and will save a lot of time with large maps.

2. Collision optimization: This has NOT been implemented yet, but instead of using a linked list for the collisions, you can use a binary search tree to improve collision look up from $O(n)$ to $O(\log(n))$. This optimization frankly saves very little time because at most you are getting around 6 collisions per index even with massive maps, so the linear lookup time is very little. (I found the max collisions per index through numerical testing, but the expected amount of collisions per index is $\frac{n}{S^2}$ where n is the amount of elements hashed).

3. Random Constant instead of Random Table: I've included this line commented out, if you want to save time from computing the hashtable, you can just compute one random constant and multiply your keys by that instead of using the XOR method. This will save some time, but will increase the probability of collisions.


