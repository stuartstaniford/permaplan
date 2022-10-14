// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Interface to the Global Historical Climatology Network - Daily data.  This provides 
// global harmonized data for recent daily climate from all over the world.  The purpose 
// of this class is to provide the data in a usable form to the rest of permaserv.  For 
// more on the dataset:
// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html

#include "GHCNDatabase.h"
#include "ClimateInfo.h"
#include "Logging.h"
#include "loadFileToBuf.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <regex>
#include <zlib.h>


// =======================================================================================
// Static variables etc

char byStationUrl[] = "https://www.ncei.noaa.gov/pub/data/ghcn/daily/by_station/";


// =======================================================================================
/// @brief Constructor

GHCNDatabase::GHCNDatabase(char* path): dbPath(path)
{
  readStations();
  checkFileIndex();
}


// =======================================================================================
/// @brief Destructor

GHCNDatabase::~GHCNDatabase(void)
{
}


// =======================================================================================
/// @brief Check if a file is too old, and update from a URL if necessary.
///
/// @returns True if the file is now ok, false if it's missing or old
/// @param fileName - Char* pointer to the fileName
/// @param 
/// @todo This should probably be consolidated into the ResourceManager and done on a 
/// queue.
/// @todo This should download to a temporary filename and then swap only when the download
/// is done.

bool GHCNDatabase::checkUpdateFile(char* fileName, char* url, float maxAge)
{
  float fileAge = getFileAge(fileName);
  if(fileAge < -0.0f || fileAge > maxAge)
   {
    if(fileAge == -1.0f)
      LogClimateDbOps("Could not stat file %s:%s\n", fileName, strerror(errno));
    if(fetchFile(url, fileName))
     {
      LogClimateDbOps("Refreshed file %s after %.2f days\n", 
                                                        fileName, fileAge/24.0f/3600.0f);
     }
    else
     {
      LogClimateDbErr("Could not refresh file %s from %s.\n", fileName, url);
      return false;
     }
   } 
  else
    LogGHCNExhaustive("File %s is still valid after %.2f days\n", fileName, 
                                                                  fileAge/24.0f/3600.0f); 
    
  return true;
}


// =======================================================================================
/// @brief Get the directory listing of all the station data files if necessary.
///
/// This is the HTML page at this url, which has a directory listing
/// of file names and sizes which we use to estimate how big a buffer we need to download
/// any particular file.
/// https://www.ncei.noaa.gov/pub/data/ghcn/daily/by_station/

#define MAX_STATION_FILE_AGE  604800.0f   // 7*24*3600

void GHCNDatabase::checkFileIndex(void)
{
  char fileName[128];

  //ghcnd-stations.txt
  char stationUrl[] = "https://www.ncei.noaa.gov/pub/data/ghcn/daily/ghcnd-stations.txt";
  if(snprintf(fileName, 128, "%s/ghcnd-stations.txt", dbPath) >= 128)
   {
    LogClimateDbErr("Overflow in ghcnd-stations.txt in GHCNDatabase::checkFileIndex.\n");
    return;
   };
  checkUpdateFile(fileName, stationUrl, MAX_STATION_FILE_AGE);  

  // Stations index.html
  if(snprintf(fileName, 128, "%s/by_station/stations.html", dbPath) >= 128)
   {
    LogClimateDbErr("Overflow in fileName in GHCNDatabase::checkFileIndex.\n");
    return;
   };
  checkUpdateFile(fileName, byStationUrl, MAX_STATION_FILE_AGE);  
  
  unless(parseStationFileWithC(fileName))
   {
    LogClimateDbErr("Failing due to missing or bad station file %s.\n", fileName);
    exit(-1);
   }
}


// =======================================================================================
/// @brief Parse the directory listing of all the station data files.
///
/// This is called by checkFileIndex to do the actual parsing
/// @params fileName The path of the file to parse
/// @returns True if all went well, false if we couldn't parse the file

