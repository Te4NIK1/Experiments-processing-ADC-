#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

#define clr(x) memset((x), 0, sizeof(x))
#define pb push_back
#define mp make_pair
#define sz size()
#define x first
#define y second
#define forn(i, n) for(int i=0; i<(n); i++)
#define ford(i, n) for(int i=(n)-1; i>=0; i--)

typedef long double ld;

// Constants
const ld PI = 3.1415926535897932384626433832795;
const ld EPS = 1e-11;

// Types
typedef signed   long long i64;
typedef unsigned long long u64;
typedef pair < int, int > PII;

int rate=20000;

int c;
double d,d0,d1,MAXP,pan;
bool rec,open,door;
vector <double> p1,p2;
vector <double> inp1,inp2;

double VtoP(double V)
{
	return (V-d0)*50000/4.5;
}

int main()
{
	freopen("11.17 35l window01.dat_converted.txt","rt",stdin);
	freopen("11.17_35l_window1.txt","wt",stdout);

	door=1;
	for(int i=0; cin >>c; i++)
	{
		cin >>d>>pan;
		inp1.pb(d);
		if(door)
			inp2.pb(pan);
		
		//med of begin
		if(i<100)
		{
			d0+=inp1[i]/100;
			d1=d0;
		}
		//med of cur
		else
		{
			d1-=inp1[i-100]/100;
			d1+=inp1[i]/100;
		}

		if(fabs(d1-d0)>0.01)
			rec=1;
		if(door)
		{
			if(inp2[i]<2)
				open=1;
			if(open)
				inp2[i]=0;
		}
		if(rec && p1.size()<rate)
		{
			p1.pb(inp1[i-100]);
			if(door)
				p2.pb(inp2[i-100]);
			MAXP=max(MAXP,p1[p1.size()-1]);
		}
		//dont read too much
		if(p1.size()==rate)
			break;
	}
	cerr <<"MAXP "<<VtoP(MAXP)<<endl;
	forn(i,p1.size())
	{
		cout <<1.0/rate*i<<'\t'<<VtoP(p1[i]);
		if(door)
			cout <<'\t'<<p2[i]*1000;
		cout <<endl;
	}
	return 0;
}
