// $Id: position.cpp,v 1.3 2004/07/31 13:39:42 sonntag Exp $
#include <iostream>
using namespace std;

#include "position.h"
#include "utility.h"


Position::Position(QWidget *parent,const char *name)
    : QWidget( parent, name )
{
  tleader_sec = 15.0;
}

Position::~Position()
{
}

void Position::parseinput(QString input,double time1,double lat1,double lon1,
                          double ht1,double x,double y)
{

  // Determine the message type and parse as appropriate
  time = time1;
  lat = lat1;
  lon = lon1;
  ht  = ht1;
  stemp = input.section(',',0,0);
  if (stemp=="$GPGGA")
  {
    hdg = gccourse1(llat*DEG2RAD,llon*DEG2RAD,
                    lat*DEG2RAD,lon*DEG2RAD)/DEG2RAD;
    spd = 3600.0*RAD2NM*gcdist(llat*DEG2RAD,llon*DEG2RAD,
                    lat*DEG2RAD,lon*DEG2RAD) / (time-ltime);
    roc = 60.0*(ht-lht)/(time-ltime)*M2FT;
    llat = lat;
    llon = lon;
    lht = ht;

    // Compute heading rate
    hdgrate = (hdg-lhdg)/(time-ltime);
    lhdg = hdg;
    ltime = time;
  }
  else if (stemp=="10")
  {
    //stemp = input.section(',',1,1);
    //time  = stemp.toDouble();
    //stemp = input.section(',',2,2);
    //lat   = stemp.toDouble();
    //stemp = input.section(',',3,3);
    //lon   = stemp.toDouble();
    //stemp = input.section(',',4,4);
    //ht    = stemp.toDouble();
    stemp = input.section(',',5,5);
    hdg   = stemp.toDouble();
    stemp = input.section(',',6,6);
    spd   = stemp.toDouble();
    stemp = input.section(',',7,7);
    roc   = stemp.toDouble();

    // Compute heading rate
    hdgrate = (hdg-lhdg)/(time-ltime);
    lhdg = hdg;
    ltime = time;
  }
  else
  {
    cout << "Unrecognized network message type - exiting\n";
    exit(-1);
  }
  utmx = x;
  utmy = y;

  //cout << "heading rate:" << hdgrate << '\n';

}


QString Position::gettime()
{
  double sec = time;
  int dow = (int)(sec/86400.0);
  sec -= dow*86400;
  int hr = (int)(sec/3600.0);
  sec -= hr*3600;
  int min = (int)(sec/60.0);
  sec -= min*60.0;
  stemp.sprintf("%02d:%02d:%02d",hr,min,(int)(sec));
  return(stemp);
}

double Position::getlat()
{
  return(lat);
}



double Position::getlon()
{
  return(lon);
}


double Position::getht()
{
  return(ht);
}


double Position::gethdg()
{
  return(hdg);
}


double Position::getspd()
{
  return(spd);
}


double Position::getroc()
{
  return(roc);
}


double Position::gethdgrate()
{
  return(hdgrate);
}


double Position::getx()
{
  return(utmx);
}


double Position::gety()
{
  return(utmy);
}

double Position::getleader()
{
  return(tleader_sec);
}

double Position::getdeltahdg()
{

  if (curwp+3>nwp) // next WP is last one, so can't compute a turn at it
    return(-999.9);
  else
  {
    double latA = wplat[curwp]*DEG2RAD;
    double lonA = wplon[curwp]*DEG2RAD;
    double latB = wplat[curwp+1]*DEG2RAD;
    double lonB = wplon[curwp+1]*DEG2RAD;
    double latC = wplat[curwp+2]*DEG2RAD;
    double lonC = wplon[curwp+2]*DEG2RAD;
    double crsBA = gccourse1(latB,lonB,latA,lonA)/DEG2RAD;
    double crsAB = crsBA+180.0;
    while (crsAB>=360.0) crsAB -= 360.0;
    double crsBC = gccourse1(latB,lonB,latC,lonC)/DEG2RAD;
    double deltahdg = crsBC-crsAB;
    if (deltahdg<-180.0) deltahdg += 360.0;
    else if (deltahdg>180.0) deltahdg -= 360.0;
    return(deltahdg); // delta heading in deg
  }
}

double Position::getreversehdgnext()
{
  double latA = wplat[curwp]*DEG2RAD;
  double lonA = wplon[curwp]*DEG2RAD;
  double latB = wplat[curwp+1]*DEG2RAD;
  double lonB = wplon[curwp+1]*DEG2RAD;
  double crsBA = gccourse1(latB,lonB,latA,lonA)/DEG2RAD;
  return(crsBA); // reciprocal heading at next waypoint to prev waypoint in deg
}

