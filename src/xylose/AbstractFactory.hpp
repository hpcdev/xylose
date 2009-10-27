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

