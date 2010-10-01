// -*- c++ -*-
// $Id$
/*@HEADER
 *         olson-tools:  A variety of routines and algorithms that
 *      I've developed and collected over the past few years.  This collection
 *      represents tools that are most useful for scientific and numerical
 *      software.  This software is released under the LGPL license except
 *      otherwise explicitly stated in individual files included in this
 *      package.  Generally, the files in this package are copyrighted by
 *      Spencer Olson--exceptions will be noted.   
 *                 Copyright 2004-2008 Spencer Olson
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *  
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *                                                                                 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.                                                                           .
 * 
 * Questions? Contact Spencer Olson (olsonse@umich.edu) 
 */


#ifndef xylose_timing_element_Base_h
#define xylose_timing_element_Base_h


namespace xylose {
  namespace timing {
    namespace element {

      /** Abstract timing element class. 
       * In general, each timing element only operates in relative time.  An array
       * of these elements will then describe how a particular value (of something)
       * changes over the length of absolute time.  
       *
       * @see Timing
       */
      class Base {
      public:
        /** Length of this time interval. */
        double dt;

        Base( const double & dt = 0.0 ) : dt(dt) { }
        virtual ~Base() {}

        /** return the value of this interval at t_rel relative time.
         * @param t_rel relative time.
         */
        virtual double getValue( const double & t_rel ) = 0;

        /** Create a clone. */
        virtual Base * clone() const = 0;
      };

      inline Base * new_clone( const Base & b ) {
        return b.clone();
      }

    }/* namespace xylose::timing::element */
  }/* namespace xylose::timing */
}/* namespace xylose */

#endif // xylose_timing_element_Base_h
