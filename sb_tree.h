/*====================================================================
BSD 2-Clause License

Copyright (c) 2023, Ruler
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
====================================================================*/
#pragma once

#ifndef __RULER_SB_TREE_H__
#define __RULER_SB_TREE_H__

#include <memory>
#include <stdexcept>
#include <iterator>
#include <functional>
#include <utility>
#include "define.h"

#ifndef DEFAULT_ALLOCATOR
#define DEFAULT_ALLOCATOR(T) std::allocator<T>
#endif // !DEFAULT_ALLOCATOR

using sb_tree_node_state = signed char;
static constexpr sb_tree_node_state sb_tree_state_root    =  0x00;
static constexpr sb_tree_node_state sb_tree_state_parent  = -0x0F;
static constexpr sb_tree_node_state sb_tree_state_left    =  0x12;
static constexpr sb_tree_node_state sb_tree_state_right   =  0x13;
static constexpr sb_tree_node_state sb_tree_state_sibling =  0x04;


// Class template sb_tree_node
template <class T>
struct sb_tree_node
{
	using node_type            = sb_tree_node<T>;
	using node_pointer         = node_type*;
	using const_node_pointer   = const node_type*;
	using node_reference       = node_type&;
	using const_node_reference = const node_type&;

	node_pointer               parent;
	node_pointer               left;
	node_pointer               right;
	size_t                     size;
	T                          data;
};


// Class template sb_tree_type_traits

template <class Tree, bool IsConst>
struct sb_tree_type_traits
{
	using value_type      = typename Tree::value_type;
	using pointer         = typename Tree::pointer;
	using size_type       = typename Tree::size_type;
	using difference_type = typename Tree::difference_type;
	using node_type       = typename Tree::node_type;
	using node_pointer    = typename Tree::node_pointer;
	using reference       = value_type&;
};

template <class Tree>
struct sb_tree_type_traits<Tree, true>
{
	using value_type      = typename Tree::value_type;
	using pointer         = typename Tree::const_pointer;
	using size_type       = typename Tree::size_type;
	using difference_type = typename Tree::difference_type;
	using node_type       = typename Tree::node_type;
	using node_pointer    = typename Tree::node_pointer;
	using reference       = const value_type&;
};


// Class template sb_tree_iterator
template <class Tree, bool IsConst>
class sb_tree_iterator
{
public:
	// types:

	using value_type        = typename sb_tree_type_traits<Tree, IsConst>::value_type;
	using pointer           = typename sb_tree_type_traits<Tree, IsConst>::pointer;
	using reference         = typename sb_tree_type_traits<Tree, IsConst>::reference;
	using size_type         = typename sb_tree_type_traits<Tree, IsConst>::size_type;
	using difference_type   = typename sb_tree_type_traits<Tree, IsConst>::difference_type;
	using node_type         = typename sb_tree_type_traits<Tree, IsConst>::node_type;
	using node_pointer      = typename sb_tree_type_traits<Tree, IsConst>::node_pointer;

	using iterator_type     = sb_tree_iterator<Tree, IsConst>;
	using iterator_category = std::bidirectional_iterator_tag;

	// construct/copy/destroy:

	sb_tree_iterator(void) noexcept
		: node(nullptr)
	{}
	explicit sb_tree_iterator(const node_pointer p) noexcept
		: node(p)
	{}
	sb_tree_iterator(const sb_tree_iterator<Tree, IsConst>& other) noexcept
		: node(other.get_pointer())
	{}

	inline sb_tree_iterator<Tree, IsConst>& operator=(const sb_tree_iterator<Tree, IsConst>& other) noexcept
	{
		if (this != &other)
			node = other.get_pointer();
		return *this;
	}

	inline operator sb_tree_iterator<Tree, true>(void) const noexcept
	{
		return sb_tree_iterator<Tree, true>(node);
	}

	// sb_tree_iterator operations:

