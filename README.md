# tcp-veno-dce

step-1 :: Unzip the gfc-dumbbell file<br>
step-2 :: copy the files in the respective directory <br>
          A.dce-gfc-dumbbell to examples folder make sure u have added its location in wscript<br>
          B.gfc-dumbbell_parse_cwnd_v2.py to utils folder<br>
          c.copy linux-plotscripts-gfcdumbell and ns3-plotscriptsgfcdumbbell to ns3-dce directory remove previous contained               plotscript of dummbell<br>
          D.for linux stack ./waf "--run dce-gfc-dumbbell --stack = linux"<br>
          E.for ns3 stack ./waf "--run dce-gfc-dumbbell --stack = ns3"<br>
          F.after running above commands wait till the build compeletes when u run stack = linux u get files form file-0 to file-11 a folder will be created in ns3-dce directory saying results every time u run above D or E command u get a direcorty created in results/gfc/dumbbell/dd-tt-mm  new folders will be saved according time and date when they are executed <br>
          G.when u run stack = linux go to utils direcorty and open terminal there and run gfc_dumbbell_parse_cwnd_v2.py a folder will be created in results/gfc-dumbell naming cwnd u can see the graphs of linux-cwnd u can plot then using linux-plotscript-gfcdumbbelll similarly u can plot queue size <br>
          F.similar when u run stack = ns3 a results folder will be created and which has all the data stored inside it open the recently created folder and go to cwnd folder u can see that ns3-cwnd plotme will be there u can get the garphs using ns3-plotscript-gfc-dumbbell similarly queue size of ns3<br>
          <br>
Comparison of graphs between linux and ns3 stacks basing on the queue size and cwnd <br>
A.create the a folder nameing comparison in ns3-dce directory <br>
B.copy all the plotme of linux and ns3 of cwnd and queue to that comparision folder which was created <br>
C.copy the comparison gnuplotscriptComparison for ploting queue graph<br>
D.copy the comaparison gnuplotscriptcomparison_queue to get the combied graph of the both linux and ns3  <br>
