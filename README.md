# Size Balanced Tree

​	A Size-Balanced Tree (SBT) is a type of self-balancing binary search tree that maintains a balance between the size of its left and right subtrees. The size of a subtree is the total number of nodes in that subtree, including the root node. SBT supports the standard binary search tree operations such as insertion, deletion, searching, selection, and ranking in O(log n) time.

## Applicability

  A Size-Balanced Tree only depends on the node size to maintain balance, so it can provide selection and ranking operations without introducing additional information. Although SBT can provide all the functions of a standard binary search tree, it still cannot replace Red-Black Tree (RBT) from a performance perspective. However, SBT is particularly suitable for high-performance selection and sorting operations, such as accessing elements by index, median filtering, and so on.

## Example

```C++
#include <iostream>
#include "sb_tree.h"

int main(void)
{
	// Creates a sb-tree containing integers.
	sb_tree<int> sbt({ 10, 40, 20 });

	// Prints all elements of a sb-tree.
	std::cout << "sb-tree = { ";
	for (auto itr = sbt.cbegin(); itr != sbt.cend(); ++itr)
		std::cout << *itr << ", ";
	std::cout << "}; \n";

	// Inserts two elements
	sbt.insert_equal({ 50, 30 });

	// Prints all elements of a sb-tree.
	std::cout << "sb-tree = { ";
	for (const auto& ele : sbt)
		std::cout << ele << ", ";
	std::cout << "}; \n";

	// Subscript operation.
	std::cout << "sbt[2] = " << sbt[2] << ";\n";

	// Rank operation.
	std::cout << "rank(30) = " << sbt.rank(30) << ";\n";

    return 0;
}
```

​	Output:

```C++
sb-tree = { 10, 20, 40, };
sb-tree = { 10, 20, 30, 40, 50, };
sbt[2] = 30;
rank(30) = 2;
```

## Interface

​	This is a header-only library designed for modern C++. Its interface is similar to that of the container in the C++ standard library. Therefore, it can be used easily with almost no learning cost.

### sb_tree

Defined in header <sb_tree.h>.

```C++
template <class T, class Compare = std::less<T>, class Allocator = std::allocator<T>>
class sb_tree;
```

#### Member types

| member type                      | definition                                                   | notes                                    |
| -------------------------------- | ------------------------------------------------------------ | ---------------------------------------- |
| value_type                       | The  first template parameter (T)                            |                                          |
| allocator_type                   | The second template parameter (Allocator)                    |                                          |
| reference                        | value_type&                                                  |                                          |
| const_reference                  | const value_type&                                            |                                          |
| pointer                          | value_type*                                                  |                                          |
| const_pointer                    | const value_type*                                            |                                          |
| size_type                        | an unsigned integral type that can represent any non-negative value of difference_type | usually the same as size_t               |
| difference_type                  | a signed integral type                                       | usually the same as ptrdiff_t            |
| iterator                         | a bidirectional iterator to value_type                       | convertible to: const_iterator           |
| const_iterator                   | a bidirectional iterator to const value_type                 |                                          |
| reverse_iterator                 | a bidirectional reverse iterator to value_type               |                                          |
| const_reverse_iterator           | a bidirectional reverse iterator to const value_type         |                                          |
| primitive_iterator               | a bidirectional primitive iterator to value_type             | convertible to: const_primitive_iterator |
| const_primitive_iterator         | a bidirectional primitive iterator to const value_type       |                                          |
| reverse_primitive_iterator       | a bidirectional reverse primitive iterator to value_type     |                                          |
| const_reverse_primitive_iterator | a bidirectional reverse primitive iterator to const value_type |                                          |

#### Member functions

| function      | description                                                  |
| ------------- | ------------------------------------------------------------ |
| (constructor) | construct the sb-tree<br />*(public member function)*        |
| (destructor)  | destruct the sb-tree<br />*(public member function)*         |
| operator=     | assign values to the container<br />*(public member function)* |
| assign_equal  | assign values to the container<br />*(public member function)* |
| assign_unique | assign values to the container and remove duplicate values<br />*(public member function)* |

##### Element access

