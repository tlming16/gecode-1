/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Contributing authors:
 *     Gabor Szokoli <szokoli@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2004, 2005
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

#include <gecode/set/rel-op.hh>

namespace Gecode {
  using namespace Gecode::Set;
  using namespace Gecode::Set::Rel;
  using namespace Gecode::Set::RelOp;

  void
  rel(Home home, SetOpType op, const SetVarArgs& x, SetVar y) {
    GECODE_POST;
    ViewArray<SetView> xa(home,x);
    switch (op) {
    case SOT_UNION:
      GECODE_ES_FAIL((RelOp::UnionN<SetView,SetView>::post(home, xa, y)));
      break;
    case SOT_DUNION:
      GECODE_ES_FAIL(
                     (RelOp::PartitionN<SetView,SetView>::post(home, xa, y)));
      break;
    case SOT_INTER:
      {
        GECODE_ES_FAIL(
                       (RelOp::IntersectionN<SetView,SetView>
                        ::post(home, xa, y)));
      }
      break;
    case SOT_MINUS:
      throw IllegalOperation("Set::rel");
      break;
    default:
      throw UnknownOperation("Set::rel");
    }
  }

  void
  rel(Home home, SetOpType op, const SetVarArgs& x, const IntSet& z, SetVar y) {
    GECODE_POST;
    Set::Limits::check(z, "Set::rel");
    ViewArray<SetView> xa(home,x);
    switch (op) {
    case SOT_UNION:
      GECODE_ES_FAIL((RelOp::UnionN<SetView,SetView>::post(home, xa, z, y)));
      break;
    case SOT_DUNION:
      GECODE_ES_FAIL(
                     (RelOp::PartitionN<SetView,SetView>::post(home, xa, z, y)));
      break;
    case SOT_INTER:
      {
        GECODE_ES_FAIL(
                       (RelOp::IntersectionN<SetView,SetView>
                        ::post(home, xa, z, y)));
      }
      break;
    case SOT_MINUS:
      throw IllegalOperation("Set::rel");
      break;
    default:
      throw UnknownOperation("Set::rel");
    }
  }

}

// STATISTICS: set-post
