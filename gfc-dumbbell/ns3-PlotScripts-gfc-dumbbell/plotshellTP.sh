#Usage startTime
printf "Flow Throughput\n\n" | tee -a overallTP.txt

python2 tp-new.py --startTime=0 --destIp 10.0.6.2 N-7-0.pcap plotmeTP/TP-A-ns3.plotme plotmeTPP/TP-A-ns3.plotme A | tee -a overallTP.txt
python2 tp-new.py --startTime=0 --destIp 10.0.7.2 N-8-0.pcap plotmeTP/TP-B-ns3.plotme plotmeTPP/TP-B-ns3.plotme B | tee -a overallTP.txt
python2 tp-new.py --startTime=0 --destIp 10.0.8.2 N-9-0.pcap plotmeTP/TP-C-ns3.plotme plotmeTPP/TP-C-ns3.plotme C | tee -a overallTP.txt
python2 tp-new.py --startTime=0 --destIp 10.0.9.2 N-10-0.pcap plotmeTP/TP-D-ns3.plotme plotmeTPP/TP-D-ns3.plotme D | tee -a overallTP.txt
python2 tp-new.py --startTime=0 --destIp 10.0.10.2 N-11-0.pcap plotmeTP/TP-E-ns3.plotme plotmeTPP/TP-E-ns3.plotme E | tee -a overallTP.txt

cp ns3-gnuplotscript plotmeTP
cp ns3-perflowgnuplot plotmeTP
cd plotmeTP
awk  '{$1 = $1+10}1' OFS=' ' TP-A-ns3.plotme  > TP-A_1-ns3.plotme
rm TP-A-ns3.plotme
mv TP-A_1-ns3.plotme TP-A-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-B-ns3.plotme  > TP-B_1-ns3.plotme
rm TP-B-ns3.plotme
mv TP-B_1-ns3.plotme TP-B-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-C-ns3.plotme  > TP-C_1-ns3.plotme
rm TP-C-ns3.plotme
mv TP-C_1-ns3.plotme TP-C-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-D-ns3.plotme  > TP-D_1-ns3.plotme
rm TP-D-ns3.plotme
mv TP-D_1-ns3.plotme TP-D-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-E-ns3.plotme  > TP-E_1-ns3.plotme
rm TP-E-ns3.plotme
mv TP-E_1-ns3.plotme TP-E-ns3.plotme
gnuplot ns3-gnuplotscript
gnuplot ns3-perflowgnuplot
cd ..

cp ns3-gnuplotscriptPackets plotmeTPP
cd plotmeTPP
awk  '{$1 = $1+10}1' OFS=' ' TP-A-ns3.plotme  > TP-A_1-ns3.plotme
rm TP-A-ns3.plotme
mv TP-A_1-ns3.plotme TP-A-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-B-ns3.plotme  > TP-B_1-ns3.plotme
rm TP-B-ns3.plotme
mv TP-B_1-ns3.plotme TP-B-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-C-ns3.plotme  > TP-C_1-ns3.plotme
rm TP-C-ns3.plotme
mv TP-C_1-ns3.plotme TP-C-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-D-ns3.plotme  > TP-D_1-ns3.plotme
rm TP-D-ns3.plotme
mv TP-D_1-ns3.plotme TP-D-ns3.plotme
awk  '{$1 = $1+10}1' OFS=' ' TP-E-ns3.plotme  > TP-E_1-ns3.plotme
rm TP-E-ns3.plotme
mv TP-E_1-ns3.plotme TP-E-ns3.plotme
gnuplot ns3-gnuplotscriptPackets
cd ..

printf "\nRouter Throughput\n\n" | tee -a overallTP.txt

python2 tp-new.py --startTime=0 N-0-0.pcap plotmeRouterTP/TP-R1-ns3.plotme plotmeRouterTPP/TP-R1-ns3.plotme R1 | tee -a overallTP.txt

cp ns3-gnuplotscriptRouters plotmeRouterTP
cd plotmeRouterTP
awk  '{$1 = $1+10}1' OFS=' ' TP-R1-ns3.plotme  > TP-R1_1-ns3.plotme 
rm TP-R1-ns3.plotme
mv TP-R1_1-ns3.plotme TP-R1-ns3.plotme
gnuplot ns3-gnuplotscriptRouters
cd ..

cp ns3-gnuplotscriptRoutersPackets plotmeRouterTPP
cd plotmeRouterTPP
awk  '{$1 = $1+10}1' OFS=' ' TP-R1-ns3.plotme  > TP-R1_1-ns3.plotme 
rm TP-R1-ns3.plotme
mv TP-R1_1-ns3.plotme TP-R1-ns3.plotme
gnuplot ns3-gnuplotscriptRoutersPackets
cd ..

mkdir -p ../Graphs
cp plotmeRouterTP/*.png ../Graphs/
cp plotmeRouterTPP/*.png ../Graphs/
cp plotmeTP/*.png ../Graphs/
cp plotmeTPP/*.png ../Graphs/
cp overallTP.txt ../Graphs/

#Cwnd plot
cp ns3-gnuplotscriptQ ../
cp ns3-gnuplotscriptCwnd ../cwndTraces/

cd ../queueTraces/
echo "Queue 1" > QueueStatsAfter2sec.txt
drops=`awk '{if ($1 >= 2) print $0}' drop-0.plotme | wc -l`
marks=`awk '{if ($1 >= 2) print $0}' mark-0.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt
cp QueueStatsAfter2sec.txt ../Graphs/

cd ..
gnuplot ns3-gnuplotscriptQ
cp queueSize.png Graphs/
cp queueStats.txt Graphs/
cp config.txt Graphs/
cd cwndTraces
gnuplot ns3-gnuplotscriptCwnd
cp Cwnd*.png ../Graphs/
