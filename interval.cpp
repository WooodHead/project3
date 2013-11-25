//
//  interval.cpp
//  CP2013
//
//  Created by Farshid Hassani Bijarbooneh on 10/15/13.
//  Copyright (c) 2013 Uppsala University. All rights reserved.
//

#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;
using namespace Gecode::Int;

void interval(Home home, const IntVarArgs& x, const IntArgs& w, double p);

class IntervalBranch : Brancher {
protected:
	/// Views for x-coordinates (or y-coordinates)
	ViewArray<IntView> x;
	/// Width of rectangles
	IntSharedArray w;
	/// Fraction for mandatory part
	double p;
	/// Cache of next unassigned view
	mutable int start;
	/// Choice description
	class Description : public Choice {
	public:
		/// Position of view
		int pos;
		/// Smallest value of view
		int min;
        
		Description(const Brancher& b, unsigned int a, int _pos, int _min)
		: Choice(b,a), pos(_pos), min(_min) {}
		/// Report size occupied
		virtual size_t size(void) const {
			return sizeof(Description);
		}
        /// Archive into e
        virtual void archive(Archive& e) const {
            Choice::archive(e);
            e << alternatives() << pos << min;
        }
	};
	
public:
    
    /// Construct branching
	IntervalBranch(Home home, ViewArray<IntView>& _x, IntSharedArray& _w, double _p)
    : Brancher(home), x(_x), w(_w), p(_p), start(0) {
        home.notice(*this,AP_DISPOSE);
    }
    /// Post branching
	static BrancherHandle post(Home home, ViewArray<IntView>& x, IntSharedArray& w, double p) {
		return *new (home) IntervalBranch(home, x, w, p);
	}
	/// Copy constructor used during cloning
	IntervalBranch(Space& home, bool share, IntervalBranch& intervalbranch)
    : Brancher(home, share, intervalbranch), p(intervalbranch.p), start(intervalbranch.start) {
		x.update(home, share, intervalbranch.x);
        w.update(home, share, intervalbranch.w);
	}
    /// Copy brancher
	virtual Actor* copy(Space& home, bool share) {
		return new (home) IntervalBranch(home, share, *this);
	}
    /// Delete brancher, empty allocated shared space, and return brancher size
    virtual size_t dispose(Space& home) {
        home.ignore(*this,AP_DISPOSE);
        w.~IntSharedArray();
        return sizeof(*this);
    }
	/// Check status of branching, return true if alternatives left.
	virtual bool status(const Space&) const {
		
		/// FILL HERE....
    }
	/// Return branching choice description
	virtual Choice* choice(Space&) {

		/// FILL HERE....
	}
    /// return choice description and reconstruct from archive
    virtual Choice* choice(const Space&, Archive& e){
        int alt,pos,min;
        e >> alt >> pos >> min;
        return new Description(*this, alt, pos, min);
    }
	/// Perform commit for branching description d and alternative a.
	virtual ExecStatus commit(Space& home, const Gecode::Choice& _d,
							  unsigned int a) {
		const Description& d = static_cast<const Description&>(_d);
		
		/// FILL HERE....
    }
    /// Print explanation
    virtual void print(const Space&, const Gecode::Choice& _d,
                       unsigned int a,
                       std::ostream& o) const {
        const Description& d = static_cast<const Description&>(_d);
        
        /// FILL HERE.... (Optional! only used in Gist to view the choices in the search tree)
    }
};

void interval(Home home, const IntVarArgs& x, const IntArgs& w, double p) {
	if (x.size() != w.size())
		throw ArgumentSizeMismatch("interval");
	if (home.failed())
		return;
	ViewArray<IntView> vx(home,x);
    IntSharedArray width(w);
	IntervalBranch::post(home, vx, width, p);
}