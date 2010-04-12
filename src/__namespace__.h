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


/** \file
 * Dummy file for documenting some of the general namespaces. 
 */

#ifndef xylose___namespace___h
#define xylose___namespace___h

/** The Xylose library:  a collection of useful computational tools. */
namespace xylose {

  /** Collection of random number generators that fit a common interface.
   * The generators here are suitable for different applications.  Some require
   * only a small memory foot print due to small state vectors and some require
   * a much larger memory foot print while also providing much higher quality
   * generators (think Mersenne Twister)*/
  namespace random { }

  /** Simple, C++, context-aware, XML representation to greatly ease the
   * use/integration of XML::XPath, XML::XInclude, and libxml2. */
  namespace xml { }
}

#endif // xylose___namespace___h
