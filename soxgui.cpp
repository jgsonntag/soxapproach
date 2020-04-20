// $Id: soxgui.cpp,v 1.14 2007/05/13 21:29:31 sonntag Exp $
// Soxgui class implementation
//

#include <qapplication.h>
#include <qmessagebox.h>
#include <qhgroupbox.h>
#include <qvgroupbox.h>
#include <qvbuttongroup.h>
#include <qradiobutton.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qdatetime.h>
#include <qdir.h>


//#include "soxmap.h"
#include "soxgui.h"
#include "setupgui.h"
#include "pathdialog.h"
#include "utility.h"

#define SWIDTH 750
#define SHEIGHT 800

#include <iostream>
using namespace std;

double LON0;

QString filedialog;

double gsdeg;
double hfield;
QString tdlabel[2];
double tdlat[2];
double tdlon[2];
double tdx[2];
double tdy[2];
QString wplabel[200];
double wplat[200];
double wplon[200];
double wpx[200];
double wpy[200];
int nwp;
int curwp;
bool turnisactive;
double latcenter;
double loncenter;

Soxgui::Soxgui()
{

  // Get the saved user settings
  gpshost = usettings.readEntry("/soxapproach/network/gpshost","localhost");
  gpsport = (Q_UINT16)(usettings.readNumEntry("/soxapproach/network/gpsport",4040));
  gpscmd = usettings.readEntry("/soxapproach/network/gpscmd","gga1hzstream");
  swathwidth = usettings.readDoubleEntry("/soxapproach/map/swathwidth",180.0);
  double tleader_sec = usettings.readDoubleEntry("/soxapproach/map/tleader_sec",
                                                 15.0);
  bool maporient = usettings.readBoolEntry("/soxapproach/map/trackup",true);
  int steermode = usettings.readNumEntry("/soxapproach/nav/autoleg",0);
  bool ilsaxis = usettings.readBoolEntry("/soxapproach/nav/ilsaxis",true);
  int currentzoom = usettings.readNumEntry("/soxapproach/map/currentzoom",4);
  int pathskip = usettings.readNumEntry("/soxapproach/map/pathskip",1);
  curwp = usettings.readNumEntry("/soxapproach/map/curwp",0);
  QColor bgcolor = readcolor("bgcolor","black");
  QColor accolor = readcolor("accolor","white");
  QColor llcolor = readcolor("llcolor","white");
  QColor rrcolor = readcolor("rrcolor","yellow");
  QColor pathcolor = readcolor("pathcolor","green");
  QColor dicolor = readcolor("dicolor","yellow");
  QColor wpcolor = readcolor("wpcolor","white");
  logfile = usettings.readEntry("/soxapproach/files/logfile","");
  wpname = usettings.readEntry("/soxapproach/files/wpname","");
  cdigain = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/gain",500));
  cdileftgain = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/leftgain",500));
  cdirightgain = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/rightgain",500));
  odist = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/odist",1000));
  granularity = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/granularity",500));
  signalmodel = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/signalmodel",0));
  mu = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/mu",300));
  sigma = (Q_UINT16)(usettings.readNumEntry("/soxapproach/cdi/sigma",300));
	
  // Size the window
  setFixedSize(SWIDTH,SHEIGHT);

  // Get network settings from user
  Setupgui *setup = new Setupgui(&gpshost,&gpsport,&gpscmd,this);
  while (setup->exec() == QDialog::Rejected)
  {
    QMessageBox::warning(this,"Invalid\n","Port number is invalid\n");
  }
  if (!gpscmd.contains('\n'))
    gpscmd.append('\n');

  // Set the background flags;
  wpflag = false;

  // Construct linked lists for the coordinates
  flight = new coords_ll();
  waypoint = new wp_ll();
  touchdown = new wp_ll();

  // Construct the current position class;
  current = new Position(this);
  current->setleader(tleader_sec);
  //if (alon) current->slotsetalon();
  //else current->slotsetaloff();

  // Build the main menubar and make connections
  mainmenu = new QMenuBar(this);
  filemenu = new QPopupMenu(mainmenu);
  QPopupMenu *mapmenu = new QPopupMenu(mainmenu);
  QPopupMenu *navmenu = new QPopupMenu(mainmenu);
  QPopupMenu *helpmenu = new QPopupMenu(mainmenu);

  // Build the map display
  map = new Mapview(flight,current,this);
  //map->setGeometry(0,mainmenu->height(),500,480-mainmenu->height());
  map->setGeometry(0,mainmenu->height()+50,SWIDTH-140,SHEIGHT-mainmenu->height()-100);
  map->setcurrentzoom(currentzoom);
  map->setbgcolor(bgcolor);
  map->setaccolor(accolor);
  map->setllcolor(llcolor);
  map->setrrcolor(rrcolor);
  map->setpathcolor(pathcolor);
  map->setdicolor(dicolor);
  map->setwpcolor(wpcolor);
  map->slotsetpathskip(pathskip);

  // update map gain values
  map->setlgain(cdileftgain);
  map->setrgain(cdirightgain);

  // Build the File menu
  idwp = filemenu->insertItem("Open Waypoint file",this,SLOT(slotWPFile()));
  filemenu->insertItem("Quit",qApp,SLOT(quit()));

  // Build the Map menu
  QPopupMenu *orientmenu = new QPopupMenu(mapmenu);
  QVButtonGroup *orientgroup = new QVButtonGroup(orientmenu);
  QRadioButton *northup = new QRadioButton("North up",orientgroup);
  QRadioButton *trackup = new QRadioButton("Track up",orientgroup);
  orientmenu->insertItem(orientgroup);
  mapmenu->insertItem("Map orientation",orientmenu);
  if (maporient)
  {
    map->slotsettrackup();
    trackup->setChecked(true);
  }
  else
  {
    map->slotsetnorthup();
    northup->setChecked(true);
  }
  QPopupMenu *colormenu = new QPopupMenu(mapmenu);
  colormenu->insertItem("Map background color",map,SLOT(slotselectbgcolor()));
  colormenu->insertItem("Aircraft symbol color",map,SLOT(slotselectaccolor()));
  colormenu->insertItem("Leader line color",map,SLOT(slotselectllcolor()));
  colormenu->insertItem("Range rings color",map,SLOT(slotselectrrcolor()));
  colormenu->insertItem("Flightpath color",map,SLOT(slotselectpathcolor()));
  colormenu->insertItem("Compass color",map,SLOT(slotselectdicolor()));
  colormenu->insertItem("Waypoint color",map,SLOT(slotselectwaypointcolor()));
  mapmenu->insertItem("Color selectors",colormenu);
  mapmenu->insertItem("Set swath width",this,SLOT(slotSetSwathWidth()));
  mapmenu->insertItem("Select leader line length",this,SLOT(slotSetLeader()));
  QPopupMenu *thinmenu = new QPopupMenu(mapmenu);
  QPopupMenu *paththinmenu = new QPopupMenu(thinmenu);
  QSpinBox *paththinsb = new QSpinBox(1,10,1,paththinmenu);
  paththinsb->setValue(pathskip);
  paththinmenu->insertItem(paththinsb);
  thinmenu->insertItem("Current path thinning",paththinmenu);
  mapmenu->insertItem("Display thinning",thinmenu);

  // Build the Nav menu
  QPopupMenu *automenu = new QPopupMenu(navmenu);
  QPopupMenu *axismenu = new QPopupMenu(navmenu);
  QPopupMenu *ilsmenu = new QPopupMenu(navmenu);
  QVButtonGroup *autogroup = new QVButtonGroup(automenu);
  QVButtonGroup *axisgroup = new QVButtonGroup(axismenu);
  QVButtonGroup *ilsgroup = new QVButtonGroup(ilsmenu);
  QRadioButton *smodemanual = new QRadioButton("Manual",autogroup);
  QRadioButton *smodeautoleg = new QRadioButton("Auto-leg",autogroup);
  QRadioButton *smodeturnin = new QRadioButton("Turn-in",autogroup);
  QRadioButton *loon = new QRadioButton("Localizer Signal On",axisgroup);
  QRadioButton *gson = new QRadioButton("Glideslope Signal On",axisgroup);
  QRadioButton *ilson = new QRadioButton("CDI On",ilsgroup);
  QRadioButton *ilsoff = new QRadioButton("CDI Off",ilsgroup);
  connect(ilson,SIGNAL(clicked()),this,SLOT(slotEnableCdi()));
  connect(ilsoff,SIGNAL(clicked()),this,SLOT(slotDisableCdi()));
  connect(loon,SIGNAL(clicked()),this,SLOT(slotEnableLocalizer()));
  connect(gson,SIGNAL(clicked()),this,SLOT(slotEnableGlideslope()));
	
  ilsmenu->insertItem(ilsgroup);
  automenu->insertItem(autogroup);
  axismenu->insertItem(axisgroup);
  navmenu->insertItem("ILS signal axis",axismenu);
  navmenu->insertItem("Waypoint mode",automenu);
  idCDI = navmenu->insertItem("CDI",ilsmenu);
  setupOnce = 0;
  ilson->setChecked(true);
  ilsoff->setChecked(false);
  navmenu->setItemEnabled(idCDI,false); // never allow CDI disable
  printf("howdy1\n");
  slotEnableCdi(); // start CDI enabled
  printf("howdy2\n");
  if (ilsaxis)
  {
    current->slotsetlocalizer();
    loon->setChecked(true);
  }
  else
  {
    current->slotsetglideslope();
    gson->setChecked(true);
  }
  if (steermode==0)
  {
    current->slotsetmanual();
    smodemanual->setChecked(true);
  }
  else if (steermode==1)
  {
    current->slotsetautoleg();
    smodeautoleg->setChecked(true);
  }
  else if (steermode==2)
  {
    current->slotsetturnin();
    smodeturnin->setChecked(true);
  }
  navmenu->insertItem("Set ILS gain",this,SLOT(slotCdiGain()) );
  QPopupMenu *modelmenu = new QPopupMenu(navmenu);
  QVButtonGroup *modelgroup = new QVButtonGroup(modelmenu);
  QRadioButton *linearmodel = new QRadioButton("Linear",modelgroup);
  QRadioButton *gaussianmodel = new QRadioButton("Gaussian",modelgroup);
  modelmenu->insertItem(modelgroup);
  connect(linearmodel,SIGNAL(clicked()),this,SLOT(slotLinearModel()));
  connect(gaussianmodel,SIGNAL(clicked()),this,SLOT(slotGaussianModel()));
  navmenu->insertItem("Set Model",modelmenu);
  QPopupMenu *cdisettingsmenu = new QPopupMenu(navmenu);
  navmenu->insertItem("Model and Gain Settings",cdisettingsmenu);
  idDist=cdisettingsmenu->insertItem("Set Falloff Distance",this,SLOT(slotCdiDist()) );
  idGran=cdisettingsmenu->insertItem("Granularity",this,SLOT(slotCdiGranularity()) );
  idSigma=cdisettingsmenu->insertItem("Lobe Width Sigma",this,SLOT(slotCdiSigma()) );
  idMu=cdisettingsmenu->insertItem("Lobe Offset Mu",this,SLOT(slotCdiMu()) );	
  cdisettingsmenu->insertItem("Left 90 Gain",this,SLOT(slotCdiLeftGain()) );
  cdisettingsmenu->insertItem("Right 150 Gain",this,SLOT(slotCdiRightGain()) );

  //navmenu->insertItem("Set ROA",this,SLOT(slotCdiROA()) );

  // Build the Help menu
  helpmenu->insertItem("About",this,SLOT(about()));
  filedialog = "latlon (*.latlon);; SOXCDI log (*_sox.log);;All (*.*)";

  // Finish building the main menu
  mainmenu->insertItem("File",filemenu);
  mainmenu->insertItem("Map",mapmenu);
  mainmenu->insertItem("Nav",navmenu);
  mainmenu->insertItem("Help",helpmenu);

  // Build the From WP readout
  QHGroupBox *frombox = new QHGroupBox(this);
  frombox->setGeometry(0,SHEIGHT-45,SWIDTH-140,40);
  from = new Showfrom(current,frombox);

  // Build the To WP readout
  QHGroupBox *towpbox = new QHGroupBox(this);
  towpbox->setGeometry(0,mainmenu->height(),SWIDTH-140,40);
  towp = new Showtowp(current,towpbox);

  // Build the state readout
  QVGroupBox *statebox = new QVGroupBox(this);
  statebox->setGeometry(SWIDTH-130,mainmenu->height()+90,120,SHEIGHT-220);
  state = new Showstate(current,statebox);

  // Build the waypoint select buttons
  prevwp = new QPushButton("Prev WP",this);
  prevwp->setGeometry(SWIDTH-130,mainmenu->height(),120,40);
  connect(prevwp,SIGNAL(clicked()),map,SLOT(slotPrevWP()));
  nextwp = new QPushButton("Next WP",this);
  nextwp->setGeometry(SWIDTH-130,mainmenu->height()+45,120,40);
  connect(nextwp,SIGNAL(clicked()),map,SLOT(slotNextWP()));

  // Build the zoom buttons
  zoomin  = new QPushButton("Zoom In",this);
  zoomin->setGeometry(SWIDTH-130,mainmenu->height()+SHEIGHT-120,120,40);
  connect(zoomin,SIGNAL(clicked()),map,SLOT(slotzoomin()));
  zoomout = new QPushButton("Zoom Out",this);
  zoomout->setGeometry(SWIDTH-130,mainmenu->height()+SHEIGHT-75,120,40);
  connect(zoomout,SIGNAL(clicked()),map,SLOT(slotzoomout()));

  // Build the network client
  client = new GenericClient(gpshost,gpsport,gpscmd,this);
  connect(client,SIGNAL(gotaline(QString)),this,SLOT(slotFirstGPS(QString)));

  // Connect remaining signals and slots
  connect(trackup,SIGNAL(clicked()),map,SLOT(slotsettrackup()));
  connect(northup,SIGNAL(clicked()),map,SLOT(slotsetnorthup()));
  connect(smodeautoleg,SIGNAL(clicked()),current,SLOT(slotsetautoleg()));
  connect(smodeturnin,SIGNAL(clicked()),current,SLOT(slotsetturnin()));
  connect(smodemanual,SIGNAL(clicked()),current,SLOT(slotsetmanual()));
  connect(paththinsb,SIGNAL(valueChanged(int)),map,SLOT(slotsetpathskip(int)));

  // ILS stuff
  //setupOnce = 0;
  if (signalmodel == 0) {linearmodel->setChecked(true);slotLinearModel();}
  if (signalmodel == 1) {gaussianmodel->setChecked(true);slotGaussianModel();}
		
}