	inline node_pointer get_parent(void) noexcept
	{
		return node->parent;
	}
	inline const node_pointer get_parent(void) const noexcept
	{
		return node->parent;
	}

	inline node_pointer get_pointer(void) noexcept
	{
		return node;
	}
	inline const node_pointer get_pointer(void) const noexcept
	{
		return node;
	}

	inline size_type get_size(void) const noexcept
	{
		return node->size;
	}

	inline reference operator*(void) const noexcept
	{
		return node->data;
	}

	inline pointer operator->(void) const noexcept
	{
		return &(operator*());
	}

	// increment / decrement

	sb_tree_iterator<Tree, IsConst>& operator++(void) noexcept
	{
		if (node->right)
		{
			node = node->right;
			while (node->left)
				node = node->left;
		}
		else
		{
			node_pointer p = node->parent;
			while (node == p->right)
			{
				node = p;
				p = p->parent;
			}
			if (node->right != p)
				node = p;
		}
		return *this;
	}

	sb_tree_iterator<Tree, IsConst>& operator--(void) noexcept
	{
		if (!node->parent || node->parent->parent == node)
			node = node->right;
		else if (node->left)
		{
			node_pointer p = node->left;
			while (p->right)
				p = p->right;
			node = p;
		}
		else
		{
			node_pointer p = node->parent;
			while (node == p->left)
			{
				node = p;
				p = p->parent;
			}
			node = p;
		}
		return *this;
	}

	inline sb_tree_iterator<Tree, IsConst> operator++(int) noexcept
	{
		iterator_type itr(*this);
		this->operator++();
		return itr;
	}

	inline sb_tree_iterator<Tree, IsConst> operator--(int) noexcept
	{
		iterator_type itr(*this);
		this->operator--();
		return itr;
	}

	// relational operators:

	template <bool is_const>
	inline bool operator==(const sb_tree_iterator<Tree, is_const>& rhs) const noexcept
	{
		return node == rhs.get_pointer();
	}

	template <bool is_const>
	inline bool operator!=(const sb_tree_iterator<Tree, is_const>& rhs) const noexcept
	{
		return node != rhs.get_pointer();
	}

private:
	node_pointer node;
};


// Class template sb_tree_primitive_iterator
template <class Tree, bool IsConst>
class sb_tree_primitive_iterator
{
public:
	// types:

	using value_type        = typename sb_tree_type_traits<Tree, IsConst>::value_type;
	using pointer           = typename sb_tree_type_traits<Tree, IsConst>::pointer;
	using reference         = typename sb_tree_type_traits<Tree, IsConst>::reference;
	using size_type         = typename sb_tree_type_traits<Tree, IsConst>::size_type;
	using difference_type   = typename sb_tree_type_traits<Tree, IsConst>::difference_type;
	using node_type         = typename sb_tree_type_traits<Tree, IsConst>::node_type;
	using node_pointer      = typename sb_tree_type_traits<Tree, IsConst>::node_pointer;

	using iterator_type     = sb_tree_primitive_iterator<Tree, IsConst>;
	using iterator_category = std::bidirectional_iterator_tag;

	// construct/copy/destroy:

	sb_tree_primitive_iterator(void) noexcept
		: node(nullptr)
		, state(sb_tree_state_root)
	{}
	explicit sb_tree_primitive_iterator(const node_pointer p) noexcept
		: node(p)
		, state(sb_tree_state_root)
	{}
	sb_tree_primitive_iterator(const sb_tree_primitive_iterator<Tree, IsConst>& other) noexcept
		: node(other.get_pointer())
		, state(other.get_state())
	{}

	inline sb_tree_primitive_iterator<Tree, IsConst>& operator=(const sb_tree_primitive_iterator<Tree, IsConst>& other) noexcept
	{
		if (this != &other)
		{
			node = other.get_pointer();
			state = other.get_state();
		}
		return *this;
	}

