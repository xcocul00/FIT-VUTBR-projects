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

// @file Test1eigrp.ned 
// @author Maros Cocula

package inet.examples.ansa.eigrp.EIGRP;


import ansa.node.ANSA_EIGRPRouter;
import ansa.node.ANSA_Host;
import inet.node.ethernet.Eth100M;
import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.common.scenario.ScenarioManager;

network Test_eigrp
{
    @display("bgb=792,330");
    submodules:
        configurator: IPv4NetworkConfigurator {
            parameters:
                config = xml("<config></config>");
                assignAddresses = false;
                assignDisjunctSubnetAddresses = false;
                addStaticRoutes = false;
                addDefaultRoutes = false;
                addSubnetRoutes = false;
                optimizeRoutes = false;
                @display("p=47,281");
        }
        R1: ANSA_EIGRPRouter {
            parameters:
                @display("p=201,56");
            gates:
                ethg[4];
        }
        R2: ANSA_EIGRPRouter {
            parameters:
                @display("p=378,56");
            gates:
                ethg[3];
        }
        R3: ANSA_EIGRPRouter {
            @display("p=547,56");
            gates:
                ethg[2];
        }
        R4: ANSA_EIGRPRouter {
            @display("p=201,200");
            gates:
                ethg[3];
        }
        R5: ANSA_EIGRPRouter {
            @display("p=378,202");
            gates:
                ethg[4];
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
        scenarioManager: ScenarioManager {
            @display("p=201,281;is=s");
        }

    connections allowunconnected:
        R1.ethg[1] <--> Eth100M <--> R2.ethg[0];
        R1.ethg[2] <--> Eth100M <--> R4.ethg[2];
        R1.ethg[3] <--> Eth100M <--> R5.ethg[3];

        R2.ethg[1] <--> Eth100M <--> R5.ethg[2];
        R2.ethg[2] <--> Eth100M <--> R3.ethg[1];

        R4.ethg[1] <--> Eth100M <--> R5.ethg[1];

        R1.ethg[0] <--> Eth100M <--> PC1.ethg[0];
        R4.ethg[0] <--> Eth100M <--> PC2.ethg[0];
        R5.ethg[0] <--> Eth100M <--> PC3.ethg[0];
        R3.ethg[0] <--> Eth100M <--> PC4.ethg[0];
}