| function   | description                                                  |
| ---------- | ------------------------------------------------------------ |
| operator[] | access specified element<br />*(public member function)*     |
| at         | access specified element with bounds checking<br />*(public member function)* |

##### Iterators

| iterator              | description                                                  |
| --------------------- | ------------------------------------------------------------ |
| begin<br />cbegin     | return an iterator to the beginning<br />*(public member function)* |
| end<br />cend         | return an iterator to the end<br />*(public member function)* |
| rbegin<br />crbegin   | return a reverse iterator to the beginning<br />*(public member function)* |
| rend<br />crend       | return a reverse iterator to the end<br />*(public member function)* |
| pbegin<br />cpbegin   | return a primitive iterator to the beginning<br />*(public member function)* |
| pend<br />cpend       | return a primitive iterator to the end<br />*(public member function)* |
| rpbegin<br />crpbegin | return a reverse primitive iterator to the beginning<br />*(public member function)* |
| rpend<br />crpend     | return a reverse primitive iterator to the end<br />*(public member function)* |

##### Capacity

| function | description                                                  |
| -------- | ------------------------------------------------------------ |
| empty    | check whether container is empty<br />*(public member function)* |
| size     | return the number of elements<br />*(public member function)* |
| max_size | return the maximum possible number of elements<br />*(public member function)* |

##### Modifiers

| function       | description                                                  |
| -------------- | ------------------------------------------------------------ |
| emplace_equal  | construct and insert element <br />*(public member function)* |
| emplace_unique | construct and insert element if it is unique<br />*(public member function)* |
| insert_equal   | insert elements<br />*(public member function)*              |
| insert_unique  | insert elements  and remove duplicate values<br />*(public member function)* |
| erase          | erase elements<br />*(public member function)*               |
| swap           | swap the content<br />*(public member function)*             |
| clear          | clear the content<br />*(public member function)*            |

##### Operations

| function    | description                                                  |
| ----------- | ------------------------------------------------------------ |
| find        | get iterator to element<br />*(public member function)*      |
| lower_bound | return iterator to lower bound<br />*(public member function)* |
| upper_bound | return iterator to upper bound<br />*(public member function)* |
| select      | return iterator to specified location<br />*(public member function)* |
| rank        | return the rank of the given element<br />*(public member function)* |

## Implementation

### Properties

​	A Size-Balanced Tree is a data structure based on binary tree and has the following properties:

- The size of the left child node is not less than the sizes of its two nephew nodes.
- The size of the right child node is not less than the sizes of its two nephew nodes.
- Both left and right subtrees are Size-Balanced Trees.

​	Consider the following example where T is the node of SBT, L and R are its child nodes, A, B, C, and D are subtrees that also satisfy the above properties of SBT.

```
      T
     / \
    /   \
   L     R
  / \   / \
 A   B C   D
```

​	According to the properties of SBT, the node T must satisfy:

- size(L) >= max(size\(C\), size(D))
- size\(R\) >= max(size(A), size(B))

### Node

​	The node of SBT include a parent node, two child nodes, and the size of nodes in the subtree where the node is located.
​	The C++code for node definition is as follows:

```c++
template <class T>
struct sb_tree_node
{
	using node_type    = sb_tree_node<T>;
	using node_pointer = node_type*;
	node_pointer       parent;
	node_pointer       left;
	node_pointer       right;
	size_t             size;
	T                  data;
};
```

### Rotations

​	Like other self-balancing binary search trees, rotation operations are necessary to restore balance when inserting or deleting nodes causes the Size-Balanced Tree to become unbalanced.

​	Common rotation operations include left rotation and right rotation, which can be achieved by exchanging the position of nodes and subtrees. The following describes the operation process of left and right rotation.

#### Left rotation

​	The left rotation is used to make the right child node R of node T become its parent node and make the left child node A of its right child node R become its right child node. This operation makes the original node T become the left child node of its right child node R, thereby maintaining the balance of the binary tree.

