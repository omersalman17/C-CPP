/**
 * @file HashMap.hpp
 * @author  Omer Salman <omer.salman@mail.huji.ac.il>
 * @version 1.0
 * @date 19 Jan 2020
 *
 * @brief class declaration and implementation of HashMap data structor.
 */

// ------------------------------ includes ------------------------------
#include <vector>

// ------------------------------ HashMap Class Declaration ------------------------------
/**
 * template class presents HashMap/
 * @tparam KeyT type of keys objects in the hash map
 * @tparam ValueT type of value objects in the hash map
 */
template<class KeyT, class ValueT>
class HashMap
{
private:
    int hashMapCapacity;
    std::vector<std::pair<KeyT, ValueT>> *vectorsArray;
    int numOfElements;
    double lowerLoadFactor;
    double upperLoadFactor;

    /**
     * hash function for hash map keys.
     * @param key hash map key.
     * @return int presents the index of the key in the hash map
     */
    int _hashFunction(KeyT key) const;

    /**
     * function gets a key to search and the hash map's vector's index the key is in  and returns it's index inside
     * the vector.
     * @param suitedVectorIndex the hash map's vector index which the key is in
     * @param keyToSearch the key to search
     * @return the key's index in the vector
     */
    int _getElemIndexInSuitedVec(int suitedVectorIndex, KeyT keyToSearch) const;

    /**
     * funciton rehashes the hash map
     * @param newSize new size of the hash map
     */
    void _rehashing(int newSize);

public:
    // -------------------------- nested const_iterator class -------------------------
    /**
     * nested class presents const iterator for the hash map object
     */
    class const_iterator
    {
    private:
        std::vector<std::pair<KeyT, ValueT>> *vectorsArray;
        int capacity;
        int currentVectorIndex;
        int pairIndexInVector;
        std::pair<KeyT, ValueT> const *_pointer = nullptr;

        /**
         * function advances the interator for the next object on the hash map and update the iterator's data members
         * values accordingly.
         */
        void _updateValuesToNextObj();

    public:
        typedef int difference_type;
        typedef std::pair<KeyT, ValueT> value_type;
        typedef std::pair<KeyT, ValueT> *pointer;
        typedef std::pair<KeyT, ValueT> &reference;
        typedef std::forward_iterator_tag iterator_category;

        // ----------------- const_iterator constructors ----------------
        /**
         * constructor for const_iterator object which gets a vectors array of pair objects and it's capacity and a
         * flag for construction of iterator for the end of the objects in the vectors array and constructs a new
         * const_iterator for objects in the vectors array.
         * @param vectorsArrToCpy vectors array of pair objects to construct const_iterator for its objects.
         * @param capacity vectors array capacity (number of vectors in the array).
         * @param isEnd boolean flag for construction of cons_iterator to point to the end of the objects in the
         * vectors array.
         */
        explicit const_iterator(std::vector<std::pair<KeyT, ValueT>> *vectorsArrToCpy, int capacity,
                                bool isEnd = false) : vectorsArray(vectorsArrToCpy), capacity(capacity),
                currentVectorIndex(0),
                pairIndexInVector(-1)
        {
            if (isEnd)
            {
                _pointer = nullptr;
                return;
            }
            _updateValuesToNextObj();
        }

        /**
         * copy constructor for const_iterator object
         * @param constIteratorToCpy const_iterator object to copy it"s values
         */
        const_iterator(const_iterator const &constIteratorToCpy) : vectorsArray(constIteratorToCpy.vectorsArray),
                capacity(constIteratorToCpy.capacity),
                currentVectorIndex(
                        constIteratorToCpy.currentVectorIndex),
                pairIndexInVector(
                        constIteratorToCpy.pairIndexInVector),
                _pointer(constIteratorToCpy._pointer)
        {
        };

        // ----------------- const_iterator operators ----------------
        /**
         * operator -> overloading implementation
         * @return const pointer to pair object which the const_operator currently pointing to.
         */
        std::pair<KeyT, ValueT> const *operator->() const
        {
            return _pointer;
        }

        /**
         * operator * overloading implementation
         * @return const reference for the pair object the const_operator currently pointing to.
         */
        std::pair<KeyT, ValueT> const &operator*() const
        {
            return *_pointer;
        }

