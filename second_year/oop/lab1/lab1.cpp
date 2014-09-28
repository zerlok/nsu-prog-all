#include "table.h"


/* -------------- ACCESSORY FUNCTIONS -------------- */

/* 
	DO NOT CHANGE IT! (while DEBUG it's necessaty for collisions test).
	For example: "Danil" and "Diman" have the same hash.
*/
unsigned int hash(const String& key)
{
	return key.size() * key[0];
}


/*
	Gets key and calculates a big hash number.
*/
int good_hash(const String& key)
{
	int sum, h = 1;
	int len = key.size();

	for (int max_i = 0; max_i < len; max_i++)
	{
		sum = max_i * len;

		for (int i = 0; i < max_i; i++)
		{
			sum += key[i];
		}

		h *= sum;
	}

	return len * h;
}


/* -------------- STUDENT METHODS -------------- */

/*
	Creates new Value object.
*/
Value::Value(const String& name, const unsigned int age,
	const unsigned int course, const float average_mark,
	const String& department)
{
	_name = name;
	_age = age;
	_course = course;
	_average_mark = average_mark;
	_department = department;
}


Value::~Value()
{
	std::cout << "Destoying the Student... ";

	// ...

	std::cout << "done" << std::endl;
}


/*
	Copy the Value object from given Value object.
*/
Value::Value(const Value& value)
{
	_name = value._name;
	_age = value._age;
	_course = value._course;
	_average_mark = value._average_mark;
	_department = value._department;
}


/*
	===May be not necessary operator!===
	Copy the Value object from given Value object.
*/
Value& Value::operator=(const Value& value)
{
	_name = value._name;
	_age = value._age;
	_course = value._course;
	_average_mark = value._average_mark;
	_department = value._department;
}


/*
	Checks the equal between two Value objects.
	Returns: is all fields are equal in both Value objects.
*/
bool operator==(const Value& value1, const Value& value2)
{
	return (
			value1._name == value2._name &&
			value1._age == value2._age &&
			value1._course == value2._course &&
			value1._average_mark == value2._average_mark &&
			value1._department == value2._department
	);
}


/*
	Checks the difference between two Value objects.
	Returns: is not equal objects.
*/
bool operator!=(const Value& value1, const Value& value2)
{
	return !(value1 == value2);
}


/*
	Converts Value fields to string.
	Returns: string about Value object.
	For student example: "<name>:(<age>,<course>,<average_mark>,<department>)".
*/
String Value::as_string() const
{
	// std::stringstream str_stream;
	
	// str_stream << _name << ":("
	// 			<< _age << ","
	// 			<< _course << ","
	// 			<< _average_mark << ","
	// 			<< _department << ")";

	// return str_stream.str();
	return "<student>";
}


/* -------------- ITEM METHODS -------------- */

/*
	Creates an empty Item object with specified key.
*/
Item::Item(const String& key)
{
	_key = key;
	_value = NULL;
	_next = NULL;
}


/*
	Creates an Item object with specified Value object.
	Key generates from Value (for example the student name is a key).
*/
Item::Item(const Value& value)
{
	_key = value.get_name();
	_value = const_cast<Value*>(&value); // TODO: remove cons_cast!
	_next = NULL;
}


/*
	Creates an Item object with specified key and Value object.
*/
Item::Item(const String& key, const Value& value)
{
	_key = key;
	_value = const_cast<Value*>(&value); // TODO: remove const_cast!
	_next = NULL;
}


// TODO: use delete!
Item::~Item()
{
	std::cout << "Destoying the Item... ";

	// if (value) delete value;
	// std::cout << "... ";

	// if (next) delete next;
	/* When next not null, raises "double free corruption" */

	std::cout << "done" << std::endl;
}


/*
	Copy Item object from given Item object.
*/
Item::Item(const Item& item) // TODO: copy (no linking).
{
	_key = item._key;
	_value = new Value(*(item._value));
	_next = item._next;
}