bool GHCNDatabase::parseStationFileWithC(char* fileName)
{
#ifdef LOG_GHCN_EXHAUSTIVE
  Timeval parseStart;
  parseStart.now();
#endif
  FILE* file = fopen(fileName, "r");
  unless(file)
   {
    LogClimateDbErr("Could not open station file %s to parse.\n", fileName);
    return false;
   }
  char buf[256];
  unsigned stationCount = 0;
  
  while(fgets(buf, 256, file))
   {
    //<tr><td><a href="AF000040930.csv.gz">AF000040930.csv.gz</a></td><td align="right">2022-10-09 14:02  </td><td align="right"> 30K</td><td>&nbsp;</td></tr>
    char* parse;
    
    // Confirm the initial form of the line is one of the table rows
    unless(strncmp(buf, "<tr><td><a href=\"", 17) == 0)
      continue;
    parse = buf + 17;
    unless(parse[11] == '.')
      continue;
    
    // Extract the station id and find the station
    parse[11] = '\0';
    GHCNStation* station;
    if(stationsByName.count(parse))
      station = stationsByName[parse];
    else
     {
      LogGHCNExhaustive("Failed to match %s in stationsByName.\n", parse);        
      continue;
     }
    
    // Extract the size field
    parse += 12;
    char* end = index(parse, '&');
    unless(end && (end - parse > 20))
      continue;
    end -= 9;
    *end = '\0';
    parse = rindex(parse, '>');
    unless(parse && (end - parse > 1))
      continue;
    parse++;
    
    // Analyze the size field
    if(*(end-1) == ' ')
     {
      *(--end) = '\0';
      station->fileBufSize = (unsigned)(atof(parse)+16.0f);
     }
    else if(*(end-1) == 'K')
     {
      *(--end) = '\0';
      station->fileBufSize = (unsigned)(1024.0f*(atof(parse)+1.0f));
     }
    else if(*(end-1) == 'M')
     {
      *(--end) = '\0';
      station->fileBufSize = (unsigned)(1048576.0f*(atof(parse)+0.15f));
     }    
    else
      LogClimateDbErr("Got bad size %s\n", parse);
    
    stationCount++;
    }
  
  fclose(file);
#ifdef LOG_GHCN_EXHAUSTIVE
  Timeval parseEnd;
  parseEnd.now();
  LogClimateDbOps("Parsing %d stations from file %s took %.3lf s.\n", 
                                          stationCount, fileName, parseEnd - parseStart);
#endif

  return true;  
}


// =======================================================================================
/// @brief Parse the directory listing of all the station data files.
///
/// This is an old version that has been supplanted by parseStationFileWithC.  This
/// version used the C++ std::regex library, and took 40 seconds to parse the file.  The
/// C based parsing version above took 0.139 seconds.
/// See https://www.reddit.com/r/cpp/comments/e16s1m/what_is_wrong_with_stdregex/
/// and https://stackoverflow.com/questions/70583395/why-is-stdregex-notoriously-much-slower-than-other-regular-expression-librarie
/// for more on why std::regex turns out to suck.
/// @todo Might try rewriting this using pcre2 library (already installed via brew).
/// @params fileName The path of the file to parse
/// @returns True if all went well, false if we couldn't parse the file.

bool GHCNDatabase::parseStationFileRegEx(char* fileName)
{
#ifdef LOG_GHCN_EXHAUSTIVE
  Timeval parseStart;
  parseStart.now();
#endif
  FILE* file = fopen(fileName, "r");
  unless(file)
   {
    LogClimateDbErr("Could not open station file %s to parse.\n", fileName);
    return false;
   }
  char buf[256];
  std::regex stationIdRegEx("^<tr><td><a href=\"([a-zA-Z0-9\\-_]+)\\.csv\\.gz\">");
  std::regex sizeRegEx("right\">\\s*(\\d*\\.*\\d+)(K|M| )</td><td>.nbsp.</td></tr>\\s*$");
  std::smatch match;
  
  while(fgets(buf, 256, file))
   {
    std::string bufString = std::string(buf);

    if(std::regex_search(bufString, match, stationIdRegEx) && match.size() > 1)
     {
      GHCNStation* station;
      if(stationsByName.count(match.str(1)))
         station = stationsByName[match.str(1)];
      else
       {
        LogGHCNExhaustive("Failed to match %s in stationsByName.\n", match.str(1).c_str());        
        continue;
       }
      if(std::regex_search(bufString, match, sizeRegEx) && match.size() > 2)
       {
        if(match.str(2).c_str()[0] == ' ')
          station->fileBufSize = (unsigned)(atof(match.str(1).c_str())+16.0f);
        else if(match.str(2).c_str()[0] == 'K')
          station->fileBufSize = (unsigned)(1024.0f*(atof(match.str(1).c_str())+1.0f));
        else if(match.str(2).c_str()[0] == 'M')
          station->fileBufSize = (unsigned)(1048576.0f*(atof(match.str(1).c_str())+0.15f));
        else
          LogClimateDbErr("Got bad size %s %s\n", match.str(1).c_str(), 
                                                                  match.str(2).c_str());
       }
      else
        LogClimateDbErr("Couldn't match size for station id %s\n", station->id);
     }
    else
      LogClimateDbErr("Couldn't find station id in line %s\n", buf);
    }
  
  fclose(file);
#ifdef LOG_GHCN_EXHAUSTIVE
  Timeval parseEnd;
  parseEnd.now();
  LogClimateDbOps("Parsing the station file %s took %.3lf s.", 
                                                fileName, parseEnd - parseStart);
#endif

  return true;  
}


