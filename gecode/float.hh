/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Christian Schulte <schulte@gecode.org>
 *     Guido Tack <tack@gecode.org>
 *     Vincent Barichard <Vincent.Barichard@univ-angers.fr>
 *
 *  Contributing authors:
 *     Mikael Lagerkvist <lagerkvist@gecode.org>
 *     David Rijsman <David.Rijsman@quintiq.com>
 *
 *  Copyright:
 *     David Rijsman, 2009
 *     Mikael Lagerkvist, 2006
 *     Christian Schulte, 2002
 *     Guido Tack, 2004
 *     Vincent Barichard, 2012
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __GECODE_FLOAT_HH__
#define __GECODE_FLOAT_HH__

#include <climits>
#include <cfloat>
#include <iostream>

#include <gecode/kernel.hh>
#include <gecode/int.hh>

/*
 * Configure linking
 *
 */
#if !defined(GECODE_STATIC_LIBS) && \
    (defined(__CYGWIN__) || defined(__MINGW32__) || defined(_MSC_VER))

#ifdef GECODE_BUILD_FLOAT
#define GECODE_FLOAT_EXPORT __declspec( dllexport )
#else
#define GECODE_FLOAT_EXPORT __declspec( dllimport )
#endif

#else

#ifdef GECODE_GCC_HAS_CLASS_VISIBILITY
#define GECODE_FLOAT_EXPORT __attribute__ ((visibility("default")))
#else
#define GECODE_FLOAT_EXPORT
#endif

#endif

// Configure auto-linking
#ifndef GECODE_BUILD_FLOAT
#define GECODE_LIBRARY_NAME "Float"
#include <gecode/support/auto-link.hpp>
#endif

// Include interval implementation
#include <gecode/third-party/boost/numeric/interval.hpp>

/**
 * \namespace Gecode::Float
 * \brief Floating point numbers
 *
 * The Gecode::Float namespace contains all functionality required
 * to program propagators and branchers for floating point numbers.
 * In addition, all propagators and branchers for floating point
 * numbers provided by %Gecode are contained as nested namespaces.
 *
 */

#include <gecode/float/exception.hpp>

namespace Gecode {

  /**
   * \brief Floating point number base type
   *
   * This type defines the interval bounds used for representing floating
   * point values.
   * \ingroup TaskModelFloatVars
   */
  typedef double FloatNum;

}

namespace Gecode { namespace Float {

  /**
   * \brief Floating point rounding policy
   *
   * \ingroup TaskModelFloatVars
   */
  class FullRounding : 
    public boost::numeric::interval_lib::rounded_arith_opp<FloatNum> {
#ifdef GECODE_HAS_MPFR
  public:
    /// Define generic mpfr function
#define GECODE_GENR_FUNC(name)      \
    GECODE_FLOAT_EXPORT double name##_down(FloatNum x); \
    GECODE_FLOAT_EXPORT double name##_up  (FloatNum x);
    GECODE_GENR_FUNC(exp)
    GECODE_GENR_FUNC(log)
    GECODE_GENR_FUNC(sin)
    GECODE_GENR_FUNC(cos)
    GECODE_GENR_FUNC(tan)
    GECODE_GENR_FUNC(asin)
    GECODE_GENR_FUNC(acos)
    GECODE_GENR_FUNC(atan)
    GECODE_GENR_FUNC(sinh)
    GECODE_GENR_FUNC(cosh)
    GECODE_GENR_FUNC(tanh)
    GECODE_GENR_FUNC(asinh)
    GECODE_GENR_FUNC(acosh)
    GECODE_GENR_FUNC(atanh)
#undef GECODE_GENR_FUNC
#endif
  };

  typedef boost::numeric::interval_lib::save_state<FullRounding> R;
  typedef boost::numeric::interval_lib::checking_strict<FloatNum> P;
  typedef boost::numeric::interval<FloatNum, boost::numeric::interval_lib::policies<R, P> > GECODE_FLOAT_FLOATINTERVAL_TYPE;

}};

namespace Gecode {

