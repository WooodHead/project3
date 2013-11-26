#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

#include <stdio.h>
#include <math.h>
#include <cstdio>

#include "interval.h"
#include "customScript.cpp"

using namespace std;
using namespace Gecode;
using namespace Gecode::Driver;

const int TeamNr = 7;

int N = 9;
int n;



class Squarepack : public Script {
public:

    IntVar s;
    IntVarArray x;
    IntVarArray y;

    Squarepack(const SizeOptions& opt) :
        s(*this, sqrt((N*(N+1)*(2*N+1))/6.0), (N*N+N)/2),
        x(*this, n, 0, (N*N+N)/2),
        y(*this, n, 0, (N*N+N)/2)
    {
        //constrain coordinate range.
        for(int i = 1; i < n; ++i) {
            rel(*this, x[i] <= s - size(i));
            rel(*this, y[i] <= s - size(i));
        }


        //constraints for non-overlap
        for(int i = 0; i < n-1; ++i) {
            for(int j = i+1; j < n; ++j) {
                rel(*this, x[i] + size(i) <= x[j] ||
                    x[j] + size(j) <= x[i] ||
                    y[i] + size(i) <= y[j] ||
                    y[j] + size(j) <= y[i]);
            }
        }


        IntArgs sizes(n);
        for(int j = 0; j < n; ++j)
            sizes[j] = size(j);

    //    nonOverlapping(*this, x, sizes, y, sizes);

        //Constrain area
        rel(*this, s*s >= N*(N+1)*(2*N+1)/6);

        //symmetry removal for the largest block
        rel(*this, x[0] <= (s - size(0))/2);
        rel(*this, y[0] <= (s - size(0))/2);
        //		rel(*this, x[0] == 0);
        //		rel(*this, y[0] == 0);

        // Does not work. I have tried.
        /*
        for(int i=1; i<4; ++i)
          {
            rel(*this, (x[i] == s-size(i) || x[i] == 0));
            rel(*this, (y[i] == s-size(i) || y[i] == 0));
            }
        */

        //Empty strip dominance constraints
        for(int i = 2; i < N; ++i) {
            int b=0;

            if(i == 2)
                b=1;
            else if(i >= 3 && i <= 4)
                b=2;
            else if(i >= 5 && i <= 8)
                b=3;
            else if(i >= 9 && i <= 11)
                b=4;
            else if(i >= 12 && i <= 17)
                b=5;
            else if(i >= 18 && i <= 21)
                b=6;
            else if(i >= 22 && i <= 29)
                b=7;
            else if(i >= 30 && i <= 34)
                b=8;
            else if(i >= 34 && i <= 44)
                b=9;
            else
                b=10;
            rel(*this, x[index(i)]==s-i || (x[index(i)] < s-b-i && x[index(i)] > b) || x[index(i)]==0);
            rel(*this, y[index(i)]==s-i || (y[index(i)] < s-b-i && y[index(i)] > b) || y[index(i)]==0);
        }

        //implied column and row constraints

        for(int i = 0; i < s.max(); ++i) {
            BoolVarArray bx(*this, n, 0,1);
            BoolVarArray by(*this, n, 0,1);
            IntArgs sizes(n);

            for(int j = 0; j < n; ++j) {
                bx[j] = expr(*this, x[j] <= i && (x[j] + size(j) - 1) >= i);
                by[j] = expr(*this, y[j] <= i && (y[j] + size(j) - 1) >= i);
                sizes[j] = size(j);
            }
            linear(*this, sizes, bx, IRT_LQ, s);
            linear(*this, sizes, by, IRT_LQ, s);
        }

        branch(*this, s, INT_VAL_MIN());
    //    branch(*this, x, INT_VAR_NONE(), INT_VAL_MIN());
      //  branch(*this, y, INT_VAR_NONE(), INT_VAL_MIN());
        //branch(*this, p, INT_VAR_NONE(), INT_VAL_MIN());
        IntArgs w(x.size());
        interval(*this, x, w, 0.0);
        interval(*this, y, w, 0.0);

    }

    Squarepack(bool share, Squarepack& sh) : Script(share, sh) {
        s.update(*this, share, sh.s);
        x.update(*this, share, sh.x);
        y.update(*this, share, sh.y);
    }

    virtual Space* copy(bool share) {
        return new Squarepack(share, *this);
    }

    virtual void print(std::ostream& os) const {
//        os << N << " & " ;
          os << "n: " << N << endl;

        os << "x: {";
        for(int i=0; i<n-1; ++i)
            os << x[i] << ", ";
        os << x[n-1] << "}" << endl;

        os << "y: {";
        for(int i=0; i<n-1; ++i)
            os << y[i] << ", ";
        os << y[n-1] << "}" << endl;

        os << "s: " << s << endl;


    }

    static int size(int index) {
        return N - index;
    }

    static int index(int size) {
        return N - size;
    }
};

int main(int argc, char* argv[]) {

    SizeOptions opt("Squarepack");

    // comment out the following line to get a graphical view of the search tree
    opt.mode(Gecode::SM_GIST);

    if(argc < 2) return 1;
    N = atoi(argv[1]);
    if(N < 2) {
        cout << "The solver does not accept n < 2." << endl;
        return 0;
    }
    n = N-1;

//    std::stringstream filename;
//    filename << "project1-t" << TeamNr << "-n" << N << ".txt";
 //   freopen(filename.str().c_str(),"w",stdout);

    //cout << "enter n: " << endl;
    //cin >> N;

    opt.parse(argc,argv);
    ScriptOutput::run<Squarepack,DFS,SizeOptions>(opt);
    return 0;
}