/*
	Returns: Value object from Item object field.
	If Item object is empty, raises an exception.
*/
Value& Item::get_value()
{
	if (_value == NULL)
	{
		throw std::exception(); // TODO: raise an bad_argument_exception!
	}

	return *(_value);
}


/*
	Returns: the pointer to the next Value object in list of Value objects.
*/
Item *Item::get_next() const
{
	return _next;
}


/*
	===May be not necessary===
	Links the given Item object with current.
*/
void Item::set_next(Item *item) // TODO: reference to the item object!
{
	_next = item;
}


/*
	Sets fields of Item object from given Item object.
*/
Item& Item::operator=(const Item& item)
{
	_key = item._key;
	_value = item._value;
	_next = item._next;
}


/*
	Checks is two Item object are equal.
	Returns: is keys are equal.
*/
bool operator==(const Item& value1, const Item& value2)
{
	return (value1._key == value2._key);
}


/*
	Checks is two Item objects are NOT equal.
	Returns: is keys are NOT equal.
*/
bool operator!=(const Item& value1, const Item& value2)
{
	return (value1._key != value2._key);
}


/*
	Pushes back an Item object to the list of Item objects.
	Fails if current Item object and the given Item object are equal.
*/
bool Item::push_back(Item& item)
{
	if (*this == item)
	{
		return false;
	}

	_next = &(item);
	return true;
}


/*
	Checks the current object is empty (value field not specified).
	Returns: is value pointer a NULL.
*/
bool Item::is_empty() const
{
	return (_value == NULL);
}


/*
	Pushes the list of Item objects from given Item objects to the current
	Item object.
*/
bool Item::push_chain(Item& item)
{
	Item *own_item = this, *curr_item = item.get_next();

	while (own_item->_next != NULL)
	{
		own_item = own_item->_next;
	}

	while (curr_item != NULL)
	{
		own_item->_next = new Item(curr_item->get_value());
		curr_item = curr_item->get_next();
	}

	own_item->_next = NULL;

	return true;
}


/* -------------- HASHTABLE METHODS -------------- */

HashTable::HashTable(int mem)
{
	_data_end = mem;

	try
	{
		_data = new Item*[mem];
	}
	catch (std::bad_alloc)
	{
		std::cout << ERR_BAD_ALLOC << std::endl;
	}

	clear();
}


HashTable::~HashTable()
{
	// std::cout << "Destroying the HashTable... ";
	delete[] _data;
	// std::cout << "done" << std::endl;
}


HashTable::HashTable(const HashTable& hashtable)
{
	_data_end = hashtable._data_end;

	try
	{
		_data = new Item*[_data_end];
	}
	catch (std::bad_alloc)
	{
		std::cout << ERR_BAD_ALLOC << std::endl;
	}

	for (int i = 0; i < _data_end; i++)
	{
		if (hashtable._data[i] != NULL)
		{
			_data[i] = new Item(*(hashtable._data[i]));
			_data[i]->push_chain(*(hashtable._data[i]));
		}
		else
		{
			_data[i] = NULL;
		}
	}
}


Value& HashTable::at(const String& key)
{
	Item *curr_item = _data[get_index(key)];

	while (curr_item != NULL && curr_item->get_key() != key)
	{
		curr_item = curr_item->get_next();
	}

	if (curr_item == NULL)
	{
		throw std::invalid_argument(ERR_KEY_NOT_FOUND);
	}

	return curr_item->get_value();
}


const Value& HashTable::at(const String& key) const
{
	Item *curr_item = _data[get_index(key)];

	while (curr_item != NULL && curr_item->get_key() != key)
	{
		curr_item = curr_item->get_next();
	}
	
	if (curr_item == NULL)
	{
		throw std::exception();
	}

	return curr_item->get_value();
}


