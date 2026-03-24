## GitHub Link
[GITHUB!](https://github.com/Steven-Sunny/OS_Tutorial_8)

## How to Run:

1. Compile sigtrap.c using: 
```bash
gcc sigtrap.c \-o process 
``` 
2. Compile question1.c using: 
```bash
gcc question1.c \-o question1
```  
3. Compile question2.c using:
```bash
gcc question2.c \-o question2
```

Or try running the makefile using the command:
```bash 
make
```

## Conceptual Questions:

1. An Abstract Data Type (ADT) is a conceptual model that describes a data structure by what it does or should do, and not by defining the functions that make up the data structure. An abstract data type can be anything from a Stack to a FIFO queue with operations like push or enqueue. Typically, C ADTs are given by the header files which act as an interface for providing the functions in the defined data type. An ADT would then be logically implemented when it gets defined in an actual C file where it would become a fully-fledged data type/structure.

2. A Queue(FIFO) is a list-like data structure where the first node added is the first to be removed from the queue. Nodes added to a queue are typically added to the back and move to the front as other nodes are removed. In a stack(LIFO), the most recent node added is removed first. Nodes added to a stack are typically the new head and thus, the first to be removed.

3. 
    1\. Arrays: A basic array is a collection of contiguous memory locations of the same data type. Each location of data is located by an index allowing for quick random access of all elements in the array. An array is a versatile data structure and can be used for organizing datasets or even implementing mathematical structures like matrices. 

   2\. Stacks: These data structures follow the Last-In-First-Out (LIFO) principle with operations that add (push) and remove (pop) items only on one end of the stack. The latest item that is added to the stack using push will be the first item that will be removed with pop. Stacks are commonly used for short-term memory storage like undo/redo.

   3\. Linked Lists: A linear collection of data nodes. Each node is composed of 2 parts, a data portion and a pointer node to the next node in the list. Linked lists are also very easy to extend or shrink, requiring O(1) to complete. A linked list does not have to be stored in contiguous memory, this makes it fantastic for storing things in disconnected chunks of memory. 

4. Binary trees are a tree data structure where each node has no more than 2 child nodes. Common binary tree operations are searches for a given node as well as popping (removing) and appending a node. Both of these example operations may require additional work to keep the binary tree property.

5. Hash tables are array data structures which use a hashing function to access elements resulting in a high chance for O(1) time complexity. Indexes of a hashed element aren’t simply appended to the end of the array, but hashed using the hash function. Hash table operations include accessing and inserting data as well as deleting a given element.
