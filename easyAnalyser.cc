#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

bool debug = true;

struct event {
  int index;
  int vote;
  int verified;
  float confidence;
};

std::vector<float> split_to_float (const std::string &s, char delim) {
  std::vector<float> result;
  std::stringstream ss (s);
  std::string item;

  while (getline (ss, item, delim)) {
    result.push_back (std::stof(item));
  }

  return result;
}

int main(int argc, char *argv[]){
  if (argc!=2){
    std::cout << "[ERROR] Please provide a data file path in input" <<std::endl;
    exit(1);
  }

  std::string datafile (argv[1]);

  std::fstream f;
  f.open (datafile.c_str());
  if (!f.is_open ()){
    std::cout << "[ERROR] easyAnalyser: data file " << datafile << " not foud: exit" <<std::endl;
    exit(1);
  }

  std::cout << "Reading datafile: " << datafile << std::endl;
  std::string line;
  std::vector<event> events;
  int tot_events = 0;
  float tot_weighted_events = 0;
  int i = 0;
  int total_no = 0;
  float total_weighted_no = 0;
  while (getline (f,line)){
    if (line.find ("#")==0) continue;
    if (line=="") continue;
    std::vector <float> dataline;
    dataline = split_to_float (line,' ');
    event tmp;
    tmp.index = i; tmp.vote = dataline[0];
    tmp.verified = dataline[1]; tmp.confidence = dataline[2];
    events.push_back (tmp);
    i++;
    tot_events++;
    if (tmp.vote == 0) { total_no++; total_weighted_no += (tmp.verified==1) ? 2*tmp.confidence : tmp.confidence;}
    tot_weighted_events += (tmp.verified==1) ? 2*tmp.confidence : tmp.confidence;
    if (debug) std::cout << "  DEBUG vote = " << tmp.vote << " verified " << tmp.verified << " confidence " << tmp.confidence << std::endl;

  }
  std::cout << "  Number of events (raw) = " << tot_events << std::endl;
  std::cout << "  Number of events (weighted) = " << tot_weighted_events << std::endl;
  std::cout << "    > Number of no (raw) = " << total_no << " (" << ((float)total_no/tot_events) << ")" << std::endl;
  std::cout << "    > Number of no (weighted) = " << total_weighted_no << " (" << ((float)total_weighted_no/tot_weighted_events) << ")" << std::endl;




}