size_t HashTable::size() const
{
	size_t items_num = 0;
	Item *curr_item;

	for (int i = 0; i < _data_end; i++)
	{
		curr_item = _data[i];

		while (curr_item != NULL)
		{
			items_num++;
			curr_item = curr_item->get_next();
		}
	}

	return items_num;
}


bool HashTable::empty() const
{
	for (int i = 0; i < _data_end; i++)
	{
		if (_data[i] != NULL) { return false; }
	}

	return true;
}


Value& HashTable::operator[](const String& key)
{
	Value *st = new Value(key);
	
	try
	{
		st = &(at(key));
	}
	catch (std::exception)
	{
		insert(key, *(st));
	}
	
	return *(st);
}


HashTable& HashTable::operator=(const HashTable& hashtable)
{
	_data_end = hashtable._data_end;

	try
	{
		delete[] _data; // TODO: call clear method!
		_data = new Item*[_data_end];
	}
	catch (std::bad_alloc)
	{
		std::cout << ERR_BAD_ALLOC << std::endl;
	}

	for (int i = 0; i < _data_end; i++)
	{
		_data[i] = hashtable._data[i];
	}
}


bool operator==(const HashTable& hashtable1, const HashTable& hashtable2)
{
	if (hashtable1._data_end == hashtable2._data_end)
	{
		Item *a_item, *b_item;

		for (int i = 0; i < hashtable1._data_end; i++)
		{
			a_item = hashtable1._data[i];
			b_item = hashtable2._data[i];

			while (a_item != NULL && b_item != NULL)
			{
				if (a_item->get_key() != b_item->get_key() ||
					a_item->get_value() != b_item->get_value())
				{
					return false;
				}

				a_item = a_item->get_next();
				b_item = b_item->get_next();
			}

			if ((a_item == NULL && b_item != NULL) ||
				(a_item != NULL && b_item == NULL))
			{
				return false;
			}
		}

		return true;
	}

	return false;
}


bool operator!=(const HashTable& hashtable1, const HashTable& hashtable2)
{
	return !(hashtable1 == hashtable2);
}


void HashTable::swap(HashTable& hashtable)
{
	unsigned int tmp_data_end = _data_end;
	Item **tmp_data = _data;

	_data_end = hashtable._data_end;
	_data = hashtable._data;

	hashtable._data_end = tmp_data_end;
	hashtable._data = tmp_data;	
}


void HashTable::clear()
{
	for (int i = 0; i < _data_end; i++)
	{
		_data[i] = NULL;
	}
}


bool HashTable::erase(const String& key)
{
	int i= get_index(key);
	Item *last_item = _data[i], *curr_item = last_item;

	if (last_item == NULL)
	{
		return false;
	}

	if (last_item->get_key() == key)
	{
		_data[i] = last_item->get_next();
		return true;
	}

	while (curr_item != NULL)
	{
		if (curr_item->get_key() == key)
		{
			last_item->set_next(curr_item->get_next());
			// delete curr_item;

			return true;
		}

		last_item = curr_item;
		curr_item = curr_item->get_next();
	}

	return false;
}


bool HashTable::insert(const String& key, const Value& value)
{
	int i = get_index(key);
	Item *curr_item = _data[i];

	if (curr_item == NULL)
	{
		_data[i] = new Item(key, value);
		return true;
	}

	while (curr_item->get_next() != NULL)
	{
		if (curr_item->get_key() == key)
		{
			return false;
		}

		curr_item = curr_item->get_next();
	}

	if (curr_item->get_key() == key)
	{
		return false;
	}

	curr_item->set_next(new Item(key, value));

	return true;
}


bool HashTable::contains(const String& key) const
{
	try
	{
		at(key);
	}
	catch (std::exception)
	{
		return false;
	}

	return true;
}


int HashTable::get_index(const String& key) const
{
	return (DEBUG) ? (hash(key) % _data_end) : (good_hash(key) % _data_end);
}
