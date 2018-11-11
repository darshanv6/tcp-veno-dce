#include <iostream>
#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/packet-sink.h"
#include "ns3/traffic-control-module.h"
#include "ns3/log.h"
#include "ns3/random-variable-stream.h"
#include "gtk-config-store.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/callback.h"
#include "ns3/dce-module.h"

using namespace ns3;
Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
std::string dir = "results/gfc-dumbbell/";
double stopTime = 30;

void
LinuxCheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &LinuxCheckQueueSize, queue);
  std::ofstream fPlotQueue (std::stringstream (dir + "linux-queue-size.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

void
ns3CheckQueueSize (Ptr<QueueDisc> queue)
{
  uint32_t qSize = queue->GetCurrentSize ().GetValue ();

  // check queue size every 1/100 of a second
  Simulator::Schedule (Seconds (0.001), &ns3CheckQueueSize, queue);
  std::ofstream fPlotQueue (std::stringstream (dir + "ns3-queue-size.plotme").str ().c_str (), std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << qSize << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeA (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/A-ns3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeB (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/B-ns3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeC (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/C-ns3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeD (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/D-ns3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
CwndChangeE (uint32_t oldCwnd, uint32_t newCwnd)
{
  std::ofstream fPlotQueue (dir + "cwndTraces/E-ns3.plotme", std::ios::out | std::ios::app);
  fPlotQueue << Simulator::Now ().GetSeconds () << " " << newCwnd/1446.0 << std::endl;
  fPlotQueue.close ();
}

static void
DropAtQueue (Ptr<OutputStreamWrapper> stream, Ptr<const QueueDiscItem> item)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " 1" << std::endl;
}

static void
MarkAtQueue (Ptr<OutputStreamWrapper> stream, Ptr<const QueueDiscItem> item, const char* reason)
{
  *stream->GetStream () << Simulator::Now ().GetSeconds () << " 1" << std::endl;
}


void
TraceCwnd (uint32_t node, uint32_t cwndWindow,
           Callback <void, uint32_t, uint32_t> CwndTrace)
{
  Config::ConnectWithoutContext ("/NodeList/" + std::to_string (node) + "/$ns3::TcpL4Protocol/SocketList/" + std::to_string (cwndWindow) + "/CongestionWindow", CwndTrace);
}

void ns3InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port,uint32_t nodeId, uint32_t cwndWindow,Callback <void, uint32_t, uint32_t> CwndTrace)
{
  BulkSendHelper source ("ns3::TcpSocketFactory",InetSocketAddress (address, port));

  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  Time timeToStart = Seconds (uv->GetValue (10, 11));
  sourceApps.Start (timeToStart);
  Simulator::Schedule (timeToStart + Seconds (0.001), &TraceCwnd, nodeId, cwndWindow, CwndTrace);
  sourceApps.Stop (Seconds (stopTime));
}

void InstallBulkSend (Ptr<Node> node, Ipv4Address address, uint16_t port, std::string sock_factory)
{
  BulkSendHelper source (sock_factory, InetSocketAddress (address, port));
  source.SetAttribute ("MaxBytes", UintegerValue (0));
  ApplicationContainer sourceApps = source.Install (node);
  Time timeToStart = Seconds (uv->GetValue (10, 11));
  sourceApps.Start (timeToStart);
  sourceApps.Stop (Seconds (stopTime));
}

void InstallPacketSink (Ptr<Node> node, uint16_t port, std::string sock_factory)
{
  PacketSinkHelper sink (sock_factory, InetSocketAddress (Ipv4Address::GetAny (), port));
  ApplicationContainer sinkApps = sink.Install (node);
  sinkApps.Start (Seconds (10.0));
  sinkApps.Stop (Seconds (stopTime));
     
}

static void GetSSStats (Ptr<Node> node, Time at, std::string stack)
{
  if(stack == "linux")
  {
    DceApplicationHelper process;
    ApplicationContainer apps;
    process.SetBinary ("ss");
    process.SetStackSize (1 << 20);
    process.AddArgument ("-a");
    process.AddArgument ("-e");
    process.AddArgument ("-i");
    apps.Add(process.Install (node));
    apps.Start(at);
  }
}

int main (int argc, char *argv[])
{
  uint32_t stream = 1;
  std::string stack = "linux";
  std::string sock_factory = "ns3::TcpSocketFactory";
  std::string transport_prot = "TcpVeno";
  std::string linux_prot = "veno";
  //bool m_sackEnabled= true;
  std::string recovery = "ns3::TcpClassicRecovery";
  std::string queue_disc_type = "FifoQueueDisc";
  bool useEcn = true;
  uint32_t dataSize = 1460;
  uint32_t delAckCount = 2;

  //Enable checksum if linux and ns3 node communicate 
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];

  time (&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer,sizeof(buffer),"%d-%m-%Y-%I-%M-%S",timeinfo);
  std::string currentTime (buffer);

  CommandLine cmd;
  cmd.AddValue ("stream", "Seed value for random variable", stream);
  cmd.AddValue ("stack", "Set TCP/IP stack as ns3 or linux", stack);
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus, TcpLedbat, "
                "TcpLp", transport_prot);
  cmd.AddValue ("linux_prot", "Linux network protocol to use: reno, "
                "hybla, highspeed, htcp, vegas, scalable, veno, "
                "bic, yeah, illinois, westwood, lp", linux_prot);
  cmd.AddValue ("queue_disc_type", "Queue disc type for gateway (e.g. ns3::CoDelQueueDisc)", queue_disc_type);
  cmd.AddValue ("recovery", "Recovery algorithm type to use (e.g., ns3::TcpPrrRecovery", recovery);
  cmd.AddValue ("useEcn", "Use ECN", useEcn);
  cmd.AddValue ("dataSize", "Data packet size", dataSize);
  cmd.AddValue ("delAckCount", "Delayed ack count", delAckCount);
 // cmd.AddValue ("m_sackEnabled", "Enable or disable Sack option",m_sackEnabled);
  cmd.AddValue ("stopTime", "Stop time for applications / simulation time will be stopTime", stopTime);
  cmd.Parse (argc,argv);

  uv->SetStream (stream);
  queue_disc_type = std::string ("ns3::") + queue_disc_type;
  if (stack=="ns3")
    {
      transport_prot = std::string ("ns3::") + transport_prot;
    }

  TypeId qdTid;
  NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (queue_disc_type, &qdTid), "TypeId " << queue_disc_type << " not found");

  // Enables PRR in ns-3 and sets the TCP in ns-3
  if (stack == "ns3")
    {        
      Config::SetDefault ("ns3::TcpL4Protocol::RecoveryType", TypeIdValue (TypeId::LookupByName ("ns3::TcpPrrRecovery")));

      if (transport_prot.compare ("ns3::TcpWestwoodPlus") == 0)
        {
          // TcpWestwoodPlus is not an actual TypeId name; we need TcpWestwood here
          Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
          // the default protocol type in ns3::TcpWestwood is WESTWOOD
          Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
        }
      else
        {
          TypeId tcpTid;
          NS_ABORT_MSG_UNLESS (TypeId::LookupByNameFailSafe (transport_prot, &tcpTid), "TypeId " << transport_prot << " not found");
          Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TypeId::LookupByName (transport_prot)));
        }
    }

  // Create nodes
  NodeContainer leftNodes, rightNodes, routers;
  routers.Create (2);
  leftNodes.Create (5);
  rightNodes.Create (5);

  // Create the point-to-point link helpers
  PointToPointHelper pointToPointRouter;
  //pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("100Mbps"));
  //pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("0.00075ms"));

  pointToPointRouter.SetDeviceAttribute  ("DataRate", StringValue ("10Mbps"));
  pointToPointRouter.SetChannelAttribute ("Delay", StringValue ("60ms"));
  NetDeviceContainer r1r2ND = pointToPointRouter.Install (routers.Get (0), routers.Get (1));

  std::vector <NetDeviceContainer> leftToRouter;
  std::vector <NetDeviceContainer> routerToRight;
  PointToPointHelper pointToPointLeaf;
  pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("4Mbps"));
  //pointToPointLeaf.SetDeviceAttribute    ("DataRate", StringValue ("100Mbps"));
  
 // Node 1
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("120ms"));
 // pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.00025ms"));
  leftToRouter.push_back (pointToPointLeaf.Install (leftNodes.Get (0), routers.Get (0)));
  routerToRight.push_back (pointToPointLeaf.Install (routers.Get (1), rightNodes.Get (0)));

  // Node 2
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("120ms"));
 //pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.0001ms"));
  leftToRouter.push_back (pointToPointLeaf.Install (leftNodes.Get (1), routers.Get (0)));
  routerToRight.push_back (pointToPointLeaf.Install (routers.Get (1), rightNodes.Get (1)));

  // Node 3
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("120ms"));
  //pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.00005ms"));
  leftToRouter.push_back (pointToPointLeaf.Install (leftNodes.Get (2), routers.Get (0)));
  routerToRight.push_back (pointToPointLeaf.Install (routers.Get (1), rightNodes.Get (2)));

  // Node 4
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("120ms"));
  //pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.000025ms"));
  leftToRouter.push_back (pointToPointLeaf.Install (leftNodes.Get (3), routers.Get (0)));
  routerToRight.push_back (pointToPointLeaf.Install (routers.Get (1), rightNodes.Get (3)));

  // Node 5
  pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("120ms"));
  //pointToPointLeaf.SetChannelAttribute   ("Delay", StringValue ("0.000005ms"));
  leftToRouter.push_back (pointToPointLeaf.Install (leftNodes.Get (4), routers.Get (0)));
  routerToRight.push_back (pointToPointLeaf.Install (routers.Get (1), rightNodes.Get (4)));

  DceManagerHelper dceManager;
  LinuxStackHelper linuxStack;
  InternetStackHelper internetStack;

  if (stack=="linux")
    {
      sock_factory = "ns3::LinuxTcpSocketFactory";
      dceManager.SetTaskManagerAttribute ("FiberManagerType", StringValue ("UcontextFiberManager"));
      dceManager.SetNetworkStack ("ns3::LinuxSocketFdFactory", "Library", StringValue ("liblinux.so"));
      linuxStack.Install (leftNodes);
      linuxStack.Install (rightNodes);
      internetStack.Install (routers);
    }
  else if (stack=="ns3")
    {
      internetStack.InstallAll (); 
    }

  // Assign ip addresses to all the net devices
  Ipv4AddressHelper ipAddresses ("10.0.0.0", "255.255.255.0");

  Ipv4InterfaceContainer r1r2IPAddress = ipAddresses.Assign (r1r2ND);        ipAddresses.NewNetwork ();

  std::vector <Ipv4InterfaceContainer> leftToRouterIPAddress;
  leftToRouterIPAddress.push_back (ipAddresses.Assign (leftToRouter [0]));   ipAddresses.NewNetwork ();
  leftToRouterIPAddress.push_back (ipAddresses.Assign (leftToRouter [1]));   ipAddresses.NewNetwork ();
  leftToRouterIPAddress.push_back (ipAddresses.Assign (leftToRouter [2]));   ipAddresses.NewNetwork ();
  leftToRouterIPAddress.push_back (ipAddresses.Assign (leftToRouter [3]));   ipAddresses.NewNetwork ();
  leftToRouterIPAddress.push_back (ipAddresses.Assign (leftToRouter [4]));   ipAddresses.NewNetwork ();

  std::vector <Ipv4InterfaceContainer> routerToRightIPAddress;
  routerToRightIPAddress.push_back (ipAddresses.Assign (routerToRight [0]));   ipAddresses.NewNetwork ();
  routerToRightIPAddress.push_back (ipAddresses.Assign (routerToRight [1]));   ipAddresses.NewNetwork ();
  routerToRightIPAddress.push_back (ipAddresses.Assign (routerToRight [2]));   ipAddresses.NewNetwork ();
  routerToRightIPAddress.push_back (ipAddresses.Assign (routerToRight [3]));   ipAddresses.NewNetwork ();
  routerToRightIPAddress.push_back (ipAddresses.Assign (routerToRight [4]));

  dceManager.Install (leftNodes);
  dceManager.Install (rightNodes);
  dceManager.Install (routers);
  
  // Set configuration for inux stack
  if (stack=="linux")
    {
      linuxStack.SysctlSet (leftNodes, ".net.ipv4.conf.default.forwarding", "1");
      linuxStack.SysctlSet (rightNodes, ".net.ipv4.conf.default.forwarding", "1");
      linuxStack.SysctlSet (leftNodes, ".net.ipv4.tcp_congestion_control", linux_prot);
      linuxStack.SysctlSet (rightNodes, ".net.ipv4.tcp_congestion_control", linux_prot);
      //linuxStack.SysctlSet (leftNodes, ".net.ipv4.tcp_ecn", "1");
      //linuxStack.SysctlSet (rightNodes, ".net.ipv4.tcp_ecn", "1");
      linuxStack.SysctlSet (leftNodes, ".net.ipv4.tcp_fack", "0");
      linuxStack.SysctlSet (rightNodes, ".net.ipv4.tcp_sack", "1");
     
      linuxStack.SysctlSet (leftNodes, ".net.ipv4.tcp_sack", "1");
      linuxStack.SysctlSet (rightNodes, ".net.ipv4.tcp_fack", "0");
    
      linuxStack.SysctlSet (leftNodes, ".net.ipv4.tcp_dsack", "0");
      linuxStack.SysctlSet (rightNodes, ".net.ipv4.tcp_dsack", "0");
    }

  if (stack=="linux")
    {
      //Static Routing
      Ptr<Ipv4> ipv4Router1 = routers.Get (0)->GetObject<Ipv4> ();
      Ptr<Ipv4> ipv4Router2 = routers.Get (1)->GetObject<Ipv4> ();

      Ipv4StaticRoutingHelper routingHelper;

      Ptr<Ipv4StaticRouting> staticRoutingRouter1 = routingHelper.GetStaticRouting (ipv4Router1);
      Ptr<Ipv4StaticRouting> staticRoutingRouter2 = routingHelper.GetStaticRouting (ipv4Router2);

      //Routing for Router 1
      staticRoutingRouter1->AddNetworkRouteTo (Ipv4Address ("10.0.6.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.2"), 1);
      staticRoutingRouter1->AddNetworkRouteTo (Ipv4Address ("10.0.7.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.2"), 1);
      staticRoutingRouter1->AddNetworkRouteTo (Ipv4Address ("10.0.8.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.2"), 1);
      staticRoutingRouter1->AddNetworkRouteTo (Ipv4Address ("10.0.9.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.2"), 1);
      staticRoutingRouter1->AddNetworkRouteTo (Ipv4Address ("10.0.10.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.2"), 1);

      //Routing for Router 2
      staticRoutingRouter2->AddNetworkRouteTo (Ipv4Address ("10.0.1.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.1"), 1);
      staticRoutingRouter2->AddNetworkRouteTo (Ipv4Address ("10.0.2.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.1"), 1);
      staticRoutingRouter2->AddNetworkRouteTo (Ipv4Address ("10.0.3.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.1"), 1);
      staticRoutingRouter2->AddNetworkRouteTo (Ipv4Address ("10.0.4.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.1"), 1);
      staticRoutingRouter2->AddNetworkRouteTo (Ipv4Address ("10.0.5.0"), Ipv4Mask ("255.255.255.0"), Ipv4Address ("10.0.0.1"), 1);

      std::ostringstream cmd_oss;

      //Default route for senders
      for(uint32_t i=1; i<6; i = i+1)
        {
           cmd_oss.str ("");
           cmd_oss << "route add default via " << (routers.Get (0)->GetObject<Ipv4> ())->GetAddress (i+1, 0).GetLocal () << " dev sim0";
           LinuxStackHelper::RunIp (leftNodes.Get (i-1), Seconds (0.00001), cmd_oss.str ());
           LinuxStackHelper::RunIp (leftNodes.Get (i-1), Seconds (0.00001), "link set sim0 up");
        }

      //Default route for receivers
      for(uint32_t i=1; i<6; i = i+1)
        {
           cmd_oss.str ("");
           cmd_oss << "route add default via " << (routers.Get (1)->GetObject<Ipv4> ())->GetAddress (i+1, 0).GetLocal () << " dev sim0";
           LinuxStackHelper::RunIp (rightNodes.Get (i-1), Seconds (0.00001), cmd_oss.str ());
           LinuxStackHelper::RunIp (rightNodes.Get (i-1), Seconds (0.00001), "link set sim0 up");
        }
    }
  else if (stack=="ns3")
      { 
        Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
      }

  //Creates directories to store various plotme files
  dir += (currentTime + "/");
  std::string dirToSave = "mkdir -p " + dir;
  system (dirToSave.c_str ());
  system ((dirToSave + "/pcap/").c_str ());
  system ((dirToSave + "/markTraces/").c_str ());
  system ((dirToSave + "/queueTraces/").c_str ());
  if (stack == "linux")
    {
      system (("cp -R Linux-PlotScripts-gfc-dumbbell/* " + dir + "/pcap/").c_str ());
    }
  else if (stack == "ns3")
    {
      system ((dirToSave + "/cwndTraces/").c_str ());
      system (("cp -R ns3-PlotScripts-gfc-dumbbell/* " + dir + "/pcap/").c_str ());
    }
  
  //Set default parameters for TCP in ns-3
  if (stack=="ns3")
    {
      Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 20));
      Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 20));
      Config::SetDefault ("ns3::TcpSocket::InitialCwnd", UintegerValue (10));
      Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (delAckCount));
      Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (dataSize));
      Config::SetDefault ("ns3::TcpSocketBase::EcnMode", StringValue ("ClassicEcn"));
      Config::SetDefault ("ns3::TcpSocketBase::Sack",BooleanValue (true));      
    }

  // Set default parameters for queue disc
  Config::SetDefault (queue_disc_type + "::MaxSize", QueueSizeValue (QueueSize ("666p")));

  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> streamWrapper;

  // Install queue disc on router
  TrafficControlHelper tch;
  tch.SetRootQueueDisc (queue_disc_type);
  QueueDiscContainer qd;
  tch.Uninstall (routers.Get (0)->GetDevice (0));
  qd.Add (tch.Install (routers.Get (0)->GetDevice (0)).Get (0));
  
 // Calls function to check queue size 
 if (stack == "linux")
    {
      Simulator::ScheduleNow (&LinuxCheckQueueSize, qd.Get (0));
    }
  else if (stack == "ns3")
    {
      Simulator::ScheduleNow (&ns3CheckQueueSize, qd.Get (0));
    }
  
  // Create plotme to store packets dropped and marked at the router
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/drop-0.plotme");
  qd.Get (0)->TraceConnectWithoutContext ("Drop", MakeBoundCallback (&DropAtQueue, streamWrapper));
  streamWrapper = asciiTraceHelper.CreateFileStream (dir + "/queueTraces/mark-0.plotme");
  qd.Get (0)->TraceConnectWithoutContext ("Mark", MakeBoundCallback (&MarkAtQueue, streamWrapper));

  // Install packet sink at receiver side
  uint16_t port = 50000;
  InstallPacketSink (rightNodes.Get (0), port, sock_factory);      // A Sink 0 Applications
  InstallPacketSink (rightNodes.Get (1), port, sock_factory);      // B Sink 0 Applications
  InstallPacketSink (rightNodes.Get (2), port, sock_factory);      // C Sink 0 Applications
  InstallPacketSink (rightNodes.Get (3), port, sock_factory);      // D Sink 0 Applications
  InstallPacketSink (rightNodes.Get (4), port, sock_factory);      // E Sink 0 Applications

  //Install BuulkSend application
  if (stack == "linux")
    {
      InstallBulkSend (leftNodes.Get (0), routerToRightIPAddress [0].GetAddress (1), port, sock_factory);
      InstallBulkSend (leftNodes.Get (1), routerToRightIPAddress [1].GetAddress (1), port, sock_factory);
      InstallBulkSend (leftNodes.Get (2), routerToRightIPAddress [2].GetAddress (1), port, sock_factory);
      InstallBulkSend (leftNodes.Get (3), routerToRightIPAddress [3].GetAddress (1), port, sock_factory);
      InstallBulkSend (leftNodes.Get (4), routerToRightIPAddress [4].GetAddress (1), port, sock_factory);
    }
  else if (stack == "ns3")
    {
      ns3InstallBulkSend (leftNodes.Get (0), routerToRightIPAddress [0].GetAddress (1), port, 2, 0, MakeCallback (&CwndChangeA));
      ns3InstallBulkSend (leftNodes.Get (1), routerToRightIPAddress [1].GetAddress (1), port, 3, 0, MakeCallback (&CwndChangeB));
      ns3InstallBulkSend (leftNodes.Get (2), routerToRightIPAddress [2].GetAddress (1), port, 4, 0, MakeCallback (&CwndChangeC));
      ns3InstallBulkSend (leftNodes.Get (3), routerToRightIPAddress [3].GetAddress (1), port, 5, 0, MakeCallback (&CwndChangeD));
      ns3InstallBulkSend (leftNodes.Get (4), routerToRightIPAddress [4].GetAddress (1), port, 6, 0, MakeCallback (&CwndChangeE));
    }

