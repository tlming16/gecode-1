/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main author:
 *     Christian Schulte <schulte@gecode.org>
 *
 *  Copyright:
 *     Christian Schulte, 2012
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

  /**
   * \defgroup TaskBranchValSelCommit Generic value selection and value commit for brancher based on view and value selection
   *
   * \ingroup TaskBranchViewVal
   */
  //@{
  /// Base class for value selection and commit
  template<class _View, class _Val>
  class ValSelCommitBase {
  public:
    /// View type
    typedef _View View;
    /// Value type
    typedef _Val Val;
  public:
    /// Constructor for initialization
    ValSelCommitBase(Space& home, const ValBranch& vb);
    /// Constructor for cloning
    ValSelCommitBase(Space& home, bool shared, 
                     ValSelCommitBase<View,Val>& vsc);
    /// Return value of view \a x
    virtual Val val(const Space& home, View x) = NULL;
    /// Commit view \a x to value \a n for alternative \a a
    virtual ModEvent commit(Space& home, unsigned int a, View x, Val n) = NULL;
    /// Perform cloning
    virtual ValSelCommitBase<View,Val>* copy(Space& home, bool shared) = NULL;
    /// Whether dispose must always be called (that is, notice is needed)
    virtual bool notice(void) const = NULL;
    /// Delete value selection
    virtual void dispose(Space& home) = NULL;
    /// \name Memory management
    //@{
    /// Allocate memory from space
    static void* operator new(size_t s, Space& home);
    /// Return memory to space
    static void operator delete(void* p, Space& home);
    /// Needed for exceptions
    static void operator delete(void* p);
    //@}
  };

  /// Class for value selection and commit
  template<class ValSel, class ValCommit>
  class ValSelCommit 
    : public ValSelCommitBase<typename ValSel::View,typename ValSel::Val> {
  protected:
    /// The value selection object used
    ValSel s;
    /// The commit object used
    ValCommit c;
  public:
    /// Constructor for initialization
    ValSelCommit(Space& home, const ValBranch& vb);
    /// Constructor for cloning
    ValSelCommit(Space& home, bool shared, 
                 ValSelCommit<ValSel,ValCommit>& vsc);
    /// Return value of view \a x
    virtual Val val(const Space& home, View x);
    /// Commit view \a x to value \a n for alternative \a a
    virtual ModEvent commit(Space& home, unsigned int a, View x, Val n);
    /// Perform cloning
    virtual ValSelCommit<ValSel,ValCommit>* copy(Space& home, bool shared);
    /// Whether dispose must always be called (that is, notice is needed)
    virtual bool notice(void) const;
    /// Delete value selection
    virtual void dispose(Space& home);
  };
  //@}


  template<class View, class Val>
  forceinline
  ValSelCommitBase<View,Val>::ValSelCommitBase(Space& home, 
                                               const ValBranch& vb) {}
  template<class View, class Val>
  forceinline
  ValSelCommitBase<View,Val>::
    ValSelCommitBase(Space& home, bool shared, 
                     ValSelCommitBase<View,Val>& vsc) {}

  template<class View, class Val>
  forceinline void
  ValSelCommitBase<View,Val>::operator delete(void*) {}
  template<class View, class Val>
  forceinline void
  ValSelCommitBase<View,Val>::operator delete(void*, Space&) {}
  template<class View, class Val>
  forceinline void*
  ValSelCommitBase<View,Val>::operator new(size_t s, Space& home) {
    return home.ralloc(s);
  }




  template<class ValSel, class ValCommit>
  forceinline
  ValSelCommit<ValSel,ValCommit>::ValSelCommit(Space& home, 
                                               const ValBranch& vb) 
    : ValSelCommitBase<View,Val>(home,vb), s(home,vb), c(home,vb) {}

  template<class ValSel, class ValCommit>
  forceinline
  ValSelCommit<ValSel,ValCommit>::ValSelCommit(Space& home, bool shared, 
                                               ValSelCommit<ValSel,ValCommit>& vsc)
    : ValSelCommitBase<View,Val>(home,shared,vsc),
      s(home,shared,vsc.s), c(home,shared,vsc.c) {}

  template<class ValSel, class ValCommit>
  typename ValSelCommit<ValSel,ValCommit>::Val
  ValSelCommit<ValSel,ValCommit>::val(const Space& home, View x) {
    return s.val(home,x);
  }

  template<class ValSel, class ValCommit>
  ModEvent
  ValSelCommit<ValSel,ValCommit>::commit(Space& home, unsigned int a, 
                                         View x, Val n) {
    return c.commit(home,a,x,n);
  }

  template<class ValSel, class ValCommit>
  ValSelCommit<ValSel,ValCommit>*
  ValSelCommit<ValSel,ValCommit>::copy(Space& home, bool shared) {
    return new (home) ValSelCommit<ValSel,ValCommit>(home,shared,*this);
  }

  template<class ValSel, class ValCommit>
  bool
  ValSelCommit<ValSel,ValCommit>::notice(void) const {
    return s.notice() || c.notice();
  }

  template<class ValSel, class ValCommit>
  void
  ValSelCommit<ValSel,ValCommit>::dispose(Space& home) {
    s.dispose(home);
    c.dispose(home);
  }

}

// STATISTICS: kernel-branch