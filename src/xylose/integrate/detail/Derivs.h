/*==============================================================================
 * Public Domain Contributions 2010 United States Government                   *
 * as represented by the U.S. Air Force Research Laboratory.                   *
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
 * (derivs) calling wrapper.
 */

#ifndef xylose_integrate_detail_Derivs_h
#define xylose_integrate_detail_Derivs_h

namespace xylose {
  namespace integrate {
    namespace detail {

      struct NoOp { };

      /** Derivative calling wrapper to allow an optional argument.  */
      template < typename Optional >
      struct Derivs {
        template < typename DerivativeFunctor
                   typename Arg0,
                   typename Arg1,
                   typename Arg2,
                   typename Arg3,
                   typename Optional >
        void operator() ( DerviativeFunctor & derivs,
                          const Arg0 & arg0,
                          const Arg1 & arg1,
                          const Arg2 & arg2,
                          const Arg3 & arg3,
                                Optional & opt ) {
          derivs( arg0, arg1, arg2, arg3, opt );
        }
      };

      template<>
      struct Derivs<NoOp> {
        template < typename DerivativeFunctor
                   typename Arg0,
                   typename Arg1,
                   typename Arg2,
                   typename Arg3,
                   typename NoOp >
        void operator() ( DerviativeFunctor & derivs,
                          const Arg0 & arg0,
                          const Arg1 & arg1,
                          const Arg2 & arg2,
                          const Arg3 & arg3,
                          const NoOp & ) {
          derivs( arg0, arg1, arg2, arg3 );
        }
      };

    }/* namespace xylose::integrate::detail */
  }/* namespace xylose::integrate */
}/* namespace xylose */

#endif // xylose_integrate_detail_Derivs_h