//  GtkConfigStore gcs;
//  gcs.ConfigureAttributes ();

  // Calls function to run ss command on linux stack after every 0.05 seconds
  if (stack=="linux")
    {
      for (int j=0; j<leftNodes.GetN (); j++)
       {
          for(float i = 10.0; i <= stopTime ; i=i+0.05)
            {
               GetSSStats(leftNodes.Get (j), Seconds(i), stack);
            }
       }
    }

  //Enables pcap on all the interfaces
  pointToPointLeaf.EnablePcapAll (dir + "pcap/N", true);

  Simulator::Stop (Seconds (stopTime));
  Simulator::Run ();

  // Store queue stats in a file
  std::ofstream myfile;
  myfile.open (dir + "queueStats.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  myfile << std::endl;
  myfile << "Stat for Queue 1";
  myfile << qd.Get (0)->GetStats ();
  myfile.close ();

  // Stores configuration of the simulation in a file
  myfile.open (dir + "config.txt", std::fstream::in | std::fstream::out | std::fstream::app);
  myfile << "useEcn " << useEcn << "\n";
  myfile << "recovery" << recovery << "\n";
  myfile << "queue_disc_type " << queue_disc_type << "\n";
  myfile << "stream  " << stream << "\n";
  myfile << "stack  " << stack << "\n";
  (stack=="ns3") ? myfile << "transport_prot " << transport_prot << "\n" : myfile << "linux_prot " << linux_prot << "\n";
  myfile << "dataSize " << dataSize << "\n";
  myfile << "delAckCount " << delAckCount << "\n";
  myfile << "stopTime " << stopTime << "\n";
  myfile.close ();

  Simulator::Destroy ();

  return 0;
}
