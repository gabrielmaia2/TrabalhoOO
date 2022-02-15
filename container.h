#ifndef CONTAINER_H
#define CONTAINER_H

#define err_not_found			-1

using stype = long long;
using ustype = unsigned long long;

template <typename T>
class Container final
{
private:
	class Node final
	{
	private:
		T _data;
		
		Node *_prev;
		Node *_next;
	public:
		inline Node(T d, Node *pos = nullptr, bool last = false) : _data(d)
		{
			if(pos)
			{
				if(last)
				{
					_next = nullptr;
					_prev = pos;
					pos->_next = this;
				}
				else
				{
					_next = pos;
					pos->_prev = this;
					_prev = pos->_prev;
					if(_prev) _prev->_next = this;
				}
			}
			else
			{
				_next = nullptr;
				_prev = nullptr;
			}
		}
		inline ~Node()
		{
			if(_next)
				_next->_prev = _prev;
			if(_prev)
				_prev->_next = _next;
		}
		
		inline T &data() { return _data; }
		inline const T &data() const { return _data; }
		
		inline Node *next() { return _next; }
		inline Node *prev() { return _prev; }
		inline const Node *next() const { return _next; }
		inline const Node *prev() const { return _prev; }
		
		inline Node *getPos(ustype pos)
		{
			Node *node = this;
			
			for(ustype i = 0; i < pos; i++)
			{
				if(node->_next)
					node = node->_next;
				else
					break;
			}
			return node;
		}
		
		inline void clear()
		{
			if(_next)
				_next->clear();
			delete this;
		}
	};
	
	Node *_root;
	Node *_last;
	
	ustype _size;
public:
	class iterator;
	class const_iterator;
	
	inline Container() : _root(nullptr), _last(nullptr), _size(0) { }
	inline Container(Container &c): Container<T>() { for(auto &i : c) push_back(i); }
	inline Container(const Container &c): Container<T>() { for(auto &i : c) push_back(i); }
	inline Container(Container &&c): Container<T>() { for(auto &i : c) push_back(i); c.clear(); }
	inline virtual ~Container() { clear(); }
	
	inline void operator=(const Container &c)
	{
		clear();
		for(auto &i : c)
			push_back(i);
	}
	inline void operator=(Container &c)
	{
		clear();
		for(auto &i : c)
			push_back(i);
	}
	inline void operator=(Container &&c)
	{
		clear();
		for(auto &i : c)
			push_back(i);
		c.clear();
	}
	inline void operator+=(const Container &c)
	{
		for(auto &i : c)
			push_back(i);
	}
	inline void operator+=(Container &c)
	{
		for(auto &i : c)
			push_back(i);
	}
	inline void operator+=(Container &&c)
	{
		for(auto &i : c)
			push_back(i);
		c.clear();
	}
	
	/// Searches in container for an item.
	/// @param item is the searched item.
	/// @return the position in container of the requested item.
	inline long long find(T item) const
	{
		stype i = 0;
		if(_root)
		{
			Node *node = _root;
			while (node)
			{
				if (node->data() == item) return i;
				i++;
				node = node->next();
			}
		}
		return err_not_found;
	}
	
	/// Gets an item in container.
	/// @param pos is the position of the item in container.
	/// @return the item in the requested position.
	/// @warning Do not use this to iterate through the container, use iterator instead.
	/// @details If the position is out of the container, the function will return the last item.
	inline T &operator[](ustype pos)
	{
		if(_size)
		{
			if(pos >= _size - 1) return _last->data();
			Node *node = _root->getPos(pos);
			return node->data();
		}
		else
		{
			return *static_cast<T *>(nullptr);
		}
	}
	/// Gets an item in container.
	/// @param pos is the position of the item in container.
	/// @return the item in the requested position.
	/// @warning Do not use this to iterate through the container, use iterator instead.
	/// @details If the position is out of the container, the function will return the last item.
	inline const T &operator[](ustype pos) const
	{
		if(_size)
		{
			if(pos >= _size - 1) return _last->data();
			Node *node = _root->getPos(pos);
			return node->data();
		}
		else
		{
			return *static_cast<T *>(nullptr);
		}
	}
	