// =======================================================================================
/// @brief Read all the stations into our data structures.
///
/// For more detail, search for 'IV. FORMAT OF "ghcnd-stations.txt"' in
///  https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

void GHCNDatabase::readStations(void)
{
  // Figure out the name of the station file
  char stationFileName[128];
  if(snprintf(stationFileName, 128, "%s/ghcnd-stations.txt", dbPath) >= 128)
   {
    LogClimateDbErr("Overflow in stationFileName in GHCNDatabase::readStations.\n");
    return;
   };
 
  // Open the station file
  FILE* stationFile = fopen(stationFileName, "r");
  unless(stationFile)
   {
    LogClimateDbErr("Couldn't open GHCNDatabase station file %s.\n", stationFileName);
    return;
   };
  
  // Loop over the lines in the file
  char buf[96];
  while(fgets(buf, 96, stationFile))
   {
    // Read a line and create a GHCNStation
    GHCNStation* station = new GHCNStation(buf);
#ifdef LOG_CLIMATE_DB_ERR
    int line = 1;
#endif
    unless(station)
     {
      LogClimateDbErr("Couldn't create new station in line %d of station file %s.\n",
                                                            line++, stationFileName);
      return;
     }
    
    // Store the GHCNStation in our R-tree data structure
    stationTree.Insert(station->latLong, station->latLong, station);
    
    // Store it also in the index by name
    stationsByName.insert({std::string(station->id), station});
   }
  
  // Close up and go home
  fclose(stationFile);
}


// =======================================================================================
/// @brief Callback for use in R-tree search in GHCNDatabase::getStations.

bool searchCallback(GHCNStation* station, void* context)
{
  GHCNDatabase* db = (GHCNDatabase*)context;
  db->stationResults.push_back(station);
  return true; // keep going  
}


// =======================================================================================
/// @brief Retrieve the closest stations from our data structures.
///
/// @param lat A float containing the latitude to search for
/// @param longT A float containing the longtitude to search for
/// @todo We are searching on raw lat/long squares here, which will tend to get skeevy
/// towards the poles - might want to add a cos(lat) factor.
/// @todo We currently just take the first result, rather than examining results for 
/// altitude etc.

void GHCNDatabase::getStations(float lat, float longT)
{
  float searchBound = 0.5f;  // half the side of the search rectangle in degrees.
  float searchMin[2];
  float searchMax[2];
  
  stationResults.clear();
  
  while(1) // keep adjusting the search rectangle until we get a good result
   {
    searchMin[0] = lat    - searchBound;
    searchMin[1] = longT  - searchBound;
    searchMax[0] = lat    + searchBound;
    searchMax[1] = longT  + searchBound;
    
    int hits = stationTree.Search(searchMin, searchMax, searchCallback, this);
    LogClimateDbOps("Got %d results in search with %.4f degrees of [%.4f, %.4f].\n",
                      hits, searchBound, lat, longT);
    if(hits)
     {
      break; // temp
     }
    else
      searchBound *= 1.4142f;
   }
}


// =======================================================================================
/// @brief Form the filename for a single CSV file.
///
/// @param station A pointer to the GHCNStation record which is requested
/// @param fileName A pointer to the buffer to write the fileName in.
/// @param len The size of the buffer
/// @returns True if all is well, false if we couldn't create the fileName in the space

bool GHCNDatabase::snprintCSVFileName(char* fileName, int len, GHCNStation* station)
{
  if(snprintf(fileName, len, "%s/%s.csv.gz", dbPath, station->id) >= len)
   {
    LogClimateDbErr("Overflow in csv filename for %s in"
                                      " GHCNDatabase::snprintCSVFileName.\n", station->id);
    return false;
   };
  return true;
}


