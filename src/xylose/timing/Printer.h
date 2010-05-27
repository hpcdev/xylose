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


#ifndef xylose_timing_Printer_h
#define xylose_timing_Printer_h

#include <xylose/timing/Timing.h>

#include <string>
#include <fstream>
#include <ostream>
#include <vector>
#include <limits>


namespace xylose {
  namespace timing {

    /** Generic timing print class.  
     * This class prints a set of timings out to file.
     *
     * @see Timing
     * @see TimingElement
     */
    struct Printer {
      /* MEMBER STORAGE */
      std::vector<Timing *> timers;


      /* MEMBER FUNCTIONS */

      /** Open a new file and print to it. */
      void print( const std::string & filename,
                  const double & ti,
                  const double & dt,
                  const double & tf ) {
        std::ofstream fout( filename.c_str() );
        print( fout, ti, dt, tf );
        fout.close();
      }

      /** Print to an output stream. */
      std::ostream & print( std::ostream & out,
                            const double & ti,
                            const double & dt,
                            const double & tf ) {

        /* save old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
          timers[i]->push_time();
        }

        double t_max = tf * (1. + 10. * std::numeric_limits<double>::epsilon());
        for (double t = ti; t <= t_max ; t+=dt) {
          out << t << '\t';

          for (unsigned int i = 0; i < timers.size(); i++) {
            timers[i]->set_time(t);
            out << timers[i]->getVal() << '\t';
          }

          out << '\n';
        }

        /* restore old timing values. */
        for (unsigned int i = 0; i < timers.size(); i++) {
          timers[i]->pop_time();
        }

        return out;
      }
    };

  }/* namespace xylose::timing */
}/* namespace xylose */

#endif // xylose_timing_Printer_h
