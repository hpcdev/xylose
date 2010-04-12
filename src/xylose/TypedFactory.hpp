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

