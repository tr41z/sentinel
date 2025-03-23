import { useState } from "react";
import Header from "../components/Header";
import { motion } from "framer-motion";

const InspectorPage = ({ flows }: { flows: { id: string; total_bytes: number; duration: number; rate: number; src_ip: string; dst_ip: string }[] }) => {
  const [currentPage, setCurrentPage] = useState(1);
  const [selectedFlow, setSelectedFlow] = useState<{ id: string; total_bytes: number; duration: number; rate: number; src_ip: string; dst_ip: string } | null>(null);
  const [isModalOpen, setIsModalOpen] = useState(false);

  const itemsPerPage = 20;
  const totalPages = Math.ceil(flows.length / itemsPerPage);

  const handlePageChange = (page: number) => {
    setCurrentPage(page);
  };

  const handleFlowClick = (flow: { id: string; total_bytes: number; duration: number; rate: number; src_ip: string; dst_ip: string }) => {
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
      <div className="flex-1 overflow-auto p-4">
        <table className="table-auto w-full border-collapse border border-gray-700">
          <thead>
            <tr className="bg-gray-950 text-left">
              <th className="border border-gray-700 px-4 py-2">Source IP</th>
              <th className="border border-gray-700 px-4 py-2">Destination IP</th>
              <th className="border border-gray-700 px-4 py-2">Total Bytes</th>
              <th className="border border-gray-700 px-4 py-2">Duration</th>
            </tr>
          </thead>
          <tbody>
            {paginatedFlows.map((flow, index) => (
              <motion.tr
                key={index}
                className="hover:bg-gray-900 cursor-pointer duration-150"
                onClick={() => handleFlowClick(flow)}
                initial={{ opacity: 0 }}
                animate={{ opacity: 1 }}
                transition={{ duration: 0.3, ease: "easeInOut" }}
              >
                <td className="border border-gray-700 px-4 py-2">{flow.src_ip}</td>
                <td className="border border-gray-700 px-4 py-2">{flow.dst_ip}</td>
                <td className="border border-gray-700 px-4 py-2">{flow.total_bytes}</td>
                <td className="border border-gray-700 px-4 py-2">{flow.duration}s</td>
              </motion.tr>
            ))}
          </tbody>
        </table>
        <div className="mt-4 flex justify-between items-center">
          <motion.button
            disabled={currentPage === 1}
            onClick={() => handlePageChange(currentPage - 1)}
            className="px-4 py-2 bg-gray-700 rounded disabled:opacity-50"
            whileHover={{ scale: 1.05 }}
            transition={{ duration: 0.2 }}
          >
            Previous
          </motion.button>
          <span>
            Page {currentPage} of {totalPages}
          </span>
          <motion.button
            disabled={currentPage === totalPages}
            onClick={() => handlePageChange(currentPage + 1)}
            className="px-4 py-2 bg-gray-700 rounded disabled:opacity-50"
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
            className="bg-black p-10 rounded-lg shadow-lg w-1/2"
            initial={{ scale: 0.95 }}
            animate={{ scale: 1 }}
            transition={{ duration: 0.3 }}
          >
            <h2 className="text-xl font-bold mb-4">Flow Details</h2>
            <div className="space-y-4 mb-4">
              <p><strong>ID:</strong> {selectedFlow.id}</p>
              <p><strong>Source IP:</strong> {selectedFlow.src_ip}</p>
              <p><strong>Destination IP:</strong> {selectedFlow.dst_ip}</p>
              <p><strong>Total Bytes:</strong> {selectedFlow.total_bytes}</p>
              <p><strong>Duration:</strong> {selectedFlow.duration}s</p>
              <p><strong>Rate:</strong> {selectedFlow.rate}</p>
            </div>
            <motion.button
              onClick={closeModal}
              className="mt-4 px-4 py-2 bg-red-600 rounded"
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
