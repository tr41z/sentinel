import React, { useState, useEffect } from "react";
import { BrowserRouter, Route, Routes, useLocation } from "react-router-dom";
import Sidebar from "./components/sidebar/Sidebar";
import Home from "./pages/Home";
import Settings from "./pages/Settings";
import { invoke } from "@tauri-apps/api";
import PacketSettings from "./components/packet/PacketSettings";
import PacketInspector from "./pages/PacketInspector";

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
            <Route path="/inspector/view" element={<PacketInspector flows={flows}/>} />
            <Route path="/inspector/config" element={<PacketSettings />} />
          </Routes>
        </div>
      </div>
    </BrowserRouter>
  );
};

export default App;
