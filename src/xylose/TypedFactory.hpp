#ifndef xylose_TypedFactory_hpp
#define xylose_TypedFactory_hpp

#include <xylose/Factory.hpp>

namespace xylose {

/// Implementation of the Factory interface that constructs subclasses of the
/// Factory creation type by passing the construction parameter into the
/// constructor of the subclass. This can be used to quickly define factories
/// that create a type parameterized by a single constructor input.
///
/// @param T The type of object that is created.
/// @param DerivedT The subclass of T to instantiate.
/// @param ParamT The type that parameterized the creation of the objects and is
/// passed into the constructor of DerivedT.
template< typename T, typename DerivedT, typename ParamT >
class TypedFactory : public Factory< T, ParamT > {
public:
  typedef T ValueType;
  typedef T* Pointer;
  typedef DerivedT DerivedType;
  typedef ParamT ParameterType;

  TypedFactory() {}
  virtual ~TypedFactory() {}

  /// Construct an object of DerivedType by passing a ParameterType into the
  /// DerivedType constructor. This will allocate an object on the heap. The
  /// caller must take ownership of the returned pointer.
  virtual Pointer create( const ParameterType& parameter ) {
    return new DerivedType( parameter );
  }
};

} // namespace xylose

#endif // xylose_TypedFactory_hpp