        /**
         * operator ++ overloading implementation. advance the iterator to point for the next pair object in the
         * vectors array.
         * @return reference for the current const_operator object after it advanced to point to the next pair
         * object in the vectors array.
         */
        const_iterator &operator++()
        {
            _updateValuesToNextObj();
            return (*this);
        }

        /**
         * operator ++ overloading implementation. advance the iterator to point for the next pair object in the
         * vectors array adn returns const_operator object which points to the previous pair object in the vectors
         * array.
         * @return const_operator object which points to the previous pair object in the vectors array.
         */
        const_iterator operator++(difference_type)
        {

            const_iterator tmpConstIterator = *this;
            ++(*this);
            return tmpConstIterator;
        }

        /**
         * operator == overloading implementation.
         * @param constIterToCmp const_iterator object to compare to.
         * @return true if both iterators point to the same pair object in the vectors array, false otherwise.
         */
        bool operator==(const_iterator const &constIterToCmp) const
        {
            return (_pointer == constIterToCmp._pointer);
        }

        /**
         * operator != overloading implementation.
         * @param constIterToCmp const_iterator object to compare to.
         * @return true if both iterators dont point to the same pair object in the vectors array, false otherwise.
         */
        bool operator!=(const_iterator const &constIterToCmp) const
        {
            return (_pointer != constIterToCmp._pointer);
        }

        /**
         * operator = overloading implementation.
         * @param constIterToCpy const_iterator object to copy it's values.
         * @return reference to const_iterator object after all it's values were copied from constIterToCpy object
         * values.
         */
        const_iterator &operator=(const_iterator const &constIterToCpy)
        {
            vectorsArray = constIterToCpy.vectorsArray;
            capacity = constIterToCpy.capacity;
            currentVectorIndex = constIterToCpy.currentVectorIndex;
            pairIndexInVector = constIterToCpy.pairIndexInVector;
            _pointer = constIterToCpy._pointer;
            return *this;
        }
    };

    // ----------------- HashMap constructors and destructor ----------------
    /**
     * constructor for hash map object
     */
    HashMap() : hashMapCapacity(16), vectorsArray(new std::vector<std::pair<KeyT, ValueT>>[16]), numOfElements(0),
            lowerLoadFactor(0.25), upperLoadFactor(0.75)
    {
    };

    /**
     * constructor for hash map object which gets two vectors: one of KeyT objects and one of ValueT objects and
     * constructs a new hash map form their values accordingly.
     * @param keysVector vector of KeyT objects
     * @param valuesVector vector of ValueT objects
     */
    HashMap(std::vector<KeyT> keysVector, std::vector<ValueT> valuesVector);

    /**
     * copy constructor for hash map object
     * @param hashMapToCpy hash map object to copy it's values and create a new one using them.
     */
    HashMap(const HashMap<KeyT, ValueT> &hashMapToCpy);

    /**
     * destructor for HashMap object
     */
    ~HashMap();

    // ----------------- HashMap functions ----------------
    /**
     * function returns  the number of pair objects in the hash map.
     * @return int presents the number of pair object in the hash map/
     */
    int size() const;

    /**
     * function returns the number of vectors in the hash map's vectors array
     * @return the number of vectors in the hash map's vectors array
     */
    int capacity() const;

    /**
     * function checks if the hash map is empty.
     * @return true if if the hash map is empty, false otherwise.
     */
    bool empty() const;

    /**
     * function inserts a new pair object to the hash map using given key and value
     * (of type KeyT and ValueT accordingly) and returns a boolean value describing if the action ended
     * successfully or not.
     * @param keyToInsert KeyT object to insert to the hash map
     * @param valueToInsert ValueT object to insert to the hash map
     * @return true in case values inserted successfully, false otherwise.
     */
    bool insert(KeyT keyToInsert, ValueT valueToInsert);

    /**
     * function gets a KeyT objects presents a key in the hash map and returns a boolean value describing if the key
     * is in the hash map or not.
     * @param keyToCheck KeyT object to check if it is in the hash map.
     * @return true if the key is in the hash map, false otherwise.
     */
    bool containsKey(KeyT keyToCheck) const;

    /**
     * function gets a KeyT object and returns it's suited ValueT object in the hash map.
     * @param keyToSearch KeyT object to search in the hash map and returns it's ValueT object
     * @return the suited ValueT object of the KeyT object in the hash map. throws std::exception() if the
    * key is not in the hash map.
     */
    ValueT at(const KeyT &keyToSearch) const;

