//
//  simulator.cpp
//  MAC protocol
//
//  Created by Bolong Zhang&Bowen Li on 10/30/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <queue>

struct packet {
  int pkt_id;
  int scr_node;
  int dst_node;
  int pkt_size;
  int time;
  int cls_flag;
  int new_time;
  int slot_time;
};
struct packet_temp{
  char pkt_id[50];
  char scr_node[50];
  char dst_node[50];
  char pkt_size[50];
  char time[100];
};
void convert_int (char *strLine,struct packet * packet_out) {
  struct packet_temp packet_out_temp;
  sscanf( strLine, "%s %s %s %s %s",packet_out_temp.pkt_id,packet_out_temp.scr_node,packet_out_temp.dst_node,packet_out_temp.pkt_size,packet_out_temp.time);
  packet_out->pkt_id=atoi(packet_out_temp.pkt_id);
  packet_out->scr_node=atoi(packet_out_temp.scr_node);
  packet_out->dst_node=atoi(packet_out_temp.dst_node);
  packet_out->pkt_size=atoi(packet_out_temp.pkt_size);
  packet_out->time=atoi(packet_out_temp.time);
  packet_out->new_time=packet_out->time;
  packet_out->cls_flag=0;
  if(packet_out->time % packet_out->pkt_size == 0){
    packet_out->slot_time = packet_out->time;
  }else{
    packet_out->slot_time = (packet_out->time / packet_out->pkt_size + 1) *  packet_out->pkt_size; //compute the slot id
  }
}

