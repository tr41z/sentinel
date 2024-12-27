import React from "react";

const FlowDetails = ({ isOpened, flow }) => {
  return (
    isOpened && (
      <div className="border border-[#03045e] rounded-lg absolute top-5 left-[500px]">
        <div>
          {flow.src_ip} -- {flow.dst_ip}
        </div>
      </div>
    )
  );
};

export default FlowDetails;
