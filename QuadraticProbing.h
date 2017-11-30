#ifndef _QUADRATIC_PROBING_H_
#define _QUADRATIC_PROBING_H_

#include "vector.h"
#include "DefragRunner.h"
#include "mynew.h"

// QuadraticProbing Hash table class
//
// CONSTRUCTION: an initialization for ITEM_NOT_FOUND
//               and an approximate initial size or default of 101
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// Hashable find( x )     --> Return item that matches x
// void makeEmpty( )      --> Remove all items
// int hash( String str, int tableSize )
//                        --> Static method to hash strings

class HashBlock
{ 

public:
  int newLoc;
  int oldLoc;
  HashBlock() {newLoc=0, oldLoc = 0;}
  bool operator != (const HashBlock block)const
  {
    if(this->oldLoc != block.oldLoc)
     return true;
    else
      return false;
  }

};



template <class HashedObj>
class QuadraticHashTable
{
  public:
    explicit QuadraticHashTable( const HashedObj & notFound, int size = 101 );
    QuadraticHashTable( const QuadraticHashTable & rhs )
      : array( rhs.array), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
        currentSize( rhs.currentSize ) { }

    const HashedObj & find( const HashedObj & x ) const;

    void makeEmpty( );
    void insert( const HashedObj & x);
    void remove( const HashedObj & x );
    const HashedObj &findInt( int x ) const;
    int findPosInt(int x ) const;
    const QuadraticHashTable & operator=( const QuadraticHashTable & rhs );
    bool isActive( int currentPos ) const;

    enum EntryType { ACTIVE, EMPTY, DELETED };
  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
          : element( e ), info( i ) { }
    };

    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;
    bool isPrime( int n ) const;
    int nextPrime( int n ) const;
    int findPos( const HashedObj & x ) const;
    int hash( int key, int tableSize ) const;
    int hash(HashBlock key, int tableSize) const;

    void rehash( );
};

#include "QuadraticProbing.cpp"
#endif