	inline operator sb_tree_primitive_iterator<Tree, true>(void) const noexcept
	{
		return sb_tree_primitive_iterator<Tree, true>(node);
	}

	// sb_tree_primitive_iterator operations:

	inline node_pointer get_parent(void) noexcept
	{
		return node->parent;
	}
	inline const node_pointer get_parent(void) const noexcept
	{
		return node->parent;
	}

	inline node_pointer get_pointer(void) noexcept
	{
		return node;
	}
	inline const node_pointer get_pointer(void) const noexcept
	{
		return node;
	}

	inline sb_tree_node_state get_state(void) const noexcept
	{
		return state;
	}

	inline intptr_t get_depth(void) const noexcept
	{
		return static_cast<intptr_t>(state >> 4);
	}

	inline size_type get_size(void) const noexcept
	{
		return node->size;
	}

	inline reference operator*(void) const noexcept
	{
		return node->data;
	}

	inline pointer operator->(void) const noexcept
	{
		return &(operator*());
	}

	// increment / decrement

	sb_tree_primitive_iterator<Tree, IsConst>& operator++(void) noexcept
	{
		if (state != sb_tree_state_parent && node->left)
		{
			node = node->left;
			state = sb_tree_state_left;
		}
		else if (state != sb_tree_state_parent && node->right)
		{
			node = node->right;
			state = sb_tree_state_right;
		}
		else if (node != node->parent->parent &&
			node->parent->right && node != node->parent->right)
		{
			node = node->parent->right;
			state = sb_tree_state_sibling;
		}
		else
		{
			node = node->parent;
			state = sb_tree_state_parent;
		}
		return *this;
	}

	sb_tree_primitive_iterator<Tree, IsConst>& operator--(void) noexcept
	{
		if (state != sb_tree_state_parent && node->right)
		{
			node = node->right;
			state = sb_tree_state_right;
		}
		else if (state != sb_tree_state_parent && node->left)
		{
			node = node->left;
			state = sb_tree_state_left;
		}
		else if (node != node->parent->parent &&
			node->parent->left && node != node->parent->left)
		{
			node = node->parent->left;
			state = sb_tree_state_sibling;
		}
		else
		{
			node = node->parent;
			state = sb_tree_state_parent;
		}
		return *this;
	}

	inline sb_tree_primitive_iterator<Tree, IsConst> operator++(int) noexcept
	{
		iterator_type itr(*this);
		this->operator++();
		return itr;
	}

	inline sb_tree_primitive_iterator<Tree, IsConst> operator--(int) noexcept
	{
		iterator_type itr(*this);
		this->operator--();
		return itr;
	}

	// relational operators:

	template <bool is_const>
	inline bool operator==(const sb_tree_primitive_iterator<Tree, is_const>& rhs) const noexcept
	{
		return node == rhs.get_pointer();
	}

	template <bool is_const>
	inline bool operator!=(const sb_tree_primitive_iterator<Tree, is_const>& rhs) const noexcept
	{
		return node != rhs.get_pointer();
	}

private:
	node_pointer       node;
	sb_tree_node_state state;
};


// Class template sb_tree_node_allocator
template <class T, class Allocator>
class sb_tree_node_allocator
{
public:
	// types:

	using tree_traits_type     = std::allocator_traits<Allocator>;
	using tree_node_type       = typename sb_tree_node<T>::node_type;
	using allocator_type       = typename tree_traits_type::template rebind_alloc<T>;
	using traits_type          = typename tree_traits_type::template rebind_traits<T>;
	using node_allocator_type  = typename tree_traits_type::template rebind_alloc<tree_node_type>;
	using node_traits_type     = typename tree_traits_type::template rebind_traits<tree_node_type>;
	using node_type            = typename node_traits_type::value_type;
	using node_pointer         = typename node_traits_type::pointer;
	using node_size_type       = typename node_traits_type::size_type;
	using node_difference_type = typename node_traits_type::difference_type;

	// construct/copy/destroy:

	sb_tree_node_allocator(void)
		: allocator()
	{}
	explicit sb_tree_node_allocator(const Allocator& alloc)
		: allocator(alloc)
	{}
	explicit sb_tree_node_allocator(Allocator&& alloc)
		: allocator(std::forward<Allocator>(alloc))
	{}

	// sb_tree_node_allocator operations:

	inline allocator_type get_allocator(void) noexcept
	{
		return allocator;
	}
	inline const allocator_type get_allocator(void) const noexcept
	{
		return allocator;
	}

	inline node_size_type max_size(void) const noexcept
	{
		return node_alloc.max_size();
	}

protected:

	template <class ...Args>
	inline node_pointer create_node(Args&&... args)
	{
		node_pointer p = node_traits_type::allocate(node_alloc, 1);
		traits_type::construct(allocator, std::addressof(p->data), std::forward<Args>(args)...);
		return p;
	}

	inline void destroy_node(const node_pointer p)
	{
		traits_type::destroy(allocator, std::addressof(p->data));
		node_traits_type::deallocate(node_alloc, p, 1);
	}

private:
	allocator_type      allocator;
	node_allocator_type node_alloc;
};


// Class template sb_tree
template <class T, class Compare = std::less<T>, class Allocator = DEFAULT_ALLOCATOR(T)>
class sb_tree : public sb_tree_node_allocator<T, Allocator>
{
public:
	// types:

	using compare_type                     = Compare;
	using tree_type                        = sb_tree<T, Compare, Allocator>;
	using tree_traits_type                 = std::allocator_traits<Allocator>;
	using node_type                        = typename sb_tree_node<T>::node_type;
	using node_pointer                     = typename sb_tree_node<T>::node_pointer;
	using const_node_pointer               = typename sb_tree_node<T>::const_node_pointer;
	using node_allocator_type              = typename tree_traits_type::template rebind_alloc<node_type>;
	using allocator_type                   = typename tree_traits_type::template rebind_alloc<T>;
	using traits_type                      = typename tree_traits_type::template rebind_traits<T>;
	using value_type                       = typename traits_type::value_type;
	using reference                        = value_type&;
	using const_reference                  = const value_type&;
	using pointer                          = typename traits_type::pointer;
	using const_pointer                    = typename traits_type::const_pointer;
	using size_type                        = typename traits_type::size_type;
	using difference_type                  = typename traits_type::difference_type;

	using iterator                         = sb_tree_iterator<tree_type, false>;
	using const_iterator                   = sb_tree_iterator<tree_type, true>;
	using primitive_iterator               = sb_tree_primitive_iterator<tree_type, false>;
	using const_primitive_iterator         = sb_tree_primitive_iterator<tree_type, true>;
	using reverse_iterator                 = std::reverse_iterator<iterator>;
	using const_reverse_iterator           = std::reverse_iterator<const_iterator>;
	using reverse_primitive_iterator       = std::reverse_iterator<primitive_iterator>;
	using const_reverse_primitive_iterator = std::reverse_iterator<const_primitive_iterator>;

	// construct/copy/destroy:

	explicit sb_tree(const compare_type& compare = compare_type(), const Allocator& alloc = Allocator())
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(compare)
		, header(nullptr)
	{
		create_header();
	}
	explicit sb_tree(const Allocator& alloc)
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(compare_type())
		, header(nullptr)
	{
		create_header();
	}
	template <class InputIt>
	sb_tree(InputIt first, InputIt last, const compare_type& compare = compare_type(), const Allocator& alloc = Allocator())
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(compare)
		, header(nullptr)
	{
		create_header();
		insert_unique(first, last);
	}
	template <class InputIt>
	sb_tree(InputIt first, InputIt last, const Allocator& alloc)
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(compare_type())
		, header(nullptr)
	{
		create_header();
		insert_unique(first, last);
	}
	sb_tree(const tree_type& other)
		: sb_tree_node_allocator<T, Allocator>()
		, comp(other.comp)
		, header(nullptr)
	{
		create_header();
		if (other.header->parent)
			copy_node(other.header->parent);
	}
	sb_tree(const tree_type& other, const Allocator& alloc)
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(other.comp)
		, header(nullptr)
	{
		create_header();
		if (other.header->parent)
			copy_node(other.header->parent);
	}
	sb_tree(tree_type&& other) noexcept
		: sb_tree_node_allocator<T, Allocator>()
		, comp(Compare())
		, header(nullptr)
	{
		create_header();
		swap(other);
	}
	sb_tree(tree_type&& other, const Allocator& alloc) noexcept
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(Compare())
		, header(nullptr)
	{
		create_header();
		swap(other);
	}
	sb_tree(std::initializer_list<T> init, const Allocator& alloc = Allocator())
		: sb_tree_node_allocator<T, Allocator>(alloc)
		, comp(Compare())
		, header(nullptr)
	{
		create_header();
		insert_unique(init.begin(), init.end());
	}

	~sb_tree(void)
	{
		clear();
		destroy_header();
	}

	inline tree_type& operator=(const tree_type& other)
	{
		if (this != &other)
		{
			clear();
			if (other.header->parent)
				copy_root(other.header->parent);
			comp = other.comp;
		}
		return *this;
	}
	inline tree_type& operator=(tree_type&& other) noexcept
	{
		if (this != &other)
			swap(other);
		return *this;
	}

	inline void assign_equal(size_type n, const value_type& value)
	{
		clear();
		insert_equal(n, value);
	}
	template <class InputIt>
	inline void assign_equal(InputIt first, InputIt last)
	{
		clear();
		insert_equal(first, last);
	}
	inline void assign_equal(std::initializer_list<value_type> init)
	{
		assign_equal(init.begin(), init.end());
	}

	inline void assign_unique(const value_type& value)
	{
		clear();
		insert_unique(value);
	}
	template <class InputIt>
	inline void assign_unique(InputIt first, InputIt last)
	{
		clear();
		insert_unique(first, last);
	}
	inline void assign_unique(std::initializer_list<value_type> init)
	{
		assign_unique(init.begin(), init.end());
	}

	// iterators:

	inline iterator begin(void) noexcept
	{
		return iterator(header->left);
	}
	inline const_iterator begin(void) const noexcept
	{
		return const_iterator(header->left);
	}
	inline const_iterator cbegin(void) const noexcept
	{
		return const_iterator(header->left);
	}
	inline iterator end(void) noexcept
	{
		return iterator(header);
	}
	inline const_iterator end(void) const noexcept
	{
		return const_iterator(header);
	}
	inline const_iterator cend(void) const noexcept
	{
		return const_iterator(header);
	}

	inline reverse_iterator rbegin(void) noexcept
	{
		return reverse_iterator(end());
	}
	inline const_reverse_iterator rbegin(void) const noexcept
	{
		return const_reverse_iterator(end());
	}
	inline const_reverse_iterator crbegin(void) const noexcept
	{
		return const_reverse_iterator(cend());
	}
	inline reverse_iterator rend(void) noexcept
	{
		return reverse_iterator(begin());
	}
	inline const_reverse_iterator rend(void) const noexcept
	{
		return const_reverse_iterator(begin());
	}
	inline const_reverse_iterator crend(void) const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	inline primitive_iterator pbegin(void) noexcept
	{
		return primitive_iterator(root());
	}
	inline const_primitive_iterator pbegin(void) const noexcept
	{
		return const_primitive_iterator(root());
	}
	inline const_primitive_iterator cpbegin(void) const noexcept
	{
		return const_primitive_iterator(root());
	}
	inline primitive_iterator pend(void) noexcept
	{
		return primitive_iterator(header);
	}
	inline const_primitive_iterator pend(void) const noexcept
	{
		return const_primitive_iterator(header);
	}
	inline const_primitive_iterator cpend(void) const noexcept
	{
		return const_primitive_iterator(header);
	}