```
      T                   R
     / \                 / \
    L   R       -->     T   B
       / \             / \
      A   B           L   A
```
​	The C++ code for left rotation is as follows:
```C++
node_pointer left_rotate(node_pointer t)
{
	node_pointer r = t->right;
	t->right = r->left;
	if (r->left)
		r->left->parent = t;
	r->parent = t->parent;
	if (t == header->parent)
		header->parent = r;
	else if (t == t->parent->left)
		t->parent->left = r;
	else
		t->parent->right = r;
	r->left = t;
	r->size = t->size;
	t->parent = r;
	t->size = (t->left ? t->left->size : 0) + (t->right ? t->right->size : 0) + 1;
	return r;
}
```

#### Right rotation

​	The right rotation is similar to the left rotation but in the opposite direction. The right rotation is used to make the left child node L of node T become its parent node and make the right child node B of its left child node L become its left child node. This operation makes the original node T become the right child node of its left child node L, thereby maintaining the balance of the binary tree.

```
     T                     L
    / \                   / \
   L   R       -->       A   T
  / \                       / \
 A   B                     B   R
```
​	The C++ code for right rotation is as follows:
```C++
node_pointer right_rotate(node_pointer t)
{
	node_pointer l = t->left;
	t->left = l->right;
	if (l->right)
		l->right->parent = t;
	l->parent = t->parent;
	if (t == header->parent)
		header->parent = l;
	else if (t == t->parent->right)
		t->parent->right = l;
	else
		t->parent->left = l;
	l->right = t;
	l->size = t->size;
	t->parent = l;
	t->size = (t->left ? t->left->size : 0) + (t->right ? t->right->size : 0) + 1;
	return l;
}
```

### Rebalancing

​	After insertion or deletion in SBT, the properties of SBT may be violated, and it is necessary to rebalance the SBT rooted at node T. The prerequisite is that the child nodes of T are already SBTs themselves. There are four cases to consider when rebalancing:

1. size(T.left) < size(T.right.left)

​	It may occur when inserting a node into the right child of T or deleting a node from the left child of T. First, perform a right rotation on the right child node of T, and then perform a left rotation on T. Now, the subtrees A, B, D, E, F, and L still satisfy the properties of SBT, while the left subtree T and the right subtree R may violate the properties of SBT. For the left subtree T, due to the decrease in nodes of its right subtree, it may occur that size(T.right) < size(T.left.child), which requires rebalancing. For the right subtree R, due to the decrease in nodes of its left subtree, it may occur that size(R.left) < size(R.right.child), which requires rebalancing. Finally, rebalance node C and its ancestor nodes one by one until reaching the root node.
```
         T                  T                  C
        / \                / \                / \
       /   \              /   \              /   \
      L     R     -->    L     C     -->    T     R
     / \   / \          / \   / \          / \   / \
    A   B C   D        A   B E   R        L   E F   D
         / \                    / \      / \
        E   F                  F   D    A   B
```

2. size(T.left) < size(T.right.right)

​	It may occur when inserting a node into the right child of T or deleting a node from the left child of T. After performing a left rotation on T, the subtree A, B, C, D, E, F, and L still satisfy the properties of SBT. However, due to the decrease in nodes of the right subtree of T, it may occur that size(T.right) < size(T.left.child), which requires rebalancing. Finally, rebalance node R and its ancestor nodes one by one until reaching the root node.
```
         T                  R
        / \                / \
       /   \              /   \
      L     R     -->    T     D
     / \   / \          / \   / \
    A   B C   D        L   C E   F
             / \      / \
            E   F    A   B
```
3. size(T.right) < size(T.left.right)

​	It may occur when inserting a node into the left child of T or deleting a node from the right child of T. First, perform a left rotation on the left child node of T, and then perform a right rotation on T. Now, the subtrees A, C, D, E, F, and R still satisfy the properties of SBT, while the left subtree L and the right subtree T may violate the properties of SBT. For the left subtree L, due to the decrease in nodes of its right subtree, it may occur that size(L.right) < size(L.left.child), which requires rebalancing. For the right subtree T, due to the decrease in nodes of its left subtree, it may occur that size(T.left) < size(T.right.child), which requires rebalancing. Finally, rebalance node B and its ancestor nodes one by one until reaching the root node.
```
         T                  T                  B
        / \                / \                / \
       /   \              /   \              /   \
      L     R     -->    B     R     -->    L     T
     / \   / \          / \   / \          / \   / \
    A   B C   D        L   F C   D        A   E F   R
       / \            / \                          / \
      E   F          A   E                        C   D
```

