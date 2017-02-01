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

//ADC frequency
int rate=10000;
//filter window
double wsize=20;
//ignition button (1-5V) treshold
double ignit=3.0;

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

double ArmtoCm(double v)
{
	//from arm_sensor3.xls
	return 6.9387*v*v*v - 41.593*v*v + 89.149*v - 48.223;
}

double CircleToCm(double v)
{
	//from torir_circle
	return -3.8967*v + 22.275;
}

void print_data()
{
	// output all params
	forn(i,outp.size())
	{
		//timestamp | pressure | richag | circle | CM from arm |  
		cout <<1.0/rate*i<<'\t'
		     <<outp[i]<<'\t'
		     <<1600*outr[i]<<'\t'
		     <<1600*outc[i]<<'\t'
		     <<ArmtoCm(outr[i])<<'\t'
		     <<CircleToCm(outc[i])<<endl;
	}
}

int main()
{
	freopen("C:\\Users\\User\\Documents\\31.01 35l window01.dat_converted.txt","rt",stdin);
	freopen("31.01 35l wind.txt","wt",stdout);

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
		if(initr-datR>0.01)
			open=1;

		//med of begin
		if(i<wsize)
		{
			initp+=vp[i]/wsize;
			curp=initp;
		}
		//med of cur
		else
		{
			curp-=vp[i-wsize]/wsize;
			curp+=vp[i]/wsize;
		}

		//begin of burning (dec from 5V to 1V)
		if(button<ignit)
			rec=1;

		if(rec && outp.size()<rate)
		{
			//filtered P
			outp.pb(VtoP(curp));//vp[i]);
			//filtered Richag
			outr.pb(vr[i]);
			/*
			if(open==0)
				outr.pb(5.0);
			else
				outr.pb(0.0);
			*/
			//clear circle
			outc.pb(vc[i]);

			//filtered MAXP
			MAXP=max(MAXP,outp[outp.size()-1]);
		}
		//dont read too much
		if(outp.size()==rate)
			break;
	}
	cerr <<"MAXP "<<MAXP<<endl;
	print_data();
	return 0;
}