	inline reverse_primitive_iterator rpbegin(void) noexcept
	{
		return reverse_primitive_iterator(pend());
	}
	inline const_reverse_primitive_iterator rpbegin(void) const noexcept
	{
		return const_reverse_primitive_iterator(pend());
	}
	inline const_reverse_primitive_iterator crpbegin(void) const noexcept
	{
		return const_reverse_primitive_iterator(cpend());
	}
	inline reverse_primitive_iterator rpend(void) noexcept
	{
		return reverse_primitive_iterator(pbegin());
	}
	inline const_reverse_primitive_iterator rpend(void) const noexcept
	{
		return const_reverse_primitive_iterator(pbegin());
	}
	inline const_reverse_primitive_iterator crpend(void) const noexcept
	{
		return const_reverse_primitive_iterator(cpbegin());
	}

	// capacity:

	inline bool empty(void) const noexcept
	{
		return !header->parent;
	}

	inline size_type size(void) const noexcept
	{
		return header->parent ? header->parent->size : 0;
	}

	// observers:

	inline compare_type compare(void) const
	{
		return comp;
	}

	// element access:

	template <class KeyType>
	inline reference at(const KeyType& key)
	{
		if (empty())
			throw std::domain_error(SBT_NOT_INITIALIZED);
		iterator itr = find(key);
		if (itr == end())
			throw std::out_of_range(SBT_OUT_OF_RANGE);
		return itr->data;
	}
	template <class KeyType>
	inline const_reference at(const KeyType& key) const
	{
		if (empty())
			throw std::domain_error(SBT_NOT_INITIALIZED);
		const_iterator itr = find(key);
		if (itr == cend())
			throw std::out_of_range(SBT_OUT_OF_RANGE);
		return itr->data;
	}

	// modifiers:

	template <class... Args>
	inline iterator emplace_equal(Args&&... args)
	{
		return iterator(insert_equal_node(std::forward<Args>(args)...));
	}

	inline iterator insert_equal(const value_type& value)
	{
		return iterator(insert_equal_node(value));
	}
	inline iterator insert_equal(value_type&& value)
	{
		return iterator(insert_equal_node(std::forward<value_type>(value)));
	}
	inline iterator insert_equal(size_type n, const value_type& value)
	{
		node_pointer t = header;
		if (n != 0)
		{
			t = insert_equal_node(value);
			for (; n > 1; --n)
				insert_equal_node(value);
		}
		return iterator(t);
	}
	template <class InputIt>
	inline iterator insert_equal(InputIt first, InputIt last)
	{
		node_pointer t = header;
		if (first != last)
		{
			t = insert_equal_node(*first);
			for (++first; first != last; ++first)
				insert_equal_node(*first);
		}
		return iterator(t);
	}
	inline iterator insert_equal(std::initializer_list<value_type> init)
	{
		return insert_equal(init.begin(), init.end());
	}

	template <class... Args>
	inline std::pair<iterator, bool> emplace_unique(Args&&... args)
	{
		std::pair<node_pointer, bool> res = insert_unique_node(std::forward<Args>(args)...);
		return std::make_pair(iterator(res.first), res.second);
	}

	inline std::pair<iterator, bool> insert_unique(const value_type& value)
	{
		std::pair<node_pointer, bool> res = insert_unique_node(value);
		return std::make_pair(iterator(res.first), res.second);
	}
	inline std::pair<iterator, bool> insert_unique(value_type&& value)
	{
		std::pair<node_pointer, bool> res = insert_unique_node(std::forward<value_type>(value));
		return std::make_pair(iterator(res.first), res.second);
	}
	template <class InputIt>
	inline void insert_unique(InputIt first, InputIt last)
	{
		for (; first != last; ++first)
			insert_unique_node(*first);
	}
	inline void insert_unique(std::initializer_list<value_type> init)
	{
		insert_unique(init.begin(), init.end());
	}

