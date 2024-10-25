import React, { useState } from "react";
import Homepage from "./components/homepage/Homepage";
import Navbar from "./components/navbar/Navbar";

const App = () => {
  return (
    <div>
      <Navbar />
      <Homepage />
    </div>
  );
};

export default App;