Soxgui::~Soxgui()
{

  // Inform user of log file name and path
  cout << "Soxcdi log file written to " << logfile << '\n';

  // Close the network connection
  client->closeConnection();

  // Close the path file
  pathfile->close();

  // Save user settings
  usettings.writeEntry("/soxapproach/network/gpshost",gpshost);
  usettings.writeEntry("/soxapproach/network/gpsport",gpsport);
  usettings.writeEntry("/soxapproach/network/gpscmd",gpscmd);
  usettings.writeEntry("/soxapproach/map/swathwidth",swathwidth);
  usettings.writeEntry("/soxapproach/map/tleader_sec",current->getleader());
  usettings.writeEntry("/soxapproach/map/trackup",map->gettrackup());
  usettings.writeEntry("/soxapproach/map/currentzoom",map->getcurrentzoom());
  usettings.writeEntry("/soxapproach/map/pathskip",map->getpathskip());
  usettings.writeEntry("/soxapproach/map/curwp",curwp);
  usettings.writeEntry("/soxapproach/nav/autoleg",current->getsteermode());
  usettings.writeEntry("/soxapproach/nav/ilsaxis",current->getilsaxis());
  writecolor("bgcolor",map->getbgcolor());
  writecolor("accolor",map->getaccolor());
  writecolor("llcolor",map->getllcolor());
  writecolor("rrcolor",map->getrrcolor());
  writecolor("pathcolor",map->getpathcolor());
  writecolor("dicolor",map->getdicolor());
  writecolor("wpcolor",map->getwpcolor());
  usettings.writeEntry("/soxapproach/files/logfile",logfile);
  usettings.writeEntry("/soxapproach/files/wpname",wpname);
  usettings.writeEntry("/soxapproach/cdi/gain",cdigain);
  usettings.writeEntry("/soxapproach/cdi/leftgain",cdileftgain);
  usettings.writeEntry("/soxapproach/cdi/rightgain",cdirightgain);
  usettings.writeEntry("/soxapproach/cdi/odist",odist);
  usettings.writeEntry("/soxapproach/cdi/granularity",granularity);

  // Delete the objects still out there
  delete mainmenu;
  delete client;
  delete flight;
  gen.terminate();

}


