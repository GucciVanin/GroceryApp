#include <cmath>                                                      // abs(), pow()
#include <compare>                                                    // weak_ordering
#include <iomanip>                                                    // quoted()
#include <iostream>
#include <string>
#include <type_traits>                                                // is_floating_point_v, common_type_t
#include <utility>                                                    // move()

#include "GroceryItem.hpp"



/*******************************************************************************
**  Implementation of non-member private types, objects, and functions
*******************************************************************************/
namespace    // unnamed, anonymous namespace
{
  // Avoid direct equality comparisons on floating point numbers. Two values are equal if they are "close enough", which is
  // represented by Epsilon.  Usually, this is a pretty small number, but since we are dealing with money (only two, maybe three
  // decimal places) we can be a bit more tolerant.  Two floating point values are considered equal if they are within EPSILON of
  // each other.
  template< typename T,  typename U >   requires std::is_floating_point_v<std::common_type_t<T, U> >
  constexpr bool floating_point_is_equal( T const lhs,  U const rhs,  double const EPSILON = 1e-4 ) noexcept
  {
    ///////////////////////// TO-DO (1) //////////////////////////////
      ///  Write the lines of code that compare two floating point numbers.  Return true when the left hand side (lhs) and the right
      ///  hand side (rhs) are within Epsilon, and false otherwise.
      ///
      ///  See: "Floating point equality" in https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/
      ///
      ///  Hint:  Avoid writing code that looks like this:
      ///           if( some expression that is true ) return the constant "true"
      ///           else                               return the constant "false"
      ///         for example, avoid:
      ///           if (a < b) return true;
      ///           else       return false;
      ///         do this instead:
      ///           return a < b;
    return std::abs(lhs - rhs) < EPSILON;
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }
}    // unnamed, anonymous namespace







/*******************************************************************************
**  Constructors, assignments, and destructor
*******************************************************************************/

// Default and Conversion Constructor
GroceryItem::GroceryItem( std::string productName, std::string brandName, std::string upcCode, double price )
  /// Copying the parameters into the object's attributes (member variables) "works" but is not correct.  Be sure to move the parameters into the object's attributes
:_productName(std::move(productName)), 
_brandName(std::move(brandName)),
_upcCode(std::move(upcCode)),
_price(price){}




// Copy constructor
GroceryItem::GroceryItem( GroceryItem const & other )
:_productName(other._productName),
_brandName(other._brandName),
_upcCode(other._upcCode),
_price(other._price){}




// Move constructor
GroceryItem::GroceryItem( GroceryItem && other ) noexcept
:_productName(std::move(other._productName)),
_brandName(std::move(other._brandName)),
_upcCode(std::move(other._upcCode)),
_price(std::move(other._price)){}




// Copy Assignment Operator
GroceryItem & GroceryItem::operator=( GroceryItem const & rhs ) &
{
  this->_productName = rhs._productName;
  this->_brandName = rhs._brandName;
  this->_upcCode = rhs._upcCode;
  this->_price = rhs._price;

  return *this;
}




// Move Assignment Operator
GroceryItem & GroceryItem::operator=( GroceryItem && rhs ) & noexcept
{
  this->_productName = std::move(rhs._productName);
  this->_brandName = std::move(rhs._brandName);
  this->_upcCode = std::move(rhs._upcCode);  
  this->_price = std::move(rhs._price);

  return *this;
}



// Destructor
GroceryItem::~GroceryItem() noexcept
{}








/*******************************************************************************
**  Accessors
*******************************************************************************/

// upcCode() const
std::string const & GroceryItem::upcCode() const &
{
return this->_upcCode;
}




// brandName() const
std::string const & GroceryItem::brandName() const &
{
return this->_brandName;
}




// productName() const
std::string const & GroceryItem::productName() const &
{
return this->_productName;
}



// price() const
double GroceryItem::price() const &
{
return this->_price;
}




