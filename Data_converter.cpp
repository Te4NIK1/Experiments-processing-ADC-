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

int rate=10000;

//input Pressure, Ignition, Richag, Circle
double datP,button,datR,datC;
vector <double> vp,vb,vr,vc;
//initial pressure & filtered cur p
double initp,curp;
//init richag
double initr;
//start recording & open door
bool rec,open;
//output params
vector <double> outp,outr,outc;
double MAXP;

double VtoP(double V)
{
	return (V-initp)*50000/4.5;
}

int main()
{
	freopen("C:\\Users\\User\\Documents\\30.01 35l 10.dat_converted.txt","rt",stdin);
	freopen("30.01 35l 10.txt","wt",stdout);

	for(int i=0; cin >>datP; i++)
	{
		cin >>button>>datR>>datC;
		vp.pb(datP);
		vb.pb(button);
		vr.pb(datR);
        vc.pb(datC);
		
		//init richag
		if(i==0)
			initr=datR;
		//open door
		if(initr-datR>20.0/200)
			open=1;

		//med of begin
		if(i<100)
		{
			initp+=vp[i]/100;
			curp=initp;
		}
		//med of cur
		else
		{
			curp-=vp[i-100]/100;
			curp+=vp[i]/100;
		}

		if(button<3.0) // dec from 5V to 1V
			rec=1;
		if(rec && outp.size()<rate)
		{
			//filtered P
			outp.pb(VtoP(curp));//vp[i]);
			//filtered Richag
			if(open==0)
				outr.pb(5.0);
			else
				outr.pb(0.0);
			//outp.pb(vp[i]);
			//outr.pb(vr[i]);
			outc.pb(vc[i]);
			MAXP=max(MAXP,outp[outp.size()-1]);
		}
		//dont read too much
		if(outp.size()==rate)
			break;
	}
	cerr <<"MAXP "<<MAXP<<endl;
	// output all params
	forn(i,outp.size())
	{
		//time pressure richag circle
		cout <<1.0/rate*i<<'\t'
		     <<outp[i]<<'\t'
		     <<200*outr[i]<<'\t'
		     <<200*outc[i]<<endl;
	}
	return 0;
}