QColor Soxgui::readcolor(QString clabel,QString defaultcolor)
{
  QColor ctemp;
  QString stemp = "/soxapproach/map/";
  stemp.append(clabel);
  QString stemp2 = stemp + "/red";
  int ired = usettings.readNumEntry(stemp2,-999);
  stemp2 = stemp + "/green";
  int igreen = usettings.readNumEntry(stemp2,-999);
  stemp2 = stemp + "/blue";
  int iblue = usettings.readNumEntry(stemp2,-999);
  if (ired!=-999||igreen!=-999||iblue!=-999)
    ctemp.setRgb(ired,igreen,iblue);
  else
    ctemp.setNamedColor(defaultcolor);
  return(ctemp);

}


void Soxgui::writecolor(QString clabel,QColor color)
{
  QString stemp = "/soxapproach/map/";
  stemp.append(clabel);
  QString stemp2 = stemp + "/red";
  usettings.writeEntry(stemp2,color.red());
  stemp2 = stemp + "/green";
  usettings.writeEntry(stemp2,color.green());
  stemp2 = stemp + "/blue";
  usettings.writeEntry(stemp2,color.blue());
}


void Soxgui::ingest_latlon(QFile *infile, coords_ll *coords)
{
  const char *sline;
  double dtemp1,dtemp2,dtemp3,x,y;
  QString line;

  infile->open(IO_ReadOnly);
  QTextStream instream(infile);
  while (!instream.atEnd())
  {
    line = instream.readLine();
    sline = line.latin1();
    // if the line length is 0, pretend we got a -999 flag value
    if ( line.length() > 0 ) {
      sscanf(sline,"%lf %lf %lf ",&dtemp1,&dtemp2,&dtemp3);
    } else {
      dtemp1 = dtemp2 = -999.0;
    }
    coords->add_element(dtemp1,dtemp2,dtemp3*FT2M,&x,&y);
  }
  infile->close();
}


