import React, { useState } from "react";
import { invoke } from "@tauri-apps/api/tauri";

const Homepage = () => {
  const [flows, setFlows] = useState([]);
  const [error, setError] = useState(null);

  async function loadFlows() {
    try {
      const fetchedFlows = await invoke("fetch_flows");
      setFlows(fetchedFlows); // store the fetched flows in state
    } catch (err) {
      console.error("Error fetching flows:", err);
      setError("Failed to load flows");
    }
  }

  return (
    <div>
      <button onClick={loadFlows} className="text-black bg-white rounded-lg">Load Flows</button>
      <p className="text-white">Flows: {flows.length}</p>
      {error && <p>{error}</p>} {/* Display error message if any */}
      <ul>
        {flows.map((flow, index) => (
          <li key={index} className="text-red-200">
            {flow.src_ip} → {flow.dst_ip} ({flow.protocol})
          </li>
        ))}
      </ul>
    </div>
  );
};

export default Homepage;
