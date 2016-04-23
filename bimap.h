#ifndef BIMAP_H
#define BIMAP_H

#include <map>
#include <set>
#include <utility>
#include <iterator>

using namespace std;

template<class Key, class T, class Cmp_key = less<Key>, class Cmp_value = less<T>, class A = allocator<pair<const Key, T> > >
class BiMap
{
	map<key_type, mapped_type, Cmp_key, A> 		_left;
	map<mapped_type, key_type, Cmp_value, A> 	_right;
public:
	typedef Key key_type;
	typedef T mapped_type;

	typedef pair<const Key, T> value_type;
	typedef pair<const T, Key> value_type_right;
	typedef Cmp_key key_compare;
	typedef Cmp_value value_compare;
	typedef A allocator_type;

	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;

	

	struct iterator
	{
		typedef typename std::map<key_type, mapped_type, key_compare, allocator_type>::iterator Iter;
		iterator() : _iter() {}
		iterator(value_type* p) : _iter(p) {}
		iterator(const iterator& other) : _iter(other._iter) {}
		iterator(const Iter& other) : _iter(other) {};
		iterator& operator =(const iterator& other)
		{
			if (this != &other) {
				_iter = other._iter;
			}
			return *this;
		}
		iterator& operator ++()
		{
			++_iter;
			return *this;
		}
		iterator operator ++(int)
		{
			iterator tmp(*this);
			++(*this);
			return tmp;
		}
		bool operator == (iterator const& i)
		{
			return *(_iter) == *(i._iter);
		}
		bool operator != (iterator const& i)
		{
			return !(*this == i);
		}
		reference operator *() { return *(_iter); }
		value_type* operator ->() { return &(*_iter); }

		Iter get_iter()
		{
			return _iter;
		}
		Iter get_iter() const
		{
			return _iter;
		}
		void change_iter(Iter tmp)
		{
			_iter = tmp;
		}

	private:
		Iter _iter;
	};

	//типы

	
	typedef const iterator const_iterator; //tmp

	typedef typename A::size_type size_type;
	typedef typename A::difference_type difference_type;

	typedef std::reverse_iterator<iterator> reverse_iterator;//tmp
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;//tmp


	//итераторы
	iterator begin()
	{
		iterator tmp;
		tmp.change_iter(_left.begin());
		return tmp;
	}
	const_iterator begin() const
	{
		const_iterator tmp;
		tmp.change_iter(_left.begin());
		return tmp;
	}

	iterator end()
	{
		iterator tmp;
		tmp.change_iter(_left.end());
		return tmp;
	}
	const_iterator end() const
	{
		const_iterator tmp;
		tmp.change_iter(_left.end());
		return tmp;
	}

	reverse_iterator rbegin()
	{
		reverse_iterator tmp;
		tmp.change_iter(_left.rbegin());
		return tmp;
	}

	const_reverse_iterator rbegin() const
	{
		const_reverse_iterator tmp;
		tmp.change_iter = (_left.rbegin());
		return tmp;
	}

	reverse_iterator rend()
	{
		reverse_iterator tmp;
		tmp.change_iter(_left.rend());
		return tmp;
	}

	const_reverse_iterator rend() const
	{
		const_reverse_iterator tmp;
		tmp.change_iter(_left.rend());
		return tmp;
	}


	// доступ к элементу с ключом k
	mapped_type& operator[] (const key_type& k) 
	{
		return _left.find(k)->second;
	}

	//конструкторы
	explicit BiMap(const Cmp_key& ck = Cmp_key(), const Cmp_value& cv = Cmp_value(), const A& a = A()) : _left(ck, a), _right(cv, a) {} //empty

	template <class InputIterator>
	BiMap(InputIterator first, InputIterator last,
		const key_compare& ck = key_compare(),
		const value_compare& cv = value_compare(),
		const allocator_type& a = allocator_type()) : _left(ck, a), _right(cv, a) //range
	{
		insert(first, last);
	}

	BiMap(const BiMap& x) : _left(x._left), _right(x._right) {} //copy

	~BiMap() 
	{
		_left.~map();
		_right.~map();
		
	}

	BiMap& operator= (const BiMap& x)
	{
		_left = x._left;
		_right = x._right;
		
		return *this;
	}


	// находит элемент с ключом k
	iterator find(const key_type& k) 
	{
		iterator tmp(_left.find(k));
		return tmp;
	}
	const_iterator find(const key_type& k) const
	{
		const_iterator tmp(_left.find(k));
		return tmp;
	}

	// находит элемент по значению m
	iterator find_value(const mapped_type& m) 
	{
		iterator tmp(_left.find(_right.find(m)->second));
		return tmp;
	}
	const_iterator find_value(const mapped_type& m) const
	{
		const_iterator tmp(_left.find(_right.find(m)->second));
		return tmp;
	}

	pair<iterator, bool> insert(const value_type& val) //вставить пару (key, value)
	{
		if ((find(val.first).get_iter() == _left.end()) && (_right.find(val.second) == _right.end())) {
			_left.insert(val);
			const value_type_right tmp = make_pair(val.second, val.first);
			_right.insert(tmp);
			return make_pair(find(val.first), true);
		}
		else {
			return make_pair(end(), false);
		}
	}


	iterator insert(iterator position, const value_type& val) 
	{
		if (find(val.first) == _left.end() && find_value(val.second) == _right.end()) {
			_left.insert(position._iter, val);
			_right.insert(val.swap());
			return find(val.first);
		}
		return end();

	}

	//вставить элементы из последовательности
	template <class InputIterator> 
	void insert(InputIterator first, InputIterator last)
	{
		while (first != last) {
			insert(*first);
			++first;
		}
	}

	//удалить выбранный элемент
	void erase(iterator pos) 
	{
		if (find(*pos) != end())
		{
			_left.erase(pos->first);
			_right.erase(pos->second);
		}
	}

	//удалить элемент с ключом k
	void erase(const key_type& k)
	{
		erase(find(k));
	}

	//удалить диапазон
	void erase(iterator first, iterator last) 
	{
		while (first != last) {
			iterator tmp = ++first;
			erase(first);
			first = tmp;
		}
	}

	//удалить элемент со значением m
	void erase_value(const mapped_type& m) 
	{
		erase(find_value(m));
	}

	//удалить все элементы
	void clear() 
	{
		_left.clear();
		_right.clear();
	}

	//число элементов
	size_type size() const 
	{
		return _left.size();
	}

	// размер максимально возможного BiMap
	size_type max_size() const 
	{
		return _left.max_size();
	}

	// проверка пустоты bimap-а
	bool empty() const
	{
		return size() == 0;
	}

	//замена
	void swap(BiMap& x)
	{
		BiMap c(x);
		x = *this;
		*this = c;
	}


	const map<key_type, mapped_type, Cmp_key, A>& get_left() const
	{
		return _left;
	}
};

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator== (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() == rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator!= (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() != rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator< (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() < rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator<= (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() <= rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator> (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() > rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
bool operator>= (const BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, const BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	return lhs.get_left() >= rhs.get_left();
}

template <class Key, class T, class Cmp_key, class Cmp_value, class A>
void swap(BiMap<Key, T, Cmp_key, Cmp_value, A>& lhs, BiMap<Key, T, Cmp_key, Cmp_value, A>& rhs)
{
	BiMap<Key, T, Cmp_key, Cmp_value, A> c(lhs);
	lhs = rhs;
	rhs = c;
}



#endif