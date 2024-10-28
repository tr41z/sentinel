import React, { useState, useEffect } from "react";
import { BrowserRouter, Route, Routes } from "react-router-dom";
import Sidebar from "./components/sidebar/Sidebar";
import Home from "./pages/Home";
import Settings from "./pages/Settings";
import { invoke } from "@tauri-apps/api";
import FlowSettings from "./components/flow/FlowSettings";
import FlowInspector from "./pages/FlowInspector";

const App = () => {
  const [flows, setFlows] = useState([]);

  async function loadFlows() {
    try {
      const fetchedFlows = await invoke("fetch_flows");
      setFlows(fetchedFlows);
    } catch (err) {
      console.error("Error fetching flows:", err);
    }
  }

  useEffect(() => {
    const interval = setInterval(loadFlows, 2000);
    return () => clearInterval(interval);
  }, []);

  return (
    <BrowserRouter>
      <div className="flex h-screen overflow-hidden">
        <Sidebar className="w-64 bg-gray-800 text-white" />
        <div className="flex-1 p-6 overflow-scroll">
          <Routes>
            <Route path="/" element={<Home flows={flows} />} />
            <Route path="/settings" element={<Settings />} />
            <Route
              path="/inspector/view"
              element={<FlowInspector flows={flows} />}
            />
            <Route path="/inspector/config" element={<FlowSettings />} />
          </Routes>
        </div>
      </div>
    </BrowserRouter>
  );
};

export default App;