  /**
   * \brief Float value type
   *
   * \ingroup TaskModelFloatVars
   */
  typedef Gecode::Float::GECODE_FLOAT_FLOATINTERVAL_TYPE FloatVal;

  /**
   * \brief Float value type
   *
   * \ingroup TaskModelFloatVars
   */
  class NewFloatVal {
  protected:
    /// Implementation of float value
    FloatVal x;
    /// Initialize from implementation \a i
    NewFloatVal(const FloatVal& i);
  public:
    /// Default constructor
    NewFloatVal(void);
    /// Initialize with float number \a n
    NewFloatVal(const FloatNum& n);
    /// Initialize with lower bound \a l and upper bound \a u
    NewFloatVal(const FloatNum& l, const FloatNum& u);
    /// Copy constructor
    NewFloatVal(const NewFloatVal& v);

    /// Assignment operator
    NewFloatVal& operator =(const FloatNum& n);
    /// Assignment operator
    NewFloatVal& operator =(const NewFloatVal& v);
    
    //    void assign(FloatNum const &l, FloatNum const &u);
    /// Return lower bound
    const FloatNum& lower(void) const;
    /// Return upper bound
    const FloatNum& upper(void) const;
    
    /// Return empty value
    static NewFloatVal empty(void);
    /// Return whole value
    static NewFloatVal whole(void);
    /// Return hull of \a x and \a y
    static NewFloatVal hull(FloatNum x, FloatNum y);
    
