import os
import sys
import numpy as np

def get_sockets(in_filepath):
  all_sockets = []
  curr_socket = []
  first_socket = 1
  with open(in_filepath) as in_file:
    for line in in_file:
      if "ss -a -e -i" in line:
        if first_socket:
          first_socket = 0
        else:
          all_sockets.append(curr_socket)
          curr_socket = []
      else:
        if not first_socket:
          curr_socket.append(line)
		
  all_sockets.append(curr_socket)
  return all_sockets

sender_node_id = range (2, 7)
receiver_node_id = range (7, 12)

if (not os.path.isdir("../results/gfc-dumbbell/cwnd_data")):
  os.system("mkdir ../results/gfc-dumbbell/cwnd_data")

for i in sender_node_id:
  os.system ("cat ../files-"+str(i)+"/var/log/*/* > "+"../results/gfc-dumbbell/cwnd_data/"+str(i))

for i in sender_node_id:
  in_filepath = "../results/gfc-dumbbell/cwnd_data/"+str(i)
  all_sockets = get_sockets(in_filepath)
  write_mode = 'w'
  for socket in all_sockets:
    time_found = 0
    cwnd_found = 0
    port_found = 0
    time = None
    cwnd = None
    for info in socket:
      if "NS3" in info:
        time = info.split("NS3 Time:")[1].split(',')[0].strip().split('(')[1].strip().strip(')').strip().strip('+').strip('ns')
        time_format = float(time)/1000000000
        time_found = 1
      if ":50000" in info:
        port = 50000
        port_found = 1
			
      if "cwnd" in info:
        cwnd = info.split("cwnd:")[1].split()[0]
        if port_found:
          with open("../results/gfc-dumbbell/cwnd_data/"+chr(i+63) + "-linux.plotme", write_mode) as out_file:
            out_file.write(str(time_format) + " " + str(cwnd) + "\n")
          write_mode = 'a'
          port_found = 0
  