    /**
    * function gets a KeyT object and returns it's suited ValueT object in the hash map.
    * @param keyToSearch KeyT object to search in the hash map and returns it's ValueT object
    * @return reference for the suited ValueT object of the KeyT object in the hash map. throws std::exception() if the
    * key is not in the hash map.
    */
    ValueT &at(const KeyT &keyToSearch);

    /**
     * function gets a KeyT object and erase it and it's suited ValueT object from the hash map
     * @param keyToEraseSuitedValue KeyT object to erase both its and it's suited ValueT object from the hash map
     * @return true if the key and it's value were erased form the hash map successfully, false otherwise.
     */
    bool erase(KeyT keyToEraseSuitedValue);

    /**
     * function returns the load factor of the hash map
     * @return double presents the load factor of the hash map.
     */
    double getLoadFactor() const;

    /**
     * function gets a key and returns it's bucket size in the hash map throws std::exception() if the key is not in
     * the hash map.
     * @param keyToSearchSuitedVector key to return it's bucket size
     * @return the size of the bucket which the key is in
     */
    int bucketSize(KeyT keyToSearchSuitedVector) const;

    /**
     * function gets a key and returns it's bucket index in the hash map throws std::exception() if the key is not in
     * the hash map.
     * @param keyToSearchSuitedVector key to return it's bucket index
     * @return the index of the bucket which the key is in
     */
    int bucketIndex(KeyT keyToSearchSuitedVector) const;

    /**
     * function clears the hash map without changing it's capacity.
     */
    void clear();

    /**
     * function returns a const_iterator object for the beginning of the hash map - points to the first element
     * in the hash map
     * @return const_iterator object for the beginning of the hash map - points to the first element
     * in the hash map
     */
    const_iterator begin() const
    {
        return const_iterator(this->vectorsArray, capacity());
    }

    /**
     * function returns a const_iterator object for the end of the hash map - points to the end of the hash map.
     * @return function returns a const_iterator object for the end of the hash map: points to the end of the hash map.
     */
    const_iterator end() const
    {
        return const_iterator(this->vectorsArray, capacity(), true);
    }

    /**
     * function returns a const_iterator object for the beginning of the hash map - points to the first element
     * in the hash map
     * @return const_iterator object for the beginning of the hash map - points to the first element
     * in the hash map
     */
    const_iterator cbegin() const
    {
        return begin();
    }

    /**
     * function returns a const_iterator object for the end of the hash map - points to the end of the hash map.
     * @return function returns a const_iterator object for the end of the hash map: points to the end of the hash map.
     */
    const_iterator cend() const
    {
        return end();
    }

    // ----------------- HashMap operators ----------------
    /**
     * overloading operator[] implementation. assign all given hash map data members' value to this
     * hash map data members'.
     * @param hashMapToCpyDataFrom hash map object to copy it's data members' values
     * @return reference to hash map object whose dtat member's values are identical to the argument given hash map
     * data members' values.
     */
    HashMap &operator=(const HashMap &hashMapToCpyDataFrom);

    /**
     * overloading operator[] implementation. function gets a key and returns it's value in the hash map.
     * @param keyToGetSuitedValue key to return it's value in the hash map
     * @return reference to ValueT object presents the suited value for the argument given key
     */
    ValueT &operator[](const KeyT &keyToGetSuitedValue);

    /**
     * overloading operator[] implementation. function gets a key and returns it's value in the hash map.
     * @param keyToGetSuitedValue key to return it's value in the hash map
     * @return ValueT object presents the suited value for the argument given key
     */
    ValueT operator[](const KeyT &keyToGetSuitedValue) const;

    /**
     * overloading operator== implementation. function gets a hash map object and returns true if this hash map
     * is identical to the given one and false otherwise.
     * @param hashMapToEqual hash map object to compare this hash map object to.
     * @return true if the hash map's are identical, false otherwise.
     */
    bool operator==(const HashMap &hashMapToEqual) const;

    /**
     * overloading operator== implementation. function gets a hash map object and returns true if this hash map
     * is not identical to the given one and false otherwise.
     * @param hashMapToEqual hash map object to compare this hash map object to.
     * @return true if the hash map's are not identical, false otherwise.
     */
    bool operator!=(const HashMap &hashMapToEqual) const;
};