    /// Increment by \a n
    NewFloatVal& operator +=(const FloatNum& n);
    /// Subtract by \a n
    NewFloatVal& operator -=(const FloatNum& n);
    /// Multiply by \a n
    NewFloatVal& operator *=(const FloatNum& n);
    /// Divide by \a n
    NewFloatVal& operator /=(const FloatNum& n);
    /// Increment by \a v
    NewFloatVal& operator +=(const NewFloatVal& v);
    /// Subtract by \a v
    NewFloatVal& operator -=(const NewFloatVal& v);
    /// Multiply by \a v
    NewFloatVal& operator *=(const NewFloatVal& v);
    /// Divide by \a v
    NewFloatVal& operator /=(const NewFloatVal& v);
  };

//   /* arithmetic operators involving intervals */
//   NewFloatVal operator+(const NewFloatVal& x);
//   NewFloatVal operator-(const NewFloatVal& x);

//   NewFloatVal operator+(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal operator+(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal operator+(const FloatNum& x, const NewFloatVal& y);

//   NewFloatVal operator-(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal operator-(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal operator-(const FloatNum& x, const NewFloatVal& y);

//   NewFloatVal operator*(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal operator*(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal operator*(const FloatNum& x, const NewFloatVal& y);

//   NewFloatVal operator/(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal operator/(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal operator/(const FloatNum& r, const NewFloatVal& x);

//   /* algebraic functions: sqrt, abs, square, pow, nth_root */
//   NewFloatVal abs(const NewFloatVal& x);
//   NewFloatVal sqrt(const NewFloatVal& x);
//   NewFloatVal square(const NewFloatVal& x);
//   NewFloatVal pow(const NewFloatVal& x, int y);
//   NewFloatVal nth_root(const NewFloatVal& x, int y);

//   /* transcendental functions: exp, log */
//   NewFloatVal exp(const NewFloatVal& x);
//   NewFloatVal log(const NewFloatVal& x);

//   /* fmod, for trigonometric function argument reduction (see below) */
//   NewFloatVal fmod(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal fmod(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal fmod(const FloatNum& x, const NewFloatVal& y);

//   /* trigonometric functions */
//   NewFloatVal sin(const NewFloatVal& x);
//   NewFloatVal cos(const NewFloatVal& x);
//   NewFloatVal tan(const NewFloatVal& x);
//   NewFloatVal asin(const NewFloatVal& x);
//   NewFloatVal acos(const NewFloatVal& x);
//   NewFloatVal atan(const NewFloatVal& x);

//   /* hyperbolic trigonometric functions */
//   NewFloatVal sinh(const NewFloatVal& x);
//   NewFloatVal cosh(const NewFloatVal& x);
//   NewFloatVal tanh(const NewFloatVal& x);
//   NewFloatVal asinh(const NewFloatVal& x);
//   NewFloatVal acosh(const NewFloatVal& x);
//   NewFloatVal atanh(const NewFloatVal& x);

//   /* min, max external functions (NOT std::min/max, see below) */
//   NewFloatVal max(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal max(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal max(const FloatNum& x, const NewFloatVal& y);
//   NewFloatVal min(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal min(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal min(const FloatNum& x, const NewFloatVal& y);

//   /* bounds-related interval functions */
//   FloatNum lower(const NewFloatVal& x);
//   FloatNum upper(const NewFloatVal& x);
//   FloatNum width(const NewFloatVal& x);
//   FloatNum median(const NewFloatVal& x);
//   FloatNum norm(const NewFloatVal& x);

//   /* bounds-related interval functions */
//   bool empty(const NewFloatVal& b);
//   bool singleton(const NewFloatVal& x);
//   bool equal(const NewFloatVal& x, const NewFloatVal& y);
//   bool in(const FloatNum& r, const NewFloatVal& b);
//   bool zero_in(const NewFloatVal& b);
//   bool subset(const NewFloatVal& a, const NewFloatVal& b);
//   bool proper_subset(const NewFloatVal& a, const NewFloatVal& b);
//   bool overlap(const NewFloatVal& x, const NewFloatVal& y);

//   /* set manipulation interval functions */
//   NewFloatVal intersect(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal hull(const NewFloatVal& x, const NewFloatVal& y);
//   NewFloatVal hull(const NewFloatVal& x, const FloatNum& y);
//   NewFloatVal hull(const FloatNum& x, const NewFloatVal& y);
//   NewFloatVal hull(const FloatNum& x, const FloatNum& y);
//   std::pair<NewFloatVal, NewFloatVal > bisect(const NewFloatVal& x);

//   /* interval comparison operators */
//   bool operator<(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator<(const NewFloatVal& x, const FloatNum& y);
//   bool operator<(const FloatNum& x, const NewFloatVal& y);

//   bool operator<=(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator<=(const NewFloatVal& x, const FloatNum& y);
//   bool operator<=(const FloatNum& x, const NewFloatVal& y);

//   bool operator>(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator>(const NewFloatVal& x, const FloatNum& y);
//   bool operator>(const FloatNum& x, const NewFloatVal& y);

//   bool operator>=(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator>=(const NewFloatVal& x, const FloatNum& y);
//   bool operator>=(const FloatNum& x, const NewFloatVal& y);
//   bool operator==(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator==(const NewFloatVal& x, const FloatNum& y);
//   bool operator==(const FloatNum& x, const NewFloatVal& y);

//   bool operator!=(const NewFloatVal& x, const NewFloatVal& y);
//   bool operator!=(const NewFloatVal& x, const FloatNum& y);
//   bool operator!=(const FloatNum& x, const NewFloatVal& y);


  /**
   * \brief Float rounding mode
   *
   * \ingroup TaskModelFloatVars
   */
  static FloatVal::traits_type::rounding Round;
  
}

#include <gecode/float/val.hpp>

namespace Gecode { namespace Float {

  /**
   * \brief Numerical limits for floating point variables
   *
   * \ingroup TaskModelFloatVars
   */
  namespace Limits {
    /// Largest allowed float value
    const FloatNum max =  std::numeric_limits<FloatNum>::max();
    /// Smallest allowed float value
    const FloatNum min = -max;
    /// Return whether float \a n is a valid number
    bool valid(const FloatVal& n);
    /// Check whether float \a n is a valid number, otherwise throw out of limits exception with information \a l
    void check(const FloatVal& n, const char* l);
  }

}}

#include <gecode/float/limits.hpp>

#include <gecode/float/var-imp.hpp>

namespace Gecode {

  namespace Float {
    class FloatView;
  }

