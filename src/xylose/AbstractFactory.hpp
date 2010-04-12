/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
 * Portions copyright Copyright (C) 2010 Stellar Science                       *
 *                                                                             *
 * This file is part of xylose                                                 *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public        *
 * License for more details.                                                   *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                             *
 -----------------------------------------------------------------------------*/


#ifndef xylose_AbstractFactory_hpp
#define xylose_AbstractFactory_hpp

#include <xylose/Factory.hpp>

#include <algorithm>
#include <map>

namespace xylose {

/// Interface for constructing families of objects with a common base type.
/// The objects are parameterized by both a key type and a parameter type.  The
/// key type is used to select a specific Factory for instantion, while the
/// parameter type specifies the type of parameter to pass to the factory as
/// information for construction.
/// @param KeyT The key type for Factory selection.
/// @param T The type of object to be created.
/// @param ParamT Type of parameter to be passed into object construction.
template< typename KeyT, typename FactoryT >
class AbstractFactory {
private:
  typedef FactoryT FactoryType;
  typedef std::map< KeyT, FactoryType* > FactoryMap;

  FactoryMap mFactories;

public:

  AbstractFactory() {}

  virtual ~AbstractFactory() {
    for ( 
      typename FactoryMap::iterator factory = mFactories.begin();
      factory != mFactories.end();
      ++factory ) {
      delete factory->second;
    }
  }

  /// Register a single factory with the given key.  If a Factory with the given
  /// key already exists, it is replaced.  The abstract factory takes ownership.
  virtual void registerFactory( const KeyT& key, FactoryType* factory ) {
    typename FactoryMap::iterator location = mFactories.find( key );
    if ( location != mFactories.end() ) {
      delete location->second;
    }
    mFactories[key] = factory;
  }

  /// Create an object given a key and a construction parameter.  If there is no
  /// factory with the registered key, a NULL pointer is returned.  Otherwise,
  /// the factory's create is called with the given construction parameter.
  virtual typename FactoryType::Pointer create( 
    const KeyT& key, 
    const typename FactoryType::ParameterType parameter ) const {
    typename FactoryMap::const_iterator factory = mFactories.find( key );
    if ( factory == mFactories.end() ) {
      return 0;
    } else {
      return factory->second->create( parameter );
    }
  }

};

} // namespace xylose

#endif // xylose_AbstractFactory_hpp

