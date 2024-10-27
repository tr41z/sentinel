import React from "react";
import InspectorElement from "../components/packet/InspectorElement";

const PacketInspector = ({ flows }) => {
  return (
    <div>
      {/* Display labels only once at the top */}
      <div className="flex flex-row font-bold">
        <div className="flex-1">Source IP</div>
        <div className="flex-1">Source Port</div>
        <div className="flex-1">Destination IP</div>
        <div className="flex-1">Destination Port</div>
      </div>

      {/* Display each flow item in a row below the labels */}
      {flows.map((flow) => (
        <div className="flex flex-row" key={flow.id}>
          {" "}
          <InspectorElement item={flow.src_ip} />
          <InspectorElement item={flow.src_port} />
          <InspectorElement item={flow.dst_ip} />
          <InspectorElement item={flow.dst_port} />
        </div>
      ))}
    </div>
  );
};

export default PacketInspector;