void Soxgui::ingest_waypoint(QFile *infile, wp_ll *coords, wp_ll *coords2)
{
  const char *sline;
  char stemp[15];
  //int i;
  double dtemp1,dtemp2,dtemp3,x,y;
  double rwylength,rwyhdg,tddist,lattd,lontd,latl,lonl,latr,lonr;
  QString line;

  infile->open(IO_ReadOnly);
  QTextStream instream(infile);

  // Read start and end runway coordinates
  line = instream.readLine();
  sline = line.latin1();
  sscanf(sline,"%s %lf %lf %lf",stemp,&dtemp2,&dtemp3,&dtemp1);
  coords->add_element(stemp,dtemp2,dtemp3,double(0.0),&x,&y);
  wplabel[0] = stemp;
  wplat[0] = dtemp2;
  wplon[0] = dtemp3;
  wpx[0] = x;
  wpy[0] = y;
  line = instream.readLine();
  sline = line.latin1();
  sscanf(sline,"%s %lf %lf %lf",stemp,&dtemp2,&dtemp3,&dtemp1);
  coords->add_element(stemp,dtemp2,dtemp3,double(0.0),&x,&y);
  wplabel[1] = stemp;
  wplat[1] = dtemp2;
  wplon[1] = dtemp3;
  wpx[1] = x;
  wpy[1] = y;
  nwp = 2;

  // Read touchdown distance fraction and compute touchdown coords
  line = instream.readLine();
  sline = line.latin1();
  sscanf(sline,"%lf",&tddist);
  printf("fractional touchdown dist is %lf\n",tddist);
  rwylength = RAD2KM*gcdist(wplat[0]*PI/180.0,wplon[0]*PI/180.0,
                            wplat[1]*PI/180.0,wplon[1]*PI/180.0);
  rwyhdg = gccourse1(wplat[0]*PI/180.0,wplon[0]*PI/180.0,
                     wplat[1]*PI/180.0,wplon[1]*PI/180.0);
  gcproject(wplat[0]*PI/180.0,wplon[0]*PI/180.0,rwyhdg,
            rwylength*tddist,&lattd,&lontd);
  printf("runway dist is %lf km\n",rwylength);
  printf("runway hdg is %lf deg\n",rwyhdg/DEG2RAD);
  printf("tdpoint %lf %lf\n",lattd*180.0/PI,lontd*180.0/PI);

  // Form line perpendicular to rwy crossing it at td point
  double lefthdg = rwyhdg-PI/2.0;
  while (lefthdg<0.0) lefthdg+=2.0*PI;
  double righthdg = rwyhdg+PI/2.0;
  while (righthdg>(2.0*PI)) righthdg-=2.0*PI;
  printf("left hdg = %lf\n",lefthdg/DEG2RAD);
  printf("right hdg = %lf\n",righthdg/DEG2RAD);
  gcproject(lattd,lontd,lefthdg,10.0,&latl,&lonl);
  printf("latl/lonl %lf %lf\n",latl,lonl);
  strcpy(stemp,"left\0");
  coords2->add_element(stemp,latl*180.0/PI,lonl*180.0/PI,
                      double(0.0),&x,&y);
  tdlabel[0] = stemp;
  tdlat[0] = latl*180.0/PI;
  tdlon[0] = lonl*180.0/PI;
  tdx[0] = x;
  tdy[0] = y;
  gcproject(lattd,lontd,righthdg,10.0,&latr,&lonr);
  printf("latr/lonr %lf %lf\n",latr,lonr);
  strcpy(stemp,"right\0");
  coords2->add_element(stemp,latr*180.0/PI,lonr*180.0/PI,
                      double(0.0),&x,&y);
  tdlabel[1] = stemp;
  tdlat[1] = latr*180.0/PI;
  tdlon[1] = lonr*180.0/PI;
  tdx[1] = x;
  tdy[1] = y;

  // Read glideslope
  line = instream.readLine();
  sline = line.latin1();
  sscanf(sline,"%lf",&gsdeg);
  printf("glideslope is %lf\n",gsdeg);

  // Read field elevation
  line = instream.readLine();
  sline = line.latin1();
  sscanf(sline,"%lf",&hfield);
  printf("field elevation is %lf\n",hfield);

  //  Close the input file
  infile->close();

}