// =======================================================================================
/// @brief Fetch a single .csv file from the GHCN repository.
///
/// @param station A pointer to the GHCNStation record which is requested
/// @returns True if we successfully fetched it, false if we failed.

#define MAX_CSV_FILE_AGE  5184000.0f   // 60*24*3600

bool GHCNDatabase::checkCSVFile(GHCNStation* station)
{    
  char fileName[128];
  unless(snprintCSVFileName(fileName, 128, station))
    return false;
  
  char url[128];
  if(snprintf(url, 128, "%s%s.csv.gz", byStationUrl, station->id) >= 128)
   {
    LogClimateDbErr("Overflow in csv url for %s in GHCNDatabase::checkCSVFile.\n",
                                                                              station->id);
    return false;
   };

  return checkUpdateFile(fileName, url, MAX_CSV_FILE_AGE);  
}


// =======================================================================================
/// @brief Handle a single line of a .csv file.
///
/// @param buf A char* pointing to buffer with the line.
/// @param station A pointer to the GHCNStation record for which we are reading.  Note
/// that it's climate variable should be valid.
/// @param fileName The name of the file (mainly for logging).
/// @param line An integer line number (mainly for logging).
/// @returns True if we read the line ok, false if we encountered a fatal error.

bool GHCNDatabase::readCSVLine(char* buf, GHCNStation* station, char* fileName, int line)
{
  // USC00304174,18930101,TMAX,67,,,6,
  int strLen = strlen(buf);
  if(strLen < 32)
   {
    LogClimateDbErr("Short line %d of csv file %s.\n", line, fileName);
    return false;
   }

  // Station id
  if(buf[11] != ',')
   {
    LogClimateDbErr("Bad station id comma in line %d of csv file %s.\n", 
                                                                      line, fileName);
    return false;
   }
  buf[11] = '\0';
  unless(strncmp(buf, station->id, 11) == 0)
   {
    LogClimateDbErr("Station id %s does not match %s in csv file %s, line %d.\n", 
                                                  buf, station->id, fileName, line);
    return false;
   }

  // Date
  if(buf[20] != ',')
   {
    LogClimateDbErr("Bad date comma in line %d of csv file %s.\n", line, fileName);
    return false;
   }
  buf[20] = '\0';
  char dateBuf[5];
  int year, month, day, yearDay;
  for(int i=0;i<4;i++)
    dateBuf[i] = buf[12+i];
  dateBuf[4] = '\0';
  year      = atoi(dateBuf);
  month     = 10*(buf[16] - '0') + (buf[17] - '0');  // converting ASCII values to numbers
  day       = 10*(buf[18] - '0') + (buf[19] - '0');
  yearDay   = yearDays(year, month, day);
  unless(yearDay >= 0 && yearDay < 366)
   {
    LogClimateDbErr("Out of array range date, %s (%d/%d/%d), in line %d of csv file %s.\n", 
                                                  buf+12, month, day, year, line, fileName);
    return false;
   }
  
  //Find the right climateDay
  ClimateInfo* climInfo = station->climate;
  if(year < climInfo->startYear || year >= climInfo->endYear)
   {
    //LogGHCNExhaustive("Ignoring out of range year %d on line %d of csv file %s.\n",         
    //                                                               year, line, fileName);
    return true;
   }
  ClimateDay* climDay = climInfo->climateYears[year-climInfo->startYear] + yearDay;

  //if(strcmp(station->id, "US1NYTG0015") == 0)
  //  printf("Recording data for yearDay %d from date %s.\n", yearDay, buf+12);  
  
  // Observation type, observation
  if(buf[25] != ',')
   {
    LogClimateDbErr("Bad observation type comma in line %d of csv file %s.\n", 
                                                                    line, fileName);
    return false;
   }
  buf[25] = '\0'; // comma after observation type
  char* commaLoc = index(buf + 26, ','); // find the comma after the observation
  unless(commaLoc)
   {
    LogClimateDbErr("Couldn't find observation in line %d of csv file %s.\n", 
                                                                    line, fileName);
    return false;
   }
  *commaLoc = '\0';
  float observation = atof(buf+26);

  if(strcmp(buf + 21, "TMAX") == 0)
   {
    climDay->hiTemp = observation/10.0f;
    /// @todo check validity of value
    climDay->flags |= HI_TEMP_VALID;
   }  
  else if(strcmp(buf + 21, "TMIN") == 0)
   {
    climDay->lowTemp = observation/10.0f;
    /// @todo check validity of value
    climDay->flags |= LOW_TEMP_VALID;
   }  
  else if(strcmp(buf + 21, "PRCP") == 0)
   {
    climDay->precip = observation/10.0f;
    /// @todo check validity of value
    climDay->flags |= PRECIP_VALID;
   }  
  else if(strcmp(buf + 21, "SNOW") == 0)
   {
    ;
   }  
   
  // Flags
  
  return true;
}


