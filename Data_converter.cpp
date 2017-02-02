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
double ignitionTreshold=3.0;
//time before ignition (ms)
int shift=rate/10;

//input Pressure, Ignition, Arm sensor, Wheel sensor
double datPressure,datIgnition,datArm,datWheel;
vector <double> inPressure,inIgnition,inArm,inWheel;

//filtered Pressure, Ignition, Arm sensor, Wheel sensor
vector <double> fiPressure,fiIgnition,fiArm,fiWheel;
//maximum values
double fiMaxPressure,fiMaxIgnition,fiMaxArm,fiMaxWheel;

//start recording & open door
int startRecording=0;

//result params pressure, arm sensor, wheel sensor
vector <double> resTime,resPressure,resIgnition,resArm,resWheel;
double MAXP;

double filterPressure(int pos, int window)
{
	double res=0;
	int k=0;

	for(int i=max(0,pos-window+1); i<=pos; i++,k++)
		res+=inPressure[i];
	return res/k;
}

double filterIgnition(int pos, int window)
{
	double res=0;
	int k=0;

	for(int i=max(0,pos-window+1); i<=pos; i++,k++)
		res+=inIgnition[i];
	return res/k;
}

double filterArm(int pos, int window)
{
	double res=0;
	int k=0;

	for(int i=max(0,pos-window+1); i<=pos; i++,k++)
		res+=inArm[i];
	return res/k;
}

double filterWheel(int pos, int window)
{
	double res=0;
	int k=0;

	for(int i=max(0,pos-window+1); i<=pos; i++,k++)
		res+=inWheel[i];
	return res/k;
}

double VtoP(double V)
{
	//Vout = VS(P x 0.018 + 0.04) ± (Pressure Error x Temp. Factor x 0.018 x VS)
	//VS = 5.0 V ± 0.25 Vdc
	//P=(Vout/Vs-0.04)/0.018;
	return 1000*(V/5.0 - 0.04)/0.018;
	//my variant
	//return (V-fiPressure[wsize])*50000/4.5;
}

double ArmToCm(double v)
{
	//from arm_sensor3.xls
	return 6.9387*v*v*v - 41.593*v*v + 89.149*v - 48.223;
}

double WheelToCm(double v)
{
	//from torir_circle
	//dist 19cm from 0 to 5 V
	return v/5.0*19;
}

void printClearData()
{
   	freopen("31.01 35l wind_CLR.txt","wt",stdout);
	double HIGH=1000;

	forn(i,fiPressure.size())
	{
		//timestamp | pressure | ignition signal | arm sensor | wheel sensor 
		//FAIL for input using FILTERED params
		cout <<1.0*i/rate<<'\t'
		     <<HIGH*inPressure[i]/fiMaxPressure<<'\t'
		     <<HIGH*inIgnition[i]/fiMaxIgnition<<'\t'
		     <<HIGH*inArm[i]/fiMaxArm<<'\t'
		     <<HIGH*inWheel[i]/fiMaxWheel<<endl;
	}
}

void printData()
{
  	freopen("31.01 35l wind_NEW.txt","wt",stdout);
	// output all params
	forn(i,resPressure.size())
	{
		//timestamp | pressure | ignition signal | arm sensor | wheel sensor | arm cm | wheel cm 
		cout <<resTime[i]<<'\t'
		     <<resPressure[i]<<'\t'
		     <<1000*resIgnition[i]<<'\t'
		     <<1000*resArm[i]<<'\t'
		     <<1000*resWheel[i]<<'\t'
		     <<ArmToCm(resArm[i])<<'\t'
		     <<WheelToCm(resWheel[i])<<endl;
	}
}

int main()
{
	freopen("C:\\Users\\User\\Documents\\31.01 35l window01.dat_converted.txt","rt",stdin);
	freopen("31.01 35l wind_NEW.txt","wt",stdout);

	for(int i=0; cin >>datPressure; i++)
	{
		//read lline of params
		cin >>datIgnition>>datArm>>datWheel;
		//datWheel*=-1;

		//put data
		inPressure.pb(datPressure);
		inIgnition.pb(datIgnition);
		inArm.pb(datArm);
        inWheel.pb(datWheel);
		
		//filter data
		fiPressure.pb(filterPressure(inPressure.size()-1,20));
		fiArm.pb(filterArm(inArm.size()-1,20));
		fiWheel.pb(filterWheel(inWheel.size()-1,20));
        
        //max values
        fiMaxPressure=max(fiMaxPressure,fiPressure[fiPressure.size()]);
        fiMaxIgnition=max(fiMaxIgnition,inIgnition[inIgnition.size()]);
		fiMaxArm=max(fiMaxArm,fiArm[fiArm.size()]);
        fiMaxWheel=max(fiMaxWheel,fiWheel[fiWheel.size()]);

		//begin of burning (dec from 5V to 1V)
		if(startRecording==0 && inIgnition[i]<ignitionTreshold)
			startRecording=i;

		if(startRecording>0 && i-shift<0)
			cerr <<"TOO MUCH SHIFT!"<<endl;
		
		//record 1 sec and time shift
		if(startRecording>0 && resPressure.size()<rate+shift)
		{
			//current Time
			resTime.pb(1.0*(i-startRecording-shift)/rate);
			//filtered P
			resPressure.pb(VtoP(fiPressure[i-shift]));
			
			//original Ignition signal
			resIgnition.pb(inIgnition[i-shift]);
			
			//filtered Richag sensor (sub start value)
			resArm.pb(fiArm[i-shift]-fiArm[wsize]);
			//filtered Wheel sensor (sub start value)
			resWheel.pb(fiWheel[i-shift]-fiWheel[wsize]);
			
			//filtered MAXP
			MAXP=max(MAXP,resPressure[resPressure.size()-1]);
		}

		//dont read too much
		if(resPressure.size()==rate+shift)
			break;
	}

	cerr <<"MAXP "<<MAXP<<endl;
	//printClearData();
	printData();
	return 0;
}