double Position::getleaddist()
{
  if (curwp+3>nwp) // next WP is last one, so can't compute a turn at it
    return(-1);
  else
  {
    double turnradius = getturnradius(); 
    double deltahdg = getdeltahdg();
    double gamma = (deltahdg/2.0)*DEG2RAD;
    double leaddist = fabs(turnradius*tan(gamma));
    return(leaddist); // lead dist in nm
  }
}

double Position::getturnradius()
{
  double theta = MAXBANK*DEG2RAD;
  double vms = getspd()*6076.1/3600.0;
  turnradius = vms*vms/32.2/tan(theta)/6076.1;
  return(turnradius);  // turnradius in nm
}

double Position::getxtd()
{

  // Compute XTD for active turn
  if (turnisactive)
  {
    double turndist = gcdist(latcenter,loncenter,getlat()*DEG2RAD,getlon()*DEG2RAD)*RAD2NM;
    //cout << "turn is active! " << latcenter/DEG2RAD << "  " << loncenter/DEG2RAD << '\n';
    //cout << "turn is active! " << getlat() << "  " << getlon() << '\n';
    //cout << "center " << latcenter/DEG2RAD << "  " << loncenter/DEG2RAD << '\n';
    cout << turndist << "  " << getturnradius() << '\n';
    xtd = (turndist-getturnradius())*6076.1;
    if (getdeltahdg()>0.0)
      xtd *= -1.0;
  }

  // Compute XTD for a straight line segment
  else
  {
    double latA = wplat[curwp]*DEG2RAD;
    double latB = wplat[curwp+1]*DEG2RAD;
    double latD = getlat()*DEG2RAD;
    double lonA = wplon[curwp]*DEG2RAD;
    double lonB = wplon[curwp+1]*DEG2RAD;
    double lonD = getlon()*DEG2RAD;
    double distAD = gcdist(latA,lonA,latD,lonD);
    double crsAD = gccourse1(latA,lonA,latD,lonD);
    double crsAB = gccourse1(latA,lonA,latB,lonB);
    xtd = asin(sin(distAD)*sin(crsAD-crsAB))*RAD2NM;
    xtd *= 6076.1; // XTD is returned in feet
  }

  // Return the xtd value in feet
  return(xtd);

}

double Position::getglideslope()
{

  printf("glideslope is %lf\n",gsdeg);
  printf("hfield is %lf\n",hfield);

  // Compute XTD from the touchdown line
  double latA = tdlat[curwp]*DEG2RAD;
  double latB = tdlat[curwp+1]*DEG2RAD;
  double latD = getlat()*DEG2RAD;
  double lonA = tdlon[curwp]*DEG2RAD;
  double lonB = tdlon[curwp+1]*DEG2RAD;
  double lonD = getlon()*DEG2RAD;
  double distAD = gcdist(latA,lonA,latD,lonD);
  double crsAD = gccourse1(latA,lonA,latD,lonD);
  double crsAB = gccourse1(latA,lonA,latB,lonB);
  xtdtd = asin(sin(distAD)*sin(crsAD-crsAB))*RAD2NM;
  xtdtd *= 6076.1; // XTD is returned in feet

  // Compute height above the field
  double hagl = getht() - hfield;

  // Compute current height of the glideslope
  double hgs = xtdtd*tan(gsdeg*PI/180.0);

  //printf("hagl = %lf  hgs = %lf\n",hagl,hgs);

  // Return the glideslope value in feet
  return(hagl-hgs);

}

double Position::gettot()
{
  double latC,lonC;

  // Compute lat/lon at perpendicular from current pos to desired track (point C)
  xtd = getxtd()/3.28/1000.0/RAD2KM; // xtd in radians
  double latA = wplat[curwp]*DEG2RAD;
  double latB = wplat[curwp+1]*DEG2RAD;
  double latD = getlat()*DEG2RAD;
  double lonA = wplon[curwp]*DEG2RAD;
  double lonB = wplon[curwp+1]*DEG2RAD;
  double lonD = getlon()*DEG2RAD;
  double distAD = gcdist(latA,lonA,latD,lonD);
  double distAC = acos(cos(distAD)/cos(xtd));
  //double distAC = sqrt(distAD*distAD-xtd*xtd); // dist AC in km
  double crsAB = gccourse1(latA,lonA,latB,lonB);
  gcproject(latA,lonA,crsAB,distAC*RAD2KM,&latC,&lonC);

  // Now compute course from point C to next waypoint
  double crsCB = gccourse1(latC,lonC,latB,lonB);

  // Check for reverse sensing
  double crsdiff = abs(crsCB-crsAB)/DEG2RAD;
  crsCB /= DEG2RAD;
  if (crsdiff>150.0&&crsdiff<210.0)
  {
    crsCB += 180.0;
    while(crsCB>=360.0) crsCB-=360.0;
  }
  //cout << "xtd= " << xtd*RAD2NM << endl;
  //cout << "distAD= " << distAD*RAD2NM << endl;
  //cout << "distAC= " << distAC*RAD2NM << endl;
  //cout << "crsAB= " << crsAB/DEG2RAD << endl;
  //cout << "crsdiff= " << crsdiff << endl;
  return(crsCB);

}