void Soxgui::slotFirstGPS(QString gps)
{

  // Disconnect this slot - we only want to invoke it once!
  disconnect(client,SIGNAL(gotaline(QString)),this,SLOT(slotFirstGPS(QString)));

  // Determine the message type and parse as appropriate
  // This sets the central meridian for the map projection
  stemp = gps.section(',',0,0);
  if (stemp=="$GPGGA")
  {
    parseGGA(gps,&t,&lat,&lon,&ht);
    LON0 = lon;
  }
  else if (stemp=="10")
  {
    stemp2 = gps.section(',',3,3);
    LON0 = stemp2.toDouble();
  }
  else
  {
    cout << "Unrecognized network message type - exiting\n";
    exit(-1);
  }

  // Check for an existing path file
  QDate qdate = QDate::currentDate(Qt::UTC);
  int qyear = qdate.year();
  qyear -= 100*(int)(qdate.year()/100);
  QTime qtime = QTime::currentTime();

  newlogfile.sprintf("%04d%02d%02d_%02d%02d%02d_soxapproach.log",qdate.year(),qdate.month(),
              qdate.day(),qtime.hour(),qtime.minute(),qtime.second());
  pathfile = new QFile(logfile);
  if (pathfile->exists())
  {
    Pathdialog *pathsetup = new Pathdialog(logfile,this);
    if (pathsetup->exec() == QDialog::Accepted)
    {
      ingest_latlon(pathfile,flight); // read the file
      pathfile->open(IO_WriteOnly|IO_Append);
    }
    else
    {
      //pathfile->open(IO_WriteOnly);
      logfile = newlogfile;
      pathfile = new QFile(logfile);
      pathfile->open(IO_WriteOnly);
    }
  }
  else
  {
    logfile = newlogfile;
    pathfile = new QFile(logfile);
    pathfile->open(IO_WriteOnly);
  }
  pathstream = new QTextStream(pathfile);
  
  // Now that we have the central meridian set, we can also ingest any
  // previously existing target files
  wpfile = new QFile(wpname);
  if (wpfile->exists())
  {
    ingest_waypoint(wpfile,waypoint,touchdown);
    wpflag = true;
    stemp = QString("Close Waypoint file (");
    stemp.append(wpname);
    stemp.append(")");
    filemenu->changeItem(idwp,stemp);
  }

  // Now connect the network GPS signal with the regular map update etc
  connect(client,SIGNAL(gotaline(QString)),this,SLOT(slotNewGPS(QString)));

}


