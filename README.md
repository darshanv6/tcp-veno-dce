# Compare TCP Veno's performance on linux and ns3 stack

step-1: Unzip the gfc-dumbbell file. <br><br>

step-2: Copy the files as mentioned below: <br><br>
          A.The dce-gfc-dumbbell to the dce/dce-linux-dev/source/ns-3-dce/example folder and make sure you have added its location in the wscript file present in the dce/dce-linux-dev/source/ns-3-dce directory. <br><br>
          B.gfc-dumbbell_parse_cwnd_v2.py to the utils folder<br><br>
          C.Copy linux-plotscripts-gfcdumbell and ns3-plotscriptsgfcdumbbell to the ns3-dce directory. <br><br>
          D.To run the simulation on linux stack, type ./waf "--run dce-gfc-dumbbell --stack = linux" in the terminal after                               changing to the dce/dce-linux-dev/source/ns-3-dce directory. <br><br>
          E.To run the simulation on linux stack, type ./waf "--run dce-gfc-dumbbell --stack = ns3"<br><br>
          F.After running above commands, wait for the build to compelete. Folders named file-0, file-1,.......file-11 will be created in the ns3-dce directory. Also, the results of the simulations will be created in ../ns3-dce/results/gfc/dumbbell/dd-mm-yy hh-mm-ss folders with the folder names being the timestamps of the time of start of simulation <br><br>
          G.When you run stack = linux go to ns3-dce/utils direcorty and open the terminal there and run gfc_dumbbell_parse_cwnd_v2.py. A folder will be created in results/gfc-dumbell named Cwnd_data. You will be able to see the graphs of linux-cwnd. You can plot the graphs using the plotme files generated. <br><br>
          H.Similarly, when you run stack = ns3, a folder with the timestamp of the time of simulation will be created which has all the plotme files in it. <br>
          <br>
          
## Comparison of graphs between linux and ns3 stacks based on the queue size and cwnd
A.Create a folder named comparison in ns3-dce directory or anywhere else you are comfortable with. <br><br>
B.Copy all the plotmes obtained from the simulation of the gfc-dumbbell on linux and ns3 to that folder. <br><br>
C.Run the comparison gnuplotscriptComparison for ploting queue graph. <br><br>
D.Run the comaparison gnuplotscriptcomparison_queue to get the combied graph of the both linux and ns3. <br><br>