// --------------------- functions implementation ---------------------
/**
 * hash function for hash map keys.
 * @param key hash map key.
 * @return int presents the index of the key in the hash map
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::_hashFunction(KeyT key) const
{
    int index = std::hash<KeyT>{}(key);
    return index & (hashMapCapacity - 1);
}

/**
 * constructor for hash map object which gets two vectors: one of KeyT objects and one of ValueT objects and
 * constructs a new hash map form their values accordingly.
 * @param keysVector vector of KeyT objects
 * @param valuesVector vector of ValueT objects
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(std::vector<KeyT> keysVector, std::vector<ValueT> valuesVector): HashMap()
{
    if (keysVector.size() != valuesVector.size())
    {
        throw (std::exception());
    }
    for (int i = 0; i < (int) keysVector.size(); i++)
    {
        (*this)[keysVector[i]] = valuesVector[i];
    }
}

/**
 * copy constructor for hash map object
 * @param hashMapToCpy hash map object to copy it's values and create a new one using them.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(const HashMap<KeyT, ValueT> &hashMapToCpy):
        hashMapCapacity(hashMapToCpy.hashMapCapacity),
        vectorsArray(new std::vector<std::pair<KeyT, ValueT>>[hashMapToCpy.capacity()]),
        numOfElements(hashMapToCpy.numOfElements),
        lowerLoadFactor(hashMapToCpy.lowerLoadFactor), upperLoadFactor(hashMapToCpy.upperLoadFactor)
{
    for (int i = 0; i < hashMapToCpy.hashMapCapacity; i++)
    {
        vectorsArray[i] = hashMapToCpy.vectorsArray[i];
    }

}

/**
 * destructor for HashMap object
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::~HashMap()
{
    delete[] vectorsArray;
}

/**
 * function returns  the number of pair objects in the hash map.
 * @return int presents the number of pair object in the hash map/
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::size() const
{
    return numOfElements;
}

/**
 * function returns the number of vectors in the hash map's vectors array
 * @return the number of vectors in the hash map's vectors array
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::capacity() const
{
    return hashMapCapacity;
}

/**
 * function checks if the hash map is empty.
 * @return true if if the hash map is empty, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::empty() const
{
    return (numOfElements == 0);
}

/**
 * function inserts a new pair object to the hash map using given key and value
 * (of type KeyT and ValueT accordingly) and returns a boolean value describing if the action ended
 * successfully or not.
 * @param keyToInsert KeyT object to insert to the hash map
 * @param valueToInsert ValueT object to insert to the hash map
 * @return true in case values inserted successfully, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(KeyT keyToInsert, ValueT valueToInsert)
{
    if (containsKey(keyToInsert))
    {
        return false;
    }
    (*this)[keyToInsert] = valueToInsert;
    return true;
}

/**
 * funciton rehashes the hash map
 * @param newSize new size of the hash map
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_rehashing(int newSize)
{
    int oldCap = hashMapCapacity;
    hashMapCapacity = newSize;
    auto tmp = new std::vector<std::pair<KeyT, ValueT>>[newSize];
    for (int i = 0; i < oldCap; i++)
    {
        for (unsigned long j = 0; j < vectorsArray[i].size(); j++)
        {
            std::pair<KeyT, ValueT> currentPair = vectorsArray[i][j];
            int newSuitedVectorIndex = _hashFunction(currentPair.first);
            tmp[newSuitedVectorIndex].push_back(currentPair);
        }
    }
    delete[] vectorsArray;
    vectorsArray = tmp;
}

/**
 * function gets a KeyT objects presents a key in the hash map and returns a boolean value describing if the key
 * is in the hash map or not.
 * @param keyToCheck KeyT object to check if it is in the hash map.
 * @return true if the key is in the hash map, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(KeyT keyToCheck) const
{
    int suitedVectorIndex = _hashFunction(keyToCheck);
    int elemIndexInSuitedVec = _getElemIndexInSuitedVec(suitedVectorIndex, keyToCheck);
    if (elemIndexInSuitedVec == -1)
    {
        return false;
    }
    return true;
}

/**
 * function gets a KeyT object and returns it's suited ValueT object in the hash map. throws std::exception() if the
 * key is not in the hash map.
 * @param keyToSearch KeyT object to search in the hash map and returns it's ValueT object
 * @return the suited ValueT object of the KeyT object in the hash map. throws std::exception() if the
 * key is not in the hash map.
 */