void ALOHA(struct packet *packet_out,int no){
    //channel=0, free,chnnel=1,busy
    int channel=0;
    int channel_begin=0;
    int channel_end=0;
    int temp=0;
    for(int i=0;i<no-1;i++){
        //when channel is free, sending the frame
        if(channel==0){
            printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[i].time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
            channel=1;
            channel_begin=packet_out[i].time;
            channel_end=channel_begin+packet_out[i].pkt_size-1;
            //if the time gap between next packer is higher than 100, this packet can be sent sucessfully, or cannot
            if(packet_out[i+1].time>channel_end){
                temp++;
                if(packet_out[i].pkt_id==packet_out[no-1].pkt_id){
                    temp--;
                }
                printf("Time: %d, Packet %d: %d %d %d %d finish sending: successfully transmitted\n",packet_out[i].time+packet_out[i].pkt_size-1,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                channel=0;
                //printf("%d\n",packet_out[i].pkt_id);
            }
            else{
                channel_end=packet_out[i].time+packet_out[i].pkt_size-1;
                printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed\n",packet_out[i].time+packet_out[i].pkt_size-1,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
            }
        }
        //when the channel is busy
        else {
            //this channel can be sent
            if(packet_out[i].time>channel_end){
                channel=0;
                channel_begin=packet_out[i].time;
                channel_end=channel_begin+packet_out[i].pkt_size-1;
                //go back
                i=i-1;
            }
            else{
                channel_end=packet_out[i].time+packet_out[i].pkt_size-1;
                printf("Time: %d, Packet %d: %d %d %d %d start sending: collision\n",packet_out[i].time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed\n",packet_out[i].time+packet_out[i].pkt_size-1,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                
            }
            
        }
    }
    if(packet_out[no-1].time>packet_out[no-2].time+packet_out[no-1].pkt_size-1){
        printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[no-1].time,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        printf("Time: %d, Packet %d: %d %d %d %d finish sending: successfully transmitted\n",packet_out[no-1].time+packet_out[no-1].pkt_size-1,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        temp++;
    }
    else{
        printf("Time: %d, Packet %d: %d %d %d %d start sending:collsion\n",packet_out[no-1].time,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed\n",packet_out[no-1].time+packet_out[no-1].pkt_size-1,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        
    }
    printf("%d packets succefully transmitted.\n",temp);
    printf("%d packets failed transmission due to collision.\n",no-temp);
    int  throughput=int(1000 * (float)(temp*packet_out[no-1].pkt_size)/(float)(packet_out[no-1].time +packet_out[no-1].pkt_size-1));
    printf("throughput %d kps\n",throughput);
    
    
}
void SLOTTED_ALOHA(struct packet *packet_out,int no){
    //channel=0, free,chnnel=1,busy
    //slotted aloha with slot time 100
    //slotted id starting with 0
    int channel=0;
    int channel_begin=0;
    int channel_end=0;
    int temp=0;
    std::queue<int> failed_list;
    for(int i=0;i<no-1;i++){
        //when channel is free, sending the frame
        
        while(!failed_list.empty()){
            int tt = failed_list.front();
            if((packet_out[i].slot_time < packet_out[tt].slot_time + packet_out[tt].pkt_size - 1)){ //&&
                break;
            }else if ( packet_out[i+1].slot_time > packet_out[tt].slot_time + packet_out[tt].pkt_size - 1){
                printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed\n",packet_out[tt].slot_time+packet_out[tt].pkt_size-1,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                failed_list.pop();
                
            }
        }
        
        if(channel==0){
            printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[i].slot_time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
            channel=1;
            channel_begin=packet_out[i].slot_time;
            channel_end=channel_begin+packet_out[i].pkt_size-1;
            //if the time gap between next packer is higher than 100, this packet can be sent sucessfully, or cannot
            if(packet_out[i+1].slot_time>channel_end){
                temp++;
                printf("Time: %d, Packet %d: %d %d %d %d finish sending: successfully transmitted\n",packet_out[i].slot_time+packet_out[i].pkt_size-1,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                channel=0;
            }
            else{
                failed_list.push(i);
                channel_end=packet_out[i].slot_time+packet_out[i].pkt_size-1;
            }
        }
        //when the channel is busy
        else {
            //this channel can be sent
            if(packet_out[i].slot_time>=channel_end){
                channel=0;
                channel_begin=packet_out[i].slot_time;
                channel_end=channel_begin+packet_out[i].pkt_size-1;
                //go back
                i=i-1;
            }
            else{
                channel_end=packet_out[i].slot_time+packet_out[i].pkt_size-1;
                printf("Time: %d, Packet %d: %d %d %d %d start sending: collision\n",packet_out[i].slot_time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                failed_list.push(i);
                
            }
            
        }
    }
    if(packet_out[no-1].slot_time>packet_out[no-2].slot_time){
        printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[no-1].slot_time,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        printf("Time: %d, Packet %d: %d %d %d %d finish sending: successfully transmitted\n",packet_out[no-1].slot_time+packet_out[no-1].pkt_size-1,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        temp++;
    }
    else{
        printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[no-1].slot_time,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        printf("Time: %d, Packet %d: %d %d %d %d start sending:collision\n",packet_out[no-1].slot_time,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed\n",packet_out[no-1].slot_time+packet_out[no-1].pkt_size-1,packet_out[no-1].pkt_id,packet_out[no-1].scr_node,packet_out[no-1].dst_node,packet_out[no-1].pkt_size,packet_out[no-1].time);
        
    }
    printf("%d packets succefully transmitted.\n",temp);
    printf("%d packets failed transmission due to collision.\n",no-temp);
    int throughput=int(1000* (float)(temp*packet_out[no-1].pkt_size)/(float)(packet_out[no-1].slot_time+packet_out[no-1].pkt_size-1));
    printf("throughput %d kps\n",throughput);

}
void CSMA_1(struct packet *packet_out,int no){
 
    int channel=1;
    int temp=0;
    int channel_end=1000000;
    std::queue<int> collision_list, back_up;
    bool start_flag = true;
    for(int i=0;i<no;i++){
        if(channel==0){
            //      std::cout << "Channel is idle right now! " << std::endl;
            printf("Time: %d, Packet %d: %d %d %d %d start sending\n",packet_out[i].new_time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
            printf("Time: %d, Packet %d: %d %d %d %d finish sending: successfully transmitted\n",packet_out[i].new_time+packet_out[i].pkt_size-1,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
            channel=1;
            channel_end=packet_out[i].new_time+packet_out[i].pkt_size-1;
            temp++;
        } else{
            //      std::cout << packet_out[i].time << " " << channel_end << std::endl;
            if(packet_out[i].time > channel_end && packet_out[i].time > packet_out[i-1].new_time){
                if(collision_list.size() == 0){
                    channel = 0;
                    //  std::cout << "Channel is reset to be idle right now! " << std::endl;
                    //packet_out[i].new_time=channel_end+1;
                    i = i -1;
                    continue;
                }else if(collision_list.size() == 1){
                    int tt = collision_list.front();
                    i = tt -1;
                    channel = 0;
                    collision_list.pop();
                    back_up.pop();
                    //std::cout << "Channel is reset to be idle right now! " << std::endl;
                    //packet_out[tt].new_time=channel_end+1;
                    continue;
                }else if(collision_list.size() >= 2){
                    start_flag = false;
                    int start = collision_list.front();
                    while(!collision_list.empty()){
                        int tt = collision_list.front();
                        if(start != 0){
                            printf("Time: %d, Packet %d: %d %d %d %d start sending: collison \n",channel_end +1,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        }else{
                            printf("Time: %d, Packet %d: %d %d %d %d start sending: collison \n",packet_out[tt].time ,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        }
                        collision_list.pop();
                        
                    }
                    while(!back_up.empty()){
                        int tt = back_up.front();
                        if(start != 0){
                            printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed \n",channel_end +packet_out[tt].pkt_size,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        }else{
                            printf("Time: %d, Packet %d: %d %d %d %d start sending: failed \n",packet_out[tt].time ,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        }
                        back_up.pop();
                    }
                    
                }
                i=i-1;
                //channel = 0;
                channel_end += packet_out[i].pkt_size;
                
            }else{
                if(!start_flag){
                    printf("Time: %d, Packet %d: %d %d %d %d is waitting for idle\n",packet_out[i].time,packet_out[i].pkt_id,packet_out[i].scr_node,packet_out[i].dst_node,packet_out[i].pkt_size,packet_out[i].time);
                }else{
                    channel_end=packet_out[i].new_time+packet_out[i].pkt_size-1;
                }
                collision_list.push(i);
                back_up.push(i);
                packet_out[i].new_time=channel_end+1;
                if( i == no-1 && collision_list.size() == 1) {
                    channel = 0;
                    i = i -1;
                }else if(i == no-1 && collision_list.size() >=2){
                    while(!collision_list.empty()){
                        int tt = collision_list.front();
                        printf("Time: %d, Packet %d: %d %d %d %d start sending: collison \n",channel_end +1,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        collision_list.pop();
                        
                    }
                    while(!back_up.empty()){
                        int tt = back_up.front();
                        printf("Time: %d, Packet %d: %d %d %d %d finish sending: failed \n",channel_end +packet_out[tt].pkt_size,packet_out[tt].pkt_id,packet_out[tt].scr_node,packet_out[tt].dst_node,packet_out[tt].pkt_size,packet_out[tt].time);
                        back_up.pop();
                    }
                    
                }
                
            }
        }
    }
    
    printf("%d packets succefully transmitted.\n",temp);
    printf("%d packets failed transmission due to collision.\n",no-temp);
    int throughput=int(1000 * (float)(temp*packet_out[no-1].pkt_size)/(float)(channel_end));
    printf("throughput %d kps\n",throughput);
    
}

int main(int argc, char *argv[]) {
  int packet_num;
  int temp_no=0;
    //int method_no;
  struct packet packte_out[100001];
  char strLine[1024];
  FILE *fp = fopen("data", "rb");
  if( fp == NULL){
    printf("Error at opening files!\n");
    return 0;
  }
  fgets(strLine,1024,fp);
  packet_num = atoi(strLine);
  while (!feof(fp))
    {
      fgets(strLine,1024,fp);
      convert_int(strLine,&packte_out[temp_no]);
      temp_no++;
    }
  if(argc < 2){
    std::cout << "Must input one of the following simulator method: aloha, slotted_aloha, csma_1 " << std::endl;
    exit(1);
  }
  std::string method(argv[1]);
  if(method == "aloha"){
    ALOHA(packte_out, packet_num);
  }else if(method == "slotted_aloha"){
    SLOTTED_ALOHA(packte_out, packet_num);
  }else if(method == "csma_1"){
    CSMA_1(packte_out, packet_num);
  }else{
    std::cout << "Wrong simulator method " << std::endl;
  }
  return 0;
}
