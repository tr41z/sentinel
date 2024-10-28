import React from 'react'

const FlowDetails = ({ isOpened, flow }) => {
  return (
    isOpened && (
      <div className="border border-[#560bad] rounded-lg absolute top-5 left-[500px]">
        <div>
          {flow.src_ip} -- {flow.src_port}
        </div>
      </div>
    )
  );
}

export default FlowDetails