  /**
   * \brief Float variables
   *
   * \ingroup TaskModelFloatVars
   */
  class FloatVar : public VarImpVar<Float::FloatVarImp> {
    friend class FloatVarArray;
    friend class FloatVarArgs;
  private:
    using VarImpVar<Float::FloatVarImp>::x;
    /**
     * \brief Initialize variable with range domain
     *
     * The variable is created with a domain ranging from \a min
     * to \a max. No exceptions are thrown.
     */
    void _init(Space& home, FloatNum min, FloatNum max);
  public:
    /// \name Constructors and initialization
    //@{
    /// Default constructor
    FloatVar(void);
    /// Initialize from float variable \a y
    FloatVar(const FloatVar& y);
    /// Initialize from float view \a y
    FloatVar(const Float::FloatView& y);
    /**
     * \brief Initialize variable with range domain
     *
     * The variable is created with a domain ranging from \a min
     * to \a max. The following exceptions might be thrown:
     *  - If \a min is greater than \a max, an exception of type
     *    Gecode::Float::VariableEmptyDomain is thrown.
     *  - If \a min or \a max exceed the limits for floats as defined
     *    in Gecode::Float::Limits, an exception of type
     *    Gecode::Float::OutOfLimits is thrown.
     */
    GECODE_FLOAT_EXPORT FloatVar(Space& home, FloatNum min, FloatNum max);
    //@}

    /// \name Value access
    //@{
    /// Return domain
    FloatVal domain(void) const;
    /// Return minimum of domain
    FloatNum min(void) const;
    /// Return maximum of domain
    FloatNum max(void) const;
    /// Return median of domain (interval)
    FloatVal med(void) const;
    /// Return median of domain (closest representation)
    FloatNum median(void) const;
    /**
     * \brief Return assigned value
     *
     * Throws an exception of type Float::ValOfUnassignedVar if variable
     * is not yet assigned.
     *
     */
    FloatVal val(void) const;

    /// Return width of domain (distance between maximum and minimum)
    FloatVal width(void) const;
    //@}

    /// \name Domain tests
    //@{
    /// Test whether \a n is contained in domain
    bool in(const FloatVal& n) const;
    //@}
  };

  /**
   * \brief Print float value \a x
   * \relates Gecode::FloatVal
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const FloatVal& x);
  /**
   * \brief Print float variable \a x
   * \relates Gecode::FloatVar
   */
  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const FloatVar& x);
}

#include <gecode/float/view.hpp>
#include <gecode/float/array-traits.hpp>

namespace Gecode {

  /// Passing float arguments
  class FloatArgs : public PrimArgArray<FloatNum> {
  public:
    /// \name Constructors and initialization
    //@{
    /// Allocate empty array
    FloatArgs(void);
    /// Allocate array with \a n elements
    explicit FloatArgs(int n);
    /// Allocate array and copy elements from \a x
    FloatArgs(const SharedArray<FloatNum>& x);
    /// Allocate array and copy elements from \a x
    FloatArgs(const std::vector<FloatNum>& x);
    /// Allocate array with \a n elements and initialize with \a e0, ...
    GECODE_FLOAT_EXPORT
    FloatArgs(int n, int e0, ...);
    /// Allocate array with \a n elements and initialize with elements from array \a e
    FloatArgs(int n, const FloatNum* e);
    /// Initialize from primitive argument array \a a (copy elements)
    FloatArgs(const PrimArgArray<FloatNum>& a);

    /// Allocate array with \a n elements such that for all \f$0\leq i<n: x_i=\text{start}+i\cdot\text{inc}\f$
    static FloatArgs create(int n, FloatNum start, int inc=1);
    //@}
  };

  /// \brief Passing float variables
  class FloatVarArgs : public VarArgArray<FloatVar> {
  public:
    /// \name Constructors and initialization
    //@{
    /// Allocate empty array
    FloatVarArgs(void) {}
    /// Allocate array with \a n elements
    explicit FloatVarArgs(int n) : VarArgArray<FloatVar>(n) {}
    /// Initialize from variable argument array \a a (copy elements)
    FloatVarArgs(const FloatVarArgs& a) : VarArgArray<FloatVar>(a) {}
    /// Initialize from variable array \a a (copy elements)
    FloatVarArgs(const VarArray<FloatVar>& a) : VarArgArray<FloatVar>(a) {}
    /**
     * \brief Initialize array with \a n new variables
     *
     * The variables are created with a domain ranging from \a min
     * to \a max. The following execptions might be thrown:
     *  - If \a min is greater than \a max, an exception of type
     *    Gecode::Float::VariableEmptyDomain is thrown.
     *  - If \a min or \a max exceed the limits for floats as defined
     *    in Gecode::Float::Limits, an exception of type
     *    Gecode::Float::OutOfLimits is thrown.
     */
    GECODE_FLOAT_EXPORT
    FloatVarArgs(Space& home, int n, FloatNum min, FloatNum max);
    //@}
  };
  //@}

