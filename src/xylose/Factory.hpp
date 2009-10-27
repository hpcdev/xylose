#ifndef xylose_Factory_hpp
#define xylose_Factory_hpp

namespace xylose {

/// Interface for constructing objects of a specified type with a single
/// parameter for input.
/// @param T The type of object that is constructed.
/// @param ParamT The type that parameterizes the creation of the objects.
template< typename T, typename ParamT >
class Factory {
public:
  
  typedef T ValueType;
  typedef T* Pointer;
  typedef ParamT ParameterType;

  virtual ~Factory() {}

  /// Pure virtual method for constructing objects given the input parameter.
  /// Inheritors should implement this to create the specific object that uses
  /// the parameter to determine construction options.
  /// @param parameter Parameter that controls the instantiation of specific
  /// objects.
  virtual Pointer create( const ParameterType& parameter ) = 0;

protected:
  Factory() {}
};

} // namespace xylose

#endif // xylose_Factory_hpp

