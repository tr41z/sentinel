import { invoke } from "@tauri-apps/api"

export const startPacketCapture = async () => {
    try {
        await invoke("start_sniffer");
        console.log("Packet capturing started!");
    } catch (error) {
        console.error("Failed to start packet capture: ", error)
    }
}