  /**
   * \defgroup TaskModelFloatVarArrays Variable arrays
   *
   * Variable arrays can store variables. They are typically used
   * for storing the variables being part of a solution (script). However,
   * they can also be used for temporary purposes (even though
   * memory is not reclaimed until the space it is created for
   * is deleted).
   * \ingroup TaskModelFloat
   */

  /**
   * \brief Float variable array
   * \ingroup TaskModelFloatVarArrays
   */
  class FloatVarArray : public VarArray<FloatVar> {
  public:
    /// \name Creation and initialization
    //@{
    /// Default constructor (array of size 0)
    FloatVarArray(void);
    /// Allocate array for \a n float variables (variables are uninitialized)
    FloatVarArray(Space& home, int n);
    /// Initialize from float variable array \a a (share elements)
    FloatVarArray(const FloatVarArray& a);
    /// Initialize from float variable argument array \a a (copy elements)
    FloatVarArray(Space& home, const FloatVarArgs& a);
    /**
     * \brief Initialize array with \a n new variables
     *
     * The variables are created with a domain ranging from \a min
     * to \a max. The following execptions might be thrown:
     *  - If \a min is greater than \a max, an exception of type
     *    Gecode::Float::VariableEmptyDomain is thrown.
     *  - If \a min or \a max exceed the limits for floats as defined
     *    in Gecode::Float::Limits, an exception of type
     *    Gecode::Float::OutOfLimits is thrown.
     */
    GECODE_FLOAT_EXPORT
    FloatVarArray(Space& home, int n, FloatNum min, FloatNum max);
    //@}
  };

}

#include <gecode/float/array.hpp>

namespace Gecode {

  /**
   * \brief Relation types for floats
   * \ingroup TaskModelFloat
   */
  enum FloatRelType {
    FRT_EQ, ///< Equality (\f$=\f$)
    FRT_LQ, ///< Less or equal (\f$\leq\f$)
    FRT_GQ  ///< Greater or equal (\f$\geq\f$)
  };

  /**
   * \defgroup TaskModelFloatRelFloat Simple relation constraints over float variables
   * \ingroup TaskModelFloat
   */
  /** \brief Post propagator for \f$ x_0 \sim_{frt} x_1\f$
   *
   * \ingroup TaskModelFloatRelFloat
   */
  GECODE_FLOAT_EXPORT void
  rel(Home home, FloatVar x0, FloatRelType frt, FloatVar x1);
  /** \brief Propagates \f$ x \sim_{frt} c\f$
   * \ingroup TaskModelFloatRelFloat
   */
  GECODE_FLOAT_EXPORT void
  rel(Home home, FloatVar x, FloatRelType frt, FloatVal c);
  /** \brief Post propagator for if (b) \f$(x \sim_{frt} c)\equiv r\f$ else \f$(x \sim_{frt} c)\equiv \neg r\f$
   * \ingroup TaskModelFloatRelFloat
   */
  GECODE_FLOAT_EXPORT void
  rel(Home home, FloatVar x, FloatRelType frt, FloatVal c, Reify r, bool b = true);
  /** \brief Post propagator for if (b) \f$(x_0 \sim_{frt} x_1)\equiv r\f$ else \f$(x_0 \sim_{frt} x_1)\equiv \neg r\f$
   * \ingroup TaskModelFloatRelFloat
   */
  GECODE_FLOAT_EXPORT void
  rel(Home home, FloatVar x0, FloatRelType frt, FloatVar x1, Reify r, bool b = true);
  /** \brief Propagates \f$ x_i \sim_{frt} c \f$ for all \f$0\leq i<|x|\f$
   * \ingroup TaskModelFloatRelFloat
   */
  GECODE_FLOAT_EXPORT void
  rel(Home home, const FloatVarArgs& x, FloatRelType frt, FloatVal c);

}