4. size(T.right) < size(T.left.left)

​	It may occur when inserting a node into the left child of T or deleting a node from the right child of T. After performing a right rotation on T, the subtrees A, B, C, D, E, F, and R still satisfy the properties of SBT. However, due to the decrease in nodes of the left subtree of T, it may occur that size(T.left) < size(T.right.child), which requires rebalancing. Finally, rebalance node L and its ancestor nodes one by one until reaching the root node.
```
           T                  L
          / \                / \
         /   \              /   \
        L     R     -->    A     T
       / \   / \          / \   / \
      A   B C   D        E   F B   R
     / \                          / \
    E   F                        C   D
```
​	The C++ code for rebalancing after inserting a node into the child node of T is as follows:
```C++
node_pointer insert_rebalance(node_pointer t, bool flag)
{
	if (flag)
	{
		if (t->right)
		{
			size_type left_size = t->left ? t->left->size : 0;
			// case 1: size(T.left) < size(T.right.left)
			if (t->right->left && left_size < t->right->left->size)
			{
				t->right = right_rotate(t->right);
				t = left_rotate(t);
				t->left = insert_rebalance(t->left, false);
				t->right = insert_rebalance(t->right, true);
				t = insert_rebalance(t, true);
			}
			// case 2. size(T.left) < size(T.right.right)
			else if (t->right->right && left_size < t->right->right->size)
			{
				t = left_rotate(t);
				t->left = insert_rebalance(t->left, false);
				t = insert_rebalance(t, true);
			}
		}
	}
	else
	{
		if (t->left)
		{
			size_type right_size = t->right ? t->right->size : 0;
			// case 3. size(T.right) < size(T.left.right)
			if (t->left->right && right_size < t->left->right->size)
			{
				t->left = left_rotate(t->left);
				t = right_rotate(t);
				t->left = insert_rebalance(t->left, false);
				t->right = insert_rebalance(t->right, true);
				t = insert_rebalance(t, false);
			}
			// case 4. size(T.right) < size(T.left.left)
			else if (t->left->left && right_size < t->left->left->size)
			{
				t = right_rotate(t);
				t->right = insert_rebalance(t->right, true);
				t = insert_rebalance(t, false);
			}
		}
	}
	return t;
}
```
​	The C++ code for rebalancing after deleting a node from the child node of T is as follows:
```C++
node_pointer erase_rebalance(node_pointer t, bool flag)
{
	if (!flag)
	{
		if (t->right)
		{
			size_type left_size = t->left ? t->left->size : 0;
			// case 1: size(T.left) < size(T.right.left)
			if (t->right->left && left_size < t->right->left->size)
			{
				t->right = right_rotate(t->right);
				t = left_rotate(t);
				t->left = erase_rebalance(t->left, true);
				t->right = erase_rebalance(t->right, false);
				t = erase_rebalance(t, false);
			}
			// case 2. size(T.left) < size(T.right.right)
			else if (t->right->right && left_size < t->right->right->size)
			{
				t = left_rotate(t);
				t->left = erase_rebalance(t->left, true);
				t = erase_rebalance(t, false);
			}
		}
	}
	else
	{
		if (t->left)
		{
			size_type right_size = t->right ? t->right->size : 0;
			// case 3. size(T.right) < size(T.left.right)
			if (t->left->right && right_size < t->left->right->size)
			{
				t->left = left_rotate(t->left);
				t = right_rotate(t);
				t->left = erase_rebalance(t->left, true);
				t->right = erase_rebalance(t->right, false);
				t = erase_rebalance(t, true);
			}
			// case 4. size(T.right) < size(T.left.left)
			else if (t->left->left && right_size < t->left->left->size)
			{
				t = right_rotate(t);
				t->right = erase_rebalance(t->right, false);
				t = erase_rebalance(t, true);
			}
		}
	}
	return t;
}
```
### Insertion

​	If the SBT is empty, directly add the node as the root node. Otherwise, find the insertion position according to the properties of the binary search tree and insert the new node. The node counts of its ancestor nodes should all be incremented by 1. When inserting a value in the left subtree of node T, the above-mentioned case (3) or case (4) may occur. When inserting a value in the right subtree of node T, the above-mentioned case (1) or case (2) may occur.