// upcCode()
std::string GroceryItem::upcCode() &&
{
return std::move(this->_upcCode);
}




// brandName()
std::string GroceryItem::brandName() &&
{
return std::move(this->_brandName);
}




// productName()
std::string GroceryItem::productName() &&
{
return std::move(this->_productName);
}








/*******************************************************************************
**  Modifiers
*******************************************************************************/

// upcCode()
GroceryItem & GroceryItem::upcCode( std::string newUpcCode ) &
{
    /// Copy assignment "works" but is not correct.  Be sure to move newUpcCode into _upcCode
  this->_upcCode = std::move(newUpcCode);
  return *this;
}




// brandName()
GroceryItem & GroceryItem::brandName( std::string newBrandName ) &
{
this->_brandName = std::move(newBrandName);
return *this;
}




// productName()
GroceryItem & GroceryItem::productName( std::string newProductName ) &
{
this->_productName = std::move(newProductName);
return *this;
}




// price()
GroceryItem & GroceryItem::price( double newPrice ) &
{
this->_price = std::move(newPrice);
return *this;
}








/*******************************************************************************
**  Relational Operators
*******************************************************************************/

// operator<=>
std::weak_ordering GroceryItem::operator<=>( const GroceryItem & rhs ) const noexcept
{
  
  // Grocery items are equal if all attributes are equal (or within Epsilon for floating point numbers, like price). Grocery items are ordered
  // (sorted) by UPC code, product name, brand name, then price.

  if(this->_upcCode <=> rhs._upcCode != 0) return this->_upcCode <=> rhs._upcCode;
  if(this->_productName <=> rhs._productName != 0) return this->_productName <=> rhs._productName;
  if(this->_brandName <=> rhs._brandName != 0) return this->_brandName <=> rhs._brandName;
  

  if(floating_point_is_equal(this->_price, rhs._price)) return std::weak_ordering::equivalent;
  if(this->_price < rhs._price) return std::weak_ordering::less;
  else return std::weak_ordering::greater;
}




// operator==
bool GroceryItem::operator==( const GroceryItem & rhs ) const noexcept
{
  // All attributes must be equal for the two grocery items to be equal to the other.  This can be done in any order, so put the
  // quickest and then the most likely to be different first.

  return this->_productName == rhs._productName && this->_brandName == rhs._brandName && this->_upcCode == rhs._upcCode && floating_point_is_equal(this->_price, rhs._price);
}








/*******************************************************************************
**  Insertion and Extraction Operators
*******************************************************************************/

// operator>>
std::istream & operator>>( std::istream & stream, GroceryItem & groceryItem )
{
  
    /// Assume fields are separated by commas & optional spaces, and string attributes are enclosed with double quotes.  For example:
    ///    UPC Code         | Brand Name | Product Name                                                  | Price
    ///    -----------------+------------+---------------------------------------------------------------+-------
    ///    "00034000020706",  "York",      "York Peppermint Patties Dark Chocolate Covered Snack Size"  ,  12.64
    

  GroceryItem workingItem;
  char delimiter = '\0';

  stream >> std::ws >> std::quoted(workingItem._upcCode);
  if(!stream) return stream;
  stream >> std::ws >> delimiter;
  if(delimiter != ',') return stream;

  if(stream >> std::ws >> std::quoted(workingItem._brandName) >> delimiter && delimiter == ',' && stream >> std::ws >> 
  std::quoted(workingItem._productName ) >> delimiter && delimiter == ',' && stream >> std::ws >> workingItem._price ) groceryItem = std::move(workingItem);
  else stream.setstate(std::ios::failbit);
  return stream;


}




// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryItem & groceryItem )
{

  char delimiter = ',';

  stream << std::quoted(groceryItem._upcCode);
  if(!stream) return stream;
  stream << delimiter << std::quoted(groceryItem._brandName) << delimiter << std::quoted(groceryItem._productName) << delimiter << groceryItem._price;

  return stream;
}