void Soxgui::parseGGA(QString gga,double *t,double *lat,
                      double *lon,double *ht)
{

  // Get the time
  stemp = gga.section(',',1);
  sec = stemp.toDouble();
  hr = int(sec/10000.0);
  sec = sec-hr*10000.0;
  min = int(sec/100.0);
  sec = sec-min*100.0;
  *t = hr*3600.0+min*60.0+sec;

  // Get the latitude
  stemp = gga.section(',',2);
  latm = stemp.toDouble();
  latd = (int)(latm/100.0);
  latm = latm - (double)(latd)*100.0;
  *lat = (double)(latd)+latm/60.0;
  stemp = gga.section(',',3,3);
  if (stemp.contains('N'))
    *lat *= 1.0;
  else
    *lat *= -1.0;

  // Get the longitude
  stemp = gga.section(',',4);
  lonm = stemp.toDouble();
  lond = (int)(lonm/100.0);
  lonm = lonm - (double)(lond)*100.0;
  *lon = (double)(lond)+lonm/60.0;
  stemp = gga.section(',',5,5);
  if (stemp.contains('E'))
    *lon *= 1.0;
  else
    *lon *= -1.0;
  if (*lon<0.0)
    *lon += 360.0;

  // Get the height
  stemp = gga.section(',',9,9);
  *ht = M2FT*stemp.toDouble();

}


void Soxgui::slotNewGPS(QString gps)
{
  double x,y;

  // Determine the message type and parse as appropriate
  stemp = gps.section(',',0,0);
  if (stemp=="$GPGGA")
  {
    parseGGA(gps,&t,&lat,&lon,&ht);
  }
  else if (stemp=="10")
  {
    stemp1 = gps.section(',',2,2);
    stemp2 = gps.section(',',3,3);
    lat = stemp1.toDouble();
    lon = stemp2.toDouble();
    stemp = gps.section(',',1,1);
    t = stemp.toDouble();
    stemp = gps.section(',',4,4);
    ht = stemp.toDouble();
  }
  else
  {
    cout << "Unrecognized network message type - exiting\n";
    exit(-1);
  }

  // Add to linked-list of positions for this flight
  pathstream->precision(8);
  *pathstream << lat << ' ' << lon << ' ' << swathwidth/FT2M << '\n';
  flight->add_element(lat,lon,swathwidth,&x,&y);
  current->parseinput(gps,t,lat,lon,ht,x,y);
  map->updatemap();
  from->update();
  towp->update();
  state->update();
}

void Soxgui::slotLinearModel()
{
	std::cout << "Enabling Linear Model" << std::endl;
	signalmodel=0;
	usettings.writeEntry("/soxapproach/cdi/signalmodel",signalmodel);
	// disable entries in menu
	mainmenu->setItemEnabled(idGran,true);
	mainmenu->setItemEnabled(idDist,true);
	mainmenu->setItemEnabled(idSigma,false);
	mainmenu->setItemEnabled(idMu,false);

}

void Soxgui::slotGaussianModel()
{
	std::cout << "Enabling Gaussian Model" << std::endl;
	signalmodel=1;
	usettings.writeEntry("/soxapproach/cdi/signalmodel",signalmodel);
	// disable entries in menu
	mainmenu->setItemEnabled(idGran,false);
	mainmenu->setItemEnabled(idDist,false);
	mainmenu->setItemEnabled(idSigma,true);
	mainmenu->setItemEnabled(idMu,true);
	
}

void Soxgui::slotCdiSigma()
{
	cout << "CDI Sigma change" << endl;
	bool ok;
    int l = QInputDialog::getInteger(
            "Lobe Width (Sigma)", "Width of lobe (ft):", sigma, 0, 20000, 1,
            &ok, this );
    if ( ok ) {
	sigma=l;
    usettings.writeEntry("/soxapproach/cdi/sigma",sigma);
	}	
}

