//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
// @file Test1rip.ned
// @author Maros Cocula

package inet.examples.rip.RIPv2;

import ansa.linklayer.cdp.tables.CDPNeighbourTable;
import ansa.node.ANSA_Host;
import ansa.node.ANSA_Router;
import inet.common.misc.ThruputMeteringChannel;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.networklayer.xmipv6.xMIPv6Support;
import inet.node.rip.RIPRouter;
import inet.common.scenario.ScenarioManager;
import inet.physicallayer.contract.packetlevel.INeighborCache;
import inet.routing.rip.RIPRouting;


network Test_rip2
{
    @display("bgb=804,392");
    types:
        channel C extends ThruputMeteringChannel
        {
            delay = 0.1us;
            datarate = 100Mbps;
            thruputDisplayFormat = "#N";
        }
    submodules:
        configurator: IPv4NetworkConfigurator {
            parameters:
                config = xml("<config>"+
                            "<interface among='PC1 R1' address='192.168.1.x' netmask='255.255.255.0' />"+
                            "<interface among='PC2 R4' address='192.168.2.x' netmask='255.255.255.0' />"+
                            "<interface among='PC3 R5' address='192.168.3.x' netmask='255.255.255.0' />"+
                            "<interface among='PC4 R3' address='192.168.4.x' netmask='255.255.255.0' />"+
                            "<interface among='R1 R4' address='192.168.5.x' netmask='255.255.255.252' />"+         
                            "<interface among='R1 R5' address='192.168.5.x' netmask='255.255.255.252' />"+
                            "<interface among='R1 R2' address='192.168.5.x' netmask='255.255.255.252' />"+
                            "<interface among='R2 R5' address='192.168.5.x' netmask='255.255.255.252' />"+
                            "<interface among='R2 R3' address='192.168.5.x' netmask='255.255.255.252' />"+
                            "<interface among='R4 R5' address='192.168.5.x' netmask='255.255.255.252' />"+
                            "</config>");
                addStaticRoutes = false;
                addDefaultRoutes = false;
                @display("p=54.54,292.9");
        }
        PC1: ANSA_Host {
            @display("p=47,56;i=device/laptop");
            gates:
                ethg[1];
        }
        PC2: ANSA_Host {
            @display("p=47,200;i=device/laptop");
            gates:
                ethg[1];
        }
        PC3: ANSA_Host {
            @display("p=547,202;i=device/laptop");
            gates:
                ethg[1];
        }
        PC4: ANSA_Host {
            @display("p=713,56;i=device/laptop");
            gates:
                ethg[1];
        }
        R1: RIPRouter {
            @display("p=181.8,55.55");
            gates:
                ethg[4];
        }
        R2: RIPRouter {
            @display("p=372,58");
            gates:
                ethg[3];
        }
        R3: RIPRouter {
            @display("p=555,57");
            gates:
                ethg[2];
        }
        R4: RIPRouter {
            @display("p=182,203");
            gates:
                ethg[3];
        }
        R5: RIPRouter {
            @display("p=370,202");
            gates:
                ethg[4];
        }

    connections:
        R1.ethg[1] <--> C <--> R2.ethg[0];
        R1.ethg[2] <--> C <--> R4.ethg[2];
        R1.ethg[3] <--> C <--> R5.ethg[3];

        R2.ethg[1] <--> C <--> R5.ethg[2];
        R2.ethg[2] <--> C <--> R3.ethg[1];

        R4.ethg[1] <--> C <--> R5.ethg[1];

        R1.ethg[0] <--> C <--> PC1.ethg[0];
        R4.ethg[0] <--> C <--> PC2.ethg[0];
        R5.ethg[0] <--> C <--> PC3.ethg[0];
        R3.ethg[0] <--> C <--> PC4.ethg[0];
}

