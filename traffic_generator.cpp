/* Project 2 for Data comm */
/* Traffic Generator */
/* Written by Bolong Zhang & Bowen Li */


#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<random>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

/* traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed] */
/* [seed] is the optional parameters */
//class for traffic file line
class Package_line{
private:
  int pkt_id;
  int src_node;
  int dst_node;
  int pkt_size;
  int time;
public:
  Package_line(int pi, int sn, int dn, int ps, int t): pkt_id(pi), src_node(sn), dst_node(dn), pkt_size(ps), time(t){}
  const int get_pkt_id() const {return pkt_id;}
  const int get_src_node() const {return src_node;}
  const int get_dst_node() const {return dst_node;}
  const int get_pkt_size()const {return pkt_size;};
  const int get_time()const {return time;};
  void print_line(){
    std::cout << pkt_id << " " << src_node << " " << dst_node << " " << pkt_size << " " << time << std::endl;
  }
};

int main(int argc, char *argv[]){

  int seed; //seed
  if(argc == 6){
    seed = atoi(argv[5]);
  }
  std::mt19937 gen1(seed);

  int num_node = atoi(argv[1]);
  int pkt_size = atoi(argv[2]);
  double offered_load = atof(argv[3]);
  int num_pkts_per_node = atoi(argv[4]);

  int gap = int((pkt_size * num_node)/offered_load + 0.5) - pkt_size; // compute the gap
  std::vector<Package_line> traffic_file; //store the
  traffic_file.reserve(10000); //reserve some space for the vector
  std::uniform_int_distribution<> dis(0, gap * 2); //unifrom distribution [0, 2 * gap]
  std::uniform_int_distribution<> dis2(0, num_node-1);
  for(int i = 0 ; i < num_node; i++){
    int last = 0;
    for(int j = 0; j < num_pkts_per_node; j++){
      int pkt_id = i * num_pkts_per_node + j;
      int src_node = i;
      int dst_node, time;
      if(argc == 6){
        time = last + dis(gen1);
        dst_node = dis2(gen1);
      }else{
        time = last + dis(gen);
        dst_node = dis2(gen);
      }
      if(dst_node >= i) dst_node += 1;
      last = time;
      Package_line pl(pkt_id, src_node, dst_node, pkt_size, time);
      traffic_file.push_back(pl);
    }
  }

  std::ofstream output;
  output.open("traffic_file", std::ios::out);

//	std::sort(traffic_file.begin(), traffic_file.end(), Compare_line);
  std::sort(traffic_file.begin(), traffic_file.end(), [](const Package_line & one, const Package_line & two){return one.get_time() < two.get_time();});
  output << traffic_file.size() << std::endl;
  for(size_t i = 0; i < traffic_file.size() ;i++){
    //traffic_file[i].print_line();
    output << traffic_file[i].get_pkt_id() << " " << traffic_file[i].get_src_node() << " " << traffic_file[i].get_dst_node() << " " << traffic_file[i].get_pkt_size() << " " << traffic_file[i].get_time() << std::endl;
  }
  output.close();
  return 0;

}
