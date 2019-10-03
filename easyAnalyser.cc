#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

bool debug = true;

/*
 Event declaration 
 */
struct event {
  int index;
  int vote;
  int verified;
  float confidence;
};

/*
  Utility routine for reading data
 */
std::vector<float> split_to_float (const std::string &s, char delim) {
  std::vector<float> result;
  std::stringstream ss (s);
  std::string item;

  while (getline (ss, item, delim)) {
    result.push_back (std::stof(item));
  }

  return result;
}

/*____________________________________________________*/


int main(int argc, char *argv[]){
  if (argc!=2){
    std::cout << "[ERROR] Please provide a data file path in input" <<std::endl;
    exit(1);
  }

  std::string datafile (argv[1]);

  // Opening file in input
  std::fstream f;
  f.open (datafile.c_str());
  if (!f.is_open ()){
    std::cout << "[ERROR] easyAnalyser: data file " << datafile << " not foud: exit" <<std::endl;
    exit(1);
  }

  // Main container
  std::map <std::string, std::vector<event>> dataset;
  std::map <std::string, float> numbers;

  
  // Storing the nominal file
  std::cout << "Reading datafile: " << datafile << std::endl;
  std::string line;
  dataset ["nominal"] ={};
  numbers["tot_events_nominal"] = 0;
  numbers["tot_weighted_events_nomnal"] = 0;
  int i = 0;
  numbers["total_no_nominal"] = 0;
  numbers["total_weighted_no_nominal"] = 0;
  while (getline (f,line)){
    if (line.find ("#")==0) continue;
    if (line=="") continue;
    std::vector <float> dataline;
    dataline = split_to_float (line,' ');
    event tmp;
    tmp.index = i; tmp.vote = dataline[0];
    tmp.verified = dataline[1]; tmp.confidence = dataline[2];
    dataset["nominal"].push_back (tmp);
    i++;
    numbers["tot_events_nominal"]++;
    if (tmp.vote == 0) { numbers["total_no_nominal"]++; numbers["total_weighted_no_nominal"] += (tmp.verified==1) ? 2*tmp.confidence : tmp.confidence;}
    numbers["tot_weighted_events_nominal"] += (tmp.verified==1) ? 2*tmp.confidence : tmp.confidence;
    if (debug) std::cout << "  DEBUG vote = " << tmp.vote << " verified " << tmp.verified << " confidence " << tmp.confidence << std::endl;

  }

  std::cout << " DATASET: NOMINAL " <<  std::endl;
  std::cout << "  Number of events (raw) = " << numbers["tot_events_nominal"] << std::endl;
  std::cout << "  Number of events (weighted) = " << numbers["tot_weighted_events_nominal" ] << std::endl;
  std::cout << "    > Number of no (raw) = " << numbers["total_no_nominal"] << " (" << ((float)numbers["total_no_nominal"]/numbers["tot_events_nominal"]) << ")" << std::endl;
  std::cout << "    > Number of no (weighted) = " << numbers["total_weighted_no_nominal"] << " (" << ((float)numbers["total_weighted_no_nominal"]/numbers["tot_weighted_events_nominal"]) << ")" << std::endl;

  //// Preparing systematics
  // Syst on weight assignment: assumed max ( 50%, 0.5 ) of the total
  // weight = confidence*(1*(verified==0) + 2*(verified==1))
  // NOTE: maybe better to have 2 syst: on the factor two of verified and confidence, but this way easier for manual minimisation of Likelihood
  dataset["confidence_1up"] = {};
  dataset["confidence_1down"] = {};
  for (auto ev : dataset["nominal"]){
    event ev_1down = ev;
    ev_1down.verified = (ev_1down.verified==1) ? 0.5*ev_1down.verified : ev_1down.verified;
    ev_1down.confidence = (ev_1down.verified==1) ? ev_1down.confidence : ev_1down.confidence-0.5;
    dataset["confidence_1down"].push_back (ev_1down);
    event ev_1up = ev;
    ev_1up.verified = (ev_1up.verified==1) ? 1.5*ev_1up.verified : ev_1up.verified;
    ev_1up.confidence = (ev_1up.verified==1) ? ev_1up.confidence : ev_1up.confidence+0.5;
    dataset["confidence_1up"].push_back (ev_1up);
  }


}