	/// Inserts an item in the container.
	/// @param item is the item to insert.
	/// @param pos is the position where the item will be inserted.
	/// @details If the position is out of the container, the function will return the last item.
	inline void insert(T item, ustype pos)
	{
		if(pos == 0)
		{
			_root = new Node(item, _root);
			if(!_size)
				_last = _root;
		}
		else if(pos >= _size)
		{
			_last = new Node(item, _last, true);
			if(!_size)
				_root = _last;
		}
		else
		{
			Node *node = _root->getPos(pos);
			new Node(item, node);
		}
		_size++;
	}
	/// Inserts an item in the last position of the container.
	/// @param item is the item to insert.
	inline void push_back(T item)
	{
		_last = new Node(item, _last, true);
		if(!_size)
			_root = _last;
		
		_size++;
	}
	/// Inserts an item in the first position of the container.
	/// @param item is the item to insert.
	inline void push_front(T item)
	{
		_root = new Node(item, _root);
		if(!_size)
			_last = _root;
		
		_size++;
	}
	
	/// Removes an item from the container.
	/// @param item is the item to remove.
	/// @details If the item is not in the list, nothing happens.
	/// @details If the container is empty, nothing happens.
	inline void erase(T item)
	{
		stype pos = find(item);
		if(pos != err_not_found)
		{
			Node *node = _root->getPos(static_cast<ustype>(pos));
			
			if(_size == 1)
			{
				_root = nullptr;
				_last = nullptr;
			}
			else if(pos == 0)
				_root = _root->next();
			else if(static_cast<ustype>(pos) >= _size - 1)
				_last = _last->prev();
			
			delete node;
			_size--;
		}
	}
	/// Removes an item from the container.
	/// @param pos is the position of the item to remove.
	/// @details If the pos is out of the container, the last item will be removed.
	/// @details If the container is empty, nothing happens.
	inline void remove(ustype pos)
	{
		if(_size)
		{
			Node *node;
			if(pos >= _size - 1)
			{
				node = _last;
				_last = _last->prev();
				if(_size == 1)
					_root = nullptr;
			}
			else
			{
				node = _root->getPos(pos);
				if(pos == 0)
					_root = _root->next();
			}
			
			delete node;
			_size--;
		}
	}
	/// Removes the item in the end of the container.
	/// @details If the container is empty, nothing happens.
	inline void pop_back()
	{
		if(_size)
		{
			Node *node = _last;
			_last = _last->prev();
			if(_size == 1)
				_root = nullptr;
			
			delete node;
			_size--;
		}
	}
	/// Removes the item in the start of the container.
	/// @details If the container is empty, nothing happens.
	inline void pop_front()
	{
		if(_size)
		{
			Node *node = _root;
			_root = _root->next();
			if(_size == 1)
				_last = nullptr;
			
			delete node;
			_size--;
		}
	}
	/// Removes all the items in container.
	/// @details If the container is empty, nothing happens.
	inline void clear()
	{
		if(_size)
		{
			_root->clear();
			_root = nullptr;
			_last = nullptr;
			_size = 0;
		}
	}
	
	/// Gets the size of the container
	inline const ustype &size() const { return _size; }
	
	/// Iterator for the beginning of the container.
	inline iterator begin() { return _root; }
	/// Iterator to the end of the container.
	inline iterator end() { return _last; }
	/// Const Iterator for the beginning of the container.
	inline const_iterator cbegin() { return _root; }
	/// Const Iterator to the end of the container.
	inline const_iterator cend() { return _last; }
	
//
	
	/// Class to iterate through the items of the container.
	class iterator final
	{
	private:
		Node *_data;
		bool _flag;
	public:
		inline iterator(Node *n = nullptr) : _data(n), _flag(true) { }
		inline iterator(const iterator &it) : _data(it._data) { }
		inline iterator(iterator &&it) : _data(it._data) { it._data = nullptr; }
		inline ~iterator() { }
		
		inline operator bool() { return static_cast<bool>(_data); }
		
