import { useState } from "react";
import Header from "../components/Header";
import { motion } from "framer-motion";
import { MapPin } from "lucide-react";

const InspectorPage = ({
  flows,
}: {
  flows: {
    id: string;
    total_bytes: number;
    duration: number;
    rate: number;
    src_ip: string;
    dst_ip: string;
  }[];
}) => {
  const [currentPage, setCurrentPage] = useState(1);
  const [selectedFlow, setSelectedFlow] = useState<{
    id: string;
    total_bytes: number;
    duration: number;
    rate: number;
    src_ip: string;
    dst_ip: string;
  } | null>(null);
  const [isModalOpen, setIsModalOpen] = useState(false);

  const itemsPerPage = 20;
  const totalPages = Math.ceil(flows.length / itemsPerPage);

  const handlePageChange = (page: number) => {
    setCurrentPage(page);
  };

  const handleFlowClick = (flow: {
    id: string;
    total_bytes: number;
    duration: number;
    rate: number;
    src_ip: string;
    dst_ip: string;
  }) => {
    setSelectedFlow(flow);
    setIsModalOpen(true);
  };

  const closeModal = () => {
    setIsModalOpen(false);
    setSelectedFlow(null);
  };

  const paginatedFlows = flows.slice(
    (currentPage - 1) * itemsPerPage,
    currentPage * itemsPerPage
  );

  return (
    <div className="flex-1 flex flex-col relative z-10">
      <Header title="Inspector" />
      <div className="flex-1 overflow-auto p-6 space-y-4">
        {paginatedFlows.map((flow, index) => (
          <motion.div
            key={index}
            className="flex items-center p-4 bg-[#121212] rounded-lg shadow-md hover:shadow-lg transition-shadow cursor-pointer"
            onClick={() => handleFlowClick(flow)}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1 }}
            transition={{ duration: 0.3, ease: "easeInOut" }}
          >
            <p className="text-sm font-medium text-gray-300 flex-1">
              <strong>Source:</strong> {flow.src_ip}
            </p>
            <p className="text-sm font-medium text-gray-300 flex-1">
              <strong>Destination:</strong> {flow.dst_ip}
            </p>
            <p className="text-sm font-medium text-gray-300 flex-1">
              <strong>Bytes:</strong> {flow.total_bytes}
            </p>
            <p className="text-sm font-medium text-gray-300 flex-1">
              <strong>Duration:</strong> {flow.duration}s
            </p>
          </motion.div>
        ))}
        <div className="mt-6 flex justify-between items-center">
          <motion.button
            disabled={currentPage === 1}
            onClick={() => handlePageChange(currentPage - 1)}
            className="px-4 py-2 bg-gray-700 text-gray-200 rounded-lg disabled:opacity-50 hover:bg-gray-600 transition-transform"
            whileHover={{ scale: 1.05 }}
            transition={{ duration: 0.2 }}
          >
            Previous
          </motion.button>
          <span className="text-gray-400">
            Page {currentPage} of {totalPages}
          </span>
          <motion.button
            disabled={currentPage === totalPages}
            onClick={() => handlePageChange(currentPage + 1)}
            className="px-4 py-2 bg-gray-700 text-gray-200 rounded-lg disabled:opacity-50 hover:bg-gray-600 transition-transform"
            whileHover={{ scale: 1.05 }}
            transition={{ duration: 0.2 }}
          >
            Next
          </motion.button>
        </div>
      </div>

      {isModalOpen && selectedFlow && (
        <motion.div
          className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-20"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ duration: 0.3 }}
        >
          <motion.div
            className="bg-[#1A1A1A] p-6 rounded-lg shadow-lg border border-gray-700 w-2/3 flex"
            initial={{ scale: 0.95 }}
            animate={{ scale: 1 }}
            transition={{ duration: 0.3 }}
          >
            <div className="flex-1 text-gray-300 space-y-3 text-left">
              <h2 className="text-xl font-semibold text-green-500 mb-4">
                Flow Details
              </h2>
              <p>
                <strong>ID:</strong> {selectedFlow.id}
              </p>
              <p>
                <strong>Source IP:</strong> {selectedFlow.src_ip}
              </p>
              <p>
                <strong>Destination IP:</strong> {selectedFlow.dst_ip}
              </p>
              <p>
                <strong>Total Bytes:</strong> {selectedFlow.total_bytes}
              </p>
              <p>
                <strong>Duration:</strong> {selectedFlow.duration}s
              </p>
              <p>
                <strong>Rate:</strong> {selectedFlow.rate}
              </p>
            </div>
            <div className="flex-1 flex items-center justify-center">
              <div className="bg-gray-800 rounded-lg p-4 flex flex-col items-center justify-center">
                <MapPin size={48} className="text-gray-400 mb-2" />
                <p className="text-sm text-gray-400">Location Map</p>
              </div>
            </div>
            <motion.button
              onClick={closeModal}
              className="absolute top-4 right-4 px-4 py-2 bg-red-600 text-gray-200 rounded-lg hover:bg-red-500 transition-transform"
              whileHover={{ scale: 1.05 }}
              transition={{ duration: 0.2 }}
            >
              Close
            </motion.button>
          </motion.div>
        </motion.div>
      )}
    </div>
  );
};

export default InspectorPage;