namespace Gecode {

  /**
   * \defgroup TaskModelFloatArith Arithmetic constraints
   * \ingroup TaskModelFloat
   */

  //@{
  /** \brief Post propagator for \f$ \min\{x_0,x_1\}=x_2\f$
   */
  GECODE_FLOAT_EXPORT void
  min(Home home, FloatVar x0, FloatVar x1, FloatVar x2);
  /** \brief Post propagator for \f$ \min x=y\f$
   * If \a x is empty, an exception of type Float::TooFewArguments is thrown.
   */
  GECODE_FLOAT_EXPORT void
  min(Home home, const FloatVarArgs& x, FloatVar y);
  /** \brief Post propagator for \f$ \max\{x_0,x_1\}=x_2\f$
   */
  GECODE_FLOAT_EXPORT void
  max(Home home, FloatVar x0, FloatVar x1, FloatVar x2);
  /** \brief Post propagator for \f$ \max x=y\f$
   * If \a x is empty, an exception of type Float::TooFewArguments is thrown.
   */
  GECODE_FLOAT_EXPORT void
  max(Home home, const FloatVarArgs& x, FloatVar y);

  /** \brief Post propagator for \f$ |x_0|=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  abs(Home home, FloatVar x0, FloatVar x1);

  /** \brief Post propagator for \f$x_0\cdot x_1=x_2\f$
   */
  GECODE_FLOAT_EXPORT void
  mult(Home home, FloatVar x0, FloatVar x1, FloatVar x2);

  /** \brief Post propagator for \f$x_0\cdot x_0=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  sqr(Home home, FloatVar x0, FloatVar x1);

  /** \brief Post propagator for \f$\sqrt{x_0}=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  sqrt(Home home, FloatVar x0, FloatVar x1);

  /** \brief Post propagator for \f${x_0}^{exp}=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  pow(Home home, FloatVar x0, int exp, FloatVar x1);

  /** \brief Post propagator for \f${x_0}^{1/exp}=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  nroot(Home home, FloatVar x0, int exp, FloatVar x1);

  /** \brief Post propagator for \f$x_0\ \mathrm{div}\ x_1=x_2\f$
   */
  GECODE_FLOAT_EXPORT void
  div(Home home, FloatVar x0, FloatVar x1, FloatVar x2);
  //@}

#ifdef GECODE_HAS_MPFR
  /**
   * \defgroup TaskModelFloatTrans Transcendental constraints
   * \ingroup TaskModelFloat
   */

  //@{
  /** \brief Post propagator for \f$ \mathrm{exp}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  exp(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{exp}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  log(Home home, FloatVar x0, FloatVar x1);
  //@}

  /**
   * \defgroup TaskModelFloatTrigo Trigonometric constraints
   * \ingroup TaskModelFloat
   */

  //@{
  /** \brief Post propagator for \f$ \mathrm{asin}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  asin(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{sin}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  sin(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{acos}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  acos(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{cos}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  cos(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{atan}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  atan(Home home, FloatVar x0, FloatVar x1);
  /** \brief Post propagator for \f$ \mathrm{tan}(x_0)=x_1\f$
   */
  GECODE_FLOAT_EXPORT void
  tan(Home home, FloatVar x0, FloatVar x1);
  //@}
#endif

  /**
   * \defgroup TaskModelFloatChannel Channel constraints
   * \ingroup TaskModelFloat
   */
  //@{
  /// Post domain consistent propagator for channeling a Float and an integer variable \f$ x_0 = x_1\f$
  GECODE_FLOAT_EXPORT void
  channel(Home home, FloatVar x0, IntVar x1);
  //@}

