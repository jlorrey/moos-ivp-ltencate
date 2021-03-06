/************************************************************/
/*    NAME: Lauren TenCate                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

//#include <cstdint>
#include <iterator>
#include <list>
#include <string>
#include "MBUtils.h"
#include "PrimeFactor.h"
// #include "PrimeEntry.h"


using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  // initialize variables so that they're what I expect
  m_recieved_index   = 0;
  m_calculated_index = 0;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}


//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();


    if (key == "NUM_VALUE")
      {
	
	PrimeEntry mprime;
	string sval = msg.GetString();
	uint64_t numerical_val = strtoul(sval.c_str(), NULL, 0);

	m_recieved_index += 1;
        mprime.setOriginalVal(numerical_val);
	mprime.setRecievedIndex(m_recieved_index);
	mprime.setCalculatedIndex(0);
	mprime.setDone(false);
	mprime.m_started = false;
	mprime.setRTime(MOOSTime()); //could've also initilized these with the constructor, unclear if there's a difference or preferred practice here.
	
	m_num_val_messages.push_back(mprime);
      }

    else
      {
	cout << "Incorrect message type for this app. See help" << endl;
      } 

    
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  
  
  if (m_num_val_messages.size() > 0)
    // do you have a number to read?
    {
 
      // cout << "out of the for" << endl;
      for (std::list<PrimeEntry>::iterator it = m_num_val_messages.begin(); it != m_num_val_messages.end();)
      	{
          PrimeEntry& entry = *it;
	  
	  entry.factor(10000);
	  entry.setCTime(MOOSTime());
	  
      	  if (entry.done())
      	    {
	      m_calculated_index += 1;
	      entry.setCalculatedIndex(m_calculated_index);
      	      string num_result = entry.getReport();
	      
      	      m_Comms.Notify("PRIME_RESULT", num_result);
	      it = m_num_val_messages.erase(it);
      	    }
	  else {
	    entry.m_started = true;
	    entry.m_part_way += 10000;
	    ++it;
	  }
      	}
      }
  
  return(true);
    
}
//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open



bool PrimeFactor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string original_line = *p;
      string param = stripBlankEnds(toupper(biteString(*p, '=')));
      string value = stripBlankEnds(*p);
      
      if(param == "FOO") {
        //handled
      }
      else if(param == "BAR") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
  m_Comms.Register("PRIME_RESULT",0);
  m_Comms.Register("NUM_VALUE",0); // Register("FOOBAR", 0);
}