​	The C++ code for the insertion operation is as follows:

```C++
template<class ...Args>
node_pointer insert_node(Args&&... args)
{
	// creates a new node
	node_pointer n = this->create_node(std::forward<Args>(args)...);
	n->left = nullptr;
	n->right = nullptr;
	n->size = 1;
	// if the tree is not empty
	if (header->parent)
	{
		// the initial value is root
		node_pointer t = header->parent;
		while (t)
		{
			// increases the size of nodes
			++t->size;
			if (comp(n->data, t->data))
			{
				if (t->left)
					t = t->left;
				else
				{
					// inserts the node
					n->parent = t;
					t->left = n;
					if (t == header->left)
						header->left = n;
					do
					{
						// rebalance after insertion
						t = insert_rebalance(t->parent, t == t->parent->right);
					} while (t->parent != header);
					t = nullptr;
				}
			}
			else
			{
				if (t->right)
					t = t->right;
				else
				{
					// inserts the node
					n->parent = t;
					t->right = n;
					if (t == header->right)
						header->right = n;
					do
					{
						// rebalance after insertion
						t = insert_rebalance(t->parent, t == t->parent->right);
					} while (t->parent != header);
					t = nullptr;
				}
			}
		}
	}
	else
	{
		// inserts the node
		n->parent = header;
		header->parent = n;
		header->left = n;
		header->right = n;
	}
	return n;
}
```

### Deletion

​	According to the properties of the binary search tree, find the deletion position. Assuming the node to be deleted is T, it can be divided into the following two cases based on the number of child nodes of node T:

1. Node T has at most one child node.

​	In this case, the node T can be deleted directly. The number of nodes of its ancestor nodes should be reduced by 1. If node T has a child node L or R, replace node T with its child node. Finally, rebalance the parent node of node T.

2. Node T has two child nodes.

​	In this case, the node T cannot be deleted directly, otherwise the entire tree will be destroyed. When the number of nodes in the left subtree of node T is less than the number of nodes in the right subtree, select the node with the minimum value in its right subtree as the actual deletion node X; otherwise, select the node with the maximum value in its left subtree as the actual deletion node X. Then swap the positions of node T and node X, and it is completely the same as the first case.

​	The C++ code for the deletion operation is as follows:

```C++
void erase_node(node_pointer t)
{
	bool flag;
	node_pointer x;
	node_pointer parent;
	// case 1. has one child node at most
	if (!t->left || !t->right)
	{
		x = t->left ? t->left : t->right;
		// the rebalance flag
		flag = (t == t->parent->right);
		// removes t node
		if (x)
			x->parent = t->parent;
		if (t == header->parent)
			header->parent = x;
		else if (t == t->parent->left)
			t->parent->left = x;
		else
			t->parent->right = x;
		if (t == header->left)
			header->left = x ? leftmost(x) : t->parent;
		if (t == header->right)
			header->right = x ? rightmost(x) : t->parent;
		// reduces the number of nodes
		for (node_pointer p = t->parent; p != header; p = p->parent)
			--p->size;
		if (t != header)
		{
			// rebalance after deletion
			node_pointer p = erase_rebalance(t->parent, flag);
			while (p != header)
				p = erase_rebalance(p->parent, p == p->parent->right);
		}
	}
	// case 2. has two child nodes
	else
	{
		if (t->left->size < t->right->size)
		{
			x = leftmost(t->right);
			// the rebalance flag
			flag = (x == x->parent->right);
			// reduces the number of nodes
			for (node_pointer p = x->parent; p != header; p = p->parent)
				--p->size;
			// replaces t node with x node and removes t node
			t->left->parent = x;
			x->left = t->left;
			if (x != t->right)
			{
				x->parent->left = x->right;
				if (x->right)
					x->right->parent = x->parent;
				t->right->parent = x;
				x->right = t->right;
				parent = x->parent;
			}
			else
				parent = x;
			if (t == header->parent)
				header->parent = x;
			else if (t == t->parent->left)
				t->parent->left = x;
			else
				t->parent->right = x;
			x->parent = t->parent;
			x->size = t->size;
		}
		else
		{
			x = rightmost(t->left);
			// the rebalance flag
			flag = (x == x->parent->right);
			// reduces the number of nodes
			for (node_pointer p = x->parent; p != header; p = p->parent)
				--p->size;
			// replaces t node with x node and removes t node
			t->right->parent = x;
			x->right = t->right;
			if (x != t->left)
			{
				x->parent->right = x->left;
				if (x->left)
					x->left->parent = x->parent;
				t->left->parent = x;
				x->left = t->left;
				parent = x->parent;
			}
			else
				parent = x;
			if (t == header->parent)
				header->parent = x;
			else if (t == t->parent->left)
				t->parent->left = x;
			else
				t->parent->right = x;
			x->parent = t->parent;
			x->size = t->size;
		}
		// rebalance after deletion
		node_pointer p = erase_rebalance(parent, flag);
		while (p != header)
			p = erase_rebalance(p->parent, p == p->parent->right);
	}
	// destroy node
	this->destroy_node(t);
}
```

