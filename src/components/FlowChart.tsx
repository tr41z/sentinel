import { XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, AreaChart, Area, BarChart, Bar, Legend } from 'recharts';
import { Flow } from '../utils/props';

interface FlowListProps {
  flows: Flow[];
}

const FlowChart = ({ flows }: FlowListProps) => {

const data = [
  { name: 'TCP', uv: flows.filter((flow) => flow.protocol === 6).length },
  { name: 'UDP', uv: flows.filter((flow) => flow.protocol === 17).length },
];

  return (
    <div className="w-full h-96">
      <ResponsiveContainer>
        <BarChart
          width={500}
          height={300}
          data={data}
          margin={{
            top: 20,
            right: 30,
            left: 20,
            bottom: 5,
          }}
        >
          <CartesianGrid strokeDasharray="3 3" />
          <XAxis dataKey="name" />
          <YAxis />
          <Tooltip />
          <Legend />
          <Bar dataKey="uv" stackId="a" fill="#82ca9d" />
        </BarChart>
      </ResponsiveContainer>
    </div>
  );
};

export default FlowChart;
