import React from "react";
import InspectorElement from "../components/packet/InspectorElement";

const PacketInspector = ({ flows }) => {
  return (
    <div className="p-4">
      {/* Display labels only once at the top */}
      <div className="grid grid-cols-4 font-bold mb-2 border-b-2 border-[#560bad]">
        <div className="flex-1">Source IP</div>
        <div className="flex-1">Source Port</div>
        <div className="flex-1">Destination IP</div>
        <div className="flex-1">Destination Port</div>
      </div>

      {/* Display each flow item in a row below the labels */}
      {flows.map((flow) => (
        <button
          key={flow.id}
          className="w-full text-left flex items-center hover:bg-gray-200 p-2 transition duration-200"
          onClick={() => handleFlowClick(flow)}
        >
          <div className="grid grid-cols-4 gap-4 w-full">
            <InspectorElement item={flow.src_ip} />
            <InspectorElement item={flow.src_port} />
            <InspectorElement item={flow.dst_ip} />
            <InspectorElement item={flow.dst_port} />
          </div>
        </button>
      ))}
    </div>
  );
};

// Sample function to handle click event
const handleFlowClick = (flow) => {
  console.log("Flow clicked:", flow);
};

export default PacketInspector;
