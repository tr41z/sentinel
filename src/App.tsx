import { useEffect, useState } from 'react';
import './App.css'
import FlowList from './components/FlowList'
import { Flow } from './utils/props';
import FlowChart from './components/FlowChart';

function App() {  
    const [flows, setFlows] = useState<Flow[]>([]);
    
    const fetchFlows = async () => {
      const res = await fetch("http://localhost:8080/api/v1/flows");
      
      if (res.ok) {
          const data = await res.json();
          setFlows(data);
      } else {
          console.error("There was an error while fetching flows!");
      }
    }

    useEffect(() => {
      const interval = setInterval(() => {
        fetchFlows();
      }, 1000);

      return () => clearInterval(interval);
    }, [])

    return (
      <div>
        <FlowChart flows={flows}/>
        <FlowList flows={flows}/>
      </div>
    )
}

export default App
