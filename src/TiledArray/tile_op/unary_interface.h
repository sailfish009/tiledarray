/*
 *  This file is a part of TiledArray.
 *  Copyright (C) 2013  Virginia Tech
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  justus
 *  Department of Chemistry, Virginia Tech
 *
 *  unary_interface.h
 *  Oct 7, 2013
 *
 */

#ifndef TILEDARRAY_UNARY_INTERFACE_H__INCLUDED
#define TILEDARRAY_UNARY_INTERFACE_H__INCLUDED

#include <TiledArray/tile_op/type_traits.h>

namespace TiledArray {
  namespace math {

    /// Policy class for binary tile operations

    /// \tparam Op The derived class type to \c UnaryInterface
    /// \note \c Op must be a template class with the following signature:
    /// <tt>template <typename, typename, bool> class Operation;</tt>.
    template <typename Op>
    struct UnaryTileOpPolicy;

    /// Policy class for binary tile operations

    /// \tparam Result The result type
    /// \tparam Arg The argument type
    /// \tparam Consumable A flag that is \c true when the argument is consumable.
    /// \tparam Op The unary tile operation template
    template <typename Result, typename Arg, bool Consumable,
        template <typename, typename, bool> class Op>
    struct UnaryTileOpPolicy<Op<Result, Arg, Consumable> > {
      typedef typename madness::if_c<Consumable, Arg, const Arg>::type &
          argument_type; ///< The argument type
      typedef Result result_type; ///< The result tile type


      typedef std::integral_constant<bool, Consumable && std::is_same<Result,
          Arg>::value> is_consumable; ///< Left is consumable type trait
    }; // struct UnaryTileOpPolicy


    /// Unary tile operation interface base

    /// This base class defines the user interface for unary operations. It
    /// handles tiles, lazy tiles, and runtime consumable resources.
    /// \tparam Derived The derived operation class type
    /// \tparam Consumable A flag that is \c true when the argument is consumable
    template <typename Derived>
    class UnaryInterface {
    public:
      typedef typename UnaryTileOpPolicy<Derived>::argument_type
          argument_type; ///< The argument type
      typedef typename UnaryTileOpPolicy<Derived>::result_type
          result_type; ///< The result tile type


      typedef typename UnaryTileOpPolicy<Derived>::is_consumable
          is_consumable; ///< Left is consumable type trait

    private:

      Permutation perm_; ///< The result permutation

    protected:

      /// Derived type accessor

      /// \return A const reference to the derived object
      const Derived& derived() const { return static_cast<const Derived&>(*this); }

    public:

      /// Default constructor
      UnaryInterface() : perm_() { }

      /// Permution constructor

      /// \param perm The permutation that will be applied in this operation
      explicit UnaryInterface(const Permutation& perm) : perm_(perm) { }

      /// Copy constructor

      /// \param other The object to be copied
      UnaryInterface(const UnaryInterface<Derived>& other) :
        perm_(other.perm_)
      { }

      /// Assignment operator that will be applied in this operation

      /// \param other The object to be copied
      /// \return A reference to this object
      UnaryInterface<Derived>&
      operator=(const UnaryInterface<Derived>& other) {
        perm_ = other.perm_;
        return *this;
      }

      /// Set the permutation that will be applied in this operation

      /// \param perm The permutation that will be applied in this operation
      void permutation(const Permutation& perm) { perm_ = perm; }

      /// Permutation accessor

      /// \return A const reference to this operation's permutation
      const Permutation& permutation() const { return perm_; }


      /// Permutation constructor

      /// \param The permutation that will be

      /// Evaluate non-lazy tile argument

      /// Apply \c Derived class operation to \c arg.
      /// \param arg The argument
      /// \return The result tile with the unary operation (and permuation)
      /// applied to \c arg.
      result_type operator()(argument_type arg) const {
        if(perm_)
          return derived().permute(arg);

        return derived().template no_permute<is_consumable::value>(arg);
      }

      /// Evaluate lazy tile arguments

      /// This function will evaluate the lazy tile and passes the result to the
      /// non-lazy tile interface function.
      /// \tparam A The argument type
      /// \param arg The lazy tile to be evaluated
      /// \return The result tile with the unary operation applied to the
      /// evaluated \c arg
      template <typename A>
      typename madness::enable_if<is_lazy_tile<A>, result_type>::type
      operator()(const A& arg) const {
        typename A::eval_type eval_arg(arg);
        return operator()(eval_arg);
      }

      /// Evaluate runtime consumable tile

      /// Since the tile is always consumable with this specialization, the
      /// runtime consumable parameter is ignored. Instead it is passed to one
      /// of the other evaluation functions.
      /// \tparam A The argument type
      /// \param arg The lazy tile to be evaluated
      /// \return The result tile with the unary operation applied to the
      /// evaluated \c arg
      template <typename A>
      result_type operator()(A& arg, const bool) const {
        return operator()(arg);
      }

    }; // class UnaryInterface

    /// Unary tile operation interface base

