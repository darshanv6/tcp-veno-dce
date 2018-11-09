#Usage startTime
printf "Flow Throughput\n\n" | tee -a overallTP.txt

python2 tp-new.py --startTime=10 --destIp 10.0.6.2 N-7-0.pcap plotmeTP/TP-A-linux.plotme plotmeTPP/TP-A-linux.plotme A | tee -a overallTP.txt
python2 tp-new.py --startTime=10 --destIp 10.0.7.2 N-8-0.pcap plotmeTP/TP-B-linux.plotme plotmeTPP/TP-B-linux.plotme B | tee -a overallTP.txt
python2 tp-new.py --startTime=10 --destIp 10.0.8.2 N-9-0.pcap plotmeTP/TP-C-linux.plotme plotmeTPP/TP-C-linux.plotme C | tee -a overallTP.txt
python2 tp-new.py --startTime=10 --destIp 10.0.9.2 N-10-0.pcap plotmeTP/TP-D-linux.plotme plotmeTPP/TP-D-linux.plotme D | tee -a overallTP.txt
python2 tp-new.py --startTime=10 --destIp 10.0.10.2 N-11-0.pcap plotmeTP/TP-E-linux.plotme plotmeTPP/TP-E-linux.plotme E | tee -a overallTP.txt

cp linux-gnuplotscript plotmeTP
cp linux-perflowgnuplot plotmeTP
cd plotmeTP
gnuplot linux-gnuplotscript
gnuplot linux-perflowgnuplot
cd ..

cp linux-gnuplotscriptPackets plotmeTPP
cd plotmeTPP
gnuplot linux-gnuplotscriptPackets
cd ..

printf "\nRouter Throughput\n\n" | tee -a overallTP.txt

python2 tp-new.py --startTime=0 N-0-0.pcap plotmeRouterTP/TP-R1-linux.plotme plotmeRouterTPP/TP-R1-linux.plotme R1 | tee -a overallTP.txt

cp linux-gnuplotscriptRouters plotmeRouterTP
cd plotmeRouterTP
awk  '{$1 = $1+10}1' OFS=' ' TP-R1-linux.plotme  > TP-R1_1-linux.plotme 
rm TP-R1-linux.plotme
mv TP-R1_1-linux.plotme TP-R1-linux.plotme
gnuplot linux-gnuplotscriptRouters
cd ..

cp linux-gnuplotscriptRoutersPackets plotmeRouterTPP
cd plotmeRouterTPP
awk  '{$1 = $1+10}1' OFS=' ' TP-R1-linux.plotme  > TP-R1_1-linux.plotme 
rm TP-R1-linux.plotme
mv TP-R1_1-linux.plotme TP-R1-linux.plotme
gnuplot linux-gnuplotscriptRoutersPackets
cd ..

mkdir -p ../Graphs
cp plotmeRouterTP/*.png ../Graphs/
cp plotmeRouterTPP/*.png ../Graphs/
cp plotmeTP/*.png ../Graphs/
cp plotmeTPP/*.png ../Graphs/
cp overallTP.txt ../Graphs/

#Cwnd plot
cp linux-gnuplotscriptQ ../

cd ../queueTraces/
echo "Queue 1" > QueueStatsAfter12sec.txt
drops=`awk '{if ($1 >= 12) print $0}' drop-0.plotme | wc -l`
marks=`awk '{if ($1 >= 12) print $0}' mark-0.plotme | wc -l`
echo -e  "$drops Drops, $marks Marks\n" >> QueueStatsAfter2sec.txt
cp QueueStatsAfter12sec.txt ../Graphs/

cd ..
gnuplot linux-gnuplotscriptQ
cp queueSize.png Graphs/
cp queueStats.txt Graphs/
cp config.txt Graphs/

cd ../../../utils
python2 gfc_dumbbell_parse_cwnd_v2.py
cd ..
cp Linux-PlotScripts-gfc-dumbbell/linux-gnuplotscriptCwnd results/gfc-dumbbell/cwnd_data
cd results/gfc-dumbbell/cwnd_data
gnuplot linux-gnuplotscriptCwnd
