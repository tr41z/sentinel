import React, { useState } from "react";
import InspectorElement from "../components/flow/InspectorElement";
import FlowDetails from "../components/flow/FlowDetails";

const FlowInspector = ({ flows }) => {
  const [isOpen, setIsOpen] = useState(false);
  const [selectedFlow, setSelectedFlow] = useState([]);
  // NOTE: Need pagination
  const [currentPage, setCurrentPage] = useState(1);
  const [flowsPerPage, setflowsPerPage] = useState(100);

  // Sample function to handle click event for now
  const handleFlowClick = (flow) => {
    setIsOpen(true);
    setSelectedFlow(flow);
  };

  return (
    <div className="p-4">
      <div className="grid grid-cols-4 font-bold mb-2 border-b-2 border-[#560bad]">
        <div className="flex-1">Source IP</div>
        <div className="flex-1">Source Port</div>
        <div className="flex-1">Destination IP</div>
        <div className="flex-1">Destination Port</div>
      </div>

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

      {selectedFlow && <FlowDetails flow={selectedFlow} isOpened={isOpen}/>}
    </div>
  );
};

export default FlowInspector;