  /**
   * \defgroup TaskModelFloatBranch Branching
   * \ingroup TaskModelFloat
   */
  //@{
  /// Recording activities for float variables
  class FloatActivity : public Activity {
  public:
    /**
     * \brief Construct as not yet intialized
     *
     * The only member functions that can be used on a constructed but not
     * yet initialized activity storage is init or the assignment operator.
     *
     */
    FloatActivity(void);
    /// Copy constructor
    FloatActivity(const FloatActivity& a);
    /// Assignment operator
    FloatActivity& operator =(const FloatActivity& a);      
    /// Initialize for integer variables \a x with decay factor \a d
    GECODE_FLOAT_EXPORT 
    FloatActivity(Home home, const FloatVarArgs& x, double d);
    /**
     * \brief Initialize for integer variables \a x with decay factor \a d
     *
     * This member function can only be used once and only if the
     * activity storage has been constructed with the default constructor.
     *
     */
    GECODE_FLOAT_EXPORT void
    init(Home, const FloatVarArgs& x, double d);
  };

  /// Which variable to select for branching
  enum FloatVarBranch {
    FLOAT_VAR_NONE = 0,        ///< First unassigned
    FLOAT_VAR_RND,             ///< Random (uniform, for tie breaking)
    FLOAT_VAR_DEGREE_MIN,      ///< With smallest degree
    FLOAT_VAR_DEGREE_MAX,      ///< With largest degree
    FLOAT_VAR_AFC_MIN,         ///< With smallest accumulated failure count
    FLOAT_VAR_AFC_MAX,         ///< With largest accumulated failure count
    FLOAT_VAR_ACTIVITY_MIN,    ///< With lowest activity
    FLOAT_VAR_ACTIVITY_MAX,    ///< With highest activity
    FLOAT_VAR_MIN_MIN,         ///< With smallest min
    FLOAT_VAR_MIN_MAX,         ///< With largest min
    FLOAT_VAR_MAX_MIN,         ///< With smallest max
    FLOAT_VAR_MAX_MAX,         ///< With largest max
    FLOAT_VAR_WIDTH_MIN,        ///< With smallest domain size
    FLOAT_VAR_WIDTH_MAX,        ///< With largest domain size
    FLOAT_VAR_WIDTH_DEGREE_MIN, ///< With smallest domain size divided by degree
    FLOAT_VAR_WIDTH_DEGREE_MAX, ///< With largest domain size divided by degree
    FLOAT_VAR_WIDTH_AFC_MIN,    ///< With smallest domain size divided by accumulated failure count
    FLOAT_VAR_WIDTH_AFC_MAX,    ///< With largest domain size divided by accumulated failure count
    FLOAT_VAR_WIDTH_ACTIVITY_MIN, ///< With smallest domain size divided by activity
    FLOAT_VAR_WIDTH_ACTIVITY_MAX, ///< With largest domain size divided by activity
  };
  
  /// Which values to select first for branching
  enum FloatValBranch {
    FLOAT_VAL_SPLIT_MIN, ///< Select values not greater than mean of smallest and largest value
    FLOAT_VAL_SPLIT_MAX ///< Select values not smaller than mean of largest and smallest value
  };
  
  /// Branch over \a x with variable selection \a vars and value selection \a vals
  GECODE_FLOAT_EXPORT void
  branch(Home home, const FloatVarArgs& x,
         FloatVarBranch vars, FloatValBranch vals,
         const VarBranchOptions& o_vars = VarBranchOptions::def,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Branch over \a x with tie-breaking variable selection \a vars and value selection \a vals
  GECODE_FLOAT_EXPORT void
  branch(Home home, const FloatVarArgs& x,
         const TieBreakVarBranch<FloatVarBranch>& vars, FloatValBranch vals,
         const TieBreakVarBranchOptions& o_vars = TieBreakVarBranchOptions::def,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
  /// Branch over \a x with value selection \a vals
  GECODE_FLOAT_EXPORT void
  branch(Home home, FloatVar x, FloatValBranch vals,
         const ValBranchOptions& o_vals = ValBranchOptions::def);
  
  //@}
}

#include <gecode/float/activity.hpp>

#endif

// IFDEF: GECODE_HAS_FLOAT_VARS
// STATISTICS: float-post
