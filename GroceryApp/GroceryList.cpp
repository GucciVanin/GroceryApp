#include <algorithm>                                                                // find(), move(), move_backward(), equal(), swap(), lexicographical_compare()
#include <cmath>                                                                    // min()
#include <cstddef>                                                                  // size_t
#include <initializer_list>
#include <iomanip>                                                                  // setw()
#include <iterator>                                                                 // distance(), next()
#include <stdexcept>                                                                // logic_error
#include <string>

#include "GroceryItem.hpp"
#include "GroceryList.hpp"




#define exception_location "\n detected in function \"" + std::string(__func__) +  "\""    \
                           "\n at line " + std::to_string( __LINE__ ) +                    \
                           "\n in file \"" __FILE__ "\""

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializer List Constructor
GroceryList::GroceryList( const std::initializer_list<GroceryItem> & initList )
{
  for( auto && groceryItem : initList )   insert( groceryItem, Position::BOTTOM );

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size() const
std::size_t GroceryList::size() const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

    /// All the containers are the same size, so pick one and return the size of that.  Since the forward_list has to calculate the
    /// size on demand, stay away from using that one.
  return _gList_array_size;

}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  for (size_t i = 0; i < _gList_array_size; ++i){
    if(_gList_array[i] == groceryItem) return i;
  }

  return _gList_array_size;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifiers
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// insert( position )
void GroceryList::insert( const GroceryItem & groceryItem, Position position )
{
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work
  if     ( position == Position::TOP    )  insert( groceryItem, 0      );
  else if( position == Position::BOTTOM )  insert( groceryItem, size() );
  else                                     throw std::logic_error( "Unexpected insertion position" exception_location );  // Programmer error.  Should never hit this!
}



// insert( offset )
void GroceryList::insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )        // insert provided grocery item at offsetFromTop, which places it before the current grocery item at offsetFromTop
{
  // Validate offset parameter before attempting the insertion.  std::size_t is an unsigned type, so no need to check for negative
  // offsets, and an offset equal to the size of the list says to insert at the end (bottom) of the list.  Anything greater than the
  // current size is an error.
  if( offsetFromTop > size() ){   
    InvalidOffset_Ex( "Insertion position beyond end of current list size" exception_location );
  }

  /**********  Prevent duplicate entries  ***********************/
  

    if(find(groceryItem) != _gList_array_size) return ;


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    

    if(_gList_array_size + 1 == _gList_array.size() ) throw CapacityExceeded_Ex("Cannot fit Another item into fixed size array");

    for(size_t i = _gList_array_size; i > offsetFromTop; --i){
      _gList_array[i] = std::move(_gList_array[i - 1]);
    }
    _gList_array[offsetFromTop] = groceryItem;
    ++_gList_array_size;

  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    
    _gList_vector.insert(_gList_vector.begin() + offsetFromTop, groceryItem);
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    
    _gList_dll.insert(std::next(_gList_dll.begin(), offsetFromTop), groceryItem);
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
   
    _gList_sll.insert_after(std::next(_gList_sll.before_begin(), offsetFromTop), groceryItem);
  } // Part 4 - Insert into singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )



// remove( groceryItem )
void GroceryList::remove( const GroceryItem & groceryItem )
{
  // Delegate to the version of remove() that takes an index as a parameter
  remove( find( groceryItem ) );
}



// remove( offset )
void GroceryList::remove( std::size_t offsetFromTop )
{
  

  if( offsetFromTop >= size() )   return;                                           // no change occurs if (zero-based) offsetFromTop >= size()


  { /**********  Part 1 - Remove from array  ***********************/
    
    for(size_t i = offsetFromTop; i < _gList_array_size - 1; ++i){
      _gList_array.at(i) = _gList_array.at(i + 1);
    }
    --_gList_array_size;
    _gList_array.at(_gList_array_size) = {};
    
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    
    _gList_vector.erase(_gList_vector.begin() + offsetFromTop);
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    
    _gList_dll.erase(std::next(_gList_dll.begin(), offsetFromTop));
  } // Part 3 - Remove from doubly linked list




  {/**********  Part 4 - Remove from singly linked list  **********/
    
    _gList_sll.erase_after(std::next(_gList_sll.before_begin(), offsetFromTop));
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  
  if(find(groceryItem) != _gList_array_size){
    remove(groceryItem);
    insert(groceryItem);
  }
  else return;
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  
  for(GroceryItem grocery : rhs){
    insert(grocery, _gList_array_size + 1);
  }


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  
  for(GroceryItem grocery : rhs._gList_array){
    insert(grocery, Position::BOTTOM);
  }

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Relational Operators
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<=>
std::weak_ordering GroceryList::operator<=>( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  size_t temp = (size()) ? size() >= rhs.size() : rhs.size();

  for(size_t i = 0; i < temp; ++i){
    if(auto result = _gList_array[i] <=> rhs._gList_array[i]; result != 0) return result;
  }

  return size() <=> rhs.size();
  
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  if(_gList_array_size != rhs._gList_array_size) return false;
  for(size_t i = 0; i < _gList_array.size(); ++i){
    if(_gList_array[i] != rhs._gList_array[i]) return false;
  }


  return true;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// containersAreConsistant() const
bool GroceryList::containersAreConsistant() const
{
  // Sizes of all containers must be equal to each other
  if(    _gList_array_size != _gList_vector.size()
      || _gList_array_size != _gList_dll.size()
      || _gList_array_size !=  gList_sll_size() ) return false;

  // Element content and order must be equal to each other
  auto current_array_position   = _gList_array .cbegin();
  auto current_vector_position  = _gList_vector.cbegin();
  auto current_dll_position     = _gList_dll   .cbegin();
  auto current_sll_position     = _gList_sll   .cbegin();

  auto end = _gList_vector.cend();
  while( current_vector_position != end )
  {
    if(    *current_array_position != *current_vector_position
        || *current_array_position != *current_dll_position
        || *current_array_position != *current_sll_position ) return false;

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dll_position;
    ++current_sll_position;
  }

  return true;
}



// gList_sll_size() const
std::size_t GroceryList::gList_sll_size() const
{
  
  return std::distance(_gList_sll.cbegin(), _gList_sll.cend());
  
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  // For each grocery item in the provided grocery list, insert the grocery item into the provided stream.  Each grocery item is
  // inserted on a new line and preceded with its index (aka offset from top)
  unsigned count = 0;
  for( auto && groceryItem : groceryList._gList_sll )   stream << '\n' << std::setw(5) << count++ << ":  " << groceryItem;

  return stream;
}



// operator>>
std::istream & operator>>( std::istream & stream, GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

 
  GroceryItem workingItem;

  while(stream >> workingItem) groceryList.insert(workingItem, GroceryList::Position::BOTTOM);
 

  return stream;
}
