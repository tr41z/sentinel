import React, { useMemo } from "react";
import FlowChart from "./FlowChart";

const Homepage = ({ flows }) => {
  const { httpCount, dnsCount, tcpCount, udpCount, otherCount } =
    useMemo(() => {
      let http = 0,
        dns = 0,
        tcp = 0,
        udp = 0,
        other = 0;

      flows.forEach((flow) => {
        const srcPort = flow.src_port.toString();
        const protocol = flow.protocol.toString();

        if (srcPort === "443") http++;
        else if (srcPort === "53") dns++;
        else other++;

        if (protocol === "6") tcp++;
        else if (protocol === "17") udp++;
      });

      return {
        httpCount: http,
        dnsCount: dns,
        tcpCount: tcp,
        udpCount: udp,
        otherCount: other,
      };
    }, [flows]);

  return (
    <div>
      <FlowChart
        httpCount={httpCount}
        dnsCount={dnsCount}
        tcpCount={tcpCount}
        udpCount={udpCount}
        otherCount={otherCount}
      />
    </div>
  );
};

export default Homepage;