// =======================================================================================
/// @brief Read a single .csv file.
///
/// @param station A pointer to the GHCNStation record for which we are reading
/// @returns The number of complete records we read
/// @todo We create the ClimateInfo with hard-coded years that are a temp hack.

// https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

int GHCNDatabase::readOneCSVFile(GHCNStation* station)
{
  // Create the climateInfo if needed
  unless(station->climate)
    station->climate = new ClimateInfo(2000, 2022);
  
  // Get the fileName
  char fileName[128];
  unless(snprintCSVFileName(fileName, 128, station))
    return -1;

  // Loop variables
  char buf[128];
  int line = 0;

  // Check if we are gzipped or not
  char* dotLoc = rindex(fileName, '.');
  if(!dotLoc || strcmp(dotLoc, ".gz"))
   {
    // Open the file which is not gzipped
    FILE* file = fopen(fileName, "r");
    unless(file)
     {
      LogClimateDbErr("Couldn't open station csv file %s.\n", fileName);
      return -1;
     }
  
    // Loop over the lines in the file
    while(fgets(buf, 128, file) && ++line)
      unless(readCSVLine(buf, station, fileName, line))
       {
        LogClimateDbErr("Couldn't read station csv file %s.\n", fileName);
        fclose(file);
        return -1;
       }
    fclose(file);
   }
  else
   {
    // Open the gzip file
    // Code idea borrowed from:
    // https://stackoverflow.com/questions/35091855/how-to-sequentially-parse-in-\
    // c-a-gzip-compressed-text-file-without-fully-decompr
    gzFile file = gzopen(fileName, "rb");
    unless(file)
     {
      LogClimateDbErr("Couldn't open station csv.gz file %s.\n", fileName);
      return -1;
     }

    // Loop over the lines in the file
    while(gzgets(file, buf, 128) && ++line)
    unless(readCSVLine(buf, station, fileName, line))
     {
      LogClimateDbErr("Couldn't read station csv.gz file %s.\n", fileName);
      gzclose(file);
      return -1;
     }
    gzclose(file);
  }

  // Close up and go home
  unsigned total, valid;
  station->climate->countValidDays(total, valid);
  LogClimateDbOps("Station %s has %d valid days from %d total.\n", station->id, 
                                                                            valid, total);
  return valid;
}


// =======================================================================================
/// @brief Create a single station from a line in the file.
///
/// @param buf A char* pointing to the line buffer

// ------------------------------
// Variable   Columns   Type
// ------------------------------
// ID            1-11   Character
// LATITUDE     13-20   Real
// LONGITUDE    22-30   Real
// ELEVATION    32-37   Real
// STATE        39-40   Character
// NAME         42-71   Character
// GSN FLAG     73-75   Character
// HCN/CRN FLAG 77-79   Character
// WMO ID       81-85   Character
// ------------------------------

GHCNStation::GHCNStation(char* buf)
{
  strncpy(id, buf, 11);
  id[11] = '\0';
  for(int i = 10; isspace(id[i]); i--)
    id[i] = '\0';
  
  strncpy(name, buf+41, 30);
  name[30] = '\0';
  for(int i = 29; isspace(name[i]); i--)
    name[i] = '\0';
  
  latLong[0]  = atof(buf+12);
  latLong[1]  = atof(buf+21);
  elevation   = atof(buf+31);  // in meters, as we are in permaserv

  // Make sure variables that will be filled out later are not random garbage
  fileBufSize = 0u;
  climate     = NULL;
  
  LogGHCNExhaustive("Read station %s (%s) at [%.4f, %.4f], el: %.1fm.\n",
                                              id, name, latLong[0], latLong[1], elevation);
}


// =======================================================================================