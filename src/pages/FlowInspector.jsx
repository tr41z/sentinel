import React, { useState } from "react";
import InspectorElement from "../components/flow/InspectorElement";
import FlowDetails from "../components/flow/FlowDetails";

const FlowInspector = ({ flows }) => {
  const [isOpen, setIsOpen] = useState(false);
  const [selectedFlow, setSelectedFlow] = useState([]);
  const [currentPage, setCurrentPage] = useState(1);
  const [flowsPerPage, setFlowsPerPage] = useState(30); // Set flows per page to 30

  // Sample function to handle click event for now
  const handleFlowClick = (flow) => {
    setIsOpen(true);
    setSelectedFlow(flow);
  };

  // Logic for pagination
  const indexOfLastFlow = currentPage * flowsPerPage;
  const indexOfFirstFlow = indexOfLastFlow - flowsPerPage;
  const currentFlows = flows.slice(indexOfFirstFlow, indexOfLastFlow);

  // Handle page changes
  const paginate = (pageNumber) => setCurrentPage(pageNumber);

  // Calculate total pages based on 30 flows per page
  const totalPages = Math.ceil(flows.length / flowsPerPage);

  // Calculate the page numbers to display (limit to 3 pages)
  const paginateNumbers = () => {
    const left = currentPage - 1 > 0 ? currentPage - 1 : 1;
    const right = currentPage + 1 <= totalPages ? currentPage + 1 : totalPages;

    // Ensure that 3 pages are shown, and adjust if close to start or end
    const pages = [];
    if (currentPage > 1) pages.push(left); // Add previous page if not the first page
    pages.push(currentPage); // Add current page
    if (currentPage < totalPages) pages.push(right); // Add next page if not the last page

    return pages;
  };

  return (
    <div className="p-4">
      <div className="grid grid-cols-4 font-bold mb-2 border-b-2 border-[#03045e]">
        <div className="flex-1">Source IP</div>
        <div className="flex-1">Destination IP</div>
        <div className="flex-1">Protocol</div>
        <div className="flex-1">Total Bytes</div>
      </div>

      {currentFlows.map((flow) => (
        <button
          key={flow.id}
          className="w-full text-left flex items-center hover:bg-gray-200 p-2 transition duration-200"
          onClick={() => handleFlowClick(flow)}
        >
          <div className="grid grid-cols-4 gap-4 w-full">
            <InspectorElement item={flow.src_ip} />
            <InspectorElement item={flow.dst_ip} />
            <InspectorElement item={flow.protocol} />
            <InspectorElement item={flow.total_bytes} />
          </div>
        </button>
      ))}

      {selectedFlow && <FlowDetails flow={selectedFlow} isOpened={isOpen} />}

      {/* Pagination Controls */}
      <div className="flex justify-center mt-4">
        <button
          onClick={() => paginate(currentPage - 1)}
          disabled={currentPage === 1}
          className="px-4 py-2 bg-gray-200 rounded-l-md disabled:opacity-50"
        >
          Prev
        </button>

        {paginateNumbers().map((pageNumber) => (
          <button
            key={pageNumber}
            onClick={() => paginate(pageNumber)}
            className={`px-4 py-2 ${
              currentPage === pageNumber
                ? "bg-[#03045e] text-white"
                : "bg-gray-200"
            } mx-1 rounded-md`}
          >
            {pageNumber}
          </button>
        ))}

        <button
          onClick={() => paginate(currentPage + 1)}
          disabled={currentPage === totalPages}
          className="px-4 py-2 bg-gray-200 rounded-r-md disabled:opacity-50"
        >
          Next
        </button>
      </div>
    </div>
  );
};

export default FlowInspector;