template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::at(const KeyT &keyToSearch) const
{
    if (!containsKey(keyToSearch))
    {
        throw (std::exception());
    }
    return (*this)[keyToSearch];
}

/**
* function gets a KeyT object and returns it's suited ValueT object in the hash map. throws std::exception() if the
* key is not in the hash map.
* @param keyToSearch KeyT object to search in the hash map and returns it's ValueT object
* @return reference for the suited ValueT object of the KeyT object in the hash map. throws std::exception() if the
 * key is not in the hash map.
*/
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::at(const KeyT &keyToSearch)
{
    if (!containsKey(keyToSearch))
    {
        throw (std::exception());
    }
    return (*this)[keyToSearch];
}

/**
 * function gets a KeyT object and erase it and it's suited ValueT object from the hash map
 * @param keyToEraseSuitedValue KeyT object to erase both its and it's suited ValueT object from the hash map
 * @return true if the key and it's value were erased form the hash map successfully, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(KeyT keyToEraseSuitedValue)
{
    if (!containsKey(keyToEraseSuitedValue))
    {
        return false;
    }
    int suitedVectorIndex = _hashFunction(keyToEraseSuitedValue);
    for (auto it = vectorsArray[suitedVectorIndex].begin(); it != vectorsArray[suitedVectorIndex].end(); it++)
    {
        if ((*it).first == keyToEraseSuitedValue)
        {
            (vectorsArray[suitedVectorIndex]).erase(it);
            numOfElements--;
            while (hashMapCapacity > 1 && getLoadFactor() < lowerLoadFactor)
            {
                _rehashing(hashMapCapacity / 2);
            }
            break;
        }
    }
    return true;
}

/**
 * function gets a key to search and the hash map's vector's index the key is in  and returns it's index inside
 * the vector.
 * @param suitedVectorIndex the hash map's vector index which the key is in
 * @param keyToSearch the key to search
 * @return the key's index in the vector
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::_getElemIndexInSuitedVec(int suitedVectorIndex, KeyT keyToSearch) const
{
    for (int i = 0; i < (int) vectorsArray[suitedVectorIndex].size(); i++)
    {
        if ((vectorsArray[suitedVectorIndex])[i].first == keyToSearch)
        {
            return i;
        }
    }
    return -1; // error indicator - value not found
}

/**
 * function returns the load factor of the hash map
 * @return double presents the load factor of the hash map.
 */
template<class KeyT, class ValueT>
double HashMap<KeyT, ValueT>::getLoadFactor() const
{
    return (double) numOfElements / hashMapCapacity;
}