void Soxgui::slotCdiMu()
{
	cout << "CDI mu change" << endl;
	bool ok;
    int l = QInputDialog::getInteger(
            "Transmitter Offset Value (Mu)", "Enter offset of centerline (ft):", mu, 0, 20000, 1,
            &ok, this );
    if ( ok ) {
	mu=l;
    usettings.writeEntry("/soxapproach/cdi/mu",mu);
	}
}

void Soxgui::slotCdiDist()
{
	// Set outside distance from leg before CDI begins attenuation
	bool ok;
    int l = QInputDialog::getInteger(
            "Attenuation Limit Distance", "Limit of attenuation from leg[ft]:", odist, 1, 264000, 1,
            &ok, this );
    if ( ok ) {
	odist=l;

    } else {
        
    }
  usettings.writeEntry("/soxapproach/cdi/odist",odist);
	
}


void Soxgui::slotEnableCdi()
{
cout << "CDI enabled" << endl;
/* Start thread to make 90Hz and 150Hz tones
   and timer to adjust the thread */
gen.exitme = 0;

cout << "point A" << endl;

if (!setupOnce) 
 	
	{
	gen.setup(); setupOnce = 1;
	gen.setmaxGainLeft(cdileftgain);
	gen.setmaxGainRight(cdirightgain);
	gen.setGranularity(granularity);
  	connect( &cdiTimer, SIGNAL(timeout()), this, SLOT(cdiTimerDone()) );
	}

   gen.start();
   cdiTimer.start( 50, FALSE ); // 50 = 20 Hz update    	
cout << "point Z" << endl;
}

void Soxgui::slotDisableCdi()
{
cdiTimer.stop();
//gen->mutex->lock();
gen.exitThread();
//gen->mutex->unlock();
//gen->exitThread();
gen.wait(3000);  // wait 3 sec
gen.terminate();

cout << "CDI disabled" << endl;
}

void Soxgui::slotEnableLocalizer()
{
  current->slotsetlocalizer();
}

void Soxgui::slotEnableGlideslope()
{
  current->slotsetglideslope();
}

void Soxgui::slotCdiGain()
{
cout << "CDI gain change" << endl;
//int l = gainbox->value();
bool ok;
    int l = QInputDialog::getInteger(
            "Gain Setting (resets L&R settings)", "Enter max gain value:", cdigain, 0, 1000, 1,
            &ok, this );
    if ( ok ) {
	gen.setmaxGainLeft(l);
	gen.setmaxGainRight(l);        
	cdigain=l;
	cdileftgain=l;
	cdirightgain=l;
    } else {
        
    }
  usettings.writeEntry("/soxapproach/cdi/gain",cdigain);
  usettings.writeEntry("/soxapproach/cdi/leftgain",cdigain);
  usettings.writeEntry("/soxapproach/cdi/rightgain",cdigain);
  map->setlgain(cdigain);
  map->setrgain(cdigain);
}

void Soxgui::slotCdiLeftGain()
{
cout << "CDI gain change LEFT" << endl;
bool ok;
    int l = QInputDialog::getInteger(
            "Left (90Hz) Gain Setting", "Enter max gain value:", cdileftgain, 0, 1000, 1,
            &ok, this );
    if ( ok ) {
	gen.setmaxGainLeft(l);
//	gen.setmaxGainRight(l);        
	cdileftgain=l;
	map->setlgain(cdileftgain);
    } else {
        
    }
  usettings.writeEntry("/soxapproach/cdi/leftgain",cdileftgain);
}

void Soxgui::slotCdiRightGain()
{
cout << "CDI gain change RIGHT" << endl;
bool ok;
    int l = QInputDialog::getInteger(
            "Right (150Hz) Gain Setting", "Enter max gain value:", cdirightgain, 0, 1000, 1,
            &ok, this );
    if ( ok ) {
//	gen.setmaxGainLeft(l);
	gen.setmaxGainRight(l);        
	cdirightgain=l;
    map->setrgain(cdirightgain);
    } else {
        
    }
  usettings.writeEntry("/soxapproach/cdi/rightgain",cdirightgain);
}

void Soxgui::slotCdiGranularity()
{
cout << "CDI Granularity" << endl;
bool ok;
    int l = QInputDialog::getInteger(
            "Granularity", "Granularity (<= Att. Dist!):", granularity, 0, 9999999, 1,
            &ok, this );
    if ( ok ) {
		granularity=l;
		gen.setGranularity(granularity);
    } else {
        
    }
  usettings.writeEntry("/soxapproach/cdi/granularity",granularity);
}

