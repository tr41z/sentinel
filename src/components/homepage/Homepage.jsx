import React, { useMemo } from "react";
import FlowChart from "./FlowChart";

const Homepage = ({ flows }) => {
  const { httpCount, dnsCount, tcpCount, udpCount, otherCount } =
    useMemo(() => {
      let http = 0,
        dns = 0,
        tcp = 0,
        udp = 0,
        other = 0;

      flows.forEach((flow) => {
        const srcPort = flow.src_port.toString();
        const protocol = flow.protocol.toString();

        if (srcPort === "443") http++;
        else if (srcPort === "53") dns++;
        else other++;

        if (protocol === "6") tcp++;
        else if (protocol === "17") udp++;
      });

      return {
        httpCount: http,
        dnsCount: dns,
        tcpCount: tcp,
        udpCount: udp,
        otherCount: other,
      };
    }, [flows]);

  return (
    <div>
      <FlowChart
        httpCount={httpCount}
        dnsCount={dnsCount}
        tcpCount={tcpCount}
        udpCount={udpCount}
        otherCount={otherCount}
      />
      <div className="flex h-[350px]">
        <div className="bg-gray-100 p-4 rounded-lg shadow-md mt-3 w-[30%]">
          <h3 className="text-xl text-[#343a40] uppercase font-light mb-4 flex justify-center items-center">
            System Health
          </h3>
          <div className="space-y-5">
            <div className="flex justify-between text-gray-700">
              <span>CPU Usage:</span>
              <span className="font-bold">45%</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>Memory Usage:</span>
              <span className="font-bold">70%</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>Network Load:</span>
              <span className="font-bold">60%</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>OPT</span>
              <span className="font-bold">OPT</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>OPT</span>
              <span className="font-bold">OPT</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>OPT</span>
              <span className="font-bold">OPT</span>
            </div>
          </div>
        </div>
        <div className="bg-gray-100 p-4 rounded-lg shadow-md mt-3 w-[70%] ml-4">
          <h3 className="text-xl text-[#343a40] uppercase font-light mb-4 flex justify-center items-center">
            IP by Countries
          </h3>
        </div>
      </div>
    </div>
  );
};

export default Homepage;
