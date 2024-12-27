import React, { useMemo } from "react";
import FlowChart from "./FlowChart";

const Homepage = ({ flows }) => {
  const { tcpCount, udpCount, averageRate, averageSize, averageDuration } =
    useMemo(() => {
      let tcp = 0;
      let udp = 0;
      let totalRate = 0.0;
      let totalBytes = 0.0;
      let totalDuration = 0;

      flows.forEach((flow) => {
        const protocol = flow.protocol.toString();

        if (protocol === "6") tcp++;
        else if (protocol === "17") udp++;

        totalRate += flow.rate;
        totalBytes += flow.total_bytes;
        totalDuration += flow.duration;
      });

      const averageRate = totalRate / flows.length;
      const averageSize = totalBytes / flows.length;
      const averageDuration = totalDuration / flows.length;

      return {
        tcpCount: tcp,
        udpCount: udp,
        averageRate: averageRate.toFixed(2),
        averageSize: averageSize.toFixed(2),
        averageDuration: averageDuration.toFixed(2),
      };
    }, [flows]);

  return (
    <div>
      <FlowChart tcpCount={tcpCount} udpCount={udpCount} />
      <div className="flex h-[350px]">
        <div className="bg-gray-100 p-4 rounded-lg shadow-md mt-3 w-[40%]">
          <h3 className="text-xl text-[#343a40] uppercase font-light mb-4 flex justify-center items-center">
            Network Statistics
          </h3>
          <div className="space-y-5">
            <div className="flex justify-between text-gray-700">
              <span>Total Flows:</span>
              <span className="font-bold">{tcpCount + udpCount}</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>TCP Traffic:</span>
              <span className="font-bold">
                {((tcpCount / (tcpCount + udpCount)) * 100).toFixed(2)}%
              </span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>UDP Traffic:</span>
              <span className="font-bold">
                {((udpCount / (tcpCount + udpCount)) * 100).toFixed(2)}%
              </span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>Average Flow Rate:</span>
              <span className="font-bold">{averageRate} byte/s</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>Average Flow Size:</span>
              <span className="font-bold">{averageSize} bytes</span>
            </div>
            <div className="flex justify-between text-gray-700">
              <span>Average Flow Duration:</span>
              <span className="font-bold">{averageDuration} s</span>
            </div>
          </div>
        </div>
        <div className="bg-gray-100 p-4 rounded-lg shadow-md mt-3 w-[60%] ml-4">
          <h3 className="text-xl text-[#343a40] uppercase font-light mb-4 flex justify-center items-center">
            IP by Countries
          </h3>
        </div>
      </div>
    </div>
  );
};

export default Homepage;
