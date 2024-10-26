import React, { useEffect, useState } from "react";
import { invoke } from "@tauri-apps/api/tauri";
import FlowChart from "./FlowChart";

const Homepage = () => {
  const [flows, setFlows] = useState([]);
  const [error, setError] = useState(null);

  const [httpCount, setHttpCount] = useState(0);
  const [dnsCount, setDnsCount] = useState(0);
  const [tcpCount, setTcpCount] = useState(0);
  const [udpCount, setUdpCount] = useState(0);

  async function loadFlows() {
    try {
      const fetchedFlows = await invoke("fetch_flows");
      setFlows(fetchedFlows);

      let newHttpCount = 0;
      let newDnsCount = 0;
      let newTcpCount = 0;
      let newUdpCount = 0;

      fetchedFlows.forEach((flow) => {
        if (flow.src_port == "443") {
          newHttpCount += 1;
        } else if (flow.src_port == "53") {
          newDnsCount += 1;
        }

        if (flow.protocol == '6') {
          newTcpCount += 1;
        } else if (flow.protocol == '17') {
          newUdpCount += 1;
        }
      });

      // Update the state with the new counts
      setHttpCount(newHttpCount);
      setDnsCount(newDnsCount);
      setTcpCount(newTcpCount);
      setUdpCount(newUdpCount);

    } catch (err) {
      console.error("Error fetching flows:", err);
      setError("Failed to load flows");
    }
  }

  useEffect(() => {
    const interval = setInterval(() => {
      loadFlows();
    }, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <FlowChart
        httpCount={httpCount}
        dnsCount={dnsCount}
        // icmpCount={icmpCount}
        tcpCount={tcpCount}
        udpCount={udpCount}
      />
    </div>
  );
};

export default Homepage;
