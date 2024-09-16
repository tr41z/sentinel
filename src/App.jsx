import React, { useState } from "react";
import "./styles/App.css";
import Homepage from "./components/homepage/Homepage";
import Navbar from "./components/navbar/Navbar";

const App = () => {
  return (
    <div className="App">
      <Navbar />
      <Homepage />
    </div>
  );
};

export default App;
