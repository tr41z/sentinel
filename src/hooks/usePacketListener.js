import { listen } from "@tauri-apps/api/event";
import { useEffect, useState } from "react";
import { PACKET_TYPES } from "../utils/constants";

// helper function to match flows based on their unique keys
const isSameFlow = (flow1, flow2) => {
  return (
    flow1.src_ip === flow2.src_ip &&
    flow1.dst_ip === flow2.dst_ip &&
    flow1.src_port === flow2.src_port &&
    flow1.dst_port === flow2.dst_port &&
    flow1.protocol === flow2.protocol
  );
};

const usePacketListener = () => {
  const [packets, setPackets] = useState([]);

  useEffect(() => {
    const unlistenFlow = listen(PACKET_TYPES.FLOW, (event) => {
      setPackets((prevPackets) => {
        const updatedFlow = event.payload;

        // check if the flow already exists
        const existingIndex = prevPackets.findIndex((flow) =>
          isSameFlow(flow, updatedFlow)
        );

        if (existingIndex !== -1) {
          // ipdate the existing flow with new data
          const updatedPackets = [...prevPackets];
          updatedPackets[existingIndex] = updatedFlow;
          return updatedPackets;
        } else {
          // add the new flow if it doesn't exist
          return [...prevPackets, updatedFlow];
        }
      });
    });

    return () => {
      unlistenFlow.then((unsub) => unsub());
    };
  }, []);

  return packets;
};

export default usePacketListener;