	inline iterator erase(const_iterator pos)
	{
		iterator next = iterator(pos.get_pointer());
		if (pos != cend())
		{
			++next;
			erase_node(pos.get_pointer());
		}
		return next;
	}
	inline iterator erase(const_iterator first, const_iterator last)
	{
		iterator next = iterator(last.get_pointer());
		if (first == cbegin() && last == cend())
			clear();
		else
			while (first != last)
				erase(first++);
		return next;
	}
	template <class KeyType>
	inline size_type erase(const KeyType& key)
	{
		size_type n = 0;
		iterator first = lower_bound(key);
		iterator last = upper_bound(key);
		while (first != last)
		{
			++n;
			erase(first++);
		}
		return n;
	}

	inline void swap(tree_type& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(header, rhs.header);
			std::swap(comp, rhs.comp);
		}
	}

	inline void clear(void)
	{
		if (header->parent)
		{
			erase_root();
			header->parent = nullptr;
			header->left = header;
			header->right = header;
		}
	}

	// operations:

	template <class KeyType>
	inline iterator find(const KeyType& key) noexcept
	{
		return iterator(find_node(key));
	}
	template <class KeyType>
	inline const_iterator find(const KeyType& key) const noexcept
	{
		return const_iterator(find_node(key));
	}

	template <class KeyType>
	inline iterator lower_bound(const KeyType& key) noexcept
	{
		return iterator(lower_bound_node(key));
	}
	template <class KeyType>
	inline const_iterator lower_bound(const KeyType& key) const noexcept
	{
		return const_iterator(lower_bound_node(key));
	}

	template <class KeyType>
	inline iterator upper_bound(const KeyType& key) noexcept
	{
		return iterator(upper_bound_node(key));
	}
	template <class KeyType>
	inline const_iterator upper_bound(const KeyType& key) const noexcept
	{
		return const_iterator(upper_bound_node(key));
	}

	inline iterator select(size_type idx) noexcept
	{
		return iterator(select_node(idx));
	}
	inline const_iterator select(size_type idx) const noexcept
	{
		return const_iterator(select_node(idx));
	}

	inline size_type rank(const value_type& key) const noexcept
	{
		return rank_node(key);
	}

