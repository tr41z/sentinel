import React, { useState } from "react";
import { IoMdInformationCircleOutline } from "react-icons/io";
import {
  Area,
  AreaChart,
  CartesianGrid,
  Tooltip,
  XAxis,
  YAxis,
} from "recharts";

const FlowChart = ({ dnsCount, httpCount, tcpCount, udpCount, otherCount }) => {
  const [showInfo, setShowInfo] = useState(false);

  const data = [
    { name: "TCP", pc: tcpCount },
    { name: "UDP", pc: udpCount },
    { name: "HTTP", pc: httpCount },
    { name: "DNS", pc: dnsCount },
    { name: "Other", pc: otherCount },
  ];

  return (
    <div className="relative flex flex-col py-5 items-center justify-center border  bg-gray-100 rounded-lg shadow-md p-4">
      <div className="flex items-center mb-2">
        <h2 className="text-xl text-[#343a40] uppercase font-light tracking-tight">
          Network Protocol Flow
        </h2>
        <div
          className="absolute left-3 bottom-3 text-gray-600 cursor-pointer"
          onMouseEnter={() => setShowInfo(true)}
          onMouseLeave={() => setShowInfo(false)}
        >
          <IoMdInformationCircleOutline size={20} />
          {showInfo && (
            <div
              className={`absolute left-[130px] transform -translate-x-1/2 mt-1 w-64 bg-white border border-gray-300 rounded-md shadow-md p-2 transition-opacity duration-300 ease-in-out 
              opacity-100 translate-y-0`}
            >
              <p className="text-sm text-gray-600">
                This chart shows the count of network flows for various network
                protocols
              </p>
            </div>
          )}
        </div>
      </div>
      <AreaChart
        width={820}
        height={300}
        data={data}
        margin={{ top: 20, right: 60, left: 20, bottom: 5 }}
      >
        <defs>
          <linearGradient id="colorUv" x1="0" y1="0" x2="0" y2="1">
            <stop offset="5%" stopColor="#03045e" stopOpacity={0.8} />
            <stop offset="95%" stopColor="#03045e" stopOpacity={0} />
          </linearGradient>
        </defs>
        <XAxis
          dataKey="name"
          stroke="#6c757d"
          tick={{ fontSize: 12 }}
          tickLine={{ stroke: "#ccc" }}
        />
        <YAxis
          stroke="#6c757d"
          tick={{ fontSize: 12 }}
          tickLine={{ stroke: "#ccc" }}
        />
        <CartesianGrid strokeDasharray="3 3" stroke="#e0e0e0" />
        <Tooltip
          contentStyle={{
            backgroundColor: "#fff",
            border: "none",
            borderRadius: "8px",
          }}
          itemStyle={{ color: "#333" }}
        />
        <Area
          type="monotone"
          dataKey="pc"
          stroke="#03045e"
          fillOpacity={1}
          fill="url(#colorUv)"
          name="Flow Count"
        />
      </AreaChart>
    </div>
  );
};

export default FlowChart;
