import React from "react";
import { BrowserRouter, Route, Routes } from "react-router-dom";
import Sidebar from "./components/sidebar/Sidebar";
import Home from "./pages/Home";
import Settings from "./pages/Settings";

const App = () => {
  return (
    <BrowserRouter>
      <div className="flex h-screen overflow-hidden">
        <Sidebar className="w-64 bg-gray-800 text-white" />{" "}
        {/* Sidebar that appears on all pages */}
        <div className="flex-1 p-6">
          <Routes>
            <Route path="/" element={<Home />} />
            <Route path="/settings" element={<Settings/>}/>
          </Routes>
        </div>
      </div>
    </BrowserRouter>
  );
};

export default App;