private:

	inline node_pointer root(void) const noexcept
	{
		return header->parent ? header->parent : header;
	}

	inline node_pointer leftmost(node_pointer t) const noexcept
	{
		while (t->left)
			t = t->left;
		return t;
	}

	inline node_pointer rightmost(node_pointer t) const noexcept
	{
		while (t->right)
			t = t->right;
		return t;
	}

	inline void create_header(void)
	{
		if (!header)
		{
			header = this->create_node(value_type());
			header->parent = nullptr;
			header->left = header;
			header->right = header;
			header->size = 0;
		}
	}

	inline void destroy_header(void)
	{
		if (header)
		{
			this->destroy_node(header);
			header = nullptr;
		}
	}

	template <class KeyType>
	node_pointer find_node(const KeyType& key) const noexcept
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

	template <class KeyType>
	node_pointer lower_bound_node(const KeyType& key) const noexcept
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
		return pre;
	}

	template <class KeyType>
	node_pointer upper_bound_node(const KeyType& key) const noexcept
	{
		node_pointer pre = header;
		node_pointer cur = header->parent;
		while (cur)
		{
			if (comp(key, cur->data))
			{
				pre = cur;
				cur = cur->left;
			}
			else
				cur = cur->right;
		}
		return pre;
	}

	node_pointer select_node(size_type k) const noexcept
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

	template <class KeyType>
	size_type rank_node(const KeyType& key) const noexcept
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

	void copy_node(const node_pointer t)
	{
		bool flag = true;
		node_pointer src = t;
		node_pointer dst = header;
		// copies the t node
		node_pointer n = this->create_node(t->data);
		n->parent = dst;
		n->left = nullptr;
		n->right = nullptr;
		n->size = t->size;
		dst->parent = n;
		// update dst to root node
		dst = n;
		do
		{
			if (flag && src->left)
			{
				src = src->left;
				// copies the left child node
				n = this->create_node(src->data);
				n->parent = dst;
				n->left = nullptr;
				n->right = nullptr;
				n->size = src->size;
				dst->left = n;
				// update dst to left child node
				dst = n;
			}
			else if (flag && src->right)
			{
				src = src->right;
				// copies the right child node
				n = this->create_node(src->data);
				n->parent = dst;
				n->left = nullptr;
				n->right = nullptr;
				n->size = src->size;
				dst->right = n;
				// update dst to right child node
				dst = n;
			}
			else if (src->parent->right && src != src->parent->right)
			{
				src = src->parent->right;
				// copies the sibling node
				n = this->create_node(src->data);
				n->parent = dst->parent;
				n->left = nullptr;
				n->right = nullptr;
				n->size = src->size;
				dst->parent->right = n;
				// update dst to sibling node
				dst = n;
				flag = true;
			}
			else
			{
				// return to parent node
				src = src->parent;
				dst = dst->parent;
				flag = false;
			}
		} while (src != t);
		header->left = leftmost(header->parent);
		header->right = rightmost(header->parent);
	}

	template<class ...Args>
	node_pointer insert_equal_node(Args&&... args)
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

	template<class ...Args>
	std::pair<node_pointer, bool> insert_unique_node(Args&&... args)
	{
		node_pointer n = nullptr;
		value_type val = value_type(std::forward<Args>(args)...);
		// if the tree is not empty
		if (header->parent)
		{
			// the initial value is root
			node_pointer t = header->parent;
			while (t)
			{
				if (comp(val, t->data))
				{
					if (t->left)
						t = t->left;
					else
					{
						// creates a new node
						n = this->create_node(std::forward<value_type>(val));
						n->left = nullptr;
						n->right = nullptr;
						n->size = 1;
						// inserts the node
						n->parent = t;
						t->left = n;
						if (t == header->left)
							header->left = n;
						// increases the size of nodes
						for (node_pointer p = t; p != header; p = p->parent)
							++p->size;
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
					// if it already exists
					if (!comp(t->data, val))
						return std::make_pair(t, false);
					if (t->right)
						t = t->right;
					else
					{
						// creates a new node
						n = this->create_node(std::forward<value_type>(val));
						n->left = nullptr;
						n->right = nullptr;
						n->size = 1;
						// inserts the node
						n->parent = t;
						t->right = n;
						if (t == header->right)
							header->right = n;
						// increases the size of nodes
						for (node_pointer p = t; p != header; p = p->parent)
							++p->size;
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
			// creates a new node
			n = this->create_node(std::forward<value_type>(val));
			n->left = nullptr;
			n->right = nullptr;
			n->size = 1;
			// inserts the node
			n->parent = header;
			header->parent = n;
			header->left = n;
			header->right = n;
		}
		return std::make_pair(n, true);
	}

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

	void erase_root(void)
	{
		node_pointer next;
		node_pointer cur = header->parent;
		do
		{
			while (cur->left)
				cur = cur->left;
			if (cur->right)
				cur = cur->right;
			else
			{
				next = cur->parent;
				if (cur == next->left)
					next->left = nullptr;
				else
					next->right = nullptr;
				this->destroy_node(cur);
				cur = next;
			}
		} while (cur != header);
	}

	node_pointer left_rotate(node_pointer t) const noexcept
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

	node_pointer right_rotate(node_pointer t) const noexcept
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

	node_pointer insert_rebalance(node_pointer t, bool flag) const noexcept
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

	node_pointer erase_rebalance(node_pointer t, bool flag) const noexcept
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

private:
	compare_type comp;
	node_pointer header;
};

#endif