### Searching

​	The search operation is exactly the same as that of the standard binary search tree. It starts by searching from the root node; if the value being searched for, "key," is less than the node's key value, the search continues in the left subtree. If "key" is greater than the node's key value, the search continues in the right subtree. If "key" is equal to the node's key value, the search is complete.

​	The C++ code for the deletion operation is as follows:

```C++
node_pointer find_node(const value_type& key)
{
	node_pointer pre = header;
	node_pointer cur = header->parent;
	while (cur)
	{
		if (!comp(cur->data, key))
		{
			pre = cur;
			cur = cur->left;
		}
		else
			cur = cur->right;
	}
	if (comp(key, pre->data))
		pre = header;
	return pre;
}
```

### Selection

​	The select operation provides random access function in SBT. Each node keeps track of the number of nodes in its subtree, and this information can be used to find elements of a specific rank. To search for the node with rank k, you check the number of nodes in the left subtree. If the size of the left subtree is greater than k, then continue searching in the left subtree. If the size of the left subtree is less than k, then search for the node with rank k - size - 1 in the right subtree. If the size is equal to k, the search is complete.

​	The C++ code for the deletion operation is as follows:

```C++
node_pointer select_node(size_type k)
{
	node_pointer t = header->parent;
	while (t)
	{
		size_type left_size = t->left ? t->left->size : 0;
		if (left_size < k)
		{
			t = t->right;
			k -= (left_size + 1);
		}
		else if (k < left_size)
			t = t->left;
		else
			return t;
	}
	return header;
}
```

### Ranking

​	The rank operation is the inverse of the select operation, which returns the rank of the given key. If the key of the current node is not less than the given key, continue searching in the left subtree. Otherwise, return the rank of the current node plus the rank of the key in the right subtree.

​	The C++ code for the deletion operation is as follows:

```C++
size_type rank_node(const value_type& key)
{
	size_type rank = 0;
	node_pointer pre = header;
	node_pointer cur = header->parent;
	while (cur)
	{
		if (!comp(cur->data, key))
		{
			pre = cur;
			cur = cur->left;
		}
		else
		{
			rank += cur->left ? cur->left->size + 1 : 1;
			cur = cur->right;
		}
	}
	if (pre == header || comp(key, pre->data))
		rank = static_cast<size_type>(-1);
	return rank;
}
```

## Contribute

​	This passage describes the improvement of deletion and insertion functionalities in SBT along with the provision of standard implementations that can be directly applied to your project. After performing a deletion operation on SBT, it may violate the properties of SBT, so it needs to be rebalanced to ensure that subsequent insert operations can proceed normally. The insert operation provides the functions insert_equal and insert_unique, which can be used to implement set and multiset containers based on SBT.

​	Based on SBT's self-balancing feature, a new data structure called Array Based on Tree (ABT) is derived. For more information, please refer to: https://github.com/RulerCN/Array-Based-on-Tree.

## References

1. Chen, Qifeng. "Size Balanced Tree", Guandong, China, 29 December 2006.
2. https://github.com/jishanshaikh4/size-balanced-tree



------

Copyright (c) 2023, Ruler. All rights reserved.
