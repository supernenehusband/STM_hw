import asyncio
from bleak import BleakScanner


async def scan():
    print("🔍 Scanning for BLE devices (5 seconds)...")
    devices = await BleakScanner.discover(timeout=5)

    if not devices:
        print("❌ No BLE devices found.")
        return

    print("✅ Found devices:")
    for i, device in enumerate(devices):
        if not device.name:
            continue
        print(
            f"{i+1:2d}. Name: {device.name or '(no name)'}\n     Address: {device.address}\n")

if __name__ == "__main__":
    asyncio.run(scan())