double Position::getomega()
{

  
/* double latA = wplat[curwp]*DEG2RAD;
  double latB = wplat[curwp+1]*DEG2RAD;
  double latD = getlat()*DEG2RAD;
  double lonA = wplon[curwp]*DEG2RAD;
  double lonB = wplon[curwp+1]*DEG2RAD;
  double lonD = getlon()*DEG2RAD;
  double distAD = gcdist(latA,lonA,latD,lonD);
  double distAB = gcdist(latA,lonA,latB,lonB);
  double distBD = gcdist(latB,lonB,latD,lonD);
  double xtd=getxtd()/6076.1;
  double atd = acos(cos(distAD)/cos(xtd));//*RAD2NM;
  double distCB=distAB-atd;
  double R=3440.07; //nm
  double alpha=(distCB*RAD2NM)/R;
  double beta=(distBD*RAD2NM)/R;
  double gamma=(xtd*6076.1)/R;
  // Now get the angle
  std::cout << distAD << " " << distAB << " " << distBD << " " << distCB << ": " << atd << ":" << xtd << std::endl;
  std::cout << alpha << " " << beta << " " << gamma << endl;
//  double B=acos( (cos(gamma)-cos(alpha)*cos(beta) ) / ( sin(alpha)*sin(beta) ) );
  double B=acos( (1/tan(1.57079633-beta)) * (1/tan(alpha)) );
  return B;
  double latC = wplat[curwp]*DEG2RAD;
  double latB = wplat[curwp+1]*DEG2RAD;
  double latA = getlat()*DEG2RAD;
  double lonC = wplon[curwp]*DEG2RAD;
  double lonB = wplon[curwp+1]*DEG2RAD;
  double lonA = getlon()*DEG2RAD;
  double distCB = gcdist(latC,lonC,latB,lonB);
  double distAB = gcdist(latA,lonA,latB,lonB);

  double R=3440.07; //nm
  double cb=distCB/R; //beta
  double ab=distAB/R; // alpha


std::cout << distCB << " " << distAB << " " << distCB/distAB << std::endl;
double B=acos(distCB/distAB);
	return B;
*/

}

double Position::getdistto()
{

  // Compute distance to next waypoint
  double latB = wplat[curwp+1]*DEG2RAD;
  double latD = getlat()*DEG2RAD;
  double lonB = wplon[curwp+1]*DEG2RAD;
  double lonD = getlon()*DEG2RAD;
  double distBD = gcdist(latB,lonB,latD,lonD);
  distto = distBD*RAD2NM;

  // Update waypoint if required for autoleg mode
  if (getsteermode()==1 && distto<1.0)
    ++curwp;

  // Activate turn if required for turn-in mode
  if (getsteermode()==2 && distto<getleaddist())
    turnisactive = true;

  // Deactivate turn if required
  if (getsteermode()==2 && turnisactive && distto>getleaddist())
  {
    turnisactive = false;
    ++curwp;
  }

  // Return distance
  return(distto);

}

double Position::getdistfrom()
{
  double latA = wplat[curwp]*DEG2RAD;
  double latD = getlat()*DEG2RAD;
  double lonA = wplon[curwp]*DEG2RAD;
  double lonD = getlon()*DEG2RAD;
  double distAD = gcdist(latA,lonA,latD,lonD);
  distfrom = distAD*RAD2NM;
  return(distfrom);
}

void Position::setleader(double ddum)
{
  tleader_sec = ddum;
}

void Position::slotsetturnin()
{
  turnisactive = false;
  steermode = 2;
}

void Position::slotsetautoleg()
{
  steermode = 1;
}

void Position::slotsetmanual()
{
  steermode = 0;
}

void Position::slotsetlocalizer()
{
  ilsaxis=true;
}

void Position::slotsetglideslope()
{
  ilsaxis=false;
}

int Position::getsteermode()
{
  return(steermode);
}

bool Position::getilsaxis()
{
  return(ilsaxis); //true=localizer, false=glideslope
}

