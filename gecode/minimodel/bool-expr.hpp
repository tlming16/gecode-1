/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004
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

namespace Gecode {

  /*
   * Operations for expressions
   *
   */

  forceinline
  BoolExpr::BoolExpr(const BoolExpr& e) : n(e.n) {
    n->use++;
  }

  forceinline BoolVar
  BoolExpr::post(Space& home, IntConLevel icl, PropKind pk) const {
    Region r(home);
    return NNF::nnf(r,n,false)->post(home,icl,pk);
  }

  forceinline void
  BoolExpr::post(Space& home, bool t, IntConLevel icl, PropKind pk) const {
    Region r(home);
    return NNF::nnf(r,n,false)->post(home,t,icl,pk);
  }


  inline BoolExpr
  operator&&(const BoolExpr& l, const BoolExpr& r) {
    return BoolExpr(l,BoolExpr::NT_AND,r);
  }

  inline BoolExpr
  operator||(const BoolExpr& l, const BoolExpr& r) {
    return BoolExpr(l,BoolExpr::NT_OR,r);
  }
  
  inline BoolExpr
  operator^(const BoolExpr& l, const BoolExpr& r) {
    return BoolExpr(BoolExpr(l,BoolExpr::NT_EQV,r),BoolExpr::NT_NOT);
  }
  
  template <class Var>
  inline BoolExpr
  operator~(const LinRel<Var>& rl) {
    return BoolExpr(rl);
  }
  
  inline BoolExpr
  operator!(const BoolExpr& e) {
    return BoolExpr(e,BoolExpr::NT_NOT);
  }


  inline BoolExpr
  eqv(const BoolExpr& l, const BoolExpr& r) {
    return BoolExpr(l, BoolExpr::NT_EQV, r);
  }

  inline BoolExpr
  imp(const BoolExpr& l, const BoolExpr& r) {
    return BoolExpr(BoolExpr(l,BoolExpr::NT_NOT),
                    BoolExpr::NT_OR,r);
  }
  

  inline BoolVar
  post(Space& home, const BoolExpr& e, 
       IntConLevel icl, PropKind pk) {
    return e.post(home,icl,pk);
  }

  inline BoolVar
  post(Space&, const BoolVar& b, IntConLevel, PropKind) {
    return b;
  }


}

// STATISTICS: minimodel-any