void Soxgui::cdiTimerDone()
{
/* Change gain values based on xdt */
//cout << "Updating output to ILS:";

  double mxtd=current->getxtd();
  if(current->getilsaxis()) //localizer on
  {
    //printf("localizer on\n");
    mxtd=current->getxtd();
  }
  else
  {
    //printf("glideslope on\n");
    mxtd=-current->getglideslope();
  }


	if (!gen.isOn)
	{gen.setmaxGainLeft(cdigain);
			gen.setmaxGainRight(cdigain);        
			gen.isOn=true;
			}	// Now adjust gain settings on CDI 
	double interv=odist/granularity;
	if (signalmodel==0)
	{
	if (mxtd==0)
		gen.steerPlane(granularity,granularity);
	else if (mxtd < 0){
		// target is right of plane
		if (mxtd < -odist)
			mxtd=-odist;

		int r_amt=granularity-((-mxtd)/interv);
		if (r_amt<0) r_amt=0;
		int l_amt=granularity;
		//std::cout << "Right target: " << l_amt << " " << r_amt << " " << mxtd <<std::endl;
		gen.steerPlane(l_amt,r_amt);
	}
	else{
		// target is left of plane
		if (mxtd > odist)
			mxtd=odist;
		int r_amt=granularity;
		int l_amt=granularity-(mxtd/interv);
		if (l_amt<0) l_amt=0;
		//std::cout << "Left target: " << l_amt << + " " << r_amt << " " << mxtd << std::endl;
		gen.steerPlane(l_amt,r_amt);
	}
	}
	else if (signalmodel==1)
	{
		// gaussian signal model
		double r_amt,l_amt;
		if (mxtd == 0)
		{
			gen.steerPlane(cdileftgain,cdirightgain);
		}
		else if (mxtd < 0)
		{
			// target right of plane
			r_amt=exp(- pow(-mxtd+mu,2)/(2*pow(sigma,2)) )*(double)cdirightgain;
			l_amt=exp(- pow(-mxtd-mu,2)/(2*pow(sigma,2)) )*(double)cdileftgain;

		}
		else if (mxtd > 0)
		{
			// target left of plane
			r_amt=exp(- pow(mxtd-mu,2)/(2*pow(sigma,2)) )*(double)cdirightgain;
			l_amt=exp(- pow(mxtd+mu,2)/(2*pow(sigma,2)) )*(double)cdileftgain;
		}
		//std::cout << "Vars: " << mxtd << " " << mu << " " << sigma << std::endl;
		std::cout << l_amt << " " << r_amt << std::endl;
		gen.steerPlane((int)l_amt,(int)r_amt);
	}
	
	
}

void Soxgui::slotSetSwathWidth()
{
  bool ok;

  double dtemp = QInputDialog::getDouble("Swath width","Enter swath width (ft)",
                 swathwidth/FT2M,0.0,5000.0,0,&ok,this);
  if (ok)
    swathwidth = dtemp*FT2M;
}


void Soxgui::slotSetLeader()
{
  bool ok;

  double dtemp = QInputDialog::getDouble("Leader line",
                 "Enter leader line length (sec)",
                 current->getleader(),0.0,5000.0,0,&ok,this);
  if (ok)
    current->setleader(dtemp);
}


void Soxgui::slotWPFile()
{

  // If waypoint file not already open, allow user to open one
  if (!wpflag)
  {
    cout << "waypoint file not already open\n";
    wpname = QFileDialog::getOpenFileName(".",
                       "approach (*.approach);;All (*.*)",
                       this,"waypoint","Select a waypoint file");
    wpfile = new QFile(wpname);
    if (wpfile->exists())
    {
      if (wpflag)
        waypoint->empty();
      ingest_waypoint(wpfile,waypoint,touchdown);
      wpflag = true;
      stemp = QString("Close waypoint file (");
      stemp.append(wpname);
      stemp.append(")");
      filemenu->changeItem(idwp,stemp);
    }
    //delete target4file;
  }

  // Otherwise null the waypoint list
  else
  {
    cout << "waypoint file already open\n";
    waypoint->empty();
    wpflag = false;
    filemenu->changeItem(idwp,"Open Waypoint file");
    wpname = "";
  }

}


void Soxgui::slotGPSClosed()
{
  stemp.sprintf("Connection to GPS network server closed by the server");
  QMessageBox::critical(this,"GPS Connection Closed",stemp);
}


void Soxgui::slotGPSError(int err)
{
  if (err==QSocket::ErrConnectionRefused)
    stemp.sprintf("Connection to GPS server refused\n");
  else if (err==QSocket::ErrHostNotFound)
    stemp.sprintf("GPS server host not found\n");
  else if (err==QSocket::ErrSocketRead)
    stemp.sprintf("Socket read from GPS server failed");
  QMessageBox::critical(this,"GPS Network Error",stemp);
}


void Soxgui::about()
{
  QMessageBox::about(this,"About SOXCDI",
                     "SOXCDI\nAuthors:\tJohn Sonntag\n\tRobbie Russell\n"
		     "Released: 10 August 2009\n"
		    );
}