		inline iterator &next()
		{
			static iterator it;
			it = _data->next();
			return it;
		}
		inline iterator &prev()
		{
			static iterator it;
			it = _data->prev();
			return it;
		}
		
		inline iterator &operator=(const iterator &it)
		{
			_data = it._data;
			return *this;
		}
		inline iterator &operator=(iterator &&it)
		{
			_data = it._data;
			it._data = nullptr;
			return *this;
		}
		
		inline iterator &operator++()
		{
			if(_data)
				if(_data->next())
					_data = _data->next();
			return *this;
		}
		inline iterator &operator++(int)
		{
			if(_data)
				if(_data->next())
					_data = _data->next();
			return *this;
		}
		inline iterator &operator--()
		{
			if(_data)
				if(_data->prev())
					_data = _data->prev();
			return *this;
		}
		inline iterator &operator--(int)
		{
			if(_data)
				if(_data->prev())
					_data = _data->prev();
			return *this;
		}
		inline iterator &operator+=(ustype pos)
		{
			if(_data)
				for(ustype i = 0; i < pos; i++, (*this)++)
					if(!_data->next())
						break;
			return *this;
		}
		inline bool operator==(iterator &it)
		{
			return _data == it._data;
		}
		/// Condition for using the c++ ranged-for
		inline bool operator!=(iterator &it)
		{
			if(_data && it._data)
			{
				if(!_flag)
				{
					_flag = true;
					return _data != it._data;
				}
				else
				{
					_flag = _data != it._data;
					return true;
				}
			}
			else
			{
				_flag = false;
				return false;
			}
		}
		inline T &operator*()
		{
			if(_data)
				return _data->data();
			else
				return *static_cast<T *>(nullptr);
		}
		inline T operator->()
		{
			return _data->data();
		}
	};
	
	/// Class to iterate through the items of the container.
	class const_iterator final
	{
	private:
		const Node *_data;
		bool _flag;
	public:
		inline const_iterator(const Node *n = nullptr) : _data(n), _flag(true) { }
		inline const_iterator(const const_iterator &it) : _data(it._data) { }
		inline const_iterator(const_iterator &&it) : _data(it._data) { it._data = nullptr; }
		inline ~const_iterator() { }
		
		inline const_iterator &operator=(const const_iterator &it)
		{
			_data = it._data;
			return *this;
		}
		inline const_iterator &operator=(const_iterator &&it)
		{
			_data = it._data;
			it._data = nullptr;
			return *this;
		}
		
		inline operator bool() { return static_cast<bool>(_data); }
		
		inline const_iterator &next()
		{
			static const_iterator it;
			it = _data->next();
			return it;
		}
		inline const_iterator &prev()
		{
			static const_iterator it;
			it = _data->prev();
			return it;
		}
		
		inline const_iterator &operator++()
		{
			if(_data)
				if(_data->next())
					_data = _data->next();
			return *this;
		}
		inline const_iterator &operator++(int)
		{
			if(_data)
				if(_data->next())
					_data = _data->next();
			return *this;
		}
		inline const_iterator &operator--()
		{
			if(_data)
				if(_data->prev())
					_data = _data->prev();
			return *this;
		}
		inline const_iterator &operator--(int)
		{
			if(_data)
				if(_data->prev())
					_data = _data->prev();
			return *this;
		}
		inline const_iterator &operator+=(ustype pos)
		{
			if(_data)
				for(ustype i = 0; i < pos; i++, (*this)++)
					if(!_data->next())
						break;
			return *this;
		}
		inline bool operator==(const_iterator &it)
		{
			return _data == it._data;
		}
		/// Condition for using the c++ ranged-for
		inline bool operator!=(const_iterator &it)
		{
			if(_data && it._data)
			{
				if(!_flag)
				{
					_flag = true;
					return _data != it._data;
				}
				else
				{
					_flag = _data != it._data;
					return true;
				}
			}
			else
			{
				_flag = true;
				return false;
			}
		}
		inline const T &operator*()
		{
			if(_data)
				return _data->data();
			else
				return *static_cast<T *>(nullptr);
		}
		inline T operator->()
		{
			return _data->data();
		}
	};
};

#endif
