#include <exception>
#include <iostream>
#include <sstream>                                                                    // istringstream
#include <typeinfo>

#include "GroceryItem.hpp"
#include "GroceryList.hpp"




namespace
{
  void basicScenario()
  {
    // Let's start a grocery list
    GroceryList thingsToBuy = { { "milk"     },
                                { "hot dogs" },
                                { "eggs"     },
                                { "bread"    } };

    // Changed my mind, I want to make sure I can get eggs before running out of money so I'm going to move that to the top of my list
    thingsToBuy.moveToTop( { "eggs" } );

    // Let's see what's on the list so far
    std::cout << "My Grocery List" << thingsToBuy << "\n\n";



    // Hmm ..., no.  Need to add a few more things
    thingsToBuy += { { "bananas" },
                     { "apples"  } };




    // My roommate also has a grocery list
    GroceryList roommatesList = { { "potato chips", "Ruffles"    },
                                  { "potato chips", "Frito Lays" },
                                  { "beer",         "Bud Lite"   },
                                  { "eggs"                       },
                                  { "pretzels"                   } };

    std::cout << "My roommate's Grocery List" << roommatesList << "\n\n";

    // Let's combine the lists (if different) and go shopping
    if( thingsToBuy != roommatesList ) thingsToBuy += roommatesList;


    // Oops, forgot butter.  Let's add that right before bread.
    thingsToBuy.insert( { "butter", "Lakes 'Ole" }, thingsToBuy.find( { "bread" } ) );

    // And beer, really?  You should be studying!
    thingsToBuy.remove( { "beer", "Bud Lite" } );


    // Now let's see what's on the list
    std::cout << "Combined Grocery Lists" << thingsToBuy << "\n\n";

    //                                            UPC | Brand Name   | Product Name   | Price
    //                                            ----+--------------+----------------+--------
    std::istringstream expectedResultsStream( R"( "",   "",            "eggs",          0.0
                                                  "",   "",            "milk",          0.0
                                                  "",   "",            "hot dogs",      0.0
                                                  "",   "Lakes 'Ole",  "butter",        0.0
                                                  "",   "",            "bread",         0.0
                                                  "",   "",            "bananas",       0.0
                                                  "",   "",            "apples",        0.0
                                                  "",   "Ruffles",     "potato chips",  0.0
                                                  "",   "Frito Lays",  "potato chips",  0.0
                                                  "",   "",            "pretzels",      0.0 )"   // multi-line raw string literal of grocery items
                                               );

    GroceryList expectedResults;
    expectedResultsStream >> expectedResults;

    std::cout << "\nExpected results:" << expectedResults << "\n\n"
              << "\nActual results:  " << thingsToBuy     << "\n\n"
              << "\nTest results:    " << ( thingsToBuy == expectedResults ? "PASS" : "FAIL" ) << '\n';
  }
}




int main()
{
  try
  {
    basicScenario();


  // Implement realistic grocery list purchase ->>>
  }

  catch( const std::exception & ex )
  {
    std::cerr << "Fatal Error:  unhanded exception \"" << typeid( ex ).name() << "\"\n"
              << ex.what() << '\n';
  }
}