/**
 * function gets a key and returns it's bucket size in the hash map throws std::exception() if the key is not in
 * the hash map.
 * @param keyToSearchSuitedVector key to return it's bucket size
 * @return the size of the bucket which the key is in
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(KeyT keyToSearchSuitedVector) const
{
    int suitedVectorIndex = bucketIndex(keyToSearchSuitedVector);
    return vectorsArray[suitedVectorIndex].size();
}

/**
 * function gets a key and returns it's bucket index in the hash map. throws std::exception() if the key is not in
 * the hash map.
 * @param keyToSearchSuitedVector key to return it's bucket index
 * @return the index of the bucket which the key is in
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketIndex(KeyT keyToSearchSuitedVector) const
{
    if (!containsKey(keyToSearchSuitedVector))
    {
        throw (std::exception());
    }
    int suitedVectorIndex = _hashFunction(keyToSearchSuitedVector);
    return suitedVectorIndex;
}

/**
 * function clears the hash map without changing it's capacity.
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for (int i = 0; i < hashMapCapacity; i++)
    {
        vectorsArray[i].clear();
    }
    numOfElements = 0;
}

/**
 * overloading operator[] implementation. assign all given hash map data members' value to this
 * hash map data members'.
 * @param hashMapToCpyDataFrom hash map object to copy it's data members' values
 * @return reference to hash map object whose dtat member's values are identical to the argument given hash map
 * data members' values.
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator=(const HashMap &hashMapToCpyDataFrom)
{
    if (&hashMapToCpyDataFrom == this)
    {
        return *this;
    }
    hashMapCapacity = hashMapToCpyDataFrom.hashMapCapacity;
    numOfElements = hashMapToCpyDataFrom.numOfElements;
    lowerLoadFactor = hashMapToCpyDataFrom.lowerLoadFactor;
    upperLoadFactor = hashMapToCpyDataFrom.upperLoadFactor;
    delete[] vectorsArray;
    vectorsArray = new std::vector<std::pair<KeyT, ValueT>>[hashMapCapacity];
    for (int i = 0; i < hashMapToCpyDataFrom.hashMapCapacity; i++)
    {
        vectorsArray[i] = hashMapToCpyDataFrom.vectorsArray[i];
    }
    return *this;
}

/**
 * overloading operator[] implementation. function gets a key and returns it's value in the hash map.
 * @param keyToGetSuitedValue key to return it's value in the hash map
 * @return reference to ValueT object presents the suited value for the argument given key
 */
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &keyToGetSuitedValue)
{
    int suitedVectorIndex = _hashFunction(keyToGetSuitedValue);
    if (!containsKey(keyToGetSuitedValue))
    {
        vectorsArray[suitedVectorIndex].push_back(std::pair<KeyT, ValueT>(keyToGetSuitedValue, ValueT()));
        numOfElements++;
        if (hashMapCapacity > 1 && getLoadFactor() > upperLoadFactor)
        {
            _rehashing(hashMapCapacity * 2);
        }
        suitedVectorIndex = _hashFunction(keyToGetSuitedValue);
    }
    int elemIndexInSuitedVec = _getElemIndexInSuitedVec(suitedVectorIndex, keyToGetSuitedValue);
    return vectorsArray[suitedVectorIndex][elemIndexInSuitedVec].second;
}

/**
 * overloading operator[] implementation. function gets a key and returns it's value in the hash map.
 * @param keyToGetSuitedValue key to return it's value in the hash map
 * @return ValueT object presents the suited value for the argument given key
 */
template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::operator[](const KeyT &keyToGetSuitedValue) const
{
    int suitedVectorIndex = _hashFunction(keyToGetSuitedValue);
    int elemIndexInSuitedVec = _getElemIndexInSuitedVec(suitedVectorIndex, keyToGetSuitedValue);
    // undefined behaviour in case key is not in Hash Map
    std::pair<KeyT, ValueT> currentPair = vectorsArray[suitedVectorIndex][elemIndexInSuitedVec];
    return currentPair.second;
}

/**
 * overloading operator== implementation. function gets a hash map object and returns true if this hash map
 * is identical to the given one and false otherwise.
 * @param hashMapToEqual hash map object to compare this hash map object to.
 * @return true if the hash map's are identical, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap &hashMapToEqual) const
{
    if (numOfElements != hashMapToEqual.numOfElements)
    {
        return false;
    }
    for (auto iter: *this)
    {
        if (!hashMapToEqual.containsKey(iter.first) || hashMapToEqual.at(iter.first) != iter.second)
        {
            return false;
        }
    }
    return true;
}

/**
 * overloading operator== implementation. function gets a hash map object and returns true if this hash map
 * is not identical to the given one and false otherwise.
 * @param hashMapToEqual hash map object to compare this hash map object to.
 * @return true if the hash map's are not identical, false otherwise.
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator!=(const HashMap &hashMapToEqual) const
{
    return (!((*this) == hashMapToEqual));
}

/**
 * function advances the interator for the next object on the hash map and update the iterator's data members
 * values accordingly.
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::const_iterator::_updateValuesToNextObj()
{
    int nextPairIndexInVec = pairIndexInVector + 1;
    if ((int) vectorsArray[currentVectorIndex].size() > nextPairIndexInVec)
    {
        pairIndexInVector = nextPairIndexInVec;
        _pointer = &(vectorsArray[currentVectorIndex][pairIndexInVector]);
    }
    else
    {
        int nextVectorIndexInHashMap = currentVectorIndex + 1;
        for (int i = nextVectorIndexInHashMap; i < capacity; i++)
        {
            if (vectorsArray[i].size() > 0)
            {
                currentVectorIndex = i;
                pairIndexInVector = 0;
                _pointer = &(vectorsArray[currentVectorIndex][pairIndexInVector]);
                return;
            }
        }
        _pointer = nullptr;
    }
}