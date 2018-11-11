#include "ns3/network-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/dce-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/constant-position-mobility-model.h"
#include "ccnx/misc-tools.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("DceIperf");

int main (int argc, char *argv[])
{

// Node Container creation
	
	NodeContainer nodes;
	nodes.Create (4);

        DceManagerHelper dceManager;

// Linux stack creation	
	
	InternetStackHelper stack; 
	stack.Install (nodes);
        dceManager.Install (nodes);

// For real time 
// GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl")); 
// GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

// Device and channel  creation
	PointToPointHelper p2p;
	p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));

//  Node0-Node1 setup
	Ipv4AddressHelper address;  
	address.SetBase ("10.1.1.0", "255.255.255.252"); 

//  Node0-Node1 addresses
	NetDeviceContainer devices;
	devices = p2p.Install (nodes.Get (0), nodes.Get (1));

// connecting nodes
	Ipv4InterfaceContainer interfaces = address.Assign (devices); 

//  assign addresses
//  Node1-Node2 setup
devices = p2p.Install (nodes.Get (1), nodes.Get (2));

// connecting nodes
address.SetBase ("10.1.2.0", "255.255.255.252"); 

//  Node1-Node2 addresses
interfaces = address.Assign (devices); 


//  Node2-Node3 setup
devices = p2p.Install (nodes.Get (2), nodes.Get (3));

// connecting nodes
address.SetBase ("10.1.3.0", "255.255.255.252"); 

//  Node2-Node3 addresses
interfaces = address.Assign (devices); 


//  assign addresses
// setup ip routes
Ipv4GlobalRoutingHelper::PopulateRoutingTables ();



DceApplicationHelper dce;
ApplicationContainer apps;
dce.SetStackSize (1 << 20);      // 1MB stack
	dce.SetBinary ("iperf"); // Launch iperf client on node 0
	dce.ResetArguments ();   // clean arguments 
	dce.ResetEnvironment (); // clean environnent
	dce.AddArgument ("-c");  // client 
        dce.AddArgument ("10.1.2.2"); //target machine address
	dce.AddArgument ("-i");   // interval 
	dce.AddArgument ("1"); 
	dce.AddArgument 
("--time"); // how long
	dce.AddArgument ("10");

apps = dce.Install (nodes.Get (0)); //install application 
apps.Start (Seconds (0.7));         //start at 0.7 simulation time
apps.Stop (Seconds (20));           //stop at 20s    simulation time
dce.SetBinary ("iperf");            // Launch iperf server on node 2
dce.ResetArguments ();              // clean arguments 
dce.ResetEnvironment ();            // clean environnent
dce.AddArgument ("-s");             // server
dce.AddArgument ("-P");             // number of paralell servers
dce.AddArgument ("1");
apps = dce.Install (nodes.Get (3));
apps = dce.Install (nodes.Get (3));
apps.Start (Seconds (0.6));

p2p.EnablePcapAll ("iperf-", true);
//pointToPoint.EnablePcapAll ("iperf-" + stack, false);
// Simulation stop time
Simulator::Stop (Seconds (40.0));
// Run 
Simulator::Run ();
// Stop
Simulator::Destroy ();
return 0;
}

