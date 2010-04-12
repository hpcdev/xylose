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