    /// This base class defines unary operations with zero or non-zero tiles,
    /// and maps arguments given to the appropriate evaluation kernel.
    /// \tparam Derived The derived operation class type
    template <typename Result, typename Arg,
        template <typename, typename, bool> class Op>
    class UnaryInterface<Op<Result, Arg, false> > {
    public:
      typedef typename UnaryTileOpPolicy<Op<Result, Arg, false> >::argument_type
          argument_type; ///< The argument type
      typedef typename UnaryTileOpPolicy<Op<Result, Arg, false> >::result_type
          result_type; ///< The result tile type

    private:

      Permutation perm_; ///< The result permutation

    protected:

      /// Derived type accessor

      /// \return A const reference to the derived object
      const Op<Result, Arg, false>& derived() const {
        return static_cast<const Op<Result, Arg, false>&>(*this);
      }

    public:

      /// Default constructor
      UnaryInterface() : perm_() { }

      /// Permution constructor

      /// \param perm The permutation that will be applied in this operation
      explicit UnaryInterface(const Permutation& perm) : perm_(perm) { }

      /// Copy constructor

      /// \param other The object to be copied
      UnaryInterface(const UnaryInterface<Op<Result, Arg, false> >& other) :
        perm_(other.perm_)
      { }

      /// Assignment operator that will be applied in this operation

      /// \param other The object to be copied
      /// \return A reference to this object
      UnaryInterface<Op<Result, Arg, false> >&
      operator=(const UnaryInterface<Op<Result, Arg, false> >& other) {
        perm_ = other.perm_;
        return *this;
      }

      /// Set the permutation that will be applied in this operation

      /// \param perm The permutation that will be applied in this operation
      void permutation(const Permutation& perm) { perm_ = perm; }

      /// Permutation accessor

      /// \return A const reference to this operation's permutation
      const Permutation& permutation() const { return perm_; }

      /// Evaluate non-lazy tile argument

      /// Apply \c Derived class operation to \c arg.
      /// \param arg The argument
      /// \return The result tile with the unary operation (and permuation)
      /// applied to \c arg.
      result_type operator()(argument_type arg) const {
        if(perm_)
          return derived().permute(arg);

        return derived().template no_permute<false>(arg);
      }

      /// Evaluate non-array lazy tile arguments

      /// This function will evaluate the lazy tile and passes the result to the
      /// non-lazy tile interface function.
      /// \tparam A The argument type
      /// \param arg The lazy tile to be evaluated
      /// \return The result tile with the unary operation applied to the
      /// evaluated \c arg
      template <typename A>
      typename madness::enable_if<is_non_array_lazy_tile<A>, result_type>::type
      operator()(const A& arg) const {
        typename A::eval_type eval_arg(arg);
        return operator()(eval_arg);
      }

      /// Evaluate non-lazy tile with runtime consumable parameter

      /// Since \c arg is const with this version, runtime consumable parameter
      /// is ignored. Instead the tile is evaluated as a non-consumable tile.
      /// \tparam A The argument type
      /// \param arg The argument
      /// \return The result tile with the unary operation (and permuation)
      /// applied to \c arg.
      template <typename A>
      typename madness::disable_if<is_lazy_tile<A>, result_type>::type
      operator()(const A& arg, const bool) const {
        operator()(arg);
      }

      /// Evaluate non-lazy tile with runtime consumable parameter

      /// If \c consume is \c true, then arg will be evaluated as a consumable
      /// resource. Otherwise, it will not be consumed.
      /// \tparam A The argument type
      /// \param arg The argument
      /// \param consume Consumability flag; true means the tile will be consumed
      /// \return The result tile with the unary operation (and permuation)
      /// applied to \c arg.
      template <typename A>
      typename madness::disable_if<is_lazy_tile<typename std::remove_const<A>::type>,
          result_type>::type
      operator()(A& arg, const bool consume) const {
        if(perm_)
          return derived().permute(arg);

        if(consume)
          return derived().template no_permute<true>(arg);

        return derived().template no_permute<false>(arg);
      }

      /// Evaluate array lazy tile arguments

      /// This function will evaluate the array tile and passes the result to the
      /// non-lazy tile interface function.
      /// \tparam A The argument type
      /// \param arg The array tile to be evaluated
      /// \return The result tile with the unary operation applied to the
      /// evaluated \c arg
      template <typename A>
      typename madness::enable_if<is_array_tile<A>, result_type>::type
      operator()(const A& arg) const {
        typename A::eval_type eval_arg(arg);
        return operator()(eval_arg, arg.is_consumable());
      }

      /// Evaluate non-array lazy tile arguments

      /// This function will evaluate the lazy tile and passes the result to the
      /// non-lazy tile interface function.
      /// \tparam A The argument type
      /// \param arg The lazy tile to be evaluated
      /// \param consume Consumability flag; true means the tile will be consumed
      /// \return The result tile with the unary operation applied to the
      /// evaluated \c arg
      template <typename A>
      typename madness::enable_if<is_non_array_lazy_tile<A>, result_type>::type
      operator()(const A& arg, const bool consume) const {
        typename A::eval_type eval_arg(arg);
        return operator()(eval_arg, consume);
      }

    }; // class UnaryInterface

  }  // namespace math
} // namespace TiledArray

#endif // TILEDARRAY_UNARY_INTERFACE_H__INCLUDED
