import React from "react";

const InspectorElement = ({ flows }) => {

  return (
    <div className="flex flex-col">
      <div className="flex flex-row">
        <span>Source IP</span>
        <span>Source Port</span>
        <span>Destination IP</span>
        <span>Destination Port</span>
      </div>
      {flows.map((flow, index) => (
        <div key={index} className="flex flex-row">
          <span>{flow.src_ip}</span>
          <span className="ml-10">{flow.src_port}</span>
          <span>{flow.dst_ip}</span>
          <span>{flow.dst_port}</span>
        </div>
      ))}
    </div>
  );
};

export default InspectorElement;
