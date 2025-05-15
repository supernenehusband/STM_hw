import asyncio
from bleak import BleakScanner, BleakClient
import queue
import threading

TARGET_NAME = "BlueNRG"
UUID_CMD = "00000100-0000-0001-11e1-ac360002a5d5"

def start_ble_listener(cmd_queue):
    def run_ble_loop():
        asyncio.run(ble_loop(cmd_queue))

    threading.Thread(target=run_ble_loop, daemon=True).start()

async def ble_loop(cmd_queue):
    print("🔍 Scanning for BLE devices...")
    devices = await BleakScanner.discover(timeout=5)
    target = None
    for d in devices:
        if d.name == TARGET_NAME:
            print(f"✅ Found {d.name} ({d.address})")
            target = d
            break

    if not target:
        print(f"❌ Device '{TARGET_NAME}' not found.")
        return

    async with BleakClient(target.address) as client:
        if not client.is_connected:
            print("❌ Failed to connect.")
            return
        print("✅ Connected!")

        def handle_notify(_, data):
            try:
                cmd = data.decode("utf-8").strip().lower()
                if cmd in ("up", "down", "left", "right"):
                    print(f"[BLE CMD] {cmd}")
                    cmd_queue.put(cmd)
            except Exception as e:
                print(f"[ERROR] Bad data: {e}")

        await client.start_notify(UUID_CMD, handle_notify)
        print("📥 Listening for commands...")
        while True:
            await asyncio.sleep(1)